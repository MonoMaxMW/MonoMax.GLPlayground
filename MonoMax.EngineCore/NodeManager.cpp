#include "NodeManager.h"

namespace MonoMaxEngine
{
	NodeManager::NodeManager()
	{
		mNodes = new std::vector<MeshNode*>();
	}

	NodeManager::~NodeManager()
	{
		delete mNodes;
	}

	MeshNode* NodeManager::AddMeshNode(const unsigned int startIndex, const unsigned int count)
	{
		MeshNode* node = new MeshNode(startIndex, count, ++mMeshIdCounter);
		mNodes->push_back(node);


		return node;
	}

	void NodeManager::Update(void)
	{
		if (mNodes->size() <= 0)
			return;

		for (unsigned int i = 0; i < mNodes->size(); i++)
		{
			mNodes->at(i)->Update();
		}
	}
}