#include "jpeg_ls.h"
#include "jpeg_ls_decoder.h"
int main(int argc, char const *argv[])
{
    cv::Mat image;
    std::vector<std::vector<int>> encoded;
    image = cv::imread("lena.png", cv::IMREAD_GRAYSCALE); // Read the file
    if (image.depth() != CV_8U){
        std::cout << "wrong type!\n";
        return 0;
    }
    std::cout << "num: " << image.cols * image.rows << "\n";
	jpeg_ls encoder = jpeg_ls(&image);
	encoded = encoder.encode_frame();
    std::cout << "(q, r)\n";
    std::cout << "size: " << encoded.size() << "\n";
    std::cout << "inner size: " << encoded[0].size() << "\n";
    jpeg_ls_decoder jd(512, 512);
    cv::Mat deco = jd.decode(encoded);
    cv::imshow("img",deco);
    for (int i = 0; i < image.cols * image.rows; ++i) {
        if(*(image.data + i) != *(deco.data + i) ){
            std::cout << "Diferent at i: " << i << "\n";
            std::cout << "original: " << (int)*(image.data + i) << " decoded: " << (int)*(deco.data + i) << "\n";
        }
    }
    cv::waitKey(0);
    /*
    for (auto &i : encoded) {
        std::cout << "(" << i[0] << ", " << i[1] << ")\n";
    }
    */
    return 0;
}