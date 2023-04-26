#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat ContrastStretching(const Mat& image, int r1, int r2, int s1, int s2) {
    Mat imageContrastStretching = image.clone();
    vector<Mat> channels;
    split(imageContrastStretching, channels);

    for (auto& channel : channels) {
        for (int y = 0; y < channel.rows; y++) {
            for (int x = 0; x < channel.cols; x++) {
                int pixel = channel.at<uchar>(y, x);
                if (0 <= pixel && pixel <= r1) {
                    channel.at<uchar>(y, x) = s1 / r1 * pixel;
                }
                else if (r1 < pixel && pixel <= r2) {
                    channel.at<uchar>(y, x) = ((s2 - s1) / (r2 - r1)) * (pixel - r1) + s1;
                }
                else if (r2 < pixel && pixel <= 255) {
                    channel.at<uchar>(y, x) = ((255 - s2) / (255 - r2)) * (pixel - r2) + s2;
                }
            }
        }
    }

    merge(channels, imageContrastStretching);
    return imageContrastStretching;
}

int main() {
    Mat color_image = imread("color_image.jpg", IMREAD_COLOR);
    if (color_image.empty()) {
        cerr << "Error: Could not read the image" << endl;
        return -1;
    }
    int r1, r2, s1, s2;
    cout << "Enter r1: ";
    cin >> r1;
    cout << "Enter r2: ";
    cin >> r2;
    cout << "Enter s1: ";
    cin >> s1;
    cout << "Enter s2: ";
    cin >> s2;
    Mat contrast_stretched_image = ContrastStretching(color_image, r1, r2, s1, s2);
    imshow("Original Image", color_image);
    imshow("Contrast-Stretched Image", contrast_stretched_image);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
