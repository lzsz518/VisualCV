#include <QRadioButton>
#include "Command/BlurFilter.h"

#include "FilterPanel.h"

QFilterPanel::QFilterPanel(QWidget *parent, Qt::WindowFlags f)
{
    ui.setupUi(this);
    setWindowFlags(f);
	CreateBorderType();

	Disable();

    command_para = new CommandParameter_Filter;

	connect(ui.dsp_delta,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
	connect(ui.dsp_scale,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
	connect(ui.dsp_sigmacolor,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
	connect(ui.dsp_sigmaspace,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
	connect(ui.dsp_sigmax,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
	connect(ui.dsp_sigmay,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));

	connect(ui.sp_height,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
	connect(ui.sp_width,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
	connect(ui.sp_x,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
	connect(ui.sp_y,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
	connect(ui.sp_iterations,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
	connect(ui.sp_ksize,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));

	connect(ui.pb_Ok,SIGNAL(clicked(bool)),this,SLOT(PushOk()));
	connect(ui.pb_Cancel,SIGNAL(clicked(bool)),this,SLOT(PushCancel()));
}

QFilterPanel::~QFilterPanel()
{
    if(command_para!=NULL)
        delete command_para;
}

double QFilterPanel::GetSigmaColor()
{
    return ui.dsp_sigmacolor->value();
}

double QFilterPanel::GetSigmaSpace()
{
    return ui.dsp_sigmaspace->value();
}

double QFilterPanel::GetScale()
{
    return ui.dsp_scale->value();
}

double QFilterPanel::GetDelta()
{
    return ui.dsp_delta->value();
}

Size QFilterPanel::GetSize()
{
    return Size(ui.sp_width->value(),ui.sp_height->value());
}

Point QFilterPanel::GetPoint()
{
    return Point(ui.sp_x->value(),ui.sp_y->value());
}

int QFilterPanel::GetIterations()
{
    return ui.sp_iterations->value();
}

int QFilterPanel::GetKSize()
{
    return ui.sp_ksize->value();
}

int QFilterPanel::GetBorderType()
{
	return BORDER_DEFAULT;	//temp

	if(rb_default->isChecked())
		return BORDER_DEFAULT;
		
	if(rb_wrap->isChecked())
		return BORDER_WRAP;

	if(rb_replicate->isChecked())
		return BORDER_REPLICATE;

	if(rb_reflect->isChecked())
		return BORDER_REFLECT;

	if(rb_replicate101->isChecked())
		return BORDER_REFLECT_101;
		
	if(rb_reflect101->isChecked())
		return BORDER_REFLECT101;

	if(rb_constant->isChecked())
		return BORDER_CONSTANT;

	if(rb_transparent->isChecked())
		return BORDER_TRANSPARENT;

	if(rb_isolated->isChecked())
		return BORDER_ISOLATED;

	return BORDER_DEFAULT;
}

void QFilterPanel::BeginOperation(VCV_IMAGE_OPERATION operation)
{
    //image_operation = operation;
	switch(operation)
	{
	case IMAGE_FILTER_BLUR:
		EnableBlur();
		break;
	case IMAGE_FILTER_GAUSSIANBLUR:
		EnableGaussianBlur();
		break;
	case IMAGE_FILTER_MEDIANBLUR:
		EnableMedianBlur();
		break;
	case IMAGE_FILTER_BILATERAL:
		EnableBilateral();
		break;
	case IMAGE_FILTER_EROSION:
		EnableErosion();
		break;
	case IMAGE_FILTER_DILATION:
		EnableErosion();
		break;
	default:
		break ;
	}
    QControlPanel::SetOperation(operation);
}

void QFilterPanel::EndOperation(VCV_IMAGE_OPERATION operation)
{
	switch(operation)
	{
	case IMAGE_FILTER_BLUR:
		DisableBlur();
		break;
	case IMAGE_FILTER_GAUSSIANBLUR:
		DisableGaussianBlur();
		break;
	case IMAGE_FILTER_MEDIANBLUR:
		DisableMedianBlur();
		break;
	case IMAGE_FILTER_BILATERAL:
		DisableBilateral();
		break;
	case IMAGE_FILTER_EROSION:
		DisableErosion();
		break;
	case IMAGE_FILTER_DILATION:
		DisableErosion();
		break;
	default:
		break ;
	}

    SetOperation(IMAGE_NONE);
}

void QFilterPanel::ValueChange()
{
    GetAllParameter();

    emit ParameterChange(command_para);
}

void QFilterPanel::PushOk()
{
    EndOperation(GetOperation());

    GetAllParameter();

    emit PanelOk(command_para);
    close();
}

void QFilterPanel::PushCancel()
{
    EndOperation(GetOperation());

    GetAllParameter();

    emit PanelCancel(command_para);
    close();
}

void QFilterPanel::CreateBorderType()
{
	rb_default = new QRadioButton(tr("BORDER_DEFAULT"));
	rb_wrap = new QRadioButton(tr("BORDER_WRAP"));
	rb_replicate = new QRadioButton(tr("BORDER_DEFAULT"));
	rb_reflect = new QRadioButton(tr("BORDER_REFLECT"));
	rb_replicate101 = new QRadioButton(tr("BORDER_REFLECT_101"));
	rb_reflect101 = new QRadioButton(tr("BORDER_REFLECT101"));
	rb_constant = new QRadioButton(tr("BORDER_CONSTANT"));
	rb_transparent = new QRadioButton(tr("BORDER_TRANSPARENT"));
	rb_isolated = new QRadioButton(tr("BORDER_ISOLATED"));
	QGridLayout *gridbox = new QGridLayout;

	gridbox->addWidget(rb_default,0,0);
	gridbox->addWidget(rb_wrap,1,0);
	gridbox->addWidget(rb_replicate,2,0);
	gridbox->addWidget(rb_reflect,3,0);
	gridbox->addWidget(rb_replicate101,4,0);
	gridbox->addWidget(rb_reflect101,0,1);
	gridbox->addWidget(rb_constant,1,1);
	gridbox->addWidget(rb_transparent,2,1);
	gridbox->addWidget(rb_isolated,3,1);

	ui.gb_BoardType->setLayout(gridbox);

}

void QFilterPanel::GetAllParameter()
{
    ((CommandParameter_Filter*)command_para)->delta = GetDelta();
    ((CommandParameter_Filter*)command_para)->iterations =GetIterations();
    ((CommandParameter_Filter*)command_para)->ksize = GetKSize();
    ((CommandParameter_Filter*)command_para)->point = GetPoint();
    ((CommandParameter_Filter*)command_para)->scale = GetScale();
    ((CommandParameter_Filter*)command_para)->sigmacolor = GetSigmaColor();
    ((CommandParameter_Filter*)command_para)->sigmaspace = GetSigmaSpace();
    ((CommandParameter_Filter*)command_para)->size = GetSize();
    ((CommandParameter_Filter*)command_para)->bordertype = GetBorderType();
}

void QFilterPanel::Disable()
{
	ui.dsp_delta->setEnabled(false);
	ui.dsp_scale->setEnabled(false);
	ui.dsp_sigmacolor->setEnabled(false);
	ui.dsp_sigmaspace->setEnabled(false);
	ui.dsp_sigmax->setEnabled(false);
	ui.dsp_sigmay->setEnabled(false);
	ui.sp_height->setEnabled(false);
	ui.sp_width->setEnabled(false);
	ui.sp_x->setEnabled(false);
	ui.sp_y->setEnabled(false);
	ui.sp_iterations->setEnabled(false);
	ui.sp_ksize->setEnabled(false);
	ui.gb_BoardType->setEnabled(false);
	ui.pb_Ok->setEnabled(false);
	ui.pb_Cancel->setEnabled(false);
}

void QFilterPanel::EnableBlur()
{
	Disable();

	ui.sp_width->setEnabled(true);
	ui.sp_height->setEnabled(true);
	ui.sp_x->setEnabled(true);
	ui.sp_y->setEnabled(true);
	ui.pb_Ok->setEnabled(true);
	ui.pb_Cancel->setEnabled(true);
	ui.gb_BoardType->setEnabled(true);
	ui.sp_width->setMinimum(1);
	ui.sp_height->setMinimum(1);
	ui.sp_width->setValue(0);
	ui.sp_height->setValue(0);
	ui.sp_x->setMinimum(-1);
	ui.sp_y->setMinimum(-1);
	ui.sp_x->setValue(-1);
	ui.sp_y->setValue(-1);
	
	connect(ui.sp_width,SIGNAL(valueChanged(int)),ui.sp_height,SLOT(setValue(int)));
	connect(ui.sp_height,SIGNAL(valueChanged(int)),ui.sp_width,SLOT(setValue(int)));
}

void QFilterPanel::DisableBlur()
{
	Disable();

	disconnect(ui.sp_width,SIGNAL(valueChanged(int)),ui.sp_height,SLOT(setValue(int)));
	disconnect(ui.sp_height,SIGNAL(valueChanged(int)),ui.sp_width,SLOT(setValue(int)));
}

void QFilterPanel::EnableGaussianBlur()
{
	Disable();

	ui.dsp_sigmacolor->setEnabled(true);
	ui.dsp_sigmaspace->setEnabled(true);
	ui.sp_width->setEnabled(true);
	ui.sp_height->setEnabled(true);
	ui.pb_Ok->setEnabled(true);
	ui.pb_Cancel->setEnabled(true);
	ui.gb_BoardType->setEnabled(true);
	ui.dsp_sigmacolor->setMinimum(0);
	ui.dsp_sigmaspace->setMinimum(0);
	ui.dsp_sigmacolor->setValue(1);
	ui.sp_width->setMinimum(1);
	ui.sp_height->setMinimum(1);
	ui.sp_width->setValue(1);
	ui.sp_height->setValue(1);
	ui.sp_width->setSingleStep(2);
	ui.sp_height->setSingleStep(2);
}

void QFilterPanel::DisableGaussianBlur()
{
	ui.sp_width->setSingleStep(1);
	ui.sp_height->setSingleStep(1);
	Disable();
}

void QFilterPanel::EnableMedianBlur()
{
	Disable();
	ui.sp_ksize->setEnabled(true);
	ui.pb_Ok->setEnabled(true);
	ui.pb_Cancel->setEnabled(true);
	ui.sp_ksize->setMinimum(1);
	ui.sp_ksize->setValue(1);
	ui.sp_ksize->setSingleStep(2);
}

void QFilterPanel::DisableMedianBlur()
{
	ui.sp_ksize->setMinimum(0);
	ui.sp_ksize->setSingleStep(1);
	Disable();
}

void QFilterPanel::EnableBilateral()
{
	Disable();
	ui.dsp_sigmacolor->setEnabled(true);
	ui.dsp_sigmaspace->setEnabled(true);
	ui.sp_ksize->setEnabled(true);
	ui.pb_Ok->setEnabled(true);
	ui.pb_Cancel->setEnabled(true);
}

void QFilterPanel::DisableBilateral()
{
	Disable();
}

void QFilterPanel::EnableErosion()
{
	Disable();
	ui.sp_x->setEnabled(true);
	ui.sp_y->setEnabled(true);
	ui.sp_iterations->setEnabled(true);
	ui.pb_Ok->setEnabled(true);
	ui.pb_Cancel->setEnabled(true);
	ui.sp_x->setMinimum(-1);
	ui.sp_y->setMinimum(-1);
	ui.sp_iterations->setMinimum(1);
	ui.sp_iterations->setSingleStep(2);
}

void QFilterPanel::DisableErosion()
{
	Disable();
}
