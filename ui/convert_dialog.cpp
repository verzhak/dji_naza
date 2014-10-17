
#include "ui/convert_dialog.hpp"

SFilterDesc::SFilterDesc() :
	id(""), suffix("")
{
	;
}

SFilterDesc::SFilterDesc(const QString __id, const QString __suffix) :
	id(__id), suffix(__suffix)
{
	;
}

SFilterDesc & SFilterDesc::operator=(const SFilterDesc & flt)
{
	id = flt.id;
	suffix = flt.suffix;

	return * this;
}

// ############################################################################ 

CConvertDialog::CConvertDialog(QWidget * parent) : QDialog(parent)
{
	setupUi(this);

	dst_filters["Super stend format (*.xml)"] = SFilterDesc("super_stend_xml", "xml");
	dst_filters["Raw XML (*.xml)"] = SFilterDesc("raw_xml", "xml");
	dst_filters["KML (*.kml)"] = SFilterDesc("kml", "kml");
}

void CConvertDialog::on_src_button_clicked()
{
	QFileDialog dia(this);

	dia.setAcceptMode(QFileDialog::AcceptOpen);
	dia.setFileMode(QFileDialog::ExistingFile);
	dia.setNameFilter(tr("Naza file (*.dat)"));
	dia.setViewMode(QFileDialog::Detail);
	
	if(dia.exec())
	{
		QStringList fnames = dia.selectedFiles();

		if(fnames.size())
			src_fname = fnames[0];
		else
			src_fname = "";
	}
	else
		src_fname = "";

	refresh();
}

void CConvertDialog::on_dst_button_clicked()
{
	QFileDialog dia(this);
	QString filter, suffix;
	const QStringList dst_filters_keys = dst_filters.keys();
	const unsigned dst_filters_num = dst_filters.size();
	unsigned v;

	dia.setAcceptMode(QFileDialog::AcceptSave);
	dia.setFileMode(QFileDialog::AnyFile);
	dia.setViewMode(QFileDialog::Detail);

	for(v = 0; v < dst_filters_num - 1; v++)
		filter += dst_filters_keys[v] + ";;";

	filter += dst_filters_keys[v];

	dia.setNameFilter(filter);
	
	if(dia.exec())
	{
		QStringList fnames = dia.selectedFiles();

		if(fnames.size())
		{
			dst_fname = fnames[0];
			dst_filter_name = dia.selectedNameFilter();
			suffix = dst_filters[dst_filter_name].suffix;
			QRegExp reg_exp = QRegExp(QString("*.%1").arg(suffix));

			reg_exp.setPatternSyntax(QRegExp::Wildcard);

			if(! reg_exp.exactMatch(dst_fname))
				dst_fname += QString(".%1").arg(suffix);
		}
		else
			dst_fname = "";
	}
	else
		dst_fname = "";

	refresh();
}

void CConvertDialog::refresh()
{
	convert_button->setEnabled(! (src_fname.isEmpty() || dst_fname.isEmpty()));

	src_edit->setText(src_fname);
	dst_edit->setText(dst_fname);
}

void CConvertDialog::on_convert_button_clicked()
{
	const QString dst_filter_id = dst_filters[dst_filter_name].id;
	shared_ptr<CLoader> loader;
	CLoader * p_loader;

	if(dst_filter_id == "raw_xml")
		loader.reset(new CRawXmlLoader(dst_fname));
	else if(dst_filter_id == "super_stend_xml")
		loader.reset(new CSuperStendXmlLoader(dst_fname));
	else if(dst_filter_id == "kml")
		loader.reset(new CKmlLoader(dst_fname));
	else
		throw_;
	
	throw_null(p_loader = loader.get());
	process(src_fname, p_loader);
}

