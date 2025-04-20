#include "wavereader.h"
#include <iostream>

int main()
{
	rolling::WaveReader reader;
	reader.Load("test.wav");
	std::cout << "Hello, World!" << std::endl;
	return 0;
}