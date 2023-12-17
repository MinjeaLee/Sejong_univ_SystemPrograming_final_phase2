/* server.h
 * Author : Lee Minjae
 * ID     : 21011741
 * dept   : Information Security
 * date   : 2023.12.17
 * Contact: leejoy2@sju.ac.kr
 * repo   : https://github.com/MinjeaLee/Sejong_univ_SystemPrograming_final_phase2
*/

#ifndef SERVER_H
# define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/ipc.h>
#include <sys/msg.h>

# define QUEUE_KEY 12345

typedef struct 
{
	int sock; // 클라이언트 소켓
	int num1;
	int num2;
} operation_data;	

typedef struct t_massage{		// 메시지 큐 메시지 구조체
	long msg_type;				// 메시지 타입
	int sock;					// 클라이언트 소켓 파일 디스크립터
	char operation[10];			// 연산 종류
	int num1;					// 두 숫자
	int num2;
}	message;



void *handle_client(void *data);

#endif