#include "MeshNode.h"

namespace MonoMaxGraphics
{
	void MeshNode::Set(const int beginIdx, const int verticesCount)
	{
		mBeginIdx = beginIdx;
		mVerticesCount = verticesCount;
	}
}
