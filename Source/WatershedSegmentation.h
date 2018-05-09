#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkWatershedImageFilter.h"
#include "itkChangeLabelImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"

#include <string>

#ifndef WATERSHEDSEGMENTATION_H
#define WATERSHEDSEGMENTATION_H

class WatershedSegmentation
{
private:
	typedef float SegPixelType; // used for the input image (3D greyscale dicom image)
	typedef itk::Image<SegPixelType, 3> SegImageType;

	typedef itk::RGBPixel<unsigned char> RGBPixelType; // used to color output image
	typedef itk::Image<RGBPixelType, 3> RGBImageType;

	typedef itk::ImageFileReader<SegImageType> SegReaderType;
	typedef itk::ImageFileWriter<RGBImageType> SegWriterType;
	typedef itk::WatershedImageFilter<SegImageType> WatershedFilterType;

	// Used to map labels to new labels
	typedef itk::ChangeLabelImageFilter
		<WatershedFilterType::OutputImageType, WatershedFilterType::OutputImageType> 
		ChangeLabelImageFilterType;

	// Used to get statistics on labelImages
	typedef itk::LabelStatisticsImageFilter
		<SegImageType, WatershedFilterType::OutputImageType> 
		LabelStatisticsImageFilterType;

	char* filename; // 3D greyscale dicom image file name
	SegReaderType::Pointer seg_reader; // For reading dcm files
	SegWriterType::Pointer seg_writer; // For writing dcm files

	/*
	Helper function that updates the labelImage produced by watershed segmentation.
	Used to remove labels that do not meet a specified criteria. 

	labelImage: label image produced by watershed segmentation.
	originalImage: image produced by gradient image filter.
	changeLabelImageFilter: ChangeLabelImageFilter that maps labels to background pixels.
		changeLabelImageFilter is modified by this function. 
	value: the number that determines the criteria that decides if a label should be removed. 
	return: void.
	*/
	void updateLabels(
		WatershedFilterType::OutputImageType::Pointer labelImage, 
		SegImageType::Pointer originalImage,
		ChangeLabelImageFilterType::Pointer changeLabelImageFilter,
		int labelValue);

public:
	//constructor
	WatershedSegmentation(char* file);

	/*
	Apply watershed segmentation to the specified image.

	threshold: threshold value that segmentation will use
	level: level value that segmentation will use
	labelValue: used to remove labels/segments that have a count less than labelValue
	return: Name of file where segmented image has been written to.
	Returns empty string, "", if an error occured.
	*/
	std::string applyWatershedSegmentation(float threshold, float level, int labelValue = 0);
};

#endif