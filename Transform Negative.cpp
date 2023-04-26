#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main(int argc, char** argv) {

    // Read the image
    Mat grayscale_image = imread("Grayscale_Image.jpg", IMREAD_GRAYSCALE);
    Mat color_image = imread("color_image.jpg", IMREAD_COLOR);
    // Transforming Grayscale Image to Negatives
    Mat negative_image2;
    bitwise_not(grayscale_image, negative_image2);
    // Transforming Color Image to Negatives
    Mat negative_image;
    bitwise_not(color_image, negative_image);
    // Display the images
    imshow("Grayscale Image", grayscale_image);
    imshow("Negative Grayscale Image", negative_image2);
    waitKey(0);
    return 0;
}