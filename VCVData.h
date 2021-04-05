#ifndef VCVDATA_H
#define VCVDATA_H
#include <QObject>
#include <vector>
#include <QImage>

#include "cxcore.h"
#include "highgui.h"

#include "SystemDefine.h"

class QAction;
class QUndoStack;
class QUndoView;
class QVCVChildWindow;
class QVCVUndoCommand;

class QVCVData : public QObject
{
	Q_OBJECT
public:
	QVCVData();
	QVCVData(const QVCVData &VCVData);
	~QVCVData();

public:
	const QVCVData& operator = (const QVCVData &VCVData);
	
	bool Initialize();

	void Notify();

	void RegisterViewer(QVCVChildWindow *viewer);
    void RegisterViewer(QUndoView *viewer);
    bool New(int width,int height,int type , int resolution,const Scalar& s = Scalar(1,1,1,1));
	bool Load(const QString &file_name);
	bool Save();
	bool SaveAs(QString &filen_ame);

	QVCVUndoCommand* GetStep(int step);
	bool AddStep(QVCVUndoCommand *command);
	bool DeleteStep(int step);

	QString GetFileName();

	int width();
	int height();

    const Mat &GetDisplayImage();
	bool SetDisplayImage(Mat &image);
	
    const Mat &GetOriginalImage();

	QAction* GetUndoAction();
	QAction* GetRedoAction();

    vector<Rect>* GetSpriteRegionList();

protected:
	bool initicialition;
	bool is_new;

	int _resolution;

	QString image_file_name;

    Mat original_image;
    Mat display_image;
	
	QAction *undo_action;
	QAction *redo_action;
	QUndoStack *operation_undostack;

	QVCVChildWindow *data_viewer;
    QUndoView *undo_viewer;
    std::vector<Rect> sprite_region;
};

#endif
