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

void QVCVMouseEvent::MousePressEvent(QMouseEvent *event)
{

}

void QVCVMouseEvent::MouseMoveEvent(QMouseEvent *event)
{

}

void QVCVMouseEvent::MouseReleseEvent(QMouseEvent *event)
{

}

void QVCVMouseEvent::draw(QPainter *painter)
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

void QVCVMouseEvent_Line::MousePressEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)this->GetData();
    if(data!=nullptr)
    {
        data->SetPoint1(event->pos());
        data->SetPoint2(event->pos());
    }
}

void QVCVMouseEvent_Line::MouseMoveEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)GetData();
    if(data!=nullptr)
    {
        data->SetPoint2(event->pos());
    }
}

void QVCVMouseEvent_Line::MouseReleaseEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)this->GetData();
    if(data!=nullptr)
        data->SetPoint2(event->pos());
}

void QVCVMouseEvent_Line::draw(QPainter *painter)
{
    if(painter==nullptr)
        return;

    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)this->GetData();
    painter->setPen(QColor(0,255,255));
    painter->drawLine(data->GetPoint1(),data->GetPoint2());
}
