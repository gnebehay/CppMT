#include "CMT.h"
#include "cwrapper.h"

#include <opencv2/core/core.hpp>

using namespace cmt;

c_CMT newCMT() {
    return new CMT();
}

void delCMT(c_CMT cmt) {
    delete (CMT *)cmt;
}

void cmt_initialize(c_CMT cmt, unsigned char* im_data, size_t rows, size_t cols,
    int x, int y, int w, int h) {

    Rect rect(x,y,w,h);

    Mat im_gray(rows, cols, CV_8UC1, im_data);

    ((CMT *)cmt)->initialize(im_gray, rect);
}

void processFrame(c_CMT cmt, unsigned char* im_data, size_t rows, size_t cols,
        int * x, int * y, int * w, int * h) {

    Mat im_gray(rows, cols, CV_8UC1, im_data);

    ((CMT *)cmt)->processFrame(im_gray);

    Rect rect = ((CMT *)cmt)->bb_rot.boundingRect();

    *x = rect.x;
    *y = rect.y;
    *w = rect.width;
    *h = rect.height;
}
