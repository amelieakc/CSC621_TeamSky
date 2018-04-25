#include "WatershedSegmentation.h"

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

WatershedSegmentation::WatershedSegmentation(char* file)
{
	filename = file;
	seg_reader = SegReaderType::New();
	seg_writer = SegWriterType::New();
}

/*
Apply watershed segmentation to the specified image.
threshold: threshold value that segmentation will use
level: level value that segmentation will use
return: Name of file where segmented image has been written to.
Returns empty string, "", if an error occured.
*/
std::string WatershedSegmentation::applyWatershedSegmentation(float threshold, float level)
{
	// for testing
	std::cout << "Watershed segmentation on file " << filename << " with threshold: " << threshold << ", level: " << level << std::endl;

	// check that file exists
	struct stat file_stats;
	if (stat(filename, &file_stats) != 0) {
		std::cerr << "ERROR: Could not read file: " << filename << std::endl;
		return ""; // stat error, no files read
	}

	// Read in file
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

	seg_writer->SetFileName(outfile.str().c_str());
	seg_writer->SetInput(colorMapFilter->GetOutput());
	try {
		seg_writer->Update();
	}
	catch (itk::ExceptionObject& e) {
		std::cerr << e << std::endl;
		std::cerr << "" << std::endl;
		return "";
	}
	return outfile.str();
}