#include "ThresholdPanel.h"

QThresholdPanel::QThresholdPanel(QWidget *parent,Qt::WindowFlags f)
{
    ui.setupUi(this);
    ui.rb_binary->setChecked(true);
    ui.rb_mean_c->setChecked(true);
    setWindowFlags(f);
    command_para = new CommandParameter_Threshold;

    connect(ui.dsp_thresh,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
    connect(ui.dsp_max,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
    connect(ui.dsp_constant,SIGNAL(valueChanged(double)),this,SLOT(ValueChange()));
    connect(ui.sp_blocksize,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.rb_binary,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    connect(ui.rb_binary_inv,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    connect(ui.rb_trunc,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    connect(ui.rb_tozero,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    connect(ui.rb_tozero_inv,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    connect(ui.rb_mask,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    connect(ui.rb_otsu,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    connect(ui.rb_triangle,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    connect(ui.rb_mean_c,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    connect(ui.rb_gaussian_c,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    connect(ui.pb_Ok,SIGNAL(clicked(bool)),this,SLOT(PushOk()));
    connect(ui.pb_Cancel,SIGNAL(clicked(bool)),this,SLOT(PushCancel()));
}

QThresholdPanel::~QThresholdPanel()
{
    if(command_para!=NULL)
        delete command_para;
}

void QThresholdPanel::BeginOperation(VCV_IMAGE_OPERATION operation)
{
//    image_operation = operation;
    switch(operation)
    {
    case IMAGE_THRESHOLD_THRESHOLD:
        EnableThreshold();
        break;
    case IMAGE_THRESHOLD_ADAPTIVE:
        EnableAdaptive();
        break;
    default:
        break;
    }

    QControlPanel::SetOperation(operation);
}

void QThresholdPanel::EndOperation(VCV_IMAGE_OPERATION operation)
{
    DisableAll();
    SetOperation(IMAGE_NONE);
}

void QThresholdPanel::ValueChange()
{
    GetAllParameter();

    emit ParameterChange(command_para);
}

void QThresholdPanel::PushOk()
{
    GetAllParameter();

    emit PanelOk(command_para);

    DisableAll();
    close();
}

void QThresholdPanel::PushCancel()
{
    GetAllParameter();

    emit PanelCancel(command_para);

    DisableAll();
    close();
}

void QThresholdPanel::EnableThreshold()
{
    DisableAll();
    ui.dsp_thresh->setEnabled(true);
    ui.dsp_max->setEnabled(true);
    ui.ThresholdTypeGroup->setEnabled(true);
    ui.rb_otsu->setEnabled(true);
    ui.rb_tozero->setEnabled(true);
    ui.rb_tozero_inv->setEnabled(true);
    ui.rb_triangle->setEnabled(true);
    ui.rb_trunc->setEnabled(true);
    ui.rb_mask->setEnabled(true);
}

void QThresholdPanel::EnableAdaptive()
{
    DisableAll();
    ui.rb_binary->setChecked(true);
    ui.dsp_constant->setEnabled(true);
    ui.dsp_max->setEnabled(true);
    ui.sp_blocksize->setEnabled(true);
    ui.ThresholdTypeGroup->setEnabled(true);
    ui.AdaptiveTypeGroup->setEnabled(true);
    ui.rb_otsu->setEnabled(false);
    ui.rb_tozero->setEnabled(false);
    ui.rb_tozero_inv->setEnabled(false);
    ui.rb_triangle->setEnabled(false);
    ui.rb_trunc->setEnabled(false);
    ui.rb_mask->setEnabled(false);
}

void QThresholdPanel::DisableAll()
{
    ui.dsp_thresh->setEnabled(false);
    ui.dsp_max->setEnabled(false);
    ui.dsp_constant->setEnabled(false);
    ui.sp_blocksize->setEnabled(false);
    ui.ThresholdTypeGroup->setEnabled(false);
    ui.AdaptiveTypeGroup->setEnabled(false);
}

void QThresholdPanel::GetAllParameter()
{
    if(command_para==NULL)
        return;

    ((CommandParameter_Threshold*)command_para)->thresh = ui.dsp_thresh->value();
    ((CommandParameter_Threshold*)command_para)->max = ui.dsp_max->value();
    ((CommandParameter_Threshold*)command_para)->constant = ui.dsp_constant->value();
    ((CommandParameter_Threshold*)command_para)->block_size = ui.sp_blocksize->value();

    if(ui.rb_binary->isChecked())
        ((CommandParameter_Threshold*)command_para)->threshold_type = THRESH_BINARY;
    if(ui.rb_binary_inv->isChecked())
        ((CommandParameter_Threshold*)command_para)->threshold_type = THRESH_BINARY_INV;
    if(ui.rb_trunc->isChecked())
        ((CommandParameter_Threshold*)command_para)->threshold_type = THRESH_TRUNC;
    if(ui.rb_tozero->isChecked())
        ((CommandParameter_Threshold*)command_para)->threshold_type = THRESH_TOZERO;
    if(ui.rb_tozero_inv->isChecked())
        ((CommandParameter_Threshold*)command_para)->threshold_type = THRESH_TOZERO_INV;
    if(ui.rb_mask->isChecked())
        ((CommandParameter_Threshold*)command_para)->threshold_type = THRESH_MASK;
    if(ui.rb_otsu->isChecked())
        ((CommandParameter_Threshold*)command_para)->threshold_type = THRESH_OTSU;
    if(ui.rb_triangle->isChecked())
        ((CommandParameter_Threshold*)command_para)->threshold_type = THRESH_TRIANGLE;
    if(ui.rb_mean_c->isChecked())
        ((CommandParameter_Threshold*)command_para)->adaptive_type = ADAPTIVE_THRESH_MEAN_C;
    if(ui.rb_gaussian_c->isChecked())
        ((CommandParameter_Threshold*)command_para)->adaptive_type = ADAPTIVE_THRESH_GAUSSIAN_C;
}

ThresholdTypes QThresholdPanel::GetThresholdType()
{
    if(ui.rb_binary->isChecked())
        return THRESH_BINARY;
    if(ui.rb_binary_inv->isChecked())
        return THRESH_BINARY_INV;
    if(ui.rb_trunc->isChecked())
        return THRESH_TRUNC;
    if(ui.rb_tozero->isChecked())
        return THRESH_TOZERO;
    if(ui.rb_tozero_inv->isChecked())
        return THRESH_TOZERO_INV;
    if(ui.rb_mask->isChecked())
        return THRESH_MASK;
    if(ui.rb_otsu->isChecked())
        return THRESH_OTSU;
    if(ui.rb_triangle->isChecked())
        return THRESH_TRIANGLE;

    return THRESH_OTSU;
}

AdaptiveThresholdTypes QThresholdPanel::GetAdaptiveType()
{
    if(ui.rb_mean_c->isChecked())
        return ADAPTIVE_THRESH_MEAN_C;
    if(ui.rb_gaussian_c->isChecked())
        return ADAPTIVE_THRESH_GAUSSIAN_C;

    return ADAPTIVE_THRESH_MEAN_C;
}

