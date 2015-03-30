#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* c_CMT;

c_CMT newCMT();
void delCMT(c_CMT cmt);
void cmt_initialize(c_CMT cmt, unsigned char* im_data, size_t rows, size_t cols,
    int x, int y, int w, int h);
void processFrame(c_CMT cmt, unsigned char* im_data, size_t rows, size_t cols,
    int * x, int * y, int * w, int * h);

#ifdef __cplusplus
}
#endif

#endif /* WRAPPER_H */
