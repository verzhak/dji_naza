
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "all.hpp"
#include "ui_main_window.h"
#include "ui/convert_dialog.hpp"
#include "ui/about_dialog.hpp"

class CMainWindow : public QMainWindow, private Ui::main_window
{
	Q_OBJECT

	public:

		CMainWindow();
		~CMainWindow();

	public slots:

		void convert();
		void about();
};

#endif

