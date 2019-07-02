#pragma once

namespace MonoMaxGraphics
{
	class MeshNode
	{
	private:
		int mBeginIdx;
		int mVerticesCount;

	public:
		void Set(const int beginIdx, const int verticesCount);

	};
}