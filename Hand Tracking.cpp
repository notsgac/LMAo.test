#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    VideoCapture capture(0); // Open the default camera
    if (!capture.isOpened())
    {
        std::cerr << "Error: Unable to open camera." << std::endl;
        return -1;
    }
    namedWindow("Hand Tracking", WINDOW_NORMAL);
    while (true)
    {
        Mat frame;
        capture >> frame; // Read a frame from the camera
        if (frame.empty())
        {
            std::cerr << "Error: Unable to read frame from camera." << std::endl;
            break;
        }
        Mat hsv;
        cvtColor(frame, hsv, COLOR_BGR2HSV); // Convert the frame to HSV color space
        Mat mask1, mask2;
        inRange(hsv, Scalar(0, 20, 70), Scalar(20, 255, 255), mask1); // Threshold the HSV image to extract the first hand region
        inRange(hsv, Scalar(160, 20, 70), Scalar(180, 255, 255), mask2); // Threshold the HSV image to extract the second hand region
        Mat mask = mask1 | mask2; // Combine the two masks
        erode(mask, mask, Mat(), Point(-1, -1), 2); // Erode the mask to remove noise
        dilate(mask, mask, Mat(), Point(-1, -1), 2); // Dilate the mask to fill gaps
        std::vector<std::vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // Find the contours in the mask
        for (const auto& contour : contours)
        {
            double area = contourArea(contour); // Get the area of the contour
            if (area < 1000 || area > 10000) // Filter out small and large contours
            {
                continue;
            }
            Rect boundingBox = boundingRect(contour); // Get the bounding box of the contour
            if (boundingBox.y < frame.rows * 0.5) // Filter out contours that are too high or too low
            {
                continue;
            }
            convexHull(contour, contour); // Compute the convex hull of the contour
            std::vector<int> hullIndexes;
            convexHull(contour, hullIndexes); // Get the indices of the convex hull points
            std::vector<Vec4i> defects;
            convexityDefects(contour, hullIndexes, defects); // Compute the convexity defects of the contour
            if (defects.size() < 3) // Filter out contours that do not have enough convexity defects
            {
                continue;
            }
            // Compute hand posture features and recognize the gesture
            // ...
            rectangle(frame, boundingBox, Scalar(0, 255, 0), 2); // Draw a green rectangle around the hand region
        }
        imshow("Hand Tracking", frame); // Display the processed frame with the hand regions
        if (waitKey(1) == 27) // Exit loop if user presses 'Esc' key
        {
            break;
        }
    }
    capture.release(); // Release the camera
    destroyAllWindows(); // Close all windows
    return 0;
}
