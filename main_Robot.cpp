#include "opencv2/opencv.hpp"
#include "main_Robot.h"
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;
using namespace cv;

// Вывод в формате all_data = [["угол до старта", "dist до старта"], ["угол до доз", "dist до доз"], ["угол до пуф", "dist до пуф"]]


vector<vector<double>> coords() {

    //char mainWindow[] = "Main";
	//char trackbarWindow_blue[] = "Trackbar_pink";
	char thresholdWindow_pink[] = "Threshold_pink";
    char thresholdWindow_blue[] = "Threshold_blue";
    char thresholdWindow_yellow[] = "Threshold_yellow";
	int min = 0, max = 1000;
	int pink_hmin = 157, pink_smin = 119, pink_vmin = 187,
		pink_hmax = 186, pink_smax = 200, pink_vmax = 255;

    int blue_hmin = 92, blue_smin = 221, blue_vmin = 156,
		blue_hmax = 118, blue_smax = 255, blue_vmax = 207;

    int yellow_hmin = 43, yellow_smin = 47, yellow_vmin = 170,
		yellow_hmax = 87, yellow_smax = 87, yellow_vmax = 255;

	Mat frame, HSV, threshold_pink, threshold_blue, threshold_yellow, blurred;


    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap(0);

    //Создаем окна
	//namedWindow(mainWindow, 0); хз зачем это окно
	//namedWindow(trackbarWindow_blue, 0);
	//namedWindow(thresholdWindow_pink, 0);
    //namedWindow(thresholdWindow_blue, 0);
    //namedWindow(thresholdWindow_yellow, 0);

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        //return -1;
    }
    /*цикл чтения с камеры*/
    vector <vector<double>> all_data = {{307, 307}, {307, 307}, {307, 307}};
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

        int Xc_yellow = 0;
        int Yc_yellow = 0;
        int counter_yellow = 0; // счётчик числа белых пикселей (yellow)

        int X_dispenser = 100; // центр дозатора
        int Y_dispenser = 100;

        int X_start = 100;  // центр точки старта
        int Y_start = 300;

        cvtColor(frame, HSV, COLOR_BGR2HSV);
        medianBlur(HSV, blurred, 21);
        
        inRange(blurred, Scalar(blue_hmin, blue_smin, blue_vmin), Scalar(blue_hmax, blue_smax, blue_vmax), threshold_blue);
        for(int y = 0; y < threshold_blue.rows; y++){
		    for(int x = 0; x < threshold_blue.cols; x++){
			    int value = threshold_blue.at<uchar>(y, x);
			    if(value == 255){
				    Rect rect;
				    int count = floodFill(threshold_blue, Point(x, y), Scalar(200), &rect);
                    
                    Xc_blue += x;
                    Yc_blue += y;
                    counter_blue++;
				    if(rect.width >= min && rect.width <= max
					    && rect.height >= min && rect.height <= max)
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
			    if(value == 255){
				    Rect rect;
				    int count = floodFill(threshold_pink, Point(x, y), Scalar(200), &rect);
                    Xc_pink += x;
                    Yc_pink += y;
                    counter_pink++;
                    
				    if(rect.width >= min && rect.width <= max
					    && rect.height >= min && rect.height <= max)
                    {
					    rectangle(frame, rect, Scalar(255, 0, 255, 4));
                    }
                }
            }
        }

        inRange(blurred, Scalar(yellow_hmin, yellow_smin, yellow_vmin), Scalar(yellow_hmax, yellow_smax, yellow_vmax), threshold_yellow);
        for(int y = 0; y < threshold_yellow.rows; y++){
		    for(int x = 0; x < threshold_yellow.cols; x++){
			    int value = threshold_yellow.at<uchar>(y, x);
			    if(value == 255){
				    Rect rect;
				    int count = floodFill(threshold_yellow, Point(x, y), Scalar(200), &rect);
                    
                    Xc_yellow += x;
                    Yc_yellow += y;
                    counter_yellow++;
				    if(rect.width >= min && rect.width <= max
					    && rect.height >= min && rect.height <= max)
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
        
        if (counter_pink != 0 && counter_blue != 0 && counter_yellow != 0)
        {
            double x1 = double(Xc_pink) / counter_pink - double(Xc_blue) / counter_blue; // Px - Bx
            double y1 = double(Yc_pink) / counter_pink - double(Yc_blue) / counter_blue; // Py - By
            double x2 = double(Xc_yellow) / counter_yellow - double(Xc_blue) / counter_blue; // Yx - Bx
            double y2 = double(Yc_yellow) / counter_yellow - double(Yc_blue) / counter_blue; // Yy - By
            double x3 = double(X_dispenser) - double(Xc_blue) / counter_blue; // Dx - Bx
            double y3 = double(Y_dispenser) - double(Yc_blue) / counter_blue; // Dy - By
            double x4 = double(X_start) - double(Xc_blue) / counter_blue; // Sx - Bx
            double y4 = double(Y_start) - double(Yc_blue) / counter_blue; // Sy - By


            double BP = sqrt(x1 * x1 + y1 * y1);
            double BY = sqrt(x2 * x2 + y2 * y2);
            double BD = sqrt(x3 * x3 + y3 * y3);
            double BS = sqrt(x4 * x4 + y4 * y4);
            double cos_to_puf = (x1 * x2 + y1 * y2) / (BY * BP);
            double cos_to_disp = (x1 * x3 + y1 * y3) / (BP * BD);
            double cos_to_start = (x1 * x4 + y1 * y4) / (BP * BS);


            double dist_to_puf = sqrt(pow(double(Xc_yellow) / counter_yellow - double(Xc_pink) / counter_pink, 2) +\
            pow(double(Yc_yellow) / counter_yellow - double(Yc_pink) / counter_pink, 2));
            double dist_to_disp = sqrt(pow(double(X_dispenser)- double(Xc_pink) / counter_pink, 2) +\
            pow(double(Y_dispenser) - double(Yc_pink) / counter_pink, 2));
            double dist_to_start = sqrt(pow(double(X_start)- double(Xc_pink) / counter_pink, 2) +\
            pow(double(Y_start) - double(Yc_pink) / counter_pink, 2));
            
            for (int i = 0; i < 3; ++i)
            {
                if (i == 0)
                {
                    //vector <double> temp = {acos(cos_to_start), dist_to_start};
                    //cout << acos(cos_to_start) << "\t" << dist_to_start;
                    all_data[0][0] = cos(cos_to_start); // НУЖНО ЛИ ВООБЩЕ ПИСАТЬ МЕТОД COS??? МОЖЕТ ПРОСТО ПЕРЕДАВАТЬ СРАЗУ КОСИНУС?
                    all_data[0][1] = dist_to_start;

                } else if (i == 1)
                {
                    //vector <double> temp = {acos(cos_to_disp), dist_to_disp};
                    //cout << acos(cos_to_disp) << "\t" << dist_to_disp;
                    all_data[1][0] = cos(cos_to_disp); 
                    all_data[1][1] = dist_to_disp;
                } else 
                {
                    //vector <double> temp = {acos(cos_to_puf), dist_to_puf};
                    //cout << acos(cos_to_puf) << "\t" << dist_to_puf;
                    all_data[2][0] = cos(cos_to_puf); 
                    all_data[2][1] = dist_to_puf;
                    //all_data.push_back(temp);
                }
            }
            break;
        }
        
        // Display the resulting frame
        //imshow("Frame", frame);
        //imshow(thresholdWindow_pink, threshold_pink);
        //imshow(thresholdWindow_blue, threshold_blue);
        //imshow(thresholdWindow_yellow, threshold_yellow);

        // Press  ESC on keyboard to exit
        char c = (char)waitKey(25);
        if (c == 27)
            break;
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();
    
    return all_data;    
}

int main()
{
    vector<vector<double>> a = coords();
    for (auto elem: a)
    {
        for (auto el: elem)
        {
            cout << el << ' ';
        }
    }
    return 0;
}
