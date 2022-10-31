#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core/utility.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>



using namespace cv;
int main() {
    int numberOfDevices = 0;
    bool noError = true;

    while (noError)
    {
        try
        {
            VideoCapture videoCapture(numberOfDevices);
            std::cout<<videoCapture.getBackendName();
            numberOfDevices++;
        }
        catch (...)
        {
            noError = false;
        }
    }
    if(numberOfDevices == 0) {
        std::cout<< "no camera";
        return 0;
    }

    VideoCapture cap;
    Mat frame;
    cap.open(0);
    std::cout<<"start camera";
    while(1) {

        cap >> frame;
        imshow("cadr", frame);
        char c = cv::waitKey(33);
        if (c == 27) {
            break;
        }
    }
}
