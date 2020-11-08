#pragma once

#include <WinSock2.h>

class NetworkMessage
{
public:
	// constructor/destructor
	NetworkMessage();
	~NetworkMessage();

	// resets the internal buffer to an empty message
	void Reset();

	// socket functions
	bool ReadFromSocket(SOCKET socket);
	bool WriteToSocket(SOCKET socket);

	// simply read functions for incoming message
	unsigned char  GetByte();
	unsigned short GetU16();
	unsigned short GetSpriteId();
	unsigned int GetU32();
	std::string GetString();

	// skips count unknown/unused bytes in an incoming message
	void skip( void );
	void skip( int count );

	// simply write functions for outgoing message
	void AddByte(unsigned char  value);
	void AddU16(unsigned short value);
	void AddU32(unsigned int value);
	void AddBytes(const char* bytes, unsigned int size);

	void AddString(const std::string &value);
	void AddString(const char* value);

protected:
	inline bool canAdd(int size);
  	
  	int m_MsgSize;
  	int m_ReadPos;

	unsigned char m_MsgBuf[16768];
	
	unsigned int m_key[4];
};