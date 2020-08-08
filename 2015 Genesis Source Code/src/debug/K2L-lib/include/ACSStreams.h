/*=== K2L GmbH ===============================================================*/
/**
\file		ACSStreams.h

\ingroup	ACSStreams

\par        COPYRIGHT (c) 2007-2008 by K2L GmbH
            All rights reserved.

*/
/*============================================================================*/

#ifndef __CFSTREAMS_H
#define __CFSTREAMS_H

#include <K2LACSBaseImportExport.h>

namespace k2l
{
    namespace acs
    {
        namespace base
        {
            /// This class is able to write into streams using big endian byte order.
            class K2LACSBASE_API CBigEndianStreamWriter
            {
                uint8_t *m_pBuffer;
                uint32_t m_capacity;
                uint32_t m_position;

            public:
                /// Creates an "empty" object. Allows to defer initialization.
                CBigEndianStreamWriter()
                    : m_pBuffer(NULL)
                    , m_capacity(0)
                    , m_position(0)
                {
                }

                CBigEndianStreamWriter( uint8_t *pBuffer, uint32_t capacity )
                    : m_pBuffer(pBuffer)
                    , m_capacity(capacity)
                    , m_position(0)
                {
                }

				/// Call Initialize() to make the object ready to work.
                void Initialize( uint8_t *pBuffer, uint32_t capacity )
                {
                    m_pBuffer = pBuffer;
                    m_capacity = capacity;
                    m_position = 0;
                }

                /// Seekts the given offset in the stream relative to its beginning.
                inline bool SeekFromStart( uint32_t offset )
                {
                    bool success = false;
                    if( offset <= m_capacity )
                    {
                        m_position = offset;
                        success = true;
                    }
                    return success;
                }

                /// Returns the maximum capacity of the sink the stream writes to.
                inline uint32_t Capacity() const
                {
                    return m_capacity;
                }

                /// Returns the current position on the sink the stream writes to.
                inline uint32_t Position() const
                {
                    return m_position;
                }
                
                /// Writes a byte.
                inline bool Write8( uint8_t value )
                {
                    if( NULL == m_pBuffer )
                    {
                        return false;
                    }

                    if( m_position >= m_capacity )
                    {
                        return false;
                    }

                    m_pBuffer[m_position] = value;
                    m_position++;
                    return true;
                }

                /// Writes a word.
                inline bool Write16( uint16_t value )
                {
                    if( NULL == m_pBuffer )
                    {
                        return false;
                    }

                    if( (m_position + 1) >= m_capacity )
                    {
                        return false;
                    }

                    m_pBuffer[m_position++] = (uint8_t)(value >> 8);
                    m_pBuffer[m_position++] = (uint8_t)value;

                    return true;
                }

                /// Writes a double word.
                inline bool Write32( uint32_t value )
                {
                    if( NULL == m_pBuffer )
                    {
                        return false;
                    }

                    if( (m_position + 3) >= m_capacity )
                    {
                        return false;
                    }                    

                    m_pBuffer[m_position++] = (uint8_t)(value >> 24);
                    m_pBuffer[m_position++] = (uint8_t)(value >> 16);
                    m_pBuffer[m_position++] = (uint8_t)(value >> 8);
                    m_pBuffer[m_position++] = (uint8_t)value;

                    return true;
                }

                /// Writes a block of bytes.
                bool WriteBlock( const uint8_t *pBlock, uint32_t length );
            };

            /// This class is able to read from streams using big endian byte order.
            class K2LACSBASE_API CBigEndianStreamReader
            {
                const uint8_t *m_pBuffer;
                uint32_t m_capacity;
                uint32_t m_position;

            public:
				CBigEndianStreamReader()
					: m_pBuffer(NULL)
					, m_capacity(0)
					, m_position(0)
				{
				}

                CBigEndianStreamReader( const uint8_t *pBuffer, uint32_t capacity )
                    : m_pBuffer(pBuffer)
                    , m_capacity(capacity)
                    , m_position(0)
                {
                }

                /// Call Initialize() to make the object ready to work.
				void Initialize( const uint8_t*pBuffer, uint32_t capacity )
				{
					m_pBuffer = pBuffer;
					m_capacity = capacity;
					m_position = 0;
				}

                /// Seekts the given offset in the stream relative to its beginning.
                inline bool SeekFromStart( uint32_t offset )
                {
                    bool success = false;
                    if( offset <= m_capacity )
                    {
                        m_position = offset;
                        success = true;
                    }
                    return success;
                }

                /// Returns the maximum capacity of the sink the stream reads from.
                inline uint32_t Capacity() const
                {
                    return m_capacity;
                }

                /// Returns the current position on the sink the strean reads from.
                inline uint32_t Position() const
                {
                    return m_position;
                }
                
                /// Reads a byte.
                inline bool Read8( uint8_t *pValue )
                {
                    if( NULL == m_pBuffer || NULL == pValue)
                    {
                        return false;
                    }

                    if( m_position >= m_capacity )
                        return false;

                    (*pValue) = m_pBuffer[m_position++];

                    return true;
                }

                /// Reads a word.
                inline bool Read16( uint16_t *pValue )
                {
                    if( NULL == m_pBuffer || NULL == pValue)
                    {
                        return false;
                    }

                    if( (m_position + 1) >= m_capacity )
                        return false;

                    uint16_t value;
                    value  = (m_pBuffer[m_position++] << 8);
                    value |=  m_pBuffer[m_position++];

                    *pValue = value;

                    return true;
                }

                /// Reads a double word.
                inline bool Read32( uint32_t *pValue )
                {
                    if( NULL == m_pBuffer || NULL == pValue)
                    {
                        return false;
                    }

                    if( (m_position + 3) >= m_capacity )
                        return false;

                    uint32_t value;
                    value =  (m_pBuffer[m_position++] << 24);
                    value |= (m_pBuffer[m_position++] << 16); 
                    value |= (m_pBuffer[m_position++] << 8);
                    value |= m_pBuffer[m_position++];

                    *pValue = value;
                    
                    return true;
                }

                /// Reads a block of bytes.
                bool ReadBlock( uint8_t * pBuffer, uint32_t length );

                /// Peak
                const uint8_t *PointerAtCurrentPosition() const
                {
                    return &m_pBuffer[m_position];
                }
            };


            typedef CBigEndianStreamWriter CACSStreamWriter;
            typedef CBigEndianStreamReader CACSStreamReader;
        }
    }
}

#endif

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
