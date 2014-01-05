/*
SDLEngine
Matt Hoyle
*/

#include "assert.h"
#include <malloc.h>
#include <new>

template<class ObjectType, class Allocator, uint32_t GrowSize>
Array<ObjectType, Allocator, GrowSize>::Array()
: m_objects(nullptr)
, m_objectCount(0)
, m_maxObjects(0)
{

}

template<class ObjectType, class Allocator, uint32_t GrowSize>
Array<ObjectType, Allocator, GrowSize>::~Array()
{
	for (uint32_t i = 0; i < m_objectCount; ++i)
	{
		m_objects[i].~ObjectType();
	}

	m_objects = (ObjectType*)m_allocator.ReallocateBuffer(m_objects, 0);
	m_objectCount = 0;
	m_maxObjects = 0;
}

template<class ObjectType, class Allocator, uint32_t GrowSize>
uint32_t Array<ObjectType, Allocator, GrowSize>::Size()
{
	return m_objectCount;
}

template<class ObjectType, class Allocator, uint32_t GrowSize>
uint32_t Array<ObjectType, Allocator, GrowSize>::Capacity()
{
	return m_maxObjects;
}

template<class ObjectType, class Allocator, uint32_t GrowSize>
void Array<ObjectType, Allocator, GrowSize>::ResizeFast(uint32_t objectCount)
{
	if (objectCount < m_objectCount)	// shrink no resize
	{
		for (uint32_t i = objectCount; i < m_objectCount; ++i)
		{
			m_objects[i].~ObjectType();
		}

		m_objectCount = objectCount;
	}
	else if (objectCount < m_maxObjects)	// grow, no resize
	{
		for (uint32_t i = m_objectCount; i < objectCount; ++i)
		{
			new(&m_objects[i]) ObjectType();
		}

		m_objectCount = objectCount;
	}
	else
	{
		Resize(objectCount);
	}
}

template<class ObjectType, class Allocator, uint32_t GrowSize>
void Array<ObjectType, Allocator, GrowSize>::Resize(uint32_t objectCount)
{
	// if shrinking, destruct the end ones
	for (uint32_t i = objectCount; i < m_objectCount; ++i)
	{
		m_objects[i].~ObjectType();
	}

	m_objects = (ObjectType*)m_allocator.ReallocateBuffer(m_objects, sizeof(ObjectType)* objectCount);

	// if it grew, default construct the new ones
	if (m_objects)
	{
		for (uint32_t i = m_objectCount; i < objectCount; ++i)
		{
			new(&m_objects[i]) ObjectType();
		}
	}
	
	m_maxObjects = objectCount;
	m_objectCount = objectCount;

	if (objectCount > 0 && m_objects == nullptr)
	{
		SDE_ASSERT(false, "Failed to reallocate array");
		m_maxObjects = 0;
		m_objectCount = 0;
	}
}

template<class ObjectType, class Allocator, uint32_t GrowSize>
void Array<ObjectType, Allocator, GrowSize>::Reserve(uint32_t objectCount)
{
	if (objectCount > m_maxObjects)
	{
		m_objects = (ObjectType*)m_allocator.ReallocateBuffer(m_objects, sizeof(ObjectType)* objectCount);
		m_maxObjects = objectCount;
		if (objectCount > 0 && m_objects == nullptr)
		{
			SDE_ASSERT(false, "Failed to reallocate array");
			m_maxObjects = 0;
			m_objectCount = 0;
		}
	}
}

template<class ObjectType, class Allocator, uint32_t GrowSize>
ObjectType* Array<ObjectType, Allocator, GrowSize>::PushBack()
{
	if (m_objectCount < m_maxObjects)
	{
		return new(&m_objects[m_objectCount++]) ObjectType();
	}
	else if ( GrowSize > 0 )
	{
		uint32_t newSize = m_maxObjects + GrowSize;
		Reserve(newSize);
		return new(&m_objects[m_objectCount++]) ObjectType();
	}
	else
	{
		SDE_ASSERT(false, "Array is full!");
		return nullptr;
	}
}

template<class ObjectType, class Allocator, uint32_t GrowSize>
ObjectType& Array<ObjectType, Allocator, GrowSize>::operator[](uint32_t index)
{
	SDE_ASSERT(m_objects != nullptr, "Array is empty");
	SDE_ASSERT(index < m_objectCount, "Index out of bounds");

	return m_objects[index];
}

template<class ObjectType, class Allocator, uint32_t GrowSize>
const ObjectType& Array<ObjectType, Allocator, GrowSize>::operator[](uint32_t index) const
{
	SDE_ASSERT(m_objects != nullptr, "Array is empty");
	SDE_ASSERT(index < m_objectCount, "Index out of bounds");

	return m_objects[index];
}

template<class ObjectType, class Allocator, uint32_t GrowSize>
void Array<ObjectType, Allocator, GrowSize>::DeleteAt(uint32_t index)
{
	SDE_ASSERT(m_objects != nullptr, "Array is empty");
	SDE_ASSERT(m_objectCount > 0, "Array is empty");
	SDE_ASSERT(m_objectCount < index, "Index out of bounds");

	uint32_t i = index;
	uint32_t end = m_objectCount - 1;
	while (i < end)
	{
		m_objects[i] = std::move(m_objects[i + 1]);
	}
	m_objectCount--;
}

template<class ObjectType, class Allocator, uint32_t GrowSize>
void Array<ObjectType, Allocator, GrowSize>::DeleteAtFast(uint32_t index)
{
	SDE_ASSERT(m_objects != nullptr, "Array is empty");
	SDE_ASSERT(m_objectCount > 0, "Array is empty");
	SDE_ASSERT(m_objectCount < index, "Index out of bounds");
	m_objects[index] = std::move(m_objects[--m_objectCount]);
}

template<class ObjectType>
void* HeapArrayAllocator<ObjectType>::ReallocateBuffer(void* oldPtr, size_t size)
{
	return realloc(oldPtr, size);
}

template<class ObjectType, uint32_t MaxObjects>
void* StaticArrayAllocator<ObjectType, MaxObjects>::ReallocateBuffer(void* oldPtr, size_t size)
{
	SDE_ASSERT(size <= (sizeof(ObjectType)* MaxObjects));
	if (size <= (sizeof(ObjectType)* MaxObjects))
	{
		return m_buffer;
	}

	return nullptr;
}
