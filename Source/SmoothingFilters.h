
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMedianImageFilter.h"
//#include "itksys/SystemTools.hxx"

#include <string>

#ifndef SMOOTHINGFILTERS_H
#define SMOOTHINGFILTERS_H

class SmoothingFilters
{
private:
	// Median filter items
	typedef unsigned char MedPixelType;
	typedef itk::Image<MedPixelType, 3> MedImageType;
	typedef itk::ImageFileReader<MedImageType> MedReaderType;
	typedef itk::ImageFileWriter<MedImageType> MedWriterType;
	typedef itk::MedianImageFilter<MedImageType, MedImageType> MedFilterType;

	char* filename;
	MedReaderType::Pointer med_reader;
	MedWriterType::Pointer med_writer;
	MedFilterType::Pointer med_filter;

public:
	// Constructor
	SmoothingFilters(char* file);

	/*
	Apply median filter to the specified image.
	filename: File path to where image is located
	r: Radius that median filter will use
	return: Name of file where median image has been written to.
	Returns empty string, "", if an error occured.
	*/
	std::string applyMedianFilter(int r);
};

#endif