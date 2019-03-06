#ifndef COMMANDBUILDER_H
#define COMMANDBUILDER_H

#include "SystemDefine.h"
class QVCVUndoCommand;

class QCommandBuilder
{
public:
	QCommandBuilder();
	~QCommandBuilder();

    QVCVUndoCommand* CreateCommand(VCV_IMAGE_OPERATION operation);
};


#endif
