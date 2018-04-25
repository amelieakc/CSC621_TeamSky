#include "SmoothingFilters.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMedianImageFilter.h"
#include "itksys/SystemTools.hxx"

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>

SmoothingFilters::SmoothingFilters(char* file)
{
	filename = file;
	med_reader = MedReaderType::New();
	med_writer = MedWriterType::New();
	med_filter = MedFilterType::New();
}

/*
Apply median filter to the specified image.
filename: File path to where image is located
r: Radius that median filter will use
return: Name of file where median image has been written to.
Returns empty string, "", if an error occured.
*/
std::string SmoothingFilters::applyMedianFilter(int r)
{
	// testing
	printf("Applying median filter with a radius of %d to file: %s\n", r, filename);

	// check that file exists
	struct stat file_stats;
	if (stat(filename, &file_stats) != 0) {
		std::cerr << "ERROR: Could not read file: " << filename << std::endl;
		return ""; // stat error, no files read
	}

	// Read image from file
	med_reader->SetFileName(filename);

	// Setup and apply median filter with radius r
	MedFilterType::InputSizeType radius;
	radius.Fill(r);
	med_filter->SetRadius(radius);
	med_filter->SetInput(med_reader->GetOutput());
	med_filter->Update();

	// write to filesystem and return file name
	std::stringstream outfile;
	outfile << "out_" << r << "_median.dcm";

	med_writer->SetInput(med_filter->GetOutput());
	med_writer->SetFileName(outfile.str().c_str());
	try {
		med_writer->Update();
	}
	catch (itk::ExceptionObject& e) {
		std::cerr << e << std::endl;
		std::cerr << "ERROR: Could not create file: " << outfile.str() << std::endl;
		return ""; // error, file not saved
	}
	return outfile.str();
}
