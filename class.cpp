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

int main()
{
    
}

