#define WIN32_LEAN_AND_MEAN
#include <string>
#include <iostream>
#include <sstream>
#include <windows.h>

#include "networkmessage.h"

NetworkMessage::NetworkMessage()
{
	Reset();
}

NetworkMessage::~NetworkMessage()
{
}

void NetworkMessage::Reset()
{
	m_MsgSize = 0;
	m_ReadPos = 2;
}

bool NetworkMessage::ReadFromSocket(SOCKET Socket)
{
    if ( Socket == INVALID_SOCKET )
    {
        return false;
    }

    m_MsgSize = recv ( Socket, (char*)m_MsgBuf, 2, 0 );

    if ( m_MsgSize < 2 || m_MsgSize == SOCKET_ERROR )
    {
        return false;
    }

    unsigned short Length = (m_MsgBuf[0] + (m_MsgBuf[1] << 8));

    m_MsgSize = recv ( Socket, ((char*)m_MsgBuf + 2), Length, 0 );

    if ( m_MsgSize == SOCKET_ERROR )
    {
        return false;
    }

    m_ReadPos = 2;

    return true;
}


bool NetworkMessage::WriteToSocket(SOCKET Socket)
{
 	if (m_MsgSize == 0)
		return true;   
 
	m_MsgBuf[0] = (unsigned char)(m_MsgSize);
	m_MsgBuf[1] = (unsigned char)(m_MsgSize >> 8);

    if ( send ( Socket, (char*)m_MsgBuf + 2, (m_MsgSize + 2), 0 ) == SOCKET_ERROR )
    {
        return false;
    }

    return true;
}

unsigned char NetworkMessage::GetByte()
{
	return m_MsgBuf[m_ReadPos++];
}


unsigned short NetworkMessage::GetU16()
{
    short* Pointer = (short*)(m_MsgBuf + m_ReadPos);
    m_ReadPos += 2;

    return *Pointer;
}

unsigned int NetworkMessage::GetU32()
{
    int* Pointer = (int*)(m_MsgBuf + m_ReadPos);
    m_ReadPos += 4;

    return *Pointer;
}

std::string NetworkMessage::GetString()
{
    unsigned int Length = GetU16();
    char* Message = (char*)(m_MsgBuf + m_ReadPos);

    m_ReadPos += Length;
	return std::string( Message, Length );
}

void NetworkMessage::skip( void )
{
	m_ReadPos++;
}

void NetworkMessage::skip( int count )
{
	m_ReadPos += count;
}

void NetworkMessage::AddByte(unsigned char value)
{
	if(!canAdd(1))
		return;
	m_MsgBuf[m_ReadPos++] = value;
	m_MsgSize++;
}

void NetworkMessage::AddU16(unsigned short value)
{
	if(!canAdd(2))
		return;
	
	*(unsigned short*)(m_MsgBuf + m_ReadPos) = value;
	m_ReadPos += 2;
	m_MsgSize += 2;
}

void NetworkMessage::AddU32(unsigned int value)
{
	if(!canAdd(4))
		return;
	
	*(unsigned int*)(m_MsgBuf + m_ReadPos) = value;
	m_ReadPos += 4;
	m_MsgSize += 4;
}

void NetworkMessage::AddString(const std::string& value)
{
	AddString(value.c_str());
}

void NetworkMessage::AddString(const char* value)
{
	unsigned int stringlen = (unsigned int)strlen(value);
	if(!canAdd(stringlen+2) || stringlen > 8192)
		return;
	
	AddU16(stringlen);
	strcpy((char*)(m_MsgBuf + m_ReadPos), value);
	m_ReadPos += stringlen;
	m_MsgSize += stringlen;
}

void NetworkMessage::AddBytes(const char* bytes, unsigned int size)
{
	if(!canAdd(size) || size > 8192)
		return;
	
	memcpy(m_MsgBuf + m_ReadPos, bytes, size);
	m_ReadPos += size;
	m_MsgSize += size;
}

bool NetworkMessage::canAdd(int size)
{
  	return (size + m_ReadPos < 16752);
}

