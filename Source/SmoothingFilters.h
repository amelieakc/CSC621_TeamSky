
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMedianImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkBilateralImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "QuickView.h"
//#include "itksys/SystemTools.hxx"

#include <string>

#ifndef SMOOTHINGFILTERS_H
#define SMOOTHINGFILTERS_H

class SmoothingFilters
{
private:
	// Median filter items
	typedef unsigned char PixelType;
	typedef itk::Image<PixelType, 3> ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::ImageFileWriter<ImageType> WriterType;
	typedef itk::MedianImageFilter<ImageType, ImageType> MedFilterType;
	typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType> DGausFilterType;
	typedef itk::BilateralImageFilter<ImageType, ImageType> BilateralFilterType;

	char* filename;
	ReaderType::Pointer filter_reader;
	WriterType::Pointer filter_writer;
	MedFilterType::Pointer med_filter;
	DGausFilterType::Pointer dgaus_filter;
	BilateralFilterType::Pointer bilateral_filter;

public:
	// Constructor
	SmoothingFilters(char* file);

	/*
	Apply filters to the specified image.
	filename: File path to where image is located
	r: Radius that median filter will use
	return: Name of file where median image has been written to.
	Returns empty string, "", if an error occured.
	*/
	std::string applyMedianFilter(int r);
	std::string applyDiscreteGaussianFilter(int v);
	std::string applyBilateralFilter(int d, int r);
};

#endif