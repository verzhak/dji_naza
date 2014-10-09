
#include "all.hpp"
#include "ui/main_window.hpp"

int main(int argc, char * argv[])
{
	int ret = 0;

	amv_init();

	try
	{
		QApplication app(argc, argv);
		CMainWindow main_window;

		Q_INIT_RESOURCE(resources);
		main_window.show();
		ret = app.exec();
	}
	catch(...)
	{
		ret = -1;
	}

	amv_destroy();

	return ret;
}

