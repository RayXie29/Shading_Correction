#pragma once

#include <stdio.h>
#include <opencv2\opencv.hpp>

void BayerLSC(cv::Mat &src, cv::Mat &Dst);
void BayerLSCParameters(cv::Mat &src);