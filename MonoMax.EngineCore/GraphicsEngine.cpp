#include "GraphicsEngine.h";
#include "io.h"
#include "common.h"
#include "ShaderPrg.h"


namespace MonoMaxGraphics
{
	float rotY;

	float vertices[] =
	{
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};

	std::string GraphicsEngine::getShaderCode(const char* filename)
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

	void GraphicsEngine::initShaders(void)
	{
		mShaderManager = new ShaderManger();

		ShaderPrg* simple = new ShaderPrg();
		simple->Create("SimpleShader");

		std::string shaderCode[] =
		{
			getShaderCode("C:/DEV/shaders/simple_color_vs.glsl"),
			getShaderCode("C:/DEV/shaders/simple_color_fs.glsl")
		};
		simple->AddShader(GL_VERTEX_SHADER, shaderCode[0]);
		simple->AddShader(GL_FRAGMENT_SHADER, shaderCode[1]);
		simple->Compile();
		{
			simple->AddVariable("uModlMat");
			simple->AddVariable("uViewMat");
			simple->AddVariable("uProjMat");
		}
		mShaderManager->AddShaderPrg(simple);

		glBindVertexArray(mVao);
		{
			glBindBuffer(GL_ARRAY_BUFFER, mVbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);
	}

	void GraphicsEngine::Render(char* imgBuffer)
	{
		glClearColor(0.8f, 0.8f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_MULTISAMPLE);

		ShaderPrg* prg = mShaderManager->GetShaderPrg("SimpleShader");

		glBindVertexArray(mVao);
		{
			glUseProgram(prg->GetId());

			glUniformMatrix4fv((*prg)["uModlMat"], 1, GL_FALSE, glm::value_ptr(mModlMat));
			glUniformMatrix4fv((*prg)["uViewMat"], 1, GL_FALSE, glm::value_ptr(mViewMat));
			glUniformMatrix4fv((*prg)["uProjMat"], 1, GL_FALSE, glm::value_ptr(mProjMat));

			glEnableVertexAttribArray(0);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		glBindVertexArray(0);

		if(imgBuffer != nullptr)
			glReadPixels(0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, imgBuffer);

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	const int GraphicsEngine::GetHeight(void) { return m_height; }
	const int GraphicsEngine::GetWidth(void) { return m_width; }

	void GraphicsEngine::initWindow(bool invisibleWindow)
	{
		if (!glfwInit())
			throw std::exception();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		if(invisibleWindow)
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		//glfwWindowHint(GLFW_SAMPLES, 4);

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
	}

	void GraphicsEngine::initRenderData(void)
	{
		glGenVertexArrays(1, &mVao);
		glGenBuffers(1, &mVbo);
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
			0.01f,
			100.0f);

		mViewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glViewport(0, 0, width, height);

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

}
