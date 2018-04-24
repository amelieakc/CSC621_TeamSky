#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <string>

#ifndef WATERSHEDSEGMENTATION_H
#define WATERSHEDSEGMENTATION_H

class WatershedSegmentation
{
private:
	typedef float SegPixelType;
	typedef itk::RGBPixel<unsigned char> RGBPixelType;
	typedef itk::Image<SegPixelType, 3> SegImageType;
	typedef itk::Image<RGBPixelType, 3> RGBImageType;
	typedef itk::ImageFileReader<SegImageType> SegReaderType;
	typedef itk::ImageFileWriter<RGBImageType> SegWriterType;

	char* filename;
	SegReaderType::Pointer seg_reader;
	SegWriterType::Pointer seg_writer;

public:
	//constructor
	WatershedSegmentation(char* file);

	/*
	Apply watershed segmentation to the specified image.
	threshold: threshold value that segmentation will use
	level: level value that segmentation will use
	return: Name of file where segmented image has been written to.
	Returns empty string, "", if an error occured.
	*/
	std::string applyWatershedSegmentation(float threshold, float level);
};

#endif