#include <QBrush>
#include <QPainter>
#include <QVariant>
#include <QPushButton>
#include <QAbstractItemModel>
#include <QModelIndex>
#include "opencv2/imgproc.hpp"
#include "Utility.h"
#include "CustomFilterDlg.h"

QCustomFilterDlg::QCustomFilterDlg(QWidget *parent , Qt::WindowFlags f )
{
    setWindowFlags(f);
	initialization = false;
    command_para = new CommandParameter_Filter;
}

QCustomFilterDlg::~QCustomFilterDlg()
{
	src_mat.release();
	dst_mat.release();

    if(command_para!=NULL)
        delete command_para;
}

bool QCustomFilterDlg::Initialize(const Mat &mat)
{
	if(mat.empty())
		return false;

    mat.copyTo(src_mat);
    mat.copyTo(dst_mat);

	InitUI();

	initialization = true;

	return true;
}

bool QCustomFilterDlg::IsInit()
{
	return initialization;
}

Point QCustomFilterDlg::GetAnchor()
{
	return Point(ui.sp_anchorx->value(),ui.sp_anchory->value());
}

int QCustomFilterDlg::GetDepth()
{
	return ui.sp_depth->value();
}

double QCustomFilterDlg::GetDelta()
{
	return ui.dsp_delta->value();
}

Mat QCustomFilterDlg::GetKernel()
{
	GetKernelFromUI();

	return kernel;
}

void QCustomFilterDlg::InitUI()
{
	ui.setupUi(this);

	connect(ui.sp_width,SIGNAL(valueChanged(int)),ui.sp_height,SLOT(setValue(int)));
	connect(ui.sp_height,SIGNAL(valueChanged(int)),ui.sp_width,SLOT(setValue(int)));
	connect(ui.sp_width,SIGNAL(valueChanged(int)),this,SLOT(KernelSizeChange()));
	connect(ui.sp_height,SIGNAL(valueChanged(int)),this,SLOT(KernelSizeChange()));

    connect(ui.pb_Ok,SIGNAL(clicked()),this,SLOT(PushOk()));
    connect(ui.pb_apply,SIGNAL(clicked()),this,SLOT(PushApply()));
    connect(ui.pb_cancel,SIGNAL(clicked()),this,SLOT(PushCancel()));


	ui.tableWidget->setItemDelegate(&sp_delegate);
	ui.sp_width->setMinimum(3);
	ui.sp_height->setMinimum(3);
	ui.sp_width->setSingleStep(2);
	ui.sp_height->setSingleStep(2);
	ui.sp_depth->setMinimum(-1);
	ui.sp_depth->setValue(-1);
	ui.sp_anchorx->setMinimum(-100);
	ui.sp_anchory->setMinimum(-100);
	ui.sp_anchorx->setValue(-1);
	ui.sp_anchory->setValue(-1);
	ui.tableWidget->setColumnCount(ui.sp_width->value());
	ui.tableWidget->setRowCount(ui.sp_width->value());
}

void QCustomFilterDlg::PushOk()
{
    GetKernelFromUI();
    command_para->delta = ui.dsp_delta->value();
    command_para->depth = ui.sp_depth->value();
    command_para->point = GetAnchor();
    command_para->kernel = kernel;
    command_para->bordertype = BORDER_DEFAULT;
    emit PanelOk(command_para);
//    src_mat.release();
//	dst_mat.release();
}

void QCustomFilterDlg::PushCancel()
{
	src_mat.release();
	dst_mat.release();

    command_para->delta = ui.dsp_delta->value();
    command_para->point = GetAnchor();
    command_para->point = GetAnchor();
    command_para->kernel = kernel;
    command_para->bordertype = BORDER_DEFAULT;

    emit PanelCancel(command_para);
}

void QCustomFilterDlg::PushApply()
{
	GetKernelFromUI();
	if(kernel.empty())
		return;

	double delta;
	Point anchor;
	delta = ui.dsp_delta->value();
	anchor.x = ui.sp_anchorx->value();
	anchor.y = ui.sp_anchory->value();
    filter2D(src_mat,dst_mat,ui.sp_depth->value(),kernel,anchor,delta);

    repaint();

    command_para->delta = ui.dsp_delta->value();
    command_para->depth = ui.sp_depth->value();
    command_para->point = GetAnchor();
    command_para->kernel = kernel;
    command_para->bordertype = BORDER_DEFAULT;
    emit PanelOk(command_para);
}

void QCustomFilterDlg::KernelSizeChange()
{
	if(ui.sp_width->value()!=ui.sp_height->value())
		return;

	//if((ui.tableWidget->rowCount()<3)||(ui.tableWidget->columnCount()<3))
	//	return;

	ui.tableWidget->setColumnCount(ui.sp_width->value());
	ui.tableWidget->setRowCount(ui.sp_height->value());

	int i;

	for(i=0;i<ui.tableWidget->columnCount();++i)
	{
		ui.tableWidget->setColumnWidth(i,CUSTOM_FILTER_TABLEWIDGET_ITEM_WIDTH);
	}
}

void QCustomFilterDlg::paintEvent(QPaintEvent *e)
{
	//QPainter painter(ui.image_label);

	QImage *display_image = MatToQImage(dst_mat);

	QPixmap pixmap;
	pixmap.convertFromImage(*display_image);

	ui.image_label->setPixmap(pixmap.scaled(300,300));

	//painter.drawImage(0,0,*display_image);

	delete display_image;
}

void QCustomFilterDlg::GetKernelFromUI()
{
	kernel.release();
	kernel = Mat::ones(ui.sp_height->value(),ui.sp_width->value(),CV_32F);
	int i,j;
	float *ptr;
	QAbstractItemModel *model = ui.tableWidget->model();
	QModelIndex model_index;
	for(i=0;i<ui.tableWidget->rowCount();++i)
	{
		ptr = kernel.ptr<float>(i);
		for(j=0;j<ui.tableWidget->columnCount();++j)
		{
			model_index = model->index(i,j);
			if(model_index.data(Qt::DisplayRole).toString()=="")
			{
				kernel.release();
				return;
			}
			ptr[j]  = model_index.data(Qt::DisplayRole).toFloat();
		} 
	}
}
