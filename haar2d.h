#ifndef HAAR2D_H
#define HAAR2D_H

void haar1d(float *vec, int n, int w);
void haar2d(cv::Mat& image, int rows, int cols);
cv::Mat createHaar2D(int Dimension);

#endif // HAAR2D_H
