#include "Application.h"
#include "QtAwesome/QtAwesome.h"
#include <QColor>

Application::Application(int &argc,char **argv)
	:QApplication(argc,argv)
{
	_qtawesome = new QtAwesome(this);
	_qtawesome->initFontAwesome();
	setApplicationName("Chimera");
	setOrganizationName("XTinc");
}


Application::~Application()
{
	delete _qtawesome;
}

QtAwesome * Application::qtAwesome() const
{
	return _qtawesome;
}

QFont Application::iconFont(int size) const
{
	return _qtawesome->font(size);
}

Application * myApp()
{
	return static_cast<Application*>(qApp);
}
