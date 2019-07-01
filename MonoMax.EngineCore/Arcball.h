#pragma once
#include "common_math.h"

namespace MonoMaxGraphics
{
	class Arcball
	{
	private:
		bool mLeftIsDown;
		int mWidth, mHeight;
		float mRadius;
		glm::mat4 mMatrix;
		glm::vec3 MapToSphere(const int x, const int y);
		glm::vec3 MapToSphere2(const int x, const int y);
		glm::quat mQuat;

	public:
		Arcball();
		void MouseDown(const int x, const int y);
		void MouseMove(const int x, const int y);
		void MouseUp(void);
		void Resize(const int width, const int height);
		void Update(void);
		glm::mat4 GetMatrix(void);
	};

}