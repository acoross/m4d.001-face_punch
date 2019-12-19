#pragma once

#include <unordered_map>
#include <functional>
#include <entityx/entityx.h>
#include "C_Base.h"

using CompId = unsigned int;
using ComponentFactory = std::unordered_map<CompId, std::function<C_Base * (entityx::Entity)>>;

class EntityLoader
{
public:
	template <class T>
	void Register(const CompId compId)
	{
		m_cFactory[compId] = [](entityx::Entity entity) { return entity.assign<T>().get(); };
	}

	C_Base* Assign(entityx::Entity entity, const CompId compId)
	{
		auto comp = m_cFactory.find(compId);
		if (comp == m_cFactory.end())
		{
			return nullptr;
		}

		return comp->second(entity);
	}

private:
	ComponentFactory m_cFactory;
};

