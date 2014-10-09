
#include "lib/kml.hpp"

CKmlLoader::CKmlLoader(const QString fname) :
	CLoader(), fl(fname)
{
	throw_if(! fl.open(QIODevice::WriteOnly));

	stream.setDevice(& fl);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();

	stream.writeStartElement("kml");
	stream.writeAttribute("xmlns", "http://www.opengis.net/kml/2.2");

	stream.writeStartElement("Placemark");
	stream.writeTextElement("name", "iOSD Naza-M v2 log data - flight path (GPS/absolute)");
	stream.writeStartElement("MultiGeometry");
	stream.writeStartElement("LineString");

	stream.writeTextElement("extrude", "1");
	stream.writeTextElement("altitudeMode", "absolute");

	stream.writeStartElement("coordinates");
}

CKmlLoader::~CKmlLoader()
{
	stream.writeEndElement();
	stream.writeEndElement();
	stream.writeEndElement();
	stream.writeEndElement();
	stream.writeEndElement();
	stream.writeEndDocument();
}

void CKmlLoader::packet_1001(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content)
{
	;
}

void CKmlLoader::packet_1002(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content)
{
#define LON_LAT(name)\
	QString::number(content[name].toDouble() / M_PI * 180, 'g', 11)

#define DOUBLE(name)\
	QString::number(content[name].toDouble(), 'g', 11)

	const QString field = QString("%1,%2,%3\n")
		.arg(LON_LAT("longitude"))
		.arg(LON_LAT("latitude"))
		.arg(DOUBLE("altitude"));

	stream.writeCharacters(field);
}

void CKmlLoader::packet_TODO(const unsigned id)
{
	;
}

void CKmlLoader::packet_unknown(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload)
{
	;
}

void CKmlLoader::bad_packet(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload)
{
	;
}

