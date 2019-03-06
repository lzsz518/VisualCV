#ifndef DILATION_H
#define DILATION_H

#include "SystemDefine.h"
#include "Command/ImageProcCommand.h"

class QDilation : public QImageProcCommand
{
public:
	QDilation();
	~QDilation();

    virtual bool SetParameter(const CommandParameter *para);

	virtual void undo();
	virtual void redo();

protected:
	Point anchor;
	int iterations;
};

#endif
