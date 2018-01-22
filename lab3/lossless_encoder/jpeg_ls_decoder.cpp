//
// Created by gonccalo on 19-01-2018.
//

#include "jpeg_ls_decoder.h"
#define MAXVAL 255
jpeg_ls_decoder::jpeg_ls_decoder(int x, int y) : x(x), y(y){
    range = MAXVAL + 1;
    index = 1;
    int tmp;
    tmp = MAX(2, (range + 32) / 64);
    for (int i = 0; i < 367; ++i)
    {
        A[i] = tmp;
        N[i] = 1;
    }
    for (int i = 0; i < 365; ++i)
    {
        B[i] = 0;
        C[i] = 0;
    }
    last_line = (uchar*)std::calloc((x + 2), sizeof(uchar));
    if(last_line == NULL){
        std::cout << "ERROR malloc l_line\n";
    }
    sample = {0,0,0,0,0};
}

jpeg_ls_decoder::~jpeg_ls_decoder() {
    free(last_line);
}

cv::Mat jpeg_ls_decoder::decode(encoded_t const &encoded) {
    cv::Mat im(y, x, CV_8U);
    for (int p = 0; p < encoded.size(); ++p) {
        int Q[3];
        int sign;
        int ctx_id;
        int Px;
        int pix;
        int k;
        Q[0] = lut_grad[sample.d - sample.b];
        Q[1] = lut_grad[sample.b - sample.c];
        Q[2] = lut_grad[sample.c - sample.a];

        /*
        for (int i = 0; i < 3; ++i){
            if (D[i] <= -T3)
                Q[i] = -4;
            else if (D[i] <= -T2)
                Q[i] = -3;
            else if (D[i] <= -T1)
                Q[i] = -2;
            else if (D[i] < -NEAR)
                Q[i] = -1;
            else if (D[i] <= NEAR)
                Q[i] =  0;
            else if (D[i] < T1)
                Q[i] =  1;
            else if (D[i] < T2)
                Q[i] =  2;
            else if (D[i] < T3)
                Q[i] =  3;
            else
                Q[i] =  4;
        }
        */
        ctx_id = MAPPING(Q);
        sign = ctx_id >> (INT_SIZE - 1);
        ctx_id = (ctx_id ^ sign) - sign;
        /*
        if (ctx_id < 0){
            sign = -1;
            ctx_id = -ctx_id;
        }
        */

        int max_a_b = MAX(sample.a, sample.b);
        int min_a_b = MIN(sample.a, sample.b);
        if (sample.c >= max_a_b) {
            Px = min_a_b;
        }
        else{
            if (sample.c <= min_a_b)
                Px = max_a_b;
            else
                Px = sample.a + sample.b - sample.c;
        }

        if (sign == 0)
            Px += C[ctx_id];
        else
            Px -= C[ctx_id];

        if (Px > MAXVAL)
            Px = MAXVAL;
        else if (Px < 0)
            Px = 0;

        for (k = 0; (N[ctx_id] << k) < A[ctx_id]; ++k);

        int q,r;
        q = encoded[p][0];
        r = encoded[p][1];

        int m_errval = (q << k) | r;

        int err;
        if ((k == 0) && (2 * B[ctx_id] <= -N[ctx_id])){
            if((m_errval  & 0x01) == 0x01){ //impar
                err = (m_errval - 1) / 2;
            }
            else{
                err = (m_errval / -2) - 1;
            }
            //err = m_errval & 0x1 ? (m_errval - 1) / 2 : (m_errval / -2) - 1; //err = m_errval >= 0 ? 2 * m_errval + 1 : -2 * (m_errval + 1);
        }
        else {
            if ((m_errval & 0x01) == 0x01){
                err = (m_errval / -2) - 1;
            }
            else{
                err = m_errval / 2;
            }
            //err = m_errval & 0x1 ? (m_errval + 1) / -2 : m_errval / 2;
        }

        update_ctx(ctx_id, err);
        err = (err ^ sign) - sign;
        //if(sign == -1)
        //    err = -err;

        pix = (err + Px) % range; //real value!

        if (pix < -NEAR)
            pix += range; //*(2*NEAR + 1);
        else if(pix > MAXVAL + NEAR)
            pix -= range; //*(2*NEAR + 1);

        if (pix < 0)
            pix = 0;
        else if (pix > MAXVAL)
            pix = MAXVAL;

        update_bias(ctx_id);
        sample.x = pix;
        im.data[p] = ((uchar)pix);
        new_sample();
    }
    return im;
}


void jpeg_ls_decoder::update_bias(int ctx_idx){
    if (B[ctx_idx] <= -N[ctx_idx]){
        B[ctx_idx] += N[ctx_idx];
        if (C[ctx_idx] > MIN_C)
            C[ctx_idx] -= 1;
        if (B[ctx_idx] <= -N[ctx_idx])
            B[ctx_idx] = -N[ctx_idx] + 1;
    }
    else if (B[ctx_idx] > 0){
        B[ctx_idx] -= N[ctx_idx];
        if (C[ctx_idx] < MAX_C)
            C[ctx_idx] += 1;
        if (B[ctx_idx] > 0)
            B[ctx_idx] = 0;
    }
}

void jpeg_ls_decoder::update_ctx(int q, int errval){
    B[q] += errval * (2 * NEAR + 1);
    A[q] += abs(errval);
    if (N[q] == RESET){
        A[q] >>= 1;
        if (B[q] >= 0)
            B[q] >>= 1;
        else
            B[q] = -((1 - B[q]) >> 1);
        N[q] >>= 1;
    }
    ++N[q];
}

void jpeg_ls_decoder::new_sample(){
    if (index >= x){
        sample.c = last_line[0];
        last_line[0] = last_line[1];
        sample.a = last_line[0];
        sample.b = last_line[1];
        sample.d = last_line[2];
        last_line[index] = (uchar)sample.x;
        last_line[index + 1] = (uchar)sample.x;
        index = 1;
        return;
    }
    sample.a = sample.x;
    sample.c = sample.b;
    sample.b = sample.d;

    sample.d = last_line[index + 2]; //i+2?
    last_line[index] = (uchar)sample.x;
    index++;
}
