#include "VideoFile.h"

VideoFile::VideoFile() {
}

VideoFile::VideoFile(std::string filename)
{
	file.open(filename);
	/* Get header info from header */								
	std::getline(file, header);
	yCols = stoi(header.substr(header.find(" W") + 2, header.find(" H") - header.find(" W") - 2));
	yRows = stoi(header.substr(header.find(" H") + 2, header.find(" F") - header.find(" H") - 2));
    try {
    	chroma = stoi(header.substr(header.find(" C") + 2, header.length()));
    }
    catch(std::invalid_argument) {
    	std::cout << "Chroma data not found: using 4:2:0";
        chroma = 420;
    }

    /* OpenCv Image */
    img = cv::Mat(cv::Size(yCols, yRows), CV_8UC3);

    /* Buffer to store the frame */
    imgData = new unsigned char[yCols * yRows * 3];
}

double VideoFile::getFPS() {
	return fps;
}

void VideoFile::setFPS(double fps) {
	this->fps = fps;
}

cv::Mat VideoFile::get_frame() {
	return img;
}

int VideoFile::next_frame(int loop) {

	int i, r, g, b, y, u, v; /* auxiliary variables */

	/* load a new frame, if possible */
	getline (file, header); // Skipping word FRAME

	/* Check if we hit the end of the file */
	file.read((char *)imgData, yCols * yRows * 3);
	if(file.gcount() == 0)
	{
		if(loop)
		{
			file.clear();
			file.seekg(0);
			getline (file,header); // read the header
			return 1;
		}
		else
		{
			return 0;
		}
	}

	/* The video is stored in YUV planar mode but OpenCv uses packed modes*/

	buffer = (uchar*)img.ptr();
	switch(chroma) {
		case 444:
			for(i = 0 ; i < yRows * yCols * 3 ; i += 3)
        	{
                /* Accessing to planar info */
                y = imgData[i / 3];
                u = imgData[(i / 3) + (yRows * yCols)];
                v = imgData[(i / 3) + (yRows * yCols) * 2];

                /* convert to RGB */
                b = (int)(1.164*(y - 16) + 2.018*(u-128));
                g = (int)(1.164*(y - 16) - 0.813*(u-128) - 0.391*(v-128));
                r = (int)(1.164*(y - 16) + 1.596*(v-128));

                /* clipping to [0 ... 255] */
                if(r < 0) r = 0;
                if(g < 0) g = 0;
                if(b < 0) b = 0;
                if(r > 255) r = 255;
                if(g > 255) g = 255;
                if(b > 255) b = 255;

                /* if you need the inverse formulas */
                //y = r *  .299 + g *  .587 + b *  .114 ;
                //u = r * -.169 + g * -.332 + b *  .500  + 128.;
                //v = r *  .500 + g * -.419 + b * -.0813 + 128.;

                /* Fill the OpenCV buffer - packed mode: BGRBGR...BGR */
                buffer[i] = b;
                buffer[i + 1] = g;
                buffer[i + 2] = r;
       		}
       		break;
       	case 422:
       		for(i = 0; i < yRows * yCols; i++)
        	{
                /* Accessing to planar info */
                y = imgData[i];
                u = imgData[(yRows * yCols) + (i/2)];
                v = imgData[(yRows * yCols)*3/2 + (i/2)];

                /* convert to RGB*/
                r = (int)(y + 1.28033*(v-128));
                g = (int)(y - 0.21482*(u-128) - 0.38059*(v-128));
                b = (int)(y + 2.12798*(u-128));

                //r = y + 1.28033*v;
                //g = y - 0.21482*u - 0.38059*v;
                //b = y + 2.12798*u;

                /* clipping to [0 ... 255] */
                if(r < 0) r = 0;
                if(g < 0) g = 0;
                if(b < 0) b = 0;
                if(r > 255) r = 255;
                if(g > 255) g = 255;
                if(b > 255) b = 255;

                /* if you need the inverse formulas */
                //y = r *  .299 + g *  .587 + b *  .114 ;
                //u = r * -.169 + g * -.332 + b *  .500  + 128.;
                //v = r *  .500 + g * -.419 + b * -.0813 + 128.;

                /* Fill the OpenCV buffer - packed mode: BGRBGR...BGR */
                buffer[i*3] = b;
                buffer[i*3 + 1] = g;
                buffer[i*3 + 2] = r;
        	}
        	break;
        case 420:

            for(i = 0; i < yRows * yCols; i++)
        	{
                /* Accessing to planar info */
                y = imgData[i];
                int nRow = i/yCols/2;
                u = imgData[i/2%yCols + ((nRow-(nRow%2))/2)*yCols + (yRows * yCols)];
                v = imgData[i/2%yCols + ((nRow-(nRow%2))/2)*yCols + (yRows * yCols)*5/4];

                /* convert to RGB*/
                r = (int)(y + 1.28033*(v-128));
                g = (int)(y - 0.21482*(u-128) - 0.38059*(v-128));
                b = (int)(y + 2.12798*(u-128));

                //r = y + 1.28033*v;
                //g = y - 0.21482*u - 0.38059*v;
                //b = y + 2.12798*u;

                /* clipping to [0 ... 255] */
                if(r < 0) r = 0;
                if(g < 0) g = 0;
                if(b < 0) b = 0;
                if(r > 255) r = 255;
                if(g > 255) g = 255;
                if(b > 255) b = 255;

                /* if you need the inverse formulas */
                //y = r *  .299 + g *  .587 + b *  .114 ;
                //u = r * -.169 + g * -.332 + b *  .500  + 128.;
                //v = r *  .500 + g * -.419 + b * -.0813 + 128.;

                /* Fill the OpenCV buffer - packed mode: BGRBGR...BGR */
                buffer[i*3] = b;
                buffer[i*3 + 1] = g;
                buffer[i*3 + 2] = r;
       		}
       		break;
       	default:

       		break;       
    }

    return 1;
}