#include "stdafx.h"
#include "protocol.h"
#include "ServerManager.h"

void CServerManager::processpacket(char *ptr)
{
	static bool first_time = true;
	int id = 0;

	switch (ptr[1]) {
		if (SCENE_MGR->g_nowScene->GetSceneTag() == SceneTag::eMainScene) {
			case ePacket_Update:   //계속 받을때
			{
				sc_packet_pos*         my_Pos_packet;
				my_Pos_packet = reinterpret_cast<sc_packet_pos *>(ptr);
				id = my_Pos_packet->id;
				if (id == m_myid)
				{
					return;
				}
				else
				{
					int i = 0;
					for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer())
					{
						if (character->GetServerID() == id)
							break;
						i++;
					}
		
					XMVECTOR Animation = XMVectorZero();
					if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eReload))
						SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsReload(true);
	
					if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eRun))
						SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsRun(true);
					else
						SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsRun(false);
		
					if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eLeftMouse))
						SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsFire(true);
					else
						SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsFire(false);

					if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eCrouch))
						SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsCrouch(true);
					else
						SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsCrouch(false);	
					
					SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetPosition(XMVectorSet(my_Pos_packet->x, my_Pos_packet->y, my_Pos_packet->z, 0.0f));
					
					
					if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eLeft))
						Animation += XMVectorSet(-1, 0, 0, 0);
		
					if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eRight))
						Animation += XMVectorSet(1, 0, 0, 0);

					if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eForward))
						Animation += XMVectorSet(0, 0, 1, 0);

					if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eBackward))
						Animation += XMVectorSet(0, 0, -1, 0);

					XMFLOAT3 Temp;
					XMStoreFloat3(&Temp, Animation);
					SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetRelativeVelocity(Temp);
				}
			}
			break;	
			case ePacket_CreateOthderPlayer:   //처음 받았을때.
			{
				sc_packet_put_player*   my_put_packet;
				my_put_packet = reinterpret_cast<sc_packet_put_player *>(ptr);
				id = my_put_packet->id;
		
				if (first_time) {
					m_myid = id;
		
					first_time = false;
				}
				// ================================================================================================================ //
				// - 05. 10 - 레이캐스트 세부 충돌 구현 중
				// 서버에서 생성한 아이디를 클라이언트에도 해당 똑같이 적용하기 위해서 ServerID 만들었다 이거 짚고 넘어가
				// ServerID는 클라이언트 자체적으로 ID만 가지고 어떤 객체인지 알 수 있게 만든거임 
				// ObjectID는 클라이언트 자체 ID이니까 ServerID랑 구분 하셈!
		
				// 그리고 이제 여러 캐릭터 받도록 if문 수정해야겠다.
				// ================================================================================================================ //
		
				if (id == m_myid) {
					SCENE_MGR->g_pPlayer->SetPosition(XMFLOAT3(my_put_packet->x, my_put_packet->y, my_put_packet->z));
					SCENE_MGR->g_pPlayer->SetPlayerlife(static_cast<UINT>(my_put_packet->hp));
					SCENE_MGR->g_pPlayerCharacter->SetServerID(id);
					SCENE_MGR->g_pPlayerCharacter->SetID(my_put_packet->characterID);
					
					SCENE_MGR->g_pPlayerCharacter->SetTeamTag(static_cast<TeamTag>(my_put_packet->Team));
					SCENE_MGR->g_pMainScene->SetGameMode(static_cast<GameMode>(my_put_packet->mode));
					SCENE_MGR->g_pPlayerCharacter->CreateMaterial();
				}
				else
				{
					CTerroristCharacterObject *pCharObject = new CTerroristCharacterObject(static_cast<TeamTag>(my_put_packet->Team));
					pCharObject->CreateObjectData(STATEOBJ_MGR->g_pd3dDevice);
					pCharObject->SetPosition(XMVectorSet(my_put_packet->x, my_put_packet->y, my_put_packet->z, 0.0f));
					pCharObject->SetLife(static_cast<UINT>(my_put_packet->hp));
					pCharObject->SetServerID(id);
					pCharObject->SetID(my_put_packet->characterID);
					SCENE_MGR->g_pMainScene->SetGameMode(static_cast<GameMode>(my_put_packet->mode));
					SCENE_MGR->g_pMainScene->GetCharcontainer().push_back(pCharObject);
					SCENE_MGR->g_pMainScene->GetBbBoxcontainer().push_back(pCharObject);
		
					COLLISION_MGR->m_vecCharacterContainer.push_back(pCharObject);
				}
				SCENE_MGR->g_pMainScene->SetRedTeamKill(static_cast<UINT>(my_put_packet->RED));
				SCENE_MGR->g_pMainScene->SetBlueTeamKill(static_cast<UINT>(my_put_packet->Blue));
			}
			break;
			case ePacket_MouseRotate:   //rotate된 값 처리
			{
				sc_rotate_vector*      my_put_rotate;
				my_put_rotate = reinterpret_cast<sc_rotate_vector *>(ptr);
				id = my_put_rotate->id;
		
				if (id == m_myid) {
				}
				else
				{
					int i = 0;
					for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer())
					{
						if (character->GetServerID() == id)
							break;
						i++;
					}
					
					SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetRotate(my_put_rotate->x, my_put_rotate->y, 0);
					SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetFireDirection(my_put_rotate->FireDirection);
				}
			}
			break;
		
			case ePacket_CollisionCheck:	// 총알 충돌 체크
			{
				SC_Collison*         my_collision;
				my_collision = reinterpret_cast<SC_Collison*>(ptr);
		
				// 서버로부터 받은 아이디를 컨테이너에서 같은 아이디를 찾으면 해당 객체가 충돌 객체가 된다.
				// 그 객체의 바운딩 박스를 검사해서 충돌 확인하고 서버로 정보를 보내준다.
				CollisionInfo info;
				bool bIsPartsCollisionCS = false;
				for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer()) {
					if (character->GetServerID() == my_collision->hitCharacterID)
						info.m_pHitObject = character;
				}

				bIsPartsCollisionCS = COLLISION_MGR->RayCastCollisionToCharacter_Parts(info, XMLoadFloat3(&my_collision->position), XMLoadFloat3(&my_collision->direction));
		
				if (bIsPartsCollisionCS) {
					CS_FinalCollison Collison;
					Collison.hitParts = static_cast<BYTE>(info.m_HitParts);
					Collison.type = CS_HEAD_HIT;
					Collison.size = sizeof(CS_FinalCollison);
					Collison.fireCharacterID = my_collision->fireCharacterID;
					Collison.hitCharacterID = my_collision->hitCharacterID;
					Collison.collisionDistance = info.m_fDistance;
					Collison.position = my_collision->position;
					Collison.direction = my_collision->direction;
					Collison.weaponType = my_collision->weaponType;

					SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&Collison));
				}
			}
			break;
			case ePacket_HP:
			{
				SC_Player_Hp *packet;
				packet = reinterpret_cast<SC_Player_Hp *>(ptr);
				id = packet->m_nCharacterID;
		
				if (id == m_myid) {
					SCENE_MGR->g_pPlayerCharacter->SetLife(packet->m_nLife);
					SCENE_MGR->g_pPlayerCharacter->SetArmorPoint(packet->m_nArmorPoint);
					SCENE_MGR->g_pPlayerCharacter->SetIsHeadHit(packet->m_bIsHeadHit);
				}
				else {
					int i = 0;
					for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer())
					{
						if (character->GetServerID() == id)
							break;
						i++;
					}
					SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetLife(packet->m_nLife);
					SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetArmorPoint(packet->m_nArmorPoint);
					SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsHeadHit(packet->m_bIsHeadHit);
				}
			}
			break;
		
			case ePacket_KillUpdate:
			{
				SC_System_kill* packet;
				packet = reinterpret_cast<SC_System_kill *>(ptr);
		
				assert(packet->m_nRedTeamTotalKill < 128 && packet->m_nBlueTeamTotalKill < 128);
		
				SCENE_MGR->g_pMainScene->SetRedTeamKill(static_cast<UINT>(packet->m_nRedTeamTotalKill));
				SCENE_MGR->g_pMainScene->SetBlueTeamKill(static_cast<UINT>(packet->m_nBlueTeamTotalKill));
			}
			break;
		
			case ePacket_GameTimer:   //게임 타이머
			{
				SC_Starting_Timer*   packet;
				packet = reinterpret_cast<SC_Starting_Timer *>(ptr);
				SCENE_MGR->g_pMainScene->SetGameTime(packet->Starting_timer);
			}
			break;
			case ePacket_Respawn:   //리스폰
			{
				SC_Respawn *packet;
				packet = reinterpret_cast<SC_Respawn *>(ptr);
				id = packet->id;

				if (id == m_myid){
					SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->SetPosition(XMVectorSet(packet->m_f3Position.x, packet->m_f3Position.y, packet->m_f3Position.z, 0));
					SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->Revival();
				}
				else
				{
					int findCharacterID = 0;
					for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer()) {
						if (character->GetServerID() == id)
							break;
						findCharacterID++;
					}
					SCENE_MGR->g_pMainScene->GetCharcontainer()[findCharacterID]->SetPosition(packet->m_f3Position);
					SCENE_MGR->g_pMainScene->GetCharcontainer()[findCharacterID]->Revival();
				}
			}
			break;

			case ePacket_OccupyTeam:	//어떤팀이 점령했는지 보낸다.
			{
				sc_occupy *packet = reinterpret_cast<sc_occupy *>(ptr);
				if(packet->OccupyTeam == static_cast<int>(TeamTag::eRedTeam))
					SCENE_MGR->g_pMainScene->SetOccupyTeam(TeamTag::eRedTeam);
				else
					SCENE_MGR->g_pMainScene->SetOccupyTeam(TeamTag::eBlueTeam);
				SCENE_MGR->g_pMainScene->SetOccupyTime(0);
			}
			break;
		
			case ePacket_DamageInfo:	// 피격 UI 표현용 패킷
			{
				SC_Damegedirection* packet;
				packet = reinterpret_cast<SC_Damegedirection *>(ptr);
		
				SCENE_MGR->g_pPlayer->SetDamagedInfo(DamagedInfo(true, packet->position));
			}
			break;
	}
	case ePacket_SuccessMyCharacter:	//방에 들어가지면 받고 신변경해주고 서버에 정보 넣어달라고 보낸다.
	{
		SCENE_MGR->ChangeScene(SceneTag::eLoadingScene);
	}
	break;

	case ePacket_Disconnect :	// 다른클라이언트 종료시 벡터에서 팝해준다.
	{
		sc_packet_remove_player *my_packet = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int other_id = my_packet->id;
		if (other_id == m_myid) {
			//상태를 없애준다.
		}
		else
		{
			int i = 0;
			auto& container = SCENE_MGR->g_pMainScene->GetCharcontainer();
			for (auto iter = begin(container); iter != end(container); ++iter)
			{
				if ((*iter)->GetServerID() != id) continue;
				delete *iter;
				container.erase(iter);
				break;
			}
		}
	}
	break;

	case ePacket_WeaponReplace:	//Weapon_Change.
	{
		sc_weapon_type *my_packet = reinterpret_cast<sc_weapon_type *>(ptr);

		int findCharacterID = 0;
		for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer()) {
			if (character->GetServerID() == my_packet->id)
				break;
			findCharacterID++;
		}

		SCENE_MGR->g_pMainScene->GetCharcontainer()[findCharacterID]->ReplaceWeapon(static_cast<WeaponTag>(my_packet->Weapontype));
	}
	break;

	case ePacket_KillLog: 
	{
		SC_KillLog* my_packet = reinterpret_cast<SC_KillLog *>(ptr);

		int findKiller = 0;
		int findDiedPlayer = 0;
		for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer()) {
			if (character->GetServerID() == my_packet->killerPlayer)
				break;
			findKiller++;
		}
		for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer()) {
			if (character->GetServerID() == my_packet->diePlayer)
				break;
			findDiedPlayer++;
		}

		SCENE_MGR->g_pMainScene->CreateKillLog(SCENE_MGR->g_pMainScene->GetCharcontainer()[findKiller], SCENE_MGR->g_pMainScene->GetCharcontainer()[findDiedPlayer]->GetID(), my_packet->bIsHead);
	}
	break;
	case ePacket_Particle:
	{
		SC_Particle* packet = reinterpret_cast<SC_Particle *>(ptr);

		PARTICLE_MGR->CreateParticle(static_cast<ParticleTag>(packet->particleType), XMLoadFloat3(&packet->position));
	}
	break;

	default:
		std::cout << "Unknown PACKET type :" << (int)ptr[1] << "\n";
		Sleep(2000);
		break;
	}
}

void CServerManager::ReadPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret) {
		int err_code = WSAGetLastError();
		printf("Recv Error [%d]\n", err_code);
	}

	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (iobyte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			processpacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			iobyte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, iobyte);
			saved_packet_size += iobyte;
			iobyte = 0;
		}
	}
}

void CServerManager::error_display(char *msg, int err_num)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_num,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void CServerManager::Server_init()
{
#ifdef USE_AUTOIP
	char ip[20];
	GetPrivateProfileStringA("Server", "IP", "127.0.0.1", ip, 20, "./Config.Ini");
#else
	char ip[20];		
	cout << " ip 입력 : ";
	cin >> ip;
#endif



	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	g_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	char opt_val = TRUE;
	setsockopt(g_socket, IPPROTO_TCP, TCP_NODELAY, &opt_val, sizeof(opt_val));

	SOCKADDR_IN Serveraddr;
	ZeroMemory(&Serveraddr, sizeof(SOCKADDR_IN));
	Serveraddr.sin_family = AF_INET;
	Serveraddr.sin_port = htons(ServerPort);
	Serveraddr.sin_addr.s_addr = inet_addr(ip);

	int Result = WSAConnect(g_socket, (sockaddr *)&Serveraddr, sizeof(Serveraddr), NULL, NULL, NULL, NULL);

	if (Result == SOCKET_ERROR) {
		cout << "연결안됨 !!";
		Sleep(2000);
		exit(1);
	}
	WSAAsyncSelect(g_socket, m_handle, WM_SOCKET, FD_CLOSE | FD_READ);

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = BUF_SIZE;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = BUF_SIZE;

	
}

void CServerManager::Sendpacket(unsigned char* Data)
{
	DWORD iobyte;

	send_wsabuf.len = (size_t)Data[0];
	memcpy(send_buffer, Data, (size_t)Data[0]);
	send_wsabuf.buf = send_buffer;


	int retval = WSASend(g_socket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);

	if (retval) {
		int error_code = WSAGetLastError();
		printf("Error while sending packet [%d]", error_code);
	}
	Sleep(1);


	//cout<<"packet type : " << (int)Data[1] << endl;
}
