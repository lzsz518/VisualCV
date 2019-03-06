#include "VCVData.h"
#include "SystemDefine.h"
#include "Command/Morphology.h"

QMorphology::QMorphology()
{

}

QMorphology::~QMorphology()
{

}

bool QMorphology::SetParameter(const CommandParameter *para)
{
    if(para==NULL)
        return false;
    CommandParameter_Morphology *p = (CommandParameter_Morphology*)para;
    op = p->op;
    shape = p->shape;
    ksize = p->ksize;
    kernelAnchor = p->kernelAnchor;
    anchor = p->anchor;
    iteration = p->iteration;
    borderType = p->borderType;
    tp1 = p->tp1;
    tp2 = p->tp2;
    tp3 = p->tp3;
    tp4 = p->tp4;

    return true;
}

void QMorphology::redo()
{
    if(!initialization)
        return;

    if(kernelAnchor.x>=ksize)
        kernelAnchor.x = ksize-1;
    if(kernelAnchor.y>ksize)
        kernelAnchor.y = ksize-1;

    if(anchor.x>=original_image.cols)
        anchor.x = original_image.cols-1;
    if(anchor.y>=ksize)
        anchor.y = original_image.rows-1;

    final_image.release();
    final_image.create(original_image.rows,original_image.cols,original_image.type());

    Mat kernel = getStructuringElement(shape,Size(ksize, ksize),kernelAnchor);

    if(op==MORPH_HITMISS)
    {
        Mat temp;
        cvtColor(original_image,temp,CV_BGR2GRAY);
        morphologyEx(temp,final_image,op,kernel,anchor,iteration,borderType,Scalar(tp1,tp2,tp3,tp4));
    }
    else
        morphologyEx(original_image,final_image,op,kernel,anchor,iteration,borderType,Scalar(tp1,tp2,tp3,tp4));

    connection_data->SetDisplayImage(final_image);
}
