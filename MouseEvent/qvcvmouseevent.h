#ifndef QVCVMOUSEEVENT_H
#define QVCVMOUSEEVENT_H

#include <QWidget>

class QVCVMouseEventData
{
public:
    QVCVMouseEventData(){}
    ~QVCVMouseEventData(){}
};

class QVCVMouseEventData_Line : public QVCVMouseEventData
{
public:
    QVCVMouseEventData_Line(){
        Point1 = QPoint(0,0);
        Point2 = QPoint(0,0);
    }
    ~QVCVMouseEventData_Line();
    void SetPoint1(const QPoint &p){
        Point1 = p;
    }
    void SetPoint2(const QPoint &p){
        Point2 = p;
    }
    QPoint GetPoint1(){
        return Point1;
    }
    QPoint GetPoint2(){
        return Point2;
    }
    void GetPoints(QPoint &p1,QPoint &p2){
        p1 = Point1;
        p2 = Point2;
    }
private:
    QPoint Point1;
    QPoint Point2;
};

class QVCVMouseEventData_Rectangle : public QVCVMouseEventData
{
public:
    QVCVMouseEventData_Rectangle(){
        rect = QRect(0,0,0,0);
    }
    ~QVCVMouseEventData_Rectangle();
    QRect GetRect(){
        return rect;
    }
    void SetRect(const QRect &r){
        rect = r;
    }
private:
        QRect rect;
};

class QVCVMouseEvent : public QWidget
{
    Q_OBJECT
public:
    explicit QVCVMouseEvent(QWidget *parent = nullptr);
    virtual ~QVCVMouseEvent();
signals:

public slots:

public:
    virtual void MousePressEvent(QMouseEvent *event)=0;
    virtual void MouseMoveEvent(QMouseEvent *event);
    virtual void MouseReleaseEvent(QMouseEvent *event);
    virtual void draw(QPainter *painter);

    virtual QVCVMouseEventData* GetData();
    void InitializeData(QVCVMouseEventData *d);
private:
    QVCVMouseEventData *mouse_data;
};

class QVCVMouseEvent_Line : public QVCVMouseEvent
{
    Q_OBJECT
public:
    QVCVMouseEvent_Line();
    virtual ~QVCVMouseEvent_Line();
public:
    virtual void MousePressEvent(QMouseEvent *event);
    virtual void MouseMoveEvent(QMouseEvent *event);
    virtual void MouseReleaseEvent(QMouseEvent *event);
    virtual void draw(QPainter *painter);
};

class QVCVMouseEvent_Rectangle : public QVCVMouseEvent
{
    Q_OBJECT
public:
    QVCVMouseEvent_Rectangle();
    virtual ~QVCVMouseEvent_Rectangle();
public:
    virtual void MousePressEvent(QMouseEvent *event);
    virtual void MouseMoveEvent(QMouseEvent *event);
    virtual void MouseReleaseEvent(QMouseEvent * event);
    virtual void darw(QPainter *painter);
private:
    void CorrectThePosition(QPoint &p1, QPoint &p2);
};

#endif // QVCVMOUSEEVENT_H
