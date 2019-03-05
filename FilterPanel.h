#ifndef CONTROLPLAN_H
#define CONTROLPLAN_H

#include "ControlPanel.h"

#include "ui_FilterPanel.h"
#include "SystemDefine.h"


using namespace std;

class QRadioButton;

class QFilterPanel : public QControlPanel
{
	Q_OBJECT
public:
    explicit QFilterPanel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~QFilterPanel();

    double GetSigmaColor();
    double GetSigmaSpace();
    double GetScale();
    double GetDelta();
    Size GetSize();
    Point GetPoint();
    int GetIterations();
    int GetKSize();
	int GetBorderType();

	void Disable();

    virtual void BeginOperation(VCV_IMAGE_OPERATION operation);
    virtual void EndOperation(VCV_IMAGE_OPERATION operation);

protected slots:
    virtual void ValueChange();
    virtual void PushOk();
    virtual void PushCancel();

private:
    Ui_FilterPanel ui;

	QRadioButton *rb_default;
	QRadioButton *rb_wrap;
	QRadioButton *rb_replicate;
	QRadioButton *rb_reflect;
	QRadioButton *rb_replicate101;
	QRadioButton *rb_reflect101;
	QRadioButton *rb_constant;
	QRadioButton *rb_transparent;
	QRadioButton *rb_isolated;

    //VCV_IMAGE_OPERATION image_operation;

	void CreateBorderType();
    virtual void GetAllParameter();

	void EnableBlur();
	void DisableBlur();

	void EnableGaussianBlur();
	void DisableGaussianBlur();

	void EnableMedianBlur();
	void DisableMedianBlur();

	void EnableBilateral();
	void DisableBilateral();

	void EnableErosion();
	void DisableErosion();

	void EnableDilation();
	void DisableDilation();
};

#endif // CONTROLPLAN_H
