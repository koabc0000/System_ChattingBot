#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256
#define MAX_SEATS 30

void * handle_clnt(void * arg);
void error_handling(char * msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

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

info in;

void send_msg(int client, info * in, int len);

void handle_rcvmsg(int clnt_sock, info inf);

int main(int argc, char *argv[])
{
 in.cnt = 0;
 int serv_sock, clnt_sock;
 struct sockaddr_in serv_adr, clnt_adr;
 int clnt_adr_sz;
 pthread_t t_id;
 if (argc != 2) {
  printf("Usage : %s <port>\n", argv[0]);
  exit(1);
 }

 pthread_mutex_init(&mutex1, NULL);
 pthread_mutex_init(&mutex2, NULL);

 serv_sock = socket(PF_INET, SOCK_STREAM, 0);

 memset(&serv_adr, 0, sizeof(serv_adr));
 serv_adr.sin_family = AF_INET;
 serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
 serv_adr.sin_port = htons(atoi(argv[1]));

 if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
  error_handling("bind() error");
 if (listen(serv_sock, 5) == -1)
  error_handling("listen() error");

 while (1)
 {
  clnt_adr_sz = sizeof(clnt_adr);
  clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

  pthread_mutex_lock(&mutex1);
  clnt_socks[clnt_cnt++] = clnt_sock;
  pthread_mutex_unlock(&mutex1);

  pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
  pthread_detach(t_id);
  printf("Connected client IP: %s , clnt_sock=%d\n", inet_ntoa(clnt_adr.sin_addr), clnt_sock);
 }
 close(serv_sock);
 pthread_mutex_destroy(&mutex1);
 pthread_mutex_destroy(&mutex2);

 return 0;
}

void *handle_clnt(void * arg)
{
 int clnt_sock = *((int*)arg);
 int str_len = 0, i;
  memset(&in, 0, sizeof(info));
 FILE *fp;
 while (read(clnt_sock,&in,sizeof(in)))
 {
  int cmd = in.menu_num;
 if (cmd == 1)
 {
    int cnt = 0;
    if (in.opt == 1)
     fp = fopen("curriculum_g.txt", "r");
    else if (in.opt == 2)
     fp = fopen("curriculum_s.txt", "r");
    while (!feof(fp))
    {
     fgets(in.line[cnt++], 1024, fp);
    }
    in.cnt = cnt;
    fclose(fp);

 }
 else if (cmd == 2)
 {
  int i, cnt = 0;
  fp = fopen("professor.txt", "r");
  while (!feof(fp))
  {
   fgets(in.pro[cnt].name, 50, fp);
   fgets(in.pro[cnt].mail, 50, fp);
   fgets(in.pro[cnt].phone, 50, fp);
   fgets(in.pro[cnt].loc, 50, fp);
   fgets(in.pro[cnt].site, 50, fp);
   fgets(in.pro[cnt++].lab, 100, fp);
  }
  in.cnt = cnt;
  fclose(fp);
 
 }
 else if (cmd == 3)
 {
  fp = fopen("building.txt", "r");
  int cnt = 0;
  while (!feof(fp))
  {
   fgets(in.line[cnt++], 1024, fp);
  }
  in.cnt = cnt;
  fclose(fp);

 }
 else if(cmd == 4)
 {
  if(in.opt==1){
  printf("[client %d] : %s", clnt_sock, in.msg);
  printf("입력하세요 : ");
  fgets(in.msg, 60, stdin);
  printf("\n");
  }
 }
 else if(cmd==6)
   printf("통신 종료\n");
  pthread_mutex_lock(&mutex1);
   write(clnt_sock,&in,sizeof(in));
 pthread_mutex_unlock(&mutex1);
  memset(&in, 0, sizeof(info));
 }


 pthread_mutex_lock(&mutex1);
 for (i = 0; i<clnt_cnt; i++)   // remove disconnected client
 {
  if (clnt_sock == clnt_socks[i])
  {
   printf("clnt_sock=%d, i=%d\n", clnt_sock, i);
   while (i < clnt_cnt) {
    clnt_socks[i] = clnt_socks[i + 1];
    i++;
   }
   break;
  }
 }
 clnt_cnt--;
 pthread_mutex_unlock(&mutex1);
 close(clnt_sock);
 return NULL;
}

void error_handling(char * msg)
{
 fputs(msg, stderr);
 fputc('\n', stderr);
 exit(1);
}