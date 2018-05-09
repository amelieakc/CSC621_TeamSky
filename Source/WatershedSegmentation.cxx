#include "WatershedSegmentation.h"

#include "itkWatershedImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"
#include "itkChangeLabelImageFilter.h"

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
std::string WatershedSegmentation::applyWatershedSegmentation(float threshold, float level, int labelValue)
{
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
	WatershedFilterType::Pointer watershedFilter = WatershedFilterType::New();
	watershedFilter->SetInput(gradientMagnitudeFilter->GetOutput());
	watershedFilter->SetThreshold(threshold);
	watershedFilter->SetLevel(level);

	// Apply label manipulation
	watershedFilter->Update();
	ChangeLabelImageFilterType::Pointer changeLabelFilter = ChangeLabelImageFilterType::New();
	// remove labels from the orginal labelImage produced by Watershed Segmentation
	updateLabels(watershedFilter->GetOutput(), gradientMagnitudeFilter->GetOutput(), changeLabelFilter, labelValue);

	// Encode label image into color image
	typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long> ColorMapFunctorType;
	typedef WatershedFilterType::OutputImageType LabeledImageType;
	typedef itk::UnaryFunctorImageFilter<LabeledImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;
	ColorMapFilterType::Pointer colorMapFilter = ColorMapFilterType::New();
	//colorMapFilter->SetInput(watershedFilter->GetOutput());

	// use updated labelImage
	colorMapFilter->SetInput(changeLabelFilter->GetOutput());

	// write to filesystem and return file name
	std::stringstream outfile;
	if (labelValue != 0) {
		outfile << "out_" << threshold << "_" << level << "_" << labelValue << "_segmentation.dcm";
	}
	else {
		outfile << "out_" << threshold << "_" << level << "_segmentation.dcm";
	}

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

/*
Update the labelImage produced by watershed segmentation.
Used to remove labels that do not meet a specified criteria.

labelImage: label image produced by watershed segmentation.
originalImage: image produced by gradient image filter.
changeLabelImageFilter: ChangeLabelImageFilter that maps labels to background pixels.
	changeLabelImageFilter is modified by this function. 
value: the number that determines the criteria that decides if a label should be removed.
return: void.
*/
void WatershedSegmentation::updateLabels(
	WatershedFilterType::OutputImageType::Pointer labelImage, 
	SegImageType::Pointer originalImage,
	ChangeLabelImageFilterType::Pointer changeLabelImageFilter,
	int labelValue = 0)
{
	// Get statistics on the original labelImage produced by watershed segmentation
	LabelStatisticsImageFilterType::Pointer labelStatisticsImageFilter = LabelStatisticsImageFilterType::New();
	labelStatisticsImageFilter->SetLabelInput(labelImage);
	labelStatisticsImageFilter->SetInput(originalImage);
	labelStatisticsImageFilter->Update();

	// typedefs for LabelStatisticsImageFilter for saving typing/space further down
	typedef LabelStatisticsImageFilterType::LabelPixelType LabelPixelType;
	typedef LabelStatisticsImageFilterType::ValidLabelValuesContainerType ValidLabelValuesType;

	// for testing
	std::cout << std::endl;
	std::cout << "Before number of labels: " << labelStatisticsImageFilter->GetNumberOfLabels() << std::endl;

	// Background pixel (black) to set removed labels to
	const WatershedFilterType::OutputImagePixelType background = 0;

	// Used to track total label stats (average count of all segments)
	long avgCount = 0;
	int labelMean = 0;

	// Iterate through the valid labels in the labelImage
	int labelCount = 0;
	for (ValidLabelValuesType::const_iterator vIt = labelStatisticsImageFilter->GetValidLabelValues().begin();
		vIt != labelStatisticsImageFilter->GetValidLabelValues().end();
		++vIt)
	{
		if (labelStatisticsImageFilter->HasLabel(*vIt)) {

			// Remove labels that have count less than the argument: labelValue
			LabelPixelType labelImageValue = *vIt;
			labelCount = labelStatisticsImageFilter->GetCount(labelImageValue);
			if (labelCount <= labelValue) {
				changeLabelImageFilter->SetChange(labelImageValue, background);
			}
			// Remove labels for dark regions (extra regions on the edge)
			
			labelMean = labelStatisticsImageFilter->GetMean(labelImageValue);
			if (labelMean < 5) {
				changeLabelImageFilter->SetChange(labelImageValue, background);
			}

			// For tracking total label stats
			avgCount += labelCount;
		}
	}
	changeLabelImageFilter->SetInput(labelStatisticsImageFilter->GetLabelInput());
	changeLabelImageFilter->Update();

	// for testing
	std::cout << "Average label count: " << avgCount / labelStatisticsImageFilter->GetNumberOfLabels() << std::endl;

	// Used to get statistics on new labelImage produced by changeLabelImageFilter
	LabelStatisticsImageFilterType::Pointer after_labelStatisticsImageFilter = LabelStatisticsImageFilterType::New();
	after_labelStatisticsImageFilter->SetLabelInput(changeLabelImageFilter->GetOutput());
	after_labelStatisticsImageFilter->SetInput(originalImage);
	after_labelStatisticsImageFilter->Update();

	// for testing
	std::cout << "After number of labels: " << after_labelStatisticsImageFilter->GetNumberOfLabels() << std::endl;
	std::cout << std::endl;
}