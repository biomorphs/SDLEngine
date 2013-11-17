/*
SDLEngine
Matt Hoyle
*/
#pragma once

// This is an allocator for keeping the bucket in static memory (on the stack, etc)
template<class ObjectType, uint32_t MaxObjects>
class StaticBucketAllocator
{
public:
	void* AllocateBuffer(size_t size);
	uint32_t* AllocateFreeList(size_t size);
	uint32_t* AllocateUsedList(size_t size);
	void Free(void* buffer) { }
private:
	static const size_t c_BucketSize = sizeof(ObjectType)* MaxObjects;
	uint8_t m_buffer[c_BucketSize];
	uint32_t m_freeListBuffer[MaxObjects];
	uint32_t m_usedListBuffer[MaxObjects];
};

// This is an allocator for keeping the bucket on the heap
class HeapBucketAllocator
{
public:
	void* AllocateBuffer(size_t size);
	uint32_t* AllocateFreeList(size_t size);
	uint32_t* AllocateUsedList(size_t size);
	void Free(void* buffer);
};

// This is a very fast object bucket, used for tracking fixed-sized lists of
// objects. O(1) allocate, O(N/2) free
template<class ObjectType, uint32_t MaxObjects, class Allocator = StaticBucketAllocator<ObjectType, MaxObjects>>
class ObjectBucket
{
public:
	ObjectBucket();
	~ObjectBucket();

	ObjectType* AddObject();
	void RemoveObject(ObjectType* theObject);

private:
	ObjectBucket(const ObjectBucket& other);
	ObjectBucket& operator=(const ObjectBucket& other);

	Allocator m_allocator;
	ObjectType* m_objectBuffer;
	uint32_t* m_freeList;
	uint32_t m_freeCount;
	uint32_t* m_usedList;
	uint32_t m_usedCount;
	bool m_usedListDirty;		// track if we need to resort the used list for iteration

public:
	class iterator
	{
		friend class ObjectBucket;
	public:
		iterator()
			: m_usedListIndex((uint32_t)-1)
			, m_parent(nullptr)
		{

		}
		iterator(const iterator& other)
		{
			m_usedListIndex = other.m_usedListIndex;
			m_parent = other.m_parent;
		}
		bool operator==(const iterator& other)
		{
			return (m_parent == other.m_parent) && (m_usedListIndex == other.m_usedListIndex);
		}
		bool operator!=(const iterator& other)
		{
			return !(*this == other);
		}
		void operator++()
		{
			if (m_parent && (m_usedListIndex + 1) < m_parent->m_usedCount)
				++m_usedListIndex;
			else
			{
				m_parent = nullptr;
				m_usedListIndex = (uint32_t)-1;
			}
		}
		ObjectType& operator*()
		{
			static ObjectType s_badObject;

			if (m_parent && m_usedListIndex < m_parent->m_usedCount)
				return m_parent->m_objectBuffer[m_parent->m_usedList[m_usedListIndex]];

			SDE_ASSERT(false, "Bad iterator");
			return s_badObject;
		}
	private:
		uint32_t m_usedListIndex;
		ObjectBucket* m_parent;
	};

	iterator Begin()
	{
		iterator it;
		if (m_usedCount > 0)
		{
			// Resort the used list if required to ease cache thrashing
			if (m_usedListDirty)
			{
				qsort(m_usedList, m_usedCount, sizeof(*m_usedList), CompareUsedList);
				m_usedListDirty = false;
			}
			it.m_parent = this;
			it.m_usedListIndex = 0;
		}
		return it;
	}

	iterator End()
	{
		return iterator();
	}

private:
	static int CompareUsedList(const void * a, const void * b)
	{
		uint32_t ia = *(uint32_t*)a;
		uint32_t ib = *(uint32_t*)b;

		if (ia < ib)
			return -1;
		else if (ia > ib)
			return 1;
		else
			return 0;
	}
};

#include "object_bucket.inl"