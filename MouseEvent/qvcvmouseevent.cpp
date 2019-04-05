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
    QVCVMouseEventData_Shape *data = static_cast<QVCVMouseEventData_Shape*>(GetData());
    QPen *pen = nullptr;
    if(data!=nullptr)
    {
        data->GetPen(pen);
    }
    if(pen==nullptr)
        data->SetPen(painter->pen());
    else
        painter->setPen(*pen);
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
    QVCVMouseEventData_Line *data = static_cast<QVCVMouseEventData_Line*>(GetData());
    if(data!=nullptr)
    {
        data->SetPoint1(event->pos());
        data->SetPoint2(event->pos());
    }
}

void QVCVMouseEvent_Line::MouseMoveEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Line *data = static_cast<QVCVMouseEventData_Line*>(GetData());
    if(data!=nullptr)
    {
        data->SetPoint2(event->pos());
    }
}

void QVCVMouseEvent_Line::MouseReleaseEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Line *data = static_cast<QVCVMouseEventData_Line*>(GetData());
    if(data!=nullptr)
        data->SetPoint2(event->pos());
}

void QVCVMouseEvent_Line::draw(QPainter *painter)
{
    if(painter==nullptr)
        return;

    QVCVMouseEvent::draw(painter);
    QVCVMouseEventData_Line *data = static_cast<QVCVMouseEventData_Line*>(GetData());
    if(data!=nullptr)
    {
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
    QVCVMouseEventData_Rectangle *data = static_cast<QVCVMouseEventData_Rectangle*>(GetData());
    if(data!=nullptr)
    {
        data->SetRect(QRect(event->pos(),event->pos()));
    }
}

void QVCVMouseEvent_Rectangle::MouseMoveEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Rectangle *data = static_cast<QVCVMouseEventData_Rectangle*>(GetData());
    if(data!=nullptr)
    {
        QPoint topleft = data->GetRect().topLeft();
        QPoint bottomright = event->pos();
        data->SetRect(QRect(topleft,bottomright));
    }
}

void QVCVMouseEvent_Rectangle::MouseReleaseEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Rectangle *data = static_cast<QVCVMouseEventData_Rectangle*>(GetData());
    if(data!=nullptr)
    {
        QPoint topleft = data->GetRect().topLeft();
        QPoint bottomright = event->pos();
        data->SetRect(QRect(topleft,bottomright));
    }
}

void QVCVMouseEvent_Rectangle::draw(QPainter *painter)
{
    QVCVMouseEventData_Rectangle *data = static_cast<QVCVMouseEventData_Rectangle*>(GetData());
    QVCVMouseEvent::draw(painter);
    if(data!=nullptr)
    {
        painter->drawRect(data->GetRect());
    }
}

QVCVMouseEvent_Ellipse::QVCVMouseEvent_Ellipse()
{

}

QVCVMouseEvent_Ellipse::~QVCVMouseEvent_Ellipse()
{

}

void QVCVMouseEvent_Ellipse::draw(QPainter *painter)
{
    QVCVMouseEventData_Rectangle *data = static_cast<QVCVMouseEventData_Rectangle*>(GetData());
    QVCVMouseEvent::draw(painter);
    if(data!=nullptr)
    {
        painter->drawEllipse(data->GetRect());
    }
}

QVCVMouseEvent_Pencil::QVCVMouseEvent_Pencil()
{
    InitializeData(new QVCVMouseEventData_Pencil);
}

QVCVMouseEvent_Pencil::~QVCVMouseEvent_Pencil()
{

}

void QVCVMouseEvent_Pencil::MousePressEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Pencil *data = static_cast<QVCVMouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        data->AddPoint(event->pos());
    }
}

void QVCVMouseEvent_Pencil::MouseMoveEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Pencil *data = static_cast<QVCVMouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        data->AddPoint(event->pos());
    }
}

void QVCVMouseEvent_Pencil::MouseReleaseEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Pencil *data = static_cast<QVCVMouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        data->AddPoint(event->pos());
    }
}

void QVCVMouseEvent_Pencil::draw(QPainter *painter)
{
    QVCVMouseEventData_Pencil *data = static_cast<QVCVMouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        const QVector<QPoint>* points = data->GetPoints();
        if(points->isEmpty())
            return;
        QVector<QPoint>::const_iterator itor = points->begin();
        QPoint p = *itor;
        ++itor;
        for(;itor<points->end();++itor)
        {
            painter->drawLine(p,*itor);
            p = *itor;
        }
    }
}

QVCVMouseEvent_Polygon::QVCVMouseEvent_Polygon()
{
   InitializeData(new QVCVMouseEventData_Pencil);
}

QVCVMouseEvent_Polygon::~QVCVMouseEvent_Polygon()
{

}

void QVCVMouseEvent_Polygon::MousePressEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Pencil *data = static_cast<QVCVMouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        data->AddPoint(event->pos());
        data->SetCurrentPos(event->pos());
    }
}

void QVCVMouseEvent_Polygon::MouseMoveEvent(QMouseEvent *event)
{
    QVCVMouseEventData_Pencil *data = static_cast<QVCVMouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        data->SetCurrentPos(event->pos());
    }

}

void QVCVMouseEvent_Polygon::MouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        QVCVMouseEventData_Pencil *data = static_cast<QVCVMouseEventData_Pencil*>(GetData());
        if(data!=nullptr)
        {
            if(!data->GetPoints()->isEmpty())
            {
                data->SetCurrentPos(data->GetPoints()->at(0));
                data->AddPoint(data->GetPoints()->at(0));
            }
        }
    }
}

void QVCVMouseEvent_Polygon::draw(QPainter *painter)
{
    QVCVMouseEvent_Pencil::draw(painter);
    QVCVMouseEventData_Pencil *data = static_cast<QVCVMouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        const QVector<QPoint> *points = data->GetPoints();
        if(points->size()>0)
            painter->drawLine(points->at(points->size()-1),data->GetCurrentPos());
    }
}


