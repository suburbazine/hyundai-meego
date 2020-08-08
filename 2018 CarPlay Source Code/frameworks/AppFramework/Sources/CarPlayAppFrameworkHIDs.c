/*
	File:       CarPlayAppFrameworkHIDs.c
	Package: 	LG CarPlay Application Framework.
	Author:     Jaeheon Kim (jaeheon.kim@lge.com)

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

#include "CarPlayAppFrameworkHIDs.h"
#include "CarPlayAppFrameworkPrivate.h"

//===========================================================================================================================
//	HIDPointingCreateDescriptor
//===========================================================================================================================

OSStatus	HIDCustomPointingCreateDescriptor( uint8_t **outDescriptor, size_t *outLen )
{
	static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x0D,			// Usage Page (Digitizer)
		0x09, 0x04,			// Usage (Touch Screen)
		0xA1, 0x01,			// Collection (Application)

			0x05, 0x0D,			//   Usage Page (Digitizer)
			0x09, 0x22,			//   Usage (Finger)
			0xA1, 0x02,			//   Collection (Logical)

				0x05, 0x0D,			//     Usage Page (Digitizer)
				0x09, 0x33,			//     Usage (Touch)
				0x15, 0x00,			//     Logical Minimum......... (0)
				0x25, 0x01,			//     Logical Maximum......... (1)
                0x35, 0x00,         //     Physical Minimum........ (0)
                0x46, 0x64, 0x00,   //     Physical Maximum........ (100)
                0x55, 0x0f,         //     Unit Exponent (-1)
                0x65, 0x11,         //     Unit (cm)
				0x75, 0x01,			//     Report Size............. (1)
				0x95, 0x01,			//     Report Count............ (1)
				0x81, 0x02,			//     Input...................(Data, Variable, Absolute)
				0x75, 0x07,			//     Report Size............. (7)
				0x95, 0x01,			//     Report Count............ (1)
				0x81, 0x01,			//     Input...................(Constant)
				0x05, 0x01,			//     Usage Page (Generic Desktop)
				0x09, 0x30,			//     Usage (X)
				0x15, 0x00,			//     Logical Minimum......... (0)
				0x26, 0xFF, 0x7F,	//     Logical Maximum......... (0x7FFF)
				0x75, 0x10,			//     Report Size............. (16)
				0x95, 0x01,			//     Report Count............ (1)
				0x81, 0x02,			//     Input...................(Data, Variable, Absolute)
				0x09, 0x31,			//     Usage (Y)
				0x15, 0x00,			//     Logical Minimum......... (0)
				0x26, 0xFF, 0x7F,	//     Logical Maximum......... (0x7FFF)
				0x75, 0x10,			//     Report Size............. (16)
				0x95, 0x01,			//     Report Count............ (1)
				0x81, 0x02,			//     Input...................(Data, Variable, Absolute)
			0xC0, 			// end collection
		0xC0 				// end collection
	};

	OSStatus		err;
	size_t			len;
	uint8_t *		desc;

	len = sizeof( kDescriptorTemplate );
	desc = (uint8_t *) malloc( len );
	require_action( desc, exit, err = kNoMemoryErr );
	memcpy( desc, kDescriptorTemplate, len );

	*outDescriptor = desc;
	*outLen = len;
	err = kNoErr;

exit:
	return( err );
}

//===========================================================================================================================
//	HIDMultiPointingCreateDescriptor
//===========================================================================================================================

OSStatus HIDMultiPointingCreateDescriptor(
		uint8_t **outDescriptor,
		size_t *outLen)
{
	static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x0D, 		// Usage Page (Digitizers)
		0x09, 0x04, 		// Usage (Touch Screen)
		0xA1, 0x01, 		// Collection (Application)

			0x05, 0x0D,			// Usage Page (Digitizer)
			0x09, 0x22,			// Usage (Finger)
			0xA1, 0x02,			// Collection (Logical)

				0x05, 0x0D,			// Usage Page (Digitizer)
				0x09, 0x38,			// Usage (Transducer Index)
				0x75, 0x08,			// Report Size (8)
				0x95, 0x01,			// Report Count (1)
				0x81, 0x02,			// Input (Data)

				0x09, 0x33,			// Usage (Touch)
				0x15, 0x00,			// Logical Minimum (0)
				0x25, 0x01,			// Logical Maximum (1)
				0x75, 0x01,			// Report Size (1)
				0x95, 0x01,			// Report Count (1)
				0x81, 0x02,			// Input (Data)

				0x75, 0x07,			// Report Size (7)
				0x95, 0x01,			// Report Count (1)
				0x81, 0x01,			// Input (Constant)

				0x05, 0x01,			// Usage Page (Generic Desktop)
				0x09, 0x30,			// Usage (X)
				0x15, 0x00,			// Logical Minimum (0)
				0x26, 0xFF, 0x7F,	// Logical Maximum (0x7FFF)
				0x75, 0x10,			// Report Size (16)
				0x95, 0x01,			// Report Count (1)
				0x81, 0x02,			// Input (Data)

				0x09, 0x31,			// Usage (Y)
				0x15, 0x00,			// Logical Minimum (0)
				0x26, 0xFF, 0x7F,	// Logical Maximum (0x7FFF)
				0x75, 0x10,			// Report Size (16)
				0x95, 0x01,			// Report Count (1)
				0x81, 0x02,			// Input (Data)
			0xC0,				// End Collection

			0x05, 0x0D,			// Usage Page (Digitizer)
			0x09, 0x22,			// Usage (Finger)
			0xA1, 0x02,			// Collection (Logical)

				0x05, 0x0D, 		// Usage Page (Digitizer)
				0x09, 0x38, 		// Usage (Transducer Index)
				0x75, 0x08,			// Report Size (8)
				0x95, 0x01,			// Report Count (1)
				0x81, 0x02,			// Input

				0x09, 0x33, 		// Usage (Touch)
				0x15, 0x00,			// Logical Minimum (0)
				0x25, 0x01,			// Logical Maximum (1)
				0x75, 0x01,			// Report Size (1)
				0x95, 0x01,			// Report Count (1)
				0x81, 0x02,			// Input

				0x75, 0x07,			// Report Size (7)
				0x95, 0x01,			// Report Count (1)
				0x81, 0x01,			// Input (Constant)

				0x05, 0x01, 		// Usage Page (Generic Desktop)
				0x09, 0x30, 		// Usage (X)
				0x15, 0x00,			// Logical Minimum (0)
				0x26, 0xFF, 0x7F,	// Logical Maximum (0x7FFF)
				0x75, 0x10,			// Report Size (16)
				0x95, 0x01,			// Report Count (1)
				0x81, 0x02,			// Input

				0x09, 0x31, 		// Usage (Y)
				0x15, 0x00,			// Logical Minimum (0)
				0x26, 0xFF, 0x7F,	// Logical Maximum (0x7FFF)
				0x75, 0x10,			// Report Size (16)
				0x95, 0x01,			// Report Count (1)
				0x81, 0x02,			// Input
			0xC0,				// End Collection
		0xC0 				// End Collection
	};

	OSStatus		err;
	size_t			len;
	uint8_t *		desc;

	len = sizeof(kDescriptorTemplate);
	desc = (uint8_t *) malloc(len);
	require_action(desc, exit, err = kNoMemoryErr);
	memcpy(desc, kDescriptorTemplate, len);

	*outDescriptor = desc;
	*outLen = len;
	err = kNoErr;

exit:
	return( err );
}

//===========================================================================================================================
//	HIDMultiPointingFillReport
//===========================================================================================================================

void HIDMultiPointingFillReport(
		uint8_t inReport[12],
		uint8_t inIDs[2],
		uint8_t inButtons[2],
		double inX[2],
		double inY[2])
{
	uint16_t 	x1, y1, x2, y2;

	x1 = (uint16_t) TranslateValue( inX[0], 0.0, 1.0, 0, 0x7FFF );
	y1 = (uint16_t) TranslateValue( inY[0], 0.0, 1.0, 0, 0x7FFF );
	x2 = (uint16_t) TranslateValue( inX[1], 0.0, 1.0, 0, 0x7FFF );
	y2 = (uint16_t) TranslateValue( inY[1], 0.0, 1.0, 0, 0x7FFF );

	// Primary Touch
	inReport[ 0 ] = inIDs[0];
	inReport[ 1 ] = inButtons[0];
	inReport[ 2 ] = (uint8_t)(   x1        & 0xFF );
	inReport[ 3 ] = (uint8_t)( ( x1 >> 8 ) & 0xFF );
	inReport[ 4 ] = (uint8_t)(   y1        & 0xFF );
	inReport[ 5 ] = (uint8_t)( ( y1 >> 8 ) & 0xFF );

	// Secondary Touch
	inReport[ 6 ] = inIDs[1];
	inReport[ 7 ] = inButtons[1];
	inReport[ 8 ] = (uint8_t)(   x2        & 0xFF );
	inReport[ 9 ] = (uint8_t)( ( x2 >> 8 ) & 0xFF );
	inReport[ 10] = (uint8_t)(   y2        & 0xFF );
	inReport[ 11] = (uint8_t)( ( y2 >> 8 ) & 0xFF );
}

//===========================================================================================================================
//	HIDCustomButtonsCreateDescriptor
//===========================================================================================================================
OSStatus HIDCustomButtonsCreateDescriptor(
		uint8_t **outDescriptor,
		size_t *outLen)
{
#if defined(GEELY)
	static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x0C,		// Usage Page (Consumer)
		0x09, 0x01,		// Usage 1 (0x1)
		0xA1, 0x01,		// Collection (Application)
			0x05, 0x0C,			//	 Usage Page (Consumer)
			0x09, 0xB5,     	//   Usage 181 (0xb5) 	// Scan Next Track
			0x09, 0xB6,     	//   Usage 182 (0xb6) 	// Scan Previous Track
			0x09, 0xCD,     	// 	 Usage 205 (0xcd) 	// Play / Pause
			0x15, 0x00,			//   Logical Minimum......... (0)
			0x25, 0x01,			//   Logical Maximum......... (1)
			0x75, 0x01,			//   Report Size............. (1)
			0x95, 0x03,			//   Report Count............ (3)
			0x81, 0x02,			//   Input...................(Data, Variable, Absolute)
			0x75, 0x05,			//   Report Size............. (5)
			0x95, 0x01,			//   Report Count............ (1)
			0x81, 0x01,			//   Input...................(Constant)
		0xC0			// End Collection
	};
#elif defined(GEELY_KC1B)
	static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x0C,		// Usage Page (Consumer)
		0x09, 0x01,		// Usage 1 (0x1)
		0xA1, 0x01,		// Collection (Application)
			0x05, 0x0C,			//	 Usage Page (Consumer)
			0x09, 0xB5,     	//   Usage 181 (0xb5) 	// Scan Next Track
			0x09, 0xB6,     	//   Usage 182 (0xb6) 	// Scan Previous Track
			0x09, 0xCD,     	// 	 Usage 205 (0xcd) 	// Play / Pause
			0x0a, 0x24, 0x02,	//	 Usage ac back
			0x15, 0x00,			//   Logical Minimum......... (0)
			0x25, 0x01,			//   Logical Maximum......... (1)
			0x75, 0x01,			//   Report Size............. (1)
			0x95, 0x04,			//   Report Count............ (4)
			0x81, 0x02,			//   Input...................(Data, Variable, Absolute)
			0x75, 0x04,			//   Report Size............. (4)
			0x95, 0x01,			//   Report Count............ (1)
			0x81, 0x01,			//   Input...................(Constant)
		0xC0			// End Collection
	};
#elif defined(MM2014)
	static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x0C,		// Usage Page (Consumer)
		0x09, 0x01,		// Usage 1 (0x1)
		0xA1, 0x01,		// Collection (Application)
			0x05, 0x0C,			//	 Usage Page (Consumer)
			0x09, 0xB0,     	//   Usage 176 (0xb0) 	// Play
			0x09, 0xB1,     	//   Usage 177 (0xb1) 	// Pause
			0x09, 0xB5,     	//   Usage 181 (0xb5) 	// Scan Next Track
			0x09, 0xB6,     	//   Usage 182 (0xb6) 	// Scan Previous Track
			0x09, 0xCD,     	// 	 Usage 205 (0xcd) 	// Play / Pause
			0x15, 0x00,			//   Logical Minimum......... (0)
			0x25, 0x01,			//   Logical Maximum......... (1)
			0x75, 0x01,			//   Report Size............. (1)
			0x95, 0x05,			//   Report Count............ (5)
			0x81, 0x02,			//   Input...................(Data, Variable, Absolute)
			0x75, 0x03,			//   Report Size............. (3)
			0x95, 0x01,			//   Report Count............ (1)
			0x81, 0x01,			//   Input...................(Constant)
		0xC0			// End Collection
	};
#elif defined(MEEGO)
	static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x0C,		// Usage Page (Consumer)
		0x09, 0x01,		// Usage 1 (0x1)
		0xA1, 0x01,		// Collection (Application)
			0x05, 0x0C,			//	 Usage Page (Consumer)
			0x09, 0xB0,     	//   Usage 176 (0xb0) 	// Play
			0x09, 0xB5,     	//   Usage 181 (0xb5) 	// Scan Next Track
			0x09, 0xB6,     	//   Usage 182 (0xb6) 	// Scan Previous Track
			0x15, 0x00,			//   Logical Minimum......... (0)
			0x25, 0x01,			//   Logical Maximum......... (1)
			0x75, 0x01,			//   Report Size............. (1)
			0x95, 0x03,			//   Report Count............ (3)
			0x81, 0x02,			//   Input...................(Data, Variable, Absolute)
			0x75, 0x05,			//   Report Size............. (5)
			0x95, 0x01,			//   Report Count............ (1)
			0x81, 0x01,			//   Input...................(Constant)
		0xC0			// End Collection
	};
#else
	static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x0C,		// Usage Page (Consumer)
		0x09, 0x01,		// Usage 1 (0x1)
		0xA1, 0x01,		// Collection (Application)
			0x05, 0x0C,			//	 Usage Page (Consumer)
			0x09, 0xB0,     	//   Usage 176 (0xb0) 	// Play
			0x09, 0xB1,     	//   Usage 177 (0xb1) 	// Pause
			0x09, 0xB5,     	//   Usage 181 (0xb5) 	// Scan Next Track
			0x09, 0xB6,     	//   Usage 182 (0xb6) 	// Scan Previous Track
			0x15, 0x00,			//   Logical Minimum......... (0)
			0x25, 0x01,			//   Logical Maximum......... (1)
			0x75, 0x01,			//   Report Size............. (1)
			0x95, 0x04,			//   Report Count............ (4)
			0x81, 0x02,			//   Input...................(Data, Variable, Absolute)
			0x75, 0x04,			//   Report Size............. (4)
			0x95, 0x01,			//   Report Count............ (1)
			0x81, 0x01,			//   Input...................(Constant)
		0xC0			// End Collection
	};
#endif

	OSStatus		err;
	size_t			len;
	uint8_t *		desc;

	len = sizeof( kDescriptorTemplate );
	desc = (uint8_t *) malloc( len );
	require_action( desc, exit, err = kNoMemoryErr );
	memcpy( desc, kDescriptorTemplate, len );

	*outDescriptor = desc;
	*outLen = len;
	err = kNoErr;

exit:
	return( err );
}

//===========================================================================================================================
//	HIDCustomTelephonyCreateDescriptor
//===========================================================================================================================

OSStatus HIDCustomTelephonyCreateDescriptor(
		uint8_t **outDescriptor,
		size_t *outLen )
{

#if defined(MM2014)
	static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x0B,     // Usage Page (Telephony Device)
		0x09, 0x01,     // Usage 1 (0x1)
		0xA1, 0x01,     // Collection (Application)
			0x05, 0x0B,     //   Usage Page (Telephony Device)
			0x09, 0x21,     //   Usage 33 (Flash)
			0x15, 0x00,     //   Logical Minimum......... (0)
			0x25, 0x01,     //   Logical Maximum......... (1)
			0x75, 0x01,     //   Report Size............. (1)
			0x95, 0x01,     //   Report Count............ (1)
			0x81, 0x02,     //   Input...................(Data, Variable, Absolute)
			0x75, 0x07,     //   Report Size............. (7)
			0x95, 0x01,     //   Report Count............ (1)
			0x81, 0x01,     //   Input...................(Constant)
		0xC0,           // End Collection
	};
#else
	static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x0B,     // Usage Page (Telephony Device)
		0x09, 0x01,     // Usage 1 (0x1)
		0xA1, 0x01,     // Collection (Application)
			0x05, 0x0B,     //   Usage Page (Telephony Device)
			0x09, 0x20,     //   Usage 32 (Hook)
			// 0x09, 0x21,     //   Usage 33 (Flash)
			0x09, 0x26,     //   Usage 38 (Drop)
			0x09, 0x2F,     //   Usage 47 (Phone Mute)
			0x15, 0x00,     //   Logical Minimum......... (0)
			0x25, 0x01,     //   Logical Maximum......... (1)
			0x75, 0x01,     //   Report Size............. (1)
			0x95, 0x03,     //   Report Count............ (3)
			0x81, 0x02,     //   Input...................(Data, Variable, Absolute)
			0x75, 0x05,     //   Report Size............. (5)
			0x95, 0x01,     //   Report Count............ (1)
			0x81, 0x01,     //   Input...................(Constant)
		0xC0,           // End Collection
	};
#endif

	OSStatus		err;
	size_t			len;
	uint8_t *		desc;

	len = sizeof( kDescriptorTemplate );
	desc = (uint8_t *) malloc( len );
	require_action( desc, exit, err = kNoMemoryErr );
	memcpy( desc, kDescriptorTemplate, len );

	*outDescriptor = desc;
	*outLen = len;
	err = kNoErr;

exit:
	return( err );
}

//===========================================================================================================================
//	HIDCustomKnobCreateDescriptor
//===========================================================================================================================

OSStatus	HIDCustomKnobCreateDescriptor( uint8_t **outDescriptor, size_t *outLen )
{
#if defined(MEEGO) || defined(MM2014)
	static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x01,
		0x09, 0x08,
		0xA1, 0x01,
			// Button
			0x15, 0x00,
			0x25, 0x01,
			0x75, 0x01,
			0x05, 0x09,
			0x09, 0x01,
			0x95, 0x01,
			0x81, 0x02,

			// consumer
			0x05, 0x0c,
			//0x0a, 0x23, 0x02,// ac home
			0x0a, 0x24, 0x02,// ac back
			0x95, 0x01,
			0x81, 0x02,

			// Constant
			0x95, 0x06,  //change value 5->6
			0x81, 0x01,

			// Pointer
			0x05, 0x01,
			0x09, 0x01,
			0xA1, 0x00,
				0x09, 0x30,
				0x09, 0x31,

				// log min/max
				0x15, 0x81,
				0x25, 0x7f,

				// report size
				0x75, 0x08,
				// report count
				0x95, 0x02,
				// variable, absolute
				0x81, 0x02,
			0xC0,

			// Wheel Padding
			0x09, 0x38,
			// log min/max
			0x15, 0x81,
			0x25, 0x7f,
			// report size
			0x75, 0x08,
			// report count
			0x95, 0x01,
			// variable, relative
			0x81, 0x06,
		0xC0
	};
#else
    static const uint8_t		kDescriptorTemplate[] =
	{
		0x05, 0x01,
		0x09, 0x08,
		0xA1, 0x01,
			// Button
			0x15, 0x00,
			0x25, 0x01,
			0x75, 0x01,
			0x05, 0x09,
			0x09, 0x01,
			0x95, 0x01,
			0x81, 0x02,

			// consumer
			0x05, 0x0c,
			0x0a, 0x23, 0x02,// ac home
			0x0a, 0x24, 0x02,// ac back
			0x95, 0x02,
			0x81, 0x02,

			// Constant
			0x95, 0x05,
			0x81, 0x01,

			// Pointer
			0x05, 0x01,
			0x09, 0x01,
			0xA1, 0x00,
				0x09, 0x30,
				0x09, 0x31,

				// log min/max
				0x15, 0x81,
				0x25, 0x7f,

				// report size
				0x75, 0x08,
				// report count
				0x95, 0x02,
				// variable, absolute
				0x81, 0x02,
			0xC0,

			// Wheel Padding
			0x09, 0x38,
			// log min/max
			0x15, 0x81,
			0x25, 0x7f,
			// report size
			0x75, 0x08,
			// report count
			0x95, 0x01,
			// variable, relative
			0x81, 0x06,
		0xC0
	};
#endif

	OSStatus		err;
	size_t			len;
	uint8_t *		desc;

	len = sizeof( kDescriptorTemplate );
	desc = (uint8_t *) malloc( len );
	require_action( desc, exit, err = kNoMemoryErr );
	memcpy( desc, kDescriptorTemplate, len );

	*outDescriptor = desc;
	*outLen = len;
	err = kNoErr;

exit:
	return( err );
}

//===========================================================================================================================
//	HIDCustomKnobFillReport
//===========================================================================================================================

void
	HIDCustomKnobFillReport(
		uint8_t	inReport[ 4 ],
		Boolean	inButton,
		Boolean	inHome,
		Boolean	inBack,
		int8_t	inX,
		int8_t	inY,
		int8_t	inWheel )
{

#if defined(MEEGO) || defined(MM2014)
	inReport[ 0 ] = (uint8_t)( inButton | ( inBack << 1 ) );
#else
    inReport[ 0 ] = (uint8_t)( inButton | ( inHome << 1 ) | ( inBack << 2 ) );
#endif

	inReport[ 1 ] = (uint8_t) inX;
	inReport[ 2 ] = (uint8_t) inY;
	inReport[ 3 ] = (uint8_t) inWheel;
}
