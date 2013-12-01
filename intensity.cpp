#include <opencv2/opencv.hpp>
#include "intensity.h"

using namespace cv;

int getAverageIntensity(cv::Mat& img)
{
    int intensity = 0;

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            intensity += img.at<unsigned char>(i,j);
        }
    }

    intensity /= (img.rows*img.cols);
    return intensity;
}

int round_to_precision(double num, int precision)
{
    long candidate = round(num);
    if (candidate%precision)
    {
    return ((candidate/precision)+((candidate%precision)>=precision/2?1:0))*precision;
    }
    return candidate;
}
