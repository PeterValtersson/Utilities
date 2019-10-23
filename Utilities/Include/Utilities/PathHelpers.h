#ifndef _UTILZ_PATH_PARSING_H_
#define _UTILZ_PATH_PARSING_H_

#include <string>
#include <Windows.h>
#include <vector>

namespace Utilities
{
	namespace Path
	{
		const std::string get_extension( std::string_view path )
		{
			const size_t period_idx = path.find_last_of( '.' );
			if ( std::string::npos != period_idx )
			{
				return std::string( path.substr( period_idx + 1 ) );
			}
			return std::string( path );
		}
		const std::string remove_extension( std::string_view path )
		{
			const size_t period_idx = path.find_last_of( '.' );
			if ( std::string::npos != period_idx )
			{
				return std::string( path.substr( 0, period_idx ) );
			}
			return std::string( path );
		}

		const std::string get_path( std::string_view path )
		{
			const size_t last_slash_idx = path.find_last_of( '\\/' );
			if ( std::string::npos != last_slash_idx )
			{
				return std::string( path.substr( 0, last_slash_idx ) );
			}
			return std::string( path );
		}

		const std::string get_filename( std::string_view path )
		{
			const size_t last_slash_idx = path.find_last_of( "\\/" );
			if ( std::string::npos != last_slash_idx )
			{
				return std::string( path.substr( last_slash_idx + 1 ) );
			}
			return std::string( path );
		}

		const std::string remove_root( std::string_view path )
		{
			const size_t first_slash_idx = path.find_first_of( '\\/' );
			if ( std::string::npos != first_slash_idx )
			{
				return std::string( path.substr( first_slash_idx + 1 ) );
			}
			return std::string( path );
		}
	}


	struct File_Info{
		std::string name;
		std::string fullPath;
	};

	template<class _Container = std::vector<File_Info>>
	void get_all_file_names_within_folder( const std::string folder_path, _Container& files )
	{
		std::string search_path = folder_path + "/*";
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile( search_path.c_str(), &fd );
		if ( hFind != INVALID_HANDLE_VALUE )
		{
			do
			{
				if ( fd.cFileName )
				{
					std::string name = fd.cFileName;
					if ( !(name == "." || name == "..") )
					{
						if ( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
							files.insert( files.end(), { name, folder_path + "/" + name } );
						else
							get_all_file_names_within_folder( folder_path + "/" + name, files );
					}
				}

			} while ( ::FindNextFile( hFind, &fd ) );
			::FindClose( hFind );
		}
	}

	template<class _Container = std::vector<File_Info>>
	inline const _Container get_all_file_names_within_folder( const std::string folder_path )
	{
		_Container files;
		get_all_file_names_within_folder( folder_path, files );
		return files;
	}

	
}

#endif _UTILZ_PATH_PARSING_H_