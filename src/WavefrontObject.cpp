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


/// Loads a texture coordinate from a string stream
TextureCoordinate loadTextureCoordinate(std::stringstream is)
{
	std::string command{};
	TextureCoordinate t{};
	is >> command >> t.u;
	if (is.fail()) { throw TextureCoordinateLoadingException{ "Error loading texture coordinate" }; }
	is >> t.v;
	if (is.fail()) { t.v = 0.0f; } // Default to 0
	return t;
}


/// Loads a vertex normal from a string stream
VertexNormal loadVertexNormal(std::stringstream is)
{
	std::string command{};
	VertexNormal n{};
	is >> command >> n.i >> n.j >> n.k;
	if (is.fail()) { throw VertexNormalLoadingException{ "Error loading vertex normal" }; }
	return n;
}


template <char C>
std::istream& expect(std::istream& in)
{
	if ((in >> std::ws).peek() == C) { in.ignore(); }
	else { in.setstate(std::ios_base::failbit); }
	return in;
}


/// Loads a face from a string stream
Face loadFace(std::stringstream is)
{
	std::string command{};
	Face f{};
	is >> command >> f.indices[0];
	if (is.fail()) { throw FaceLoadingException{ "Error loading face" }; }
	char next{ static_cast<char>(is.peek()) };
	if (next == ' ') { // Only faces
		is >> f.indices[1] >> f.indices[2];
		if (is.fail()) { throw FaceLoadingException{ "Error loading face" }; }
		is >> f.indices[3];
		if (is.fail()) { f.indices[3] = -1; } // Default to invalid value
	} else if (next == '/') {
		is.ignore();
		next = is.peek();
		if (next == '/') { // Only vertices and normals
			is.ignore();
			is >> f.normals[0] >> f.indices[1] >> expect<'/'> >> expect<'/'> >> f.normals[1]
				>> f.indices[2] >> expect<'/'> >> expect<'/'> >> f.normals[2];
			if (is.fail()) { throw FaceLoadingException{ "Error loading face" }; }
			is >> f.indices[3] >> expect<'/'> >> expect<'/'> >> f.normals[3];
			if (is.fail()) { f.indices[3] = -1; } // Default to invalid value
		}
		else { // Vertices, coordinates and normals
			is >> f.textures[0] >> expect<'/'> >> f.normals[0] >>
				f.indices[1] >> expect<'/'> >> f.textures[1] >> expect<'/'> >> f.normals[1] >>
				f.indices[2] >> expect<'/'> >> f.textures[2] >> expect<'/'> >> f.normals[2];
			if (is.fail()) { throw FaceLoadingException{ "Error loading face" }; }
			is >> f.indices[3] >> expect<'/'> >> f.textures[3] >> expect<'/'> >> f.normals[3];
			if (is.fail()) { f.indices[3] = -1; } // Default to invalid value
		}
	}
	return f;
}


/// Reads a Wavefront Object
std::ifstream &operator>>(std::ifstream &is, WavefrontObject &obj)
{
	std::string line;
	unsigned lineNumber{ 1 };

	while (std::getline(is, line)) {
		if (line.length() == 0) { continue; } // Ignore empty line
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
						std::cout << "v(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n";
					}
					catch (VertexLoadingException &e) { // Invalid vertex command
						std::cerr << "Error at line " << lineNumber << ": " << e.what() <<  std::endl;
					}
					break;
				}
				case 'p': {
					std::cout << " > Point in the parameter space of a curve or a surface\n";
					break;
				}
				case 'n': { // Vertex Normal command
					try {
						VertexNormal n{ loadVertexNormal(std::stringstream{ line }) };
						obj.addVertexNormal(n);
						std::cout << "n(" << n.i << ", " << n.j << ", " << n.k << ")\n";
					} catch (VertexNormalLoadingException &e) {
						std::cerr << "Error at line " << lineNumber << ": " << e.what() << std::endl;
					}
					break;
				}
				case 't': { // Texture Coordinate command
					try {
						TextureCoordinate t{ loadTextureCoordinate(std::stringstream{ line }) };
						obj.addTextureCoordinate(t);
						std::cout << "vt(" << t.u << ", " << t.v << ")\n";
					} catch (TextureCoordinateLoadingException &e) { // Invalid texture coordinate command
						std::cerr << "Error at line " << lineNumber << ": " << e.what() << std::endl;
					}
					break;
				}
			}
			break;
		}
		case 'f': { // Face command
			try {
				Face f{ loadFace(std::stringstream{ line }) };
				obj.addFace(f);
				std::cout << "f(" << f.indices[0] << "/" << f.textures[0] << "/" << f.normals[0] <<
						", " << f.indices[1] << "/" << f.textures[1] << "/" << f.normals[1] <<
						", " << f.indices[2] << "/" << f.textures[2] << "/" << f.normals[2] <<
						", " << f.indices[3] << "/" << f.textures[3] << "/" << f.normals[3] << ")\n";
				break;
			}
			catch (FaceLoadingException &e) { // Invalid face command
				std::cerr << "Error at line " << lineNumber << ": " << e.what() << std::endl;
				break;
			}
		}
		default: {
			std::cerr << "Line " << lineNumber << " ignored: " << line <<  std::endl;
			break;
		}}
		++lineNumber;
	}
	return is;
}

