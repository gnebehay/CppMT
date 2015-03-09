#ifndef TRACKER_H

#define TRACKER_H

#include "common.h"

namespace cmt {

class Tracker
{
public:
    Tracker() : thr_fb(30) {};
    void track(const Mat im_prev, const Mat im_gray, const vector<Point2f> & points_prev,
            vector<Point2f> & points_tracked, vector<unsigned char> & status);

private:
    float thr_fb;
};

} /* namespace CMT */

#endif /* end of include guard: TRACKER_H */
