#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
    CascadeClassifier faceCascade("E:/Github/opencv/data/haarcascades/haarcascade_frontalface_default.xml");
    VideoCapture capture(0); // Open the default camera
    if (!capture.isOpened())
    {
        cerr << "Error: Unable to open camera." << endl;
        return -1;
    }
    double fps = capture.get(CAP_PROP_FPS); // Get the frame rate of the camera
    int width = capture.get(CAP_PROP_FRAME_WIDTH); // Get the width of the frames captured by the camera
    int height = capture.get(CAP_PROP_FRAME_HEIGHT); // Get the height of the frames captured by the camera
    int brightness = capture.get(CAP_PROP_BRIGHTNESS); // Get the brightness setting of the camera
    int contrast = capture.get(CAP_PROP_CONTRAST); // Get the contrast setting of the camera
    int saturation = capture.get(CAP_PROP_SATURATION); // Get the saturation setting of the camera
    int hue = capture.get(CAP_PROP_HUE); // Get the hue setting of the camera
    int exposure = capture.get(CAP_PROP_EXPOSURE); // Get the exposure setting of the camera
    cout << "Camera Settings:" << endl;
    cout << "FPS: " << fps << endl;
    cout << "Resolution: " << width << " x " << height << endl;
    cout << "Brightness: " << brightness << endl;
    cout << "Contrast: " << contrast << endl;
    cout << "Saturation: " << saturation << endl;
    cout << "Hue: " << hue << endl;
    cout << "Exposure: " << exposure << endl;
    namedWindow("Face Tracking", WINDOW_NORMAL);
    while (true)
    {
        Mat frame;
        capture >> frame; // Read a frame from the camera
        if (frame.empty())
        {
            cerr << "Error: Unable to read frame from camera." << endl;
            break;
        }
        vector<Rect> faces;
        faceCascade.detectMultiScale(frame, faces, 1.1, 2, 0, Size(30, 30));
        for (const auto& face : faces)
        {
            rectangle(frame, face, Scalar(0, 255, 0), 2); // Draw a green rectangle around each detected face
        }
        imshow("Face Tracking", frame); // Display the processed frame with detected faces
        if (waitKey(16) == 27) // Exit loop if user presses 'Esc' key
        {
            break;
        }
    }
    capture.release(); // Release the camera
    destroyAllWindows(); // Close all windows
    return 0;
}
