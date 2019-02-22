#ifndef VCVCHILDWINDOW_H
#define VCVCHILDWINDOW_H

#include <QWidget>
#include "SystemDefine.h"
class QMenu;
class QAction;
class QScrollBar;
class QFilterPanel;
class QControlPanel;
class QVCVData;
class QCommandBuilder;
class QVCVUndoCommand;
class QImageProcCommand;
class QCustomFilterDlg;
class QThresholdPanel;
class QMorphologyPanel;
class QEdgeDetectionPanel;

#define SCROLLBAR_SIZE 20

class QVCVChildWindow : public QWidget
{
	Q_OBJECT
public:
    QVCVChildWindow(QWidget *parent = nullptr, Qt::WindowFlags f = nullptr);
	~QVCVChildWindow();

public:

	bool Update(const Mat &image);

	void SetDisplayScale(float scale);
	float GetDisplayScale();

    QFilterPanel* GetFilterPanel(VCV_IMAGE_OPERATION operation);
    QCustomFilterDlg* GetCustomFilterPanel();
    QThresholdPanel* GetThresholdPanel(VCV_IMAGE_OPERATION operation);
    QMorphologyPanel* GetMorphologyPanel(VCV_IMAGE_OPERATION operation);
    QEdgeDetectionPanel* GetEdgeDetectionPanel(VCV_IMAGE_OPERATION operation);
public slots:
    void FilterParameterChangeRespond(const CommandParameter *para);
    void FilterPanelOk(const CommandParameter *para);
    void FilterPanelCancel(const CommandParameter *para);
    void CustomFilterParameterChange(const CommandParameter *para);
    void CustomFilterPanelOk(const CommandParameter *para);
    void CustomFilterPanelCancel(const CommandParameter *para);
    void ThresholdParameterChange(const CommandParameter *para);
    void ThresholdPanelOk(const CommandParameter *para);
    void ThresholdPanelCancel(const CommandParameter *para);
    void MorphologyParameterChange(const CommandParameter *para);
    void MorphologyPanelOk(const CommandParameter *para);
    void MorphologyPanelCancel(const CommandParameter *para);
    void EdgeDetectionParameterChange(const CommandParameter *para);
    void EdgeDetectionPanelOk(const CommandParameter *para);
    void EdgeDetectionPanelCancel(const CommandParameter *para);
protected slots:
	void VScrollBarRangeChanged(int min, int max);
	void HScrollBarRangeChanged(int min, int max);

protected:
	virtual void paintEvent(QPaintEvent *e);
    virtual void closeEvent(QCloseEvent *e);
	virtual void resizeEvent(QResizeEvent *e);

protected:
	QImage *update_image;

	QScrollBar *h_scrollbar;
	QScrollBar *v_scrollbar;

    QFilterPanel *filter_panel;
    QCustomFilterDlg *customfilter_panel;
    QThresholdPanel *threshold_panel;
    QMorphologyPanel *morphology_panel;
    QEdgeDetectionPanel *edgedetection_panel;

    QCommandBuilder *command_builder;
    QImageProcCommand *filter_command;
    QImageProcCommand *custom_filter_command;
    QImageProcCommand *threshold_command;
    QImageProcCommand *morphology_command;
    QImageProcCommand *edgedetection_command;
    QVCVData *operator_data;

	int image_top;
	int image_left;
	QRect image_display_area;
	QRect widget_display_area;
	float display_scale;

    void AdjustScrollBar();
    void AdjustScrollBarRange();

    void DrawClient();

    void DoOperation(QControlPanel *panel, QVCVUndoCommand **command, VCV_IMAGE_OPERATION operation);
    void PanelValueChange(QVCVUndoCommand *command, const CommandParameter *para);
    void PanelOk(QVCVUndoCommand **command, const CommandParameter *para);
    void PanelCencel(QVCVUndoCommand **command, const CommandParameter *para);
};

#endif
