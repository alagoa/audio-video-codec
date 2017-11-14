#include <sndfile.h>
#include <utility>
#include <string>
#include <stdio.h>

#define BUFFER_LEN 1024
typedef int (*reader_func)(SNDFILE*, void*, sf_count_t);
typedef std::vector< std::vector<short> > audio_data_t;
class AudioReader
{
public:
	AudioReader();
	AudioReader(std::string filename);
	~AudioReader();
	void close();
	audio_data_t get_values();
	int read(void* data_ptr);
private:
	void get_reader(int format);
	int read_short(SNDFILE* sndfile, void* data_ptr, sf_count_t items);
	int read_int(SNDFILE* sndfile, void* data_ptr, sf_count_t items);
	int read_float(SNDFILE* sndfile, void* data_ptr, sf_count_t items);
	int read_double(SNDFILE* sndfile, void* data_ptr, sf_count_t items);
	SF_INFO snd_info;
	SNDFILE* snd_file;
	reader_func reader_f;
};