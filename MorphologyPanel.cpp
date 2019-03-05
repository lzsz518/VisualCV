#include "SystemDefine.h"
#include "MorphologyPanel.h"

QMorphologyPanel::QMorphologyPanel(QWidget *parent, Qt::WindowFlags f)
{
    ui.setupUi(this);
    setWindowFlags(f);
    command_para = new CommandParameter_Morphology;
    CreateBorderGroup(ui.groupBox);

    connect(ui.pb_ok,SIGNAL(clicked(bool)),this,SLOT(PushOk()));
    connect(ui.pb_cancel,SIGNAL(clicked(bool)),this,SLOT(PushCancel()));
    connect(ui.cb_op,SIGNAL(currentIndexChanged(int)),this,SLOT(ValueChange()));
    connect(ui.cb_morphshape,SIGNAL(currentIndexChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_kernelsize,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_anchorx,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_anchory,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_kernelanchorx,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_kernelanchory,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_iterations,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_tp1,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_tp2,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_tp3,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
    connect(ui.sp_tp4,SIGNAL(valueChanged(int)),this,SLOT(ValueChange()));
}

QMorphologyPanel::~QMorphologyPanel()
{
    if(command_para!=NULL)
        delete command_para;
}

void QMorphologyPanel::BeginOperation(VCV_IMAGE_OPERATION operation)
{
    setEnabled(true);
    QControlPanel::BeginOperation(operation);
}

void QMorphologyPanel::EndOperation(VCV_IMAGE_OPERATION operation)
{
//    DisableAll();

    SetOperation(IMAGE_NONE);
}

void QMorphologyPanel::ValueChange()
{
    GetAllParameter();
    emit ParameterChange(command_para);
}

void QMorphologyPanel::PushOk()
{
    GetAllParameter();
    emit PanelOk(command_para);
    close();
}

void QMorphologyPanel::PushCancel()
{
    GetAllParameter();
    emit PanelCancel(command_para);
    close();
}

void QMorphologyPanel::GetAllParameter()
{
    ((CommandParameter_Morphology*)command_para)->anchor.x = ui.sp_anchorx->value();
    ((CommandParameter_Morphology*)command_para)->anchor.y = ui.sp_anchory->value();
    ((CommandParameter_Morphology*)command_para)->borderType = GetBorderType();
    ((CommandParameter_Morphology*)command_para)->iteration = ui.sp_iterations->value();
    ((CommandParameter_Morphology*)command_para)->kernelAnchor.x = ui.sp_kernelanchorx->value();
    ((CommandParameter_Morphology*)command_para)->kernelAnchor.y = ui.sp_kernelanchory->value();
    ((CommandParameter_Morphology*)command_para)->ksize = ui.sp_kernelsize->value();

    switch(ui.cb_op->currentIndex())
    {
    case 0:
        ((CommandParameter_Morphology*)command_para)->op = MORPH_ERODE;
        break;
    case 1:
        ((CommandParameter_Morphology*)command_para)->op = MORPH_DILATE;
        break;
    case 2:
        ((CommandParameter_Morphology*)command_para)->op = MORPH_OPEN;
        break;
    case 3:
        ((CommandParameter_Morphology*)command_para)->op = MORPH_CLOSE;
        break;
    case 4:
        ((CommandParameter_Morphology*)command_para)->op = MORPH_GRADIENT;
        break;
    case 5:
        ((CommandParameter_Morphology*)command_para)->op = MORPH_TOPHAT;
        break;
    case 6:
        ((CommandParameter_Morphology*)command_para)->op = MORPH_BLACKHAT;
        break;
    case 7:
        ((CommandParameter_Morphology*)command_para)->op = MORPH_HITMISS;
        break;
    default:
        ((CommandParameter_Morphology*)command_para)->op = MORPH_ERODE;
        break;
    }

    switch(ui.cb_morphshape->currentIndex())
    {
    case 0:
        ((CommandParameter_Morphology*)command_para)->shape = MORPH_RECT;
        break;
    case 1:
        ((CommandParameter_Morphology*)command_para)->shape = MORPH_CROSS;
        break;
    case 2:
        ((CommandParameter_Morphology*)command_para)->shape = MORPH_ELLIPSE;
        break;
    default:
        ((CommandParameter_Morphology*)command_para)->shape = MORPH_RECT;
    }

    ((CommandParameter_Morphology*)command_para)->tp1 = ui.sp_tp1->value();
    ((CommandParameter_Morphology*)command_para)->tp2 = ui.sp_tp2->value();
    ((CommandParameter_Morphology*)command_para)->tp3 = ui.sp_tp3->value();
    ((CommandParameter_Morphology*)command_para)->tp4 = ui.sp_tp4->value();
}

void QMorphologyPanel::DisableAll()
{
    setEnabled(false);
}
