#include "SmoothingFilters.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itksys/SystemTools.hxx"

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>

SmoothingFilters::SmoothingFilters(char* file)
{
	filename = file;
	filter_reader = ReaderType::New();
	filter_writer = WriterType::New();
	med_filter = MedFilterType::New();
	dgaus_filter = DGausFilterType::New();
	bilateral_filter = BilateralFilterType::New();
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
	filter_reader->SetFileName(filename);

	// Setup and apply median filter with radius r
	MedFilterType::InputSizeType radius;
	radius.Fill(r);
	med_filter->SetRadius(radius);
	med_filter->SetInput(filter_reader->GetOutput());
	med_filter->Update();

	// write to filesystem and return file name
	std::stringstream outfile;
	outfile << "out_" << r << "_median.dcm";

	filter_writer->SetInput(med_filter->GetOutput());
	filter_writer->SetFileName(outfile.str().c_str());
	try {
		filter_writer->Update();
	}
	catch (itk::ExceptionObject& e) {
		std::cerr << e << std::endl;
		std::cerr << "ERROR: Could not create file: " << outfile.str() << std::endl;
		return ""; // error, file not saved
	}
	return outfile.str();
}
std::string SmoothingFilters::applyDiscreteGaussianFilter(int v)
{
	// testing
	printf("Applying discrete Gaussian filter with a variance of %d to file: %s\n", v, filename);

	// check that file exists
	struct stat file_stats;
	if (stat(filename, &file_stats) != 0) {
		std::cerr << "ERROR: Could not read file: " << filename << std::endl;
		return ""; // stat error, no files read
	}

	// Read image from file
	filter_reader->SetFileName(filename);

	// Setup and apply median filter with variance v
	DGausFilterType::ArrayType variance;
	dgaus_filter->SetVariance(v);
	dgaus_filter->SetInput(filter_reader->GetOutput());
	dgaus_filter->Update();

	// write to filesystem and return file name
	std::stringstream outfile;
	outfile << "out_" << v << "_dGaus.dcm";

	filter_writer->SetInput(dgaus_filter->GetOutput());
	filter_writer->SetFileName(outfile.str().c_str());
	try {
		filter_writer->Update();
	}
	catch (itk::ExceptionObject& e) {
		std::cerr << e << std::endl;
		std::cerr << "ERROR: Could not create file: " << outfile.str() << std::endl;
		return ""; // error, file not saved
	}
	return outfile.str();
}
std::string SmoothingFilters::applyBilateralFilter(int d, int r)
{
	// testing
	printf("Applying bilateral filter with domainSigma = %d and rangeSigma = %d to file: %s\n", d, r, filename);

	// check that file exists
	struct stat file_stats;
	if (stat(filename, &file_stats) != 0) {
		std::cerr << "ERROR: Could not read file: " << filename << std::endl;
		return ""; // stat error, no files read
	}

	// Read image from file
	filter_reader->SetFileName(filename);

	// Setup and apply bilateral filter with Sigmas
	bilateral_filter->SetInput(filter_reader->GetOutput());
	bilateral_filter->SetDomainSigma(d);
	bilateral_filter->SetRangeSigma(r);
	bilateral_filter->Update();

	// write to filesystem and return file name
	std::stringstream outfile;
	outfile << "out_" << d << "_" << r << "_bilateral.dcm";

	filter_writer->SetInput(bilateral_filter->GetOutput());
	filter_writer->SetFileName(outfile.str().c_str());
	try {
		filter_writer->Update();
	}
	catch (itk::ExceptionObject& e) {
		std::cerr << e << std::endl;
		std::cerr << "ERROR: Could not create file: " << outfile.str() << std::endl;
		return ""; // error, file not saved
	}
	return outfile.str();
}
