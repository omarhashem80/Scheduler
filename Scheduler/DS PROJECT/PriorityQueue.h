#ifndef PRIORITY_QUEUE_
#define PRIORITY_QUEUE_
#include"Pair.h"
template <class ItemType>
class PriorityQueue
{
private:
	Node<ItemType>* head;
	int queueCount;

	Node<ItemType>* getNodeBefore(const ItemType& anEntry) const
	{
		Node<ItemType>* curPtr = head;
		Node<ItemType>* prevPtr = nullptr;

		while (curPtr && (anEntry > curPtr->getItem()))
		{
			prevPtr = curPtr;
			curPtr = curPtr->getNext();
		}
		return prevPtr;
	}
	Node<ItemType>* getNodeAt(int position) const
	{
		Node<ItemType>* curPtr = head;

		for (int skip = 1; skip < position; skip++)
			curPtr = curPtr->GetNext();

		return curPtr;
	}
	
public:
	PriorityQueue() :queueCount(0), head(nullptr)
	{
	}
	PriorityQueue(const PriorityQueue<ItemType>& pQueue)
	{
		head = 0;
		queueCount = 0;
		if (pQueue.head)
		{
			Node<ItemType>* p = pQueue.head;
			while (p)
			{
				this->enqueue(p->getItem());
				p = p->getNext();
			}
		}
	}
	bool isempty() const
	{
		return queueCount == 0;
	}
	int GetSize() const
	{
		return queueCount;
	}
	bool enqueue(const ItemType& anEntry)
	{
		Node<ItemType>* newNodePtr = new Node<ItemType>(anEntry);
		Node<ItemType>* prevPtr = getNodeBefore(anEntry);

		if (isempty() || prevPtr == nullptr)
		{
			newNodePtr->setNext(head);
			head = newNodePtr;
		}
		else {
			Node<ItemType>* afterPtr = prevPtr->getNext();
			newNodePtr->setNext(afterPtr);
			prevPtr->setNext(newNodePtr);
		}

		queueCount++;
		return true;
	}
	bool dequeue(ItemType& val)
	{
		if (isempty())
		{
			return false;
		}

		Node<ItemType>* temp = head;
		head = head->getNext();
		val = temp->getItem();
		delete temp;

		queueCount--;
		return true;
	}
	ItemType peek(ItemType& val) const
	{
		val = head->getItem();
		return head;
	}
	virtual ~PriorityQueue()
	{
	}
};
#endif
