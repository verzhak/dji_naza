
#ifndef CONVERT_DIALOG_HPP
#define CONVERT_DIALOG_HPP

#include "all.hpp"
#include "ui_convert_dialog.h"
#include "lib/base.hpp"
#include "lib/raw_xml.hpp"
#include "lib/super_stend_xml.hpp"
#include "lib/kml.hpp"
#include "ui/QProgressIndicator/QProgressIndicator.h"

struct SFilterDesc
{
	QString id;
	QString suffix;

	SFilterDesc();
	SFilterDesc(const QString __id, const QString __suffix);
	SFilterDesc & operator=(const SFilterDesc & flt);
};

class CConvertDialog : public QDialog, private Ui::convert_dialog
{
	Q_OBJECT

	QString src_fname, dst_fname, dst_filter_name;
	QMap<QString, SFilterDesc> dst_filters;

	void refresh();

	public:

		CConvertDialog(QWidget * parent);

	public slots:

		void on_src_button_clicked();
		void on_dst_button_clicked();
		void on_convert_button_clicked();
};

#endif

