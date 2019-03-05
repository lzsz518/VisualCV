#include "opencv2/imgproc.hpp"
#include "Utility.h"

QImage* MatToQImage(const Mat &mat)
{
	if(mat.empty())
		return NULL;

	Mat dst;

	switch(mat.type())
	{
	case CV_8UC1:
		cvtColor(mat,dst,CV_GRAY2RGB);//The cvtColor function can`t direct convert GRAY to RGBA, I don`t know why.
		cvtColor(dst,dst,CV_RGB2RGBA);
		break;
	case CV_8UC2:
		cvtColor(mat,dst,CV_BGR5652RGBA);
		break;
	case CV_8UC3:
		cvtColor(mat,dst,CV_RGB2RGBA);
		break;
	case CV_8UC4:
		dst = mat.clone();
		break;
	default:
        return nullptr;
	}

	QImage *result = new QImage(dst.cols,dst.rows,QImage::Format_ARGB32);
	memcpy(result->bits(),dst.data,dst.total()*dst.elemSize());

	return result;
}

Mat* QImageToMat(QImage &image)
{
    return nullptr;
}
