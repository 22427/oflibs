#if 0
<begin_doc>
//The MIT License (MIT)
//================================================================================
//
//Copyright (c) 2016 Timon Zietlow
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of
//this software and associated documentation files (the "Software"), to deal in
//the Software without restriction, including without limitation the rights to
//use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
//the Software, and to permit persons to whom the Software is furnished to do so,
//subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

--------------------------------------------------------------------------------

//What is this?
//================================================================================
//The oflibs are a set of usefull classes and tools dealing with all kinds of 
//problems.  
//My personal focus is in 3D-visualisation and VR, so most of these deal with 
//these topics. OFLIBS stands for "**o**ne **f**ile **lib**rarie**s**". The basic 
//idea is to write libraries in a single file, which you just have to include into
//your project, and you are ready to go.
//I find these usefull for fast prototypes and "experimental" code, which is what 
//this collection currently is, so don't expect a well tested, woking toolset, but
//maybe you find the one or two lines of code you were searching ;).
//I'm constantly working whith these tools and I am shure they will get better 
//over time.
//
//Structure
//--------------------------------------------------------------------------------
//In oflibs/ you find the finished one file libraries. They are ready to use. The 
//documentation to each library is also inside of the lib. Here is a short list of
//the different modules and libraries:
//
//- ofl_all: All oflibs in one 
//- ofl_ogl_geo - A class reperesenting vertex-data on the GPU
//- ofl_ogl_state - A state-wrapper imulating the "classic" OpenGL-fixed-function 
//  states, including matrix stacks and lighting.
//- ofl_ogl_win - An interface to create an OpenGL-Context with window and reading 
//  events. Currently implemented using glfw.
//- ofl_open_gl_tools: combines the above.
//- ofl_socket: A class wrapping sockets.
//- ofl_stru: Some string utilities. Used by many other oflibs.
//- ofl_track - Tracking: An interface to the ART-DTrack2 tracking system.
//- olf_vd - Vertex Data: A data structure representing geometry on the client 
//  side. This is OpenGL independend. 
//- ofl_vdman - Vertex Data MANufacturer: Allows you to create VertexData using
//  the old begin(..) ... end() paradigma from fixed-function-GL, but is OpenGL
//  independend.
//- ofl_vmath - Vector MATH : Some simple vector operations, but no alternative to 
//  GLM.
//- ofl_vrpv - Virtual Reality Projection and View - A module to genereate view-
//  and projection-matrices for VR systems like PowerWalls or CAVEs
//
//How do oflibs work?
//--------------------------------------------------------------------------------
//You use the oflib as a regular header, for example:
//```c_cpp
	#include <ofl_all.h>
//```
//Then in one .cpp-file of your project you include the oflib defining
//`OFL_IMPLEMENTATION` before:
//```c_cpp
//	#define OFL_IMPLEMENTATION
	#include <ofl_all.h>
//	#undef OFL_IMPLEMENTATION
//```
//That's it :)
//
//Internally the oflib-header is organized in two parts, the declaration-part 
//(that's what you would write into an ordinary header) and the implementation-part 
//(that's what you would write into *.cpp files)
//Example:
//```c_cpp
//	#ifndef OFL_ALL_H
//	#define OFL_ALL_H
//		// ALL DECLARATIONS
//	#endif
//	#ifdef OFL_IMPLEMENTATION
//		// ALL THE IMPLEMENTATIONS
//	#endif
//```
//
//Where can I get more information?
//--------------------------------------------------------------------------------
//Inside the oflib files there is the documentation for each module used. 
//In the subdirectory tools/ there are also some more or less simple tools using 
//the oflibs.
//
//
//

--------------------------------------------------------------------------------

//PROCESSING_GRAPH: Processing Graph
//================================================================================
//This is a set of classes to build a processing graph.
//The idea is, that processing data often requires different steps or operations.
//In order to keep the opeartions interchangeable and reusable, you can use this
//interface to join different operation nodes together to a processing graph.
//
//Each nodes performes asyncronous to the other joints(spanws a thread),
//so this system makes uses of all available cores.
//
//API
//--------------------------------------------------------------------------------
//
//###Nodes
//A Node is an Object, that processes data. Nodes are connected via Joints. A pair
//of Joints represents a queue/dequeue interaface. Each node works assyncronous.
//
//####Transformer
//A Transformer is like a Source and a Sink glued together. On one side
//It is a sink for items of type A (left) and on the other side it is a Source of
//items of type B. So a Transformer would, for example, read Images on the left
//and write positions of detected features on the right.
//
//
//

--------------------------------------------------------------------------------

<end_doc>
#endif 
#ifndef USING_OFL_PROCESSING_GRAPH_H
 #define USING_OFL_PROCESSING_GRAPH_H
 #include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

/**
 * @class SemQueue
 * @author Timon Zietlow
 * @date 29/08/16
 * @brief A semaphore-like-queue.
 * @details A generic queue which behaves like a semaphore.
 * If the queue is empty all calls for dequeue will block untill new items
 * are enqueued. All methods are threadsave!.
 */
enum SemQueueErrorCode {
	OK = 0,
	NO_ERROR = OK,
	IS_EMPTY = 1,
	TIMEOUT = 2,
	SHUTDOWN = 4,
	SHUTDOWN_AND_EMPTY =5,

};

template <typename T> class SemQueue
{
	std::queue<T> q;
	std::condition_variable cv;
	std::mutex mtx;
	bool running;

public:

	/** Constructor
	*/
	SemQueue() : running(true)
	{
	}

	/** Destructor
	*/
	~SemQueue()
	{
		shutdown();
	}

	/**
	 * @brief Adds an item to the queue.
	 * @param item The item to enqueue.
	 */
	void enqueue(T&& item)
	{
		std::lock_guard<std::mutex> g(mtx);
		q.push(std::move(item));
		cv.notify_one();
	}

	/**
	 * @brief  Adds an item to the queue.
	 * @param item The item to enqueue.
	 */
	void enqueue(const T& item)
	{
		std::lock_guard<std::mutex> g(mtx);
		q.push(std::move(item));
		cv.notify_one();
	}

	/**
	 * @brief Tells you whether the queue is empty.
	 * @return 'true' if the queue is empty, 'false' if the queue is not empty
	 */
	bool empty()
	{
		std::lock_guard<std::mutex> g(mtx);
		bool to_ret = q.empty();
		return to_ret;
	}

	/**
	 * @brief Get the first item from the queue.
	 * If the queue is empty this funcion will block until another item
	 * is enqueued into the queue, or the SemQueue is shutdown.
	 * @retval t The first item from the queue
	 * 'OK' if everything is fine,
	 * 'SHUTDOWN' if the queue is on shutdown, but there are still items to
	 *		dequeue,
	 * 'SHUTDOWN_AND_EMPTY' if the queue  is on shutdown and empty and
	 * 'IS_EMPTY' if the queue was empty.
	 */
	SemQueueErrorCode dequeue(T* t)
	{
		std::unique_lock<std::mutex> lock(mtx);
		if (q.empty())
			cv.wait(lock, [&]() { return !q.empty() || !running; });
		if (!running)
		{
			if(!q.empty())
			{
				*t = q.front();
				q.pop();
				return SHUTDOWN;
			}
			else
				return SHUTDOWN_AND_EMPTY;

		}


		*t = q.front();
		q.pop();

		return OK;
	}

	/**
	 * @brief Get the first item from the queue or time out.
	 * @retval t The first item from the queue
	 * @param ms The time to wait till timeout in ms
	 * @return
	 * 'OK' if everything is fine,
	 * 'TIMEOUT' if the call timed out.
	 * 'SHUTDOWN' if the queue is on shutdown, but there are still items to
	 *		dequeue,
	 * 'SHUTDOWN_AND_EMPTY' if the queue  is on shutdown and empty and
	 * 'IS_EMPTY' if the queue was empty.
	 */
	SemQueueErrorCode dequeue(T* t, uint32_t ms)
	{
		std::unique_lock<std::mutex> lock(mtx);
		auto wres = std::cv_status::no_timeout;

		if (q.empty()) {
			wres = cv.wait_for(lock, std::chrono::milliseconds(ms));
		}

		if (wres == std::cv_status::timeout)
			return TIMEOUT;
		if (!running)
		{
			if(!q.empty())
			{
				*t = q.front();
				q.pop();
				return SHUTDOWN;
			}
			else
				return SHUTDOWN_AND_EMPTY;
		}

		*t = q.front();
		q.pop();

		return OK;
	}

	/**
	 * @brief Try to get the first item from the queue.
	 * @details If the queue is empty this funcion will return immediatley.
	 * @retval t The first item from the queue
	 * @return
	 * 'OK' if everything is fine,
	 * 'SHUTDOWN' if the queue is on shutdown, but there are still items to
	 *		dequeue,
	 * 'SHUTDOWN_AND_EMPTY' if the queue  is on shutdown and empty and
	 * 'IS_EMPTY' if the queue was empty.
	 */
	SemQueueErrorCode try_dequeue(T* t)
	{
		std::lock_guard<std::mutex> g(mtx);
		if (q.empty())
		{
			return IS_EMPTY;
		}
		if (!running)
		{
			if(!q.empty())
			{
				*t = q.front();
				q.pop();
				return SHUTDOWN;
			}
			else
				return SHUTDOWN_AND_EMPTY;
		}

		*t = q.front();
		q.pop();
		return OK;
	}

	/**
	 * @brief Set this queue to shutdown mode
	 * @details Will wake up all waiting 'dequeue()' calls and make them
	 * return 'SHUTDOWN' or 'SHUTDOWN_AND_EMPTY'. All coming 'dequeue()' calls
	 * will return 'SHUTDOWN' or 'SHUTDOWN_AND_EMPTY' immediately, until
	 * 'start()' is called.
	 */
	void shutdown()
	{
		std::lock_guard<std::mutex> g(mtx);
		this->running = false;
		cv.notify_all();
	}

	/**
	 * @brief Set this queue to running mode.
	 * @details All upcoming 'dequeue()' calls will now block if the queue is
	 * empty.
	 */
	void start()
	{
		mtx.lock();
		this->running = true;
		mtx.unlock();
	}
};
#include <functional>
#include <atomic>
#include <thread>
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

#endif //USING_OFL_PROCESSING_GRAPH_H
#ifndef USING_OFL_SEM_QUEUE_H
 #define USING_OFL_SEM_QUEUE_H
 
#endif //USING_OFL_SEM_QUEUE_H
#ifdef OFL_IMPLEMENTATION
#endif
