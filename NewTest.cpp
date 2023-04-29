#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const int HIST_SIZE = 256;

Mat HE(const Mat& src) {
    // Tạo mảng để lưu trữ input image
    int hist[HIST_SIZE] = { 0 };

    // Lặp qua từng pixel trong input image và tăng giá trị tương ứng trong histogram
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            hist[src.at<uchar>(y, x)]++;
        }
    }

    // Tạo mảng để lưu trữ CDF(Cumulative Distribution Function)(Hàm phân phối tích lũy)
    int cdf[HIST_SIZE] = { 0 };
    // Đặt giá trị đầu tiên của CDF bằng giá trị đầu tiên của histogram
    cdf[0] = hist[0];
    // Tính CDF bằng cách cộng giá trị hiện tại với giá trị trước đó
    for (int i = 1; i < HIST_SIZE; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    // Tạo mảng để lưu trữ CDF chuẩn hóa
    float cdf_normalized[HIST_SIZE]{};
    // Tính tổng số pixel
    float totalPixels = src.rows * src.cols;
    // Chia giá trị CDF cho tổng số pixel để chuẩn hóa
    for (int i = 0; i < HIST_SIZE; i++) {
        cdf_normalized[i] = cdf[i] / totalPixels;
    }

    // Tạo mảng để lưu trữ LUT(Look Up Table)
    uchar lut[HIST_SIZE]{};
    // Tính giá trị LUT bằng cách nhân giá trị CDF chuẩn hóa với 255 và làm tròn
    for (int i = 0; i < HIST_SIZE; i++) {
        lut[i] = round(cdf_normalized[i] * 255);
    }

    // Tạo mảng để lưu trữ output image
    Mat dst(src.size(), src.type());
    // Lặp qua từng pixel trong input image và gán giá trị tương ứng từ LUT vào output image
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            dst.at<uchar>(y, x) = lut[src.at<uchar>(y, x)];
        }
    }

    // Return the equalized image
    return dst;
}

int main() {
    // Load the image
    Mat image = imread("color_image.jpg", IMREAD_COLOR);

    if (image.empty()) {
        cout << "Could not open or find the image!\n";
        return -1;
    }

    // Split the image into its color channels
    vector<Mat> bgr_planes;
    split(image, bgr_planes);

    // Equalize the histogram of each color channel using the custom histogram equalization algorithm
    for (int i = 0; i < 3; i++) {
        bgr_planes[i] = HE(bgr_planes[i]);
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

    namedWindow("Equalized Image", WINDOW_NORMAL);
    imshow("Equalized Image", equalized_image);
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
    namedWindow("Original Histograms", WINDOW_NORMAL);
    imshow("Original Histograms", histImage);

    namedWindow("Equalized Histograms", WINDOW_NORMAL);
    imshow("Equalized Histograms", histImage_HE);
    waitKey();
    return 0;
}
