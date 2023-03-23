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



int main(int argc, char **argv) {

    //char mainWindow[] = "Main";
	//char trackbarWindow_pink[] = "Trackbar_pink";
	char thresholdWindow_pink[] = "Threshold_pink";
    char thresholdWindow_blue[] = "Threshold_blue";
	int min = 0, max = 1000;
	int pink_hmin = 157, pink_smin = 119, pink_vmin = 187,
		pink_hmax = 186, pink_smax = 200, pink_vmax = 255;

    int blue_hmin = 92, blue_smin = 221, blue_vmin = 156,
		blue_hmax = 118, blue_smax = 255, blue_vmax = 207;

	Mat frame, HSV, threshold, blurred;


    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap(0);

    //Создаем окна
	//namedWindow(mainWindow, 0); хз зачем это окно
	//namedWindow(trackbarWindow_pink, 0);
	namedWindow(thresholdWindow_pink, 0);
    namedWindow(thresholdWindow_blue, 0);

    /*Создаем трэкбар для регулирования цвета*/
    /*
    createTrackbar("H min:", trackbarWindow_pink, &pink_hmin, pink_hmax);
    createTrackbar("H max:", trackbarWindow_pink, &pink_hmax, pink_hmax);
    createTrackbar("S min:", trackbarWindow_pink, &pink_smin, pink_smax);
    createTrackbar("S max:", trackbarWindow_pink, &pink_smax, pink_smax);
    createTrackbar("V min:", trackbarWindow_pink, &pink_vmin, pink_vmax);
    createTrackbar("V max:", trackbarWindow_pink, &pink_vmax, pink_vmax);
    createTrackbar("Size min:", trackbarWindow_pink, &min, max);
    createTrackbar("Size max:", trackbarWindow_pink, &max, max);
    */

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    /*цикл чтения с камеры*/
    while (1) {

        
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        int Xc_pink = 0;
        int Yc_pink = 0;
        int counter_pink = 0; // счётчик числа белых пикселей (pink)

        int Xc_blue = 0;
        int Yc_blue = 0;
        int counter_blue = 0; // счётчик числа белых пикселей (blue)

        cvtColor(frame, HSV, COLOR_BGR2HSV);
        medianBlur(HSV, blurred, 21);
	    inRange(blurred, Scalar(pink_hmin, pink_smin, pink_vmin), Scalar(pink_hmax, pink_smax, pink_vmax), threshold);
        inRange(blurred, Scalar(blue_hmin, blue_smin, blue_vmin), Scalar(blue_hmax, blue_smax, blue_vmax), threshold);
        for(int y = 0; y < threshold.rows; y++){
		    for(int x = 0; x < threshold.cols; x++){
			    int value = threshold.at<uchar>(y, x);
			    if(value == 255){
				    Rect rect;
				    int count = floodFill(threshold, Point(x, y), Scalar(200), &rect);
                    Xc_pink += x;
                    Yc_pink += y;
                    counter_pink++;
                    /* а че это они тут вместе считаются? м? почему координаты двух цветов считаются вместе? думайте!  */

                    Xc_blue += x;
                    Yc_blue += y;
                    counter_blue++;
				    if(rect.width >= min && rect.width <= max
					    && rect.height >= min && rect.height <= max)
                    {
					    rectangle(frame, rect, Scalar(255, 0, 255, 4));
                    }
                }
            }
        }
        if (counter_pink != 0)
        {
            cout << "Центр X: " << double(Xc_pink) / counter_pink  << ' ' << double(Xc_blue) / counter_blue << endl; 
            cout << "Центр Y: " << double(Yc_pink) / counter_pink << ' ' << double(Yc_blue) / counter_blue << endl;
        }

        // Display the resulting frame
        imshow("Frame", frame);
        imshow(thresholdWindow_pink, threshold);
        imshow(thresholdWindow_blue, threshold);
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
