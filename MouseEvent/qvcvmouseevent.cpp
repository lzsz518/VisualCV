#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include "qvcvmouseevent.h"

QVCVMouseEvent::QVCVMouseEvent(QWidget *parent) : QWidget(parent)
{
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

void QVCVMouseEvent::paintEvent(QPaintEvent *event)
{

}

QVCVMouseEventData* QVCVMouseEvent::GetData()
{
    return mouse_data;
}

void QVCVMouseEvent::InitializeData(QVCVMouseEventData *d)
{
    mouse_data = d;
}

QVCVMouseEvent_Line::QVCVMouseEvent_Line()
{
    InitializeData(new QVCVMouseEventData_Line);
}

QVCVMouseEvent_Line::~QVCVMouseEvent_Line()
{

}

void QVCVMouseEvent_Line::MousePressEvent(QMouseEvent *event, QPaintDevice *device)
{
    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)this->GetData();
    if(data!=nullptr)
    {
        data->SetPoint1(event->pos());
        data->SetPoint2(event->pos());
        client_device = device;
    }
}

void QVCVMouseEvent_Line::MouseMoveEvent(QMouseEvent *event, QPaintDevice *device)
{
    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)GetData();
    if(data!=nullptr && device!=nullptr)
    {
        data->SetPoint2(event->pos());
        repaint();
    }
}

void QVCVMouseEvent_Line::MouseReleaseEvent(QMouseEvent *event, QPaintDevice *device)
{
    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)this->GetData();
    if(data!=nullptr)
        data->SetPoint2(event->pos());
}

void QVCVMouseEvent_Line::paintEvent(QPaintEvent *event)
{
    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)this->GetData();
    QPainter painter(client_device);
    painter.begin(client_device);
    painter.setPen(QColor(0,255,255));
    painter.drawLine(data->GetPoint1(),data->GetPoint2());
    painter.end();
}
