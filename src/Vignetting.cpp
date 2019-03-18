#include "Vignetting.h"
#include <fstream>
#include <string>
#include <iostream>

void BayerLSCParameters(cv::Mat &src)
{
	int i, j;
	int width = src.cols;
	int height = src.rows;
	int frameSize = width * height;
	int channels = src.channels();

	// central point of frame
	int halfWidth = width / 2 + 0.5, halfHeight = height / 2 + 0.5;

	// P=AR =====> A = PR+
	cv::Mat PixelValues(1, frameSize, CV_32FC1); // 1XN
	cv::Mat FunctionParameter(1, 3, CV_32FC1); // 1X3
	cv::Mat PixelDistance(3, frameSize, CV_32FC1, cv::Scalar(1)); //3XN

	float *PValPtr = PixelValues.ptr<float>(0);
	float *R0 = PixelDistance.ptr<float>(0);
	float *R1 = PixelDistance.ptr<float>(1);


	for (i = 0; i < height; ++i)
	{
		uchar *SrcPtr = src.ptr<uchar>(i);
		int BaseFrameIndex = i * width;
		int ydist = (i - halfHeight) * (i - halfHeight);
		for (j = 0; j < width; ++j)
		{
			// loading pixel values
			int BaseJIndex = j * channels;
			for (int ch = 0; ch < channels; ++ch) { PValPtr[BaseFrameIndex + j] += SrcPtr[BaseJIndex + ch]; }
			//calculate distance between pixel(i,j) to central pixel(height/2,width/2)
			int distance = ydist + (j - halfWidth)*(j - halfWidth);
			R0[BaseFrameIndex + j] = distance;
			R1[BaseFrameIndex + j] = distance * distance;
		}
	}

	cv::Mat PixelDistanceT = PixelDistance.t(); //Nx3
	cv::Mat temp = PixelDistance * PixelDistanceT;

	cv::Mat PseudoPD = PixelDistanceT * temp.inv();
	FunctionParameter = PixelValues * PseudoPD;

	float *FPPtr = FunctionParameter.ptr<float>(0);
	std::string filename = std::string("./ref/ShadingParameters.csv");
	std::fstream ParameterFile(filename.c_str(), std::ios::out);

	ParameterFile << width << "," << height << "\n";
	ParameterFile << FPPtr[0] << "," << FPPtr[1] << "," << FPPtr[2];


	ParameterFile.close();
}

void BayerLSC(cv::Mat &src, cv::Mat &Dst)
{
	int i = 0, j = 0;
	std::string filename = std::string("/ref/ShadingParameters.csv");
	std::fstream ShadingParametets(filename.c_str(), std::ios::in);

	if (!ShadingParametets)
	{
		std::cerr << "open the file error" << std::endl;
		exit(EXIT_FAILURE);
	}

	double parameters[5] = { 0 };
	std::string line;

	j = 0;
	while (getline(ShadingParametets, line))
	{
		std::istringstream templine(line);
		std::string data;
		while (getline(templine, data, ',')) { parameters[j++] = atof(data.c_str()); }
	}

	ShadingParametets.close();

	int width = src.cols;
	int height = src.rows;
	int halfWidth = width / 2 + 0.5, halfHeight = height / 2 + 0.5;
	int frameSize = width * height;

	//We should check the input image resoultion to confirm that the parameters is blong to this device
	if ((int)parameters[0] != width || (int)parameters[1] != height)
	{
		std::cerr << "Frame size is different to frame size in ShadingParameters file" << std::endl;
		exit(EXIT_FAILURE);
	}
	int channels = src.channels();
	Dst = cv::Mat(src.size(), src.type());
	
	for (i = 0; i < height; ++i)
	{
		uchar *DstPtr = Dst.ptr<uchar>(i);
		uchar *SrcPtr = src.ptr<uchar>(i);
		int ydist = (i - halfHeight)*(i - halfHeight);
		for (j = 0; j < width; ++j)
		{
			int BaseJIndex = j * channels;
			int r = ydist + (j - halfWidth)*(j - halfWidth);
			double functionValue = parameters[4] / (parameters[2] * r + parameters[3] * r * r + parameters[4]);
			for (int ch = 0; ch < channels; ++ch)
			{
				DstPtr[BaseJIndex + ch] = cv::saturate_cast<uchar>(functionValue*SrcPtr[BaseJIndex + ch]);
			}
		}
	}
	

}
