#pragma once
#include "math.h"
#include "opengl.h"
#include "ShaderPrg.h"
#include "ShaderManger.h"
#include "Arcball.h"

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
		int m_width, m_height;
		GLFWwindow* mWindow;
		bool isRunning = false;

		void initWindow(bool invisibleWindow);
		void initRenderData(void);
		void initShaders(void);
		void initDefaultObjects(void);

		void update(void);
		void updateObjects(void);
		void updateMeshData(void);

		void draw(void);
		void drawObjects(void);
		void drawDefaultObjects(void);
		void drawBackground(void);

	public:
		char* GLRenderHandle;
		const int GetWidth(void);
		const int GetHeight(void);
		void Init(bool offscreen);
		void DeInit(void);
		void Resize(int width, int height);
		void Stop(void);
		void Mainloop(char* imgBuffer);
		void AddMesh(const char* filename);
		void MouseMove(const int x, const int y);
		void MouseDown(const int x, const int y, const int button);
		void MouseUp(void);
		void MouseScroll(const int delta);

		GLFWwindow* GetWindow(void);
	};
}

