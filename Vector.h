#pragma once

#include <initializer_list>
#include <iostream>

template<typename T>
class Vector
{
public:

	class Iterator;

	Vector();
	Vector(const size_t size);
	Vector(const size_t size, const T value);
	Vector(const Vector& other);
	Vector(const std::initializer_list<T> listValues);
	~Vector();


	void push_back(T value);
	void pop_back();
	size_t size();
	const size_t size() const;
	size_t capacity();
	const size_t capacity() const;
	bool empty();
	bool empty() const;
	void clear();
	void reserve(size_t space);
	void insert(size_t index, T value);
	
	void erase(const size_t index);
	void resize(const size_t Newsize);
	void resize(const size_t Newsize, const T& val);
	T* data();
	const T* data() const;
	T& at(const size_t index);
	const T& at(const size_t index) const;
	Vector<T>::Iterator begin();
	const Vector<T>::Iterator begin() const;
	const T* cbegin() const;
	T* end();
	const T* end() const;
	const T* cend() const;
	void shrink_to_fit();


	void print();

	T& operator[](const size_t el);
	const T& operator[](const size_t el) const;

	Vector<T>& operator=(const Vector<T>& other);
	Vector<T>& operator=(const std::initializer_list<T> listValue);

	bool operator==(const Vector<T>& other);
	bool operator==(const Vector<T>& other) const;
	bool operator!=(const Vector<T>& other);
	bool operator!=(const Vector<T>& other) const;


	
	class Iterator
	{
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
	public:

		Iterator() : m_pointer(nullptr) { }
		Iterator(pointer ptr) { m_pointer = ptr; }
		Iterator(Vector<T>::Iterator&& it) { m_pointer = it.m_pointer; }
		Iterator& operator=(Vector<T>::Iterator* it) { m_pointer = it->m_pointer; return *this; }
		
		Iterator& operator++() { m_pointer++; return *this; }
		value_type operator++(int stepForward) { value_type value = *m_pointer; m_pointer--; return value; }
		Iterator& operator--() { m_pointer++; return *this; }
		value_type operator--(int stepBack) { value_type value = *m_pointer; m_pointer--; return value; }
		Iterator& operator+=(int step) { m_pointer += step; return *this; }
		reference operator*() {	return *m_pointer;}
		Iterator* operator->(){	return this;}

		
	private:
		
		pointer m_pointer;
	};

private:
	T* m_arr;
	size_t m_size;
	size_t m_capacity;
};

template<typename T>
inline Vector<T>::Vector() : m_arr(nullptr)
{
	this->m_size = 0;
	this->m_capacity = 0;
}

template<typename T>
Vector<T>::Vector(const size_t size)
{
	this->m_size = size;
	this->m_capacity = size;
	this->m_arr = new T[m_capacity];
}

template<typename T>
inline Vector<T>::Vector(const size_t size, const T value)
{
	this->m_size = size;
	this->m_capacity = size;
	this->m_arr = new T[m_capacity];

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

	this->m_arr = new T[this->m_capacity];

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

	this->m_arr = new T[m_capacity];

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
	delete[] this->m_arr;
}

template<typename T>
inline void Vector<T>::push_back(T value)
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
	if (space <= this->m_capacity) return;

	T* newArr = nullptr;
	try
	{
		newArr = new T[space];
	}
	catch (const std::exception& el )
	{
		std::cout << el.what() << std::endl;
		abort();
	}
	
	for (size_t i = 0; i < m_size; i++)
	{
		newArr[i] = m_arr[i];
	}
	delete[] this->m_arr;

	this->m_arr = newArr;
	this->m_capacity = space;
}

template<typename T>
inline void Vector<T>::insert(size_t index, T value)
{
	if (index == m_size)
	{
		push_back(value);
		return;
	}
	if (index > m_size)
	{
		std::cout << "Not valid index";
		throw;
	}
	if (m_size >= m_capacity)
	{
		reserve(m_capacity == 0 ? 2 : m_capacity * 1.5);
	}
	m_size++;

	T l_value = m_arr[index];
	for (size_t i = index; i < m_size; i++)
	{
		T l_timeValue = m_arr[i + 1];
		m_arr[i + 1] = l_value;
		l_value = l_timeValue;
	}

	m_arr[index] = value;
}

template<typename T>
inline void Vector<T>::erase(const size_t index)
{
	if (index >= m_size)
	{
		return;
	}
	if (index == m_size - 1)
	{
		pop_back();
	}
	
	for (size_t i = 0; i < m_size - 1; i++)
	{
		m_arr[index] = m_arr[index + 1];
	}
	m_size--;
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
inline void Vector<T>::resize(const size_t Newsize, const T& val)
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
inline const typename Vector<T>::Iterator Vector<T>::begin() const
{
	Vector<T>::Iterator it(m_arr);
	return it;
}

template<typename T>
inline const T* Vector<T>::cbegin() const
{
	return m_arr;
}

template<typename T>
inline T* Vector<T>::end()
{
	return m_arr + m_size;
}

template<typename T>
inline const T* Vector<T>::end() const
{
	return m_arr + m_size;
}

template<typename T>
inline const T* Vector<T>::cend() const
{
	return m_arr + m_size;
}

template<typename T>
inline void Vector<T>::shrink_to_fit()
{
	if (m_capacity == m_size) return;

	m_capacity = m_size;
	T* Newspace = new T[m_capacity];

	for (size_t i = 0; i < m_size; i++)
	{
		Newspace[i] = m_arr[i];
	}
	delete[] m_arr;

	m_arr = Newspace;
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
inline void Vector<T>::print()
{
	for (size_t i = 0; i < m_size; i++)
	{
		std::cout << m_arr[i] << " ";
	}
}

template<typename T>
inline T& Vector<T>::operator[](const size_t el)
{
	if (el >= m_size)
	{
		abort();
	}
	return this->m_arr[el];
}

template<typename T>
inline const T& Vector<T>::operator[](const size_t el) const
{
	if (el >= m_size)
	{
		abort();
	}
	return this->m_arr[el];
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this->m_arr != nullptr)
	{
		delete[] this->m_arr;
	}

	this->m_size = other.m_size;
	this->m_capacity = other.m_capacity;

	this->m_arr = new T[this->m_capacity];

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
		delete[] m_arr;
		m_capacity = listValue.size();
		m_arr = new T[m_capacity];
	}
	else
	{
		this->clear();
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