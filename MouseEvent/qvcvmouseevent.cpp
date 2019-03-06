#include "qvcvmouseevent.h"

QVCVMouseEvent::QVCVMouseEvent(QWidget *parent) : QWidget(parent)
{
    data = nullptr;
}

QVCVMouseEvent::~QVCVMouseEvent()
{

}

void QVCVMouseEvent::MousePressEvent(QMouseEvent *event,QPaintDevice *device)
{

}

void QVCVMouseEvent::MouseMoveEvent(QMouseEvent *event,QPaintDevice *device)
{

}

void QVCVMouseEvent::MouseReleseEvent(QMouseEvent *event,QPaintDevice *device)
{

}

QVCVMouseEventData* QVCVMouseEvent::GetData()
{
    return nullptr;
}
