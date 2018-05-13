#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include <string>

/*
Create a 3D image from set of 2D dicom image files.
dir: Directory where images are located
return: Name of file where 3D image has been written to.
Returns empty string, "", if an error occured.
*/
std::string readDicom(char* dir)
{
	std::string dirName = dir; // dicom directory

	//using PixelType = signed short;
	using PixelType = unsigned char;
	constexpr unsigned int Dimension = 3;
	using ImageType = itk::Image< PixelType, Dimension >;

	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->AddSeriesRestriction("0008|0021");
	nameGenerator->SetGlobalWarningDisplay(false);
	nameGenerator->SetDirectory(dirName);

	try
	{
		using SeriesIdContainer = std::vector< std::string >;
		const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
		auto seriesItr = seriesUID.begin();
		auto seriesEnd = seriesUID.end();

		// Check if there are any valid DICOM images in the directory
		if (seriesItr == seriesEnd)
		{
			std::cout << "No DICOM images found in: " << dirName << std::endl;
			return "";
		}

		// Print series
		while (seriesItr != seriesEnd)
		{
			std::cout << seriesItr->c_str() << std::endl;
			++seriesItr;
		}

		// Read the first DICOM series found
		seriesItr = seriesUID.begin();
		std::string seriesIdentifier;
		seriesIdentifier = seriesItr->c_str();
		//seriesItr++;

		using FileNamesContainer = std::vector< std::string >;
		FileNamesContainer fileNames =
			nameGenerator->GetFileNames(seriesIdentifier);

		using ReaderType = itk::ImageSeriesReader< ImageType >;
		ReaderType::Pointer reader = ReaderType::New();
		using ImageIOType = itk::GDCMImageIO;
		ImageIOType::Pointer dicomIO = ImageIOType::New();
		reader->SetImageIO(dicomIO);
		reader->SetFileNames(fileNames);
		reader->Update();

		using WriterType = itk::ImageFileWriter< ImageType >;
		WriterType::Pointer writer = WriterType::New();
		std::string outfile;

		outfile = dirName + std::string("/") + "image_sequence.nrrd";

		writer->SetFileName(outfile);
		writer->UseCompressionOn();
		writer->SetInput(reader->GetOutput());
		std::cout << "Writing: " << outfile << std::endl;
		try
		{
			writer->Update();
		}
		catch (itk::ExceptionObject& e)
		{
			std::cout << e << std::endl;
			std::cerr << "ERROR: Could not create file: " << outfile << std::endl;
			return "";
		}
		return outfile;
	}
	catch (itk::ExceptionObject& e)
	{
		std::cerr << e << std::endl;
		std::cerr << "ERROR: Could not create 3D image" << std::endl;
		return "";
	}
	return EXIT_SUCCESS;
}