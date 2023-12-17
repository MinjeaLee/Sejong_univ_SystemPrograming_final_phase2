/* server.c
 * Author : Lee Minjae
 * ID     : 21011741
 * dept   : Information Security
 * date   : 2023.12.06
 * Contact: leejoy2@sju.ac.kr
 * repo   : https://github.com/MinjeaLee/Sejong_univ_SystemPrograming_final_phase1
 */

#include "server.h"

void *addition(void *data)
{
	operation_data *op_data = (operation_data *)data; // 연산 데이터 구조체
	int result = op_data->num1 + op_data->num2;	// 두 숫자를 더함
	char buffer[256];							// 송수신 버퍼

	sprintf(buffer, "Addition result: %d", result);	// 버퍼에 덧셈 결과를 저장
	write(op_data->sock, buffer, strlen(buffer));	// 클라이언트에 덧셈 결과 전송
	printf("Addition result: %d\n", result);		// 덧셈 결과 출력
	free(data); // 구조체 메모리 해제
	return NULL;
}

void *multiplication(void *data)
{
	operation_data *op_data = (operation_data *)data;		// 연산 데이터 구조체
	int result = op_data->num1 * op_data->num2;			// 두 숫자를 곱함
	char buffer[256];

	sprintf(buffer, "Multiplication result: %d", result);	// 버퍼에 곱셈 결과를 저장
	write(op_data->sock, buffer, strlen(buffer));			// 클라이언트에 곱셈 결과 전송
	printf("Multiplication result: %d\n", result);		// 곱셈 결과 출력
	free(data); // 구조체 메모리 해제
	return NULL;
}

void *handle_client(void *data)
{
	message *msg = (message *)data;
	int sock = msg->sock; 			// 클라이언트 소켓 파일 디스크립터
	char buffer[256];					// 데이터 송수신에 사용되는 버퍼
	int n;								// 송수신된 바이트 수

	printf("Received: %d %s %d\n", msg->num1, msg->operation, msg->num2);	// 수신된 데이터 출력

	pthread_t thread_id;				// 스레드 아이디
	operation_data *op_data = malloc(sizeof(operation_data));	// 연산 데이터 구조체 동적 할당
	op_data->sock = msg->sock;						// 연산 데이터 구조체에 클라이언트 소켓 파일 디스크립터 저장

	if (strcmp(msg->operation, "add") == 0)			// 덧셈 연산일 경우
	{
		op_data->num1 = msg->num1;		// 문자열을 정수형으로 변환하여 연산 데이터 구조체에 저장
		op_data->num2 = msg->num2;

		pthread_create(&thread_id, NULL, addition, op_data);		// 덧셈 연산 스레드 생성
	}
	else if (strcmp(msg->operation, "mult") == 0)	// 덧셈 메커니즘과 동일
	{
		op_data->num1 = msg->num1;
		op_data->num2 = msg->num2;

		pthread_create(&thread_id, NULL, multiplication, op_data);
	}
	else
	{
		sprintf(buffer, "Invalid operation");	// 연산이 잘못되었을 경우 에러 메시지를 버퍼에 저장
		write(sock, buffer, strlen(buffer));	// 클라이언트에 에러 메시지 전송
		free(op_data);
		return NULL;
	}
	pthread_detach(thread_id);				// 스레드 종료시 자원 해제
	return NULL;
}

int main()
{
	int sockfd, newsockfd, portno = 2222; 	// 소켓 파일 디스크립터 및 포트 번호 설정
	socklen_t clilen;						//  클라이언트 주소 정보 구조체
	struct sockaddr_in serv_addr, cli_addr;	// 서버 및 클라이언트 주소 정보 구조체

	int msgig;
	message msg;

	msgig = msgget((key_t)QUEUE_KEY, IPC_CREAT | 0666);
	if (msgig == -1)
	{
		perror("msgget failed with error");
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);	// TCP/IP 소켓 생성
	if (sockfd < 0)
	{
		perror("ERROR opening socket");			// 소켓 생성 실패시 에러 메시지 출력
		exit(1);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));	// 서버 주소 구조체 초기화
	serv_addr.sin_family = AF_INET;					// 주소 체계를 IPv4로 설정
	serv_addr.sin_addr.s_addr = INADDR_ANY;			// 서버의 IP 주소를 자동으로 찾아서 설정
	serv_addr.sin_port = htons(portno);				// 포트 번호를 네트워크 바이트 순서로 변환하여 설정

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)	// 소켓에 서버 주소 정보를 할당
	{
		perror("ERROR on binding");								// 할당 실패시 에러 메시지 출력
		exit(1);
	}

	listen(sockfd, 5);							// 소켓을 수동 대기모드로 설정, 최대 5개의 클라이언트가 대기 가능
	clilen = sizeof(cli_addr);					// 클라이언트 주소 정보 구조체의 크기 설정

	while (1)
	{
		message msg;
		if (msgrcv(msgig, &msg, sizeof(message) - sizeof(long), 0, 0) == -1)
		{
			perror("msgrcv failed with error");
			continue;
		}

		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);	// 클라이언트의 연결 요청 수락
		if (newsockfd < 0)								// 연결 요청 수락 실패시 에러 메시지 출력
		{
			perror("ERROR on accept");
			continue;
		}

		msg.sock = newsockfd;

		pthread_t thread_id;							// 스레드 아이디
		message *new_msg = malloc(sizeof(message));	// 메시지 큐 메시지 구조체 동적 할당
		*new_msg = msg;
		pthread_create(&thread_id, NULL, handle_client, new_msg);	// 클라이언트 연결 핸들러 스레드 생성
		pthread_detach(thread_id);					// 스레드 종료시 자원 해제
		// free(new_msg);
	}

	close(sockfd);				// 소켓 연결 종료
	return 0;
}