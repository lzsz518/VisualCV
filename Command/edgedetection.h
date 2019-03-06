#ifndef EDGEDETECGTION_H
#define EDGEDETECGTION_H

#include "ImageProcCommand.h"

class QLaplaction : public QImageProcCommand
{
public:
    QLaplaction();
    ~QLaplaction();

    virtual bool SetParameter(const CommandParameter *para);

    virtual void redo();
    virtual void undo();

private:
    int ddepth;
    int ksize;
    BorderTypes bordertype;
    double scale;
    double delta;
};

class QSobel : public QImageProcCommand
{
public:
    QSobel();
    ~QSobel();

    virtual bool SetParameter(const CommandParameter *para);

    virtual void redo();
    virtual void undo();

private:
    int ddepth;
    int ksize;
    int dx;
    int dy;
    BorderTypes bordertype;
    double scale;
    double delta;
};

class QCanny : public QImageProcCommand
{
public:
    QCanny();
    ~QCanny();

    virtual bool SetParameter(const CommandParameter *para);

    virtual void redo();
    virtual void undo();

private:
    bool l2gradient;
    int aperturesize;
    double threshold1;
    double threshold2;
};
#endif // EDGEDETECGTION_H
