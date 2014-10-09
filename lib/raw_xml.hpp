
#ifndef RAW_XML_HPP
#define RAW_XML_HPP

#include "all.hpp"
#include "lib/base.hpp"

class CRawXmlLoader : public CLoader
{
	QFile fl;
	QXmlStreamWriter stream;

	void packet(const unsigned id, const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content);

	public:

		CRawXmlLoader(const QString fname);
		~CRawXmlLoader();

		void packet_1001(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content) { packet(0x1001, real_sequence, sequence, content); };
		void packet_1002(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content) { packet(0x1002, real_sequence, sequence, content); };
		void packet_TODO(const unsigned id);
		void packet_unknown(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload);
		void bad_packet(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload);
};

#endif

