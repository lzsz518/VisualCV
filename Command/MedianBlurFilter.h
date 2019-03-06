#ifndef MEDIANBLURFILTER_H
#define MEDIANBLURFILTER_H

#include "SystemDefine.h"
#include "Command/ImageProcCommand.h"

class QVCVData;

class QMedianBlurFilter : public QImageProcCommand
{
public:
	QMedianBlurFilter();
	~QMedianBlurFilter();

    virtual bool SetParameter(const CommandParameter *para);

	virtual void undo();
	virtual void redo();

protected:
	int ksize;
};

#endif
