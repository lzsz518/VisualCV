#ifndef BILATERALFILTER_H
#define BILATERALFILTER_H

#include "SystemDefine.h"
#include "Command/ImageProcCommand.h"

class QBilateralFilter : public QImageProcCommand
{
public:
	QBilateralFilter();
	~QBilateralFilter();

    virtual bool SetParameter(const CommandParameter *para);

	virtual void undo();
	virtual void redo();

protected:
	int d;
	int border_type;
	double sigma_color;
	double sigma_space;
};

#endif
