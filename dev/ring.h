#include <vector>
#include <cstdint>

namespace ofl {

/**
 * @brief The Ring class implements a simple ring data structure. You can
 * push data into the ring. If the ring is full the oldes element will be over
 * writteb by the new element.
 * The access operators [] and at() will allways work as follows:
 * [0] returns the newest item.
 * [1] the second newest item.
 * [i] the i+1th newest item.
 */
template <typename T>
class Ring
{
protected:
	std::vector<T> m_elements;
	uint32_t m_last;
	uint32_t m_size;
public:
	Ring(const uint32_t size= 0)
	{
		m_size = size;
		m_last = size-1;
		m_elements.resize(size);
	}

	void push(const T& t)
	{
		m_last = (m_last+1)%m_size;
		m_elements[m_last] = t;
	}

	void resize(const uint32_t s)
	{
		m_elements.resize(s);
		m_size = s;
	}

	T& at(const uint32_t id)
	{
		auto r = (m_last+m_size-id)%m_size;
		return m_elements[r];
	}
	const T& at(const uint32_t id)const
	{
		auto r = (m_last+m_size-id)%m_size;
		return m_elements[r];
	}

	T& operator[](const uint32_t id)
	{
		return at(id);
	}
	const T& operator[](const uint32_t id)const
	{
		return at(id);
	}
	uint32_t size()const {return  m_size;}
};
}
