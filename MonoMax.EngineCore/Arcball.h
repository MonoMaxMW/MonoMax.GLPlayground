#pragma once
#include "io.h"
#include "common_math.h"

using namespace glm;

namespace MonoMaxEngine
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
		void MouseDown(const int button, const int x, const int y);
		void MouseMove(const int x, const int y);
		void MouseUp(void);
		void MouseScroll(const int delta);
		void Resize(const int width, const int height);
		void Update(void);
		glm::vec3& GetTransformedPos(void);

		mat4 GetViewMatrixForDefaultNodes(void);
		mat4 GetViewMatrix(void);
	};

}