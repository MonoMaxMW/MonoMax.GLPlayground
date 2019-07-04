#include "Arcball.h"

using namespace glm;

namespace MonoMaxEngine
{
	int currX, currY, prevX, prevY;
	quat currQuat, prevQuat;
	
	
	mat4 posMat, panMat, rotMat;
	mat4 prevPanMat;
	
	
	vec3 pos, up, center, forward, pan, transformedPos;
	
	vec3 prevPos, prevUp, prevCenter;
	
	bool needsUpdate;
	bool isRightDown;


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
		mLeftIsDown = false;
		up = prevUp = glm::vec3(0.0f, 1.0f, 0.0f);
		pos = prevPos = glm::vec3(0.0f, 0.0f, 300.0f);
		center = prevCenter = glm::vec3(0.0f);

		panMat = prevPanMat = glm::mat4(1.0f);

		currQuat = identity<quat>();

		prevQuat = currQuat;
	}


	void Arcball::MouseDown(const int button, const int x, const int y)
	{
		prevX = x;
		prevY = y;

		if (button == 1)
		{
			mLeftIsDown = true;
			prevQuat = currQuat;
		}
		else if(button == 2)
		{
			isRightDown = true;
			prevPanMat = panMat;
		}
	}
	
	void Arcball::MouseUp(void)
	{
		mLeftIsDown = false;
		isRightDown = false;
	}

	void Arcball::MouseScroll(const int delta)
	{
		int j = delta;
		float newZ = pos.z - (30.0f * j);

		if (newZ > 1.0f)
		{
			needsUpdate = true;
			pos.z = newZ;
		}
	}

	void Arcball::MouseMove(const int x, const int y)
	{
		if (mLeftIsDown)
		{
			needsUpdate = true;

			glm::vec3 v1 = glm::normalize(MapToSphere2(prevX, prevY));
			glm::vec3 v2 = glm::normalize(MapToSphere2(x, y));

			glm::quat delta = glm::rotation(v1, v2);
			currQuat = delta * prevQuat;
		}
		else if (isRightDown)
		{
			needsUpdate = true;

			float panX = (prevX - x) * -0.3f;
			float panY = (prevY - y) * 0.3f;

			glm::mat4 deltaMat = glm::translate(glm::mat4(1.0f), glm::vec3(panX, panY, 0.0f));
			panMat = deltaMat * prevPanMat;

		}
	}

	void Arcball::Resize(const int w, const int h)
	{
		mWidth = w;
		mHeight = h;

		mRadius = std::fmin(mWidth, mHeight) * 0.5f;
		needsUpdate = true;
	}

	void Arcball::Update(void)
	{
		if (needsUpdate)
		{
			rotMat = mat4(currQuat);


			posMat = glm::lookAt(pos, center, up);
			mMatrix = posMat * panMat * rotMat;


			transformedPos = pos * mat3(mat4(mMatrix)) * 10.0f;

			needsUpdate = false;

		}
	}

	glm::vec3& Arcball::GetTransformedPos(void)
	{
		return transformedPos;
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