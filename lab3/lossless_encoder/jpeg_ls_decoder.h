#ifndef LOSSLESS_ENCODER_JPEG_LS_DECODER_H
#define LOSSLESS_ENCODER_JPEG_LS_DECODER_H
#include <opencv2/opencv.hpp>
#include "Structures.h"

#define T1 BASIC_T1
#define T2 BASIC_T2
#define T3 BASIC_T3

class jpeg_ls_decoder
{
public:
	jpeg_ls_decoder(int x, int y);
	~jpeg_ls_decoder();
    cv::Mat decode(encoded_t const &encoded);

private:
    void new_sample();
    //inline void new_line(std::vector<std::vector<int>> const &encoded);
    void update_bias(int ctx_idx);
    void update_ctx(int q, int errval);
    sample_t sample;
    int x,y;
    int range;
    int N[367];
    int A[367];
    int B[365];
    int C[365];
    uchar* last_line;
    int index;
};

#endif //LOSSLESS_ENCODER_JPEG_LS_DECODER_H
