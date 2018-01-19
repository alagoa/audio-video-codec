#ifndef LOSSLESS_ENCODER_JPEG_LS_DECODER_H
#define LOSSLESS_ENCODER_JPEG_LS_DECODER_H
#include <opencv2/opencv.hpp>
#include <vector>
#include "Structures.h"

#define T1 BASIC_T1
#define T2 BASIC_T2
#define T3 BASIC_T3

class jpeg_ls_decoder
{
public:
	jpeg_ls_decoder(int x, int y);
	~jpeg_ls_decoder();
    cv::Mat decode(std::vector<std::vector<int>> const &encoded);

private:
    void new_sample(std::vector<std::vector<int>> const &encoded);
    //inline void new_line(std::vector<std::vector<int>> const &encoded);
    void update_bias(int ctx_idx);
    void update_ctx(int q, int errval);
    sample_t sample;
    int x,y;
    int eof;
    int range;
    int N[367];
    int A[367];
    int B[365];
    int C[365];
    uchar* last_line;
    //uchar* current_line;
    int index;
    int row;
    int col;
};


#endif //LOSSLESS_ENCODER_JPEG_LS_DECODER_H
