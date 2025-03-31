#include "wavereader.h"
#include <iostream>

int main()
{
	raudio::WaveReader reader;
	reader.Load("test.wav");
	std::cout << "Hello, World!" << std::endl;
	return 0;
}