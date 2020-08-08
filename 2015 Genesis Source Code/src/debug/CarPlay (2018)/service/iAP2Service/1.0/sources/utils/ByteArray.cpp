/*
    File:       ByteArray.cpp
    Package:    LG iAP2 Service
    Author:     Yongil Park (yongil.park@lge.com)
    Copyright © [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (“Software”) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    to the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (“Authorized License”). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#include "ByteArray.h"
#include "iAp2ServiceLog.h"

namespace IAP2
{
#define NOT_FREED_MEMORY_SIZE 1024

ByteArray::ByteArray() : mpByteArray(NULL), mAllocSize(0), mUsedSize(0)
{
	mpByteArray = new char[1];
	mUsedSize = 0;

	if (mpByteArray == NULL)
	{
		LOGE("ERROR : mpByteArray is NULL. out of memory");
		mAllocSize = 0;
		return;
	}

	mpByteArray[0] = 0;
	mAllocSize = 1;
}

ByteArray::ByteArray(const ByteArray& rByteArray)  : mpByteArray(NULL), mAllocSize(0), mUsedSize(0)
{
    ByteArray& refByteArray = (ByteArray&)rByteArray;
    const char* pByteArray = refByteArray.constData();

	if (pByteArray == NULL)
	{
		LOGE("ERROR : pByteArray is NULL");
        return;
	}

    mUsedSize = refByteArray.length();
	mAllocSize = mUsedSize + 1;
	
	
	mpByteArray = new char[mAllocSize];

	if (mpByteArray == NULL)
	{
		LOGE("ERROR : mpByteArray is NULL");
		mUsedSize = 0;
		mAllocSize = 0;		
        return;
	}

	memcpy(mpByteArray, pByteArray, mAllocSize);
}

ByteArray::ByteArray(char* pStr, int len)  : mpByteArray(NULL), mAllocSize(0), mUsedSize(0)
{
	if (pStr== NULL || len <= 0)
	{
		LOGE("ERROR : append() pStr[%p], len[%d]", pStr, len);
		ByteArray();
		return;
	}
	
	mpByteArray = new char[len+1];
	if (mpByteArray == NULL)
	{
		LOGE("ERROR : mpByteArray is NULL. out of memory");
		mAllocSize = 0;
		mUsedSize = 0;
		return;
	}

	memcpy(mpByteArray, pStr, len);
	mpByteArray[len] = 0;

	mUsedSize = len;
	mAllocSize = mUsedSize+1;
}


ByteArray::~ByteArray()
{
	if (mpByteArray == NULL)
	{
		LOGE("ERROR : mpByteArray is NULL");
		return;
	}

	delete[] mpByteArray;
}

ByteArray& ByteArray::operator=(ByteArray& rByteArray)
{
	const char* pByteArray = rByteArray.constData();

	if (pByteArray == NULL)
	{
		LOGE("ERROR : pByteArray is NULL");
		return *this;
	}

	delete[] mpByteArray;

	mUsedSize = rByteArray.length();
	mAllocSize = mUsedSize + 1;

	mpByteArray = new char[mAllocSize];

	if (mpByteArray == NULL)
	{
		LOGE("ERROR : mpByteArray is NULL");
		mUsedSize = 0;
		mAllocSize = 0;		
		return *this;
	}

	memcpy(mpByteArray, pByteArray, mAllocSize);
	
	return *this;
}

ByteArray& ByteArray::operator=(const ByteArray& rByteArray)
{
#if 1
    ByteArray& refByteArray = (ByteArray&)rByteArray;
    const char* pByteArray = refByteArray.constData();

	if (pByteArray == NULL)
	{
		LOGE("ERROR : pByteArray is NULL");
		return *this;
	}

	delete[] mpByteArray;

    mUsedSize = refByteArray.length();
    mAllocSize = mUsedSize + 1;

	mpByteArray = new char[mAllocSize];

	if (mpByteArray == NULL)
	{
		LOGE("ERROR : mpByteArray is NULL");
		mUsedSize = 0;
		mAllocSize = 0;		
		return *this;
	}

	memcpy(mpByteArray, pByteArray, mAllocSize);
#endif
	return *this;
}

void ByteArray::clear()
{
	mUsedSize = 0;

	if (mpByteArray == NULL)
	{
		LOGE("ERROR : mpByteArray is NULL");
		return;
	}

	if (mAllocSize > NOT_FREED_MEMORY_SIZE)
	{
		delete[] mpByteArray;
		
		mpByteArray = new char[1];

		if (mpByteArray == NULL)
		{
			mAllocSize = 0;
			LOGE("ERROR : mpByteArray is NULL");
			return;
		}
		
		mAllocSize = 1;
	}		
}

int ByteArray::length()
{
	return mUsedSize;
}

int ByteArray::count()
{
	return mUsedSize;
}

ByteArray& ByteArray::append(char* pStr)
{
	int len = 0;
	if (pStr== NULL)
	{
		LOGE("WARNING : append() pStr is NULL");
		return *this;
	}

	//LOGD("append() mpByteArray[%p] mAllocSize[%d] mUsedSize[%d]", mpByteArray, mAllocSize, mUsedSize);

	if (mpByteArray == NULL)
	{
		LOGE("ERROR : mpByteArray is NULL");
		return *this;
	}

	len = strlen(pStr);
	if (len == 0)
	{
		LOGE("WARNING : len is 0");
		return *this;
	}
	
	return append(pStr, len);
}

ByteArray& ByteArray::append(char* pStr, int len)
{
	if (pStr== NULL || len <= 0)
	{
		LOGE("WARNING : append() pStr[%p], len[%d]", pStr, len);
		return *this;
	}

	//LOGD("append() mpByteArray[%p] mAllocSize[%d] mUsedSize[%d]", mpByteArray, mAllocSize, mUsedSize);

	if (mpByteArray == NULL)
	{
		LOGE("ERROR : mpByteArray is NULL");
		return *this;
	}

	if (len >= (mAllocSize - mUsedSize) )
	{
		char* pByteArray = new char[mUsedSize+len+1];
		if (pByteArray == NULL)
		{
			LOGE("ERROR : pByteArray is NULL");
			mAllocSize = 0;
			return *this;
		}

		memcpy(pByteArray, mpByteArray, mUsedSize);
		memcpy(pByteArray + mUsedSize, pStr, len);
		pByteArray[mUsedSize+len] = 0;
		mUsedSize += len ;
		mAllocSize = mUsedSize+1;

		delete[] mpByteArray;
		mpByteArray = pByteArray;		
	}
	else
	{
		memcpy(mpByteArray + mUsedSize, pStr, len);
		mpByteArray[mUsedSize+len] = 0;
		mUsedSize += len ;
	}

	return *this;
}

ByteArray& ByteArray::append(ByteArray &rData)
{
	char* pStr = NULL;
	pStr = rData.data();
		
    //LOGD("append() pStr[%p] length[%d]", pStr, rData.length());

    return append(pStr, rData.length());
}

ByteArray& ByteArray::append(char ch)
{
    return append(&ch, 1);
}

void ByteArray::resize(int size)
{
	//LOGD("resize() mpByteArray[%p] mAllocSize[%d] mUsedSize[%d] size[%d]", mpByteArray, mAllocSize, mUsedSize, size);
	
	if (mpByteArray == NULL)
	{
		LOGE("ERROR : pByteArray is NULL");
		return;
	}

	//if (size >= (mAllocSize - mUsedSize) )
	if (size > mUsedSize )
	{
        //char* pByteArray = new char[mUsedSize+size+1];
        char* pByteArray = new char[size+1];
		if (pByteArray == NULL)
		{
			LOGE("ERROR : pByteArray is NULL");
			mAllocSize = 0;
			return;
		}

        memcpy(pByteArray, mpByteArray, mUsedSize);
        pByteArray[size] = 0;
		mUsedSize = size ;
		mAllocSize = mUsedSize+1;

		delete[] mpByteArray;
		mpByteArray = pByteArray;		
	}
	else
	{
        mpByteArray[size] = 0;
		mUsedSize = size;
	}

}

const char* ByteArray::constData()
{
	if (mpByteArray == NULL)
		LOGE("ERROR : pByteArray is NULL");

	return mpByteArray;
}

char* ByteArray::data()
{
	if (mpByteArray == NULL)
		LOGE("ERROR : pByteArray is NULL");

	return mpByteArray;
}

ByteArray ByteArray::mid (int pos)
{
	return mid(pos, -1);
}

ByteArray ByteArray::mid (int pos, int len)
{
	ByteArray retByteArray;

	if (mpByteArray == NULL)
	{
		LOGE("ERROR : pByteArray is NULL");
		return retByteArray;
	}

	if ( (len == -1) || (pos + len > mUsedSize) )
	{
		retByteArray.append( &mpByteArray[pos], mUsedSize);
	}
	else
	{
		retByteArray.append( &mpByteArray[pos], len);
	}
		
	return retByteArray;
}

void ByteArray::reserve (int size)
{
	//LOGD("reserve() mpByteArray[%p] mAllocSize[%d] size[%d]", mpByteArray, mAllocSize, size);

	if (mpByteArray == NULL)
	{
		LOGE("ERROR : mpByteArray is NULL");
        return;
	}

	if (size <= mAllocSize )
		return;

	char* pByteArray = new char[size+1];
	
	if (pByteArray == NULL)
	{
		LOGE("ERROR : pByteArray is NULL");
		return;
	}

    mAllocSize = size+1;
	memcpy(pByteArray, mpByteArray, mUsedSize);
	pByteArray[mUsedSize] = 0;
	
	delete[] mpByteArray;
	mpByteArray = pByteArray;		
}

}// namespace IAP2

