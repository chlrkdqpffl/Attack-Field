#pragma once

#include "stdafx.h"
#include "Protocol.h"
#include "ServerFuntion.h"
#include "GameFramework.h"



#define ServerPort			9000
#define BUF_SIZE			1024
#define	WM_SOCKET			WM_USER + 1

CGameFramework GameFramework;

ServerFuntion::ServerFuntion()
{

}

ServerFuntion::~ServerFuntion()
{

}
void ServerFuntion::processpacket(char *ptr)
{
	static bool first_time = true;
	sc_packet_pos*			my_Pos_packet;
	sc_packet_put_player*	my_put_packet;
	sc_rotate_vector*		my_put_rotate;
	sc_bullet_fire*			my_put_bulletfire;

	int id = 0;
	switch (ptr[1])
	{
	case 1:	//계속 받을때
		my_Pos_packet = reinterpret_cast<sc_packet_pos *>(ptr);
		id = my_Pos_packet->id;

		if (id == m_myid)
		{
			//memcpy(my_Pos_packet, my_Pos_packet, ptr[0]);
			//	SCENE_MGR->g_pPlayer->SetVelocity(XMFLOAT3(my_Pos_packet->x, my_Pos_packet->y, my_Pos_packet->z));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->SetPosition(XMVectorSet(my_Pos_packet->x, my_Pos_packet->y, my_Pos_packet->z, 0.0f));;
			SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->SetAnimation(static_cast<AnimationData::CharacterAnim>(my_Pos_packet->Animation));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->SetHp(my_Pos_packet->hp);

		}
		else
		{
			//SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->SetVelocity(XMFLOAT3(my_Pos_packet->x, my_Pos_packet->y, my_Pos_packet->z));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->SetPosition(XMFLOAT3(my_Pos_packet->x, my_Pos_packet->y, my_Pos_packet->z));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->SetAnimation(static_cast<AnimationData::CharacterAnim>(my_Pos_packet->Animation));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->SetHp(my_Pos_packet->hp);


		}
		break;
	case 2:	//처음 받았을때.
	{
		my_put_packet = reinterpret_cast<sc_packet_put_player *>(ptr);
		id = my_put_packet->id;
		if (first_time)

		{
			m_myid = id;

			first_time = false;
		}
		if (id == m_myid)
		{
			//SCENE_MGR->g_pMainScene->GetCharcontainer()[id]->SetPosition(XMVectorSet(my_put_packet->x, my_put_packet->y, my_put_packet->z, 0.0f));

			//SCENE_MGR->g_pPlayer->SetVelocity(XMFLOAT3(my_put_packet->x, my_put_packet->y, my_put_packet->z));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->SetPosition(XMFLOAT3(my_put_packet->x, my_put_packet->y, my_put_packet->z));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->SetAnimation(static_cast<AnimationData::CharacterAnim>(my_put_packet->Animation));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->SetHp(my_put_packet->hp);
		}
		else 
		{
			SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->SetPosition(XMFLOAT3(my_put_packet->x, my_put_packet->y, my_put_packet->z));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->SetAnimation(static_cast<AnimationData::CharacterAnim>(my_put_packet->Animation));
			SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->SetHp(my_put_packet->hp);
		}
	}
	break;
	case 3:	//총알...
 		my_put_bulletfire = reinterpret_cast<sc_bullet_fire *>(ptr);
		id = my_put_bulletfire->id;

		if (id == m_myid)
		{
			//SCENE_MGR->g_pPlayer->m_pCharacter->Firing();
		}
		else
		{
			if(my_put_bulletfire->fire == true)
				SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->Firing();
		}
		break;

	case 4:
	{
		sc_packet_remove_player *my_packet = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int other_id = my_packet->id;
		if (other_id == m_myid) {
			//상태를 없애준다.
		}
	}
	break;
	case 5:	//rotate된 값 처리
		
		my_put_rotate = reinterpret_cast<sc_rotate_vector *>(ptr);
		id = my_put_rotate->id;

		if (id == m_myid)
		{
			//cout << "나다 : " << id << endl;
			//cout << my_put_rotate->x << " " << my_put_rotate->y << " " << my_put_rotate->z << endl;
	//		SCENE_MGR->g_pPlayer->setradian(my_put_rotate->x, my_put_rotate->y);
			SCENE_MGR->g_pMainScene->GetCharcontainer()[0]->SetRotate(my_put_rotate->x, my_put_rotate->y, 0);

			//SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->SetRotate(my_put_rotate->x, my_put_rotate->y, my_put_rotate->z);
		}
		else
		{
			//cout << "니다 : " << id << endl;
			//cout << my_put_rotate->x << " " << my_put_rotate->y << " " << my_put_rotate->z << endl;
			//SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->SetRotate(my_put_rotate->x, my_put_rotate->y, my_put_rotate->z);

			
			SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->SetRotate(my_put_rotate->x, my_put_rotate->y, my_put_rotate->z);

	//		ShowXMVector(SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->GetRight());
	//		ShowXMVector(SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->GetUp());
	//		ShowXMVector(SCENE_MGR->g_pMainScene->GetCharcontainer()[1]->GetLook());
		//	cout << my_put_rotate->x << ", " << my_put_rotate->y << ", " << my_put_rotate->z << endl;
		}
		break;
	default:
		std::cout << "Unknown PACKET type :" << (int)ptr[1] << "\n";
		break;



	}



	//cout << GameFramework.GetCamera().GetPlayer()->GetPosition().x<<" "<< GameFramework.GetCamera().GetPlayer()->GetPosition().y<<" "<< GameFramework.GetCamera().GetPlayer()->GetPosition().z <<endl;

}
void ServerFuntion::ReadPacket(SOCKET sock)
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

void ServerFuntion::error_display(char *msg, int err_num)
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

void ServerFuntion::Server_init()
{
	std::cout << " ip 입력 : ";
	char ip[20] = "127.0.0.1";
	rewind(stdin);
	//std::cin.clear();
	//std::cin >> ip;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	g_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	SOCKADDR_IN Serveraddr;
	ZeroMemory(&Serveraddr, sizeof(SOCKADDR_IN));
	Serveraddr.sin_family = AF_INET;
	Serveraddr.sin_port = htons(ServerPort);
	Serveraddr.sin_addr.s_addr = inet_addr(ip);

	int Result = WSAConnect(g_socket, (sockaddr *)&Serveraddr, sizeof(Serveraddr), NULL, NULL, NULL, NULL);

	if (Result == SOCKET_ERROR) {
		cout << "연결안됨 !!";
		Sleep(3000);
		exit(1);
	}

	

	WSAAsyncSelect(g_socket, m_handle, WM_SOCKET, FD_CLOSE | FD_READ);


	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = BUF_SIZE;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = BUF_SIZE;


}

void ServerFuntion::Sendpacket(unsigned char* Data)
{
	DWORD iobyte;

	send_wsabuf.len = (size_t)Data[0];
	memcpy(send_buffer, Data, (size_t)Data[0]);
	send_wsabuf.buf = reinterpret_cast<char *>(send_buffer);

	 
	int retval = WSASend(g_socket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);

	if (retval) {
		int error_code = WSAGetLastError();
		printf("Error while sending packet [%d]", error_code);
	}
	Sleep(1);

	//cout<<"packet type : " << (int)Data[1] << endl;
}

