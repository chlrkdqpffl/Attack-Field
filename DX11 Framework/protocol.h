#pragma once


#define CS_KEYTYPE		1
#define CS_ROTATE		2

#define RemovePlayer	4



#pragma pack(push, 1)
struct cs_key_input {	//Ű��ư �޾����� 

	BYTE	size;
	BYTE	type;

	float fDistance;

	DWORD key_button;

};

struct cs_rotate {	//Ŭ�󿡼� ȭ���� ���������� 

	BYTE	size;
	BYTE	type;

	float cx;
	float cy;
	


};

struct sc_packet_put_player {	//�������� ó�� ���������� ��ġ���� ID�� �ο��Ѵ�.

	BYTE size;
	BYTE type;

	WORD id;

	int x;
	int y;
	int z;

};

struct sc_packet_pos	//�������� ó���� ���� Ŭ�󿡰� ������. 
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

struct sc_packet_remove_player {	//������ ����Ǹ� ������ ��Ŷ�̴�.
	BYTE size;
	BYTE type;
	WORD id;
};
#pragma pack(pop)