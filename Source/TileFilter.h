#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTileImageFilter.h"

#include <string>

#ifndef TILEFILTER_H
#define TILEFILTER_H

class TileFilter {
private:
	typedef unsigned char VolPixelType;
	typedef itk::Image<VolPixelType, 2> VolInputImageType;
	typedef itk::Image<VolPixelType, 3> VolOutputImageType;
	typedef itk::ImageFileReader<VolInputImageType> VolReaderType;
	typedef itk::ImageFileWriter<VolOutputImageType> VolWriterType;
	typedef itk::TileImageFilter <VolInputImageType, VolOutputImageType> TilerType;

	char* image_dir;
	int num_files;
	VolReaderType::Pointer vol_reader;
	VolWriterType::Pointer vol_writer;
	TilerType::Pointer tiler;

public:
	// constructor
	TileFilter(char* dir, int num);

	/*
	Create a 3D image(stack of 2D images) from set of 2D image files.
	dir: Directory where images are located
	num: Number of files to form 2D stack
	return: Name of file where 3D image has been written to.
	Returns empty string, "", if an error occured.
	*/
	std::string createVolume();
};

#endif