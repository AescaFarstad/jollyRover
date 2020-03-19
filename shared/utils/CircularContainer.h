#pragma once
#include <stdint.h>
#include <iterator>
#include <iostream>

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
			return leftEdge() + (cursor + 1 + m_skipFirst + m_skipLast); //skip one element to avoid begin() == end() 
		else
			return leftEdge() + m_skipFirst;
	}
	
    iterator end()
	{
		return leftEdge() + cursor;
	}
	
	static uint32_t diff(const iterator& a, const iterator& b);
	
	template <class... Args>
	void add(Args&&... args);
	void skipFirst(uint32_t count);
	void skipLast(uint32_t count);
	
private:
	uint32_t m_skipFirst;
	uint32_t m_skipLast;
};


template <typename T, uint32_t arraySize>
CircularContainer<T, arraySize>::CircularContainer()
{
	cursor = 0;
	total = 0;
	size = arraySize;
	m_skipFirst = 0;
	m_skipLast = 0;
}

template <typename T, uint32_t arraySize>
template <class... Args>
void CircularContainer<T, arraySize>::add(Args&&... args)
{
	
	array[cursor] = T(std::forward<Args>(args)...);
	cursor = (cursor + 1) % arraySize;
	total++;
	if (m_skipLast > 0)
		m_skipLast--;
	else if (total > arraySize && m_skipFirst > 0)
		m_skipFirst--;
}

template <typename T, uint32_t arraySize>
void CircularContainer<T, arraySize>::skipFirst(uint32_t count)
{
	m_skipFirst += count;
	
	if (total < count || m_skipFirst + m_skipLast >= arraySize)
	{
		std::cout << "Removing too many elements.\n";
		abort();
	}
}

template <typename T, uint32_t arraySize>
void CircularContainer<T, arraySize>::skipLast(uint32_t count)
{
	m_skipLast += count;
	cursor = (cursor - count + arraySize) % arraySize;
	
	if (total < count || m_skipFirst + m_skipLast >= arraySize)
	{
		std::cout << "Removing too many elements.\n";
		abort();
	}
}

template <typename T, uint32_t arraySize>
uint32_t CircularContainer<T, arraySize>::diff(const iterator& a, const iterator& b)
{
	if (a.getConstPtr() >= b.getConstPtr())
		return (a.getConstPtr() - b.getConstPtr());
	else
		return (a.getConstPtr() - b.getConstPtr()) + arraySize;
}

template <uint32_t arraySize>
std::string traceCircularContainer(CircularContainer<int32_t, arraySize>& cc)
{
	std::string result = "|";
	
	for(auto i = 0; i < arraySize; i++)
	{
		std::string cell = "";
		cell += " " + std::to_string(cc.array[i]);
		if (&*cc.begin() == &cc.array[i])
			cell+= "b";			
		else if (&*cc.end() == &cc.array[i])
			cell+= "e";
		else
			cell += " ";
		
		bool iterable = false;
		for(auto& item : cc)
		{
			if (&item == &cc.array[i])
			{
				iterable = true;
				break;
			}
		}
		if (iterable)
			result += " " + cell + " ";
		else
			result += "[" + cell + "]";
	}
	return result;
}

#include <Serialization.h>

namespace Serialization
{
	
	//CircularContainer-------------------------------------------------------
	
	template <typename C, int32_t N, typename T>
	void write(const CircularContainer<C, N>& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, cursor);
		WRITE_FIELD(object, serializer, total);
		WRITE_FIELD(object, serializer, size);
		int32_t writeLength = std::min(sizeof(object.array), (long unsigned int)object.total);
		serializer.writeArray(object.array, writeLength, FIELD_NAME(array));
	}
	
	template <typename C, int32_t N, typename T>
	void read(CircularContainer<C, N>& object, T& serializer)
	{
		READ__FIELD(object, serializer, cursor);
		READ__FIELD(object, serializer, total);
		READ__FIELD(object, serializer, size);
		int32_t readLength = std::min(sizeof(object.array), (long unsigned int)object.total);
		serializer.readArray(object.array, readLength, FIELD_NAME(array));
	}
}