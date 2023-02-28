#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

struct Color {
    int R;
    int G;
    int B;
};

class Spot {
private:
    Color color;
    Mat frame;
    Mat spot;
    pair <int, int> center;
public:
    void search_spot();
    void count_center();
};

class Route {
private:
    Spot back;
    Spot front;
    int rotation;
    int distance;

public:
    int count_rotation(pair <int, int> destination);
    int count_distance(pair <int, int> start, pair <int, int> stop);
};


int main() {

    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap("0");

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    while (1) {

        Mat frame;
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        // Display the resulting frame
        imshow("Frame", frame);

        // Press  ESC on keyboard to exit
        char c = (char)waitKey(25);
        if (c == 27)
            break;
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}
