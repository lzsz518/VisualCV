#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "opencv2/imgproc/imgproc.hpp"
#include "ImageProcCommand.h"

using namespace cv;

class CommandParameter;

class QThreshold : public QImageProcCommand
{
public:
    QThreshold();
    ~QThreshold();

    virtual bool SetParameter(const CommandParameter *para);

    virtual void undo();
    virtual void redo();

protected:
    double thresh;
    double max;
    ThresholdTypes type;
};

class QAdaptiveThreshold : public QImageProcCommand
{
public:
    QAdaptiveThreshold();
    ~QAdaptiveThreshold();

    virtual bool SetParameter(const CommandParameter *para);

    virtual void undo();
    virtual void redo();

protected:
    int  	thresholdType;
    int  	blockSize;
    double  	maxValue;
    double 	constant;
    AdaptiveThresholdTypes  	adaptiveMethod;
};
#endif // THRESHOLD_H
