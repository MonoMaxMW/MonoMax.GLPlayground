#pragma once
#include "common.h"
#include "MeshNode.h"

namespace MonoMaxEngine
{
	class NodeManager
	{
	private:
		std::vector<MeshNode*>* mNodes;
		int mMeshIdCounter;

	public:
		NodeManager();
		~NodeManager();

		MeshNode* AddMeshNode(const unsigned int startIndex, const unsigned int count);
		MeshNode* GetNodeAt(const int idx);
		void Update(void);
		int NodeCount(void) const;
	};
}