#include "jpeg_ls.h"

//#define MAX(a, b) ((a) > (b) ? (a) : (b))
//#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define K_MASK(k) ((2 << ((k)-1)) - 1)

jpeg_ls::jpeg_ls(cv::Mat *frame): frame(frame){
	range = MAXVAL + 1; //= ((MAXVAL + 2 * NEAR) / (2 * NEAR)) + 1;
	qbpp = std::log2(range);
    bpp =  MAX(2, std::log2(MAXVAL + 1));
	//limit?
	int tmp = MAX(2, (range + 32) / 64);
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

	x_size = frame->cols;
    y_size = frame->rows;
    row = 1;
    index = 1;
    current_sample = {0,0,0,0,0};
    current_line = (uchar*)std::malloc((x_size + 1) * sizeof(uchar));
    if(current_line == NULL){
        std::cout << "ERROR malloc c_line\n";
    }
    last_line = (uchar*)std::calloc((x_size + 2), sizeof(uchar));
    if(last_line == NULL){
        std::cout << "ERROR malloc l_line\n";
    }
    std::memcpy(current_line + 1, frame->ptr<uchar>(0), x_size * sizeof(uchar));
    current_line[0] = 0;
    error_codes.reserve(x_size * y_size);
    eof = 0;
}

jpeg_ls::~jpeg_ls() {
	free(current_line);
	free(last_line);
}

void jpeg_ls::error_coding(int residual, int k){
    int r;
    int q;
    int mask;
    mask = K_MASK(k);
    q = (residual & (~mask)) >> k;
    r = residual & mask;
    std::vector<int> tmp = std::vector<int>(2);
    tmp[0] = q;
    tmp[1] = r;
    error_codes.push_back(tmp);
}

void jpeg_ls::next_sample(){
	if (index > x_size) {
		if (row >= y_size) {
            eof = 1;
            return;
		}
		new_line();
		index = 1;
		row++;
	}
	current_sample.a = current_line[index - 1];
	current_sample.b = last_line[index];
	current_sample.c = last_line[index - 1];
	current_sample.d = last_line[index + 1];
	current_sample.x = current_line[index];
    std::cout << "sample: x: " << current_sample.x << "\n";
	index++;
}

int jpeg_ls::new_line(){
	std::memcpy(last_line + 1, current_line + 1, x_size * sizeof(uchar));
	last_line[x_size + 1] = last_line[x_size];
	last_line[0] = current_line[0];
	std::memcpy(current_line + 1, frame->ptr<uchar>(row), x_size * sizeof(uchar));
	current_line[0] = last_line[1];
    return 1;
}

void jpeg_ls::encode_sample(){
	int D[3];
    int Q[3];
    int sign;
    int m;
    int Px;
    int max_a_b;
    int min_a_b;
    int errval;
    D[0] = current_sample.d - current_sample.b;
	D[1] = current_sample.b - current_sample.c;
	D[2] = current_sample.c - current_sample.a;
	//check mode
	//if normal mode

	//quantize context

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
    m = MAPPING(Q);
    sign = 1;
    if (m < 0){
     sign = -1;
        m = -m;
    }

	//prediction
	max_a_b = MAX(current_sample.a, current_sample.b);
	min_a_b = MIN(current_sample.a, current_sample.b);
	if (current_sample.c >= max_a_b) {
        Px = min_a_b;
    }
 	else{
 		if (current_sample.c <= min_a_b)
 			Px = max_a_b;
 		else
 			Px = current_sample.a + current_sample.b - current_sample.c;
 	}

 	//prediction correction
 	if (sign == 1)
 		Px += C[m];
 	else
 		Px -= C[m];

 	if (Px > MAXVAL)
 		Px = MAXVAL;
 	else if (Px < 0)
 		Px = 0;

 	//prediction error
 	errval = current_sample.x - Px;
 	if (sign == -1)
 		errval = -errval;

 	/* FOR NEAR LOSSLESS
 	if (Errval > 0)
		Errval = (Errval + NEAR) / (2 * NEAR + 1);
	else
		Errval = – (NEAR – Errval) / (2 * NEAR + 1);
	Rx = Px + SIGN * Errval * (2 * NEAR + 1);
	if (Rx < 0)
		Rx = 0;
	else if (Rx > MAXVAL)
		Rx = MAXVAL;
 	*/

 	//modRange()
 	if (errval < 0)
 		errval += range;
 	if (errval >= (range + 1) / 2)
 		errval -= range;

 	int k;
 	for (k = 0; (N[m] << k) < A[m]; ++k);

 	int residual;
 	if ((k == 0) && ((2 * B[m]) <= -N[m])) {
        if (errval >= 0){
            residual = 2 * errval + 1;
        }
        else{
            residual = -2 * (errval + 1);
        }
        //residual = errval >= 0 ? 2 * errval + 1 : -2 * (errval + 1);
    }
 	else {
        if (errval >= 0){
            residual = 2 * errval;
        }
        else{
            residual = -2 * errval - 1;
        }
        //residual = errval >= 0 ? 2 * errval : -2 * errval - 1;
    }
    error_coding(residual, k);
    update_ctx(m, errval);
}

void jpeg_ls::update_ctx(int q, int errval){
	B[q] += errval * (2 * NEAR + 1);
	A[q] += abs(errval);
	if (N[q] == RESET){
		A[q] >>= 1;
		if (B[q] >=123456gg
            
		else
			B[q] = -((1 - B[q]) >> 1);
		N[q] >>= 1;
	}
	N[q]++;

	if (B[q] <= -N[q]){
		B[q] += N[q];
		if (C[q] > MIN_C)
			C[q] -= 1;
		if (B[q] <= -N[q])
			B[q] = -N[q] + 1;
	}
	else if (B[q] > 0){
		B[q] -= N[q];
		if (C[q] < MAX_C)
			C[q] += 1;
		if (B[q] > 0)
			B[q] = 0;
	}
}

std::vector<std::vector<int>> jpeg_ls::encode_frame() {
    while (eof == 0){
        next_sample();
        encode_sample();
    }
    return error_codes;
}
