#include "SceneGraph.h"

#include "Engine.h"
#include "Transform.h"

#include <type_traits>

namespace mae
{
	SceneGraph::SceneGraph(Engine * const engine)
		: System(engine)
	{
		engine->SetManagingSystem<Transform>(this);
	}

	Component * const SceneGraph::OnComponentCreate(EntityHandle srcEnt, std::type_index cmpType)
	{
		Transform *t = new Transform(this, srcEnt);
		roots.push_back(t);
		return roots.back();
	}

	bool SceneGraph::OnComponentDestroy(EntityHandle srcEnt, Component * const srcCmp)
	{
		bool destroyed = false;
		for (Transform *t : roots)
		{			
			destroyed = destroyed || DestroyTransform(t, static_cast<Transform*>(srcCmp));
		}
		return destroyed;
	}

	void SceneGraph::OnUpdate()
	{
		//for (auto it = roots.begin(); it != roots.end(); it++)
		//{
		//	if ((*it)->GetParent() != nullptr)
		//	{
		//		it = roots.erase(it);
		//	}
		//}
		for (auto it = roots.begin(); it != roots.end(); it++)
		{				
			(*it)->OnUpdate();
		}
	}
	
	bool SceneGraph::DestroyTransform(Transform *const current, Transform *const target)
	{
		if (current == nullptr) return false;
		if (current == target) delete target; return true;

		auto it = current->children.begin();
		for (it; it != current->children.end(); it++)
		{
			return DestroyTransform(*it, target);
		}
	}

}
