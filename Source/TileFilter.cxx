#include "TileFilter.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTileImageFilter.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>

TileFilter::TileFilter(char* dir, int num)
{
	image_dir = dir;
	num_files = num;
	vol_reader = VolReaderType::New();
	vol_writer = VolWriterType::New();
	tiler = TilerType::New();
}

/*
	Create a 3D image(stack of 2D images) from set of 2D image files. 
	image_dir: Directory where images are located
	num_files: Number of files to form 2D stack
	return: Name of file where 3D image has been written to.
		Returns empty string, "", if an error occured. 
*/
std::string TileFilter::createVolume()
{
	// testing
	printf("Reading %d files from directrory: %s\n", num_files, image_dir);

	itk::FixedArray<unsigned int, 3> layout;
	layout[0] = 1;
	layout[1] = 1;
	layout[2] = 0;

	tiler->SetLayout(layout);

	VolInputImageType::Pointer inputImageTile;

	struct stat file_stats; // check image file status
	char filename[1024]; // image file name buffer

	for (int i = 0; i < num_files; i++) {
		// Assumes files are named as: 000000.dcm ... 000999.dcm etc.
		sprintf_s(filename, sizeof(filename), "%s%06d.dcm", image_dir, i);

		// check that file exists
		if (stat(filename, &file_stats) != 0) {
			std::cerr << "ERROR: Could not read file: " << filename << std::endl;
			return ""; // stat error, no files read
		}
		else {
			vol_reader->SetFileName(filename);
			vol_reader->UpdateLargestPossibleRegion();
			inputImageTile = vol_reader->GetOutput();
			inputImageTile->DisconnectPipeline();
			tiler->SetInput((unsigned int)i, inputImageTile);
		}
		// testing
		printf("Processing file: %s\n", filename);
	}
	VolPixelType filler = 128;
	tiler->SetDefaultPixelValue(filler);
	tiler->Update();

	// write to filesystem and return file name
	std::stringstream outfile;
	outfile << "out_" << num_files << "_3D.dcm";
	
	vol_writer->SetInput(tiler->GetOutput());
	vol_writer->SetFileName(outfile.str().c_str());
	try {
		vol_writer->Update();
	}
	catch (itk::ExceptionObject& e) {
		std::cerr << e << std::endl;
		std::cerr << "ERROR: Could not create file: " << outfile.str() << std::endl;
		return ""; // error, file not saved
	}
	return outfile.str();
}
