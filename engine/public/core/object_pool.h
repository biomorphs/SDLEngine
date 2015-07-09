/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "list.h"
#include <vector>
#include <type_traits>

namespace Core
{
	template<class ObjectType>
	class ObjectPool
	{
	public:
		ObjectPool(uint32_t poolSize);
		~ObjectPool();

		ObjectType* Allocate();
		ObjectType* Allocate(const ObjectType& other);	// copy-construct
		void Free(ObjectType* o);

	private:
		struct ObjectNodeData
		{
			typename std::aligned_storage< sizeof(ObjectType), alignof(ObjectType) >::type m_storage;
		};
		typedef ListNode<ObjectNodeData> ObjectNode;

		std::vector<ObjectNode> m_objectStorage;
		List<ObjectNode> m_freeList;
		List<ObjectNode> m_usedList;
	};
}

#include "object_pool.inl"