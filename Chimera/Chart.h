#pragma once
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCore/QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPoint>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

//![1]
class Chart : public QChart
{
	Q_OBJECT
public:
	Chart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = nullptr);
	virtual ~Chart();

public slots:
	void handleTimeout(const QString &str);

private:
	QValueAxis *xaxis, *yaxis;
	QLineSeries *m_series[12];
	QStringList m_titles;
	int maxlength;
	qreal y_max, y_min;
	qreal m_x;
	qreal m_y;
	void connectMarkers();
	void handleMarkerClicked();
	int count;
};

class QWChartView : public QChartView
{
	Q_OBJECT
public:
	QWChartView(QWidget *parent = nullptr);
	~QWChartView() override;
signals:
	void mouseMovePoint(QPoint point);
protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
private:
	QPoint beginPoint;
	QPoint endPoint;
};