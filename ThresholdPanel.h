#ifndef THRESHOLDPANEL_H
#define THRESHOLDPANEL_H

#include "SystemDefine.h"
#include "ControlPanel.h"
#include "ui_ThresholdPanel.h"

class QThresholdPanel : public QControlPanel
{
    Q_OBJECT
public:
    explicit QThresholdPanel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QThresholdPanel();

    double GetThresh();
    double GetMaxValue();
    int GetBlockSize();
    double GetConstant();

    virtual void BeginOperation(VCV_IMAGE_OPERATION operation);
    virtual void EndOperation(VCV_IMAGE_OPERATION operation);

private slots:
    virtual void ValueChange();
    virtual void PushOk();
    virtual void PushCancel();

protected:
    Ui_Threshold ui;
   // VCV_IMAGE_OPERATION image_operation;

    void EnableThreshold();
    void EnableAdaptive();

    void DisableAll();
    void GetAllParameter();
    ThresholdTypes GetThresholdType();
    AdaptiveThresholdTypes GetAdaptiveType();
};

#endif // THRESHOLDPANEL_H
