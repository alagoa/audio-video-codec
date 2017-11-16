#include <vector>
#define BUFFER_LEN 1024

typedef std::vector< short > channel_data_t;
typedef std::vector< channel_data_t > audio_data_t;
typedef std::vector<std::vector<std::pair<int,int>>> encoded_data;