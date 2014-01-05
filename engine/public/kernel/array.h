/*
SDLEngine
Matt Hoyle
*/
#pragma once

// This is an allocator for keeping the array in static memory (on the stack, etc)
template<class ObjectType, uint32_t MaxObjects>
class StaticArrayAllocator
{
public:
	void* ReallocateBuffer(void* oldPtr, size_t size);

private:
	ObjectType m_buffer[MaxObjects];
};

// This is an allocator for keeping the array on the heap
template<class ObjectType>
class HeapArrayAllocator
{
public:
	void* ReallocateBuffer(void* oldPtr, size_t size);
};

// This is an array that can be either static or growing, and can be on the heap or static
template<class ObjectType, class Allocator = HeapArrayAllocator<ObjectType>, uint32_t GrowSize = 0>
class Array
{
public:
	Array();
	~Array();

	void ResizeFast(uint32_t objectCount);
	void Resize(uint32_t objectCount);
	void Reserve(uint32_t objectCount);

	ObjectType* PushBack();
	ObjectType& operator[](uint32_t index);
	const ObjectType& operator[](uint32_t index) const;

	void DeleteAt(uint32_t index);
	void DeleteAtFast(uint32_t index);

	uint32_t Size();
	uint32_t Capacity();

private:
	Allocator m_allocator;
	ObjectType* m_objects;
	uint32_t m_objectCount;
	uint32_t m_maxObjects;
};

#include "array.inl"
