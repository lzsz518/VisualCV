#include "qvcvmouseevent.h"
#include "qvcvmouseeventcreator.h"

QVCVMouseEventCreator::QVCVMouseEventCreator()
{

}

QVCVMouseEventCreator::~QVCVMouseEventCreator()
{

}

QVCVMouseEvent* QVCVMouseEventCreator::CreateMouseEvent(const DrawStatus status)
{
    QVCVMouseEvent *result = nullptr;
    switch(status)
    {
    case DRAWLINE:
        result = new QVCVMouseEvent_Line;
        break;
    case DRAWRECTANGLE:
        result = new QVCVMouseEvent_Rectangle;
        break;
    case DRAWPENCIL:
        result = new QVCVMouseEvent_Pencil;
        break;
    case DRAWPOLYGON:
        result = new QVCVMouseEvent_Polygon;
        break;
    case DRAWELLIPSE:
        result = new QVCVMouseEvent_Ellipse;
        break;
    default:
        result = nullptr;
    }

    return result;
}
