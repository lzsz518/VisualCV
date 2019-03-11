#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include "qvcvmouseevent.h"

QVCVMouseEvent::QVCVMouseEvent(QWidget *parent) : QWidget(parent)
{
    mouse_data = nullptr;
}

QVCVMouseEvent::~QVCVMouseEvent()
{
    if(mouse_data!=nullptr)
        delete mouse_data;
}

void QVCVMouseEvent::MousePressEvent(QMouseEvent *event)
{

}

void QVCVMouseEvent::MouseMoveEvent(QMouseEvent *event)
{

}

void QVCVMouseEvent::MouseReleaseEvent(QMouseEvent *event)
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
    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)GetData();
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
    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)GetData();
    if(data!=nullptr)
        data->SetPoint2(event->pos());
}

void QVCVMouseEvent_Line::draw(QPainter *painter)
{
    if(painter==nullptr)
        return;

    QVCVMouseEventData_Line *data = (QVCVMouseEventData_Line*)GetData();
    if(data!=nullptr)
    {
        painter->setPen(QColor(0,255,0));
        painter->drawLine(data->GetPoint1(),data->GetPoint2());
    }
}

QVCVMouseEvent_Rectangle::QVCVMouseEvent_Rectangle()
{
    InitializeData(new QVCVMouseEventData_Rectangle);
}

QVCVMouseEvent_Rectangle::~QVCVMouseEvent_Rectangle()
{

}

void QVCVMouseEvent_Rectangle::MousePressEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Rectangle *data = (QVCVMouseEventData_Rectangle*)GetData();
    if(data!=nullptr)
    {
        data->SetRect(QRect(event->pos(),event->pos()));
    }
}

void QVCVMouseEvent_Rectangle::MouseMoveEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Rectangle *data = (QVCVMouseEventData_Rectangle*)GetData();
    if(data!=nullptr)
    {
        QPoint topleft = data->GetRect().topLeft();
        QPoint bottomright = event->pos();
        CorrectThePosition(topleft,bottomright);
        data->SetRect(QRect(topleft,bottomright));
    }
}

void QVCVMouseEvent_Rectangle::MouseReleaseEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Rectangle *data = (QVCVMouseEventData_Rectangle*)GetData();
    if(data!=nullptr)
    {
        QPoint topleft = data->GetRect().topLeft();
        QPoint bottomright = event->pos();
        CorrectThePosition(topleft,bottomright);
        data->SetRect(QRect(topleft,bottomright));
    }
}

void QVCVMouseEvent_Rectangle::darw(QPainter *painter)
{
    QVCVMouseEventData_Rectangle *data = (QVCVMouseEventData_Rectangle*)GetData();
    if(data!=nullptr)
    {
        painter->setPen(QColor(0,255,0));
        painter->drawRect(data->GetRect());
    }
}

void QVCVMouseEvent_Rectangle::CorrectThePosition(QPoint &p1, QPoint &p2)
{
    if(p1.x()>p2.x())
    {
        int temp = p1.x();
        p1.setX(p2.x());
        p2.setX(temp);
    }
    if(p1.y()<p2.y())
    {
        int temp = p1.y();
        p1.setY(p2.y());
        p2.setY(temp);
    }
}






















