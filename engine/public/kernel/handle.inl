/*
SDLEngine
Matt Hoyle
*/

template< class Internal >
Handle<Internal>::Handle()
: m_internalData(nullptr)
{

}

template< class Internal >
Handle<Internal>::Handle(const Handle<Internal>& other)
: Handle()
{
	*this = other;
}

template< class Internal >
Handle<Internal>::Handle(Handle<Internal>&& other)
: Handle()
{
	*this = std::move(other);
}

template< class Internal >
Handle<Internal>::Handle(Internal* internalData)
{
	m_internalData = internalData;
	if (m_internalData)
		m_internalData->AddReference();
}

template< class Internal >
Handle<Internal>& Handle<Internal>::operator=(const Handle<Internal>& other)
{
	if (m_internalData)
		m_internalData->RemoveReference();

	m_internalData = other.m_internalData;
	if (m_internalData)
		m_internalData->AddReference();

	return *this;
}

template< class Internal >
Handle<Internal>& Handle<Internal>::operator=(Handle<Internal>&& other)
{
	if (m_internalData)
		m_internalData->RemoveReference();

	m_internalData = other.m_internalData;
	other.m_internalData = nullptr;

	return *this;
}

template< class Internal >
Handle<Internal>::~Handle()
{
	if (m_internalData)
		m_internalData->RemoveReference();
	m_internalData = nullptr;
}

template< class Internal >
typename Internal::InternalType* Handle<Internal>::operator*()
{
	if (m_internalData)
		return &m_internalData->GetData();

	return nullptr;
}

template< class Internal >
const typename Internal::InternalType* Handle<Internal>::operator*() const
{
	if (m_internalData)
		return &m_internalData->GetData();

	return nullptr;
}