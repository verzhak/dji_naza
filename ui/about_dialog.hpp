
#ifndef ABOUT_DIALOG_HPP
#define ABOUT_DIALOG_HPP

#include "all.hpp"
#include "ui_about_dialog.h"

class CAboutDialog : public QDialog, private Ui::about_dialog
{
	Q_OBJECT

	public:

		CAboutDialog(QWidget * parent);
};

#endif


