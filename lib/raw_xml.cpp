
#include "lib/raw_xml.hpp"

CRawXmlLoader::CRawXmlLoader(const QString fname) :
	CLoader(), fl(fname)
{
	throw_if(! fl.open(QIODevice::WriteOnly));

	stream.setDevice(& fl);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();
	stream.writeStartElement("naza");
}

CRawXmlLoader::~CRawXmlLoader()
{
	stream.writeEndElement();
	stream.writeEndDocument();
}

void CRawXmlLoader::packet(const unsigned id, const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content)
{
	stream.writeStartElement("packet");
	stream.writeAttribute("id", QString::number(id, 16));
	stream.writeTextElement("real_sequence", QString::number(real_sequence));
	stream.writeTextElement("sequence", QString::number(sequence));

	for(auto & key : content.keys())
	{
		QString elem;

		switch(content[key].type())
		{
			case QVariant::Double:
			{
				elem = QString::number(content[key].toDouble(), 'g', 11);

				break;
			}
			case QVariant::Int:
			{
				elem = QString::number(content[key].toInt());

				break;
			}
			case QVariant::UInt:
			{
				elem = QString::number(content[key].toUInt());

				break;
			}
			default:
			{
				printf_TODO("Unknow type of %s\n", key.toStdString().c_str());

				break;
			}
		}

		stream.writeTextElement(key, elem);
	}

	stream.writeEndElement();
}

void CRawXmlLoader::packet_TODO(const unsigned id)
{
	printf_TODO("TODO packet with id = 0x%X\n", id);
}

void CRawXmlLoader::packet_unknown(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload)
{
	printf_TODO("Unknown packet with id = 0x%X\n", id);
}

void CRawXmlLoader::bad_packet(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload)
{
	printf_TODO("Bad packet with id = 0x%X\n", id);
}

