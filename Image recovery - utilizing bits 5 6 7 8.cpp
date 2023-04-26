#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("color_image.jpg", IMREAD_COLOR);

    if (img.empty())
    {
        cout << "Could not read the image: " << endl;
        return 1;
    }

    Mat bgr_channels[3];
    split(img, bgr_channels);

    Mat recovered_img = Mat::zeros(img.size(), CV_8UC3);

    // Recovering blue channel
    Mat bp_channel_b = Mat::zeros(img.size(), CV_8UC1);
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            uchar pixel_value = 0;
            for (int i = 4; i < 8; i++)
            {
                pixel_value |= ((bgr_channels[0].at<uchar>(r, c) & (1 << i)) >> i) << (7 - i);
            }
            bp_channel_b.at<uchar>(r, c) = pixel_value;
        }
    }

    // Recovering green channel
    Mat bp_channel_g = Mat::zeros(img.size(), CV_8UC1);
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            uchar pixel_value = 0;
            for (int i = 4; i < 8; i++)
            {
                pixel_value |= ((bgr_channels[1].at<uchar>(r, c) & (1 << i)) >> i) << (7 - i);
            }
            bp_channel_g.at<uchar>(r, c) = pixel_value;
        }
    }

    // Recovering red channel
    Mat bp_channel_r = Mat::zeros(img.size(), CV_8UC1);
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            uchar pixel_value = 0;
            for (int i = 4; i < 8; i++)
            {
                pixel_value |= ((bgr_channels[2].at<uchar>(r, c) & (1 << i)) >> i) << (7 - i);
            }
            bp_channel_r.at<uchar>(r, c) = pixel_value;
        }
    }

    // Merge the recovered channels into a single image
    vector<Mat> channels;
    channels.push_back(bp_channel_b);
    channels.push_back(bp_channel_g);
    channels.push_back(bp_channel_r);
    merge(channels, recovered_img);

    namedWindow("Recovered Image", WINDOW_AUTOSIZE);
    imshow("Recovered Image", recovered_img);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
