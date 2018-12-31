#ifndef FRAMERBUFFER_H
#define FRAMERBUFFER_H
#include <GL/glew.h>


class FrameBuffer {
private:
	GLuint frameBuffer;

	GLuint rbo;
	bool valid;
	GLuint quadVBO;
	GLuint quadVAO;

	int width;
	int height;

	GLuint colorBuffer;

public:
	FrameBuffer();
	~FrameBuffer();
	void init(int screenWidth, int screenHeight);
	void begin();
	void end();
	void clear();
	void savetoImage(const char* imgpath);
	bool saved;
};

#endif
