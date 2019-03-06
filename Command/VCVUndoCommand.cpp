#include "SystemDefine.h"

#include "Command/VCVUndoCommand.h"

QVCVUndoCommand::QVCVUndoCommand()
{
	initialization = false;
}

QVCVUndoCommand::~QVCVUndoCommand()
{

}

	
bool QVCVUndoCommand::Initialize(QVCVData *data, const CommandParameter *para)
{
	return false;
}

bool QVCVUndoCommand::SetParameter(const CommandParameter *para)
{
	return false;
}

bool QVCVUndoCommand::IsInit()
{
	return initialization;
}

