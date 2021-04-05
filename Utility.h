#ifndef UTILITY_H
#define UTILITY_H


#include <QImage>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"

#define     NO_OBJECT       0
#define     MIN(x, y)       (((x) < (y)) ? (x) : (y))
#define     ELEM(img, r, c) (CV_IMAGE_ELEM(img, unsigned char, r, c))
#define     ONETWO(L, r, c, col) (L[(r) * (col) + c])

using namespace cv;

QImage* MatToQImage(const Mat &mat);

Mat* QImageToMat(QImage &image);

int _find( int set[], int x ) ;
int _bwlabel(IplImage* img, int n, int* labels);
#endif
