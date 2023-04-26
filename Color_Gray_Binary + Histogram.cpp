#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void displayColorHistogram(const Mat& rHist, const Mat& gHist, const Mat& bHist, const string& title) {
    int histSize = rHist.rows;
    int histWidth = 512, histHeight = 400;
    int binWidth = cvRound((double)histWidth / histSize);

    Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0));

    // Normalize the histograms
    normalize(rHist, rHist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(gHist, gHist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(bHist, bHist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < histSize; i++) {
        // Draw red channel histogram
        line(histImage, Point(binWidth * (i - 1), histHeight - cvRound(rHist.at<float>(i - 1))),
            Point(binWidth * i, histHeight - cvRound(rHist.at<float>(i))),
            Scalar(255, 0, 0), 2, 8, 0);

        // Draw green channel histogram
        line(histImage, Point(binWidth * (i - 1), histHeight - cvRound(gHist.at<float>(i - 1))),
            Point(binWidth * i, histHeight - cvRound(gHist.at<float>(i))),
            Scalar(0, 255, 0), 2, 8, 0);

        // Draw blue channel histogram
        line(histImage, Point(binWidth * (i - 1), histHeight - cvRound(bHist.at<float>(i - 1))),
            Point(binWidth * i, histHeight - cvRound(bHist.at<float>(i))),
            Scalar(0, 0, 255), 2, 8, 0);
    }

    // Display the combined histogram
    imshow(title, histImage);
}
void displayHistogram(const Mat& hist, const string& title, Scalar color = Scalar(255, 255, 255)) {
    int histSize = hist.rows;
    int histWidth = 512, histHeight = 400;
    int binWidth = cvRound((double)histWidth / histSize);

    Mat histImage2(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0));
    normalize(hist, hist, 0, histImage2.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < histSize; i++) {
        line(histImage2, Point(binWidth * (i - 1), histHeight - cvRound(hist.at<float>(i - 1))),
            Point(binWidth * i, histHeight - cvRound(hist.at<float>(i))),
            color, 2, 8, 0);
    }

    // Display the histogram
    imshow(title, histImage2);
}

int main(int argc, char** argv) {
    // Load the color image
    Mat colorImage = imread("color_image.jpg", IMREAD_COLOR);

    if (colorImage.empty()) {
        cout << "Error loading image" << endl;
        return -1;
    }

    // Convert the color image to grayscale
    Mat grayImage;
    cvtColor(colorImage, grayImage, COLOR_BGR2GRAY);

    // Convert the grayscale image to binary using a threshold
    Mat binaryImage;
    threshold(grayImage, binaryImage, 128, 255, THRESH_BINARY);

    // Calculate histograms for the color image (R, G, B channels)
    vector<Mat> bgrChannels;
    split(colorImage, bgrChannels);

    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };

    Mat bHist, gHist, rHist, grayHist, binHist;
    calcHist(&bgrChannels[0], 1, 0, Mat(), bHist, 1, &histSize, &histRange);
    calcHist(&bgrChannels[1], 1, 0, Mat(), gHist, 1, &histSize, &histRange);
    calcHist(&bgrChannels[2], 1, 0, Mat(), rHist, 1, &histSize, &histRange);
    calcHist(&grayImage, 1, 0, Mat(), grayHist, 1, &histSize, &histRange);
    calcHist(&binaryImage, 1, 0, Mat(), binHist, 1, &histSize, &histRange);
    // Display the images
    imshow("Color Image", colorImage);
    imshow("Grayscale Image", grayImage);
    imshow("Binary Image", binaryImage);

    // Display the combined histogram
    displayColorHistogram(rHist, gHist, bHist, "Color Image Histogram");
    displayHistogram(grayHist, "Grayscale Image Histogram");
    displayHistogram(binHist, "Binary Image Histogram");

    waitKey(0);
    return 0;
}