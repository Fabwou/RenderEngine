#include "OBJLoader.h"
#include "../Core/Maths/Vectors/Vec2/Vec2.h"
#include "../Mesh/Vertex.h"
#include <vector>
#include <string>
#include <sstream>
#include<fstream>

Model* ExtractModel(Device* device, LPCWSTR filePath)
{
	//Vertex portions
	std::vector<Vec3> vertex_positions;
	std::vector<Vec2> vertex_texcoords;
	std::vector<Vec3> vertex_normals;

	//Face vectors
	std::vector<int> vertex_position_indicies;
	std::vector<int> vertex_texcoord_indicies;
	std::vector<int> vertex_normal_indicies;

	//Vertex array
	std::vector<Vertex> vertices;

	std::stringstream ss;
	std::ifstream in_file(filePath);
	std::string line = "";
	std::string prefix = "";
	Vec3 temp_vec3;
	Vec2 temp_vec2;
	int temp_int = 0;

	//File open error check
	if (!in_file.is_open())
	{
		throw "ERROR::OBJLOADER::Could not open file.";
	}

	//Read one line at a time
	while (std::getline(in_file, line))
	{
		//Get the prefix of the line
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#")
		{

		}
		else if (prefix == "o")
		{

		}
		else if (prefix == "s")
		{

		}
		else if (prefix == "use_mtl")
		{

		}
		else if (prefix == "v") //Vertex position
		{
			ss >> temp_vec3.m_x >> temp_vec3.m_y >> temp_vec3.m_z;
			vertex_positions.push_back(temp_vec3);
		}
		else if (prefix == "vt")
		{
			ss >> temp_vec2.m_x >> temp_vec2.m_y;
			vertex_texcoords.push_back(temp_vec2);
		}
		else if (prefix == "vn")
		{
			ss >> temp_vec3.m_x >> temp_vec3.m_y >> temp_vec3.m_z;
			vertex_normals.push_back(temp_vec3);
		}
		else if (prefix == "f")
		{
			int counter = 0;
			while (ss >> temp_int)
			{
				//Pushing indices into correct arrays
				if (counter == 0)
					vertex_position_indicies.push_back(temp_int);
				else if (counter == 1)
					vertex_texcoord_indicies.push_back(temp_int);
				else if (counter == 2)
					vertex_normal_indicies.push_back(temp_int);

				//Handling characters
				if (ss.peek() == '/')
				{
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					++counter;
					ss.ignore(1, ' ');
				}

				//Reset the counter
				if (counter > 2)
					counter = 0;
			}
		}
		else
		{

		}
	}

	//Build final vertex array (mesh)
	vertices.resize(vertex_position_indicies.size(), Vertex());

	//Load in all indices
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].m_position = vertex_positions[vertex_position_indicies[i] - 1];
		vertices[i].m_texCoord = vertex_texcoords[vertex_texcoord_indicies[i] - 1];
		vertices[i].m_normal = vertex_normals[vertex_normal_indicies[i] - 1];
	}

	return new Model(device, filePath, vertices, vertex_position_indicies);
}