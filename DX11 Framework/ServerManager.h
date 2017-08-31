#pragma once
#include "SingletonManager.h"

#define ServerPort			9000
#define BUF_SIZE			1024
#define	WM_SOCKET			WM_USER + 1

class CServerManager : public CSingletonManager<CServerManager>
{
	SOCKET				m_socket;
	HWND				m_handle;

	WSABUF				send_wsabuf;
	WSABUF				recv_wsabuf;

	char				send_buffer[BUF_SIZE];
    char				recv_buffer[BUF_SIZE];

	DWORD				in_packet_size = 0;
	int					saved_packet_size = 0;
	char				packet_buffer[BUF_SIZE];

	int					m_myid = 0;
public:
	CServerManager() {};
	virtual ~CServerManager() {};
	
	virtual void InitializeManager() {}
	virtual void ReleseManager() {}

	void sethandle(HWND handle) { m_handle = handle; }
	void Server_init();
	void ReadPacket(SOCKET sock);
	void processpacket(char *ptr);
	void error_display(char *msg, int err_num);
	void Sendpacket(unsigned char* Data);

	int GetId() const { return m_myid; }
	char * GetSendbuffer() { return send_buffer; }
	WSABUF GetWsaSendBuf() { return send_wsabuf; }
	void SetWsaSendbufferlen(int size) { send_wsabuf.len = size; }
};