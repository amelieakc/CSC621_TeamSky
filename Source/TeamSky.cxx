#include "TileFilter.h"
#include "TileFilter.cxx"
#include "SmoothingFilters.h"
#include "SmoothingFilters.cxx"
#include "WatershedSegmentation.h"
#include "WatershedSegmentation.cxx"
#include "ReadDicom.cxx"
#include "WarpImageFilter.cxx"

#include <iostream>
#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
	if(argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		// display help	
		//std::cout << "Team Sky ITK" << std::endl;
		printf("%-30s\n", "Team Sky ITK");
		printf("%-30s %-30s\n", "-h, --help", "Display help menu");
		printf("%-30s %-30s\n", "-r filter_name [args]", "Apply registration method given by filter_name.");
		printf("%-30s %-30s\n", " ", "Filters supported: warp");
		printf("%-30s %-30s\n", "-s image thresh level [labelVal]", "Apply watershed segmentation with threshold and level.");
		printf("%-30s %-30s\n", " ", "Remove labels that have a count less than labelVal (defaults to 0). ");
		printf("%-30s %-30s\n", "-3D image_dir num", "Create 3D image from multiple 2D images.");
		printf("%-30s %-30s\n", "-dicom image_dir", "Create 3D image from multiple dicom images using GDCM.");
		printf("%-30s %-30s\n", "-f filter_name [args]", "Apply filter to image. Filters supported: median, ");
		printf("%-30s %-30s\n", " ", "dgaussian, bilateral");
		printf("%-30s %-30s\n", " ", "Use \"-f filter_name\" without args to see usage.");
		std::cout << std::endl;
		// add more options
	}
	else if (strcmp(argv[1], "-r") == 0) {
		std::cout << "Registration" << std::endl;
		// do registration stuff
		if (argc >= 3) {
			if (strcmp(argv[2], "warp") == 0) {
				std::cout << "Applying warp image filter" << std::endl;
				if (argc >= 5) {
					// do warpimagefilter with argv[3] and argv[4]
					std::string warp_filename = applyWarpImageFilter(argv[3], argv[4]);
					if (warp_filename.empty()) {
						// file was not created or warp image filter failed
						std::cerr << "ERROR: Could not complete warp image filter" << std::endl;
						return EXIT_FAILURE;
					}
					else {
						std::cout << "Warp image filter complete" << std::endl;
					}
				}
				else {
					// did not provide fixed and moving images
					std::cerr << "ERROR: Did not provide fixed and moving images" << std::endl;
					std::cerr << "USage: TeamSky.exe -r warp image1 image2";
					return EXIT_FAILURE;
				}
			}
			else {
				// unknown registration method
				std::cerr << "ERROR: Unknown registration method: " << argv[2] << std::endl;
				return EXIT_FAILURE;
			}
		}
		else {
			// invalid number of args
			std::cerr << "ERROR: Invalid number of arguments" << std::endl;
			return EXIT_FAILURE;
		}
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
	else if (strcmp(argv[1], "-dicom") == 0) {
		std::cout << "Create 3D dicom image from multiple 2D dicom images" << std::endl;
		// Check args: image directory and number of files
		if (argc != 3) {
			std::cerr << "ERROR: Invalid number of args" << std::endl;
			std::cerr << "Usage: TeamSky.exe -dicom image_dir" << std::endl;
			return EXIT_FAILURE;
		}
		else {
			// use read dicom fcn
			std::string dicom_filename = readDicom(argv[2]);
			if (dicom_filename.empty()) {
				// file was not created
				std::cerr << "ERROR: Could not create 3D image" << std::endl;
				return EXIT_FAILURE;
			}
			else {
				// file was created
				std::cout << "3D image created: " << dicom_filename << std::endl;
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
			}else if(strcmp(argv[2], "dgaussian") == 0) {
				// Discrete Gaussian filter requires 2 additional args: file and variance
				if (argc != 5) {
					std::cerr << "ERROR: Invalid number of args" << std::endl;
					std::cerr << "Usage: TeamSky.exe -f dgaussian image_file variance" << std::endl;
					return EXIT_FAILURE;
				}
				else {
					// apply discrete gaussian filter
					SmoothingFilters dgaus_filter = SmoothingFilters(argv[3]);
					std::string dgaus_filename = dgaus_filter.applyDiscreteGaussianFilter(atoi(argv[4]));
					if (dgaus_filename.empty()) {
						// file was not created
						std::cerr << "ERROR: Could not create image" << std::endl;
						return EXIT_FAILURE;
					}
					else {
						// file was created
						std::cout << "discrete Gaussian image created: " << dgaus_filename << std::endl;
					}
				}
			}else if (strcmp(argv[2], "bilateral") == 0) {
				// bilateral filter requires 3 additional args: file and domain_sigam range_sigam
				if (argc != 6) {
					std::cerr << "ERROR: Invalid number of args" << std::endl;
					std::cerr << "Usage: TeamSky.exe -f bilateral image_file domain_sigam range_sigam" << std::endl;
					return EXIT_FAILURE;
				}
				else {
					// apply discrete gaussian filter
					SmoothingFilters bilateral_filter = SmoothingFilters(argv[3]);
					std::string bilateral_filename = bilateral_filter.applyBilateralFilter(atoi(argv[4]), atoi(argv[5]));
					if (bilateral_filename.empty()) {
						// file was not created
						std::cerr << "ERROR: Could not create image" << std::endl;
						return EXIT_FAILURE;
					}
					else {
						// file was created
						std::cout << "Bilateral image created: " << bilateral_filename << std::endl;
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

 
