#ifndef VCVUNDOCOMMAND_H
#define VCVUNDOCOMMAND_H

#include <QUndoCommand>

class QVCVData;
struct CommandParameter;

class QVCVUndoCommand : public QUndoCommand
{
public:
	QVCVUndoCommand();
	~QVCVUndoCommand();

    virtual bool Initialize(QVCVData *data, const CommandParameter *para)=0;
    virtual bool SetParameter(const CommandParameter *para);
	bool IsInit();

protected:
	bool initialization;
};

#endif
