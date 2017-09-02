#include "stdafx.h"
#include "protocol.h"
#include "ServerManager.h"

void CServerManager::processpacket(char *ptr)
{
	static bool first_time = true;
	int id = 0;

	switch (ptr[1])
	{
	case 1:   //��� ������
	{
		sc_packet_pos*         my_Pos_packet;
		my_Pos_packet = reinterpret_cast<sc_packet_pos *>(ptr);
		id = my_Pos_packet->id;
		if (id == m_myid)
		{
			SCENE_MGR->g_pPlayerCharacter->SetLife(my_Pos_packet->hp);
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

			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetPosition(XMVectorSet(my_Pos_packet->x, my_Pos_packet->y, my_Pos_packet->z, 0.0f));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetRelativeVelocity(my_Pos_packet->Animation);
			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetLife(my_Pos_packet->hp);

			if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eReload))
				SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsReload(true);
			else
				SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsReload(false);

			if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eRun))
				SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsRun(true);
			else 
				SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsRun(false);

	//		if (my_Pos_packet->key_button & static_cast<int>(KeyInput::eLeftMouse))
	//			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsFire(true);
	//		else
	//			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsFire(false);

			//SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsRun(false);
		}
	}
	break;
	case 2:   //ó�� �޾�����.
	{
		sc_packet_put_player*   my_put_packet;
		my_put_packet = reinterpret_cast<sc_packet_put_player *>(ptr);
		id = my_put_packet->id;

		

		if (first_time)
		{
			m_myid = id;

			first_time = false;
		}
		// ================================================================================================================ //
		// - 05. 10 - ����ĳ��Ʈ ���� �浹 ���� ��
		// �������� ������ ���̵� Ŭ���̾�Ʈ���� �ش� �Ȱ��� �����ϱ� ���ؼ� ServerID ������� �̰� ¤�� �Ѿ
		// ServerID�� Ŭ���̾�Ʈ ��ü������ ID�� ������ � ��ü���� �� �� �ְ� ������� 
		// ObjectID�� Ŭ���̾�Ʈ ��ü ID�̴ϱ� ServerID�� ���� �ϼ�!

		// �׸��� ���� ���� ĳ���� �޵��� if�� �����ؾ߰ڴ�.
		// ================================================================================================================ //

		if (id == m_myid)
		{
			//SCENE_MGR->g_pMainScene->GetCharcontainer()[id]->SetPosition(XMVectorSet(my_put_packet->x, my_put_packet->y, my_put_packet->z, 0.0f));

			SCENE_MGR->g_pPlayer->SetPosition(XMVectorSet(my_put_packet->x, my_put_packet->y, my_put_packet->z, 0.0f));
			SCENE_MGR->g_pPlayer->SetPlayerlife(static_cast<UINT>(my_put_packet->hp));//SCENE_MGR->g_pMainScene->-> SetLife(static_cast<UINT>(my_put_packet->hp));
			SCENE_MGR->g_pPlayerCharacter->SetServerID(id);
			SCENE_MGR->g_pPlayerCharacter->SetTagTeam(reinterpret_cast<TeamType &>(my_put_packet->Team));
			SCENE_MGR->g_pPlayerCharacter->Setmode(my_put_packet->mode);
			SCENE_MGR->g_pPlayerCharacter->CreateMaterial();
		}
		else
		{
			CTerroristCharacterObject *pCharObject = new CTerroristCharacterObject(static_cast<TeamType>(my_put_packet->Team));   //��ü ����
			pCharObject->CreateObjectData(STATEOBJ_MGR->g_pd3dDevice);
			pCharObject->SetPosition(XMVectorSet(my_put_packet->x, my_put_packet->y, my_put_packet->z, 0.0f));

			pCharObject->SetRelativeVelocity(my_put_packet->Animation);
			pCharObject->SetLife(static_cast<UINT>(my_put_packet->hp));
			pCharObject->SetServerID(id);
			pCharObject->Setmode(my_put_packet->mode);

			SCENE_MGR->g_pMainScene->GetCharcontainer().push_back(pCharObject);
			SCENE_MGR->g_pMainScene->GetBbBoxcontainer().push_back(pCharObject);

			COLLISION_MGR->m_vecCharacterContainer.push_back(pCharObject);
			/*
			UINT index;
			int i = 0;
			for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer()) {
			if (character->GetServerID() == my_put_packet->id) {
			index = i;
			}
			i++;
			}
			*/

		}

		SCENE_MGR->g_pMainScene->SetRedTeamKill(static_cast<UINT>(my_put_packet->RED));
		SCENE_MGR->g_pMainScene->SetBlueTeamKill(static_cast<UINT>(my_put_packet->Blue));

	}



	break;
	case 3:   // �� �߻��
	{
		sc_bullet_fire*         my_put_bulletfire;
		my_put_bulletfire = reinterpret_cast<sc_bullet_fire *>(ptr);
		id = my_put_bulletfire->id;

		if (id == m_myid)
		{
			//SCENE_MGR->g_pPlayer->m_pCharacter->Firing();
		}
		else
		{
			if (my_put_bulletfire->fire == true)
			{
				int i = 0;
				for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer())
				{
					if (character->GetServerID() == id)
						break;
					i++;
				}
				SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsFire(true);
				SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetFireDirection(my_put_bulletfire->FireDirection);
			}
		}
	}
	break;

	case 4:	// �ٸ�Ŭ���̾�Ʈ ����� ���Ϳ��� �����ش�.
	{
		sc_packet_remove_player *my_packet = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int other_id = my_packet->id;
		if (other_id == m_myid) {
			//���¸� �����ش�.
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
	case 5:   //rotate�� �� ó��
	{
		sc_rotate_vector*      my_put_rotate;
		my_put_rotate = reinterpret_cast<sc_rotate_vector *>(ptr);
		id = my_put_rotate->id;

		if (id == m_myid)
		{

			//SCENE_MGR->g_pPlayerCharacter->SetRotate(my_put_rotate->x, my_put_rotate->y, my_put_rotate->z);
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

			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetRotate(my_put_rotate->x, my_put_rotate->y, my_put_rotate->z);
		}
	}
	break;

	case 6:	// �Ѿ� �浹 üũ
	{
		SC_Collison*         my_collision;
		my_collision = reinterpret_cast<SC_Collison*>(ptr);

		// �����κ��� ���� ���̵� �����̳ʿ��� ���� ���̵� ã���� �ش� ��ü�� �浹 ��ü�� �ȴ�.
		// �� ��ü�� �ٿ�� �ڽ��� �˻��ؼ� �浹 Ȯ���ϰ� ������ ������ �����ش�.
		CollisionInfo info;
		bool bIsPartsCollisionCS = false;
		for (auto& character : SCENE_MGR->g_pMainScene->GetCharcontainer()) {
			if (character->GetServerID() == my_collision->id)
				info.m_pHitObject = character;
		}
		
		bIsPartsCollisionCS = COLLISION_MGR->RayCastCollisionToCharacter_Parts(info, XMLoadFloat3(&my_collision->position), XMLoadFloat3(&my_collision->direction));

		if (bIsPartsCollisionCS) {
			CS_Head_Collison Collison;
			Collison.Head = false;
			Collison.type = CS_HEAD_HIT;
			Collison.size = sizeof(CS_Head_Collison);
			Collison.id = my_collision->id;
			Collison.position = my_collision->position;
			Collison.direction = my_collision->direction;

			if (info.m_HitParts == ChracterBoundingBoxParts::eHead)
				Collison.Head = true;

			SERVER_MGR->Sendpacket(reinterpret_cast<unsigned char *>(&Collison));
		}
	}
	break;
	case 7:
	{
		SC_Player_Hp *packet;
		packet = reinterpret_cast<SC_Player_Hp *>(ptr);
		id = packet->m_nCharacterID;

		if (id == m_myid)
		{
			SCENE_MGR->g_pPlayerCharacter->SetLife(packet->m_nLife);
			SCENE_MGR->g_pPlayerCharacter->SetIsHeadHit(packet->m_bIsHeadHit);
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
			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetLife(packet->m_nLife);
			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsHeadHit(packet->m_bIsHeadHit);
		}
	}
	break;
	case 8:
	{
		SC_System_kill* packet;
		packet = reinterpret_cast<SC_System_kill *>(ptr);

		assert(packet->m_nRedTeamTotalKill < 128 && packet->m_nBlueTeamTotalKill < 128);

		SCENE_MGR->g_pMainScene->SetRedTeamKill(static_cast<UINT>(packet->m_nRedTeamTotalKill));
		SCENE_MGR->g_pMainScene->SetBlueTeamKill(static_cast<UINT>(packet->m_nBlueTeamTotalKill));
	}
	break;
	case 9:   //���� Ÿ�̸�
	{
		SC_Starting_Timer*   packet;
		packet = reinterpret_cast<SC_Starting_Timer *>(ptr);
		SCENE_MGR->g_pMainScene->SetGameTime(packet->Starting_timer);
		break;
	}
	case 11:   //������
	{
		SC_Respawn *packet;
		packet = reinterpret_cast<SC_Respawn *>(ptr);
		id = packet->id;
		if (id == m_myid)
		{
			SCENE_MGR->g_pPlayer->SetPosition(XMVectorSet(packet->m_f3Position.x, packet->m_f3Position.y, packet->m_f3Position.z, 0));
			SCENE_MGR->g_pPlayerCharacter->SetIsRespawn(packet->m_bIsRespawn);
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

			if (packet->m_bIsRespawn)
			{
				cout << "true !!" << endl;
			}
			else
			{
				cout << "false" << endl;
			}

			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetPosition(packet->m_f3Position);
			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetIsRespawn(packet->m_bIsRespawn);
		}
		break;
	}
	case 13:	//�α��� ���н� Ŭ���̾�Ʈ����
	{
		SC_login_CONNECT *packet;
		packet = reinterpret_cast<SC_login_CONNECT *>(ptr);
		id = packet->id;
		if (!packet->connect)
		{
			SCENE_MGR->m_loginfail = true;
			exit(1);
			//PostQuitMessage(0);
		}
		break;
	}
	case 14:	//�ӽ�
	{
		cs_temp_exit *packet;
		packet = reinterpret_cast<cs_temp_exit *>(ptr);

		SCENE_MGR->ChangeScene(SceneTag::eWaitScene);


		break;
	}

	case 15:	//�濡 ������ �ް� �ź������ְ� ������ ���� �־�޶�� ������.
	{
		SCENE_MGR->ChangeScene(SceneTag::eLoadingScene);
		
		cs_create_charter sendpacket;

		sendpacket.size = sizeof(cs_create_charter);
		sendpacket.type = 8;
		break;
	}

	case 16:	//����� �����ߴ��� ������.
	{
		sc_occupy *packet = reinterpret_cast<sc_occupy *>(ptr);
		SCENE_MGR->g_pMainScene->SetOccupyTeam(packet->redteam);	
		break;
	}

	case 17:
	{		
		SC_Occupy_Timer* packet;
		packet = reinterpret_cast<SC_Occupy_Timer *>(ptr);

		
		SCENE_MGR->g_pMainScene->SetOccupyTime(packet->Occupy_timer);
		break;
	}
	case 18:	//������?
	{
		SC_Damegedirection* packet;
		packet = reinterpret_cast<SC_Damegedirection *>(ptr);

		SCENE_MGR->g_pPlayer->SetDamagedInfo(DamagedInfo(true, packet->position));
		break;
	}
	case 19:	//�������� ü�� 100�� ������ ��Ŷ�� ���� ����.
	{
		SC_fullHPpacket *packet;
		packet = reinterpret_cast<SC_fullHPpacket *>(ptr);
		id = packet->id;

		if (id == m_myid)
		{
			SCENE_MGR->g_pPlayerCharacter->SetLife(packet->Hp);
			SCENE_MGR->g_pPlayerCharacter->SetIsRespawn(packet->live);
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
			SCENE_MGR->g_pMainScene->GetCharcontainer()[i]->SetLife(packet->Hp);
			SCENE_MGR->g_pPlayerCharacter->SetIsRespawn(packet->live);
		}
	
		break;
	}
	default:
		std::cout << "Unknown PACKET type :" << (int)ptr[1] << "\n";
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
	cout << " ip �Է� : ";
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
		cout << "����ȵ� !!";
		Sleep(3000);
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
