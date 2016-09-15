#pragma once
#include <functional>
#include <atomic>
#include <thread>
#include "sem_queue.h"
/**
 * @brief The Joint class The general connection interface.
 * A Joint kann receive Items of a given type T, and might
 * have an output joint.
 */


namespace ofl
{

template<typename T>
class Item
{
public:
	Item()
		  :m_flags(F_NONE)
	{
	}
	enum FLAGS
	{
		F_NONE = 0,
		F_INVALID = 1,
		F_STOP = 2 // if this flag is set the node will stop working and not use this item.
	};

	uint32_t& flags(){return  m_flags;}
	const uint32_t& flags() const {return m_flags;}

	T& payload(){return m_payload;}
	const T& payload() const{return m_payload;}
protected:
	uint32_t m_flags;
	T m_payload;


};



template<typename T>
class Joint
{
protected:
	Joint<T>* m_out;
	std::atomic_flag m_running;
	std::function<void (void)> m_on_start;
public:
	Joint(std::function<void (void)> on_start):
		m_out(nullptr),m_on_start(on_start)
	{}
	virtual ~Joint(){}

	virtual void insert(Item<T> itm) = 0;
	virtual void grab(Item<T>& itm) = 0;

	void setOutput(Joint<T> * o){m_out = o;}
	Joint<T>* getOutput(){return m_out;}

	/**
	 * @brief link Connects this joint to another and vice versa:
	 * A <------> this
	 * @param A The other joint.
	 */
	void link(Joint<T>* A)
	{
		this->setOutput(A);
		A->setOutput(this);
	}

	/**
	 * @brief linkInBetween Links this Joint between two others:
	 *
	 * A --->this --->B
	 * |______________|
	 *
	 * @param A The joint this one gets input from and B shall write to
	 * @param B The joint this one writes to.
	 */
	void linkInBetween(Joint<T>* A,Joint<T>* B)
	{
		this->setOutput(B);
		A->setOutput(this);
		B->setOutput(A);
	}

	void start()
	{
		if(!m_running.test_and_set())
		{
			m_on_start();
			if(m_out)
				m_out->start();
		}
	}
	void stop(){m_running.clear();}
};




/**
 * @brief The QueuedJoint class
 */
template <typename T>
class QueuedJoint : public Joint<T>
{
protected:
	SemQueue<Item<T>> m_queue;
public:
	QueuedJoint(std::function<void (void)> on_start)
		:Joint<T>(on_start)
	{}
	virtual ~QueuedJoint(){}

	virtual void insert(Item<T> itm)
	{
		if((itm.flags() & Item<T>::F_STOP) && !(this->m_running.test_and_set()))
		{
			this->m_running.clear();
			if(!(itm.flags() & Item<T>::F_INVALID))
			{
				itm.flags() ^= Item<T>::F_STOP;
				m_queue.enqueue(itm);
			}
		}
		else
			m_queue.enqueue(itm);
	}
	virtual void grab(Item<T>& itm)
	{
		SemQueueErrorCode ec = m_queue.dequeue(&itm);
		if(ec & IS_EMPTY)
			itm.flags() = (itm.flags() | Item<T>::F_INVALID);
	}

};





/**
 * @brief The Node class is something, that has a Joint, and will work on
 * the data, then give it to the Joints output.
 */
template <typename T>
class Node
{

protected:
	/**
	* @brief The Processor has only one joint.
	*/
	QueuedJoint<T> m_joint;

	std::thread m_thread;
	std::atomic_flag m_in_loop;


	virtual void m_loop()
	{
		Item<T> t;
		while(m_in_loop.test_and_set())
		{
			m_joint.grab(t);
			if(t.flags() & Item<T>::F_STOP)
			{
				m_in_loop.clear();
				m_joint.stop();
			}
			doSth(t);
			m_joint.getOutput()->insert(t);
		}

	}
public:

	Node() : m_joint([this](){this->start();})
	{
	}
	virtual ~Node(){}

	Joint<T>* getJoint(){return &m_joint;}

	virtual void start()
	{
		if(!m_in_loop.test_and_set())
		{
			m_joint.start();
			m_thread = std::thread(&Node::m_loop,this);
		}
	}

	virtual void stop()
	{
		Item<T> stop;
		stop.flags() = static_cast<typename Item<T>::FLAGS>(Item<T>::F_STOP |
													Item<T>::F_INVALID);
		m_joint.insert(stop);
	}

	virtual void kill()
	{
		m_in_loop.clear();
		m_joint.stop();

		Item<T> stop;
		stop.flags() = Item<T>::F_STOP |Item<T>::F_INVALID ;
		if(m_joint.getOutput())
		{
			m_joint.getOutput()->insert(stop);
		}
	}

	virtual void join()
	{
		if(m_thread.joinable())
			m_thread.join();
	}
	/**
	 * @brief doSth What you want to do with the items that pass through
	 * tis processor.
	 * @param t an item
	 */
	virtual void doSth(Item<T>& t) = 0;
};


/**
 * @brief The TransformNode class is a Node with two different
 * Types of Joints. One receives the input, which is transformed to the other
 * Type, which is then given to the right joint.
 */
template <typename T, typename Q>
class TransformNode
{
protected:
	QueuedJoint<T> m_left;
	QueuedJoint<Q> m_right;

	std::thread* m_thread;

	std::atomic_flag m_in_loop;


	virtual void m_loop()
	{
		Item<T> t;

		while(m_in_loop.test_and_set())
		{
			m_left.grab(t);
			printf("grabbed t:%d,%d\n",t.flags(),t.payload());
			if(t.flags() & Item<T>::F_STOP)
			{
				m_in_loop.clear();
				m_left.stop();


				Item<Q> s;
				m_right.stop();
				s.flags() = Item<Q>::F_STOP | Item<Q>::F_INVALID;
				if(m_right.getOutput())
					m_right.getOutput()->insert(s);

			}
			else if(t.flags() & Item<T>::F_INVALID)
			{
				m_in_loop.clear();
				continue;
			}


			doSth(t);

			if(m_left.getOutput())
				m_left.getOutput()->insert(t);
			else
				m_left.insert(t);

		}

	}
public:

	TransformNode() :
		m_left([this](){this->start();m_right.start();}),
		m_right([this](){m_left.start();})
	{}
	virtual ~TransformNode(){}

	virtual Joint<T>* getLeft(){return &m_left;}
	virtual Joint<Q>* getRight(){return &m_right;}

	virtual void start()
	{
		m_thread = new std::thread(&TransformNode::m_loop,this);
	}

	virtual void join()
	{
		m_thread->join();
	}
	/**
	 * @brief doSth What you want to do with the items.
	 * you can get outputbuffer with get_one and return them with push_out().
	 * if you decide not to return them return it with drop();
	 * @param t an item
	 */
	virtual void doSth(Item<T>& t) = 0;

	void push_out(Item<Q>&q)
	{
		m_right.getOutput()->insert(q);
	}

	void drop(Item<Q>&q)
	{
		m_right.insert(q);
	}

	void grab_right(Item<Q>& q)
	{
		m_right.grab(q);
	}

};



/**
 * @brief A pipeline shall be used to encapsulate parts of the pipeline :P
 */

template <typename T, typename Q>
class ProcessingGraph
{

public:
	virtual Joint<T>* getLeft()=0;
	virtual Joint<Q>* getRight()=0;

	virtual void start() = 0;

	virtual void stop() = 0;

	virtual void join() = 0;
};





}
