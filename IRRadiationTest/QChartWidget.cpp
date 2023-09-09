#include "QChartWidget.h"


QChartWidget::QChartWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_pchartView = nullptr;
	m_pchart = nullptr;

	m_lineCount = 0;
	m_color = QColor(255, 255, 255);

	m_enTheme = QChart::ChartTheme::ChartThemeBlueIcy;   //   ChartThemeBlueIcy;  ChartThemeBrownSand;  

	m_pchart = new QChart();
	m_pchartLine = new QChart();
	m_pchartView = new ChartLineView(m_pchartLine); 

	m_pcView = new ChartView(m_pchart);
}

QChartWidget::~QChartWidget()
{
	if (m_pchart) {
		delete m_pchart;
		m_pchart = nullptr;
	}

	if (m_pchartView) {
		delete m_pchartView;
		m_pchartView = nullptr;
	}

	//if (m_pchartLine) {
	//	delete m_pchartLine;
	//	m_pchartLine = nullptr;
	//}

	//if (m_pcView) {
	//	delete m_pcView;
	//	m_pcView = nullptr;
	//}
}
void QChartWidget::SetColor(QColor color)
{
	m_color = color;
}

void QChartWidget::DrawPie(QString strTitle, std::map<QString, float> datamap)
{
	if (datamap.size() <= 0) {
		m_pchart->removeAllSeries();
		return;
	}

	m_pchart->removeAllSeries();

	QPieSeries* pPie = new QPieSeries();


	for (auto it = datamap.begin(); it != datamap.end(); it++) {
		pPie->append(it->first, it->second);
	}

	pPie->setLabelsVisible(true);

	m_pchart->series().clear();
	m_pchart->addSeries(pPie);
	m_pchart->setTitle(strTitle);
	m_pchart->setTheme(m_enTheme);


	m_pcView->setBackgroundBrush(QBrush(m_color));
	m_pcView->setRenderHint(QPainter::Antialiasing);

	//m_playout->removeWidget(m_pcView);
	//m_playout->addWidget(m_pcView);

	ui.gridLayout->removeWidget(m_pcView);
	ui.gridLayout->addWidget(m_pcView);
	//m_pcView->show();
}

void QChartWidget::DrawBar(QString strTitle, std::map<QString, std::vector<int>> datamap)
{
	if (datamap.size() <= 0) {
		m_pchart->removeAllSeries();
		return;
	}

	m_pchart->removeAllSeries();
	QBarSeries* pBar = new QBarSeries();

	int count = 0;
	for (auto it = datamap.begin(); it != datamap.end(); it++) {

		std::vector<int> datalist = it->second;
		QBarSet* set = new QBarSet(QString::fromLocal8Bit("%1").arg(it->first));
		for (size_t i = 0; i < datalist.size(); i++) {
			*set << datalist[i];
		}

		pBar->append(set);
		count++;
	}

	pBar->setLabelsVisible(true);
	m_pchart->series().clear();

	m_pchart->addSeries(pBar);
	m_pchart->setTitle(strTitle);
	m_pchart->setAnimationOptions(QChart::SeriesAnimations);

	m_pchart->createDefaultAxes();

	m_pchart->setTitleBrush(QColor(0, 0, 0));  //设置标题颜色
	m_pchart->legend()->setVisible(true);
	m_pchart->legend()->setAlignment(Qt::AlignBottom);
	m_pchart->setTheme(m_enTheme);

	m_pcView->setBackgroundBrush(QBrush(m_color));
	m_pcView->setRenderHint(QPainter::Antialiasing);

	ui.gridLayout->removeWidget(m_pcView);
	ui.gridLayout->addWidget(m_pcView);
}

void QChartWidget::DrawLine(QString strTitle, std::map<QString, std::vector<std::pair<float, float>>> datamap)
{

	m_lineCount = datamap.size();
	QLineSeries** pLineSeries = new QLineSeries * [m_lineCount];
	QColor color = QColor(255, 0, 0);

	int count = 0;
	m_pchartLine->removeAllSeries();

	for (auto it = datamap.begin(); it != datamap.end(); it++) {

		std::vector<std::pair<float, float>> datalist = it->second;

		pLineSeries[count] = new QLineSeries;
		pLineSeries[count]->setName(it->first);
		if (count % 2 == 0)
		{
			pLineSeries[count]->setPen(QPen(QColor(255, 0, 0), 1));
		}
		else
		{
			pLineSeries[count]->setPen(QPen(QColor(0, 0, 255), 1));
		}

		for (size_t i = 0; i < datalist.size(); i++) {
			pLineSeries[count]->append(datalist[i].first, datalist[i].second);
		}

		//pLineSeries[count]->setPointLabelsVisible(true);

		m_pchartLine->addSeries(pLineSeries[count]);
		count++;
	}

	//m_pchart->legend()->hide();
	m_pchartLine->createDefaultAxes();
	m_pchartLine->setTitle(strTitle);
	m_pchartLine->setTheme(m_enTheme);

	m_pchartView->setRenderHint(QPainter::Antialiasing);

	ui.gridLayout->removeWidget(m_pchartView);
	ui.gridLayout->addWidget(m_pchartView);
}

void QChartWidget::DrawLine(QString strTitle, std::map<QString, std::vector<glm::vec2>> datamap)
{
	if (datamap.size() <= 0) {
		m_pchartLine->removeAllSeries();
		return;
	}

	m_lineCount = datamap.size();
	QLineSeries** pLineSeries = new QLineSeries * [m_lineCount];

	int count = 0;
	m_pchartLine->removeAllSeries();

	for (auto it = datamap.begin(); it != datamap.end(); it++) {

		std::vector<glm::vec2> datalist = it->second;
		pLineSeries[count] = new QLineSeries;
		pLineSeries[count]->setName(it->first);
		if (count % 2 == 0)
		{
			pLineSeries[count]->setPen(QPen(QColor(255, 0, 0), 1));
		}
		else
		{
			pLineSeries[count]->setPen(QPen(QColor(0, 0, 255), 1));
		}

		for (size_t i = 0; i < datalist.size(); i++) {

			pLineSeries[count]->append(datalist[i].x, datalist[i].y);
		}
		m_pchartLine->addSeries(pLineSeries[count]);
		count++;
	}

	m_pchartLine->createDefaultAxes();
	m_pchartLine->setTheme(m_enTheme);

	m_pchartView->setRenderHint(QPainter::Antialiasing);

	ui.gridLayout->removeWidget(m_pchartView);
	ui.gridLayout->addWidget(m_pchartView);
}

int QChartWidget::GetLineCount()

{
	return m_lineCount;
}

