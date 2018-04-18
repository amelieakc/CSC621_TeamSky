
#include "itkWatershedImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"

#include <sys/stat.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>

std::string watershedSegmentation(char*, float, float);

std::string watershedSegmentation(char* filename, float threshold, float level)
{
	std::cout << "Watershed segmentation on file " << filename << " with threshold: " << threshold << ", level: " << level << std::endl;

	typedef float SegPixelType;
	typedef itk::RGBPixel<unsigned char> RGBPixelType;
	const unsigned int SegDimension = 3;
	typedef itk::Image<SegPixelType, SegDimension> SegImageType;
	typedef itk::Image<RGBPixelType, SegDimension> RGBImageType;

	typedef itk::ImageFileReader<SegImageType> SegReaderType;
	typedef itk::ImageFileWriter<RGBImageType> SegWriterType;

	struct stat file_stats; // check image file status

	// check that file exists
	if (stat(filename, &file_stats) != 0) {
		std::cerr << "ERROR: Could not read file: " << filename << std::endl;
		return ""; // stat error, no files read
	}

	// Read in file
	SegReaderType::Pointer seg_reader = SegReaderType::New();
	seg_reader->SetFileName(filename);

	// Apply gradient magnitude filter
	typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<SegImageType, SegImageType> GradientMagnitudeFilterType;
	GradientMagnitudeFilterType::Pointer gradientMagnitudeFilter = GradientMagnitudeFilterType::New();
	gradientMagnitudeFilter->SetInput(seg_reader->GetOutput());
	gradientMagnitudeFilter->SetSigma(1.0);

	// Apply watershed filter
	typedef itk::WatershedImageFilter<SegImageType> WatershedFilterType;
	WatershedFilterType::Pointer watershedFilter = WatershedFilterType::New();
	watershedFilter->SetInput(gradientMagnitudeFilter->GetOutput());
	watershedFilter->SetThreshold(threshold);
	watershedFilter->SetLevel(level);

	// note: labelled image is stored in watershedFilter->GetOutput()

	// Encode label image into color image
	typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long> ColorMapFunctorType;
	typedef WatershedFilterType::OutputImageType LabeledImageType;
	
	typedef itk::UnaryFunctorImageFilter<LabeledImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;
	ColorMapFilterType::Pointer colorMapFilter = ColorMapFilterType::New();
	colorMapFilter->SetInput(watershedFilter->GetOutput());

	// write to filesystem and return file name
	std::stringstream outfile;
	outfile << "out_" << threshold << "_" << level << "_segmentation.dcm";

	SegWriterType::Pointer seg_writer = SegWriterType::New();
	seg_writer->SetFileName(outfile.str().c_str());
	seg_writer->SetInput(colorMapFilter->GetOutput());
	try {
		seg_writer->Update();
	}
	catch (itk::ExceptionObject& e) {
		std::cerr << "" << std::endl;
		return "";
	}
	return outfile.str();
}