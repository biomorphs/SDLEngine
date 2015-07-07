/*
SDLEngine
Matt Hoyle
*/
#include "atomics.h"
#include <SDL_atomic.h>

namespace Kernel
{
	struct AtomicInt32::Impl
	{
		SDL_atomic_t m_atomic;
	};

	AtomicInt32::AtomicInt32()
	{
		static_assert(c_storageSize >= sizeof(Impl), "c_storageSize not big enough for this platform");
		static_assert(c_storageAlign >= alignof(Impl), "c_storageAlign not big enough for this platform");
		Set(0);
	}

	AtomicInt32::AtomicInt32(int32_t initialValue)
	{
		Set(initialValue);
	}

	AtomicInt32::~AtomicInt32()
	{
	}

	int32_t AtomicInt32::Add(int32_t v)
	{
		return SDL_AtomicAdd(&GetInternal().m_atomic, v);
	}

	int32_t AtomicInt32::Set(int32_t v)
	{
		return SDL_AtomicSet(&GetInternal().m_atomic, v);
	}

	int32_t AtomicInt32::Get()
	{
		return SDL_AtomicGet(&GetInternal().m_atomic);
	}
}