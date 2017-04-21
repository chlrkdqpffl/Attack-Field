#pragma once


#define ServerPort			9000
#define BUF_SIZE			1024
#define	WM_SOCKET			WM_USER + 1


class ServerFuntion
{
private:

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
	ServerFuntion();
	~ServerFuntion();


	
	void sethandle(HWND handle) { m_handle = handle; }
	void Server_init();
	void Sendpacket(unsigned char* Data);
	void ReadPacket(SOCKET sock);
	void processpacket(char *ptr);
	void error_display(char *msg, int err_num);



};

