#include <iostream>
#include <opencv2\opencv.hpp>
/*---------------------------------------------------------------------*/
#include "Vignetting.h"


int main(int argc, char *argv[]) {

	cv::Mat src = cv::imread("./imgs/unShading.bmp", cv::IMREAD_COLOR);
	BayerLSCParameters(src);

	cv::Mat dst;
	BayerLSC(src, dst);
	
	cv::imshow("ORIGINAL", src);
	cv::imshow("RESULT", dst);
	
	cv::imwrite("./imgs/result.bmp", dst);

	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}