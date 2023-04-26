#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void displayHistogram(const string& windowName, const Mat& image)
{
    // Split the image into BGR planes
    vector<Mat> bgr_planes;
    split(image, bgr_planes);

    // Set histogram parameters
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;

    // Compute the histograms for each color channel
    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

    // Set histogram image parameters
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    // Normalize the histograms
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    // Draw the histograms
    for (int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
            Point(bin_w * i, hist_h - cvRound(b_hist.at<float>(i))),
            Scalar(255, 0, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
            Point(bin_w * i, hist_h - cvRound(g_hist.at<float>(i))),
            Scalar(0, 255, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
            Point(bin_w * i, hist_h - cvRound(r_hist.at<float>(i))),
            Scalar(0, 0, 255), 2, 8, 0);
    }

    // Display the histogram
    imshow(windowName, histImage);
}

int main(int argc, int argv)
{
    // Read the input image
    Mat image = imread("color_image.jpg", IMREAD_COLOR);

    // Display the original image and its histogram
    imshow("Original Image", image);
    displayHistogram("Original Image Histogram", image);

    // Apply gamma correction for each gamma value and display the results
    for (double gamma : { 3.0, 4.0, 5.0})
    {
        // Compute the inverse gamma value
        double inverse_gamma = 1 / gamma;

        // Compute the lookup table for the gamma correction
        Mat lut_matrix(1, 256, CV_8UC1);
        uchar* ptr = lut_matrix.ptr();
        for (int i = 0; i < 256; i++)
        {
            ptr[i] = (int)(pow((double)i / 255.0, inverse_gamma) * 255.0);
        }

        // Apply the gamma correction to the image
        Mat imageGammaCorrected;
        LUT(image, lut_matrix, imageGammaCorrected);

        // Display the gamma-corrected image and its histogram
        stringstream windowTitle;
        windowTitle << "Gamma: " << gamma;
        imshow(windowTitle.str(), imageGammaCorrected);
        displayHistogram(windowTitle.str() + " Histogram", imageGammaCorrected);
    }

    // Wait for a key press to exit
    waitKey(0);

    return 0;
}
