#pragma once
#include "math.h"
#include "opengl.h"
#include "ShaderPrg.h"
#include "ShaderManger.h"


namespace MonoMaxGraphics
{
	class GraphicsEngine
	{

	private:
		glm::mat4 mProjMat;
		glm::mat4 mViewMat;
		glm::mat4 mModlMat;
		std::vector<float> mVertices;
		std::vector<float> mNormals;
		bool mNeedsMeshDataUpdate;

		ShaderManger* mShaderManager;

		int mBufferLength;
		GLuint mVao, mVbo;
		int m_width, m_height;
		GLFWwindow* mWindow;
		bool isRunning = false;
		std::string getShaderCode(const char* filename);

		void initWindow(bool invisibleWindow);
		void initRenderData(void);
		void initShaders(void);
		void updateMeshData(void);

	public:

		char* GLRenderHandle;
		const int GetWidth(void);
		const int GetHeight(void);
		void Init(bool offscreen);
		void DeInit(void);
		void Resize(int width, int height);
		void Stop(void);
		void Render(char* imgBuffer);
		void AddMesh(const char* filename);
	};
}

