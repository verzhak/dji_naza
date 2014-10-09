
#include "ui/main_window.hpp"

CMainWindow::CMainWindow() :
	QMainWindow(NULL)
{
	setupUi(this);

	auto add_action = [ this ](const QString title, const QString icon_name, const char * fun_name)
	{
		QAction * action = tool_bar->addAction(QIcon(icon_name), title, this, fun_name);
		
		action->setToolTip(title);
		action->setStatusTip(title);

		return action;
	};

	add_action(trUtf8("Конвертировать"), ":/icons/convert", SLOT(convert()));
	add_action(trUtf8("О программе"), ":/icons/about", SLOT(about()));
}

CMainWindow::~CMainWindow()
{
	;
}

void CMainWindow::about()
{
	try
	{
		CAboutDialog(this).exec();
	}
	catch(...)
	{
		;
	}
}

void CMainWindow::convert()
{
	try
	{
		CConvertDialog(this).exec();
	}
	catch(...)
	{
		;
	}
}

