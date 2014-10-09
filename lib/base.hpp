
#ifndef BASE_HPP
#define BASE_HPP

#include "all.hpp"

class CLoader
{
	public:

		CLoader() { ; } ;

		virtual void packet_1001(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content) = 0;
		virtual void packet_1002(const unsigned real_sequence, const unsigned sequence, const QMap<QString, QVariant> content) = 0;
		virtual void packet_TODO(const unsigned id) = 0;
		virtual void packet_unknown(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload) = 0;
		virtual void bad_packet(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * payload) = 0;
};

void process(const QString src_fname, CLoader * loader);

#endif

