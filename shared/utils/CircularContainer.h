#pragma once
#include <stdint.h>
#include <iterator>

template<typename T, uint32_t arraySize>
class FixedCircularIterator : public std::iterator<std::random_access_iterator_tag, T>
{
public:

    FixedCircularIterator(T* start = nullptr)
	{
		m_start = start;
		m_ptr = start;
	}
    FixedCircularIterator(const FixedCircularIterator<T, arraySize>& that) = default;
    ~FixedCircularIterator(){}
    FixedCircularIterator<T, arraySize>& operator=(const FixedCircularIterator<T, arraySize>& that) = default;

    operator bool()const {return m_start ? true : false;}

    bool operator==(const FixedCircularIterator<T, arraySize>& that)const{return m_ptr == that.getConstPtr();}
    bool operator!=(const FixedCircularIterator<T, arraySize>& that)const{return m_ptr != that.getConstPtr();}

    FixedCircularIterator<T, arraySize>& operator+=(const ptrdiff_t& movement)
	{
		m_ptr = (m_ptr - m_start + movement) % arraySize + m_start;
		return (*this);
	}
	
    FixedCircularIterator<T, arraySize>& operator-=(const ptrdiff_t& movement)	
	{
		m_ptr = (m_ptr - m_start - movement + arraySize) % arraySize + m_start;
		return (*this);
	}	
	
    FixedCircularIterator<T, arraySize>& operator+=(const int32_t& movement)
	{
		return (*this)+=(ptrdiff_t)movement;
	}
	
    FixedCircularIterator<T, arraySize>& operator-=(const int32_t& movement)	
	{
		return (*this)-=(ptrdiff_t)movement;
	}
	
    FixedCircularIterator<T, arraySize>& operator++()
	{
		*this+=1;
		return (*this);
	}
	
    FixedCircularIterator<T, arraySize>& operator--()
	{
		*this-=1;
		return (*this);
	}
	
    FixedCircularIterator<T, arraySize> operator++(int)
	{
		auto temp(*this);
		++*this;
		return temp;
	}
	
    FixedCircularIterator<T, arraySize> operator--(int)
	{
		auto temp(*this);
		--*this;
		return temp;
	}
	
    FixedCircularIterator<T, arraySize> operator+(const ptrdiff_t& movement)
	{
		auto temp(*this);
		temp+=movement;
		return temp;
	}
	
    FixedCircularIterator<T, arraySize> operator-(const ptrdiff_t& movement)
	{
		auto temp(*this);
		temp-=movement;
		return temp;
	}
	
    FixedCircularIterator<T, arraySize> operator+(const uint32_t& movement)
	{
		return (*this) + (ptrdiff_t)movement;
	}
	
    FixedCircularIterator<T, arraySize> operator-(const uint32_t& movement)
	{
		return (*this) - (ptrdiff_t)movement;
	}	
	
    FixedCircularIterator<T, arraySize> operator+(const int32_t& movement)
	{
		return (*this) + (ptrdiff_t)movement;
	}
	
    FixedCircularIterator<T, arraySize> operator-(const int32_t& movement)
	{
		return (*this) - (ptrdiff_t)movement;
	}

    T& operator*(){return *m_ptr;}
    const T& operator*()const{return *m_ptr;}
    T* operator->()const{return m_ptr;}

    T* getPtr()const{return m_ptr;}
    const T* getConstPtr()const{return m_ptr;}

private:

    T* m_start;
    T* m_ptr;
	static const size_t m_rawLength = arraySize * sizeof(T);
};

template <typename T, uint32_t arraySize>
class CircularContainer
{
public:
	CircularContainer();
	
	T array[arraySize];
	uint32_t cursor;
	uint32_t total;
	uint32_t size;
	
	typedef FixedCircularIterator<T, arraySize> iterator;
    typedef FixedCircularIterator<const T, arraySize> const_iterator;
	
    iterator leftEdge(){return iterator(&array[0]);}
    iterator begin()
	{
		if (total >= arraySize)
			return leftEdge() + (cursor + 1); //skip one element to avoid begin() == end() 
		else
			return leftEdge();
	}
	
    iterator end()
	{
		return leftEdge() + cursor;
	}
	
	static uint32_t diff(const iterator& a, const iterator& b);
	
	template <class... Args>
	void add(Args&&... args);
};

template <typename T, uint32_t arraySize>
CircularContainer<T, arraySize>::CircularContainer()
{
	cursor = 0;
	total = 0;
	size = arraySize;	
}

template <typename T, uint32_t arraySize>
template <class... Args>
void CircularContainer<T, arraySize>::add(Args&&... args)
{
	array[cursor] = T(std::forward<Args>(args)...);
	cursor = (cursor + 1) % arraySize;
	total++;
}

template <typename T, uint32_t arraySize>
uint32_t CircularContainer<T, arraySize>::diff(const iterator& a, const iterator& b)
{
	if (a.getConstPtr() >= b.getConstPtr())
		return (a.getConstPtr() - b.getConstPtr());
	else
		return (a.getConstPtr() - b.getConstPtr()) + arraySize;
}