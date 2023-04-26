#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("94.bmp", IMREAD_COLOR);

    if (img.empty())
    {
        cout << "Could not read the image: " << endl;
        return 1;
    }

    Mat bgr_channels[3];
    split(img, bgr_channels);

    int width = img.cols;
    int height = img.rows;

    Mat bp_img_b = Mat::zeros(Size(8 * width, height), CV_8UC1);
    Mat bp_img_g = Mat::zeros(Size(8 * width, height), CV_8UC1);
    Mat bp_img_r = Mat::zeros(Size(8 * width, height), CV_8UC1);

    // Bit-plane slicing for blue channel
    for (int i = 0; i < 8; i++)
    {
        Mat bp_channel = Mat::zeros(img.size(), CV_8UC1);
        for (int r = 0; r < img.rows; r++)
        {
            for (int c = 0; c < img.cols; c++)
            {
                bp_channel.at<uchar>(r, c) = (bgr_channels[0].at<uchar>(r, c) & (1 << i)) > 0 ? 255 : 0;
            }
        }
        Mat roi(bp_img_b, Rect(i * width, 0, width, height));
        bp_channel.copyTo(roi);
    }

    // Bit-plane slicing for green channel
    for (int i = 0; i < 8; i++)
    {
        Mat bp_channel = Mat::zeros(img.size(), CV_8UC1);
        for (int r = 0; r < img.rows; r++)
        {
            for (int c = 0; c < img.cols; c++)
            {
                bp_channel.at<uchar>(r, c) = (bgr_channels[1].at<uchar>(r, c) & (1 << i)) > 0 ? 255 : 0;
            }
        }
        Mat roi(bp_img_g, Rect(i * width, 0, width, height));
        bp_channel.copyTo(roi);
    }

    // Bit-plane slicing for red channel
    for (int i = 0; i < 8; i++)
    {
        Mat bp_channel = Mat::zeros(img.size(), CV_8UC1);
        for (int r = 0; r < img.rows; r++)
        {
            for (int c = 0; c < img.cols; c++)
            {
                bp_channel.at<uchar>(r, c) = (bgr_channels[2].at<uchar>(r, c) & (1 << i)) > 0 ? 255 : 0;
            }
        }
        Mat roi(bp_img_r, Rect(i * width, 0, width, height));
        bp_channel.copyTo(roi);
    }

    Mat bp_img;
    vector<Mat> bp_channels;
    bp_channels.push_back(bp_img_b);
    bp_channels.push_back(bp_img_g);
    bp_channels.push_back(bp_img_r);
    merge(bp_channels, bp_img);

    namedWindow("Bit Planes", WINDOW_AUTOSIZE);
    imshow("Bit Planes", bp_img);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
