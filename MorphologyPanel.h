#ifndef MORPHOLOGYPANEL_H
#define MORPHOLOGYPANEL_H

#include <ControlPanel.h>
#include "ui_morphologypanel.h"

class CommandParameter_Morphology;

class QMorphologyPanel : public QControlPanel
{
public:
    QMorphologyPanel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~QMorphologyPanel();

    virtual void BeginOperation(VCV_IMAGE_OPERATION operation);
    virtual void EndOperation(VCV_IMAGE_OPERATION operation);
protected slots:
    virtual void ValueChange();
    virtual void PushOk();
    virtual void PushCancel();

    virtual void GetAllParameter();

protected:
    Ui_MorphologyPanel ui;

    void DisableAll();
};

#endif // MORPHOLOGYPANEL_H
