#include "ChartLineView.h"
#include <QApplication>
#include <QtCharts/QValueAxis>

ChartLineView::ChartLineView(QChart *chart, QWidget *parent)
    : QChartView(chart, parent)
{
	m_isPress = false;
	m_ctrlPress = false;
	m_alreadySaveRange = false;
	m_coordItem = nullptr;

	this->setDragMode(QGraphicsView::RubberBandDrag);
	this->setMouseTracking(false);
	setCursor(QCursor(Qt::PointingHandCursor)); //设置鼠标指针为手指形
}

ChartLineView::~ChartLineView()
{
}

void ChartLineView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
        m_lastPoint = event->pos();
        m_isPress = true;
	}
}

void ChartLineView::mouseMoveEvent(QMouseEvent *event)
{
	if (!m_coordItem)
	{
		m_coordItem = new QGraphicsSimpleTextItem(this->chart());
		m_coordItem->setZValue(5);
		m_coordItem->setPos(100, 60);
		m_coordItem->show();
	}
	const QPoint curPos = event->pos();
	QPointF curVal = this->chart()->mapToValue(QPointF(curPos));
	QString coordStr = QString("X = %1, Y = %2").arg(curVal.x()).arg(curVal.y());
	m_coordItem->setText(coordStr);

	if (m_isPress)
	{
		QPoint offset = curPos - m_lastPoint;
		m_lastPoint = curPos;
		if (!m_alreadySaveRange)
		{
			this->saveAxisRange();
			m_alreadySaveRange = true;
		}
		this->chart()->scroll(-offset.x(), offset.y());
	}
}

void ChartLineView::mouseReleaseEvent(QMouseEvent *event)
{
	m_isPress = false;
	if (event->button() == Qt::RightButton)
	{
		if (m_alreadySaveRange)
		{
			this->chart()->axisX()->setRange(m_xMin, m_xMax);
			this->chart()->axisY()->setRange(m_yMin, m_yMax);
		}
	}
}

void ChartLineView::wheelEvent(QWheelEvent *event)
{
	const QPoint curPos = event->pos();
	QPointF curVal = this->chart()->mapToValue(QPointF(curPos));

	if (!m_alreadySaveRange)
	{
		this->saveAxisRange();
		m_alreadySaveRange = true;
	}

	const double factor = 1.5;//缩放比例
	if (m_ctrlPress)
	{//Y轴
		QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
		if (!axisY) {
			return;
		}
		const double yMin = axisY->min();
		const double yMax = axisY->max();
		const double yCentral = curVal.y();

		double bottomOffset;
		double topOffset;
		if (event->delta() > 0)
		{//放大
			bottomOffset = 1.0 / factor * (yCentral - yMin);
			topOffset = 1.0 / factor * (yMax - yCentral);
		}
		else
		{//缩小
			bottomOffset = 1.0 * factor * (yCentral - yMin);
			topOffset = 1.0 * factor * (yMax - yCentral);
		}

		this->chart()->axisY()->setRange(yCentral - bottomOffset, yCentral + topOffset);
	}
	else
	{//X轴
		QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
		if (!axisX) {
			return;
		}
		const double xMin = axisX->min();
		const double xMax = axisX->max();
		const double xCentral = curVal.x();

		double leftOffset;
		double rightOffset;
		if (event->delta() > 0)
		{//放大
			leftOffset = 1.0 / factor * (xCentral - xMin);
			rightOffset = 1.0 / factor * (xMax - xCentral);
		}
		else
		{//缩小
			leftOffset = 1.0 * factor * (xCentral - xMin);
			rightOffset = 1.0 * factor * (xMax - xCentral);
		}

		this->chart()->axisX()->setRange(xCentral - leftOffset, xCentral + rightOffset);
	}
}

void ChartLineView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Control)
	{
		m_ctrlPress = true;
    }
}

void ChartLineView::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Control)
	{
		m_ctrlPress = false;
	}
}

void ChartLineView::saveAxisRange()
{
	QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
	if (axisX) {
		m_xMin = axisX->min();
		m_xMax = axisX->max();
	}
	
	QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
	if (axisY) {
		m_yMin = axisY->min();
		m_yMax = axisY->max();
	}
	
	
}

