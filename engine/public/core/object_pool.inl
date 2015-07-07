/*
SDLEngine
Matt Hoyle
*/

namespace Core
{
	template<class ObjectType>
	ObjectPool<ObjectType>::ObjectPool(uint32_t poolSize)
	{
		m_objectStorage.resize(poolSize);
		for (uint32_t i = 0;i < poolSize;++i)
		{
			m_freeList.PushBack(&m_objectStorage[i]);
		}
	}

	template<class ObjectType>
	ObjectPool<ObjectType>::~ObjectPool()
	{

	}

	template<class ObjectType>
	ObjectType* ObjectPool<ObjectType>::Allocate()
	{
		SDE_ASSERT(m_usedList.GetSize() + m_freeList.GetSize() == m_objectStorage.size());
		auto n = m_freeList.PopFront();
		if (n != nullptr)
		{
			new (reinterpret_cast<void*>(&n->m_storage)) ObjectType();
			m_usedList.PushBack(n);
			return reinterpret_cast<ObjectType*>(n);
		}
		return nullptr;
	}

	template<class ObjectType>
	void ObjectPool<ObjectType>::Free(ObjectType* o)
	{
		ObjectNode* n = reinterpret_cast<ObjectNode*>(o);
		SDE_ASSERT(m_usedList.GetSize() + m_freeList.GetSize() == m_objectStorage.size());

		o->~ObjectType();	// destroy
		m_usedList.Remove(n);
		m_freeList.PushFront(n);

		SDE_ASSERT(m_usedList.GetSize() + m_freeList.GetSize() == m_objectStorage.size());
	}
}