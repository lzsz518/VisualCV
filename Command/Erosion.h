#ifndef EROSION_H
#define EROSION_H

#include "SystemDefine.h"
#include "Command/ImageProcCommand.h"

class QErosion : public QImageProcCommand
{
public:
	QErosion();
	~QErosion();

    virtual bool SetParameter(const CommandParameter *para);

	virtual void undo();
	virtual void redo();

protected:
	Point anchor;
	int iterations;
};

#endif
