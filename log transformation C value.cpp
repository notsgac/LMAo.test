#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat createHistogramImage(const Mat& hist, Scalar color = Scalar(255, 255, 255)) {
    int histSize = hist.rows;
    int histWidth = 512, histHeight = 400;
    int binWidth = cvRound((double)histWidth / histSize);

    Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0));
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < histSize; i++) {
        line(histImage, Point(binWidth * (i - 1), histHeight - cvRound(hist.at<float>(i - 1))),
            Point(binWidth * i, histHeight - cvRound(hist.at<float>(i))),
            color, 2, 8, 0);
    }

    return histImage;
}

int main() {
    // Read the color image
    Mat color_image = imread("Color_image.jpg", IMREAD_COLOR);

    // Check if the image is loaded successfully
    if (color_image.empty()) {
        cerr << "Error: Could not read the image" << endl;
        return -1;
    }

    // Define the constant values for log transformation
    vector<double> c_values = { 0.01, 0.5, 1, 3 };
    double r_max;
    minMaxLoc(color_image, nullptr, &r_max);
    c_values.push_back(255 / log(r_max + 1));

    // Create a vector to store the log-transformed images
    vector<Mat> log_transformed_images;

    // Apply log transformation for each constant value
    for (double c : c_values) {
        // Create a lookup table
        Mat lookup_table(1, 256, CV_8U);
        uchar* p = lookup_table.ptr();
        for (int i = 0; i < 256; ++i) {
            p[i] = saturate_cast<uchar>(c * log(1 + i));
        }

        // Apply log transformation using the lookup table
        Mat log_transformed_image;
        LUT(color_image, lookup_table, log_transformed_image);

        // Add the log-transformed image to the vector
        log_transformed_images.push_back(log_transformed_image);
    }

    // Create a window to display the original and log-transformed images
    namedWindow("Original and Log-Transformed Images", WINDOW_NORMAL);

    // Concatenate the images horizontally
    Mat combined_image = color_image;
    for (const Mat& log_transformed_image : log_transformed_images) {
        hconcat(combined_image, log_transformed_image, combined_image);
    }

    // Resize the window to half of its original size
    Mat resized_image;
    resize(combined_image, resized_image, Size(combined_image.cols / 2, combined_image.rows / 2));

    // Display the combined image
    imshow("Original and Log-Transformed c=0.01; c= 0.5; c=1; c=3; c= 255 / log(r_max + 1) Images", resized_image);

    // Create a Mat to store concatenated histograms
    Mat combinedHistograms;

    // Calculate and concatenate histograms for each log-transformed image
    for (size_t i = 0; i < log_transformed_images.size(); i++) {
        Mat gray_image;
        cvtColor(log_transformed_images[i], gray_image, COLOR_BGR2GRAY);
        int histSize = 256;
        float range[] = { 0, 256 };
        const float* histRange = { range };
        Mat hist;
        calcHist(&gray_image, 1, 0, Mat(), hist, 1, &histSize, &histRange);
        // Create histogram image
        Mat histImage = createHistogramImage(hist);
        // Concatenate histograms horizontally
        if (combinedHistograms.empty()) {
            combinedHistograms = histImage.clone();
        }
        else {
            hconcat(combinedHistograms, histImage, combinedHistograms);
        }
    }
    Mat resizedHistograms;
    resize(combinedHistograms, resizedHistograms, Size(combinedHistograms.cols / 2, combinedHistograms.rows / 2));
    // Display the combined histograms
    imshow("Resized Combined Histograms", resizedHistograms);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
