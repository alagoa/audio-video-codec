#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
#define BUFFER_LEN 1024

#define BLOCK_SIZE 2048
//#define BLOCK_SIZE 1024

typedef std::vector< int > channel_data_t;
typedef std::vector< channel_data_t > audio_data_t;
typedef std::vector< std::pair<uint,uint> > encoded_channel_t;
typedef std::vector< encoded_channel_t > encoded_data_t;
typedef unsigned char uchar;

typedef struct block_header{
	short order;
	short m;
	ushort size;
}block_header;

typedef std::vector<std::vector< block_header >> block_data_t;

#endif