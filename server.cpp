#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>
 
#pragma comment(lib,"ws2_32.lib")
 
// .\server.exe <port> 
int main(int argc, char* argv[])
{
	//初始化网络环境
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSAStartup failed\n");
		return -1;
	}
 
	//建立一个UDP的socket
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == SOCKET_ERROR)
	{
		printf("create socket failed\n");
		return -1;
	}
 
	//绑定地址信息
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((unsigned short)atoi(argv[1]));
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
 
	bind(sock, (sockaddr*)&serverAddr, sizeof(sockaddr));
	
	char buf[32];
    char back_msg[] = "pong";

    printf("listening\n");
	while (TRUE)
	{
		memset(buf, 0, sizeof(buf));
		// 网络节点的信息，用来保存客户端的网络信息
		sockaddr_in clientAddr;
		memset(&clientAddr, 0, sizeof(sockaddr_in));
 
		int clientAddrLen = sizeof(sockaddr);
		//接收客户端发来的数据
		int ret = recvfrom(sock, buf, sizeof(buf), 0,(sockaddr*) &clientAddr,&clientAddrLen );
		
		// 发一个数据包返回给客户
		sendto(sock, back_msg, strlen(back_msg), 0, (sockaddr*)&clientAddr, clientAddrLen);

        //printf("Recv msg:%s from IP:[%s] Port:[%d]\n", buf, inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));
		//printf("Send msg back to IP:[%s] Port:[%d]\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	}
	return 0;
}