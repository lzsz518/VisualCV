#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "opencv/cv.h"
#include <QWidget>
#include "SystemDefine.h"

class QRadioButton;
class QGroupBox;
class QGridLayout;
class CommandParameter;

using namespace cv;

class QControlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit QControlPanel(QWidget *parent = nullptr);
    ~QControlPanel();

    virtual void BeginOperation(VCV_IMAGE_OPERATION operation);
    virtual void EndOperation(VCV_IMAGE_OPERATION operation);

protected slots:
    virtual void ValueChange();
    virtual void PushOk();
    virtual void PushCancel();
signals:
    void ParameterChange(const CommandParameter *para);
    void PanelOk(const CommandParameter *para);
    void PanelCancel(const CommandParameter *para);
    void PanelShow(QControlPanel *panel);
    void PanelClose(QControlPanel *panel);
protected:
    CommandParameter *command_para;
    virtual void GetAllParameter();

    void CreateBorderGroup(QGroupBox* groupbox);
    BorderTypes GetBorderType();

    void SetOperation(VCV_IMAGE_OPERATION oper);
    VCV_IMAGE_OPERATION GetOperation();
private:
    bool is_bordertype_init;
    QRadioButton *rb_default;
    QRadioButton *rb_wrap;
    QRadioButton *rb_replicate;
    QRadioButton *rb_reflect;
    QRadioButton *rb_reflect101;
    QRadioButton *rb_constant;
    QRadioButton *rb_transparent;
    QRadioButton *rb_isolated;
    QGridLayout *radiobutton_layout;

    VCV_IMAGE_OPERATION image_operation;

private:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // CONTROLPANEL_H
