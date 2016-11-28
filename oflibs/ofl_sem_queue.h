#if 0
<begin_doc>
#ifndef USING_OFL_LICENSE_MD
#define USING_OFL_LICENSE_MD
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

#endif //USING_OFL_LICENSE_MD

--------------------------------------------------------------------------------

#ifndef USING_OFL_README_MD
#define USING_OFL_README_MD
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
//- ogl_stereo_compositor - A simple way to render in stereo image formats.
//- ofl_ogl_win - An interface to create an OpenGL-Context with window and reading 
//  events. Currently implemented using glfw.
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

#endif //USING_OFL_README_MD

--------------------------------------------------------------------------------

<end_doc>
#endif 
#ifndef USING_OFL_SEM_QUEUE_H
#define USING_OFL_SEM_QUEUE_H
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

namespace ofl{
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
	EC_OK = 0,
	EC_IS_EMPTY = 1,
	EC_TIMEOUT = 2,
	EC_SHUTDOWN = 4,
	EC_SHUTDOWN_AND_EMPTY =5,

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
				return EC_SHUTDOWN;
			}
			else
				return EC_SHUTDOWN_AND_EMPTY;

		}


		*t = q.front();
		q.pop();

		return EC_OK;
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
			return EC_TIMEOUT;
		if (!running)
		{
			if(!q.empty())
			{
				*t = q.front();
				q.pop();
				return EC_SHUTDOWN;
			}
			else
				return EC_SHUTDOWN_AND_EMPTY;
		}

		*t = q.front();
		q.pop();

		return EC_OK;
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
			return EC_IS_EMPTY;
		}
		if (!running)
		{
			if(!q.empty())
			{
				*t = q.front();
				q.pop();
				return EC_SHUTDOWN;
			}
			else
				return EC_SHUTDOWN_AND_EMPTY;
		}

		*t = q.front();
		q.pop();
		return EC_OK;
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
}

#endif //USING_OFL_SEM_QUEUE_H
#ifdef OFL_IMPLEMENTATION
#endif
