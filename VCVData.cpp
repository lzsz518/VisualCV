#include <QUndoview>
#include <QUndoStack>
#include <QAction>
#include "opencv2/highgui.hpp"
#include "CommandInclude/VCVUndoCommand.h"
#include "VCVChildWindow.h"

#include "VCVData.h"

QVCVData::QVCVData()
{
	initicialition = false;
    undo_action = nullptr;
    redo_action = nullptr;
    operation_undostack = nullptr;
}

QVCVData::QVCVData(const QVCVData &VCVData)
{

}

QVCVData::~QVCVData()
{
//    original_image.release();
//    display_image.release();
    if(operation_undostack!=nullptr)
        delete operation_undostack;
    if(undo_action!=nullptr)
        delete undo_action;
    if(redo_action!=nullptr)
        delete redo_action;
}

const QVCVData& QVCVData::operator = (const QVCVData &VCVData)
{
	return *this;
}
	
bool QVCVData::New(int width,int height,int type, int resolution,const Scalar& s)
{
	is_new = true;
	_resolution = resolution;
	
	if((type!=CV_8UC1)||(type!=CV_8UC4))
		return false;

	original_image.create(height,width,type);
    return true;
}

bool QVCVData::Load(const QString &file_name)
{
    original_image = imread(file_name.toStdString().c_str(),-1);
	if(original_image.empty())
    {
		return false;
    }

	display_image.release();
	display_image.create(original_image.rows,original_image.cols,original_image.type());
	original_image.copyTo(display_image);

	image_file_name = file_name;

	is_new = false;

	return true;
}

bool QVCVData::Save()
{
	return true;
}

bool QVCVData::SaveAs(QString &file_name)
{
	return imwrite(file_name.toStdString(),display_image);
}

QVCVUndoCommand* QVCVData::GetStep(int step)
{
    return nullptr;
}

bool QVCVData::AddStep(QVCVUndoCommand *command)
{
    if(command==nullptr)
		return false;

	operation_undostack->push(command);
	return true;
}

bool QVCVData::DeleteStep(int step)
{
	return true;
}

void QVCVData::RegisterViewer(QVCVChildWindow *viewer)
{
    if(viewer!=nullptr)
		data_viewer = viewer;
}

void QVCVData::RegisterViewer(QUndoView *viewer)
{
    if(viewer!=nullptr)
        undo_viewer = viewer;
}

bool QVCVData::Initialize()
{
    if((operation_undostack = new QUndoStack)==nullptr)
		return false;

	undo_action = operation_undostack->createUndoAction(this,tr("&Undo"));
	undo_action->setShortcut(QKeySequence::Undo);
	redo_action = operation_undostack->createRedoAction(this,tr("&Redo"));
	redo_action->setShortcut(QKeySequence::Redo);

	initicialition = true;
	return true;
}

void QVCVData::Notify()
{
	if(!display_image.empty())
        data_viewer->Update(display_image);

    if(operation_undostack!=nullptr)
        undo_viewer->setStack(operation_undostack);
}

int QVCVData::width()
{
	if(display_image.empty())
        return 0;

	return display_image.cols;
}

int QVCVData::height()
{
	if(original_image.empty())
		return 0;

    return display_image.rows;
}

QString QVCVData::GetFileName()
{
	return image_file_name;
}

const Mat& QVCVData::GetDisplayImage()
{
	return display_image;
}

bool QVCVData::SetDisplayImage(Mat &image)
{
	if(image.empty())
        return false;

	display_image.release();
	display_image.create(image.rows,image.cols,image.type());
	image.copyTo(display_image);

    Notify();

    return true;
}

const Mat& QVCVData::GetOriginalImage()
{
    return original_image;
}

QAction* QVCVData::GetUndoAction()
{
	return undo_action;
}

QAction* QVCVData::GetRedoAction()
{
	return redo_action;
}
