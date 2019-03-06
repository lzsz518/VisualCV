#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "Command/BlurFilter.h"

using namespace cv;

QBlurFilter::QBlurFilter()
{
	initialization = false;
}

QBlurFilter::~QBlurFilter()
{
	original_image.release();
	final_image.release();
}

bool QBlurFilter::SetParameter(const CommandParameter *para)
{
    if(para==NULL)
        return false;
    CommandParameter_Filter *filter_para = (CommandParameter_Filter*)para;

    if(filter_para->size.width!=filter_para->size.height)
		return false;

    size = filter_para->size;
    anchor = filter_para->point;
    border_type = filter_para->bordertype;

	if(anchor.x>=size.width)
		anchor.x = size.width-1;
	if(anchor.y>=size.height)
		anchor.y = size.height-1;

	return true;
}

void QBlurFilter::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);
}

void QBlurFilter::redo()
{
	if(!initialization)
		return;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());

	blur(original_image,final_image,size,anchor,border_type);

	connection_data->SetDisplayImage(final_image);
}
