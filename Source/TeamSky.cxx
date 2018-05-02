#include "TileFilter.h"
#include "TileFilter.cxx"
#include "SmoothingFilters.h"
#include "SmoothingFilters.cxx"
#include "WatershedSegmentation.h"
#include "WatershedSegmentation.cxx"

#include <iostream>
#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
	if(argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		// display help	
		//std::cout << "Team Sky ITK" << std::endl;
		printf("%-30s\n", "Team Sky ITK");
		printf("%-30s %-30s\n", "-h", "Display help menu");
		printf("%-30s %-30s\n", "-r", "Registration");
		printf("%-30s %-30s\n", "-s image thresh level [labelVal]", "Apply watershed segmentation with threshold and level.");
		printf("%-30s %-30s\n", " ", "Remove labels that have a count less than labelVal (defaults to 0). ");
		printf("%-30s %-30s\n", "-3D image_dir num", "Create 3D image from multiple 2D images");
		printf("%-30s %-30s\n", "-f filter_name [args]", "Apply filter to image. Filters supported: median");
		std::cout << std::endl;
		// add more options
	}
	else if(strcmp(argv[1], "-r") == 0) {
		std::cout << "Registration" << std::endl;
		// do registration stuff
	}
	else if(strcmp(argv[1], "-s") == 0) {
		std::cout << "Watershed Segmentation" << std::endl;
		// Check watershed segmentation args
		if(argc < 5) {
			std::cerr << "ERROR: Invalid number of args" << std::endl;
			std::cerr << "Usage: TeamSky.exe -s image threshold level" << std::endl;
			return EXIT_FAILURE;
		}
		else {
			WatershedSegmentation watershed = WatershedSegmentation(argv[2]);
			std::string seg_filename;
			if (argc < 6)
				seg_filename = watershed.applyWatershedSegmentation(atof(argv[3]), atof(argv[4]));
			else
				seg_filename = watershed.applyWatershedSegmentation(atof(argv[3]), atof(argv[4]), atoi(argv[5]));

			if(seg_filename.empty()) {
				// file was not created or segmentation failed
				std::cerr << "ERROR: Could not complete watershed segmentation" << std::endl;
				return EXIT_FAILURE;
			}
			else {
				// file was created, watershed segmentation complete
				std::cout << "Watershed segmentation complete: " << seg_filename << std::endl;
			}
		}
	}
	else if(strcmp(argv[1], "-3D") == 0 || strcmp(argv[1], "-3d") == 0) {
		std::cout << "Create 3D image from multiple 2D images" << std::endl;
		// Check args: image directory and number of files
		if(argc != 4) {
			std::cerr << "ERROR: Invalid number of args" << std::endl;
			std::cerr << "Usage: TeamSky.exe -3D image_dir num_files" << std::endl;
			return EXIT_FAILURE;
		}
		else {
			// Use tile image filter to create 3D image from provided files
			TileFilter tile_filter = TileFilter(argv[2], atoi(argv[3]));
			std::string volume_filename = tile_filter.createVolume();
			if(volume_filename.empty()) {
				// file was not created
				std::cerr << "ERROR: Could not create 3D image" << std::endl;
				return EXIT_FAILURE;
			}
			else {
				// file was created
				std::cout << "3D image created: " << volume_filename << std::endl;
			}
		}
	}
	else if(strcmp(argv[1], "-f") == 0) {
		std::cout << "Apply filter to image" << std::endl;
		if (argc > 2) {
			// check filter name and args and apply filter
			std::cout << "applying filter: " << argv[2] << std::endl;
			if(strcmp(argv[2], "median") == 0) {
				// Median filter requires 2 additional args: file and radius
				if (argc != 5) {
					std::cerr << "ERROR: Invalid number of args" << std::endl;
					std::cerr << "Usage: TeamSky.exe -f median image_file radius" << std::endl;
					return EXIT_FAILURE;
				}
				else {
					// apply median filter
					SmoothingFilters median_filter = SmoothingFilters(argv[3]);
					std::string median_filename = median_filter.applyMedianFilter(atoi(argv[4]));
					if (median_filename.empty()) {
						// file was not created
						std::cerr << "ERROR: Could not create median image" << std::endl;
						return EXIT_FAILURE;
					}
					else {
						// file was created
						std::cout << "median image created: " << median_filename << std::endl;
					}
				}
			}
			else {
				std::cerr << "ERROR: Unknown filter type: " << argv[2] << std::endl;
				return EXIT_FAILURE;
			}
		}
		else {
			// filter name and arguments not specified
			std::cerr << "ERROR: Invalid number of args" << std::endl;
			std::cerr << "Usage: TeamSky.exe -f filter_name [args]" << std::endl;
			return EXIT_FAILURE;
		}
	}
	else {
		std::cout << "Invalid option" << std::endl;
	}

	return EXIT_SUCCESS;
}

