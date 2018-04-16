
#include "itkImage.h"
#include "TileFilter.cxx"

#include <iostream>
#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
	if(argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		// display help	
		//std::cout << "Team Sky ITK" << std::endl;
		printf("%-10s\n", "Team Sky ITK");
		printf("%-10s %-30s\n", "-h", "Display help menu");
		printf("%-10s %-30s\n", "-r", "Registration");
		printf("%-10s %-30s\n", "-s", "Segmentation");
		printf("%-10s %-30s\n", "-3D", "Create 3D image from multiple 2D images");
		std::cout << std::endl;
		// add more options
	}
	else if(strcmp(argv[1], "-r") == 0) {
		std::cout << "Registration" << std::endl;
		// do registration stuff
	}
	else if(strcmp(argv[1], "-s") == 0) {
		std::cout << "Segmentation" << std::endl;
		// do segmentation stuff
	}
	else if(strcmp(argv[1], "-3D") == 0 || strcmp(argv[1], "-3d") == 0) {
		std::cout << "Create 3D image from multiple 2D images" << std::endl;
		// Check args: image directory and number of files
		if(argc != 4) {
			std::cerr << std::endl;
			std::cerr << "ERROR: Invalid number of args" << std::endl;
			std::cerr << "Usage: TeamSky.exe -3D image_dir num_files" << std::endl;
			std::cerr << std::endl;
		}
		else {
			// Use tile image filter to create 3D image from provided files
			std::string volume_filename = createVolumeFromFiles(argv[2], atoi(argv[3]));
			if(strcmp(volume_filename.c_str(), "") == 0) {
				// file was not created
				std::cerr << std::endl;
				std::cerr << "ERROR: Could not create 3D image" << std::endl;
				std::cerr << std::endl;
			}
			else {
				// file was created
				std::cout << "3D image created: " << volume_filename << std::endl;
			}
		}
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

