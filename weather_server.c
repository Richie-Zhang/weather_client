#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define MAXRECV 33
#define MAXSEND 137 
#define SERV_PORT 5050
#define LISTENQ 8
#define NUMOFCITYS 34

char citys[NUMOFCITYS][20];

void init_citys(char citys[][20]){
	strcpy(citys[0], "beijing");
	strcpy(citys[2], "tianjin");
	strcpy(citys[3], "shanghai");
	strcpy(citys[4], "chongqin");
	strcpy(citys[5], "shijiazhuang");
	strcpy(citys[6], "taiyuan");
	strcpy(citys[7], "jinan");
	strcpy(citys[8], "zhengzhou");
	strcpy(citys[9], "shenyang");
	strcpy(citys[10], "chuangchun");
	strcpy(citys[11], "haerbin");
	strcpy(citys[12], "nanjing");
	strcpy(citys[13], "hangzhou");
	strcpy(citys[14], "hefei");
	strcpy(citys[15], "nanchang");
	strcpy(citys[16], "fuzhou");
	strcpy(citys[17], "wuhan");
	strcpy(citys[18], "changsha");
	strcpy(citys[19], "chengdu");
	strcpy(citys[20], "guiyang");
	strcpy(citys[21], "kunming");
	strcpy(citys[22], "guangzhou");
	strcpy(citys[23], "haikou");
	strcpy(citys[24], "lanzhou");
	strcpy(citys[25], "xining");
	strcpy(citys[26], "taibei");
	strcpy(citys[27], "huhehaote");
	strcpy(citys[28], "wunumuqi");
	strcpy(citys[29], "lasa");
	strcpy(citys[30], "nanning");
	strcpy(citys[31], "yinchuan");
	strcpy(citys[32], "xianggang");
	strcpy(citys[33], "aomeng");
}

int avail_city(char *city){
	int i;
	for(i = 0 ; i < NUMOFCITYS ; i++){
		if( strcmp(citys[i],city) == 0)
			return 1;
	}
	return 0;
}


void handle(char *recvline, char *sendline){
	memset(sendline, 0 , MAXSEND);
	char city[20];

	int year,month,day;
	struct tm * ptm;
	long ts; 
	ts = time(NULL);
	ptm = localtime(&ts);
	year = ptm->tm_year +1900;
	month = ptm->tm_mon+1;
	day = ptm->tm_mday;

	sscanf(&recvline[2],"%s",city);
	strcpy(&sendline[2], city);
	if(recvline[0] == 0x00 && recvline[1] == 0x00){
		if(avail_city(city) == 0){
			sendline[0] = 'B';
		}
		else{
			sendline[0] = 'A';
		}
	}
	else if(recvline[0] == 0x01 && recvline[1] == 'A'){
		sendline[32] = year >> 8;
		sendline[33] = year;
		sendline[34] = month;
		sendline[35] = day;
		sendline[1] = 'A';
		sendline[36] = recvline[32];
		sendline[37] = rand()%5;
		sendline[38] = rand()%12;
		sendline[39] = rand()%30;
		if(recvline[32] >= 0x08){
			sendline[0] = 'D';

		} 
		else{
			sendline[0] = 'C';
		}
	}
	else if(recvline[0] == 0x01 && recvline[1] == 'B'){
		sendline[32] = year >> 8;
		sendline[33] = year;
		sendline[34] = month;
		sendline[35] = day;
		sendline[0] = 'C';
		sendline[1] = 'B';
		sendline[36] = recvline[32];
		sendline[37] = rand()%5;
		sendline[38] = rand()%12;
		sendline[39] = rand()%30;
		sendline[40] = rand()%5;
		sendline[41] = rand()%12;
		sendline[42] = rand()%30;
		sendline[43] = rand()%5;
		sendline[44] = rand()%12;
		sendline[45] = rand()%30;
	}
}


int main(int argc, char **argv){
	
	int listenfd, connfd, n;
	pid_t childpid;
	socklen_t clilen;
	char sendline[MAXSEND],recvline[MAXRECV];
	struct sockaddr_in cliaddr, servaddr;
	
	init_citys(citys);	

	if((listenfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("Problem in creating the socket\n");
		exit(1);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind (listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	printf("Server running ... Waiting for connections.\n");
	
	while(1){
		clilen = sizeof(cliaddr);
		
		connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
	
		printf("Received request ...\n");

		if( (childpid = fork()) == 0 ){
			printf("Child created for dealing with client requests\n");
			close(listenfd);
			
			while( (n = recv(connfd, recvline, MAXRECV, 0) > 0)){
				handle(recvline,sendline);
				send(connfd, sendline, MAXSEND, 0);
			}
			
			if(n < 0)	printf("Read error\n");
			exit(0);
		}
		close(connfd);
	}
}
