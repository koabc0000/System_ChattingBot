#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
#define MAX_SEATS 30

#define REQUEST_SEATS   1
#define RESERVATION  2
#define CANCELLATION 3
void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);

typedef struct profe {
	char name[50];
	char mail[50];
	char phone[50];
	char loc[50];
	char site[50];
	char lab[100];
}profe;

typedef struct info {
	int menu_num;
	int opt;
	char msg[60];
	profe pro[60];
	int cnt;
	char line[500][1024];
}info;


void pro();

info in;

int main(int argc, char *argv[])
{
 in.cnt = 0;
 int sock;
 int menu = 0;
 struct sockaddr_in serv_addr;
 pthread_t snd_thread, rcv_thread;
 void * thread_return;
 if (argc != 3) {
  printf("Usage : %s <IP> <port> \n", argv[0]);
  exit(1);
 }

 sock = socket(PF_INET, SOCK_STREAM, 0);

 memset(&serv_addr, 0, sizeof(serv_addr));
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
 serv_addr.sin_port = htons(atoi(argv[2]));

 if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
  error_handling("connect() error");

 pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
 pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
 pthread_join(snd_thread, &thread_return);
 pthread_join(rcv_thread, &thread_return);
 close(sock);

 return 0;
}
void display_menu()
{
 sleep(1);
 printf("1. 커리큘럼  2. 교수진  3. 학과사무실 정보  4. 상담원 연결  5: 종료\n");
 scanf("%d", &in.menu_num);
 in.opt = 0;
}

void *send_msg(void * arg)   // send thread main
{
  char mes[60];
 int sock = *((int*)arg);
    memset(&in, 0, sizeof(info));
     display_menu();
 while (1)
 {
  if (in.menu_num == 1)
  {
   printf("1. 글로벌sw융합전공  2. 심화컴퓨터공학전공\n");
   scanf("%d", &in.opt);
  }
  
  if( in.menu_num == 4)
  {
    if(in.opt==1)   sleep(5);

    printf("입력 : ");
    
    if(in.opt==0) getchar();
    
    fgets(mes, sizeof(mes), stdin);
    strcpy(in.msg, mes); 
    if((strcmp(mes, "q\n")==0)||(strcmp(mes,"Q\n")==0))
    {
 	in.opt=0;
        in.menu_num=6;
    }
    else in.opt=1;
  }
  
  if(in.menu_num == 5)
	exit(1);

  write(sock, &in, sizeof(in));

 
  if(in.menu_num!=4)  memset(&in, 0, sizeof(info));

  if(in.menu_num==0)  display_menu();

}
 return NULL;
}

void * recv_msg(void * arg)   // read thread main
{
 int sock = *((int*)arg);
 info new;
 int str_len = 0, i;
 while (1)
 {
  memset(&new, 0, sizeof(info));
  str_len = read(sock, &new, sizeof(new));
  if (str_len == -1)
   return (void*)-1;
  else
  {
   if (new.menu_num == 1)
   {
    for (i = 0; i < new.cnt; i++){
     printf("%s\n", new.line[i]);
    }

   }
   else if (new.menu_num == 2)
   {
    pro(&new);

   }
   else if (new.menu_num == 3)
   {
    for (i = 0; i < new.cnt; i++)
     printf("%s\n", new.line[i]);
   }
   else if(new.menu_num==4)
   {
    if(new.opt==1)
        printf("[server] : %s\n", new.msg);
   }

  }
 }

 return NULL;
}

void pro(info *new)
{
 int i;
 char name[50];
  for (i = 0; i < new->cnt - 1; i++)
  {
   printf("성함       : %s", new->pro[i].name);
   printf("\n");
   printf("메일       : %s", new->pro[i].mail);
   printf("\n");
   printf("전화번호    : %s", new->pro[i].phone);
   printf("\n");
   printf("연구실 위치 : %s", new->pro[i].loc);
   printf("\n");
   printf("사이트 주소 : %s", new->pro[i].site);
   printf("\n");
   printf("연구      : %s\n", new->pro[i].lab);
  }
 

}

void error_handling(char *msg)
{
 fputs(msg, stderr);
 fputc('\n', stderr);
 exit(1);
} 