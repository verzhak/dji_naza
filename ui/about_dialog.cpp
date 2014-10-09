
#include "ui/about_dialog.hpp"

CAboutDialog::CAboutDialog(QWidget * parent) : QDialog(parent)
{
	setupUi(this);

	connect(close_button, SIGNAL(clicked()), this, SLOT(accept()));
}

