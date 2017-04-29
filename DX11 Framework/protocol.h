#pragma once


#define CS_KEYTYPE		1
#define CS_ROTATE		2

#define RemovePlayer	4



#pragma pack(push, 1)
struct cs_key_input {	//키버튼 받았을때 

	BYTE	size;
	BYTE	type;

	float fDistance;

	DWORD key_button;

};

struct cs_rotate {	//클라에서 화면을 움직였을때 

	BYTE	size;
	BYTE	type;

	float cx;
	float cy;
	


};

struct sc_packet_put_player {	//서버에서 처음 접속했을때 위치값과 ID를 부여한다.

	BYTE size;
	BYTE type;

	WORD id;

	int x;
	int y;
	int z;

};

struct sc_packet_pos	//서버에서 처리된 값을 클라에게 보낸다. 
{
	BYTE size;
	BYTE type;

	WORD id;

	float x;
	float y;
	float z;
};

struct sc_rotate_vector	
{
	BYTE size;
	BYTE type;
	WORD id;

	XMMATRIX matrix;

};

struct sc_packet_remove_player {	//접속이 종료되면 보내는 패킷이다.
	BYTE size;
	BYTE type;
	WORD id;
};
#pragma pack(pop)