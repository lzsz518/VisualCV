#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

#include "ImageProcCommand.h"

class QMorphology : public QImageProcCommand
{
public:
    QMorphology();
    ~QMorphology();

    virtual bool SetParameter(const CommandParameter *para);

    virtual void redo();
private:
    int op;
    int shape;
    int ksize;
    Point kernelAnchor;
    Point anchor;
    int iteration;
    BorderTypes borderType;
    int tp1;
    int tp2;
    int tp3;
    int tp4;
};

#endif // MORPHOLOGY_H
