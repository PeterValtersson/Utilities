

#include "ObjParser.h"
#include <sstream>

using namespace ObjParser;

Interpreter::Interpreter() :
    _scanner(*this),
    _parser(_scanner, *this),
   m_location(0),
	parsed(false),
	data(nullptr)
{
	
}

ObjParser::Interpreter::~Interpreter()
{
	
	stream = nullptr;
	if (data)
		operator delete(data);
}

int Interpreter::parse(std::istream *is)
{
	stream = is;
	if (data)
		this->~Interpreter();
	parsed = false;
	_row = 0;
	data = (ArfData::ArfData*) operator new(sizeof(ArfData::ArfData));
	*data = ArfData::ArfData();
	
	Alloc(ALLOC_ALL);
	AddSubMesh("default");
	_scanner.switch_streams(is, NULL);
    m_location = 0;
	int ret = _parser.parse();

	Alloc(ALLOC_SHRINK_TO_FIT);
	parsed = true;
	return ret;
}

void Interpreter::clear()
{
    m_location = 0;
}

void ObjParser::Interpreter::Alloc(int flag)
{
	switch (flag)
	{
	case ALLOC_ALL:
	{
		data->data.PosCap = 10000;
		data->data.TexCap = 10000;
		data->data.NormCap = 10000;
		data->data.FaceCap = 10000;
		data->data.SubMeshCap = 100;
		data->data.NumNorm = 0;
		data->data.NumFace = 0;
		data->data.NumPos = 0;
		data->data.NumSubMesh = 0;
		data->data.NumTex = 0;
		data->data.allocated = 0;

		break;
	}
	case ALLOC_POSITION:
	{
		data->data.PosCap *= 2;
		break;
	}
	case ALLOC_TECCOORD:
	{
		data->data.TexCap *= 2;
		break;
	}
	case ALLOC_NORMAL:
	{
		data->data.NormCap *= 2;
		break;
	}
	case ALLOC_FACE:
	{
		data->data.FaceCap *= 2;
		break;
	}
	case ALLOC_SUB_MESH:
	{
		data->data.SubMeshCap *= 2;
		break;
	}
	case ALLOC_SHRINK_TO_FIT:
	{
		data->data.PosCap = data->data.NumPos;
		data->data.TexCap = data->data.NumTex;
		data->data.NormCap = data->data.NumNorm;
		data->data.FaceCap = data->data.NumFace;
		data->data.SubMeshCap = data->data.NumSubMesh;
		break;
	}
	default:
		break;
	}

	data->data.allocated = 
		data->data.PosCap *		sizeof(ArfData::Position) +
		data->data.TexCap *		sizeof(ArfData::TexCoord) +
		data->data.NormCap *		sizeof(ArfData::Normal) +
		data->data.FaceCap *		sizeof(ArfData::Face) +
		data->data.SubMeshCap *	sizeof(ArfData::SubMesh);
	void * newBuffer = operator new(data->data.allocated + sizeof(ArfData::ArfData));
	auto newData = (ArfData::ArfData*)newBuffer;
	newData->data = data->data;
	newData->pointers.buffer = (void*)(newData + 1);

	newData->pointers.positions = (ArfData::Position*)newData->pointers.buffer;
	newData->pointers.texCoords = (ArfData::TexCoord*)(newData->pointers.positions + newData->data.PosCap);
	newData->pointers.normals = (ArfData::Normal*)(newData->pointers.texCoords + newData->data.TexCap);
	newData->pointers.faces = (ArfData::Face*)(newData->pointers.normals + newData->data.NormCap);
	newData->pointers.subMesh = (ArfData::SubMesh*)(newData->pointers.faces + newData->data.FaceCap);

	memcpy(newData->pointers.positions, data->pointers.positions, newData->data.NumPos * sizeof(ArfData::Position));
	memcpy(newData->pointers.texCoords, data->pointers.texCoords, newData->data.NumTex * sizeof(ArfData::TexCoord));
	memcpy(newData->pointers.normals, data->pointers.normals, newData->data.NumNorm * sizeof(ArfData::Normal));
	memcpy(newData->pointers.faces, data->pointers.faces, newData->data.NumFace * sizeof(ArfData::Face));
	memcpy(newData->pointers.subMesh, data->pointers.subMesh, newData->data.NumSubMesh * sizeof(ArfData::SubMesh));

	operator delete(data);
	data = newData;
}


void ObjParser::Interpreter::AddPosition(const ArfData::Position & pos)
{
	if (data->data.NumPos >= data->data.PosCap)
	{
		// Allocate more space
		Alloc(ALLOC_POSITION);
	}

	data->pointers.positions[data->data.NumPos] = pos;
	data->data.NumPos++;
}

void ObjParser::Interpreter::AddTexCoord(const ArfData::TexCoord & tex)
{
	if (data->data.NumTex >= data->data.TexCap)
	{
		// Allocate more space
		Alloc(ALLOC_TECCOORD);
	}

	data->pointers.texCoords[data->data.NumTex] = tex;
	data->data.NumTex++;
}

void ObjParser::Interpreter::AddNormal(const ArfData::Normal & norm)
{
	if (data->data.NumNorm >= data->data.NormCap)
	{
		// Allocate more space
		Alloc(ALLOC_NORMAL);
	}

	data->pointers.normals[data->data.NumNorm] = norm;
	data->data.NumNorm++;
}

void ObjParser::Interpreter::AddFace(const ArfData::Face & face)
{
	if (data->data.NumFace >= data->data.FaceCap)
	{
		// Allocate more space->
		Alloc(ALLOC_FACE);
	}

	if (face.indexCount == 4)
	{
		std::vector<std::vector<uint32_t>> face1v;
		for (uint8_t i = 0; i < 3; i++)
		{
			std::vector<uint32_t> in;
			for (uint8_t j = 0; j < face.indices[i].indexCount; j++)
			{
				in.push_back(face.indices[i].index[j]);
			}
			face1v.push_back(in);
		}
		data->pointers.faces[data->data.NumFace] = ArfData::Face(face1v);
		if (data->data.NumSubMesh)
			data->pointers.subMesh[data->data.NumSubMesh - 1].faceCount++;
		data->data.NumFace++;

		uint32_t indices[] = { 2, 3 ,0 };
		face1v.clear();
		for (uint8_t i = 0; i < 3; i++)
		{
			std::vector<uint32_t> in;
			for (uint8_t j = 0; j < face.indices[indices[i]].indexCount; j++)
			{
				in.push_back(face.indices[indices[i]].index[j]);
			}
			face1v.push_back(in);
		}
		data->pointers.faces[data->data.NumFace] = ArfData::Face(face1v);
		if (data->data.NumSubMesh)
			data->pointers.subMesh[data->data.NumSubMesh - 1].faceCount++;
		data->data.NumFace++;
	}
	else if (face.indexCount == 2)
		throw "Fuck yo lines bitch";
	else
	{


		data->pointers.faces[data->data.NumFace] = face;
		if (data->data.NumSubMesh)
			data->pointers.subMesh[data->data.NumSubMesh - 1].faceCount++;
		data->data.NumFace++;
	}
}

void ObjParser::Interpreter::AddSubMesh(const string & name)
{
	if (data->data.NumSubMesh >= data->data.SubMeshCap)
	{
		// Allocate more space->
		Alloc(ALLOC_SUB_MESH);
	}
	int size = min((int)name.size(), SUBMESH_NAME_MAX_LENGHT - 1);
	if (data->data.NumSubMesh == 1)
	{
		if (std::string(data->pointers.subMesh[0].name) == "default")
		{
			
			memcpy(data->pointers.subMesh[0].name, name.c_str(), size);
			data->pointers.subMesh[0].name[size] = '\0';
			return;
		}
	}
	memcpy(data->pointers.subMesh[data->data.NumSubMesh].name, name.c_str(), size);
	data->pointers.subMesh[data->data.NumSubMesh].name[size] = '\0';
	data->pointers.subMesh[data->data.NumSubMesh].faceStart = data->data.NumFace;
	data->pointers.subMesh[data->data.NumSubMesh].faceCount = 0;
	data->data.NumSubMesh++;
}


std::string Interpreter::str() const 
{
    std::stringstream s;
	s << "Positions: " << endl;
	for (uint64_t i = 0; i < data->data.NumPos; i++)
	{
		s << "\t" << data->pointers.positions[i].x << " " << data->pointers.positions[i].y << " " << data->pointers.positions[i].z/* << " " << _datap.positions[i].w*/ << endl;
	}
	s << "Texcoords: " << endl;
	for (uint64_t i = 0; i < data->data.NumTex; i++)
	{
		s << "\t" << data->pointers.texCoords[i].u << " " << data->pointers.texCoords[i].v/* << " " << data->pointers.texCoords[i].w << " "*/ << endl;
	}
	s << "Normals: " << endl;
	for (uint64_t i = 0; i < data->data.NumNorm; i++)
	{
		s << "\t" << data->pointers.normals[i].x << " " << data->pointers.normals[i].y << " " << data->pointers.normals[i].z << endl;
	}
	if (data->data.NumSubMesh)
	{
		s << "SubMesh: " << endl;
		for (uint32_t n = 0; n < data->data.NumSubMesh; n++)
		{
			s << "\t" << data->pointers.subMesh[n].name << endl;
			for (uint64_t i = data->pointers.subMesh[n].faceStart; i < data->pointers.subMesh[n].faceCount + data->pointers.subMesh[n].faceStart; i++)
			{
				s << "\t\t";
				for (uint8_t j = 0; j < data->pointers.faces[i].indexCount; j++)
				{
					for (uint8_t k = 0; k < data->pointers.faces[i].indices[j].indexCount - 1; k++)
					{
						s << data->pointers.faces[i].indices[j].index[k] << "/";
					}
					if (data->pointers.faces[i].indices[j].indexCount > 0)
						s << data->pointers.faces[i].indices[j].index[data->pointers.faces[i].indices[j].indexCount - 1] << " ";
				}
				s << endl;
			}
		}
	}
	else
	{
		s << "Faces: " << endl;
		for (uint64_t i = 0; i < data->data.NumFace; i++)
		{
			s << "\t";
			for (uint8_t j = 0; j < data->pointers.faces[i].indexCount; j++)
			{
				for (uint8_t k = 0; k < data->pointers.faces[i].indices[j].indexCount - 1; k++)
				{
					s << data->pointers.faces[i].indices[j].index[k] << "/";
				}
				if (data->pointers.faces[i].indices[j].indexCount > 0)
					s << data->pointers.faces[i].indices[j].index[data->pointers.faces[i].indices[j].indexCount - 1] << " ";
			}
			s << endl;
		}
	}
    return s.str();
}

void Interpreter::increaseLocation(unsigned int loc) 
{
    m_location += loc;
  //  cout << "increaseLocation(): " << loc << ", total = " << m_location << endl;
}

unsigned int Interpreter::location() const 
{

    return m_location;
}
