#include "opencv2/imgproc.hpp"
#include "VCVData.h"
#include "SystemDefine.h"
#include "Command/edgedetection.h"

QLaplaction::QLaplaction()
{

}

QLaplaction::~QLaplaction()
{

}

bool QLaplaction::SetParameter(const CommandParameter *para)
{
   if(para==NULL)
       return false;

   CommandParameter_Edge *edge_para = (CommandParameter_Edge*)para;

   ddepth = edge_para->ddepth;
   ksize = edge_para->ksize;
   scale = edge_para->scale;
   delta = edge_para->delta;
   bordertype = edge_para->bordertype;

   return true;
}

void QLaplaction::undo()
{
    if(!initialization)
        return;

    connection_data->SetDisplayImage(original_image);
}

void QLaplaction::redo()
{
    if(!initialization)
        return;

    final_image.release();
    final_image.create(original_image.rows,original_image.cols,original_image.type());

    Laplacian(original_image,final_image,CV_8UC1,ksize,scale,delta,bordertype);

    connection_data->SetDisplayImage(final_image);
}

QSobel::QSobel()
{

}

QSobel::~QSobel()
{

}

bool QSobel::SetParameter(const CommandParameter *para)
{
    if(para==NULL)
        return false;

    CommandParameter_Edge* edge_para = (CommandParameter_Edge*)para;

    ddepth = edge_para->ddepth;
    ksize = edge_para->ksize;
    dx = edge_para->dx;
    dy = edge_para->dy;
    bordertype = edge_para->bordertype;
    scale = edge_para->scale;
    delta = edge_para->delta;

    return true;
}

void QSobel::redo()
{
    if(!initialization)
        return;

    final_image.release();
    final_image.create(original_image.rows,original_image.cols,original_image.type());

    Mat grad_x,grad_y;

    Sobel(original_image,grad_x,CV_16S,1,0,ksize,scale,delta,bordertype);
    Sobel(original_image,grad_y,CV_16S,0,1,ksize,scale,delta,bordertype);

    convertScaleAbs(grad_x,grad_x);
    convertScaleAbs(grad_y,grad_y);

    addWeighted(grad_x,0.5,grad_y,0.5,0,final_image);

    connection_data->SetDisplayImage(final_image);
}

void QSobel::undo()
{
    if(!initialization)
        return;

    connection_data->SetDisplayImage(original_image);
}


QCanny::QCanny()
{

}

QCanny::~QCanny()
{

}

bool QCanny::SetParameter(const CommandParameter *para)
{
    if(para==NULL)
        return false;

    CommandParameter_Edge *edge_para = (CommandParameter_Edge*)para;

    aperturesize = edge_para->aperturesize;
    l2gradient = edge_para->l2gradient;
    threshold1 = edge_para->scale;
    threshold2 = edge_para->delta;

    return true;
}

void QCanny::undo()
{
    if(!initialization)
        return;

    connection_data->SetDisplayImage(original_image);
}

void QCanny::redo()
{
    if(!initialization)
        return;

    final_image.release();
    final_image.create(original_image.rows,original_image.cols,original_image.type());

    Canny(original_image,final_image,threshold1,threshold2,aperturesize,l2gradient);

    connection_data->SetDisplayImage(final_image);
}

