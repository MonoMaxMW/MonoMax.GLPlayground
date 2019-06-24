#pragma once
#include "common.h"
#include <iostream>
#include <ostream>
#include <fstream>
#include <functional>

namespace MonoMaxGraphics
{

	class GraphicsEngine
	{

	private:
		glm::mat4 mProjMat, mViewMat, mModlMat;

		int m_bufferLength;
		GLuint m_vao, m_vbo;
		GLuint m_prg;
		GLuint m_rbo, m_fbo;
		int m_width, m_height;
		GLFWwindow* m_window;
		bool isRunning = false;

		std::string getShaderCode(const char* filename);
		void addShader(GLuint prgId, const std::string shadercode, GLenum shadertype);

		void initWindow(void);
		void initRenderData(void);
		void initShaders(void);
		std::function<void(void)> m_rendercallback;

	public:

		char* GLRenderHandle;
		const int GetWidth(void);
		const int GetHeight(void);
		const int GetBufferLenght(void);
		void Init(void);
		void DeInit(void);
		void Resize(int width, int height);
		void Stop(void);
		void Render(char* imgBuffer);
	};
}

