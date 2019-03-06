#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "Command/CustomFilter2D.h"

QCustomFilter2D::QCustomFilter2D()
{

}

QCustomFilter2D::~QCustomFilter2D()
{

}

bool QCustomFilter2D::SetParameter(const CommandParameter *para)
{
    if(para==NULL)
        return false;
    CommandParameter_Filter *filter_para = (CommandParameter_Filter*)para;
    border_type = filter_para->bordertype;
    depth = filter_para->depth;
    delta = filter_para->delta;
    anchor = filter_para->point;
    kernel = filter_para->kernel;

	return true;
}

void QCustomFilter2D::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);
}

void QCustomFilter2D::redo()
{
	if(!initialization)
		return;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());

	filter2D(original_image,final_image,depth,kernel,anchor,delta,border_type);

	connection_data->SetDisplayImage(final_image);
}
