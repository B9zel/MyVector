#pragma once

#include <initializer_list>
#include <iostream>



#define _TryBegin try {
#define _TryEnd }

#define _CatchAll \
		 catch (...) { 

#define _CatchEnd }

#define _CatchException  \
		catch(const std::exception& excep) {


template<typename Vector>
class VectorIterator
{
public:
	using ValueType = typename Vector::ValueType;
	using PointerType = ValueType*;
	using ReferenceType = ValueType&;

	
	VectorIterator()// : VectorConstIterator()
	{
	}
	VectorIterator(PointerType conteiner) : _Pointer(conteiner)
	{
	}
	VectorIterator(VectorIterator<Vector>& conteiner)
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
class Allocator
{
public:
	
	static_assert(!std::is_const_v<T>, "It is forbidden to store a const element");
	static_assert(!std::is_function_v<T>, "It is forbidden to store a function element");
	static_assert(!std::is_reference_v<T>, "It is forbidden to store a reference element");

	typedef T ValueType;

	ValueType* allocate(const size_t Count)
	{
		try
		{
			return static_cast<T*>(::operator new(Count * sizeof(T)));
		} catch (...)
		{
			throw std::exception("Can't allocate memory");
		}
	}
	void deallocate(T* const Ptr, const size_t Count)
	{
		::operator delete(Ptr, Count * sizeof(T));
	}
};


template<typename T,typename Alloc = Allocator<T>>
class Vector
{
public:
	using ValueType = T;
	using PointType = ValueType*;
	using ReferenceType = ValueType&;
	using Iterator = VectorIterator<Vector<T>>;
	using const_Iterator = VectorIterator<Vector<T const>>;
public:


	Vector() noexcept(std::is_nothrow_default_constructible_v<T>);
	explicit Vector(const Alloc& alloc);
	explicit Vector(const size_t space,const Alloc& Allocator = Alloc());
	template<typename Item>
	Vector(const Item begin,const Item end, const Alloc& Allocator = Alloc());
	Vector(const size_t size, const ValueType value);
	Vector(const Vector& other);
	Vector(Vector&& other);
	Vector(std::initializer_list<T> listValues,const Alloc& Allocator = Alloc());
	~Vector();


	void push_back(ValueType&& value);
	void push_back(const ValueType& value);
	void pop_back();
	template<class... Args>
	T& emplace_back(Args&&... args);
	template<class... Args>
	size_t emplace(const size_t index, Args&&... val);

	template<typename Item>
	void assign(const Item begin, const Item end);
	void assign(const std::initializer_list<T> listValue);
	void assign(size_t Newsize,const T& value);
	

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
	void resize(const size_t Newsize, const T& val);

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
	void swap(Vector& other);

	T& operator[](const size_t el);
	const T& operator[](const size_t el) const;

	Vector& operator=(const Vector& other);
	Vector& operator=(const std::initializer_list<T> listValue);
	Vector& operator=(Vector&& other) noexcept;

	//bool operator<(const Vector& other);
	bool operator==(const Vector& other);
	bool operator==(const Vector& other) const;
	bool operator!=(const Vector& other);
	bool operator!=(const Vector& other) const;
	
private:
	
	size_t m_size;
	size_t m_capacity;
	Alloc Allocat;
	PointType m_arr;

private:

	inline size_t _ChangeGrowth(size_t capacity)
	{
		const size_t Newcapacity = capacity + capacity / 2;
		if (Newcapacity < capacity)
			return capacity;

		return Newcapacity;
	}

};

template<typename T, typename Alloc>
inline Vector<T,Alloc>::Vector() noexcept(std::is_nothrow_default_constructible_v<T>) : m_size{0}, m_capacity{0},m_arr{nullptr}
{
}

template<typename T, typename Alloc>
inline Vector<T, Alloc>::Vector(const Alloc& alloc) : m_size{ 0 }, m_capacity{ 0 }, Allocat{ alloc }, m_arr{ nullptr }
{
}

template<typename T, typename Alloc>
inline Vector<T, Alloc>::Vector(const size_t space, const Alloc& Allocator) : m_size{ 0 }, m_capacity{ space }, Allocat{ Allocator }
{
	_TryBegin

		m_arr = Allocat.allocate(capacity());

	_TryEnd
	_CatchException

		throw excep;

	_CatchEnd
	_CatchAll
		
		throw std::exception("Can't allocate memmory");

	_CatchEnd
}


template<typename T, typename Alloc>
template<typename Item>
inline Vector<T, Alloc>::Vector(const Item begin, const Item end, const Alloc& Allocator) : m_size{ 0 }, m_capacity{ 0 }, Allocat { Allocator }
{
	
	int l_size = static_cast<int>(std::distance(begin, end));
	if (l_size <= 0)
	{
		m_size = 0;
		m_capacity = 0;
		m_arr = nullptr;

		throw std::exception("Invalid range");
	}
	
	_TryBegin
		m_arr = Allocat.allocate(l_size);
	_TryEnd
	_CatchException
		throw excep;
	_CatchEnd
	_CatchAll
		throw std::exception("Can't allocate memory");
	_CatchEnd
	size_t i = 0;
	_TryBegin
		for (; i < size(); ++i)
		{
			new(m_arr + i) T(begin[i]);
		}
	_TryEnd
	_CatchAll
		for (; i > 0; --i)
			m_arr[i].~T();
		Allocat.deallocate(m_arr, l_size);
		throw std::exception("Can't create array");

	_CatchEnd

	m_size = m_capacity = l_size;
}



template<typename T, typename Alloc>
inline Vector<T,Alloc>::Vector(const size_t size, const ValueType value) : m_size{ 0 }, m_capacity{ 0 }
{
	_TryBegin
		m_arr = Allocat.allocate(size);
	_TryEnd
	_CatchException
		throw excep;
	_CatchEnd
	_CatchAll
		throw std::exception("Can't allocate memory");
	_CatchEnd

	size_t i = 0;
	_TryBegin
		for (; i < size; ++i)
		{
			new(&m_arr[i]) T(value);
		}
	_TryEnd
	_CatchAll
		for (; i > 0; --i)
			m_arr[i].~T();
		Allocat.deallocate(m_arr, size);
		
		throw std::exception("Can't create array");
	_CatchEnd

	m_size = m_capacity = size;
}

template<typename T, typename Alloc>
inline Vector<T,Alloc>::Vector(const Vector& other) : m_size{ 0 }, m_capacity{ 0 }
{
	size_t l_cap = other.capacity();
	size_t l_size = other.size();
	_TryBegin
		m_arr = Allocat.allocate(l_cap);
		
	_TryEnd
	_CatchException
		throw excep;
	_CatchEnd
	_CatchAll
		throw std::exception("Can't allocate memory");
	_CatchEnd

	size_t i = 0;
	_TryBegin
		for (; i < l_size; ++i)
		{
			new(m_arr + i) T(other.m_arr[i]);
		}
	_TryEnd
	_CatchAll
		for (; i > 0; --i)
		{
			m_arr[i].~T();
		}
		Allocat.deallocate(m_arr, l_cap);

		throw std::exception("Can't create array");
	_CatchEnd

	m_size = l_size;
	m_capacity = l_cap;
}

template<typename T, typename Alloc>
inline Vector<T, Alloc>::Vector(Vector&& other) : m_size{ other.m_size }, m_capacity{ other.m_capacity }, m_arr{ other.m_arr }, Allocat{ std::move(other.Allocat) }
{
	other.m_arr = nullptr;
	other.m_size = other.m_capacity = 0;
	
}

template<typename T, typename Alloc>
Vector<T, Alloc>::Vector(std::initializer_list<T> listValues, const Alloc& Allocator) : m_size{ 0 }, m_capacity{ 0 }, Allocat{ Allocator }
{
	size_t l_cap = _ChangeGrowth(listValues.size());
	_TryBegin
		m_arr = Allocat.allocate(l_cap);
	_TryEnd
	_CatchException
		throw excep;
	_CatchEnd
	_CatchAll
		throw std::exception("Can't allocate memory");
	_CatchEnd

	size_t i = 0;
	_TryBegin
	for (auto& el : listValues)
	{
		new(m_arr + i) T(std::move(el));
		++i;
	}
	_TryEnd
	_CatchAll

	for (;i > 0; --i)
	{
		m_arr[i].~T();
	}
	Allocat.deallocate(m_arr, l_cap);

	throw std::exception("Can't create array");

	_CatchEnd

	m_size = m_capacity = l_cap;
}

template<typename T, typename Alloc>
inline Vector<T,Alloc>::~Vector()
{
	clear();
	Allocat.deallocate(m_arr, capacity());
}

template<typename T, typename Alloc>
inline void Vector<T,Alloc>::push_back(ValueType&& value)
{
	emplace_back(std::move(value));
}

template<typename T, typename Alloc>
inline void Vector<T,Alloc>::push_back(const ValueType& value)
{
	emplace_back(value);
}

template<typename T, typename Alloc>
inline void Vector<T,Alloc>::pop_back()
{
#ifdef _DEBUG
	if (empty())
	{
		throw std::exception("Array empty before pop_back()");
	}
#endif // _DEBUG	
	
	this->m_size--;
	this->m_arr[m_size].~T();
	
}


template<typename T, typename Alloc>
template<typename Item>
inline void Vector<T, Alloc>::assign(const Item begin, const Item end)
{
	int l_size = static_cast<int>(std::distance(begin, end));
	if (l_size <= 0)
	{
		m_size = 0;
		m_capacity = 0;
		m_arr = nullptr;

		throw std::exception("Invalid range");
	}

	if (capacity() < l_size)
	{
		PointType NewArr = nullptr;

		_TryBegin
			NewArr = Allocat.allocate(l_size);
		_TryEnd
		_CatchException
			throw excep;
		_CatchEnd
		_CatchAll
			throw std::exception("Can't allocate memory");
		_CatchEnd

		size_t i = 0;
		_TryBegin
			for (; i < l_size; ++i)
				new(NewArr + i) T(begin[i]);
			
		_TryEnd
		_CatchAll
			for (; i > 0; --i)
				NewArr[i].~T();
			
			Allocat.deallocate(NewArr, l_size);
			throw std::exception("Something went wrong while recreating the array");
		_CatchEnd

		clear();
		Allocat.deallocate(m_arr, capacity());

		m_arr = NewArr;
		m_size = m_capacity = l_size;
		return;
	}

	size_t i = 0;
	_TryBegin
		if (size() <= l_size)
		{
			for (; i < l_size; ++i)
			{
				new(m_arr + i) T(begin[i]);
			}
		}
		else
		{
			for (; i < l_size; ++i)
			{
				m_arr[i] = begin[i];
			}
		}
		m_size = l_size;
	_TryEnd
	_CatchAll
		for (; i > size(); --i)
		{
			m_arr[i].~T();
		}
		std::exception("Something went wrong while assigning an array, old data is lost");
	_CatchEnd
}




template<typename T, typename Alloc>
inline void Vector<T, Alloc>::assign(const std::initializer_list<T> listValue)
{
	size_t ListSize = listValue.size();
	if (ListSize > capacity())
	{
		PointType NewArr = nullptr;

		_TryBegin
			NewArr = Allocat.allocate(ListSize);
		_TryEnd
			_CatchException
			throw excep;
		_CatchEnd
		_CatchAll
			throw std::exception("Can't allocate memory");
		_CatchEnd

		_TryBegin

			std::uninitialized_copy(listValue.begin(), listValue.end(), NewArr);

		_TryEnd
		_CatchAll

			Allocat.deallocate(NewArr, ListSize);
			throw std::exception("Something went wrong while recreating the array");

		_CatchEnd

		clear();
		Allocat.deallocate(m_arr, capacity());

		m_arr = NewArr;
		m_size = m_capacity = ListSize;

		return;
	}

	_TryBegin
	if (size() <= ListSize)
	{
		for (size_t i = 0; i < ListSize; ++i)
			new(m_arr + i) T(*(listValue.begin() + i));		
	}
	else
	{
		size_t i = 0;
		
		for (; i < ListSize; ++i)
		{
			m_arr[i] = *(listValue.begin() + i);
		}
		for (; i < size(); ++i)
		{
			m_arr[i].~T();
		}
	}
	m_size = ListSize;
	_TryEnd
	_CatchAll

		throw std::exception("Something went wrong while assigning an array, old data is lost");

	_CatchEnd
}

template<typename T, typename Alloc>
inline void Vector<T, Alloc>::assign(size_t Newsize, const T& value)
{
	if (Newsize > capacity())
	{
		PointType Newarr = nullptr;
		_TryBegin

		Newarr = Allocat.allocate(Newsize);

		_TryEnd
		_CatchException

		throw excep;

		_CatchEnd
		_CatchAll

		throw std::exception("Can't allocate memory");

		_CatchEnd

		size_t i = 0;
		_TryBegin
		for (; i < Newsize; ++i)
		{
			new(Newarr + i) T(value);
		}
		_TryEnd
		_CatchAll
			for (; i > 0; --i)
				Newarr[i].~T();
			
			Allocat.deallocate(Newarr, Newsize);
			throw std::exception("Something went wrong while recreating the array");
		_CatchEnd

		clear();
		Allocat.deallocate(m_arr, capacity());

		m_arr = Newarr;
		m_size = m_capacity = Newsize;

		return;
	}
	
	_TryBegin
	if (size() < Newsize)
	{
		m_size = Newsize;
		for (size_t i = 0; i < size(); ++i)
			new(m_arr + i) T(value);
	}
	else
	{

		size_t i = 0;
		for (; i < Newsize; ++i)
		{
			m_arr[i] = value;
		}
		for (;i < size(); ++i)
		{
			m_arr[i].~T();
		}
		m_size = Newsize;
	}
	_TryEnd
	_CatchAll
		throw std::exception("Something went wrong while assigning an array, old data is lost");
	_CatchEnd
}

template<typename T, typename Alloc>
inline size_t Vector<T,Alloc>::size()
{
	return this->m_size;
}

template<typename T, typename Alloc>
inline const size_t Vector<T,Alloc>::size() const
{
	return this->m_size;
}

template<typename T, typename Alloc>
inline size_t Vector<T,Alloc>::capacity()
{
	return this->m_capacity;
}

template<typename T, typename Alloc>
inline const size_t Vector<T,Alloc>::capacity() const
{
	return this->m_capacity;
}

template<typename T, typename Alloc>
inline bool Vector<T,Alloc>::empty()
{
	return size() == 0;
}

template<typename T, typename Alloc>
inline bool Vector<T,Alloc>::empty() const
{
	return size() == 0;
}

template<typename T, typename Alloc>
inline bool Vector<T,Alloc>::IsValidIndex(size_t index)
{
	return index < size();
}

template<typename T, typename Alloc>
inline void Vector<T,Alloc>::clear()
{
	for (size_t i = 0; i < size(); ++i)
	{
		m_arr[i].~T();
	}
	this->m_size = 0;
}

template<typename T, typename Alloc>
inline void Vector<T,Alloc>::reserve(size_t space)
{
	if (space <= capacity()) return;
	
	PointType newArr;
	_TryBegin

		newArr = Allocat.allocate(space);

	_TryEnd
	_CatchException

		throw excep;

	_CatchEnd
	_CatchAll

		throw std::exception("Can't allocate memory");

	_CatchEnd


	_TryBegin

		if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
			std::uninitialized_move(m_arr,m_arr + m_size, newArr);
		else
			std::uninitialized_copy(m_arr, m_arr + m_size, newArr);

	_TryEnd
	_CatchAll

		Allocat.deallocate(newArr, space);
		throw std::exception("Something went wrong while recreating the array");

	_CatchEnd

	for (size_t i = 0; i < size(); ++i)
		m_arr[i].~T();
	Allocat.deallocate(m_arr, capacity());

	this->m_arr = newArr;
	this->m_capacity = space;
}

template<typename T, typename Alloc>
inline size_t Vector<T,Alloc>::insert(size_t index, const ValueType& value)
{
	return emplace(index, value);
}

template<typename T, typename Alloc>
inline size_t Vector<T,Alloc>::insert(size_t index, ValueType&& value)
{
	return emplace(index, std::move(value));
}

template<typename T, typename Alloc>
inline void Vector<T,Alloc>::erase(const size_t index)
{
#ifdef _DEBUG
	if (index >= m_size)
	{
		return;
	}
#endif
	if (empty())
		return;
	if (index == size() - 1)
		return pop_back();
	

	--m_size;
	for (size_t i = index; i < size(); ++i)
	{
		m_arr[i] = m_arr[i + 1];
	}
	m_arr[m_size].~T();
}

template<typename T, typename Alloc>
inline void Vector<T,Alloc>::resize(const size_t Newsize)
{
	resize(Newsize, T());
}

template<typename T, typename Alloc>
inline void Vector<T,Alloc>::resize(const size_t Newsize, const T& val)
{
	_TryBegin
		reserve(_ChangeGrowth(m_capacity + 1));
	_TryEnd
	_CatchException
		throw excep;
	_CatchEnd

	size_t i = size();
	_TryBegin

		for (; i < Newsize; ++i)
			new(m_arr + i) T(val);
		
	_CatchEnd
	_CatchAll
		for (; i >= size(); --i)
			m_arr[i].~T();
		
		throw std::exception("Something went wrong while resize the array");
	_CatchEnd
	
	for (size_t i = Newsize; i < size(); ++i)
	{
		m_arr[i].~T();
	}
	m_size = Newsize;
}

template<typename T, typename Alloc>
inline T& Vector<T,Alloc>::at(const size_t index) 
{
	if (index >= m_size)
	{
		throw std::exception("Invalid array element");
	}
	return m_arr[index];
}

template<typename T, typename Alloc>
inline const T& Vector<T,Alloc>::at(const size_t index) const
{
	if (index >= m_size)
	{
		throw std::exception("Invalid array element");
	}
	return m_arr[index];
}

template<typename T, typename Alloc>
inline typename Vector<T,Alloc>::Iterator Vector<T,Alloc>::begin()
{
	Vector<T,Alloc>::Iterator it(m_arr);
	return it;
}

template<typename T, typename Alloc>
inline typename Vector<T,Alloc>::const_Iterator Vector<T,Alloc>::begin() const
{
	Vector<T,Alloc>::const_Iterator it(m_arr);
	return it;
}

template<typename T, typename Alloc>
inline typename Vector<T,Alloc>::const_Iterator Vector<T,Alloc>::cbegin() const
{
	Vector<T,Alloc>::const_Iterator it(m_arr);
	return it;
}

template<typename T, typename Alloc>
inline typename Vector<T,Alloc>::Iterator Vector<T,Alloc>::end()
{
	Vector<T,Alloc>::Iterator it(m_arr + size());
	return it;
}

template<typename T, typename Alloc>
inline typename Vector<T,Alloc>::const_Iterator Vector<T,Alloc>::end() const
{
	Vector<T,Alloc>::const_Iterator it(m_arr + size());
	return it;
}

template<typename T, typename Alloc>
inline typename Vector<T,Alloc>::const_Iterator Vector<T,Alloc>::cend() const
{
	Vector<T,Alloc>::const_Iterator it(m_arr + size());
	return it;
}

template<typename T, typename Alloc>
inline void Vector<T,Alloc>::shrink_to_fit()
{
	if (capacity() == size()) return;

	PointType Newspace;
	_TryBegin

		Newspace = Allocat.allocate(size());

	_TryEnd 
	_CatchException
	
		throw excep;

	_CatchEnd
	_CatchAll

		throw std::exception("Can't allocate memory");

	_CatchEnd
	

	_TryBegin

		if (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
			std::uninitialized_move(m_arr, m_arr + size(), Newspace);
		else
			std::uninitialized_copy(m_arr, m_arr + size(), Newspace);	
	_TryEnd
	_CatchAll

		Allocat.deallocate(Newspace, size());
		throw std::exception("Something went wrong while recreating the array");

	_CatchEnd

	for (size_t i = 0; i < size(); ++i)
		m_arr[i].~T();
	Allocat.deallocate(m_arr,capacity());

	m_arr = Newspace;
	m_capacity = size();
}

template<typename T, typename Alloc>
inline T& Vector<T,Alloc>::front() const
{
#ifdef _DEBUG
	if (empty())
	{
		throw std::exception("front() called on empty array");
	}
#endif // _DEBUG
	return m_arr[0];
}

template<typename T, typename Alloc>
inline T& Vector<T,Alloc>::back() const
{
#ifdef _DEBUG
	if (empty())
	{
		throw std::exception("back() called on empty array");
	}
#endif // _DEBUG

	return m_arr[size() - 1];
}

template<typename T, typename Alloc>
inline void Vector<T,Alloc>::swap(Vector& other)
{
	T* l_bufferArr = m_arr;
	size_t l_bufferCap = m_capacity;
	size_t l_bufferSize = m_size;
	
	m_arr = other.m_arr;
	m_capacity = other.m_capacity;
	m_size = other.m_size;

	other.m_arr = l_bufferArr;
	other.m_capacity = l_bufferCap;
	other.m_size = l_bufferSize;
}

template<typename T, typename Alloc>
inline T* Vector<T,Alloc>::data()
{
	return m_arr;
}

template<typename T, typename Alloc>
inline const T* Vector<T,Alloc>::data() const
{
	return m_arr;
}

template<typename T, typename Alloc>
inline T& Vector<T,Alloc>::operator[](const size_t el)
{
#ifdef _DEBUG
	if (el >= m_size)
	{
		throw std::exception("Subscript out of range");
	}
#endif // _DEBUG

	return this->m_arr[el];
}

template<typename T, typename Alloc>
inline const T& Vector<T,Alloc>::operator[](const size_t el) const
{
#ifdef _DEBUG
	if (el >= m_size)
	{
		throw std::exception("Subscript out of range");
	}
#endif // _DEBUG
	return this->m_arr[el];
}

template<typename T, typename Alloc>
inline Vector<T,Alloc>& Vector<T,Alloc>::operator=(const Vector& other)
{
	if (this == std::addressof(other))
		return *this;

	assign(other.m_arr, other.m_arr + other.size());

	return *this;
}

template<typename T, typename Alloc>
inline Vector<T,Alloc>& Vector<T,Alloc>::operator=(const std::initializer_list<T> listValue)
{
	assign(listValue);

	return *this;
}

template<typename T, typename Alloc>
inline Vector<T,Alloc>& Vector<T,Alloc>::operator=(Vector&& other) noexcept
{
	if (this == std::addressof(other))
		return *this;

	clear();
	Allocat.deallocate(m_arr, m_capacity);

	m_arr = other.m_arr;
	m_capacity = other.m_capacity;
	m_size = other.m_size;

	other.m_arr = nullptr;
	other.m_size = other.m_capacity = 0;

	return *this;
}

template<typename T, typename Alloc>
inline bool Vector<T,Alloc>::operator==(const Vector& other) 
{
	if (this->m_size != other.m_size)
	{
		return false;
	}

	for (size_t i = 0; i < m_size; ++i)
	{
		if (m_arr[i] != other.m_arr[i])
		{
			return false;
		}
	}
	return true;
}

template<typename T, typename Alloc>
inline bool Vector<T,Alloc>::operator==(const Vector& other) const
{
	return this->operator==(other);
}

template<typename T, typename Alloc>
inline bool Vector<T,Alloc>::operator!=(const Vector& other)
{
	return !(this->operator==(other));
}

template<typename T, typename Alloc>
inline bool Vector<T,Alloc>::operator!=(const Vector& other) const
{
	return !(this->operator==(other));
}


template<typename T, typename Alloc>
template<class ...Args>
inline T& Vector<T,Alloc>::emplace_back(Args&& ...args)
{
	_TryBegin

		if (m_size >= m_capacity)
		{

			reserve(_ChangeGrowth(m_capacity + 1));
		}
		new(m_arr + m_size) T(std::forward<Args>(args)...);

	_TryEnd 
	_CatchException

		throw excep;

	_CatchEnd
	_CatchAll

		throw std::exception("Something went wrong while inserting an element into an array");

	_CatchEnd
	
	return m_arr[m_size++];
}

template<typename T, typename Alloc>
template<class ...Args>
inline size_t Vector<T,Alloc>::emplace(const size_t index, Args&&... val)
{
#ifdef _DEBUG
	if (index > m_size)
	{
		throw std::exception("Not valid index");
	}
#endif // DEBUG

	_TryBegin
		if (index == size())
		{
			emplace_back(std::move(val)...);
			return index;
		}
		if (size() >= capacity())
			reserve(_ChangeGrowth(m_capacity + 1));

	_TryEnd 
	_CatchException
	
		throw excep;

	_CatchEnd

	_TryBegin
		new(m_arr + size()) T(std::forward<T>(m_arr[size() - 1])); // Construct element for offset

		if (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>)
		{
			for (size_t i = m_size - 1; i > index; --i)
				m_arr[i] = std::move(m_arr[i - 1]);
			
			m_arr[index] = T(std::move(val)...);
		}
		else
		{
			for (size_t i = m_size - 1; i > index; --i)
				m_arr[i] = m_arr[i - 1];

			m_arr[index] = T(val...);
		}
	_TryEnd
	_CatchAll

		throw std::exception("Something went wrong while inserting an element into an array");

	_CatchEnd

	++m_size;
	return index;
}