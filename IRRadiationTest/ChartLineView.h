﻿#ifndef __QCHARTLINEVIEW_H_
#define __QCHARTLINEVIEW_H_

#include <QtCharts/QChartView>
#include <QMouseEvent>
#include <QGraphicsSimpleTextItem>

QT_CHARTS_USE_NAMESPACE

class ChartLineView : public QChartView
{
	Q_OBJECT

public:
	ChartLineView(QChart *chart, QWidget *parent = nullptr);
	~ChartLineView();
	// 保存坐标区域，用于复位
	void saveAxisRange();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	QPoint m_lastPoint;
	bool m_isPress;
	bool m_ctrlPress;
	bool m_alreadySaveRange;
	double m_xMin, m_xMax, m_yMin, m_yMax;
	QGraphicsSimpleTextItem* m_coordItem;
};
#endif