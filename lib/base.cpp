
#include "lib/base.hpp"

CLoader * __loader;

void process_main(const vector<uint8_t> & packet);
void process_0921(const uint8_t * ptr_payload);
void process_1001(const uint8_t * ptr_payload);
void process_1002(const uint8_t * ptr_payload);
void process_1003(const uint8_t * ptr_payload);
void process_1004(const uint8_t * ptr_payload);
void process_1009(const uint8_t * ptr_payload);
void process_unknown(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * ptr_payload);

void process(const QString src_fname, CLoader * loader)
{
	QFile src_fl(src_fname);
	const unsigned size = src_fl.size();
	uint8_t * buf;
	vector<uint8_t> data;
	const vector<uint8_t> begin = { 0x55, 0xAA, 0x55, 0xAA };
	const vector<uint8_t> end = { 0x66, 0xCC, 0x66, 0xCC };
	int heartbeat_counter, heartbeat_thresh, v;
	bool is_first = true;

	throw_if(! src_fl.open(QIODevice::ReadOnly));
	throw_null(__loader = loader);

	throw_null(buf = src_fl.map(0, size));

	v = 0x80;
	heartbeat_counter = 0;
	heartbeat_thresh = 0;

	printf("Begin...\n");

	while(v < size)
	{
		if(heartbeat_counter >= heartbeat_thresh)
		{
			uint16_t byte_0, byte_1, check;

			if(is_first)
				is_first = false;
			else
				v += 8 - heartbeat_thresh;
			
			while(v < size)
			{
				byte_0 = buf[v];
				byte_1 = buf[v + 1];
				check = (((byte_1 << 8) | byte_0) >> 4) & 0xFF;

				v += 4;

				if(check == 0x90)
					break;

				v += 8;
			}

			if(v >= size)
				break;
			
			heartbeat_counter = 0;
			heartbeat_thresh = byte_0 & 0xF;
		}

		data.push_back(buf[v]);
		heartbeat_counter++;
		v++;

		auto from = find_end(data.begin(), data.end(), begin.begin(), begin.end());
		auto to = find_end(data.begin(), data.end(), end.begin(), end.end());

		if(from != data.end() && to != data.end() && from < to)
		{
			vector<uint8_t> packet;
			
			packet.resize(to - from);
			copy(from, to, packet.begin());
			process_main(packet);
			data.clear();
		}
	}

	printf("End...\n");
}

void process_main(const vector<uint8_t> & packet)
{
	const unsigned size = packet.size();

	if(size < 8)
		return;

	const uint8_t * ptr = packet.data();
	const uint8_t * ptr_payload = ptr + 8;
	const int __payload_size = size - 8;
	const unsigned id = * (uint16_t *) (ptr + 4);
	const unsigned payload_size = * (uint16_t *) (ptr + 6);

#define PROCESS(t_id, t_payload_size, __process)\
	case t_id:\
	{\
		if(payload_size == t_payload_size && t_payload_size <= __payload_size)\
			__process(ptr_payload);\
		else\
			__loader->bad_packet(t_id, t_payload_size, __payload_size, ptr_payload);\
		\
		break;\
	}

	switch(id)
	{
		PROCESS(0x0921, 0x0C, process_0921);
		PROCESS(0x1001, 0x0E, process_1001);
		PROCESS(0x1002, 0x78, process_1002);
		PROCESS(0x1003, 0x3A, process_1003);
		PROCESS(0x1004, 0x06, process_1004);
		PROCESS(0x1009, 0xB8, process_1009);

		default:
		{
			process_unknown(id, payload_size, __payload_size, ptr_payload);

			break;
		}
	}
}

void process_0921(const uint8_t * ptr_payload)
{
	__loader->packet_TODO(0x0921);
}

void process_1001(const uint8_t * ptr_payload)
{
	QMap<QString, QVariant> content;
	static unsigned real_sequence = 0;

	real_sequence++;

	const unsigned gyroscope_X_raw = ((uint32_t *) ptr_payload)[0];					// x100, градусы/сек.
	const unsigned gyroscope_Y_raw = ((uint32_t *) ptr_payload)[1];					// x100, градусы/сек.
	const unsigned gyroscope_Z_raw = ((uint32_t *) ptr_payload)[2];					// x100, градусы/сек.
	const unsigned sequence = ((uint16_t *) (ptr_payload + 12))[0];

	content["gyroscope_X_raw"] = QVariant(gyroscope_X_raw);
	content["gyroscope_Y_raw"] = QVariant(gyroscope_Y_raw);
	content["gyroscope_Z_raw"] = QVariant(gyroscope_Z_raw);

	__loader->packet_1001(real_sequence, sequence, content);
}

void process_1002(const uint8_t * ptr_payload)
{
	// Каждые 10 миллисекунд

	QMap<QString, QVariant> content;
	static unsigned real_sequence = 0;

	real_sequence++;

	const double longitude = ((double *) ptr_payload)[0];							// радианы
	const double latitude = ((double *) ptr_payload)[1];							// радианы
	const double altitude = ((float *) (ptr_payload + 16))[0];						// метры
	const double accelerometer_X = ((float *) (ptr_payload + 16))[1];				// g
	const double accelerometer_Y = ((float *) (ptr_payload + 16))[2];				// g
	const double accelerometer_Z = ((float *) (ptr_payload + 16))[3];				// g
	const double gyroscope_X = ((float *) (ptr_payload + 16))[4];					// радианы / сек.
	const double gyroscope_Y = ((float *) (ptr_payload + 16))[5];					// радианы / сек.
	const double gyroscope_Z = ((float *) (ptr_payload + 16))[6];					// радианы / сек.
	const double altitude_barometric = ((float *) (ptr_payload + 16))[7];			// метры
	// 57 - 84
	const double averaged_northward_velocity = ((float *) (ptr_payload + 76))[0];	// метры / сек.
	const double averaged_eastward_velocity = ((float *) (ptr_payload + 76))[1];	// метры / сек.
	const double downward_velocity = ((float *) (ptr_payload + 76))[2];				// метры / сек.
	// 97 - 108
	const int calibrated_magnetometer_X = ((int16_t *) (ptr_payload + 100))[0];
	const int calibrated_magnetometer_Y = ((int16_t *) (ptr_payload + 100))[1];
	const int calibrated_magnetometer_Z = ((int16_t *) (ptr_payload + 100))[2];
	// 115 - 124
	const unsigned number_of_satellites = ((uint8_t *) (ptr_payload + 116))[0];
	// 126
	const unsigned sequence = ((uint16_t *) (ptr_payload + 118))[0];

	content["longitude"] = QVariant(longitude);
	content["latitude"] = QVariant(latitude);
	content["altitude"] = QVariant(altitude);
	content["accelerometer_X"] = QVariant(accelerometer_X);
	content["accelerometer_Y"] = QVariant(accelerometer_Y);
	content["accelerometer_Z"] = QVariant(accelerometer_Z);
	content["gyroscope_X"] = QVariant(gyroscope_X);
	content["gyroscope_Y"] = QVariant(gyroscope_Y);
	content["gyroscope_Z"] = QVariant(gyroscope_Z);
	content["altitude_barometric"] = QVariant(altitude_barometric);
	content["averaged_northward_velocity"] = QVariant(averaged_northward_velocity);
	content["averaged_eastward_velocity"] = QVariant(averaged_eastward_velocity);
	content["downward_velocity"] = QVariant(downward_velocity);
	content["calibrated_magnetometer_X"] = QVariant(calibrated_magnetometer_X);
	content["calibrated_magnetometer_Y"] = QVariant(calibrated_magnetometer_Y);
	content["calibrated_magnetometer_Z"] = QVariant(calibrated_magnetometer_Z);
	content["number_of_satellites"] = QVariant(number_of_satellites);

	__loader->packet_1002(real_sequence, sequence, content);
}

void process_1003(const uint8_t * ptr_payload)
{
	__loader->packet_TODO(0x1003);
}

void process_1004(const uint8_t * ptr_payload)
{
	__loader->packet_TODO(0x1004);
}

void process_1009(const uint8_t * ptr_payload)
{
	__loader->packet_TODO(0x1009);
}

void process_unknown(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * ptr_payload)
{
	__loader->packet_unknown(id, payload_size, real_payload_size, ptr_payload);
}

