#ifndef LOSSLESS_ENCODER_s
#define LOSSLESS_ENCODER_s
#define BASIC_T1 3
#define BASIC_T2 7
#define BASIC_T3 21
#define MAX_C 127
#define MIN_C (-128)
#define NEAR 0
#define RESET 64
#define MAPPING(q) (((q)[0] * 9 + (q)[1]) * 9 + (q)[2])

#include <vector>
typedef std::vector<uchar> g_pair_t;
typedef std::vector<g_pair_t> encoded_t;
typedef struct{
    int a;
    int b;
    int c;
    int d;
    int x;
} sample_t;

#endif //LOSSLESS_ENCODER_s