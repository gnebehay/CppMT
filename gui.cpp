#include "gui.h"

#include <opencv2/highgui/highgui.hpp>
#if CV_MAJOR_VERSION > 2
# include <opencv2/imgproc.hpp>
#endif

using cv::setMouseCallback;
using cv::Point;
using cv::Scalar;
using cv::Size;

void screenLog(Mat im_draw, const string text)
{
    int font = cv::FONT_HERSHEY_SIMPLEX;
    float font_scale = 0.5;
    int thickness = 1;
    int baseline;

    Size text_size = cv::getTextSize(text, font, font_scale, thickness, &baseline);

    Point bl_text = Point(0,text_size.height);
    Point bl_rect = bl_text;

    bl_rect.y += baseline;

    Point tr_rect = bl_rect;
    tr_rect.x = im_draw.cols; //+= text_size.width;
    tr_rect.y -= text_size.height + baseline;

    rectangle(im_draw, bl_rect, tr_rect, Scalar(0,0,0), -1);

    putText(im_draw, text, bl_text, font, font_scale, Scalar(255,255,255));
}

//For bbox selection
static string win_name_;
static Mat im_select;
static bool tl_set;
static bool br_set;
static Point tl;
static Point br;

static void onMouse(int event, int x, int y, int flags, void *param)
{
    Mat im_draw;
    im_select.copyTo(im_draw);

    if(event == CV_EVENT_LBUTTONUP && !tl_set)
    {
        tl = Point(x,y);
        tl_set = true;
    }

    else if(event == CV_EVENT_LBUTTONUP && tl_set)
    {
        br = Point(x,y);
        br_set = true;
        screenLog(im_draw, "Initializing...");
    }

    if (!tl_set) screenLog(im_draw, "Click on the top left corner of the object");
    else
    {
        rectangle(im_draw, tl, Point(x, y), Scalar(255,0,0));

        if (!br_set) screenLog(im_draw, "Click on the bottom right corner of the object");
    }

    imshow(win_name_, im_draw);
}

Rect getRect(const Mat im, const string win_name)
{

    win_name_ = win_name;
    im_select = im;
    tl_set = false;
    br_set = false;

    setMouseCallback(win_name, onMouse);

    //Dummy call to get drawing right
    onMouse(0,0,0,0,0);

    while(!br_set)
    {
        cvWaitKey(10);
    }

    setMouseCallback(win_name, NULL);

    im_select.release(); //im_select is in global scope, so we call release manually

    return Rect(tl,br);
}

