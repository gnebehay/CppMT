#include "CMT.h"
#include "gui.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <fstream>

#ifdef __GNUC__
#include <getopt.h>
#else
#include "getopt/getopt.h"
#endif


using cmt::CMT;
using cv::imread;
using cv::namedWindow;
using cv::Scalar;
using cv::VideoCapture;
using cv::waitKey;
using std::ifstream;
using std::ofstream;

static string WIN_NAME = "CMT";

int main(int argc, char **argv)
{
    //Create a CMT object
    CMT cmt;

    //Parse args
    int challenge_flag = 0;
    int verbose_flag = 0;

    const int detector_cmd = 1000;
    const int descriptor_cmd = 1001;

    struct option longopts[] =
    {
        {"challenge", no_argument, &challenge_flag, 1},
        {"verbose", no_argument, &verbose_flag, 1},
        {"detector", required_argument, 0, detector_cmd},
        {"descriptor", required_argument, 0, descriptor_cmd},
        {0, 0, 0, 0}
    };

    int index = 0;
    int c;
    while((c = getopt_long(argc, argv, "v", longopts, &index)) != -1)
    {
        switch (c)
        {
            case 'v':
                verbose_flag = true;
                break;
            case detector_cmd:
                cmt.str_detector = optarg;
                break;
            case descriptor_cmd:
                cmt.str_descriptor = optarg;
                break;
        }

    }

    //Set up logging
    FILELog::ReportingLevel() = verbose_flag ? logDEBUG : logINFO;
    Output2FILE::Stream() = stdout; //Log to stdout

    //Challenge mode
    if (challenge_flag)
    {
        //Read list of images
        ifstream im_file("images.txt");
        vector<string> files;
        string line;
        while(im_file >> line)
        {
            files.push_back(line);
        }

        //Read region
        float x,y,w,h;
        char a;
        ifstream region_file("region.txt");
        region_file >> x >> a >> y >> a >> w >> a >> h;
        Rect rect(x,y,w,h);

        //Read first image
        Mat im0 = imread(files[0]);
        Mat im0_gray;
        cvtColor(im0, im0_gray, CV_BGR2GRAY);

        //Initialize cmt
        cmt.initialize(im0_gray, rect);

        //Write init region to output file
        ofstream output_file("output.txt");
        output_file << rect.x << ',' << rect.y << ',' << rect.width << ',' << rect.height << std::endl;

        //Process images, write output to file
        for (size_t i = 1; i < files.size(); i++)
        {
            FILE_LOG(logINFO) << "Processing frame " << i << "/" << files.size();
            Mat im = imread(files[i]);
            Mat im_gray;
            cvtColor(im, im_gray, CV_BGR2GRAY);
            cmt.processFrame(im_gray);
            rect = cmt.bb_rot.boundingRect();
            output_file << rect.x << ',' << rect.y << ',' << rect.width << ',' << rect.height << std::endl;
        }

        output_file.close();

        return 0;
    }

    //Normal mode

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
    Mat im0_gray;
    cvtColor(im0, im0_gray, CV_BGR2GRAY);

    //Initialize CMT
    cmt.initialize(im0_gray, rect);

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
