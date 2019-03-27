#ifndef QVCVMOUSEEVENTCREATOR_H
#define QVCVMOUSEEVENTCREATOR_H

#include "SystemDefine.h"

class QVCVMouseEvent;

class QVCVMouseEventCreator
{
public:
    QVCVMouseEventCreator();
    ~QVCVMouseEventCreator();
    static QVCVMouseEvent* CreateMouseEvent(const DrawStatus status);
};

#endif // QVCVMOUSEEVENTCREATOR_H
