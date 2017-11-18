#include <vector>
#define BUFFER_LEN 16384

typedef std::vector< short > channel_data_t;
typedef std::vector< channel_data_t > audio_data_t;
typedef std::vector< std::pair<int,int> > encoded_channel_t;
typedef std::vector< encoded_channel_t > encoded_data_t;