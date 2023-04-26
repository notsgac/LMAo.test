#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Read the grayscale image
    Mat img = imread("color_image.jpg", IMREAD_GRAYSCALE);

    // Extract the bit planes and concatenate them horizontally
    vector<Mat> bit_planes(8);
    for (int i = 0; i < 8; i++) {
        Mat bit_plane = img & (1 << i);
        bit_plane.convertTo(bit_plane, CV_8UC1, 255);
        bit_planes[i] = bit_plane;
    }
    Mat combined_img;
    hconcat(bit_planes, combined_img);

    // Reconstruct the original image from the bit planes
    Mat reconstructed_img = Mat::zeros(img.size(), CV_8UC1);
    for (int i = 0; i < 8; i++) {
        Mat bit_plane = img & (1 << i);
        bitwise_or(reconstructed_img, bit_plane, reconstructed_img);
    }

    // Display the original image and the reconstructed image
    namedWindow("Original Image", WINDOW_NORMAL);
    namedWindow("Reconstructed Image", WINDOW_NORMAL);
    namedWindow("Combined Bit Planes", WINDOW_NORMAL);
    imshow("Combined Bit Planes", combined_img);
    imshow("Original Image", img);
    imshow("Reconstructed Image", reconstructed_img);
    waitKey(0);
    destroyAllWindows();

    return 0;
}
