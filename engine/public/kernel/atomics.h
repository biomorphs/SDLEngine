/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "base_types.h"
#include <type_traits>

namespace Kernel
{
	class AtomicInt32
	{
	public:
		AtomicInt32();
		AtomicInt32(int32_t initialValue);
		~AtomicInt32();

		int32_t Add(int32_t v);
		int32_t Set(int32_t v);
		int32_t Get();	// danger!

	private:
		// This is nasty, but since SDL is retarded and stores atomics as structs, 
		// and we do NOT want to expose SDL outside kernel, we (ab)use aligned_storage
		// to hide it in a pimpl kind of fashion
		struct Impl;
		Impl& GetInternal() { return reinterpret_cast<Impl&>(m_storage); }
		const Impl& GetInternal() const { return reinterpret_cast<const Impl&>(m_storage); }
		static const size_t c_storageSize = 4;
		static const size_t c_storageAlign = 8;
		std::aligned_storage<c_storageSize, c_storageAlign>::type m_storage;
	};
}