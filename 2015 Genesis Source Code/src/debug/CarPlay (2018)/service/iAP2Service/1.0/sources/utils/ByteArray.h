/*
    File:       ByteArray.h
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

#ifndef BYTEARRAY_H_
#define BYTEARRAY_H_

namespace IAP2
{
    class ByteArray
	{
	private : 
		char* mpByteArray;
		int mAllocSize;
		int mUsedSize;

	public:
		ByteArray();
		ByteArray(const ByteArray& rByteArray); // copy constructor
		ByteArray(char* pStr, int len);
		~ByteArray();

		ByteArray& operator=(ByteArray& rByteArray);
		ByteArray& operator=(const ByteArray& rByteArray);
		//friend ByteArray operator=(ByteArray byteArray);
		//friend ByteArray operator=(ByteArray lByteArray, ByteArray rByteArray);
        void clear();
        int length();
		int count();
		ByteArray& append(char* pStr);
		ByteArray& append(char* pStr, int len);
		ByteArray& append(ByteArray &rData);
		ByteArray& append(char ch);
		
		void resize(int size);
		const char* constData();
		char* data();
#if 0
		void mid(ByteArray& rSrcByteArray, int pos);
		void mid (ByteArray& rSrcByteArray, int pos, int len);

#else
		ByteArray mid (int pos);
        ByteArray mid (int pos, int len);
#endif
		void reserve (int size);


    private:

	};

} // namespace IAP2


#endif // #ifndef BYTEARRAY_H_

