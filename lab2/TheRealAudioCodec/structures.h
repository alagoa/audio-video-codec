#include <vector>
#define BUFFER_LEN 1024

typedef std::vector< short > channel_data_t;
typedef std::vector< channel_data_t > audio_data_t;
typedef std::vector< std::pair<uint,uint> > encoded_channel_t;
typedef std::vector< encoded_channel_t > encoded_data_t;
typedef unsigned char uchar;