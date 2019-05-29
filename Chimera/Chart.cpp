#include "chart.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/qlegendmarker.h>
#include <QGraphicsLayout>


Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags) :
	QChart(QChart::ChartTypeCartesian, parent, wFlags),
	maxlength(0),
	y_max(-1.0e10),
	y_min(1.0e10),
	m_x(0),
	m_y(0),
	count(0)
{
	layout()->setContentsMargins(0, 0, 0, 0);
	setMargins(QMargins(0, 0, 0, 0));
	xaxis = new QValueAxis(this);
	yaxis = new QValueAxis(this);
	this->addAxis(xaxis, Qt::AlignBottom);
	this->addAxis(yaxis, Qt::AlignLeft);
	//this->axisX()->setMin(0);
}

Chart::~Chart()
{
}

void Chart::connectMarkers()
{
	int i = 0;
	const auto markers = this->legend()->markers();
	for (QLegendMarker *marker : markers) {
		i = i + 1;
		// Disconnect possible existing connection to avoid multiple connections
		QObject::disconnect(marker, &QLegendMarker::clicked,
			this, &Chart::handleMarkerClicked);
		QObject::connect(marker, &QLegendMarker::clicked, this, &Chart::handleMarkerClicked);
	}
	if (i > 4)
	{
		this->legend()->setAlignment(Qt::AlignLeft);
	}
	else
	{
		this->legend()->setAlignment(Qt::AlignBottom);
	}
}

void Chart::handleMarkerClicked()
{
	QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
	Q_ASSERT(marker);

	switch (marker->type())
	{
	case QLegendMarker::LegendMarkerTypeXY:
	{
		// Toggle visibility of series
		marker->series()->setVisible(!marker->series()->isVisible());

		// Turn legend marker back to visible, since hiding series also hides the marker
		// and we don't want it to happen now.
		marker->setVisible(true);

		// Dim the marker, if series is not visible
		qreal alpha = 1.0;

		if (!marker->series()->isVisible())
			alpha = 0.5;

		QColor color;
		QBrush brush = marker->labelBrush();
		color = brush.color();
		color.setAlphaF(alpha);
		brush.setColor(color);
		marker->setLabelBrush(brush);

		brush = marker->brush();
		color = brush.color();
		color.setAlphaF(alpha);
		brush.setColor(color);
		marker->setBrush(brush);

		QPen pen = marker->pen();
		color = pen.color();
		color.setAlphaF(alpha);
		pen.setColor(color);
		marker->setPen(pen);
		break;
	}
	default:
	{
		break;
	}
	}
}

void Chart::handleTimeout(const QString &str)
{
	QStringList list = str.split("#");
	int length = list.length() - 1;
	if (maxlength < length&&maxlength < 24)
	{
		for (int i = maxlength; i < length; i++)
		{
			m_series[i] = new QLineSeries(this);
			m_series[i]->setUseOpenGL(true);
			m_series[i]->setName(list[i + 1].split("$").first());
			this->addSeries(m_series[i]);
			m_series[i]->attachAxis(xaxis);
			m_series[i]->attachAxis(yaxis);
		}
		maxlength = length;
		connectMarkers();
	}
	m_x = list.first().split("$").last().toDouble();
	if (count == 0)
	{
		xaxis->setMin(m_x);
	}
	if (length > 0)
	{
		for (int i = 1; i < length + 1; i++)
		{
			m_y = list[i].split("$").last().toDouble();
			if (m_y > y_max || m_y < y_min)
			{
				y_max = qMax(m_y, y_max);
				y_min = qMin(m_y, y_min);
				if (y_max < 0)
				{
					yaxis->setRange(1.05*y_min, 0.95*y_max);
				}
				else
				{
					if (y_min > 0)
					{
						yaxis->setRange(0.95*y_min, 1.05*y_max);
					}
					else
					{
						yaxis->setRange(1.05*y_min, 1.05*y_max);
					}
				}
				if (y_max<1000.0&&y_min>-1000.0)
				{
					yaxis->setLabelFormat("%5.2f");
				}
				else
				{
					yaxis->setLabelFormat("%5.4E");
				}
			}
			m_series[i - 1]->append(m_x, m_y);
		}
	}
	xaxis->setMax(m_x);
	count = count + 1;
}

QWChartView::QWChartView(QWidget *parent)
	:QChartView(parent)
{
	setDragMode(QGraphicsView::RubberBandDrag);
	setRenderHint(QPainter::Antialiasing);
	setMouseTracking(true);
}

QWChartView::~QWChartView()
{

}

void QWChartView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		beginPoint = event->pos();
	}
	QChartView::mousePressEvent(event);
}

void QWChartView::mouseMoveEvent(QMouseEvent *event)
{
	QPoint point;
	point = event->pos();
	emit mouseMovePoint(point);
	QChartView::mouseMoveEvent(event);
}

void QWChartView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		endPoint = event->pos();
		QRectF rectF;
		rectF.setTopLeft(beginPoint);
		rectF.setBottomRight(endPoint);
		this->chart()->zoomIn(rectF);
	}
	else if (event->button() == Qt::RightButton) {
		this->chart()->zoomReset();
	}
	QChartView::mouseReleaseEvent(event);
}

void QWChartView::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Left:
		chart()->scroll(10, 0);
		break;
	case Qt::Key_Right:
		chart()->scroll(-10, 0);
		break;
	case Qt::Key_Up:
		chart()->scroll(0, 10);
		break;
	case Qt::Key_Down:
		chart()->scroll(0, -10);
		break;
	case Qt::Key_PageUp:
		chart()->scroll(0, 50);
		break;
	case Qt::Key_PageDown:
		chart()->scroll(0, -50);
		break;
	case Qt::Key_Home:
		chart()->zoomReset();
		break;
	default:
		QGraphicsView::keyPressEvent(event);
	}
}