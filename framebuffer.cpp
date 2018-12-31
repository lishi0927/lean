#include "frameBuffer.h"
#include <iostream>
#include "texture.h"

FrameBuffer::FrameBuffer() {
	valid = false;
	saved = false;
}

FrameBuffer::~FrameBuffer() {
	if (valid) {
		glDeleteBuffers(1, &frameBuffer);
		glDeleteBuffers(1, &quadVBO);
		glDeleteBuffers(1, &quadVAO);
	}
}

void FrameBuffer::init(int screenWidth, int screenHeight) {
	if (valid)
		return;
	width = screenWidth;
	height = screenHeight;

	GLint maxRenderbufferSize;
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	//	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);



	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("ERROR:framebuffer is not complete,%d\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		exit(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	valid = true;
}

void FrameBuffer::begin() {
	if (valid)
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	else {
		std::cout << "ERROR: frame buffer has not been initialized.\n";
	}
}

void FrameBuffer::end() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::clear() {
	begin();
	glClearDepth(1.0f);
	glClearColor(0.46f, 0.46f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	end();
}

void FrameBuffer::savetoImage(const char* imgpath)
{
	unsigned char* data = new unsigned char[width * height * 4];
	begin();
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void*)data);
	end();
	for (int i = 0; i<(int)width * height * 4; i += 4)
	{

		data[i] ^= data[i + 2] ^= data[i] ^= data[i + 2];
	}
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32, 8, 8, 8);

	for (int y = 0; y<FreeImage_GetHeight(bitmap); y++)
	{
		BYTE *bits = FreeImage_GetScanLine(bitmap, y);
		for (int x = 0; x<FreeImage_GetWidth(bitmap); x++)
		{
			bits[0] = data[(y * width + x) * 4 + 0];
			bits[1] = data[(y * width + x) * 4 + 1];
			bits[2] = data[(y * width + x) * 4 + 2];
			bits[3] = 255;
			bits += 4;

		}

	}
	bool bSuccess = FreeImage_Save(FIF_PNG, bitmap, imgpath, PNG_DEFAULT);
	FreeImage_Unload(bitmap);
}

