#pragma once

#include <initializer_list>
#include <iostream>



//template<typename Vector>
//struct BaseIterator
//{
//public:
//	//using ValueType = typename ;
//	using ValueType = typename Vector::ValueType;
//	using PointerType = ValueType*;
//	using ReferenceType = ValueType&;
//	
//
//	PointerType _Pointer;
//};


//template<typename Vector>
//class VectorConstIterator// : public BaseIterator<Vector>
//{
//public:
//	using ValueType =  typename Vector::ValueType;
//	using PointerType = ValueType *;
//	using ReferenceType = ValueType&;
//
//	VectorConstIterator() : _Pointer(nullptr)
//	{
//	}
//	VectorConstIterator(PointerType conteiner) : _Pointer(conteiner)
//	{
//	}
//	VectorConstIterator(VectorConstIterator<Vector>& conteiner)
//	{
//		_Pointer = conteiner._Pointer;
//	}
//	VectorConstIterator& operator++()
//	{
//		++_Pointer;
//		return *this;
//	}
//	VectorConstIterator operator++(int)
//	{
//		VectorConstIterator it(*this);
//		++_Pointer;
//		return it;
//	}
//	VectorConstIterator& operator--()
//	{
//		--_Pointer;
//		return *this;
//	}
//	VectorConstIterator operator--(int)
//	{
//		VectorConstIterator it(*this);
//		--_Pointer;
//		return it;
//	}
//
//	VectorConstIterator& operator+=(size_t step)
//	{
//		_Pointer += step;
//		return *this;
//	}
//	VectorConstIterator& operator-=(size_t step)
//	{
//		_Pointer -= step;
//		return *this;
//	}
//	VectorConstIterator operator+(size_t step) const
//	{
//		VectorConstIterator it(*this);
//		return it._Pointer += step;
//	}
//	VectorConstIterator operator-(size_t step) const
//	{
//		VectorConstIterator it(*this);
//		return it._Pointer -= step;
//	}
//	const ReferenceType operator*() const
//	{
//		return *_Pointer;
//	}
//	const PointerType operator->() const
//	{
//		return _Pointer;
//	}
//	bool operator==(const VectorConstIterator& it) const
//	{
//		return _Pointer == it._Pointer;
//	}
//	bool operator!=(const VectorConstIterator& it) const
//	{
//		return _Pointer != it._Pointer;
//	}
//	/*const ReferenceType operator=(const VectorConstIterator& otherIT)
//	{
//		_Pointer = otherIT._Pointer;
//	}*/
//
//	ReferenceType operator[](size_t offset)
//	{
//		return *(*this + offset);
//	}
//
//public:
//
//	PointerType _Pointer;
//};


template<typename Vector>
class VectorIterator //: public VectorConstIterator<Vector>
{
public:
	using ValueType = typename Vector::ValueType;
	using PointerType = ValueType*;
	using ReferenceType = ValueType&;

	
	VectorIterator()// : VectorConstIterator()
	{
	}
	VectorIterator(PointerType conteiner) : _Pointer(conteiner)// VectorConstIterator<Vector>(conteiner)
	{
	}
	VectorIterator(VectorIterator<Vector>& conteiner)// : VectorConstIterator<Vector>(conteiner)
	{
		_Pointer = (conteiner._Pointer);
	}
	VectorIterator& operator++()
	{
		++_Pointer;
		return *this; 
	}
	VectorIterator operator++(int)
	{
		VectorIterator it(*this);
		++_Pointer;
		return it;
	}
	VectorIterator& operator--()
	{
		--_Pointer;
		return *this;
	}
	VectorIterator operator--(int)
	{
		VectorIterator it(*this);
		--_Pointer;
		return it;
	}

	VectorIterator& operator+=(size_t step)
	{
		_Pointer += step;
		return *this;
	}
	VectorIterator& operator-=(size_t step)
	{
		_Pointer -= step;
		return *this;
	}
	VectorIterator operator+(size_t step) const
	{
		VectorIterator it(*this);
		return it._Pointer += step;
	}
	VectorIterator operator-(size_t step) const
	{
		VectorIterator it(*this);
		return it._Pointer -= step;
	}
	ReferenceType operator*() const
	{ 
		return *_Pointer;
	}
	PointerType operator->() const
	{
		return _Pointer;
	}
	bool operator==(const VectorIterator<Vector>& it) const
	{ 
		return _Pointer == it._Pointer;
	}
	bool operator!=(const VectorIterator<Vector>& it) const
	{
		return _Pointer != it._Pointer;
	}
	//bool operator!= (const std::basic_string<char,std::char_traits<char>,std::allocator<char>>)
public:

	PointerType _Pointer;
};




template<typename T>
class Vector
{
public:
	using ValueType = T;
	using PointType = ValueType*;
	using ReferenceType = ValueType&;
	using Iterator = VectorIterator<Vector<T>>;
	using const_Iterator = VectorIterator<Vector<T const>>;
public:


	Vector();
	Vector(const size_t size);
	Vector(const size_t size, const ValueType value);
	Vector(const Vector& other);
	Vector(Vector&& other);
	Vector(const std::initializer_list<T> listValues);
	~Vector();


	void push_back(ValueType&& value);
	void push_back(const ValueType& value);
	void pop_back();
	template<class... Args>
	T& emplace_back(Args&&... args);
	template<class... Args>
	size_t emplace(const size_t index, Args&&... val);
	

	size_t size();
	const size_t size() const;
	size_t capacity();
	const size_t capacity() const;

	bool empty();
	bool empty() const;
	bool IsValidIndex(size_t index);
	void clear();

	void reserve(size_t space);
	size_t insert(size_t index,const ValueType& value);
	size_t insert(size_t index, ValueType&& value);
	void erase(const size_t index);
	void resize(const size_t Newsize);
	void resize(const size_t Newsize, const ReferenceType val);

	PointType data();
	const T* data() const;
	ReferenceType at(const size_t index);
	const T& at(const size_t index) const;

	Iterator begin();
	const_Iterator begin() const;
	const_Iterator cbegin() const;
	Iterator end();
	const_Iterator end() const;
	const_Iterator cend() const;

	void shrink_to_fit();
	ValueType& front() const;
	ValueType& back() const;
	void swap(Vector<T>& other);

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
	
	size_t m_size;
	size_t m_capacity;
	PointType m_arr;
};

template<typename T>
inline Vector<T>::Vector() : m_size{0}
{
	reserve(2);
}

template<typename T>
Vector<T>::Vector(const size_t space) : m_size{0},m_capacity{space}, m_arr((T*)::operator new(m_capacity * sizeof(T)))
{
}

template<typename T>
inline Vector<T>::Vector(const size_t size, const ValueType value) : m_size{ size }, m_capacity{ size }, m_arr((T*)::operator new(m_capacity * sizeof(T)))
{
	for (int i = 0; i < m_size; i++)
	{
		new(&m_arr[i]) T(value);
	}
}

template<typename T>
inline Vector<T>::Vector(const Vector& other) : m_size{ other.m_size }, m_capacity{ other.m_capacity }, m_arr((T*)::operator new(m_capacity * sizeof(T)))
{
	for (size_t i = 0; i < m_size; i++)
	{
		new(&m_arr[i]) T(other.m_arr[i]);
	}
}

template<typename T>
inline Vector<T>::Vector(Vector&& other) : m_size{ other.m_size }, m_capacity{ other.m_capacity }, m_arr{ other.m_arr }//, m_arr((T*)::operator new(m_capacity * sizeof(T)))//
{
	other.m_arr = nullptr;
	other.m_size = other.m_capacity = 0;
	/*for (size_t i = 0; i < m_size; i++)
	{
		new(&m_arr[i]) T(std::move(other.m_arr[i]));
	}
	other.clear();*/
}

template<typename T>
Vector<T>::Vector(const std::initializer_list<T> listValues) : m_size{ listValues.size()}, m_capacity{listValues.size()},m_arr((T*)::operator new(m_capacity * sizeof(T)))
{
	int i = 0;
	for (auto& el : listValues)
	{
		new(&m_arr[i]) T(std::move(el));
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
	emplace_back(std::move(value));
}

template<typename T>
inline void Vector<T>::push_back(const ValueType& value)
{
	emplace_back(value);
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
	return this->m_size == 0;
}

template<typename T>
inline bool Vector<T>::empty() const
{
	return this->m_size == 0;
}

template<typename T>
inline bool Vector<T>::IsValidIndex(size_t index)
{
	return index < m_size;
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
		::new(newArr + i) T(std::move(m_arr[i]));
	
	for (size_t i = 0; i < m_size; i++)
		m_arr[i].~T();
	::operator delete(m_arr, m_capacity * sizeof(T));

	this->m_arr = newArr;
	this->m_capacity = space;
}

template<typename T>
inline size_t Vector<T>::insert(size_t index, const ValueType& value)
{
	return emplace(index, value);
}

template<typename T>
inline size_t Vector<T>::insert(size_t index, ValueType&& value)
{
	return emplace(index, std::move(value));
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
	if (empty())
		return;
	if (index == m_size - 1)
	{
		return pop_back();
	}

	m_size--;
	for (size_t i = index; i < m_size; i++)
	{
		m_arr[i] = m_arr[i + 1];
	}
	m_arr[m_size].~T();
}

template<typename T>
inline void Vector<T>::resize(const size_t Newsize)
{
	if (Newsize == m_size)
		return;
	if (Newsize > m_size)
	{
		reserve(Newsize);
		for (size_t i = m_size; i < Newsize; i++)
		{
			new(m_arr + i) T();
		}
	}
	else
	{
		for (size_t i = Newsize; i < m_size; i++)
		{
			m_arr[i].~T();
		}
	}
	m_size = Newsize;
}

template<typename T>
inline void Vector<T>::resize(const size_t Newsize, const ReferenceType val)
{
	if (Newsize > m_size)
	{
		reserve(Newsize);
		for (size_t i = m_size; i < Newsize; i++)
		{
			new(m_arr + i) T(std::move(val));
		}
	}
	else
	{
		for (size_t i = Newsize; i < m_capacity; i++)
		{
			m_arr[i] = ~T();
		}
	}
	m_size = Newsize;
}

template<typename T>
inline T& Vector<T>::at(const size_t index) 
{
	if (index >= m_size)
	{
		throw std::exception("Invalid array element");
	}
	return m_arr[index];
}

template<typename T>
inline const T& Vector<T>::at(const size_t index) const
{
	if (index >= m_size)
	{
		throw std::exception("Invalid array element");
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
		Newspace[i] = std::move(m_arr[i]);
	
	for (size_t i = 0; i < m_size; i++)
		m_arr[i].~T();
	::operator delete(m_arr,m_capacity * sizeof(T));

	m_arr = Newspace;
	m_capacity = m_size;
}

template<typename T>
inline T& Vector<T>::front() const
{
#ifdef _DEBUG
	if (empty())
	{
		abort();
	}
#endif // _DEBUG
	return m_arr[0];
}

template<typename T>
inline T& Vector<T>::back() const
{
#ifdef _DEBUG
	if (empty())
	{
		abort();
	}
#endif // _DEBUG

	return m_arr[m_size - 1];
}

template<typename T>
inline void Vector<T>::swap(Vector<T>& other)
{
	T* l_bufferArr = std::move(m_arr);
	size_t l_bufferCap = m_capacity;
	size_t l_bufferSize = m_size;
	
	m_arr = other.m_arr;
	m_capacity = other.m_capacity;
	m_size = other.m_size;

	other.m_arr = l_bufferArr;
	other.m_capacity = l_bufferCap;
	other.m_size = l_bufferSize;
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
	if (*this == other)
		return *this;

	if (m_capacity < other.m_size)
	{
		clear();
		::operator delete(m_arr, m_capacity * sizeof(T));

		m_capacity = other.m_capacity;
		m_arr = (T*)::operator new(m_capacity * sizeof(T));
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
		m_arr = (T*)::operator new(m_capacity * sizeof(T));
	}
	m_size = listValue.size();

	size_t i = 0;
	for (auto& el : listValue)
	{
		new(&m_arr[i]) T(std::move(el));
		i++;
	}

	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept
{
	if (*this == other)
		return *this;
	clear();
	::operator delete(m_arr, m_capacity * sizeof(T));

	m_arr = other.m_arr;
	m_capacity = other.m_capacity;
	m_size = other.m_size;

	other.m_arr = nullptr;
	other.m_size = other.m_capacity = 0;

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
inline T& Vector<T>::emplace_back(Args&& ...args)
{
	if (m_size >= m_capacity)
	{
		reserve((++m_capacity * 1.5));
	}
	::new(&m_arr[m_size]) T(std::forward<Args>(args)...);

	return m_arr[m_size++];
}

template<typename T>
template<class ...Args>
inline size_t Vector<T>::emplace(const size_t index, Args&&... val)
{
#ifdef _DEBUG
	if (index > m_size)
	{
		throw std::exception("Not valid index");
	}
#endif // DEBUG

	if (index == m_size)
	{
		emplace_back(std::move(val)...);
	}
	if (m_size >= m_capacity)
	{
		reserve((++m_capacity * 1.5));		
	}
	
	::new(&m_arr[m_size]) T(std::forward<T>(m_arr[m_size - 1])); // Memory allocation for new element

	for (size_t i = m_size - 1; i > index; i--)
	{
		m_arr[i] = std::move(m_arr[i - 1]);
	}
	++m_size;
	m_arr[index] = T(std::move(val)...);
	

	return index;
}
