#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CLINT_MAX 10
#define TRUE 1
#define FALSE 0

int g_clnt_socks[CLINT_MAX];
int g_clnt_count = 0;

int main(int argc, char** argv){
// 1. Socket 생성
    int serv_sock;
    int clnt_sock;

// 2. Socket의 IP, Protocol, Port 종류 설정
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    /*
     * socket(IP 종류, Socket 종류, Protocol 종류);
     *  1번째 : IPv4, IPv6
     *  2번째 : SOCK_STREAM(TCP), SOCK_DGRAM(UDP), SOCK_RAW
     *  3번쨰 : IPPROTO_TCP(TCP), IPPROTO_UDP(UDP), IPPROTO_RAW(RAW), IPPROTO_HOPOPTS(0 : Socket 종류에 맞춰 자동으로 설정)
     */
    
    /*
     * af : 네트워크 주소 체계
     *  Protocol 설정할 시, A -> P,
     *  Address 설정할 시, P -> A
     *  하지만, 동일하다
     *  #define AF_INET     2   >> IPv4
     *  #define AF_INET6    23  >> IPv6
     *
     * type : 소켓 타입
     *  #define SOCK_STREAM 1   >> 스트림, TCP 프롤토콜 전송 방식
     *  #define SOCK_DGRAM 2   >> 데이터 그램, UDP 프롤토콜 전송 방식
     *  #define SOCK_RAW    3   >> RAW 소켓, 가공하지 않은 소켓
     *
     * protocol : 프롤토콜
     *  #define IPPROTO_TCP  6   >> TCP 프롤토콜
     *  #define IPPROTO_UDP  17  >> UDP 프롤토콜
     *  #define IPPROTO_RAW  255 >> RAW 
    */
    
// 3. Bind로 실제 Socket에 IP, Protocol, Port 설정
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // htonl : host order to network order, l : long - 256 bit
    // INADDR_ANY : 지금 현재 PC의 IP
    //
    serv_addr.sin_port = htons(7989);
    // htons : host order to network order, s : short - 16 bit
 
    if (bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1){
        printf("Bind Error\n");
    };

// 4. Listen, 사용자를 기다림
    if (listen(serv_sock,5)==-1){
        printf("Listen Error\n");
    }
    // listen(Socket, 대기자들의 수)
    
    char buff[200]; // 받아 올 버퍼 데이터
    int recv_len = 0; // 받아 올 데이티의 길이

    while(TRUE){
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,(socklen_t*)&clnt_addr_size);
        // accept(서버 Socket, 고객의 IP, 고객의 IP 주소 사이즈)

        //g_clnt_socks[g_clnt_count++] = clnt_sock; 
        while(TRUE){
            recv_len = read(clnt_sock, buff, 200);
            // read(클라 Socket, 저장할 장소, 최대치);
            printf("recv: ");
            for (int i =0; i < recv_len; i++){
                printf("%02X",(unsigned char)buff[i]);
            }
            printf("\n");
        }
    }
    return 0;
}
