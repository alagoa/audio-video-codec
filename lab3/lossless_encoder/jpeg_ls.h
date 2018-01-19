#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstring>
#include "Structures.h"
#define MAXVAL 255
#define T1 BASIC_T1
#define T2 BASIC_T2
#define T3 BASIC_T3

class jpeg_ls
{
public:
    explicit jpeg_ls(cv::Mat *frame);
	~jpeg_ls();
	void next_sample();
	void encode_sample();
	void update_ctx(int q, int errval);
	int new_line();
    void error_coding(int residual, int k);
    std::vector<std::vector<int>> encode_frame();
private:
	int range;
	//const int near;
	cv::Mat* frame;
    int index;
    int row;
    int x_size;
    int y_size;
    int eof;
    uchar* last_line;
    uchar* current_line;
    sample_t current_sample;
    std::vector<std::vector<int>> error_codes;
	int bpp;
	int qbpp;
	int N[367];
	int A[367];
	int B[365];
	int C[365];	
};