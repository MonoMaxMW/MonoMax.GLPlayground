#pragma once
#include "common_math.h"

using namespace glm;

namespace MonoMaxEngine
{
	class MeshNode
	{
	private:
		unsigned int mStartIndex;
		unsigned int mCount;
		vec3 mPos, mScale;
		mat4 mMatrix;
		bool mNeedsUpdate;
		MeshNode* mParent;

	public:
		MeshNode(const unsigned int startIndex, const int count);


		void Update(void);

		// getters
		vec3& GetPos(void);
		vec3& GetScale(void);
		mat4& GetMatrix(void);

		//setters
		void SetPos(const vec3 newPos);
		void SetPos(const int x, const int y, const int z);
		void SetScale(const vec3 newScale);

	};
}