#pragma once
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
