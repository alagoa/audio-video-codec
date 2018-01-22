#include "jpeg_ls.h"
#include "jpeg_ls_decoder.h"
#include "Structures.h"
int main(int argc, char const *argv[])
{
    cv::Mat image;
    encoded_t encoded;
    image = cv::imread("lenas.png", cv::IMREAD_GRAYSCALE); // Read the file
    if (image.depth() != CV_8U){
        std::cout << "wrong type!\n";
        return 0;
    }
    std::cout << "Original size: " << image.cols * image.rows << " bytes\n";
	jpeg_ls encoder = jpeg_ls(&image);
	encoded = encoder.encode_frame();
    jpeg_ls_decoder jd(image.cols, image.rows);
    cv::Mat deco = jd.decode(encoded);
    cv::imshow("img",deco);
    /*
    for (int i = 0; i < image.cols * image.rows; ++i) {
        if(*(image.data + i) != *(deco.data + i) ){
            std::cout << "Diferent at i: " << i << "\n";
            std::cout << "original: " << (int)*(image.data + i) << " decoded: " << (int)*(deco.data + i) << "\n";
        }
    }
     */
    cv::waitKey(0);
    cv::imwrite("decoded.png", deco);
    /*
    for (auto &i : encoded) {
        std::cout << "(" << i[0] << ", " << i[1] << ")\n";
    }
    */
    return 0;
}