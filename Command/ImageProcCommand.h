#ifndef IMAGEPROCCOMMAND_H
#define IMAGEPROCCOMMAND_H

#include "SystemDefine.h"
#include "Command/VCVUndoCommand.h"

class QVCVData;

class QImageProcCommand : public QVCVUndoCommand
{
public:
	QImageProcCommand();
	~QImageProcCommand();

    bool Initialize(QVCVData *data, const CommandParameter* para);
    virtual bool SetParameter(const CommandParameter *para);

    virtual void undo();

protected:
    Mat original_image;
	Mat final_image;
    QVCVData *connection_data;
};

#endif
