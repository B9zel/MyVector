#pragma once

#include <initializer_list>
#include <iostream>


template<typename Vector>
class BaseIterator 
{
public:
	using ValueType = typename Vector::ValueType;
	using PointerType = ValueType*;
	using ReferenceType = ValueType&;
public:

	BaseIterator() : m_pointer(nullptr) { }
	BaseIterator(PointerType ptr)
	{
		m_pointer = ptr;
	}
	BaseIterator(const ReferenceType it)
	{
		m_pointer = it.m_pointer; 
	}

	BaseIterator& operator++()
	{
		m_pointer++;
		return *this; 
	}
	BaseIterator operator++(int stepForward)
	{
		BaseIterator it = *this;
		m_pointer++;
		return it;
	}
	BaseIterator& operator--()
	{
		m_pointer--; 
		return *this;
	}
	BaseIterator operator--(int stepBack)
	{
		BaseIterator it = *this;
		m_pointer--;
		return it;
	}

	BaseIterator& operator+=(int step)
	{
		m_pointer += step; 
		return *this;
	}
	ValueType& operator*() 
	{ 
		return *m_pointer; 
	}
	PointerType operator->() 
	{
		return m_pointer;
	}

	bool operator==(const BaseIterator& it)
	{ 
		return this->m_pointer == it.m_pointer; 
	}
	bool operator!=(const BaseIterator& it)
	{
		return !(this->operator==(it)); 
	}

private:

	PointerType m_pointer;
};



template<typename T>
class Vector
{
public:
	using ValueType = T;
	using PointType = ValueType*;
	using ReferenceType = ValueType&;
	using Iterator = BaseIterator<Vector<T>>;
	using const_Iterator = BaseIterator<Vector<T const>>;
public:


	Vector();
	Vector(const size_t size);
	Vector(const size_t size, const ValueType value);
	Vector(const Vector& other);
	Vector(const std::initializer_list<T> listValues);
	~Vector();


	void push_back(ValueType&& value);
	void push_back(const ValueType& value);
	void pop_back();
	template<class... Args>
	T& emplace_back(Args&&... args);

	size_t size();
	const size_t size() const;
	size_t capacity();
	const size_t capacity() const;

	bool empty();
	bool empty() const;
	void clear();

	void reserve(size_t space);
	void insert(size_t index, ValueType value);
	void erase(const size_t index);
	void resize(const size_t Newsize);
	void resize(const size_t Newsize, const ReferenceType val);

	T* data();
	const T* data() const;
	T& at(const size_t index);
	const T& at(const size_t index) const;

	Iterator begin();
	const_Iterator begin() const;
	const_Iterator cbegin() const;
	Iterator end();
	const_Iterator end() const;
	const_Iterator cend() const;
	void shrink_to_fit();


	T& operator[](const size_t el);
	const T& operator[](const size_t el) const;

	Vector<T>& operator=(const Vector<T>& other);
	Vector<T>& operator=(const std::initializer_list<T> listValue);
	Vector<T>& operator=(Vector<T>&& other) noexcept;

	bool operator==(const Vector<T>& other);
	bool operator==(const Vector<T>& other) const;
	bool operator!=(const Vector<T>& other);
	bool operator!=(const Vector<T>& other) const;

private:
	PointType m_arr;
	size_t m_size;
	size_t m_capacity;
};

template<typename T>
inline Vector<T>::Vector() : m_arr(nullptr)
{
	this->m_size = 0;
	reserve(2);
}

template<typename T>
Vector<T>::Vector(const size_t size)
{
	this->m_size = size;
	this->m_capacity = size;
	this->m_arr = new T[m_capacity];
}

template<typename T>
inline Vector<T>::Vector(const size_t size, const ValueType value)
{
	this->m_size = size;
	this->m_capacity = size;
	this->m_arr = new ValueType[m_capacity];

	for (int i = 0; i < m_size; i++)
	{
		m_arr[i] = value;
	}
}

template<typename T>
inline Vector<T>::Vector(const Vector& other)
{
	this->m_size = other.m_size;
	this->m_capacity = other.m_capacity;

	this->m_arr = new ValueType[this->m_capacity];

	for (size_t i = 0; i < m_size; i++)
	{
		m_arr[i] = other.m_arr[i];
	}
}

template<typename T>
Vector<T>::Vector(const std::initializer_list<T> listValues)
{
	this->m_capacity = listValues.size();
	this->m_size = m_capacity;

	this->m_arr = new ValueType[m_capacity];

	int i = 0;
	for (auto& el : listValues)
	{
		this->m_arr[i] = el;
		i++;
	}
}


template<typename T>
inline Vector<T>::~Vector()
{
	clear();
	::operator delete(m_arr, m_capacity * sizeof(T));
}

template<typename T>
inline void Vector<T>::push_back(ValueType&& value)
{
	if (m_size >= m_capacity)
	{
		reserve(static_cast<size_t>(m_capacity == 0 ? 2 : m_capacity * 1.5));
	}

	new(&m_arr[m_size++]) T(std::move(value));
}

template<typename T>
inline void Vector<T>::push_back(const ValueType& value)
{
	if (m_size >= m_capacity)
	{
		reserve(m_capacity == 0 ? 2 : m_capacity * 1.5);
	}
	this->m_arr[m_size++] = value;
}

template<typename T>
inline void Vector<T>::pop_back()
{
	if (!empty())
	{
		this->m_size--;
		this->m_arr[m_size].~T();
	}
}

template<typename T>
inline size_t Vector<T>::size()
{
	return this->m_size;
}

template<typename T>
inline const size_t Vector<T>::size() const
{
	return this->m_size;
}

template<typename T>
inline size_t Vector<T>::capacity()
{
	return this->m_capacity;
}

template<typename T>
inline const size_t Vector<T>::capacity() const
{
	return this->m_capacity;
}

template<typename T>
inline bool Vector<T>::empty()
{
	return this->m_size <= 0;
}

template<typename T>
inline bool Vector<T>::empty() const
{
	return this->m_size <= 0;
}

template<typename T>
inline void Vector<T>::clear()
{
	for (size_t i = 0; i < m_size; i++)
	{
		m_arr[i].~T();
	}
	this->m_size = 0;
}

template<typename T>
inline void Vector<T>::reserve(size_t space)
{
	if (space <= this->m_capacity ) return;

	PointType newArr = (T*)::operator new(space * sizeof(T));
	
	for (size_t i = 0; i < m_size; i++)
	{
		//new(&newArr[i]) T(m_arr[i]);
		new(&(*(newArr + i))) T(*(m_arr + i));
	}
	clear();
	::operator delete(this->m_arr, m_capacity * sizeof(T));

	this->m_arr = newArr;
	this->m_capacity = space;
}

template<typename T>
inline void Vector<T>::insert(size_t index, ValueType value)
{
#ifdef _DEBUG 
	if (index > m_size)
	{
		std::cout << "Not valid index";
		throw;
	}
#endif // DEBUG

	if (index == m_size)
	{
		push_back(value);
		return;
	}
	
	if (m_size >= m_capacity)
	{
		reserve(m_capacity == 0 ? 2 : m_capacity * 1.5);
	}
	m_size++;

	ValueType l_value = m_arr[index];
	for (size_t i = index; i < m_size; i++)
	{
		ValueType l_nextValue = m_arr[i + 1];
		m_arr[i + 1] = l_value;
		l_value = l_nextValue;
	}

	m_arr[index] = value;
}


template<typename T>
inline void Vector<T>::erase(const size_t index)
{
#ifdef _DEBUG
	if (index >= m_size)
	{
		return;
	}
#endif
	if (index == m_size - 1)
	{
		pop_back();
	}

	m_size--;
	for (size_t i = index; i < m_size; i++)
	{
		m_arr[i] = m_arr[i + 1];
	}
}

template<typename T>
inline void Vector<T>::resize(const size_t Newsize)
{
	if (Newsize > m_size)
	{
		reserve(Newsize);
		for (size_t i = m_size; i < Newsize; i++)
		{
			m_arr[i] = 0;
		}
		m_size = Newsize;
	}
	else
	{
		for (size_t i = Newsize; i < m_size; i++)
		{
			m_arr[i].~T();
		}
		this->m_size = Newsize;
	}
}

template<typename T>
inline void Vector<T>::resize(const size_t Newsize, const ReferenceType val)
{
	if (Newsize > m_size)
	{
		reserve(Newsize);
		for (size_t i = m_size; i < Newsize; i++)
		{
			m_arr[i] = val;
		}
		m_size = Newsize;
	}
	else
	{
		for (size_t i = Newsize; i < m_capacity; i++)
		{
			m_arr[i] = ~T();
		}
		this->m_size = Newsize;
	}
}

template<typename T>
inline T& Vector<T>::at(const size_t index)
{
	if (index >= m_size)
	{
		throw "Invalid array element";
	}
	return m_arr[index];
}

template<typename T>
inline const T& Vector<T>::at(const size_t index) const
{
	if (index >= m_size)
	{
		throw "Invalid array element";
	}
	return m_arr[index];
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::begin()
{
	Vector<T>::Iterator it(m_arr);
	return it;
}

template<typename T>
inline typename Vector<T>::const_Iterator Vector<T>::begin() const
{
	Vector<T>::const_Iterator it(m_arr);
	return it;
}

template<typename T>
inline typename Vector<T>::const_Iterator Vector<T>::cbegin() const
{
	Vector<T>::const_Iterator it(m_arr);
	return it;
}

template<typename T>
inline typename Vector<T>::Iterator Vector<T>::end()
{
	Vector<T>::Iterator it(m_arr + m_size);
	return it;
}

template<typename T>
inline typename Vector<T>::const_Iterator Vector<T>::end() const
{
	Vector<T>::const_Iterator it(m_arr + m_size);
	return it;
}

template<typename T>
inline typename Vector<T>::const_Iterator Vector<T>::cend() const
{
	Vector<T>::const_Iterator it(m_arr + m_size);
	return it;
}

template<typename T>
inline void Vector<T>::shrink_to_fit()
{
	if (m_capacity == m_size) return;

	PointType Newspace = (T*)::operator new(m_size * sizeof(T));

	for (size_t i = 0; i < m_size; i++)
	{
		Newspace[i] = std::move(m_arr[i]);
	}
	clear();
	::operator delete(m_arr,m_capacity * sizeof(T));

	m_arr = Newspace;
	m_capacity = m_size;
}

template<typename T>
inline T* Vector<T>::data()
{
	return m_arr;
}

template<typename T>
inline const T* Vector<T>::data() const
{
	return m_arr;
}

template<typename T>
inline T& Vector<T>::operator[](const size_t el)
{
#ifdef _DEBUG
	if (el >= m_size)
	{
		abort();
	}
#endif // _DEBUG

	return this->m_arr[el];
}

template<typename T>
inline const T& Vector<T>::operator[](const size_t el) const
{
#ifdef _DEBUG
	if (el >= m_size)
	{
		abort();
	}
#endif // _DEBUG
	return this->m_arr[el];
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (m_capacity < other.m_size)
	{
		clear();
		::operator delete(m_arr, m_capacity * sizeof(T));

		m_capacity = other.m_capacity;
		m_arr = new T[m_capacity];
	}

	this->m_size = other.m_size;

	for (size_t i = 0; i < m_size; i++)
	{
		m_arr[i] = other.m_arr[i];
	}

	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const std::initializer_list<T> listValue)
{
	if (listValue.size() > m_capacity)
	{
		clear();
		::operator delete(m_arr, m_capacity * sizeof(T));
		m_capacity = listValue.size();
		m_arr = new ValueType[m_capacity];
	}
	m_size = listValue.size();

	size_t i = 0;
	for (auto& el : listValue)
	{
		m_arr[i] = el;
		i++;
	}

	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept
{
	this->operator=(other);
	other.clear();

	return *this;
}

template<typename T>
inline bool Vector<T>::operator==(const Vector<T>& other)
{
	if (this->m_size != other.m_size)
	{
		return false;
	}

	for (size_t i = 0; i < m_size; i++)
	{
		if (m_arr[i] != other.m_arr[i])
		{
			return false;
		}
	}
	return true;
}

template<typename T>
inline bool Vector<T>::operator==(const Vector<T>& other) const
{
	return this->operator==(other);
}

template<typename T>
inline bool Vector<T>::operator!=(const Vector<T>& other)
{
	return !(this->operator==(other));
}

template<typename T>
inline bool Vector<T>::operator!=(const Vector<T>& other) const
{
	return !(this->operator==(other));
}

template<typename T>
template<class ...Args>
inline T& Vector<T>::emplace_back(Args && ...args)
{
	if (m_size >= m_capacity)
	{
		reserve(static_cast<size_t>(m_capacity == 0 ? 2 : m_capacity * 1.5));
	}
	new(&m_arr[m_size]) T(std::forward<Args>(args)...);

	return m_arr[m_size++];
}