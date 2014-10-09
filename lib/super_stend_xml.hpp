
#ifndef SUPER_STEND_XML_HPP
#define SUPER_STEND_XML_HPP

#include "all.hpp"
#include "lib/base.hpp"

class CSuperStendXmlLoader : public CLoader
{
	QFile fl;
	QXmlStreamWriter stream;
	double course = 0, roll = 0, pitch = 0;

	public:

		CSuperStendXmlLoader(const QString fname);
		~CSuperStendXmlLoader();

		void packet_1001(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content) { ; };
		void packet_1002(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content);
		void packet_TODO(const unsigned id) { ; };
		void packet_unknown(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload) { ; };
		void bad_packet(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload) { ; };
};

#endif

