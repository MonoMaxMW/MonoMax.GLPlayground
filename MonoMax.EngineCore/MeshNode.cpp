#include "MeshNode.h"

namespace MonoMaxEngine
{
	MeshNode::MeshNode(
		const unsigned int startIndex, 
		const unsigned int count,
		const unsigned int sessionId)
	{
		mStartIndex = startIndex;
		mOffset = count;
		mPos = vec3(0.0f);
		mScale = vec3(1.0f);
		mNeedsUpdate = true;
		mMatrix = mat4(1.0f);
		mColor = vec3(0.5f);
		mSessionId = sessionId;
	}

	
	unsigned int MeshNode::GetStartIdx(void) const
	{
		return mStartIndex;
	}

	unsigned int MeshNode::GetOffset(void) const
	{
		return mOffset;
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

	void MeshNode::MoveBy(const vec3 deltaVec)
	{
		mPos += deltaVec;
		mNeedsUpdate = true;
	}

	void MeshNode::MoveBy(const float x, const float y, const float z)
	{
		MoveBy(vec3(x, y, z));
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

	void MeshNode::SetScale(const float newScale)
	{
		SetScale(vec3(newScale));
	}

	void MeshNode::SetColor(const vec3 newColor)
	{
		mColor = newColor;
	}

	void MeshNode::SetColor(const short r, const short g, const short b)
	{
		SetColor(vec3(r / 255.0f, g / 255.0f, b / 255.0f));
	}

	void MeshNode::SetColor(const float r, const float g, const float b)
	{
		SetColor(vec3(r, g, b));
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
