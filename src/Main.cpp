#include <iostream>
#include <fstream>
#include <cstdlib>

#include "WavefrontObject.h"


int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " file.obj\n";
		return EXIT_FAILURE;
	}

	std::ifstream is{ argv[1] };
	if (!is.is_open()) {
		std::cerr << "Could not find " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}

	WavefrontObject obj{};
	is >> obj;
	return EXIT_SUCCESS;
}

