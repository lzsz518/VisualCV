#ifndef CUSTOMFILTER2D_H
#define CUSTOMFILTER2D_H

#include "SystemDefine.h"
#include "Command/ImageProcCommand.h"


class QCustomFilter2D : public QImageProcCommand
{
public:
	QCustomFilter2D();
	~QCustomFilter2D();

    virtual bool SetParameter(const CommandParameter *para);

	virtual void undo();
	virtual void redo();

protected:
    int depth;
	int border_type;
	double delta;
	Point anchor;
	Mat kernel;
};

#endif

