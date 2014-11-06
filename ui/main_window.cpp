
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

	add_action(trUtf8("Отладка"), ":/icons/debug", SLOT(debug()));
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

void CMainWindow::debug()
{
	const QString src_fname = "/home/amv/disser/data/uav/17-14/Полеты_28_09_2014/Полетные данные/FLY084.DAT";
	const QString dst_fname = "1.xml";
	shared_ptr<CLoader> loader;
	CLoader * p_loader;

	loader.reset(new CSuperStendXmlLoader(dst_fname));
	throw_null(p_loader = loader.get());
	process(src_fname, loader.get());
}

