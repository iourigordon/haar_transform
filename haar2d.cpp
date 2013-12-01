#include <opencv2/opencv.hpp>
#include "haar2d.h"

void haar1d(float *vec, int n, int w)
{
    int i=0;
    float *vecp = new float[n];
    for(i=0;i<n;i++)
        vecp[i] = 0;

    w/=2;
    for(i=0;i<w;i++)
    {
        vecp[i] = (vec[2*i] + vec[2*i+1])/2;
        vecp[i+w] = (vec[2*i] - vec[2*i+1])/2;
        //cout << vecp[i] << endl;
    }

    for(i=0;i<(w*2);i++)
        vec[i] = vecp[i];

    delete [] vecp;
}


/** The 2D Haar Transform **/
void haar2d(cv::Mat& image, int rows, int cols)
{
    int count = 1;
    float *temp_row = new float[cols];
    float *temp_col = new float[rows];

    int i=0,j=0;
    int w = cols, h=rows;

    while((w>1 || h>1)/* && count*/)
    {
        if(w>1)
        {
            for(i=0;i<h;i++)
            {
                for(j=0;j<cols;j++)
                    temp_row[j] = image.at<double>(i,j);

                haar1d(temp_row,cols,w);

                for(j=0;j<cols;j++)
                    image.at<double>(i,j) = temp_row[j];
            }
        }
        if(h>1)
        {
            for(i=0;i<w;i++)
            {
                for(j=0;j<rows;j++)
                    temp_col[j] = image.at<double>(j,i);
                haar1d(temp_col, rows, h);
                for(j=0;j<rows;j++)
                    image.at<double>(j,i) = temp_col[j];
            }
        }
        if(w>1)
            w/=2;
        if(h>1)
            h/=2;
        count--;
    }
    delete [] temp_row;
    delete [] temp_col;
}


cv::Mat createHaar2D(int Dimension)
{
    cv::Mat haar(Dimension,Dimension,CV_64FC1);
    int pos;
    double val;

    for (int i = 0; i < Dimension; i++)
    {
        for (int j = 0; j < Dimension; j++)
        {
            if (i<(Dimension/2))
            {
                pos=i,val=sqrt(2)/2;
                //((j == pos*2) ||(j==(pos*2)+1))?haar.at<double>(i,j)=val:haar.at<double>(i,j) = 0;
                ((j == pos*2) ||(j==(pos*2+1)))?haar.at<double>(i,j)=val:haar.at<double>(i,j) = 0;
            }
            else
            {
                pos=i-(Dimension/2),val=sqrt(2)/2;
                if (j == pos*2)
                    haar.at<double>(i,j)=-val;
                else if (j==(pos*2+1))
                    haar.at<double>(i,j)=val;
                else
                    haar.at<double>(i,j) = 0;
            }
        }
    }
    return haar;
}
