#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    // Load the image
    Mat image = imread("color_image.jpg", IMREAD_COLOR);

    if (image.empty())
    {
        cout << "Could not open or find the image!\n";
        return -1;
    }

    // Split the image into its color channels
    vector<Mat> bgr_planes;
    split(image, bgr_planes);

    // Equalize the histogram of each color channel
    for (int i = 0; i < 3; i++)
    {
        equalizeHist(bgr_planes[i], bgr_planes[i]);
    }

    // Merge the equalized color channels back into a single image
    Mat equalized_image;
    merge(bgr_planes, equalized_image);

    // Compute histograms of the original and equalized images for each color channel
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true, accumulate = false;

    Mat b_hist_original, g_hist_original, r_hist_original;
    Mat b_hist_equalized, g_hist_equalized, r_hist_equalized;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist_original, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist_original, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist_original, 1, &histSize, &histRange, uniform, accumulate);

    // Compute histograms of the histogram equalized image for each color channel
    Mat b_hist_equalized_HE, g_hist_equalized_HE, r_hist_equalized_HE;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist_equalized_HE, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist_equalized_HE, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist_equalized_HE, 1, &histSize, &histRange, uniform, accumulate);

    // Display the original and equalized images
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", image);

    namedWindow("Histogram Equalized Image", WINDOW_NORMAL);
    imshow("Histogram Equalized Image", equalized_image);

    // Display histograms for each color channel of the original image
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);

    Mat histImageB(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    Mat histImageG(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    Mat histImageR(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    normalize(b_hist_original, b_hist_original, 0, histImageB.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist_original, g_hist_original, 0, histImageG.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist_original, r_hist_original, 0, histImageR.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < histSize; i++)
    {
        line(histImageB, Point(bin_w * (i - 1), hist_h - cvRound(b_hist_original.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(b_hist_original.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
        line(histImageG, Point(bin_w * (i - 1), hist_h - cvRound(g_hist_original.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(g_hist_original.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
        line(histImageR, Point(bin_w * (i - 1), hist_h - cvRound(r_hist_original.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(r_hist_original.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
    }
    // Display histograms for each color channel of the histogram equalized image
    Mat histImageB_HE(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    Mat histImageG_HE(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    Mat histImageR_HE(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    normalize(b_hist_equalized_HE, b_hist_equalized_HE, 0, histImageB_HE.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist_equalized_HE, g_hist_equalized_HE, 0, histImageG_HE.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist_equalized_HE, r_hist_equalized_HE, 0, histImageR_HE.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < histSize; i++)
    {
        line(histImageB_HE, Point(bin_w * (i - 1), hist_h - cvRound(b_hist_equalized_HE.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(b_hist_equalized_HE.at<float>(i))), Scalar(255, 255, 0), 2, 8, 0);
        line(histImageG_HE, Point(bin_w * (i - 1), hist_h - cvRound(g_hist_equalized_HE.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(g_hist_equalized_HE.at<float>(i))), Scalar(0, 255, 255), 2, 8, 0);
        line(histImageR_HE, Point(bin_w * (i - 1), hist_h - cvRound(r_hist_equalized_HE.at<float>(i - 1))), Point(bin_w * (i), hist_h - cvRound(r_hist_equalized_HE.at<float>(i))), Scalar(255, 0, 255), 2, 8, 0);
    }
    // Concatenate the histogram images horizontally and display in a new window
    Mat histImage;
    hconcat(histImageB, histImageG, histImage);
    hconcat(histImage, histImageR, histImage);

    Mat histImage_HE;
    hconcat(histImageB_HE, histImageG_HE, histImage_HE);
    hconcat(histImage_HE, histImageR_HE, histImage_HE);

    namedWindow("Histogram Original", WINDOW_NORMAL);
    imshow("Histogram Original", histImage);

    namedWindow("Histogram Equalized", WINDOW_NORMAL);
    imshow("Histogram Equalized", histImage_HE);

    waitKey();
    return 0;
}
