#include <Profiler/Profiler.h>
#include <Profiler/Profiler_Master.h>
#include <StringUtilities.h>
#include <MonadicOptional.h>

struct Utilities::Profiler::ProfileEntry {
		ProfileEntry( const char* str ) : name( str ), file( "" ), timesCalled( 0 ), parent( nullptr ), hash( 0 ), timeSpent( 0 )
		{};
		ProfileEntry( const char* str, HashValue hash, const char* file, std::shared_ptr<ProfileEntry> parent ) : name( str ), hash( hash ), file( file ), timesCalled( 0 ), parent( parent ), timeSpent( 0 )
		{};
		std::string name;
		std::string file;
		HashValue hash;
		size_t timesCalled;
		std::shared_ptr<ProfileEntry> parent;
		std::shared_ptr<ProfileEntry> child;
		std::shared_ptr<ProfileEntry> nextChild;
		std::chrono::nanoseconds timeSpent;
		std::chrono::high_resolution_clock::time_point start;
		Utilities::optional<std::shared_ptr<ProfileEntry>> findEntry( HashValue hash )noexcept
		{
			std::shared_ptr<ProfileEntry> walker;
			walker = child;
			while ( walker && walker->hash != hash )
				walker = walker->nextChild;
			if ( walker )
				return walker;
			return std::nullopt;
		}
	};

Utilities::Profiler::Profiler() : root(std::make_shared<ProfileEntry>( "Root" )), current(root)
{

}

DECLSPEC_PROFILER std::shared_ptr<Utilities::Profiler> Utilities::Profiler::get()
{
	thread_local static std::shared_ptr<Profiler> profiler;
	if ( !profiler )
	{
		profiler = Profiler_Master::get()->get_profiler( std::this_thread::get_id() );
	}
	return profiler;
}

void Utilities::Profiler::start(HashValue hash, const char* str, const char* file)noexcept
{
	current = *current->findEntry(hash).or_else_this([&]
		{
			auto newChild = std::make_shared<ProfileEntry>(str, hash, file, current);
			addChild(current, newChild);
			return newChild;
		});

	current->timesCalled++;
	current->start = std::chrono::high_resolution_clock::now();
}

void Utilities::Profiler::stop() noexcept
{
	auto stop = std::chrono::high_resolution_clock::now();
	current->timeSpent += stop - current->start;
	current = current->parent;
}

const std::string Utilities::Profiler::to_str(int tabDepth)const noexcept
{
	std::stringstream ss;
	auto walker = root->child;
	while (walker)
	{
		to_str(ss, walker, tabDepth);
		walker = walker->nextChild;
	}
	return ss.str();
}


const std::string Utilities::Profiler::generate_tree()const noexcept
{
	std::stringstream ss;
	auto walker = root->child;
	while ( walker )
	{
		generate_tree( ss, walker );
		walker = walker->nextChild;
	}
	return ss.str();
}

void Utilities::Profiler::addChild( std::shared_ptr<ProfileEntry> parent, std::shared_ptr<ProfileEntry> child ) noexcept
{
	if ( !parent->nextChild )
		parent->nextChild = child;
	else
		addChild( parent->nextChild, child );
}

void Utilities::Profiler::addChild( std::shared_ptr<ProfileEntry> child ) noexcept
{
	if ( !current->child )
		current->child = child;
	else
		addChild( current->child, child );
}
void Utilities::Profiler::generate_tree( std::stringstream& ss, std::shared_ptr<ProfileEntry> node )const noexcept
{
	ss << "\"" << node << "\"" << "[\n shape = none" << std::endl;
	ss << "label = <<table border=\"0\" cellspacing = \"0\">" << std::endl;
	double div = 0.0;
	if ( node->parent && node->parent->hash != 0 )
		div = ( (double)node->timeSpent.count() / node->parent->timeSpent.count() );
	ss << "<tr><td port=\"port1\" border=\"1\" bgcolor = \"#"
		<< String::charToHex( uint8_t( 150 * div ) ) << String::charToHex( 50 ) << String::charToHex( uint8_t( 50 * ( 1.0 - div ) ) ) << "\">" << std::endl;
	ss << "<font color=\"white\">" << node->file << ": " << String::replaceAll( String::replaceAll( node->name, "<", "\\<" ), ">", "\\>" ) << "</font></td></tr>\n" << std::endl;

	ss << "<tr><td border=\"1\">" << "Times Called: " << node->timesCalled << "</td></tr>" << std::endl;
	ss << "<tr><td border=\"1\">" << "Time Spent(IC): " << std::chrono::duration_cast<timeunit_chrono>( node->timeSpent ).count() << " " << timeunit_str;
	if ( node->parent && node->parent->hash != 0 )
		ss << " " << div * 100 << " % of parents.</td></tr>" << std::endl;
	else
		ss << "</td></tr>" << std::endl;

	ss << "<tr><td border=\"1\">" << "Time Spent(avg): " << std::chrono::duration_cast<timeunit_chrono>( node->timeSpent ).count() / double( node->timesCalled ) << " " << timeunit_str << "</td></tr>" << std::endl;

	if ( node->child )
	{
		auto walker = node->child;
		auto timeEC = node->timeSpent;
		while ( walker )
		{
			timeEC -= walker->timeSpent;
			walker = walker->nextChild;
		}
		ss << "<tr><td border=\"1\">" << "Time Spent(EC): " << std::chrono::duration_cast<timeunit_chrono>( timeEC ).count() << " " << timeunit_str << "</td></tr>" << std::endl;
	}

	ss << "</table>>]" << std::endl;

	auto walker = node->child;
	while ( walker )
	{
		generate_tree( ss, walker);
		ss << "\"" << node << "\":port1 -> \"" << walker << "\":port1" << std::endl;
		walker = walker->nextChild;
	}

	ss << "" << std::endl;
}


void Utilities::Profiler::to_str( std::stringstream& ss, std::shared_ptr<ProfileEntry> entry, int tabDepth )const noexcept
{
	ss << String::tabs( tabDepth ) << entry->name << ": Times called: " << entry->timesCalled
		<< " Time spent: " << std::chrono::duration_cast<std::chrono::nanoseconds>( entry->timeSpent ).count()
		<< " Average: " << std::chrono::duration_cast<std::chrono::nanoseconds>( entry->timeSpent ).count() / entry->timesCalled << std::endl;
	auto walker = entry->child;
	while ( walker )
	{
		to_str( ss, walker, tabDepth + 1 );
		walker = walker->nextChild;
	}

}