#pragma once
#include "common_math.h"

using namespace glm;

namespace MonoMaxEngine
{
	class MeshNode
	{
	private:
		unsigned int mStartIndex;
		unsigned int mOffset;
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
		unsigned int GetStartIdx(void) const;
		unsigned int GetOffset(void) const;
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
		void SetColor(const short r, const short g, const short b);
		void SetColor(const float r, const float g, const float b);

	};
}