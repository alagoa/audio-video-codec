#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>

class VideoFile {

public:
	VideoFile();
	VideoFile(std::string filename);
	int next_frame(int loop);
	cv::Mat get_frame();
	double getFPS();
	void setFPS(double fps);

private:
	std::ifstream file;					// video file
	std::string header;				// header of the video file
	int chroma;						// chroma subsampling (4:4:4, 4:2:2 or 4:2:0)
	int yCols, yRows;				// frame dimension
	int fps;						// frames per second
	unsigned char *imgData;			// file data buffer
	uchar *buffer;					// unsigned char pointer to Mat data

	cv::Mat img;						// data structure for the OpenCv image
};