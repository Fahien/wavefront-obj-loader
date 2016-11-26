wavefront-obj-loader: src/Main.cpp src/WavefrontObject.cpp
	g++ -W -Wall -pedantic -std=c++11 src/Main.cpp src/WavefrontObject.cpp -o wavefront-obj-loader -I.
