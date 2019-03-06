#ifndef GAUSSIANBLURFILTER_H
#define GAUSSIANBLURFILTER_H

#include "SystemDefine.h"
#include "Command/ImageProcCommand.h"

class GaussianBlurFilter : public QImageProcCommand
{
public:
	GaussianBlurFilter();
	~GaussianBlurFilter();

    virtual bool SetParameter(const CommandParameter *para);

	virtual void undo();
	virtual void redo();

protected:
	Size size;
	double sigma_x;
	double sigma_y;
	int border_type;
};

#endif
