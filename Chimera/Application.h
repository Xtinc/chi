#pragma once
#include <QApplication>
#include <QFont>

class QtAwesome;

class Application :
	public QApplication
{
	Q_OBJECT
public:
	Application(int &argc,char **argv);
	virtual~Application();
	QtAwesome *qtAwesome()const;
	QFont iconFont(int size = 12)const;
private:
	QtAwesome *_qtawesome;
};

extern Application *myApp();