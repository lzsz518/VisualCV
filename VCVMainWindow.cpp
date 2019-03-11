#include <QMenu>
#include <QAction>
#include <QMdiArea>
#include <QSplitter>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QTabWidget>
#include <QToolBar>
#include <QUndoView>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMdiSubWindow>

#include "VCVData.h"
#include "VCVDataModel.h"
#include "DataModelInstance.h"
#include "VCVChildWindow.h"
#include "FilterPanel.h"
#include "CustomFilterDlg.h"
#include "ThresholdPanel.h"
#include "MorphologyPanel.h"
#include "edgedetectionpanel.h"
#include "NewDialog.h"
#include "AboutDialog.h"

#include "Command/CommandBuilder.h"
#include "Command/VCVUndoCommand.h"
#include "Command/CustomFilter2D.h"

#include "VCVMainWindow.h"

QVCVMainWindow::QVCVMainWindow(QWidget *parent , Qt::WindowFlags f)
        :QMainWindow(parent,f)
{
    dockwidget = nullptr;
    InitUI();

    setMinimumSize(800,600);

    mdi_area = new QMdiArea(this);
    mdi_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdi_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdi_area);

    CreateConnection();
}

QVCVMainWindow::~QVCVMainWindow()
{

}

void QVCVMainWindow::InitUI()
{
    CreateAction();
    CreateMenu();
    CreateUndoView();
    CreateToolbar();

    QLabel *label1 = new QLabel(tr("OK"));
    statusBar()->addWidget(label1);

    setWindowTitle(tr("Visual CV"));
}

void QVCVMainWindow::CreateAction()
{
    file_menu_new = new QAction(tr("&New"),this);
    file_menu_open = new QAction(tr("&Open"),this);
    file_menu_save = new QAction(tr("&Save"),this);
    file_menu_saveas = new QAction(tr("Save as"),this);

    view_tile = new QAction(tr("&Tile"),this);
    view_cascade = new QAction(tr("&Cascade"),this);
    view_zoomin = new QAction(tr("Zoom&In"),this);
    view_zoomout = new QAction(tr("Zoom&Out"),this);
    view_normalsize = new QAction("&Normal Size",this);

    filter_blur = new QAction(tr("Blur"),this);
    filter_gaussian = new QAction(tr("GaussianBlur"),this);
    filter_median = new QAction(tr("MedianBlur"),this);
    filter_bilateral = new QAction(tr("Bilateral"),this);
    filter_custom2d = new QAction(tr("CustomFilter2D"),this);
    morphological_erosion = new QAction(tr("Erosion"),this);
    morphological_dilation = new QAction(tr("Dilation"),this);
    morphological_open = new QAction(tr("Morphology"),this);
    morphological_threshold = new QAction(tr("Threshold"),this);
    morphological_adaptivethreshold = new QAction(tr("AdaptiveThreshold"),this);
    edge_laplaction = new QAction(tr("Laplacian"),this);
    edge_sobel = new QAction(tr("Sobel"),this);
    edge_canny = new QAction(tr("Canny"),this);

    draw_line = new QAction(QIcon(":/Resource/DrawLine.png"),tr("Line"),this);
    draw_rect = new QAction(QIcon(":/Resource/DrawRect.png"),tr("Rect"),this);
    draw_ellipse = new QAction(QIcon(":/Resource/DrawEllipse.png"),tr("Ellipse"),this);
    draw_free = new QAction(QIcon(":/Resource/DrawFree.png"),tr("Pencil"),this);
    draw_polygon = new QAction(QIcon(":/Resource/DrawPolygon.png"),tr("Polygon"),this);
    draw_line->setCheckable(true);
    draw_rect->setCheckable(true);
    draw_ellipse->setCheckable(true);
    draw_free->setCheckable(true);
    draw_polygon->setCheckable(true);

    about = new QAction(tr("About"),this);

    undoviewShow = new QAction(tr("UndoVeiw"),this);
}

void QVCVMainWindow::CreateMenu()
{
    file_menu = menuBar()->addMenu(tr("&File"));
    edit_menu = menuBar()->addMenu(tr("&Edit"));
    view_menu = menuBar()->addMenu(tr("&View"));
    filter_menu =  menuBar()->addMenu(tr("&Filter"));
    morphological_menu = menuBar()->addMenu(tr("Morphological"));
    edge_menu = menuBar()->addMenu(tr("EdgeDetection"));
    window_menu = menuBar()->addMenu(tr("&Window"));
    help_menu = menuBar()->addMenu(tr("&Help"));

    file_menu->addAction(file_menu_new);
    file_menu->addAction(file_menu_open);
    file_menu->addAction(file_menu_save);
    file_menu->addAction(file_menu_saveas);

    view_menu->addAction(view_tile);
    view_menu->addAction(view_cascade);
    view_menu->addSeparator();
    view_menu->addAction(view_zoomin);
    view_menu->addAction(view_zoomout);
    view_menu->addAction(view_normalsize);

    filter_menu->addAction(filter_blur);
    filter_menu->addAction(filter_gaussian);
    filter_menu->addAction(filter_median);
    filter_menu->addAction(filter_bilateral);
    filter_menu->addAction(filter_custom2d);
    morphological_menu->addAction(morphological_erosion);
    morphological_menu->addAction(morphological_dilation);
    morphological_menu->addAction(morphological_open);
    morphological_menu->addAction(morphological_threshold);
    morphological_menu->addAction(morphological_adaptivethreshold);
    edge_menu->addAction(edge_laplaction);
    edge_menu->addAction(edge_sobel);
    edge_menu->addAction(edge_canny);

    help_menu->addAction(about);
    window_menu->addAction(undoviewShow);
}

void QVCVMainWindow::CreateUndoView()
{
    dockwidget = new QDockWidget("UndoView",this);
    undoview = new QUndoView;
    dockwidget->setWidget(undoview);
    dockwidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea,dockwidget);
}

void QVCVMainWindow::WindowActive(QMdiSubWindow *subwin)
{
    if(subwin==nullptr)
    {
        edit_menu->clear();
        return;
    }
    QDataModelInstance::Instance()->Notify();
    QVCVData *current_data = QDataModelInstance::Instance()->GetData(subwin->windowTitle().toStdString().c_str());
    edit_menu->clear();
    edit_menu->addAction(current_data->GetUndoAction());
    edit_menu->addAction(current_data->GetRedoAction());

    QVCVChildWindow *childWindow = (QVCVChildWindow*)subwin->widget();
    if(childWindow!=nullptr)
    {
        if(childWindow->IsAnyPanelShowed())
            SetMenuStatus(false);
        else
            SetMenuStatus(true);

    }
    else
    {
        SetMenuStatus(true);
    }
    current_data->Notify();
}

void QVCVMainWindow::New()
{
    QNewDialog nd;
    nd.exec();
}

void QVCVMainWindow::OpenFile()
{
    QStringList files;
    files = QFileDialog::getOpenFileNames(this,
                                          tr("Open Image"), "/home/bobo/Pictures", tr("Image Files (*.png *.jpg *.bmp)"));

    if(files.size()<0)
        return;

    int i;
    QVCVData *vcvdata = nullptr;
    QVCVChildWindow *viewer = nullptr;
    for(i=0;i<files.size();++i)
    {
        vcvdata = new QVCVData;
        vcvdata->Initialize();
        viewer = new QVCVChildWindow;
        QMdiSubWindow *sub_win = new QMdiSubWindow;
        sub_win->setWidget(viewer);
        vcvdata->RegisterViewer(viewer);
        vcvdata->RegisterViewer(undoview);
        vcvdata->Load(files.at(i));
        viewer->setWindowTitle(files.at(i));
        mdi_area->addSubWindow(sub_win);
        QDataModelInstance::Instance()->AddData(vcvdata);
        QDataModelInstance::Instance()->Notify();

        viewer->show();
    }
}

void QVCVMainWindow::Save()
{

}

void QVCVMainWindow::Saveas()
{
    if(QDataModelInstance::Instance()->Count()<=0)
        return;

    QString file;
    file = QFileDialog::getSaveFileName(this,
                                        tr("Open Image"), "/home", tr("Image Files(*.jpg);;Image Files(*.png);;Image Files(*.bmp)"));

    if(!QDataModelInstance::Instance()->GetData(mdi_area->activeSubWindow()->windowTitle().toStdString().c_str())->SaveAs(file))
        QMessageBox::information(nullptr,tr("Information"),tr("Write file to headdisk fail !"));
}

void QVCVMainWindow::BlurFilter()
{
    ShowFilterPanel(IMAGE_FILTER_BLUR);
}

void QVCVMainWindow::GaussianBlurFilter()
{
    ShowFilterPanel(IMAGE_FILTER_GAUSSIANBLUR);
}

void QVCVMainWindow::MedianBlurFilter()
{
    ShowFilterPanel(IMAGE_FILTER_MEDIANBLUR);
}

void QVCVMainWindow::BilateralFilter()
{
    ShowFilterPanel(IMAGE_FILTER_BILATERAL);
}

void QVCVMainWindow::CustomFilter2D()
{
    if(mdi_area->activeSubWindow()==nullptr)
        return;

    QCustomFilterDlg *customfilterpanel = ((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->GetCustomFilterPanel();

    if(customfilterpanel!=nullptr)
    {
        customfilterpanel->Initialize(QDataModelInstance::Instance()->GetData(mdi_area->activeSubWindow()->windowTitle().toStdString().c_str())->GetDisplayImage());
        customfilterpanel->show();
    }
}

void QVCVMainWindow::Erosion()
{
    ShowFilterPanel(IMAGE_FILTER_EROSION);
}

void QVCVMainWindow::Dilation()
{
    ShowFilterPanel(IMAGE_FILTER_DILATION);
}

void QVCVMainWindow::MorphologyOpen()
{
    ShowMorphologicalPanel(IMAGE_MORPHOLOGY);
}

void QVCVMainWindow::Threshold()
{
    ShowThresholdPanel(IMAGE_THRESHOLD_THRESHOLD);
}

void QVCVMainWindow::AdaptiveThreshold()
{
    ShowThresholdPanel(IMAGE_THRESHOLD_ADAPTIVE);
}

void QVCVMainWindow::Laplacian_action()
{
    ShowEdgeDetectionPanel(IMAGE_EDGE_LAPLACIAN);
}

void QVCVMainWindow::Sobel_action()
{
    ShowEdgeDetectionPanel(IMAGE_EDGE_SOBEL);
}

void QVCVMainWindow::Canny_action()
{
    ShowEdgeDetectionPanel(IMAGE_EDGE_CANNY);
}

void QVCVMainWindow::ZoomIn()
{
    if(mdi_area->activeSubWindow()!=nullptr)
        ((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->SetDisplayScale(
                            ((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->GetDisplayScale()*1.1);
}

void QVCVMainWindow::ZoomOut()
{
    if(mdi_area->activeSubWindow()!=nullptr)
        ((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->SetDisplayScale(
                            ((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->GetDisplayScale()*0.9);
}

void QVCVMainWindow::NormalSize()
{
    if(mdi_area->activeSubWindow()!=nullptr)
        ((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->SetDisplayScale(1.0);
}


void QVCVMainWindow::ShowFilterPanel(VCV_IMAGE_OPERATION operation)
{
    if(mdi_area->activeSubWindow()==nullptr)
        return;

    QFilterPanel *filterpanel = ((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->GetFilterPanel(operation);

    if(filterpanel!=nullptr)
    {
        filterpanel->show();
        SetMenuStatus(false);
        connect(filterpanel,SIGNAL(PanelShow(QControlPanel*)),this,SLOT(slotPanelShow(QControlPanel*)));
        connect(filterpanel,SIGNAL(PanelClose(QControlPanel*)),this,SLOT(slotPanelClose(QControlPanel*)));
    }
}

void QVCVMainWindow::ShowThresholdPanel(VCV_IMAGE_OPERATION operation)
{
    if(mdi_area->activeSubWindow()==nullptr)
        return;

    QThresholdPanel *thresholdpanel = ((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->GetThresholdPanel(operation);
    if(thresholdpanel!=nullptr)
    {
        thresholdpanel->show();
        SetMenuStatus(false);
        connect(thresholdpanel,SIGNAL(PanelShow(QControlPanel*)),this,SLOT(slotPanelShow(QControlPanel*)));
        connect(thresholdpanel,SIGNAL(PanelClose(QControlPanel*)),this,SLOT(slotPanelClose(QControlPanel*)));
    }
}

void QVCVMainWindow::ShowMorphologicalPanel(VCV_IMAGE_OPERATION operation)
{
    if(mdi_area->activeSubWindow()==nullptr)
        return;
    QMorphologyPanel *moph_panel = ((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->GetMorphologyPanel(operation);

    if(moph_panel!=nullptr)
    {
        moph_panel->show();
        SetMenuStatus(false);
        connect(moph_panel,SIGNAL(PanelShow(QControlPanel*)),this,SLOT(slotPanelShow(QControlPanel*)));
        connect(moph_panel,SIGNAL(PanelClose(QControlPanel*)),this,SLOT(slotPanelClose(QControlPanel*)));
    }
}

void QVCVMainWindow::ShowEdgeDetectionPanel(VCV_IMAGE_OPERATION operation)
{
    if(mdi_area->activeSubWindow()==nullptr)
        return;
    QEdgeDetectionPanel *edge_panel = ((QVCVChildWindow*)mdi_area->activeSubWindow()->widget())->GetEdgeDetectionPanel(operation);
    if(edge_panel!=nullptr)
    {
        edge_panel->show();
        SetMenuStatus(false);
        connect(edge_panel,SIGNAL(PanelShow(QControlPanel*)),this,SLOT(slotPanelShow(QControlPanel*)));
        connect(edge_panel,SIGNAL(PanelClose(QControlPanel*)),this,SLOT(slotPanelClose(QControlPanel*)));
    }
}

void QVCVMainWindow::CreateConnection()
{
    connect(file_menu_new,SIGNAL(triggered()),this,SLOT(New()));
    connect(file_menu_open,SIGNAL(triggered()),this,SLOT(OpenFile()));
    connect(file_menu_saveas,SIGNAL(triggered()),this,SLOT(Saveas()));


    connect(mdi_area,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(WindowActive(QMdiSubWindow*)));

    connect(filter_blur,SIGNAL(triggered()),this,SLOT(BlurFilter()));
    connect(filter_gaussian,SIGNAL(triggered()),this,SLOT(GaussianBlurFilter()));
    connect(filter_median,SIGNAL(triggered()),this,SLOT(MedianBlurFilter()));
    connect(filter_bilateral,SIGNAL(triggered()),this,SLOT(BilateralFilter()));
    connect(filter_custom2d,SIGNAL(triggered()),this,SLOT(CustomFilter2D()));
    connect(morphological_erosion,SIGNAL(triggered()),this,SLOT(Erosion()));
    connect(morphological_dilation,SIGNAL(triggered()),this,SLOT(Dilation()));
    connect(morphological_open,SIGNAL(triggered()),this,SLOT(MorphologyOpen()));
    connect(morphological_threshold,SIGNAL(triggered()),this,SLOT(Threshold()));
    connect(morphological_adaptivethreshold,SIGNAL(triggered()),this,SLOT(AdaptiveThreshold()));
    connect(edge_laplaction,SIGNAL(triggered()),this,SLOT(Laplacian_action()));
    connect(edge_sobel,SIGNAL(triggered()),this,SLOT(Sobel_action()));
    connect(edge_canny,SIGNAL(triggered()),this,SLOT(Canny_action()));

    connect(view_zoomin,SIGNAL(triggered()),this,SLOT(ZoomIn()));
    connect(view_zoomout,SIGNAL(triggered()),this,SLOT(ZoomOut()));
    connect(view_normalsize,SIGNAL(triggered()),this,SLOT(NormalSize()));

    connect(view_tile,SIGNAL(triggered()),mdi_area,SLOT(tileSubWindows()));
    connect(view_cascade,SIGNAL(triggered()),mdi_area,SLOT(cascadeSubWindows()));

    connect(draw_line,SIGNAL(toggled(bool)),this,SLOT(slotMutualToolbar(bool)));
    connect(draw_rect,SIGNAL(toggled(bool)),this,SLOT(slotMutualToolbar(bool)));
    connect(draw_free,SIGNAL(toggled(bool)),this,SLOT(slotMutualToolbar(bool)));
    connect(draw_ellipse,SIGNAL(toggled(bool)),this,SLOT(slotMutualToolbar(bool)));
    connect(draw_polygon,SIGNAL(toggled(bool)),this,SLOT(slotMutualToolbar(bool)));

    connect(about,SIGNAL(triggered(bool)),this,SLOT(slotAboutDialog()));
    connect(undoviewShow,SIGNAL(triggered(bool)),this,SLOT(slotShowUndoView()));
}

void QVCVMainWindow::CreateToolbar()
{
    QToolBar *toolbar = new QToolBar;
    toolbar->addAction(draw_line);
    toolbar->addAction(draw_rect);
    toolbar->addAction(draw_free);
    toolbar->addAction(draw_ellipse);
    toolbar->addAction(draw_polygon);
    addToolBar(toolbar);

    mutual_action.append(draw_line);
    mutual_action.append(draw_rect);
    mutual_action.append(draw_free);
    mutual_action.append(draw_ellipse);
    mutual_action.append(draw_polygon);
}

void QVCVMainWindow::slotAboutDialog()
{
    AboutDialog about;
    about.exec();
}

void QVCVMainWindow::slotPanelShow(QControlPanel *panel)
{
    SetMenuStatus(false);
}

void QVCVMainWindow::slotPanelClose(QControlPanel *panel)
{
    SetMenuStatus(true);
    disconnect(panel,SIGNAL(PanelShow(QControlPanel*)),this,SLOT(slotPanelShow(QControlPanel*)));
    disconnect(panel,SIGNAL(PanelClose(QControlPanel*)),this,SLOT(slotPanelClose(QControlPanel*)));
}

void QVCVMainWindow::slotShowUndoView()
{
    if(dockwidget!=nullptr)
    {
        dockwidget->show();
        addDockWidget(Qt::LeftDockWidgetArea,dockwidget);
    }
    else {
        CreateUndoView();
    }
}

void QVCVMainWindow::slotMutualToolbar(bool checked)
{
    QAction *action = static_cast<QAction*>(sender());
    if(action==nullptr)
        return;

    if(checked)
    {
        for(int i=0;i<mutual_action.size();++i)
        {
            if(action!=mutual_action[i])
            {
                mutual_action[i]->setChecked(false);
            }
        }
    }
}

void QVCVMainWindow::SetMenuStatus(const bool flag)
{
    if(filter_menu!=nullptr)
        filter_menu->setEnabled(flag);
    if(morphological_menu!=nullptr)
        morphological_menu->setEnabled(flag);
    if(edge_menu!=nullptr)
        edge_menu->setEnabled(flag);
}
