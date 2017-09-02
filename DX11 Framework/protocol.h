#pragma once


#define CS_KEYTYPE      1
#define CS_ROTATE      2
#define   CS_WEAPONE      3
#define CS_HEAD_HIT      4


#define RemovePlayer   6



#pragma pack(push, 1)
struct cs_key_input {   //키버튼 받았을때 

	BYTE   size;               //1 2 12 2
	volatile BYTE   type;

	XMFLOAT3    Animation;
	WORD   key_button;

	float   x;
	float   y;
	float   z;

	XMFLOAT3 FireDirection;

};

struct cs_rotate {   //클라에서 화면을 움직였을때 

	BYTE   size;
	volatile BYTE   type;

	float cx;
	float cy;



};

struct sc_packet_put_player {   //서버에서 처음 접속했을때 위치값과 ID를 부여한다.

	BYTE size;
	volatile BYTE type;

	WORD   id;

	float x;
	float y;
	float z;

	BYTE Team;

	XMFLOAT3   Animation;

	BYTE   hp;

	BYTE   Goal;
	BYTE   RED;
	BYTE   Blue;
	float   timer;

	BYTE mode;
};

struct sc_packet_pos   //서버에서 처리된 값을 클라에게 보낸다. 
{
	BYTE size;
	volatile BYTE type;

	WORD	id;
	WORD	key_button;

	float   x;
	float   y;
	float   z;

	XMFLOAT3   Animation;
	BYTE   hp;


};

struct sc_rotate_vector
{
	BYTE size;
	BYTE type;

	WORD id;

	float x;
	float y;
	float z;

};

struct sc_bullet_fire
{
	BYTE size;
	BYTE type;

	WORD id;

	bool fire;
	XMFLOAT3 FireDirection;
};


struct sc_packet_remove_player {   //접속이 종료되면 보내는 패킷이다.
	BYTE size;
	BYTE type;
	WORD id;
};

struct cs_weapon {   //클라에서 무기의 정보를 보낸다.
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

	XMFLOAT3 position;
	XMFLOAT3 direction;
};


struct SC_Damegedirection
{
	BYTE size;
	BYTE type;

	XMFLOAT3 position;
};

struct SC_Player_Hp
{
	BYTE size;
	BYTE type;

	BYTE m_nLife;
	BYTE m_nCharacterID;

	BOOL m_bIsHeadHit;
	BOOL m_bIsAlive;
};

struct SC_System_kill
{
	BYTE size;
	BYTE type;

	BYTE m_nRedTeamTotalKill;
	BYTE m_nBlueTeamTotalKill;

	//FLOAT Time;   //나중에 시간 넣어서 보내준다.
};

struct SC_Respawn
{
	BYTE size;
	BYTE type;
	BYTE id;

	BOOL m_bIsRespawn;
	XMFLOAT3 m_f3Position;
};

struct SC_Starting_Timer
{
	BYTE size;
	BYTE type;

	float Starting_timer;

};

struct sc_Reload
{
	BYTE size;
	BYTE type;

	WORD id;

	bool reload;

};

struct SC_Run
{
	BYTE size;
	BYTE type;

	WORD id;

	bool Run;


};

struct cs_Gamemode
{
	BYTE size;
	BYTE type;
	BYTE mode;

};

struct cs_temp_exit
{
	BYTE size;
	BYTE type;
	BYTE Winner;
};




struct cs_login
{
	BYTE size;
	BYTE type;

	DWORD strlen;
	DWORD passstrlen;

	char id[10];
	char password[10];
};

struct SC_login_CONNECT
{
	BYTE size;
	BYTE type;
	WORD id;
	bool connect;
};



struct sc_input_game
{
	BYTE size;
	BYTE type;
};

struct cs_create_charter
{
	BYTE size;
	BYTE type;
};

struct sc_occupy
{
	BYTE size;
	BYTE type;
	BYTE redteam;

};

struct SC_Occupy_Timer
{
	BYTE size;
	BYTE type;

	float Occupy_timer;

};

struct CS_Fire
{
	BYTE size;
	BYTE type;

	XMFLOAT3 FireDirection;
};

struct SC_fullHPpacket
{
	BYTE size;
	BYTE type;

	BYTE Hp;
	BYTE id;

};



#pragma pack(pop)