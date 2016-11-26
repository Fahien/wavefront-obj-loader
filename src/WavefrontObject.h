#ifndef SST_WAVEFRONTOBJECT_H
#define SST_WAVEFRONTOBJECT_H

#include <fstream>
#include <exception>
#include <vector>


class VertexLoadingException : public std::runtime_error {
  public:
	VertexLoadingException(const std::string &message) : std::runtime_error(message) {}
};


class FaceLoadingException : public std::runtime_error {
  public:
	FaceLoadingException(const std::string &message) : std::runtime_error(message) {};
};


struct Vertex {
  public:
	float x;
	float y;
	float z;
	float w;
};


struct Face {
  public:
	int indices[4];
};


class WavefrontObject {
  public:

	inline void addVertex(const Vertex v) { vertices_.push_back(v); }
	inline void addFace(const Face f) { faces_.push_back(f); }

	friend std::ifstream &operator>>(std::ifstream &in, WavefrontObject &obj);

  private:
	std::vector<Vertex> vertices_;
	std::vector<Face> faces_;
};

#endif // SST_WAVEFRONTOBJECT_H

