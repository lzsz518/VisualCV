#ifndef OPENCOMMAND_H
#define OPENCOMMAND_H

#include "SystemDefine.h"
#include "Command/ImageProcCommand.h"


class QBlurFilter : public QImageProcCommand
{
public:
    explicit QBlurFilter();
    ~QBlurFilter();

    virtual bool SetParameter(const CommandParameter *para);

	virtual void undo();
	virtual void redo();

protected:
    int border_type;
	Size size;
	Point anchor;
};

#endif
