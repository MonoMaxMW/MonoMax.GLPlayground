#include "MeshNode.h"

namespace MonoMaxEngine
{
	MeshNode::MeshNode(
		const unsigned int startIndex, 
		const unsigned int count,
		const unsigned int sessionId)
	{
		mStartIndex = startIndex;
		mCount = count;
		mPos = vec3(0.0f);
		mScale = vec3(1.0f);
		mNeedsUpdate = true;
		mMatrix = mat4(1.0f);
		mColor = vec3(0.5f);
		mSessionId = sessionId;
	}

	vec3& MeshNode::GetColor(void)
	{
		return mColor;
	}

	vec3& MeshNode::GetPos(void)
	{
		return mPos;
	}

	vec3& MeshNode::GetScale(void)
	{
		return mScale;
	}

	mat4& MeshNode::GetMatrix(void)
	{
		return mMatrix;
	}

	unsigned int& MeshNode::GetSessionId(void)
	{
		return mSessionId;
	}

	void MeshNode::SetPos(vec3 newPos)
	{
		mPos = newPos;
		mNeedsUpdate = true;
	}

	void MeshNode::SetPos(const int x, const int y, const int z)
	{
		SetPos(vec3(x, y, z));
	}

	void MeshNode::SetScale(const vec3 newScale)
	{
		mScale = newScale;
		mNeedsUpdate = true;
	}

	void MeshNode::SetColor(const vec3 newColor)
	{
		mColor = newColor;
	}

	void MeshNode::Update(void)
	{
		if (!mNeedsUpdate)
			return;

		mat4 idt = mat4(1.0f);
		mat4 pMatrix = mat4(1.0f);

		if (mParent != nullptr)
			pMatrix = mParent->GetMatrix();

		mMatrix = 
			glm::scale(idt, mScale) *
			glm::translate(idt, mPos) * 
			pMatrix;

		mNeedsUpdate = false;
	}
}
