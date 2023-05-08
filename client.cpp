#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>
#include <chrono>   

#pragma comment(lib,"ws2_32.lib")

using namespace std;
using namespace chrono;

// .\client.exe <server_ip> <server_port> 

int64_t  mytime()
{
    static int64_t  t;
    return ++t;
}

void mysend(SOCKET sockClient, sockaddr_in addr, int cnt)
{
    auto start = system_clock::now();
    //发送数据
    char buf[] = "ping";
    int dwSent = sendto(sockClient, buf, strlen(buf), 0, (SOCKADDR *)&addr, sizeof(SOCKADDR));
    if (dwSent == 0)
    {
        printf("send %s failed\n", buf);
        exit(-1);
    }

    sockaddr_in addrSever = {};
    int nServerAddrLen=sizeof(sockaddr_in);

    // 接收数据
    char recvBuf[32] = {};
    int dwRecv = recvfrom(sockClient, recvBuf, sizeof(recvBuf), 0, (SOCKADDR *)&addrSever,&nServerAddrLen);
    auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    printf("%d: %lld us\n", cnt, duration.count());
}

int main(int argc, char* argv[])
{
	//初始化网络环境
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSAStartup failed\n");
		return -1;
	}

    
    

    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)atoi(argv[2]));
    addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    int test_time = atoi(argv[3]);

    for(int i = 0; i < test_time; i++) {
        printf("%d\n", i);
        //建立一个UDP的socket
        SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockClient == INVALID_SOCKET)
        {
            printf("create socket failed\n");
            return -1;
        }
        // 申明一个网络地址信息的结构体，保存服务器的地址信息
        
        mysend(sockClient, addr, 1);// solve route can take longer time
        mysend(sockClient, addr, 2);
        mysend(sockClient, addr, 3);

        //关闭SOCKET连接
        closesocket(sockClient);
        //Sleep(100);
    }

	//清理网络环境
	WSACleanup();
	return 0;
}