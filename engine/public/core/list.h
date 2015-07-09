/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Core
{
	template<class Data>
	class ListNode : public Data
	{
	public:
		ListNode() : m_previous(nullptr), m_next(nullptr) { }
		ListNode(const Data&other) : Data(other), m_previous(nullptr), m_next(nullptr) { }

		void SetPrevious(ListNode* p) { m_previous = p; }
		void SetNext(ListNode* n) { m_next = n; }
		const ListNode* GetPrevious() const { return m_previous; }
		const ListNode* GetNext()	const { return m_next; }
		ListNode* GetPrevious() { return m_previous; }
		ListNode* GetNext() { return m_next; }
	private:
		ListNode* m_previous;
		ListNode* m_next;
	};

	template<class NodeType>
	class List
	{
	public:
		List();
		~List();

		void PushFront(NodeType* n);
		void PushBack(NodeType* n);
		void Remove(NodeType* n);
		NodeType* PopFront();
		NodeType* PopBack();
		uint32_t GetSize() const;

		bool HasLoop() const;
		bool NodeInList(const NodeType* n) const;

		void ValidateInternal();

	private:
		NodeType* m_head;
		NodeType* m_tail;
	};
}

#include "list.inl"