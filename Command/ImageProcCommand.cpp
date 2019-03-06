#include "VCVData.h"
#include "Command/ImageProcCommand.h"

QImageProcCommand::QImageProcCommand()
{
	initialization = false;
}

QImageProcCommand::~QImageProcCommand()
{
	original_image.release();
	final_image.release();
}

bool QImageProcCommand::Initialize(QVCVData *data, const CommandParameter *para)
{
    if(data==NULL)
		return false;

	if(!SetParameter(para))
		return false;

	original_image.release();
	final_image.release();

	connection_data = data;

	if(connection_data->GetDisplayImage().empty())
		return false;

	original_image.release();
	original_image = connection_data->GetDisplayImage().clone();

	initialization = true;

	return initialization;
}

bool QImageProcCommand::SetParameter(const CommandParameter *para)
{
	return false;
}

void QImageProcCommand::undo()
{
    if(!initialization)
        return;

    connection_data->SetDisplayImage(original_image);
}
