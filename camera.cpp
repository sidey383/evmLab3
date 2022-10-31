#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/core/utility.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

void bigPixels(Mat frame, int width, int height, int pixelSize) {
    for (int x = 0; x < width/pixelSize + 1; x++) {
        for (int y = 0; y < height/pixelSize + 1; y++) {
            Vec3i result(0, 0, 0);
            int count = 0;
            for(int x_ = 0; x_ < pixelSize && x*pixelSize+x_ < width; x_++) {
                for(int y_ = 0; y_ < pixelSize && y*pixelSize+y_ < height; y_++) {
                    Vec3b intensity = frame.at<Vec3b>(x*pixelSize+x_, y*pixelSize+y_);
                    result+=intensity;
                    count++;
                }
            }
            result/=count;
            Vec3b color(result[0], result[1], result[2]);
            for(int x_ = 0; x_ < pixelSize && x*pixelSize+x_ < width; x_++) {
                for(int y_ = 0; y_ < pixelSize && y*pixelSize+y_ < height; y_++) {
                    frame.at<Vec3b>(x*pixelSize+x_, y*pixelSize+y_) = result;
                }
            }
        }
    }
}

int main() {
    int numberOfDevices = 0;
    bool noError = true;
    VideoCapture cam;

    while (noError)
    {
        try
        {
            VideoCapture camera(numberOfDevices);
            std::cout<<camera.getBackendName()<<" WIDTH:"<<camera.get(CV_CAP_PROP_FRAME_WIDTH)<<" HEIGHT:"<<camera.get(CV_CAP_PROP_FRAME_HEIGHT)<<"\n";
            numberOfDevices++;
            cam = camera;
        }
        catch (...)
        {
            noError = false;
        }
    }

    cout<<"count: "<<numberOfDevices<<"\n";


    Mat frame;
    int width = cam.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = cam.get(CV_CAP_PROP_FRAME_HEIGHT);
    cam.open(0);
    while (1) {
        cam >> frame;
        bigPixels(frame, height, width, 15);
        imshow("changed", frame);
        char c = cv::waitKey(33);
        if (c == 27) {
            break;
        }
    }
}