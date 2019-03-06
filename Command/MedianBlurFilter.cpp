#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "Command/MedianBlurFilter.h"

QMedianBlurFilter::QMedianBlurFilter()
{
		initialization = false;
}

QMedianBlurFilter::~QMedianBlurFilter()
{

}

bool QMedianBlurFilter::SetParameter(const CommandParameter *para)
{
    if(para==NULL)
        return false;
    CommandParameter_Filter *filter_para = (CommandParameter_Filter*)para;

    if(filter_para->ksize%2!=1)
		return false;

    ksize = filter_para->ksize;
    return true;
}

void QMedianBlurFilter::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);	
}

void QMedianBlurFilter::redo()
{
	if(!initialization)
		return;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());

	medianBlur(original_image,final_image,ksize);

	connection_data->SetDisplayImage(final_image);
}
