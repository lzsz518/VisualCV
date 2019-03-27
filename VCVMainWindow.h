#ifndef QVCVMAINWINDOW_H
#define QVCVMAINWINDOW_H

#include <QMainWindow>
#include "SystemDefine.h"

class QMenu;
class QAction;
class QMdiArea;
class QVCVData;
class QSplitter;
class QUndoView;
class QTabWidget;
class QControlPanel;
class QFilterPanel;
class QDockWidget;
class QMdiSubWindow;
class QCustomFilterDlg;
class QCommandBuilder;
class QVCVUndoCommand;
class QVCVChildWindow;


class QVCVMainWindow : public QMainWindow
{
	Q_OBJECT
public:
    QVCVMainWindow(QWidget *parent = nullptr, Qt::WindowFlags f= Qt::Widget);
	~QVCVMainWindow();

private:
	QVCVMainWindow(const QVCVMainWindow &VCVMainWindow);
    const QVCVMainWindow operator = (const QVCVMainWindow &VCVMainWindow);

private slots:
	void WindowActive(QMdiSubWindow *subwin);
	void New();
	void OpenFile();
	void Save();
	void Saveas();
	void BlurFilter();
	void GaussianBlurFilter();
	void MedianBlurFilter();
	void BilateralFilter();
	void CustomFilter2D();
	void Erosion();
	void Dilation();
    void MorphologyOpen();
    void Threshold();
    void AdaptiveThreshold();
    void Laplacian_action();
    void Sobel_action();
    void Canny_action();

	void ZoomIn();
	void ZoomOut();
	void NormalSize();

    void slotAboutDialog();

    void slotPanelShow(QControlPanel *panel);
    void slotPanelClose(QControlPanel *panel);

    void slotShowUndoView();

    void slotMutualToolbar(bool checked);
    void slotActionDrawLine(bool checked);
    void slotActionDrawRect(bool checked);
    void slotActionDrawFree(bool checked);
    void slotActionDrawEllipse(bool checked);
    void slotActionDrawPolygon(bool checked);
    void slotActionSelectColor();
private:
	QMenu *file_menu;
	QMenu *view_menu;
	QMenu *edit_menu;
    QMenu *window_menu;
	QMenu *help_menu;

	QMenu *filter_menu;
    QMenu *morphological_menu;
    QMenu *edge_menu;

	QAction *file_menu_new;
	QAction *file_menu_open;
	QAction *file_menu_save;
	QAction *file_menu_saveas;

	QAction *view_tile;
	QAction *view_cascade;
	QAction *view_zoomin;
	QAction *view_zoomout;
	QAction *view_normalsize;

	QAction *filter_blur;
	QAction *filter_gaussian;
	QAction *filter_median;
	QAction *filter_bilateral;
	QAction *filter_custom2d;

    QAction *morphological_erosion;
    QAction *morphological_dilation;
    QAction *morphological_open;
    QAction *morphological_threshold;
    QAction *morphological_adaptivethreshold;

    QAction *edge_laplaction;
    QAction *edge_sobel;
    QAction *edge_canny;

    QAction *draw_line;
    QAction *draw_rect;
    QAction *draw_free;
    QAction *draw_ellipse;
    QAction *draw_polygon;
    QAction *color_select;

    QAction *about;

    QAction *undoviewShow;

	QMdiArea *mdi_area;

    MainWindowStatus *mainwindow_status;

	QTabWidget *tabwidget;
    QUndoView *undoview;
    QDockWidget *dockwidget;
    VCV_IMAGE_OPERATION image_operation;

    QVector<QAction*> mutual_action;

	void InitUI();
	void CreateAction();
	void CreateMenu();
    void CreateUndoView();
	void CreateConnection();
    void CreateToolbar();

    void ShowFilterPanel(VCV_IMAGE_OPERATION operation);
    void ShowThresholdPanel(VCV_IMAGE_OPERATION operation);
    void ShowMorphologicalPanel(VCV_IMAGE_OPERATION operation);
    void ShowEdgeDetectionPanel(VCV_IMAGE_OPERATION operation);

    void SetMenuStatus(const bool flag);
    void SetDrawToolKitStatus(const DrawStatus status, const bool checked);
};

#endif
