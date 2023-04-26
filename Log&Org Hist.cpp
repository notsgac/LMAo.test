#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    // read the first image
    Mat image1 = imread("cute.jpg",IMREAD_COLOR);

    // split the first image into 3 channels
    vector<Mat> bgr1;
    split(image1, bgr1);

    // set histogram parameters for the first image
    int histSize = 256; // number of bins
    float range[] = { 0, 256 }; // range of pixel values
    const float* histRange = { range };
    bool uniform = true; // histogram is uniform
    bool accumulate = false; // histogram is not accumulated

    // calculate histograms for each channel of the first image
    Mat b_hist1, g_hist1, r_hist1;
    calcHist(&bgr1[0], 1, 0, Mat(), b_hist1, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr1[1], 1, 0, Mat(), g_hist1, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr1[2], 1, 0, Mat(), r_hist1, 1, &histSize, &histRange, uniform, accumulate);

    // set histogram display parameters for the first image
    int histWidth = 256; // width of histogram bars
    int histHeight = 200; // height of histogram bars
    int binWidth = cvRound((double)histWidth / histSize); // width of each bin

    // create an image to display histograms and images for the first image
    Mat histImage1(histHeight, histWidth * 3, CV_8UC3, Scalar(0));

    // normalize histograms of the first image to fit in the display window
    normalize(b_hist1, b_hist1, 0, histImage1.rows, NORM_MINMAX);
    normalize(g_hist1, g_hist1, 0, histImage1.rows, NORM_MINMAX);
    normalize(r_hist1, r_hist1, 0, histImage1.rows, NORM_MINMAX);

    // draw histograms for each channel of the first image
    for (int i = 1; i < histSize; i++)
    {
        line(histImage1,
            Point(binWidth * (i - 1), histHeight - cvRound(b_hist1.at<float>(i - 1))),
            Point(binWidth * (i), histHeight - cvRound(b_hist1.at<float>(i))),
            Scalar(255, 0, 0), 2);
        line(histImage1,
            Point(binWidth * (i - 1) + histWidth, histHeight - cvRound(g_hist1.at<float>(i - 1))),
            Point(binWidth * (i)+histWidth, histHeight - cvRound(g_hist1.at<float>(i))),
            Scalar(0, 255, 0), 2);
        line(histImage1,
            Point(binWidth * (i - 1) + 2 * histWidth, histHeight - cvRound(r_hist1.at<float>(i - 1))),
            Point(binWidth * (i)+2 * histWidth, histHeight - cvRound(r_hist1.at<float>(i))),
            Scalar(0, 0, 255), 2);
    }
    // resize the histograms for the first image
    Mat histImageResized1;
    resize(histImage1, histImageResized1, Size(histImage1.cols / 2, histImage1.rows / 2));

    // read the second image
    Mat image2 = imread("log.jpg");

    // split the second image into 3 channels
    vector<Mat> bgr2;
    split(image2, bgr2);

    // set histogram parameters for the second image
    Mat b_hist2, g_hist2, r_hist2;
    calcHist(&bgr2[0], 1, 0, Mat(), b_hist2, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr2[1], 1, 0, Mat(), g_hist2, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr2[2], 1, 0, Mat(), r_hist2, 1, &histSize, &histRange, uniform, accumulate);

    // create an image to display histograms and images for the second image
    Mat histImage2(histHeight, histWidth * 3, CV_8UC3, Scalar(0));

    // normalize histograms of the second image to fit in the display window
    normalize(b_hist2, b_hist2, 0, histImage2.rows, NORM_MINMAX);
    normalize(g_hist2, g_hist2, 0, histImage2.rows, NORM_MINMAX);
    normalize(r_hist2, r_hist2, 0, histImage2.rows, NORM_MINMAX);

    // draw histograms for each channel of the second image
    for (int i = 1; i < histSize; i++)
    {
        line(histImage2,
            Point(binWidth * (i - 1), histHeight - cvRound(b_hist2.at<float>(i - 1))),
            Point(binWidth * (i), histHeight - cvRound(b_hist2.at<float>(i))),
            Scalar(255, 0, 0), 2);
        line(histImage2,
            Point(binWidth * (i - 1) + histWidth, histHeight - cvRound(g_hist2.at<float>(i - 1))),
            Point(binWidth * (i)+histWidth, histHeight - cvRound(g_hist2.at<float>(i))),
            Scalar(0, 255, 0), 2);
        line(histImage2,
            Point(binWidth * (i - 1) + 2 * histWidth, histHeight - cvRound(r_hist2.at<float>(i - 1))),
            Point(binWidth * (i)+2 * histWidth, histHeight - cvRound(r_hist2.at<float>(i))),
            Scalar(0, 0, 255), 2);
    }

   // resize the histograms for the second image
    Mat histImageResized2;
    resize(histImage2, histImageResized2, Size(histImage2.cols / 2, histImage2.rows / 2));
  
    // display histograms
    namedWindow("Cute Histograms", WINDOW_AUTOSIZE);
    imshow("Cute Histograms", histImage1);
    namedWindow("LogTrans Histograms", WINDOW_AUTOSIZE);
    imshow("LogTrans Histograms", histImage2);

    // resize the images
    Mat imageResized1, imageResized2;
    resize(image1, imageResized1, Size(image1.cols / 2, image1.rows / 2));
    resize(image2, imageResized2, Size(image2.cols / 2, image2.rows / 2));

    // display images
    namedWindow("Cute", WINDOW_AUTOSIZE);
    imshow("Cute", imageResized1);
    namedWindow("LogTrans", WINDOW_AUTOSIZE);
    imshow("LogTrans", imageResized2);

    // wait for a key press
    waitKey(0);

    return 0;
}

    