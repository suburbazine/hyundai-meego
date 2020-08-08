#ifndef __NSWCONTAINERS_H
#define __NSWCONTAINERS_H

//Warning disabled because of following arrays:
//-CKeyedContainer<__KEY_TYPE,__VALUE_TYPE,__CAPACITY>::m_UsedKeys
//-CKeyedContainer<__KEY_TYPE,__VALUE_TYPE,__CAPACITY>::m_staticMemory
//-CStaticQueue<__VALUE_TYPE,__CAPACITY>::m_data
//It is only a tip from compiler that array initialization behavior was changed.
//It has no impact on our code.
#pragma warning( disable : 4351 )

#include <new>
#include <stddef.h>
#include <stdint.h>
#include <K2LDebug.h>

//Needed for some embedded C++ compilers (for example GHS V4.0.7)
#ifdef TYPENAME_NOT_USED
#define TYPENAME
#else
#define TYPENAME typename
#endif

// ATTENTION:
// Only works for integral data types like uint8_t etc.
// (By design, because this is what we need here.)
// So don't try to reuse this for object types.
template <typename __VALUE_TYPE, uint32_t __CAPACITY >
class CStaticQueue
{
public:
	typedef TYPENAME __VALUE_TYPE value_type;

	static const uint32_t capacity = __CAPACITY;
private:
	value_type m_data[capacity+1];
	uint32_t m_ReadPosition;
	uint32_t m_WritePosition;

public:
	CStaticQueue()
		: m_data()
		, m_ReadPosition(0UL)
		, m_WritePosition(0UL)
	{
	}

	virtual ~CStaticQueue()
	{
	}

	inline bool empty() const
	{
		return m_ReadPosition == m_WritePosition;
	}

	inline bool full() const
	{
		return m_ReadPosition == next(m_WritePosition);
	}

	// Only stores the value, if !full(). So make sure, you check before you try.
	bool push( value_type value )
	{
		bool bSuccess = false;
		if( !full() )
		{
			m_data[m_WritePosition] = value;
			m_WritePosition = next(m_WritePosition);
			bSuccess = true;
		}
		else
		{
			K2L_DEBUGCHK(false); // TODO: some users want to check full() others want to try...
		}
		return bSuccess;
	}

	// Only works, if !empty(). So check it before you call this function.
	value_type front() const
	{
		K2L_DEBUGCHK(!empty());
		return m_data[m_ReadPosition];
	}

	inline void pop()
	{
		K2L_DEBUGCHK(!empty());
		if( !empty() )
		{
			m_ReadPosition = next(m_ReadPosition);
		}
	}
	inline void clear()
	{
		m_ReadPosition = 0;
		m_WritePosition = 0;
	}
	inline uint32_t size() const
	{
		if( m_WritePosition >= m_ReadPosition )
		{
			return m_WritePosition - m_ReadPosition;
		}
		else
		{
			return (capacity + 1) - m_ReadPosition + m_WritePosition;
		}
	}
private:
	inline uint32_t next( uint32_t current ) const
	{
		K2L_DEBUGCHK( current <= (capacity+1) );
		uint32_t result = current + 1;
		if( result == (capacity+1) )
		{
			result = 0;
		}
		return result;
	}
};

// This is intended to work for value_type of various kind (objects, integral types).
// The key_type are treated as integral types (NO objects!).
// value_type must have a working copy constructor.
template <typename __KEY_TYPE, typename __VALUE_TYPE, __KEY_TYPE __CAPACITY >
class CKeyedContainer
{
public:
	typedef TYPENAME __KEY_TYPE key_type;
	typedef TYPENAME __VALUE_TYPE value_type;
	typedef TYPENAME __KEY_TYPE size_type;
	typedef TYPENAME __VALUE_TYPE & reference;
	typedef TYPENAME const __VALUE_TYPE &const_reference;
	typedef TYPENAME __VALUE_TYPE * pointer;

	static const key_type capacity = __CAPACITY;

private:
	typedef CStaticQueue<key_type, capacity > CKeyQueue;

	CKeyQueue	m_UnusedKeys;
	bool		m_UsedKeys[capacity];
	uint32_t	m_staticMemory[(capacity + 1) * sizeof(value_type)/sizeof(uint32_t)];

	CKeyedContainer(const CKeyedContainer & crOther); // Not available.
	const CKeyedContainer & operator=(const CKeyedContainer & crOther); // Not available.

public:
	CKeyedContainer()
		: m_UnusedKeys()
		, m_UsedKeys()
		, m_staticMemory()
	{
		key_type key;
		for( key = 0; key < capacity; ++key )
		{
			m_UnusedKeys.push(key);
			m_UsedKeys[key] = false;
		}
	}

	virtual ~CKeyedContainer()
	{
		clear();
	}

	void clear()
	{
		key_type key;
		m_UnusedKeys.clear();	// Security "feature": We prefer to reconstruct the queue instead
								// of merely updating it. The cost is nearly the same but
								// it is less error prone.

		for( key = 0; key < capacity; ++key )
		{
			if( m_UsedKeys[key] )
			{
				Destruct( reinterpret_cast<pointer>(m_staticMemory) + key );
				m_UsedKeys[key] = false;
			}
			m_UnusedKeys.push(key);
		}
	}

	inline bool empty() const
	{
		return m_UnusedKeys.full();
	}

	inline bool full() const
	{
		return m_UnusedKeys.empty();
	}

	key_type add( const_reference value )
	{
		K2L_DEBUGCHK(!full());
		if( !full() )
		{
			key_type key = m_UnusedKeys.front();
			m_UnusedKeys.pop();
			K2L_DEBUGCHK( false == m_UsedKeys[key] );
			m_UsedKeys[key] = true;
			UCopy( key, value );
			return key;
		}
		return capacity;
	}

	inline bool exists( key_type key ) const
	{
		return IsUsedKey(key);
	}

	inline const_reference get( key_type key ) const
	{
		K2L_DEBUGCHK(IsUsedKey(key));
		pointer base = reinterpret_cast<pointer>(m_staticMemory);
		return *(base+key);
	}

	inline reference get( key_type key )
	{
		K2L_DEBUGCHK(IsUsedKey(key));
		pointer base = reinterpret_cast<pointer>(m_staticMemory);
		return *(base+key);
	}

	inline void remove( key_type key )
	{
		K2L_DEBUGCHK(IsUsedKey(key));
		if( IsUsedKey(key) )
		{
			pointer base = reinterpret_cast<pointer>(m_staticMemory);
			Destruct( base + key );
			m_UsedKeys[key] = false;
			m_UnusedKeys.push( key );
		}
	}
private:
	inline void Destruct( pointer p )
	{
		K2L_DEBUGCHK(NULL != p); // Paranoia check. Cannot be NULL, since we operate on static memory.
		p->~__VALUE_TYPE();	// The typedefed equivalent value_type does not work on some compilers.
	}
	// UCopy: Uninitialized copy. The destination (where) is a piece of
	// uninitialized memory. We use placement new copy constructor for this
	// operation.
	void UCopy( key_type key, const_reference value )
	{
		pointer base = reinterpret_cast<pointer>(m_staticMemory);
		pointer location = new(base + key) value_type( value );
	}
	inline bool IsUsedKey( key_type key ) const
	{
		return (key < capacity) ? m_UsedKeys[key] : false;
	}
};

#endif
