
#include "convert/xml.hpp"

QXmlStreamWriter stream;

void process(const vector<uint8_t> & packet);
void process_0921(const uint8_t * ptr_payload);
void process_1001(const uint8_t * ptr_payload);
void process_1002(const uint8_t * ptr_payload);
void process_1003(const uint8_t * ptr_payload);
void process_1004(const uint8_t * ptr_payload);
void process_1009(const uint8_t * ptr_payload);
void process_unknown(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * ptr_payload);

void convert_to_xml(const QString src_fname, const QString dst_fname)
{
	QFile src_fl(src_fname), dst_fl(dst_fname);
	const unsigned size = src_fl.size();
	uint8_t * buf;
	vector<uint8_t> data;
	const vector<uint8_t> begin = { 0x55, 0xAA, 0x55, 0xAA };
	const vector<uint8_t> end = { 0x66, 0xCC, 0x66, 0xCC };
	int heartbeat_counter, heartbeat_thresh, v;
	bool is_first = true;

	throw_if(! src_fl.open(QIODevice::ReadOnly));
	throw_if(! dst_fl.open(QIODevice::WriteOnly));

	throw_null(buf = src_fl.map(0, size));

	stream.setDevice(& dst_fl);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();
	stream.writeStartElement("naza");

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
			process(packet);
			data.clear();
		}
	}

	printf("End...\n");

	stream.writeEndElement();
	stream.writeEndDocument();
}

void process(const vector<uint8_t> & packet)
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
			printf_TODO("Bad packet with id = 0x%X, payload size = 0x%X and real payload size = 0x%X\n", t_id, t_payload_size, __payload_size);\
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
	printf_TODO("id = 0x0921\n"); // Не приходит
}

void process_1001(const uint8_t * ptr_payload)
{
	static unsigned real_sequence = 0;

	real_sequence++;

	const unsigned gyroscope_X_raw = ((uint32_t *) ptr_payload)[0];					// x100, градусы/сек.
	const unsigned gyroscope_Y_raw = ((uint32_t *) ptr_payload)[1];					// x100, градусы/сек.
	const unsigned gyroscope_Z_raw = ((uint32_t *) ptr_payload)[2];					// x100, градусы/сек.
	const unsigned sequence = ((uint16_t *) (ptr_payload + 12))[0];

	stream.writeStartElement("packet");
	stream.writeAttribute("id", "0x1001");
	stream.writeTextElement("gyroscope_X_raw", QString::number(gyroscope_X_raw));
	stream.writeTextElement("gyroscope_Y_raw", QString::number(gyroscope_Y_raw));
	stream.writeTextElement("gyroscope_Z_raw", QString::number(gyroscope_Z_raw));
	stream.writeTextElement("sequence", QString::number(sequence));
	stream.writeEndElement();
}

void process_1002(const uint8_t * ptr_payload)
{
	// Каждые 10 миллисекунд

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

	stream.writeStartElement("packet");
	stream.writeAttribute("id", "0x1002");
	stream.writeTextElement("longitude", QString::number(longitude, 'g', 11));
	stream.writeTextElement("latitude", QString::number(latitude, 'g', 11));
	stream.writeTextElement("altitude", QString::number(altitude, 'g', 11));
	stream.writeTextElement("accelerometer_X", QString::number(accelerometer_X, 'g', 11));
	stream.writeTextElement("accelerometer_Y", QString::number(accelerometer_Y, 'g', 11));
	stream.writeTextElement("accelerometer_Z", QString::number(accelerometer_Z, 'g', 11));
	stream.writeTextElement("gyroscope_X", QString::number(gyroscope_X, 'g', 11));
	stream.writeTextElement("gyroscope_Y", QString::number(gyroscope_Y, 'g', 11));
	stream.writeTextElement("gyroscope_Z", QString::number(gyroscope_Z, 'g', 11));
	stream.writeTextElement("altitude_barometric", QString::number(altitude_barometric, 'g', 11));
	stream.writeTextElement("averaged_northward_velocity", QString::number(averaged_northward_velocity, 'g', 11));
	stream.writeTextElement("averaged_eastward_velocity", QString::number(averaged_eastward_velocity, 'g', 11));
	stream.writeTextElement("downward_velocity", QString::number(downward_velocity, 'g', 11));
	stream.writeTextElement("calibrated_magnetometer_X", QString::number(calibrated_magnetometer_X));
	stream.writeTextElement("calibrated_magnetometer_Y", QString::number(calibrated_magnetometer_Y));
	stream.writeTextElement("calibrated_magnetometer_Z", QString::number(calibrated_magnetometer_Z));
	stream.writeTextElement("number_of_satellites", QString::number(number_of_satellites));
	stream.writeTextElement("sequence", QString::number(sequence));
	stream.writeEndElement();
}

void process_1003(const uint8_t * ptr_payload)
{
	printf_TODO("id = 0x1003\n"); // Не приходит
}

void process_1004(const uint8_t * ptr_payload)
{
	printf_TODO("id = 0x1004\n"); // Не приходит
}

void process_1009(const uint8_t * ptr_payload)
{
	printf_TODO("id = 0x1009\n"); // Не приходит
}

void process_unknown(const unsigned id, const unsigned payload_size, const unsigned real_payload_size, const uint8_t * ptr_payload)
{
	printf_TODO("Unknown packet with id = 0x%X, payload_size = 0x%X and real_payload_size = 0x%X\n", id, payload_size, real_payload_size);
}

