/*
SDLEngine
Matt Hoyle
*/

#include "kernel/assert.h"
#include <malloc.h>

template<class ObjectType, uint32_t MaxObjects, class Allocator>
ObjectBucket<ObjectType, MaxObjects, Allocator>::ObjectBucket()
: m_freeCount(0)
, m_usedCount(0)
, m_usedListDirty(false)
{
	m_objectBuffer = (ObjectType*)m_allocator.AllocateBuffer(MaxObjects * sizeof(ObjectType));
	SDE_ASSERT(m_objectBuffer);

	m_freeList = m_allocator.AllocateFreeList(MaxObjects * sizeof(*m_freeList));
	SDE_ASSERT(m_freeList);

	m_usedList = m_allocator.AllocateUsedList(MaxObjects * sizeof(*m_usedList));
	SDE_ASSERT(m_usedList);

	// Init free list
	for (uint32_t i = 0; i < MaxObjects; ++i)
	{
		// In reverse order as we always pop from the back of the free list
		m_freeList[i] = MaxObjects - 1 - i;
	}
	m_freeCount = MaxObjects;
}

template<class ObjectType, uint32_t MaxObjects, class Allocator>
ObjectBucket<ObjectType, MaxObjects, Allocator>::~ObjectBucket()
{
	SDE_ASSERT(m_usedCount == 0);
	m_allocator.Free(m_objectBuffer);
	m_allocator.Free(m_freeList);
	m_allocator.Free(m_usedList);
}

template<class ObjectType, uint32_t MaxObjects, class Allocator>
ObjectType* ObjectBucket<ObjectType, MaxObjects, Allocator>::AddObject()
{
	SDE_ASSERT(m_freeCount > 0);
	if (m_freeCount == 0)
	{
		return nullptr;
	}

	// pop a new free one from the list
	uint32_t index = m_freeList[--m_freeCount];

	// add it to the used list
	m_usedList[m_usedCount++] = index;
	m_usedListDirty = true;

	// default-construct the object
	new (&m_objectBuffer[index]) ObjectType;
	return &(m_objectBuffer[index]);
}

template<class ObjectType, uint32_t MaxObjects, class Allocator>
void ObjectBucket<ObjectType, MaxObjects, Allocator>::RemoveObject(ObjectType* theObject)
{
	SDE_ASSERT(m_usedCount>0);

	// calculate the index into the buffer
	uint32_t objectIndex = (uint32_t)(theObject - m_objectBuffer);
	SDE_ASSERT(objectIndex < MaxObjects);

	// remove the object from the used list
	for (uint32_t i = 0; i < m_usedCount; ++i)
	{
		if (m_usedList[i] == objectIndex)
		{
			// swap this index with the back one
			m_usedList[i] = m_usedList[--m_usedCount];
			m_usedListDirty = true;
			break;
		}
	}

	// add the index to the back of the free list
	m_freeList[m_freeCount++] = objectIndex;

	// Finally, call the object destructor
	theObject->~ObjectType();
}

///////////////////////////////////////////////////////////////
// Static allocator

template<class ObjectType, uint32_t MaxObjects>
void* StaticBucketAllocator<ObjectType, MaxObjects>::AllocateBuffer(size_t size)
{
	SDE_ASSERT(size <= c_BucketSize);
	memset(m_buffer, 0, c_BucketSize);
	return m_buffer;
}

template<class ObjectType, uint32_t MaxObjects>
uint32_t* StaticBucketAllocator<ObjectType, MaxObjects>::AllocateFreeList(size_t size)
{
	SDE_ASSERT(size <= MaxObjects * sizeof(*m_freeListBuffer));
	memset(m_freeListBuffer, 0, sizeof(*m_freeListBuffer) * MaxObjects);
	return m_freeListBuffer;
}

template<class ObjectType, uint32_t MaxObjects>
uint32_t* StaticBucketAllocator<ObjectType, MaxObjects>::AllocateUsedList(size_t size)
{
	SDE_ASSERT(size <= MaxObjects * sizeof(*m_usedListBuffer));
	memset(m_usedListBuffer, 0, sizeof(*m_usedListBuffer) * MaxObjects);
	return m_usedListBuffer;
}

////////////////////////////////////////////////////////////////////
// Heap allocator

inline void* HeapBucketAllocator::AllocateBuffer(size_t size)
{
	return malloc(size);
}

inline uint32_t* HeapBucketAllocator::AllocateFreeList(size_t size)
{
	return (uint32_t*)malloc(size);
}

inline uint32_t* HeapBucketAllocator::AllocateUsedList(size_t size)
{
	return (uint32_t*)malloc(size);
}

inline void HeapBucketAllocator::Free(void* buffer)
{
	free(buffer);
}