
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

typedef unsigned char MedPixelType;
const unsigned int Dimension = 3;

typedef itk::Image<MedPixelType, Dimension> MedImageType;

typedef itk::ImageFileReader<MedImageType> MedReaderType;
typedef itk::ImageFileWriter<MedImageType> MedWriterType;

typedef itk::MedianImageFilter<MedImageType, MedImageType> MedFilterType;

// Function prototypes
std::string medianFilter(char*, int);

/*
Apply median filter to the specified image.
filename: File path to where image is located
r: Radius that median filter will use
return: Name of file where median image has been written to.
Returns empty string, "", if an error occured.
*/
std::string medianFilter(char* filename, int r)
{
	// testing
	printf("Applying median filter with a radius of %d to file: %s\n", r, filename);

	struct stat file_stats; // check image file status

	// check that file exists
	if (stat(filename, &file_stats) != 0) {
		std::cerr << "ERROR: Could not read file: " << filename << std::endl;
		return ""; // stat error, no files read
	}

	MedReaderType::Pointer med_reader = MedReaderType::New();
	med_reader->SetFileName(filename);

	// Setup filter with radius
	MedFilterType::Pointer medianFilter = MedFilterType::New();
	MedFilterType::InputSizeType radius;
	radius.Fill(r);
	medianFilter->SetRadius(radius);
	medianFilter->SetInput(med_reader->GetOutput());
	medianFilter->Update();

	// write to filesystem and return file name
	std::stringstream outfile;
	outfile << "out_" << r << "_median.dcm";

	MedWriterType::Pointer med_writer = MedWriterType::New();
	med_writer->SetInput(medianFilter->GetOutput());
	med_writer->SetFileName(outfile.str().c_str());
	try {
		med_writer->Update();
		return outfile.str();
	}
	catch (itk::ExceptionObject& e) {
		std::cerr << e << std::endl;
		std::cerr << "ERROR: Could not create file: " << outfile.str() << std::endl;
		return ""; // error, file not saved
	}
}
