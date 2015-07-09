/*
SDLEngine
Matt Hoyle
*/
#include "kernel/assert.h"

#ifdef SDE_DEBUG
	//#define SDE_ENABLE_AUTO_LIST_VALIDATION	// Really frickin' slow
	#define SDE_ENABLE_LIST_VALIDATION
#endif

namespace Core
{
	template<class NodeType>
	List<NodeType>::List()
		: m_head(nullptr)
		, m_tail(nullptr)
	{
	}

	template<class NodeType>
	List<NodeType>::~List()
	{
	}

	template<class NodeType>
	NodeType* List<NodeType>::PopFront()
	{
		NodeType* n = m_head;
		if (n != nullptr)
		{
			Remove(m_head);
		}
		return n;
	}

	template<class NodeType>
	NodeType* List<NodeType>::PopBack()
	{
		NodeType* n = m_tail;
		if (n != nullptr)
		{
			Remove(m_tail);
		}
		return n;
	}

	template<class NodeType>
	void List<NodeType>::PushFront(NodeType* n)
	{
		// Ensure the node is not linked to a list already
		SDE_ASSERT(n->GetNext() == nullptr);
		SDE_ASSERT(n->GetPrevious() == nullptr);
		n->SetNext(m_head);
		if (m_head != nullptr)
		{
			m_head->SetPrevious(n);
		}
		m_head = n;
		if (m_tail == nullptr)
		{
			m_tail = m_head;
		}

#ifdef SDE_ENABLE_AUTO_LIST_VALIDATION
		ValidateInternal();
#endif
	}

	template<class NodeType>
	void List<NodeType>::PushBack(NodeType* n)
	{
		// Ensure the node is not linked to a list already
		SDE_ASSERT(n->GetNext() == nullptr);
		SDE_ASSERT(n->GetPrevious() == nullptr);
		n->SetPrevious(m_tail);
		if (m_tail != nullptr)
		{
			m_tail->SetNext(n);
		}
		m_tail = n;
		if (m_head == nullptr)
		{
			m_head = m_tail;
		}

#ifdef SDE_ENABLE_AUTO_LIST_VALIDATION
		ValidateInternal();
#endif
	}

	template<class NodeType>
	void List<NodeType>::Remove(NodeType* n)
	{
#ifdef SDE_ENABLE_AUTO_LIST_VALIDATION
		SDE_ASSERT(NodeInList(n));
#endif
		if (m_head == n)
		{
			m_head = n->GetNext();
		}
		if (m_tail == n)
		{
			m_tail = n->GetPrevious();
		}

		if (n->GetNext() != nullptr)
		{
			n->GetNext()->SetPrevious(n->GetPrevious());
		}
		if (n->GetPrevious() != nullptr)
		{
			n->GetPrevious()->SetNext(n->GetNext());
		}
		
		n->SetNext(nullptr);
		n->SetPrevious(nullptr);

#ifdef SDE_ENABLE_AUTO_LIST_VALIDATION
		ValidateInternal();
#endif
	}

	template<class NodeType>
	bool List<NodeType>::HasLoop() const
	{
		if (m_head == m_tail)
		{
			return false;
		}
		NodeType* tortoise = m_head, *hare = m_head;
		while (true) 
		{
			tortoise = tortoise->GetNext();
			if (hare->GetNext() != nullptr)
			{
				hare = hare->GetNext()->GetNext();
			}
			else
			{
				return false;
			}
			if (tortoise == nullptr || hare == nullptr)
			{
				return false;
			}
			if (tortoise == hare) // if the two ever meet...we must have a loop.
			{
				return true;
			}
		}
		return false;
	}

	template<class NodeType>
	bool List<NodeType>::NodeInList(const NodeType* node) const
	{
		for (NodeType* n = m_head; n != nullptr; n = n->GetNext())
		{
			if (n == node)
			{
				return true;
			}
		}
		return false;
	}

	template<class NodeType>
	uint32_t List<NodeType>::GetSize() const
	{
		uint32_t count = 0;
		for (NodeType* n = m_head; n != nullptr; n = n->GetNext())
		{
			++count;
		}
		return count;
	}

	template<class NodeType>
	void List<NodeType>::ValidateInternal()
	{
#ifdef SDE_ENABLE_LIST_VALIDATION
		SDE_ASSERT(!HasLoop());
		if (m_head)
		{
			SDE_ASSERT(m_head->GetPrevious() == nullptr);
		}
		if (m_tail)
		{
			SDE_ASSERT(m_tail->GetNext() == nullptr);
		}
#endif
	}

}