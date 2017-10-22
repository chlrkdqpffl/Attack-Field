#pragma once


#define CS_KEYTYPE      1
#define CS_ROTATE       2
#define CS_WEAPONE      3
#define CS_HEAD_HIT     4
#define CS_GAME_MODE	5
#define RemovePlayer    6
#define CS_CreatePlayer 8
#define CS_OCCUPYTEAM	9

enum PacketType
{
	ePacket_Update = 1,
	ePacket_CreateOthderPlayer,
	ePacket_MouseRotate,
	ePacket_CollisionCheck,
	ePacket_HP,
	ePacket_KillUpdate,
	ePacket_GameTimer,
	ePacket_Respawn,
	ePacket_OccupyTeam,
	ePacket_DamageInfo,
	ePacket_WeaponReplace,

	ePacket_SuccessMyCharacter,
	ePacket_SceneChange,
	ePacket_LoginFail,
	ePacket_Disconnect
};


#pragma pack(push, 1)

// ====================== ���� �� ��Ŷ ���� ==================== //
struct cs_key_input {

	BYTE   size;
	BYTE   type;
	WORD   key_button;

	XMFLOAT3 playerPosition;
};

struct cs_rotate {   //Ŭ�󿡼� ȭ���� ���������� 

	BYTE   size;
	BYTE   type;

	XMFLOAT2 mouseRotate;
	XMFLOAT3 FireDirection;			// �̰� ���� �ڵ���??? ��� ���̴�����
};

struct sc_packet_put_player {	//�������� ó�� ���������� ��ġ���� ID�� �ο��Ѵ�.

	BYTE size;
	BYTE type;

	WORD id;
	char characterID[20];

	float x;
	float y;
	float z;

	BYTE Team;
	BYTE hp;
	BYTE Goal;
	BYTE RED;
	BYTE Blue;
	float timer;

	BYTE mode;
};

struct sc_packet_pos   //�������� ó���� ���� Ŭ�󿡰� ������. 
{
	BYTE size;
	BYTE type;

	WORD	id;
	WORD	key_button;

	float   x;
	float   y;
	float   z;

};

struct sc_rotate_vector
{
	BYTE size;
	BYTE type;

	WORD id;

	float x;
	float y;

	XMFLOAT3 FireDirection;
};

struct sc_bullet_fire
{
	BYTE size;
	BYTE type;

	WORD id;

	bool fire;
	XMFLOAT3 FireDirection;
};


struct sc_packet_remove_player {   //������ ����Ǹ� ������ ��Ŷ�̴�.
	BYTE size;
	BYTE type;
	WORD id;
};

struct cs_FireInfo
{
	BYTE size;
	BYTE type;

	XMFLOAT3 direction;
	XMFLOAT3 position;
	BYTE weaponType;
};

struct SC_Collison
{
	BYTE size;
	BYTE type;

	BYTE weaponType;
	WORD hitCharacterID;

	BOOL collision;
	XMFLOAT3 position;
	XMFLOAT3 direction;
};

struct CS_Head_Collison
{
	BYTE size;
	BYTE type;

	BYTE hitParts;
	BYTE hitCharacterID;
	BYTE weaponType;

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

	BYTE m_nCharacterID;
	BYTE m_nLife;
	BYTE m_nArmorPoint;

	BOOL m_bIsHeadHit;
	BOOL m_bIsAlive;
};

struct SC_System_kill
{
	BYTE size;
	BYTE type;

	BYTE m_nRedTeamTotalKill;
	BYTE m_nBlueTeamTotalKill;

	//FLOAT Time;   //���߿� �ð� �־ �����ش�.
};

struct SC_Respawn
{
	BYTE size;
	BYTE type;
	BYTE id;


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

	char characterID[20];
};

struct sc_occupy
{
	BYTE size;
	BYTE type;
	BYTE OccupyTeam;
};

struct SC_Occupy_Timer
{
	BYTE size;
	BYTE type;

	float Occupy_timer;

};

struct sc_change_scene
{
	BYTE size;
	BYTE type;
	BYTE Winner;
};

struct cs_round_over
{
	BYTE size;
	BYTE type;
	BYTE OccupyTeam;
};


struct cs_weapon_type
{
	BYTE size;
	BYTE type;
	BYTE Weapontype;

};

struct sc_weapon_type
{
	BYTE size;
	BYTE type;
	BYTE id;
	BYTE Weapontype;

};

#pragma pack(pop)