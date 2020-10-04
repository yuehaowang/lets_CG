
#include <iostream>
#include <algorithm>
#include <string>
#include <map>

#include "triangleMesh.hpp"

inline float clamp(float x) { return x < 0 ? 0 : x > 1 ? 1 : x; }
inline unsigned char toInt(float x) { return (unsigned char)(pow(clamp(x), 1 / 2.2) * 255 + .5); }

int main(int argc, char* argv[])
{
	std::string obj_name = "pikachu";  // default: pikachu
	if (argc > 1) {
		obj_name = argv[1];
	}

	std::string obj_filePos("../resources/" + obj_name + ".obj");
	TriangleMesh mesh(obj_filePos);
	mesh.subdivision();
	mesh.save("./" + obj_name + "_subdiv.obj");

	return 0;
}