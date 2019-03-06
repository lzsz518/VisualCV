#ifndef QVCVMOUSEEVENT_H
#define QVCVMOUSEEVENT_H

#include <QWidget>

class QVCVMouseEventData
{

};

class QVCVMouseEvent : public QWidget
{
    Q_OBJECT
public:
    explicit QVCVMouseEvent(QWidget *parent = nullptr);
    ~QVCVMouseEvent();
signals:

public slots:

public:
    virtual void MousePressEvent(QMouseEvent *event,QPaintDevice *device);
    virtual void MouseMoveEvent(QMouseEvent *event,QPaintDevice *device);
    virtual void MouseReleseEvent(QMouseEvent *event,QPaintDevice *device);

    QVCVMouseEventData* GetData();
protected:
    QVCVMouseEventData *data;
};

#endif // QVCVMOUSEEVENT_H
