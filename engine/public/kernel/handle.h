/*
SDLEngine
Matt Hoyle
*/
#pragma once

// This is a handle to data with the same prototype as RefcountedData
// Use this to keep track of shared resources
// Internal should be ref counted data or something similar to be shared
template< class Internal >
class Handle
{
public:
	Handle();
	Handle(const Handle& other);
	Handle(Handle&& other);
	explicit Handle(Internal* internalData);
	Handle& operator=(const Handle& other);
	Handle& operator=(Handle&& other);
	~Handle();

	typename Internal::InternalType* operator*();
	const typename Internal::InternalType* operator*() const;

private:
	Internal* m_internalData;
};

#include "handle.inl"