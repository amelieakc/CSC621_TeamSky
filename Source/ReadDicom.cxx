#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"

int readDicom(char* dir)
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
			std::cout << "No DICOMs in: " << dirName << std::endl;
			return EXIT_SUCCESS;
		}

		// Print series
		while (seriesItr != seriesEnd)
		{
			std::cout << seriesItr->c_str() << std::endl;
			++seriesItr;
		}

		seriesItr = seriesUID.begin();
		while (seriesItr != seriesUID.end())
		{
			std::string seriesIdentifier;
			seriesIdentifier = seriesItr->c_str();
			seriesItr++;

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
			std::string outFileName;

			outFileName = dirName + std::string("/") + "image_sequence.nrrd";

			writer->SetFileName(outFileName);
			writer->UseCompressionOn();
			writer->SetInput(reader->GetOutput());
			std::cout << "Writing: " << outFileName << std::endl;
			try
			{
				writer->Update();
			}
			catch (itk::ExceptionObject &ex)
			{
				std::cout << ex << std::endl;
				continue;
			}
		}
	}
	catch (itk::ExceptionObject &ex)
	{
		std::cout << ex << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}