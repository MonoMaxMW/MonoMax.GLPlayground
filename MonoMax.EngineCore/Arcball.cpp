#include "Arcball.h"

namespace MonoMaxGraphics
{
	int currX, currY, prevX, prevY;
	glm::quat _quat, _prevQuat;
	glm::mat4 transMat;

	glm::vec3 getVectorWithArc(const int x, const int y, const int radius)
	{
		float arc = sqrtf(x * x + y * y);   // legnth between cursor and screen center
		float a = arc / radius;         // arc = r * a
		float b = atan2f(y, x);         // angle on x-y plane
		float x2 = radius * sinf(a);    // x rotated by "a" on x-z plane

		glm::vec3 vec;
		vec.x = x2 * cosf(b);
		vec.y = x2 * sinf(b);
		vec.z = radius * cosf(a);

		return vec;
	}

	void printPos(glm::vec3 pos)
	{
		std::cout << "X=" << pos.x << "|Y=" << pos.y << "|Z=" << pos.z << std::endl;
	}

	Arcball::Arcball()
	{
		float zDis = 300.0f;
		mLeftIsDown = false;

		transMat = glm::lookAt(
			glm::vec3(0.0f, 0.0f, zDis),
			glm::vec3(0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		_quat = glm::quat();
		_quat.x = 1.0f;

		_prevQuat = _quat;
	}


	void Arcball::MouseDown(const int x, const int y)
	{
		prevX = x;
		prevY = y;
		_prevQuat = _quat;
		mLeftIsDown = true;
	}
	
	void Arcball::MouseUp(void)
	{
		mLeftIsDown = false;
	}

	void Arcball::MouseMove(const int x, const int y)
	{
		if (mLeftIsDown)
		{

			glm::vec3 v1 = glm::normalize(MapToSphere2(prevX, prevY));
			glm::vec3 v2 = glm::normalize(MapToSphere2(x, y));


			glm::quat delta = glm::rotation(v1, v2);
			_quat = delta * _prevQuat;
		}
	}

	void Arcball::Resize(const int w, const int h)
	{
		mWidth = w;
		mHeight = h;

		mRadius = std::fmin(mWidth, mHeight) * 0.5f;
	}

	void Arcball::Update(void)
	{

		mMatrix = transMat * glm::mat4(_quat);
	}

	glm::mat4 Arcball::GetMatrix(void)
	{
		return mMatrix;
	}

	glm::vec3 Arcball::MapToSphere(const int x, const int y)
	{
		glm::vec3 vec = glm::vec3(
			(x - mWidth * 0.5f) / (mWidth * 0.5f),
			(mHeight * 0.5f - y) / (mHeight * 0.5f),
			0.0f
		);

		float length_squared = (float)(vec.x * vec.x + vec.y * vec.y);

		if (length_squared <= 1.0)
		{
			vec.z = sqrtf(1.0f - length_squared);
		}
		else
		{
			glm::normalize(vec);
		}


		return vec;

	}
	glm::vec3 Arcball::MapToSphere2(const int x, const int y)
	{
		if (mRadius == 0 || mWidth == 0 || mHeight == 0)
			return glm::vec3(0, 0, 0);

		// compute mouse position from the centre of screen (-half ~ +half)
		float mx = x - mWidth * 0.5f;
		float my = mHeight * 0.5f - y;    // OpenGL uses bottom to up orientation
		//float mx = clampX(x - halfScreenWidth);
		//float my = clampY(halfScreenHeight - y);    // OpenGL uses bottom to up orientation

		return getVectorWithArc(mx, my, mRadius); // default mode
	}
}