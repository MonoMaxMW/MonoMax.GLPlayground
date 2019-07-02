#include "GraphicsEngine.h";
#include "io.h"
#include "common.h"
#include "ShaderPrg.h"


namespace MonoMaxGraphics
{
	enum MouseState
	{
		Release,
		Press
	};


	char* _imgBuffer;
	MouseState _lMs;
	MouseState _rMs;
	MouseState _mMs;

	Arcball* _arcball;
	GLuint beginIdx, meshCount;
	unsigned long beginCount = 0;
	glm::vec3 _arcballPos;
	GLuint vao, vboP, vboN;
	ShaderPrg* activePrg;

	std::string getShaderCode(const char* filename)
	{
		std::string shaderCode;
		std::ifstream file(filename, std::ios::in);

		if (!file.good())
		{
			throw new std::exception();
		}

		file.seekg(0, std::ios::end);
		shaderCode.resize((unsigned int)file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(&shaderCode[0], shaderCode.size());
		file.close();

		return shaderCode;
	}

	ShaderPrg* createSimpleColorPrg(void)
	{
		ShaderPrg* prg = new ShaderPrg();
		prg->Create("SimpleShader");

		std::string shaderCode[] =
		{
			getShaderCode("C:/DEV/shaders/simple_color_vs.glsl"),
			getShaderCode("C:/DEV/shaders/simple_color_fs.glsl")
		};
		prg->AddShader(GL_VERTEX_SHADER, shaderCode[0]);
		prg->AddShader(GL_FRAGMENT_SHADER, shaderCode[1]);
		prg->Compile();
		{
			prg->AddVar("aPos");
			prg->AddVar("uModlMat");
			prg->AddVar("uViewMat");
			prg->AddVar("uProjMat");
			prg->AddVar("uItemColor");
		}

		return prg;
	}

	ShaderPrg* createBasicLightPrg(void)
	{
		ShaderPrg* prg = new ShaderPrg();
		prg->Create("SimpleLight");

		std::string shaderCode[] =
		{
			getShaderCode("C:/DEV/shaders/basic_light_vs.glsl"),
			getShaderCode("C:/DEV/shaders/basic_light_fs.glsl")
		};
		prg->AddShader(GL_VERTEX_SHADER, shaderCode[0]);
		prg->AddShader(GL_FRAGMENT_SHADER, shaderCode[1]);
		prg->Compile();
		{
			prg->AddVar("aPos");
			prg->AddVar("aNormal");
			prg->AddVar("uModlMat");
			prg->AddVar("uViewMat");
			prg->AddVar("uProjMat");
			prg->AddVar("uItemColor");
			prg->AddVar("uViewPos");
			prg->AddVar("uLightPos");
		}

		return prg;
	}

	void GraphicsEngine::initShaders(void)
	{
		mShaderManager = new ShaderManger();
		mShaderManager->AddShaderPrg(createSimpleColorPrg());
		mShaderManager->AddShaderPrg(createBasicLightPrg());

		activePrg = mShaderManager->GetShaderPrg("SimpleLight");
	}

	void GraphicsEngine::initDefaultObjects(void)
	{

	}

	void GraphicsEngine::updateMeshData(void)
	{
		if (!mNeedsMeshDataUpdate || mVertices.size() == 0)
			return;

		meshCount = (GLuint)mVertices.size() / 3;

		glBindVertexArray(vao);
		{
			glBindBuffer(GL_ARRAY_BUFFER, vboP);
			glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), &mVertices.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(activePrg->GetVar("aPos"), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(activePrg->GetVar("aPos"));

			glBindBuffer(GL_ARRAY_BUFFER, vboN);
			glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(float), &mNormals.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(activePrg->GetVar("aNormal"), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(activePrg->GetVar("aNormal"));

		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		mNeedsMeshDataUpdate = false;
	}



	void GraphicsEngine::updateObjects(void)
	{
		_arcball->Update();
		_arcballPos = _arcball->GetTransformedPos();
	}

	void GraphicsEngine::update(void)
	{
		updateMeshData();
		updateObjects();
	}

	void GraphicsEngine::drawBackground(void)
	{
		glClearColor(0.0f, 0.6f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GraphicsEngine::drawObjects(void)
	{
		if (mVertices.size() == 0)
			return;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		glBindVertexArray(vao);
		{
			//glEnableVertexAttribArray(activePrg->GetVar("aPos"));
			glUseProgram(activePrg->GetId());

			glUniformMatrix4fv(activePrg->GetVar("uModlMat"), 1, GL_FALSE, glm::value_ptr(mModlMat));
			glUniformMatrix4fv(activePrg->GetVar("uViewMat"), 1, GL_FALSE, glm::value_ptr(_arcball->GetMatrix()));
			glUniformMatrix4fv(activePrg->GetVar("uProjMat"), 1, GL_FALSE, glm::value_ptr(mProjMat));

			glUniform3f(activePrg->GetVar("uItemColor"), 1.0f, 0.0f, 0.0f);
			glUniform3f(activePrg->GetVar("uViewPos"), _arcballPos.x, _arcballPos.y, _arcballPos.z);
			glUniform3f(activePrg->GetVar("uLightPos"), _arcballPos.x, _arcballPos.y, _arcballPos.z);
			glDrawArrays(GL_TRIANGLES, 0, meshCount);
		}
		glBindVertexArray(0);

	}

	void GraphicsEngine::drawDefaultObjects(void)
	{
	}

	void GraphicsEngine::draw(void)
	{
		drawBackground();
		drawObjects();

		if (_imgBuffer != nullptr)
			glReadPixels(0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, _imgBuffer);

	}

	void GraphicsEngine::Mainloop(char* imgBuffer)
	{
		_imgBuffer = imgBuffer;

		update();
		draw();



		glfwPollEvents();
		glfwSwapBuffers(mWindow);
	}

	void GraphicsEngine::AddMesh(const char* filename)
	{
		std::ifstream input(filename, std::ios::in | std::ios::binary);

		char header_info[80] = "";
		char nTri[4];
		unsigned long nrTri;

		input.read(header_info, 80);
		input.read(nTri, 4);

		nrTri = *((unsigned long*)nTri);

		char data[4 * 3 + 4 * 3 * 3 + 2];
		int sz = sizeof(data);
		float x, y, z;

		for (unsigned int tr = 0; tr < nrTri; tr++)
		{
			short idx = 0;
			input.read(data, sizeof(data));

			for (unsigned int i = 0; i < 4; i++)
			{
				char val0[4] = { data[idx+0], data[idx+1], data[idx+2], data[idx+3] };
				char val1[4] = { data[idx+4], data[idx+5], data[idx+6], data[idx+7] };
				char val2[4] = { data[idx+8], data[idx+9], data[idx+10], data[idx+11] };

				x = *((float*)val0);
				y = *((float*)val1);
				z = *((float*)val2);

				if (i == 0) 
				{
					for (unsigned int j = 0; j < 3; j++)
					{
						mNormals.push_back(x);
						mNormals.push_back(y);
						mNormals.push_back(z);
					}
				}
				else 
				{
					mVertices.push_back(x);
					mVertices.push_back(y);
					mVertices.push_back(z);
				}

				idx += 12;
			}
		}

		beginCount += nrTri * 3;

		input.close();
		mNeedsMeshDataUpdate = true;
	}



	void GraphicsEngine::initWindow(bool invisibleWindow)
	{
		if (!glfwInit())
			throw std::exception();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		if(invisibleWindow)
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		glfwWindowHint(GLFW_SAMPLES, 4);

		mWindow = glfwCreateWindow(640, 480, "Hidden OpenGL window", NULL, NULL);

		if (!mWindow)
		{
			glfwTerminate();
			throw std::exception();
		}

		glfwMakeContextCurrent(mWindow);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			throw std::exception();
		}

		_arcball = new MonoMaxGraphics::Arcball();

	}

	void GraphicsEngine::initRenderData(void)
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vboP);
		glGenBuffers(1, &vboN);
	}


	void GraphicsEngine::Resize(int width, int height)
	{
		glfwSetWindowSize(mWindow, width, height);

		m_width = width;
		m_height = height;
		mBufferLength = width * height * 4;

		free(GLRenderHandle);
		GLRenderHandle = (char*)malloc(mBufferLength);

		mModlMat = glm::mat4(1.0f);

		mProjMat = glm::perspective(
			glm::radians(60.0f),
			(float)width / height,
			10.0f,
			100000.0f);

		glViewport(0, 0, width, height);
		_arcball->Resize(width, height);
	}


	const int GraphicsEngine::GetHeight(void) { return m_height; }
	const int GraphicsEngine::GetWidth(void) { return m_width; }


	GLFWwindow* GraphicsEngine::GetWindow(void)
	{
		return mWindow;
	}

	void GraphicsEngine::Init(bool offscreen)
	{
		initWindow(offscreen);
		initRenderData();
		initShaders();
	}

	void GraphicsEngine::DeInit(void)
	{
		glfwTerminate();

		delete mWindow;
		delete mShaderManager;
	}

	void GraphicsEngine::Stop(void)
	{
		isRunning = false;
	}

	void GraphicsEngine::MouseDown(const int x, const int y, const int button)
	{
		switch (button)
		{
		case 1: _lMs = MouseState::Press; 
			break;

		case 2: _rMs = MouseState::Press;
			break;

		case 3: _mMs = MouseState::Press;
			break;
		}

		_arcball->MouseDown(button, x, y);
	}

	void GraphicsEngine::MouseUp(void)
	{
		_arcball->MouseUp();
	}


	void GraphicsEngine::MouseMove(const int x, const int y)
	{
		_arcball->MouseMove(x, y);
	}

	void GraphicsEngine::MouseScroll(const int delta)
	{
		_arcball->MouseScroll(delta);
	}
}
