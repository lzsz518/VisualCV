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

    QAction *about;

    QAction *undoviewShow;

	QMdiArea *mdi_area;

	QTabWidget *tabwidget;
    QUndoView *undoview;
    QDockWidget *dockwidget;
    VCV_IMAGE_OPERATION image_operation;

	void InitUI();
	void CreateAction();
	void CreateMenu();
    void CreateUndoView();
	void CreateConnection();

    void ShowFilterPanel(VCV_IMAGE_OPERATION operation);
    void ShowThresholdPanel(VCV_IMAGE_OPERATION operation);
    void ShowMorphologicalPanel(VCV_IMAGE_OPERATION operation);
    void ShowEdgeDetectionPanel(VCV_IMAGE_OPERATION operation);


    void SetMenuStatus(const bool flag);
};

#endif
