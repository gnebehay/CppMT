#include "CMT.h"
#include "gui.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using cmt::CMT;
using cv::Scalar;
using cv::namedWindow;
using cv::VideoCapture;
using cv::waitKey;

static string WIN_NAME = "CMT";

int main(int argc, char **argv)
{
    //Set up logging
    FILELog::ReportingLevel() = logDEBUG;
    Output2FILE::Stream() = stdout; //Log to stdout

    //Create window
    namedWindow(WIN_NAME);

    //Open default camera device
    VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;

    //Show preview until key is pressed
    Mat im0;
    Mat im_draw;
    while(1)
    {
        cap >> im0;
        im0.copyTo(im_draw);

        screenLog(im_draw, "Press a key to start selecting an object.");
        imshow(WIN_NAME, im_draw);

        char k = waitKey(10);
        if (k != -1) {
            break;
        }
    }

    //Get bounding box from user
    Rect rect;
    getRect(im0, WIN_NAME, rect);

    screenLog(im_draw, "Initializing..."); //Re-use im_draw for logging
    imshow(WIN_NAME, im_draw);
    waitKey(10);

    //Convert im0 to grayscale
    Mat im_gray0;
    cvtColor(im0, im_gray0, CV_BGR2GRAY);

    //Initialize CMT
    CMT cmt;
    cmt.initialize(im_gray0, rect);

    int frame = 0;

    while(1)
    {
        frame++;

        Mat im;
        cap >> im;

        Mat im_gray;
        cvtColor(im, im_gray, CV_BGR2GRAY);

        //Let CMT process the frame
        cmt.processFrame(im_gray);

        //Visualize the output
        //It is ok to draw on im itself, as CMT only uses the grayscale image
        for(size_t i = 0; i < cmt.points_active.size(); i++)
        {
            circle(im, cmt.points_active[i], 2, Scalar(255,0,0));
        }

        Point2f vertices[4];
        cmt.bb_rot.points(vertices);
        for (int i = 0; i < 4; i++)
        {
            line(im, vertices[i], vertices[(i+1)%4], Scalar(255,0,0));
        }

        imshow(WIN_NAME, im);

        char key = waitKey(5);
        if(key == 'q') break;

        //TODO: Provide meaningful output
        FILE_LOG(logINFO) << "#" << frame << " active: " << cmt.points_active.size();
    }

    return 0;
}
