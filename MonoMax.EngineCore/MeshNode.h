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
		vec3 mColor;
		mat4 mMatrix;
		bool mNeedsUpdate;
		MeshNode* mParent;
		unsigned int mSessionId;

	public:
		MeshNode(const unsigned int startIndex, const unsigned int count, const unsigned int sessionId);


		void Update(void);

		// getters
		vec3& GetColor(void);
		vec3& GetPos(void);
		vec3& GetScale(void);
		mat4& GetMatrix(void);
		unsigned int& GetSessionId(void);

		//setters
		void SetPos(const vec3 newPos);
		void SetPos(const int x, const int y, const int z);
		void SetScale(const vec3 newScale);
		void SetColor(const vec3 newColor);

	};
}