#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/core/utility.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#if defined __linux__
#include <time.h>

unsigned long long getNanoTime() {
    timespec time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    return time.tv_sec * 1000000000 + time.tv_nsec;
}
#endif
#if defined _WIN32
#include <Windows.h>

unsigned long long getNanoTime() {
    return GetTickCount64()*1000000;
}

#endif

using namespace cv;

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
            std::cout << camera.getBackendName()
                << " WIDTH:" << camera.get(CV_CAP_PROP_FRAME_WIDTH)
                << " HEIGHT:" << camera.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl  ;
            numberOfDevices++;
            cam = camera;
        }
        catch (...)
        {
            noError = false;
        }
    }

    if(numberOfDevices == 0) {
        std::cout << "no camera";
        return 0;
    } else {
        std::cout << "count: " << numberOfDevices << std::endl;
    }

    Mat frame;
    int width = cam.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = cam.get(CV_CAP_PROP_FRAME_HEIGHT);
    cam.open(0);
    std::cout << "start camera";
    unsigned long long inputTime = 0;
    unsigned long long calculateTime = 0;
    unsigned long long showTime = 0;
    unsigned long long frameCount = 0;
    unsigned long long start = getNanoTime();
    while(1) {
        unsigned long long time1 = getNanoTime();
        cam >> frame;
        inputTime += getNanoTime() - time1;
        time1 = getNanoTime();
        bigPixels(frame, height, width, 15);
        calculateTime += getNanoTime() - time1;
        time1 = getNanoTime();
        imshow("cadr", frame);
        showTime += getNanoTime() - time1;
        char c = cv::waitKey(33);
        if (c == 27) {
            break;
        }
        frameCount++;
        if(frameCount % 100 == 0) {
            unsigned long long time = getNanoTime() - start;
            std::cout<< "input time: " << inputTime << std::endl
                << "calculate time: " << calculateTime << std::endl
                << "show time: " << showTime << std::endl
                << "total time: " << time  << std::endl
                << "fps: " << frameCount / (time/1000000000) << std::endl;
        }
    }
}
