#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    Mat image = imread("color_image.jpg");
    Mat blurredImage, edgeImage;
    blur(image, blurredImage, Size(3, 3));
    Canny(image, edgeImage, 100, 200);
    imshow("Blurred Image", blurredImage);
    imshow("Edge Image", edgeImage);
    waitKey(0);
    return 0;
}