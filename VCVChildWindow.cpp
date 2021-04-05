#include <QMouseEvent>
#include <QImage>
#include <QPainter>
#include <QScrollBar>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "ControlPanel.h"
#include "FilterPanel.h"
#include "CustomFilterDlg.h"
#include "ThresholdPanel.h"
#include "MorphologyPanel.h"
#include "edgedetectionpanel.h"
#include "VCVData.h"
#include "VCVDataModel.h"
#include "DataModelInstance.h"
#include "Command/CommandBuilder.h"
#include "Command/VCVUndoCommand.h"
#include "Command/ImageProcCommand.h"
#include "Command/Threshold.h"
#include "MouseEvent/qvcvmouseevent.h"
#include "MouseEvent/qvcvmouseeventcreator.h"
#include "opencv2/highgui/highgui.hpp"
#include "VCVChildWindow.h"

QVCVChildWindow::QVCVChildWindow(QWidget *parent, Qt::WindowFlags f)
        :QWidget(parent,f)
{
    left_button_down = false;
    update_image = nullptr;
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(300,300);

    v_scrollbar = new QScrollBar(Qt::Vertical,this);
    h_scrollbar = new QScrollBar(Qt::Horizontal,this);

    display_scale = 1.0;

    filter_panel = nullptr;
    customfilter_panel = nullptr;
    filter_command = nullptr;
    custom_filter_command = nullptr;
    threshold_panel = nullptr;
    threshold_command = nullptr;
    morphology_panel = nullptr;
    morphology_command = nullptr;
    edgedetection_panel = nullptr;
    edgedetection_command = nullptr;

    mouse_event = nullptr;
    mainwindow_status = nullptr;

    connect(v_scrollbar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
    connect(h_scrollbar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
}

QVCVChildWindow::~QVCVChildWindow()
{
    if(update_image!=nullptr)
    {
        delete update_image;
        update_image = nullptr;
    }
    if(filter_panel!=nullptr)
    {
        delete filter_panel;
        filter_panel = nullptr;
    }
    if(customfilter_panel!=nullptr)
    {
        delete customfilter_panel;
        customfilter_panel = nullptr;
    }
    if(threshold_panel != nullptr)
    {
        delete threshold_panel;
        threshold_panel = nullptr;
    }
    if(morphology_panel!=nullptr)
    {
        delete morphology_panel;
        morphology_panel = nullptr;
    }
}

////////////////////////
//public function

bool QVCVChildWindow::Update(const Mat &image, const vector<Rect> &sprite_region)
{
    if(image.empty())
        return false;

    if(update_image!=nullptr)
        delete update_image;

    update_image = MatToQImage(image);

    AdjustScrollBarRange();

    repaint();

    return true;
}

void QVCVChildWindow::SetMainWindowStatus(const MainWindowStatus *status)
{
    mainwindow_status = status;
}

void QVCVChildWindow::SetDisplayScale(float scale)
{
    display_scale = scale;

    repaint();
}

float QVCVChildWindow::GetDisplayScale()
{
    return display_scale;
}

QFilterPanel* QVCVChildWindow::GetFilterPanel(VCV_IMAGE_OPERATION operation)
{
    if(filter_panel==nullptr)
    {
        filter_panel = new QFilterPanel(this,Qt::WindowStaysOnTopHint);

        if(filter_panel==nullptr)
            return nullptr;
        connect(filter_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(FilterParameterChangeRespond(const CommandParameter*)));
        connect(filter_panel,SIGNAL(PanelOk(const CommandParameter*)),this,SLOT(FilterPanelOk(const CommandParameter*)));
        connect(filter_panel,SIGNAL(PanelCancel(const CommandParameter*)),this,SLOT(FilterPanelCancel(const CommandParameter*)));
    }

    DoOperation((QControlPanel*)filter_panel,(QVCVUndoCommand**)&filter_command,operation);
    if(filter_command!=nullptr)
        filter_command->setText(UndoCommandString[operation].c_str());
    return filter_panel;
}

QCustomFilterDlg* QVCVChildWindow::GetCustomFilterPanel()
{
    if(customfilter_panel==nullptr)
    {
        customfilter_panel = new QCustomFilterDlg(this,Qt::WindowStaysOnTopHint);
        if(customfilter_panel==nullptr)
            return nullptr;
        connect(customfilter_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(CustomFilterParameterChange(const CommandParameter*)));
        connect(customfilter_panel,SIGNAL(PanelOk(const CommandParameter*)),this,SLOT(CustomFilterPanelOk(const CommandParameter*)));
        connect(customfilter_panel,SIGNAL(PanelCancel(const CommandParameter*)),this,SLOT(CustomFilterPanelCancel(const CommandParameter*)));
    }

    DoOperation((QControlPanel*)customfilter_panel,(QVCVUndoCommand**)&custom_filter_command,IMAGE_FILTER_CUSTOM2D);
    if(custom_filter_command!=nullptr)
        custom_filter_command->setText(UndoCommandString[IMAGE_FILTER_CUSTOM2D].c_str());

    return customfilter_panel;
}

QThresholdPanel* QVCVChildWindow::GetThresholdPanel(VCV_IMAGE_OPERATION operation)
{
    if(threshold_panel==nullptr)
    {
        threshold_panel = new QThresholdPanel(this,Qt::WindowStaysOnTopHint);
        if(threshold_panel==nullptr)
            return nullptr;

        connect(threshold_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(ThresholdParameterChange(const CommandParameter*)));
        connect(threshold_panel,SIGNAL(PanelOk(const CommandParameter*)),this,SLOT(ThresholdPanelOk(const CommandParameter*)));
        connect(threshold_panel,SIGNAL(PanelCancel(const CommandParameter*)),this,SLOT(ThresholdPanelCancel(const CommandParameter*)));
    }

    DoOperation(threshold_panel,(QVCVUndoCommand**)&threshold_command,operation);
    if(threshold_command!=nullptr)
        threshold_command->setText(UndoCommandString[operation].c_str());

    return threshold_panel;
}

QMorphologyPanel* QVCVChildWindow::GetMorphologyPanel(VCV_IMAGE_OPERATION operation)
{
    if(morphology_panel==nullptr)
    {
        morphology_panel = new QMorphologyPanel(this,Qt::WindowStaysOnTopHint);
        if(morphology_panel==nullptr)
            return nullptr;

        connect(morphology_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(MorphologyParameterChange(const CommandParameter*)));
        connect(morphology_panel,SIGNAL(PanelOk(const CommandParameter*)),this,SLOT(MorphologyPanelOk(const CommandParameter*)));
        connect(morphology_panel,SIGNAL(PanelCancel(const CommandParameter*)),this,SLOT(MorphologyPanelCancel(const CommandParameter*)));
    }

    DoOperation(morphology_panel,(QVCVUndoCommand**)&morphology_command,operation);
    if(morphology_command!=nullptr)
        morphology_command->setText(UndoCommandString[operation].c_str());

    return morphology_panel;
}

QEdgeDetectionPanel* QVCVChildWindow::GetEdgeDetectionPanel(VCV_IMAGE_OPERATION operation)
{
    if(edgedetection_panel==nullptr)
    {
        edgedetection_panel = new QEdgeDetectionPanel(this,Qt::WindowStaysOnTopHint);
        if(edgedetection_panel==nullptr)
            return nullptr;

        connect(edgedetection_panel,SIGNAL(ParameterChange(const CommandParameter*)),this,SLOT(EdgeDetectionParameterChange(const CommandParameter*)));
        connect(edgedetection_panel,SIGNAL(PanelOk(const CommandParameter*)),this,SLOT(EdgeDetectionPanelOk(const CommandParameter*)));
        connect(edgedetection_panel,SIGNAL(PanelCancel(const CommandParameter*)),this,SLOT(EdgeDetectionPanelCancel(const CommandParameter*)));
    }

    DoOperation(edgedetection_panel,(QVCVUndoCommand**)&edgedetection_command,operation);
    if(edgedetection_command!=nullptr)
        edgedetection_command->setText(UndoCommandString[operation].c_str());

    return edgedetection_panel;
}

bool QVCVChildWindow::IsAnyPanelShowed()
{
    if(filter_panel!=nullptr)
    {
        if(filter_panel->isVisible())
            return true;
    }

    if(customfilter_panel!=nullptr)
    {
        if(customfilter_panel->isVisible())
            return true;
    }
    if(threshold_panel!=nullptr)
    {
        if(threshold_panel->isVisible())
            return true;
    }
    if(morphology_panel!=nullptr)
    {
        if(morphology_panel->isVisible())
            return true;
    }
    if(edgedetection_panel!=nullptr)
    {
        if(edgedetection_panel->isVisible())
            return true;
    }

    return false;
}

/////////////////////
//slot function
void QVCVChildWindow::FilterParameterChangeRespond(const CommandParameter *para)
{
    if(filter_command==nullptr)
        return;

    if(!filter_command->IsInit())
    {
        if(!filter_command->Initialize(operator_data,para))
            return;
    }
    else
    {
        if(!filter_command->SetParameter(para))
            return;
    }

    filter_command->redo();
}

void QVCVChildWindow::FilterPanelOk(const CommandParameter *para)
{
    if(filter_command==nullptr)
        return;

    if(!filter_command->IsInit())
    {
        if(!filter_command->Initialize(operator_data,para))
            return;
    }
    else
    {
        if(!filter_command->SetParameter(para))
            return;
    }

    if(operator_data!=nullptr)
        operator_data->AddStep(filter_command);
    filter_command = nullptr;
}

void QVCVChildWindow::FilterPanelCancel(const CommandParameter *para)
{
    if(filter_command==nullptr)
        return;

    if(!filter_command->IsInit())
        if(!filter_command->Initialize(operator_data,para))
            return;
    filter_command->undo();
    delete filter_command;
    filter_command = nullptr;
}

void QVCVChildWindow::CustomFilterParameterChange(const CommandParameter *para)
{
    PanelValueChange(custom_filter_command,para);
}

void QVCVChildWindow::CustomFilterPanelOk(const CommandParameter *para)
{
    PanelOk((QVCVUndoCommand**)&custom_filter_command,para);
}

void QVCVChildWindow::CustomFilterPanelCancel(const CommandParameter *para)
{
    PanelCencel((QVCVUndoCommand**)&custom_filter_command,para);
}

void QVCVChildWindow::ThresholdParameterChange(const CommandParameter *para)
{
    PanelValueChange((QVCVUndoCommand*)threshold_command,para);
}

void QVCVChildWindow::ThresholdPanelOk(const CommandParameter *para)
{
    PanelOk((QVCVUndoCommand**)&threshold_command,para);
}

void QVCVChildWindow::ThresholdPanelCancel(const CommandParameter *para)
{
    PanelCencel((QVCVUndoCommand**)&threshold_command,para);
}

void QVCVChildWindow::MorphologyParameterChange(const CommandParameter *para)
{
    PanelValueChange((QVCVUndoCommand*)morphology_command,para);
}

void QVCVChildWindow::MorphologyPanelOk(const CommandParameter *para)
{
    PanelOk((QVCVUndoCommand**)&morphology_command,para);
}

void QVCVChildWindow::MorphologyPanelCancel(const CommandParameter *para)
{
    PanelCencel((QVCVUndoCommand**)&morphology_command,para);
}

void QVCVChildWindow::EdgeDetectionParameterChange(const CommandParameter *para)
{
    PanelValueChange((QVCVUndoCommand*)edgedetection_command,para);
}

void QVCVChildWindow::EdgeDetectionPanelOk(const CommandParameter *para)
{
    PanelOk((QVCVUndoCommand**)&edgedetection_command,para);
}

void QVCVChildWindow::EdgeDetectionPanelCancel(const CommandParameter *para)
{
    PanelCencel((QVCVUndoCommand**)&edgedetection_command,para);
}

void QVCVChildWindow::VScrollBarRangeChanged(int min, int max)
{
    if (max - min == 0)
        v_scrollbar->hide();
    else
        v_scrollbar->show();
}

void QVCVChildWindow::HScrollBarRangeChanged(int min, int max)
{

    if (max - min == 0)
        h_scrollbar->hide();
    else
        h_scrollbar->show();
}

/////////////////////
//event function

void QVCVChildWindow::paintEvent(QPaintEvent *e)
{
    if(update_image==nullptr)
    {
        QWidget::paintEvent(e);
        return;
    }

    DrawClient();

    QWidget::paintEvent(e);
}

void QVCVChildWindow::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    QDataModelInstance::Instance()->DeleteData(windowTitle());
}

void QVCVChildWindow::resizeEvent(QResizeEvent *e)
{
    AdjustScrollBar();

    QWidget::resizeEvent(e);
}


void QVCVChildWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton && mainwindow_status->CurrentDrawToolKit==DRAWPOLYGON)
    {
        if(mouse_event==nullptr)
            mouse_event = QVCVMouseEventCreator::CreateMouseEvent(mainwindow_status->CurrentDrawToolKit);
        if(mouse_event!=nullptr)
        {
            mouse_event->MousePressEvent(event);
            setMouseTracking(true);
        }
        return;
    }

    if(event->button()==Qt::LeftButton)
    {
        left_button_down = true;
        mouse_event = QVCVMouseEventCreator::CreateMouseEvent(mainwindow_status->CurrentDrawToolKit);
        if(mouse_event!=nullptr)
        {
            QPainter painter(this);
            mouse_event->MousePressEvent(event);
        }
        return;
    }
}

void QVCVChildWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_event==nullptr)
        return;
    if(mainwindow_status->CurrentDrawToolKit==DRAWPOLYGON)
    {
        if(mouse_event!=nullptr)
        {
            mouse_event->MouseMoveEvent(event);
            update();
            return;
        }
    }
    if(left_button_down)
    {
        mouse_event->MouseMoveEvent(event);
        update();
        return;
    }
}

void QVCVChildWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(mouse_event==nullptr)
        return;
    if(event->button()==Qt::LeftButton && mainwindow_status->CurrentDrawToolKit==DRAWPOLYGON)
    {
        mouse_event->MouseReleaseEvent(event);
    }
    if(mainwindow_status->CurrentDrawToolKit==DRAWPOLYGON && event->button()==Qt::RightButton)
    {
        mouse_event->MouseReleaseEvent(event);
        delete mouse_event;
        mouse_event = nullptr;
        setMouseTracking(false);
        return;
    }
    if(left_button_down && mainwindow_status->CurrentDrawToolKit!=DRAWPOLYGON)
    {
        mouse_event->MouseReleaseEvent(event);
        delete mouse_event;
        mouse_event = nullptr;
        left_button_down = false;
    }
}

////////////////////////
//private function

void QVCVChildWindow::AdjustScrollBar()
{
    int width = SCROLLBAR_SIZE; //scrollbar width
    int x = size().width() - width;
    x = x < 0 ? 0: x;
    int height = size().height()-SCROLLBAR_SIZE;
    v_scrollbar->setGeometry(x, 0, width, height);

    height = SCROLLBAR_SIZE;
    int y = size().height() - height;
    y = y < 0 ? 0: y;
    width  = size().width()-SCROLLBAR_SIZE;
    h_scrollbar->setGeometry(0, y, width, height);

    if(update_image!=nullptr)
        AdjustScrollBarRange();
}

void QVCVChildWindow::AdjustScrollBarRange()
{
    if(update_image==nullptr)
        return;

    if(update_image->height()*display_scale-height()+SCROLLBAR_SIZE>0)
    {
        int pos = v_scrollbar->value();
        v_scrollbar->setRange(0,update_image->height()*display_scale-height()+SCROLLBAR_SIZE);
        v_scrollbar->setValue(pos);
    }
    else
    {
        v_scrollbar->setRange(0,0);
    }

    if(update_image->width()*display_scale-width()+SCROLLBAR_SIZE>0)
    {
        int pos = h_scrollbar->value();
        h_scrollbar->setRange(0,update_image->width()*display_scale-width()+SCROLLBAR_SIZE);
        h_scrollbar->setValue(pos);
    }
    else
    {
        h_scrollbar->setRange(0,0);
    }
}

void QVCVChildWindow::DrawClient()
{
    if(update_image==nullptr)
        return;

    QPainter painter(this);

    if(update_image->height()*display_scale-height()+SCROLLBAR_SIZE>0)
    {
        widget_display_area.setTop(0);
        widget_display_area.setHeight(height()-SCROLLBAR_SIZE);
        image_display_area.setTop(v_scrollbar->value()/display_scale);
        image_display_area.setHeight((height()-SCROLLBAR_SIZE)/display_scale);
    }
    else
    {
        widget_display_area.setTop((height()-(update_image->height()*display_scale)-SCROLLBAR_SIZE)/2);
        widget_display_area.setHeight(update_image->height()*display_scale);
        image_display_area.setTop(0);
        image_display_area.setHeight(update_image->height());
    }

    if(update_image->width()*display_scale-width()+SCROLLBAR_SIZE>0)
    {
        widget_display_area.setLeft(0);
        widget_display_area.setWidth(width()-SCROLLBAR_SIZE);
        image_display_area.setLeft(h_scrollbar->value()/display_scale);
        image_display_area.setWidth((width()-SCROLLBAR_SIZE)/display_scale);
    }
    else
    {
        widget_display_area.setLeft((width()-(update_image->width()*display_scale)-SCROLLBAR_SIZE)/2);
        widget_display_area.setWidth(update_image->width()*display_scale);
        image_display_area.setLeft(0);
        image_display_area.setWidth(update_image->width());
    }

    painter.drawImage(widget_display_area,*update_image,image_display_area);

    if((mouse_event!=nullptr && left_button_down)
            ||(mouse_event!=nullptr&&mainwindow_status->CurrentDrawToolKit==DRAWPOLYGON))
    {
        mouse_event->draw(&painter);
    }
}

void QVCVChildWindow::DoOperation(QControlPanel *panel,QVCVUndoCommand **command, VCV_IMAGE_OPERATION operation)
{
    if(command == nullptr)
        return;

    if(QDataModelInstance::Instance()->Count()<=0)
        return;

    operator_data = QDataModelInstance::Instance()->GetData(windowTitle().toStdString().c_str());

    if(operator_data==nullptr)
        return;

    panel->BeginOperation(operation);

    *command = command_builder->CreateCommand(operation);
}

void QVCVChildWindow::PanelValueChange(QVCVUndoCommand *command, const CommandParameter *para)
{
    if(command==nullptr)
        return;

    if(!command->IsInit())
    {
        if(!command->Initialize(operator_data,para))
            return;
    }
    else
    {
        if(!command->SetParameter(para))
            return;
    }

    command->redo();
}

void QVCVChildWindow::PanelOk(QVCVUndoCommand **command, const CommandParameter *para)
{
    if(*command==nullptr)
        return;

    if(!(*command)->IsInit())
    {
        if(!(*command)->Initialize(operator_data,para))
            return;
    }
    else
    {
        if(!(*command)->SetParameter(para))
            return;
    }

    if(operator_data!=nullptr)
        operator_data->AddStep(*command);
    *command = nullptr;
}

void QVCVChildWindow::PanelCencel(QVCVUndoCommand **command, const CommandParameter *para)
{
     if(*command==nullptr)
        return;

    if(!(*command)->IsInit())
        if(!(*command)->Initialize(operator_data,para))
            return;
    (*command)->undo();
    delete *command;
    *command = nullptr;
}


void QVCVChildWindow::SpriteSeparate()
{
    if(operator_data==nullptr)
        return;

    Mat src_mat = QDataModelInstance::Instance()->GetData(windowTitle().toStdString().c_str())->GetOriginalImage().clone();
    if(src_mat.empty())
        return;

    vector<pair<int,int>> rowpairs;
    vector<pair<int,int>> columnpairs;
    FindRowPair(src_mat,rowpairs);
    FindColumnPair(src_mat(Rect(0,rowpairs[8].first,src_mat.cols,rowpairs[8].second-rowpairs[8].first)),columnpairs);

    for(size_t i=0;i<rowpairs.size();++i)
    {
        cv::line(src_mat,Point(0,rowpairs[i].first), Point(src_mat.cols,rowpairs[i].first),Scalar(128,0,128,255));
        cv::line(src_mat,Point(0,rowpairs[i].second), Point(src_mat.cols,rowpairs[i].second),Scalar(128,0,128,255));
    }

    RNG rng(256);
    for(size_t i=0;i<columnpairs.size();++i)
    {
        Scalar color = Scalar(rand()%257,rand()%257,rand()%257,255);
        cv::line(src_mat,Point(columnpairs[i].first,rowpairs[8].first), Point(columnpairs[i].first,rowpairs[8].second),color);
        cv::line(src_mat,Point(columnpairs[i].second,rowpairs[8].first), Point(columnpairs[i].second,rowpairs[8].second),color);
    }

    QFile file(QApplication::applicationDirPath()+"/Champion.json");
    if(!file.open(QIODevice::WriteOnly))
    {
         qDebug() << "File open failed!";
    }
    else
    {
        qDebug() <<"File open successfully!";
    }

    QJsonDocument jdoc;
    QJsonObject obj;
    QJsonArray arr;
    obj["Name"] = "Champion";
    for(size_t i=0;i<columnpairs.size();++i)
    {
        QJsonObject member;
        member["Left"] = QString("%1").arg(columnpairs[i].first);
        member["Top"] = QString("%1").arg(rowpairs[8].first);
        member["Width"] = QString("%1").arg(columnpairs[i].second-columnpairs[i].first);
        member["Height"] = QString("%1").arg(rowpairs[8].second-rowpairs[8].first);
        arr.append(member);
    }

    obj["Walk"] = arr;

    jdoc.setObject(obj);
    file.write(jdoc.toJson(QJsonDocument::Indented));
    file.close();


/*
//    if(src_mat.type()!=CV_8UC4)
//        return;

////    Mat gray_mat;
////    cvtColor(src_mat,gray_mat,CV_RGBA2GRAY);
////    imshow("gray_mat",gray_mat);
////    waitKey(0);
//    Mat mat_purebk;
//    mat_purebk.create(src_mat.rows,src_mat.cols,CV_8UC3);

//    unsigned char *src_ptr, *dst_ptr;
//    for(int i=0;i<src_mat.rows;++i)
//    {
//        src_ptr = src_mat.ptr<unsigned char>(i);
//        dst_ptr = mat_purebk.ptr<unsigned char>(i);
//        for(int j=0;j<src_mat.cols;++j)
//        {
//            if(src_ptr[j*4+3] != 0)
//            {
//                dst_ptr[j*3] = src_ptr[j*4];
//                dst_ptr[j*3+1] = src_ptr[j*4+1];
//                dst_ptr[j*3+2] = src_ptr[j*4+2];
//            }
//        }
//    }
*/

/*K-means
//    Mat data;
//    src_mat.convertTo(data,CV_32F);
//    data = data.reshape(1,data.total());
//    Mat labels, centers;
//    kmeans(data,3, labels, TermCriteria(TermCriteria::EPS+TermCriteria::COUNT,10,1.0),3,KMEANS_PP_CENTERS,centers);
//    // reshape both to a single row of Vec3f pixels:
//    centers = centers.reshape(3,centers.rows);
//    data = data.reshape(3,data.rows);

//    // replace pixel values with their center value:
//    FILE *f = fopen("labels.csv","w");
//    Vec3f *p = data.ptr<Vec3f>();
//    char str[8];
//    for (size_t i=0; i<data.rows; i++) {
//       int center_id = labels.at<int>(i);
//       memset(str,0,sizeof(char)*8);
//       itoa(center_id,str,10);
//       fprintf(f,"%s,",str);
//       if(i%src_mat.cols==0&&i!=0)
//           fprintf(f,"\n");
//       p[i] = centers.at<Vec3f>(center_id);
//    }
//    fclose(f);
*/

////     back to 2d, and uchar:
//    src_mat = data.reshape(3, src_mat.rows);
//    src_mat.convertTo(src_mat, CV_8U);
//    src_mat.convertTo(src_mat, CV_8UC1);

/*    Canny
//    blur(gray_mat, detected_edges, Size(3,3));
//    Canny(detected_edges,detected_edges,0,0,3);
//    imshow("canny",detected_edges);
//        vector<vector<Point> > contours;
//        vector<Vec4i> hierarchy;
//        findContours(detected_edges,contours,hierarchy,RETR_TREE,CHAIN_APPROX_TC89_L1);

//        RNG rng(12345);
//        Mat drawing = Mat::zeros( detected_edges.size(), CV_8UC3 );
//        for( size_t i = 0; i< contours.size(); i++ )
//        {
//            Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
//            drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
//        }
*/

/* 	Threshold
//    Mat gray_mat;
//    cvtColor(mat_purebk,gray_mat,CV_RGBA2GRAY);
//    Mat bw_mat;
//    threshold(gray_mat,bw_mat,0,255,THRESH_OTSU);
//    vector<vector<Point> > contours;
//    vector<Vec4i> hierarchy;
//    findContours(bw_mat,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

//    RNG rng(12345);
//    Mat drawing = Mat::zeros( bw_mat.size(), CV_8UC3 );
//    for( size_t i = 0; i< contours.size(); i++ )
//    {
//        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
//        drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
//    }

//    cvtColor(drawing,bw_mat,CV_RGB2GRAY);
//    contours.clear();
//    hierarchy.clear();
//    findContours(bw_mat,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

//    drawing = Mat::zeros( bw_mat.size(), CV_8UC3 );
//    for( size_t i = 0; i< contours.size(); i++ )
//    {
//        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
//        drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
//    }
//    cvtColor(drawing,bw_mat,CV_RGB2GRAY);
//    contours.clear();
//    hierarchy.clear();
//    findContours(bw_mat,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

//    drawing = Mat::zeros( bw_mat.size(), CV_8UC3 );
//    for( size_t i = 0; i< contours.size(); i++ )
//    {
//        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
//        drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
//    }
*/

    imshow("contours",src_mat);
    imwrite("rowcol.png",src_mat);
    waitKey(0);
}

void QVCVChildWindow::FindRowPair(const Mat &mat, vector<pair<int,int>> &rowpairs)
{
    rowpairs.clear();
    if(mat.empty()||mat.type()!=CV_8UC4)
        return;

    const unsigned char *ptr;
    int i,j;
    bool firstrow = false;
    bool secondrow = false;
    pair<int,int> rowpair;

    for(i=0;i<mat.rows;++i)
    {
        ptr = mat.ptr<unsigned char>(i);
        unsigned int sum = 0;
        for(j=0;j<mat.cols;++j)
        {
            sum += ptr[j*4+3];
        }
        if(sum!=0 && firstrow==false && secondrow == false)
        {
            firstrow = true;
            rowpair.first = i;
        }
        if(sum==0 && firstrow==true && secondrow==false)
        {
            firstrow = false;
            rowpair.second = i;
            rowpairs.push_back(rowpair);
        }
    }
}

void QVCVChildWindow::FindColumnPair(const Mat &mat, vector<pair<int,int>> &columnpairs)
{
    columnpairs.clear();
    if(mat.empty()||mat.type()!=CV_8UC4)
        return;

    const unsigned char *ptr;
    int i,j;
    bool firstcolumn = false;
    bool secondcolumn = false;
    pair<int,int> columnpair;

    for(j=0;j<mat.cols;++j)
    {
        int sum = 0;
        for(i=0;i<mat.rows;++i)
        {
            ptr = mat.ptr<unsigned char>(i);
            sum += ptr[j*4+3];
        }
        if(sum!=0 && firstcolumn==false && secondcolumn==false)
        {
            firstcolumn = true;
            columnpair.first = j;
        }
        if(sum==0 && firstcolumn==true && secondcolumn==false)
        {
            firstcolumn = false;
            columnpair.second =j;
            if(columnpair.second-columnpair.first>10)
                columnpairs.push_back(columnpair);
        }
    }
}
















