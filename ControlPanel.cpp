#include <QRadioButton>
#include <QGridLayout>
#include <QGroupBox>
#include "ControlPanel.h"

QControlPanel::QControlPanel(QWidget *parent) : QWidget(parent)
{
    rb_default = nullptr;
    rb_wrap = nullptr;
    rb_replicate = nullptr;
    rb_reflect = nullptr;
    rb_reflect101 = nullptr;
    rb_constant = nullptr;
    rb_transparent = nullptr;
    rb_isolated = nullptr;
    radiobutton_layout = nullptr;
    is_bordertype_init = false;
    image_operation = IMAGE_NONE;
}

QControlPanel::~QControlPanel()
{
    if(rb_default!=nullptr)
        delete rb_default;
    if(rb_wrap!=nullptr)
        delete rb_wrap;
    if(rb_replicate!=nullptr)
        delete rb_replicate;
    if(rb_reflect!=nullptr)
        delete rb_reflect;
    if(rb_reflect101!=nullptr)
        delete rb_reflect101;
    if(rb_replicate!=nullptr)
        delete rb_replicate;
    if(rb_constant!=nullptr)
        delete rb_constant;
    if(rb_transparent!=nullptr)
        delete rb_transparent;
    if(rb_isolated!=nullptr)
        delete rb_isolated;
    if(radiobutton_layout!=nullptr)
        delete radiobutton_layout;
}

void QControlPanel::BeginOperation(VCV_IMAGE_OPERATION operation)
{
    SetOperation(operation);
}

void QControlPanel::EndOperation(VCV_IMAGE_OPERATION operation)
{

}

void QControlPanel::ValueChange()
{
    GetAllParameter();

    emit ParameterChange(command_para);
}

void QControlPanel::PushOk()
{
    EndOperation(GetOperation());

    GetAllParameter();
    emit PanelOk(command_para);
    close();
}

void QControlPanel::PushCancel()
{
    EndOperation(GetOperation());
    GetAllParameter();
    emit PanelCancel(command_para);
    close();
}

void QControlPanel::GetAllParameter()
{

}

void QControlPanel::CreateBorderGroup(QGroupBox *groupbox)
{
    if(groupbox==nullptr)
        return;

    if(rb_default==nullptr)
    {
        rb_default = new QRadioButton("BORDER_DEFAULT");
        connect(rb_default,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    }
    if(rb_wrap==nullptr)
    {
        rb_wrap = new QRadioButton("BORDER_WRAP");
        connect(rb_wrap,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    }
    if(rb_replicate==nullptr)
    {
        rb_replicate = new QRadioButton("BORDER_REPLICATE");
        connect(rb_replicate,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    }
    if(rb_reflect==nullptr)
    {
        rb_reflect = new QRadioButton("BORDER_REFLECT");
        connect(rb_reflect,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    }
    if(rb_reflect101==nullptr)
    {
        rb_reflect101 = new QRadioButton("BORDER_REFLECT_101");
        connect(rb_reflect101,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    }
    if(rb_replicate==nullptr)
    {
        rb_replicate = new QRadioButton("BORDER_REPLICATE");
        connect(rb_replicate,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    }
    if(rb_constant==nullptr)
    {
        rb_constant = new QRadioButton("BORDER_CONSTANT");
        connect(rb_constant,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    }
    if(rb_transparent==nullptr)
    {
        rb_transparent = new QRadioButton("BORDER_TRANSPARENT");
        connect(rb_transparent,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    }
    if(rb_isolated==nullptr)
    {
        rb_isolated = new QRadioButton("BORDER_ISOLATE");
        connect(rb_isolated,SIGNAL(clicked(bool)),this,SLOT(ValueChange()));
    }
    if(radiobutton_layout==nullptr)
        radiobutton_layout = new QGridLayout;

    radiobutton_layout->addWidget(rb_default,0,0);
    radiobutton_layout->addWidget(rb_wrap,1,0);
    radiobutton_layout->addWidget(rb_replicate,2,0);
    radiobutton_layout->addWidget(rb_reflect,3,0);
    radiobutton_layout->addWidget(rb_reflect101,0,1);
    radiobutton_layout->addWidget(rb_constant,1,1);
    radiobutton_layout->addWidget(rb_transparent,2,1);
    radiobutton_layout->addWidget(rb_isolated,3,1);

    groupbox->setLayout(radiobutton_layout);

    is_bordertype_init = true;
}

BorderTypes QControlPanel::GetBorderType()
{
   if(!is_bordertype_init)
       return BORDER_DEFAULT;

   if(rb_default->isChecked())
       return BORDER_DEFAULT;
   if(rb_wrap->isChecked())
       return BORDER_WRAP;
   if(rb_replicate->isChecked())
       return BORDER_REPLICATE;
   if(rb_reflect->isChecked())
       return BORDER_REFLECT;
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

void QControlPanel::SetOperation(VCV_IMAGE_OPERATION oper)
{
   image_operation = oper;
}

VCV_IMAGE_OPERATION QControlPanel::GetOperation()
{
    return image_operation;
}

void QControlPanel::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    emit PanelShow(this);
}

void QControlPanel::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    emit PanelClose(this);
}
