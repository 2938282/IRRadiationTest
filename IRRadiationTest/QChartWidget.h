#ifndef QCHARTWIDGET_H
#define QCHARTWIDGET_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QValueAxis> 
#include <QGridLayout>
#include "ChartLineView.h"
#include "chartview.h"

#include "glm/detail/type_vec2.hpp"

#include "ui_QChartWidget.h"
QT_CHARTS_USE_NAMESPACE

typedef struct _DATA_X_Y
{
	float x;
	float y;
}DATA_X_Y;

class QChartWidget : public QWidget
{
	Q_OBJECT

public:
	QChartWidget(QWidget *parent = 0);
	~QChartWidget();

private:
	Ui::QChartWidget ui;
public:
	void SetColor(QColor color);
	//绘制饼图 strTitle 标题, map<名称，数据>
	void DrawPie(QString strTitle, std::map<QString, float> datamap);
	//绘制柱状图 strTitle 标题, map<名称，数据>
	void DrawBar(QString strTitle, std::map<QString, std::vector<int>> datamap);
	//绘制曲线 strTitle 标题, map<名称，数据>
	void DrawLine(QString strTitle, std::map<QString, std::vector<std::pair<float, float>>> datamap);
	//绘制曲线 strTitle 标题, map<名称，数据>
	void DrawLine(QString strTitle, std::map<QString, std::vector<glm::vec2>> datamap);

	// 返回曲线数量
	int GetLineCount();

	//设置主题
	inline void SetTheme(QChart::ChartTheme enTheme) { m_enTheme = enTheme; };
private:
	ChartLineView *m_pchartView;
	ChartView *m_pcView;
	
	QChart *m_pchart;
	QChart *m_pchartLine;
	QColor m_color;

	QChart::ChartTheme  m_enTheme;

	//QGridLayout* m_playout;
	int m_lineCount;
};

#endif // QCHARTWIDGET_H
