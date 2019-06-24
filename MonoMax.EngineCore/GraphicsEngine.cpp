#include "GraphicsEngine.h";


namespace MonoMaxGraphics
{
	glm::mat4 modelMat;
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

	void GraphicsEngine::addShader(GLuint prgId, const std::string shadercode, GLenum shadertype)
	{
		if (prgId < 0)
			throw new std::exception();

		GLuint id = glCreateShader(shadertype);

		if (id < 0)
			throw new std::exception();

		const char* code = shadercode.c_str();

		glShaderSource(id, 1, &code, NULL);
		glCompileShader(id);
		glAttachShader(prgId, id);
		glDeleteShader(id);
	}

	void GraphicsEngine::Render(char* imgBuffer)
	{
		glClearColor(0.8f, 0.8f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_MULTISAMPLE);

		modelMat = glm::mat4(1.0f);

		glBindVertexArray(m_vao);
		{
			glUseProgram(m_prg);
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(modelMat));

			glEnableVertexAttribArray(0);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		glBindVertexArray(0);

		rotY -= 0.05f;

		if(imgBuffer != nullptr)
			glReadPixels(0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, imgBuffer);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	const int GraphicsEngine::GetHeight(void) { return m_height; }
	const int GraphicsEngine::GetWidth(void) { return m_width; }

	void GraphicsEngine::initWindow(void)
	{
		if (!glfwInit())
			throw std::exception();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		//glfwWindowHint(GLFW_SAMPLES, 4);

		m_window = glfwCreateWindow(640, 480, "Hidden OpenGL window", NULL, NULL);

		if (!m_window)
		{
			glfwTerminate();
			throw std::exception();
		}

		glfwMakeContextCurrent(m_window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			throw std::exception();
		}
	}

	void GraphicsEngine::initRenderData(void)
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		modelMat = glm::mat4(1.0f);
	}

	void GraphicsEngine::initShaders(void)
	{
		std::string shaders[] =
		{
			getShaderCode("../../../../simple_color_vs.glsl"),
			getShaderCode("../../../../simple_color_fs.glsl")
		};

		m_prg = glCreateProgram();
		addShader(m_prg, shaders[0], GL_VERTEX_SHADER);
		addShader(m_prg, shaders[1], GL_FRAGMENT_SHADER);
		glLinkProgram(m_prg);

		glBindVertexArray(m_vao);
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);
	}

	void GraphicsEngine::Resize(int width, int height)
	{
		glfwSetWindowSize(m_window, width, height);

		m_width = width;
		m_height = height;
		m_bufferLength = width * height * 4;

		free(GLRenderHandle);
		GLRenderHandle = (char*)malloc(m_bufferLength);

		mProjMat = glm::perspective(
			glm::radians(60.0f),
			(float)width / height,
			0.01f,
			100.0f);

		mViewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glViewport(0, 0, width, height);

	}

	const int GraphicsEngine::GetBufferLenght(void)
	{
		return m_bufferLength;
	}

	void GraphicsEngine::Init(void)
	{
		initWindow();
		initRenderData();
		initShaders();
	}

	void GraphicsEngine::DeInit(void)
	{
		throw std::exception();
	}

	void GraphicsEngine::Stop(void)
	{
		isRunning = false;
	}

}
