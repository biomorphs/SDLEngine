/*
SDLEngine
Matt Hoyle
*/
#pragma once

// This is a wrapper for reference counted data to be used by handles
template<class DataType>
class RefcountedData
{
public:
	typedef DataType InternalType;

	RefcountedData();
	explicit RefcountedData(const DataType& data);
	RefcountedData(RefcountedData&& other);
	RefcountedData& operator=(RefcountedData&& other);
	~RefcountedData();

	void AddReference();
	void RemoveReference();
	int32_t GetReferenceCount() const;
	DataType& GetData();

private:
	RefcountedData(const RefcountedData& other);
	RefcountedData& operator=(const RefcountedData& other);

	DataType m_data;
	int32_t m_refCount;
};

#include "refcounted_data.inl"