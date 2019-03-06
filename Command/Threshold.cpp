#include "VCVData.h"
#include "SystemDefine.h"
#include "Command/Threshold.h"

QThreshold::QThreshold()
{
   thresh = 0;
   max = 0;
   type = THRESH_BINARY;
}

QThreshold::~QThreshold()
{
    original_image.release();
    final_image.release();
}

bool QThreshold::SetParameter(const CommandParameter *para)
{
   if(para==NULL)
       return false;
   CommandParameter_Threshold* p = (CommandParameter_Threshold*)para;

   thresh = p->thresh;
   max = p->max;
   type = p->threshold_type;

   return true;
}

void QThreshold::redo()
{
    if(!initialization)
        return;

    final_image.release();
    final_image.create(original_image.rows,original_image.cols,CV_8UC1);
    if(original_image.type()!=CV_8UC1)
    {
        Mat temp = Mat::zeros(original_image.rows,original_image.cols,CV_8UC1);
        cvtColor(original_image,temp,COLOR_BGR2GRAY);
        threshold(temp,final_image,thresh,max,type);
    }
    else
        threshold(original_image,final_image,thresh,max,type);

    connection_data->SetDisplayImage(final_image);
}

void QThreshold::undo()
{
    if(!initialization)
        return;

    connection_data->SetDisplayImage(original_image);
}

QAdaptiveThreshold::QAdaptiveThreshold()
{
    thresholdType = THRESH_BINARY;
    blockSize = 3;
    maxValue = 0;
    constant = 0;
    adaptiveMethod = ADAPTIVE_THRESH_MEAN_C;
}

QAdaptiveThreshold::~QAdaptiveThreshold()
{
    original_image.release();
    final_image.release();
}

bool QAdaptiveThreshold::SetParameter(const CommandParameter *para)
{
    if(para==NULL)
        return false;

    CommandParameter_Threshold *p = (CommandParameter_Threshold*)para;
    thresholdType = p->threshold_type;
    blockSize = p->block_size;
    maxValue = p->max;
    constant = p->constant;
    adaptiveMethod = p->adaptive_type;

    return true;
}

void QAdaptiveThreshold::undo()
{
     if(!initialization)
        return;

    connection_data->SetDisplayImage(original_image);
}

void QAdaptiveThreshold::redo()
{
     if(!initialization)
        return;

    final_image.release();
    final_image.create(original_image.rows,original_image.cols,CV_8UC1);

    if(original_image.type()!=CV_8UC1)
    {
        Mat temp = Mat::zeros(original_image.rows,original_image.cols,CV_8UC1);
        cvtColor(original_image,temp,COLOR_BGR2GRAY);
        adaptiveThreshold(temp,final_image,maxValue,adaptiveMethod,thresholdType,blockSize,constant);
    }
    else
       adaptiveThreshold(original_image,final_image,maxValue,adaptiveMethod,thresholdType,blockSize,constant);

    connection_data->SetDisplayImage(final_image);
}
