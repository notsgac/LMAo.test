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

    // Recover the original color image using the 4 most significant bits of each channel
    Mat recovered_img = Mat::zeros(img.size(), CV_8UC3);
    resize(bp_img, bp_img, Size(800, 300), 0, 0, INTER_AREA);
    // Recover the blue channel
    Mat bp_channel_b = Mat::zeros(img.size(), CV_8UC1);
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            uchar pixel_value = 0;
            for (int i = 4; i < 8; i++)
            {
                // Get the value of bit i from pixel (r, c) of the blue channel and set it as the (i-4)th bit of the corresponding pixel in the recovered blue channel
                pixel_value |= ((bgr_channels[0].at<uchar>(r, c) & (1 << i)) >> (i - 4)) << (7 - i);
            }
            bp_channel_b.at<uchar>(r, c) = pixel_value;
        }
    }

    // Recover the green channel
    Mat bp_channel_g = Mat::zeros(img.size(), CV_8UC1);
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            uchar pixel_value = 0;
            for (int i = 4; i < 8; i++)
            {
                // Get the value of bit i from pixel (r, c) of the green channel and set it as the (i-4)th bit of the corresponding pixel in the recovered green channel
                pixel_value |= ((bgr_channels[1].at<uchar>(r, c) & (1 << i)) >> (i - 4)) << (7 - i);
            }
            bp_channel_g.at<uchar>(r, c) = pixel_value;
        }
    }

    // Recover the red channel
    Mat bp_channel_r = Mat::zeros(img.size(), CV_8UC1);
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            uchar pixel_value = 0;
            for (int i = 4; i < 8; i++)
            {
                // Get the value of bit i from pixel (r, c) of the red channel and set it as the (i-4)th bit of the corresponding pixel in the recovered red channel
                pixel_value |= ((bgr_channels[2].at<uchar>(r, c) & (1 << i)) >> (i - 4)) << (7 - i);
            }
            bp_channel_r.at<uchar>(r, c) = pixel_value;
        }
    }

    // Merge the recovered color channels to form the final recovered color image
    vector<Mat> recovered_channels;
    recovered_channels.push_back(bp_channel_b);
    recovered_channels.push_back(bp_channel_g);
    recovered_channels.push_back(bp_channel_r);
    merge(recovered_channels, recovered_img);
    // Display the original color image, the bit-plane sliced image, and the recovered images
    
    namedWindow("94", WINDOW_NORMAL);
    namedWindow("Bit-plane sliced", WINDOW_NORMAL);
    resizeWindow("Bit-plane sliced", 800, 600); // resize the window to 800x600
    namedWindow("Bit 5-8", WINDOW_NORMAL);
    namedWindow("All bits", WINDOW_NORMAL);

    imshow("94", img);
    imshow("Bit-plane sliced", bp_img);
    imshow("Bit 5-8", recovered_img);

    // Recover the original color image using all 8 bits of each channel
    Mat recovered_img_all = Mat::zeros(img.size(), CV_8UC3);

    // Recover the blue channel using all 8 bits
    bp_channel_b = Mat::zeros(img.size(), CV_8UC1);
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            uchar pixel_value = 0;
            for (int i = 0; i < 8; i++)
            {
                // Get the value of bit i from pixel (r, c) of the blue channel and set it as the ith bit of the corresponding pixel in the recovered blue channel
                pixel_value |= ((bgr_channels[0].at<uchar>(r, c) & (1 << i)) >> i) << (7 - i);
            }
            bp_channel_b.at<uchar>(r, c) = pixel_value;
        }
    }

    // Recover the green channel using all 8 bits
    bp_channel_g = Mat::zeros(img.size(), CV_8UC1);
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            uchar pixel_value = 0;
            for (int i = 0; i < 8; i++)
            {
                // Get the value of bit i from pixel (r, c) of the green channel and set it as the ith bit of the corresponding pixel in the recovered green channel
                pixel_value |= ((bgr_channels[1].at<uchar>(r, c) & (1 << i)) >> i) << (7 - i);
            }
            bp_channel_g.at<uchar>(r, c) = pixel_value;
        }
    }

    // Recover the red channel using all 8 bits
    bp_channel_r = Mat::zeros(img.size(), CV_8UC1);
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            uchar pixel_value = 0;
            for (int i = 0; i < 8; i++)
            {
                // Get the value of bit i from pixel (r, c) of the red channel and set it as the ith bit of the corresponding pixel in the recovered red channel
                pixel_value |= ((bgr_channels[2].at<uchar>(r, c) & (1 << i)) >> i) << (7 - i);
            }
            bp_channel_r.at<uchar>(r, c) = pixel_value;
        }
    }

    // Merge the recovered color channels to form the final recovered color image using all 8 bits of each channel
    recovered_channels.clear();
    recovered_channels.push_back(bp_channel_b);
    recovered_channels.push_back(bp_channel_g);
    recovered_channels.push_back(bp_channel_r);
    merge(recovered_channels, recovered_img_all);
    imshow("All bits", recovered_img_all);

    // Wait for a key press
    waitKey(0);

    return 0;
}

