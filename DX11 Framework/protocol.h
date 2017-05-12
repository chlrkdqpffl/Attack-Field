#pragma once


#define CS_KEYTYPE		1
#define CS_ROTATE		2
#define	CS_WEAPONE		3
#define CS_HEAD_HIT		4

#define RemovePlayer	5



#pragma pack(push, 1)
struct cs_key_input {	//Ű��ư �޾����� 

	BYTE	size;
	volatile BYTE	type;

	XMFLOAT3	 Animation;


	DWORD	key_button;

	BYTE	Hp;

	float	x;
	float	y;
	float	z;

	XMFLOAT3 FireDirection;

};

struct cs_rotate {	//Ŭ�󿡼� ȭ���� ���������� 

	BYTE	size;
	volatile BYTE	type;

	float cx;
	float cy;
	


};

struct sc_packet_put_player {	//�������� ó�� ���������� ��ġ���� ID�� �ο��Ѵ�.

	BYTE size;
	volatile BYTE type;

	WORD	id;
	WORD	Charid;

	float x;
	float y;
	float z;

	XMFLOAT3	Animation;
	BYTE	hp;

};

struct sc_packet_pos	//�������� ó���� ���� Ŭ�󿡰� ������. 
{
	BYTE size;
	volatile BYTE type;

	WORD	id;
	WORD	Charid;

	float	x;
	float	y;
	float	z;

	XMFLOAT3	Animation;
	BYTE	hp;

	
};

struct sc_rotate_vector	
{
	BYTE size;
	BYTE type;

	WORD id;
	WORD Charid;

	float x;
	float y;
	float z;

};

struct sc_bullet_fire
{
	BYTE size;
	BYTE type;

	WORD id;
	WORD Charid;

	bool fire;
	XMFLOAT3 FireDirection;
};


struct sc_packet_remove_player {	//������ ����Ǹ� ������ ��Ŷ�̴�.
	BYTE size;
	BYTE type;
	WORD id;
};

struct cs_weapon {	//Ŭ�󿡼� ������ ������ ������.
	BYTE size;
	BYTE type;
	
	XMFLOAT3 direction;
	XMFLOAT3 position;

};

struct SC_Collison
{
	BYTE size;
	BYTE type;

	WORD id;
	BOOL collision;
	XMFLOAT3 position;
	XMFLOAT3 direction;
};

struct CS_Head_Collison
{
	BYTE size;
	BYTE type;

	BOOL Head;
	BYTE id;
};

struct SC_Player_Hp
{
	BYTE size;
	BYTE type;

	BYTE Hp;
	BYTE id;
	WORD Charid;
};
#pragma pack(pop)