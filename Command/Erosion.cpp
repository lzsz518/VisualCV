#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "Command/Erosion.h"

QErosion::QErosion()
{

}

QErosion::~QErosion()
{

}

bool QErosion::SetParameter(const CommandParameter *para)
{
    if(para==NULL)
        return false;
    CommandParameter_Filter *filter_para = (CommandParameter_Filter*)para;
    if(filter_para->iterations%2!=1)
		return false;

    iterations = filter_para->iterations;
    anchor = filter_para->point;

	return true;
}

void QErosion::undo()
{
	if(!initialization)
		return;

	connection_data->SetDisplayImage(original_image);	
}

void QErosion::redo()
{
	if(!initialization)
		return;

    if(anchor.x>=iterations)
        anchor.x = iterations-1;
    if(anchor.y>=iterations)
        anchor.y = iterations-1;

	final_image.release();
	final_image.create(original_image.rows,original_image.cols,original_image.type());
	
	Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(iterations, iterations),anchor );

	erode(original_image,final_image,kernel,anchor,iterations);

	connection_data->SetDisplayImage(final_image);
}
