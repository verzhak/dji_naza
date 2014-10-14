
#include "lib/super_stend_xml.hpp"

CSuperStendXmlLoader::CSuperStendXmlLoader(const QString fname) :
	CLoader(), fl(fname)
{
	throw_if(! fl.open(QIODevice::WriteOnly));

	stream.setDevice(& fl);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();
	stream.writeStartElement("amv");
}

CSuperStendXmlLoader::~CSuperStendXmlLoader()
{
	stream.writeEndElement();
	stream.writeEndDocument();
}

void CSuperStendXmlLoader::packet_1002(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content)
{
#define ANGLE_TRANSLATE(from_angle, to_angle)\
	to_angle += 0.001 * content[from_angle].toDouble() / M_PI * 180;

	ANGLE_TRANSLATE("gyroscope_Z", course);
	ANGLE_TRANSLATE("gyroscope_X", roll);
	ANGLE_TRANSLATE("gyroscope_Y", pitch);

	stream.writeStartElement("frame");

#define WRITE_ELEM(elem, type, value)\
	stream.writeStartElement(elem);\
	stream.writeAttribute("type", type);\
	stream.writeCharacters(value);\
	stream.writeEndElement();

#define WRITE_ANGLE(elem, value)\
	WRITE_ELEM(elem, "double", QString::number(value / M_PI * 180, 'g', 11));

	WRITE_ANGLE("x", content["latitude"].toDouble());
	WRITE_ANGLE("y", content["longitude"].toDouble());
	WRITE_ELEM("h", "double", QString::number(content["altitude"].toDouble(), 'g', 3));
	WRITE_ANGLE("course", course);
	WRITE_ANGLE("roll", roll);
	WRITE_ANGLE("pitch", pitch);
	WRITE_ELEM("aspect_x", "double", QString::number(44.9, 'g', 3));
	WRITE_ELEM("aspect_y", "double", QString::number(26.6, 'g', 3));
	WRITE_ELEM("coord_system", "unsigned", QString::number(2));

	stream.writeEndElement();
}

