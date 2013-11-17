/*
SDLEngine
Matt Hoyle
*/

#include "kernel/assert.h"

template<class DataType>
inline RefcountedData<DataType>::RefcountedData()
: m_refCount(0)
{
}

template<class DataType>
inline RefcountedData<DataType>::RefcountedData(const DataType& data)
: m_data(data)
, m_refCount(0)
{
}

template<class DataType>
inline RefcountedData<DataType>::~RefcountedData()
{
	SDE_ASSERT(m_refCount == 0, "Destroying reference counted data, but there are still references to it!");
}

template<class DataType>
inline RefcountedData<DataType>::RefcountedData(RefcountedData&& other)
{
	*this = std::move(other);
}

template<class DataType>
inline RefcountedData<DataType>& RefcountedData<DataType>::operator=(RefcountedData<DataType>&& other)
{
	m_data = std::move(other.m_data);
	m_refCount = other.m_refCount;
	return *this;
}

template<class DataType>
inline void RefcountedData<DataType>::AddReference()
{
	++m_refCount;
}

template<class DataType>
inline void RefcountedData<DataType>::RemoveReference()
{
	--m_refCount;
}

template<class DataType>
inline DataType& RefcountedData<DataType>::GetData()
{
	return m_data;
}

template<class DataType>
int32_t RefcountedData<DataType>::GetReferenceCount() const
{
	return m_refCount;
}