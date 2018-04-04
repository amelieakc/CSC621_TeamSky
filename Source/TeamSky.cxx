
#include "itkImage.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
	if(argc < 2 || (strcmp(argv[1], "-h") == 0) || strcmp(argv[1], "--help")) {
		// display help	
		std::cout << "Team Sky ITK" << std::endl;
		std::cout << "-h \tdisplay help" << std::endl;
		std::cout << "-r \tregistration" << std::endl;
		std::cout << "-s \tsegmentation" << std::endl;
		// add more options
	}
	else if(strcmp(argv[1], "-r") == 0) {
		std::cout << "Registration" << std::endl;
		// do regiatration stuff
	}
	else if(strcmp(argv[1], "-s") == 0) {
		std::cout << "Segmentation" << std::endl;
		// do segmentation stuff
	}
	else {
		std::cout << "Invalid option" << std::endl;
	}

	// ITK hello world test
	typedef itk::Image< unsigned short, 3 > ImageType;
	ImageType::Pointer image = ImageType::New();
	std::cout << "ITK Hello World !" << std::endl;

	return EXIT_SUCCESS;
}

