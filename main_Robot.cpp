#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>

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
    //char trackbarWindow_blue[] = "Trackbar_pink";
    char thresholdWindow_pink[] = "Threshold_pink";
    char thresholdWindow_blue[] = "Threshold_blue";
    char thresholdWindow_red[] = "Threshold_red";
    int min = 0, max = 1000;
    int pink_hmin = 157, pink_smin = 119, pink_vmin = 187,
	pink_hmax = 186, pink_smax = 200, pink_vmax = 255;

    int blue_hmin = 92, blue_smin = 221, blue_vmin = 156,
	blue_hmax = 118, blue_smax = 255, blue_vmax = 207;

    int red_hmin = 0, red_smin = 91, red_vmin = 147,
	red_hmax = 18, red_smax = 200, red_vmax = 255;

    Mat frame, HSV, threshold_pink, threshold_blue, threshold_red, blurred;


    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap(0);

    //Создаем окна
    //namedWindow(mainWindow, 0); хз зачем это окно
    //namedWindow(trackbarWindow_blue, 0);
    namedWindow(thresholdWindow_pink, 0);
    namedWindow(thresholdWindow_blue, 0);
    namedWindow(thresholdWindow_red, 0);

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
    /*
    createTrackbar("H min:", trackbarWindow_blue, &blue_hmin, blue_hmax);
    createTrackbar("H max:", trackbarWindow_blue, &blue_hmax, blue_hmax);
    createTrackbar("S min:", trackbarWindow_blue, &blue_smin, blue_smax);
    createTrackbar("S max:", trackbarWindow_blue, &blue_smax, blue_smax);
    createTrackbar("V min:", trackbarWindow_blue, &blue_vmin, blue_vmax);
    createTrackbar("V max:", trackbarWindow_blue, &blue_vmax, blue_vmax);
    createTrackbar("Size min:", trackbarWindow_blue, &min, max);
    createTrackbar("Size max:", trackbarWindow_blue, &max, max);
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

        int Xc_red = 0;
        int Yc_red = 0;
        int counter_red = 0; // счётчик числа белых пикселей (red)

        cvtColor(frame, HSV, COLOR_BGR2HSV);
        medianBlur(HSV, blurred, 21);
	    
        inRange(blurred, Scalar(blue_hmin, blue_smin, blue_vmin), Scalar(blue_hmax, blue_smax, blue_vmax), threshold_blue);
        for(int y = 0; y < threshold_blue.rows; y++){
	    for(int x = 0; x < threshold_blue.cols; x++){
	    	int value = threshold_blue.at<uchar>(y, x);
		if (value == 255){
		    Rect rect;
		    int count = floodFill(threshold_blue, Point(x, y), Scalar(200), &rect);
                    Xc_blue += x;
                    Yc_blue += y;
                    counter_blue++;
			
	            if (rect.width >= min && rect.width <= max && rect.height >= min && rect.height <= max)
                    {
		        rectangle(frame, rect, Scalar(255, 0, 0, 4));
                    }
                }
            }
        }
        
        inRange(blurred, Scalar(pink_hmin, pink_smin, pink_vmin), Scalar(pink_hmax, pink_smax, pink_vmax), threshold_pink);
        for(int y = 0; y < threshold_pink.rows; y++){
	    for(int x = 0; x < threshold_pink.cols; x++){
                 int value = threshold_pink.at<uchar>(y, x);
		 if (value == 255){
	             Rect rect;
		     int count = floodFill(threshold_pink, Point(x, y), Scalar(200), &rect);
                     Xc_pink += x;
                     Yc_pink += y;
                     counter_pink++;
                    
		    if (rect.width >= min && rect.width <= max && rect.height >= min && rect.height <= max)
                    {
		        rectangle(frame, rect, Scalar(255, 0, 255, 4));
                    }
                }
            }
        }

        inRange(blurred, Scalar(red_hmin, red_smin, red_vmin), Scalar(red_hmax, red_smax, red_vmax), threshold_red);
        for(int y = 0; y < threshold_red.rows; y++){
	    for(int x = 0; x < threshold_red.cols; x++){
		int value = threshold_red.at<uchar>(y, x);
		if (value == 255){
		    Rect rect;
		    int count = floodFill(threshold_red, Point(x, y), Scalar(200), &rect);
                    Xc_red += x;
                    Yc_red += y;
                    counter_red++;
			
		    if (rect.width >= min && rect.width <= max && rect.height >= min && rect.height <= max)
                    {
			rectangle(frame, rect, Scalar(255, 255, 0, 4));
                    }
                }
            }
        }

        /*
        cout << pink_hmin << ' ' << pink_hmax << ' ' << pink_smin <<' ' << pink_smax << ' ' << pink_vmin << ' ' << pink_vmax << endl;
        cout << blue_hmin << ' ' << blue_hmax << ' ' << blue_smin << ' ' << blue_smax << ' ' << blue_vmin << ' ' << blue_vmax << endl;
        */

        if (counter_pink != 0 && counter_blue != 0 && counter_red != 0)
        {
            double x1 = double(Xc_pink) / counter_pink - double(Xc_blue) / counter_blue; // Px - Bx
            double x2 = double(Xc_red) / counter_red - double(Xc_blue) / counter_blue; // Rx - Bx
            double y1 = double(Yc_pink) / counter_pink - double(Yc_blue) / counter_blue; // Py - By
            double y2 = double(Yc_red) / counter_red - double(Yc_blue) / counter_blue; // Ry - By

            double BP = sqrt(x1 * x1 + y1 * y1);
            double BR = sqrt(x2 * x2 + y2 * y2);
            double argument = (x1 * x2 + y1 * y2) / (BR * BP);
            cout << "Угол поворота: " << acos(argument) << endl;
            //cout << "Центр X: " << double(Xc_pink) / counter_pink  << ' ' << double(Xc_blue) / counter_blue << ' ' << double(Xc_red) / counter_red << endl; 
            //cout << "Центр Y: " << double(Yc_pink) / counter_pink << ' ' << double(Yc_blue) / counter_blue  << ' ' << double(Yc_red) / counter_red << endl;
        }
        

        // Display the resulting frame
        imshow("Frame", frame);
        imshow(thresholdWindow_pink, threshold_pink);
        imshow(thresholdWindow_blue, threshold_blue);
        imshow(thresholdWindow_red, threshold_red);
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
