#include "Player.h"

Player::Player() {
	end = 0;
	playing = 1;
	looping = 0;
	inputKey = '?';
}

Player::Player(std::string input_file, double fps, int wait, int loop) {
	video_file = VideoFile(input_file);
	video_file.setFPS(fps);
	if(wait)
		playing = 0;
	looping = loop;
	inputKey = '?';
}

void Player::load_file(std::string input_file) {
	video_file = VideoFile(input_file);
}

void Player::pause() {
	playing = 0;
}

void Player::setFPS(double fps) {
	video_file.setFPS(fps);
}

void Player::loop() {
	looping = 1;
}

void Player::play() {

	while(!end)
	{
		/* go to next frame if possible */
		if(!video_file.next_frame(looping)) {
			std::cout << "End of file reached, ending";
			break;
		}

		/* display the image */
		imshow( "rgb", video_file.get_frame() );

		if(playing)
		{
			/* wait according to the frame rate */
			inputKey = cv::waitKey(1.0 / video_file.getFPS() * 1000);
		}
		else
		{
			/* wait until user press a key */
			inputKey = cv::waitKey(0);
		}

		/* parse the pressed keys, if any */
		switch((char)inputKey)
		{
			case 'q':
				end = 1;
				break;

			case 'p':
				playing = playing ? 0 : 1;
				break;
		}
	}
}

