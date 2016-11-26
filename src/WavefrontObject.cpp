#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "WavefrontObject.h"


/// Loads a vertex from a string stream
Vertex loadVertex(std::stringstream is)
{
	std::string command{};
	Vertex v{};
	is >> command >> v.x >> v.y >> v.z;
	if (is.fail()) { throw VertexLoadingException{ "Error loading vertex" }; }
	is >> v.w;
	if (is.fail()) { v.w = 1.0f; } // Default to 0
	return v;
}


/// Loads a face from a string stream
Face loadFace(std::stringstream is)
{
	std::string command{};
	Face f{};
	is >> command >> f.indices[0] >> f.indices[1] >> f.indices[2];
	if (is.fail()) { throw FaceLoadingException{ "Error loading face" }; }
	is >> f.indices[3];
	if (is.fail()) { f.indices[3] = -1; } // Default to invalid value
	return f;
}


/// Reads a Wavefront Object
std::ifstream &operator>>(std::ifstream &is, WavefrontObject &obj)
{
	std::string line;
	unsigned lineNumber{ 1 };

	while (std::getline(is, line)) {
		switch (line[0]) {
		case '#': {
			std::cout << "## Comment\n";
			break;
		}
		case 'v': {
			if (line.length() < 2) {
				std::cerr << "Error at line " << lineNumber << ": " << line <<  std::endl;
				break;
			}
			switch (line[1]) {
				case ' ': { // Vertex command
					try {
						Vertex v{ loadVertex(std::stringstream{ line }) };
						obj.addVertex(v);
						std::cout << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
						break;
					}
					catch (VertexLoadingException &e) { // Invalid vertex command
						std::cerr << "Error at line " << lineNumber << ": " << e.what() <<  std::endl;
						break;
					}
				}
				case 'p': {
					std::cout << " > Point in the parameter space of a curve or a surface\n";
					break;
				}
				case 'n': {
					std::cout << " > Normal vector\n";
					break;
				}
				case 't': {
					std::cout << " > Texture coordinates\n";
					break;
				}
			}
			break;
		}
		case 'f': { // Face command
			try {
				Face f{ loadFace(std::stringstream{ line }) };
				obj.addFace(f);
				std::cout << f.indices[0] << " " << f.indices[1] << " "
					<< f.indices[2] << " " << f.indices[3] << std::endl;
				break;
			}
			catch (FaceLoadingException &e) { // Invalid face command
				std::cerr << "Error at line " << lineNumber << ": " << e.what() << std::endl;
				break;
			}
		}
		default: {
			std::cerr << "Error at line " << lineNumber << ": " << line <<  std::endl;
			break;
		}}
		++lineNumber;
		std::cout << "\n";
	}
	return is;
}

