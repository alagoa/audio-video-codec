#include "VideoFile.h"

class Player {

public:

	Player();
	Player(std::string input_file, double fps, int wait, int loop);
	void load_file(std::string input_file);
	void play();
	void pause();
	void setFPS(double fps);
	void loop();
private:
	VideoFile video_file;
	int end;				/* control variable */
	int playing; 			/* control variable */
	int looping; 			/* control variable */
	char inputKey; 	/* control variable */

	int fps;
};