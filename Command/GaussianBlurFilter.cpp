#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "Command/GaussianBlurFilter.h"

GaussianBlurFilter::GaussianBlurFilter()
{
	//initialization = false;
}

GaussianBlurFilter::~GaussianBlurFilter()
{

}

bool GaussianBlurFilter::SetParameter(const CommandParameter *para)
{
    if(para==NULL)
        return false;
    CommandParameter_Filter *filter_para = (CommandParameter_Filter*)para;
    if((filter_para->size.width%2==0)||(filter_para->size.height%2==0))
		return false;

    size = filter_para->size;
    sigma_x = filter_para->sigmacolor;
    sigma_y = filter_para->sigmaspace;
    border_type = filter_para->bordertype;

	return true;
}

void GaussianBlurFilter::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);
}

void GaussianBlurFilter::redo()
{
	if(!initialization)
		return;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());
	GaussianBlur(original_image,final_image,size,sigma_x,sigma_y,border_type);

	connection_data->SetDisplayImage(final_image);
}





