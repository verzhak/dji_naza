
#ifndef KML_HPP
#define KML_HPP

#include "all.hpp"
#include "lib/base.hpp"

class CKmlLoader : public CLoader
{
	QFile fl;
	QXmlStreamWriter stream;

	public:

		CKmlLoader(const QString fname);
		~CKmlLoader();

		void packet_1001(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content);
		void packet_1002(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content);
		void packet_TODO(const unsigned id);
		void packet_unknown(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload);
		void bad_packet(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload);
};

#endif

