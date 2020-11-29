#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
char flag[256] = "1";
int chdflag=0;
int pipe_fd[2];
pid_t pid;
#define PAUSE printf("Press Enter key to continue..."); fgetc(stdin); 

int reconnection_count = 0;
char reconnection [256];

void signal_handler(int signo) {
    if (signo == SIGCHLD) {
        
        while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
			
            //printf("SIGCHLD pid %d\n", pid);
			//chdflag = 0;
			//pid = fork();
			break;
        }
    }
}

void mode1_child( void *ptr, int *chdpid, void *programpath)//, int *chdflag) 
{
   	int fd = *(int *)ptr;
	int cd = *(int *)chdpid;
    printf("this is a new thread,you got connected\n");
   	printf("fd=%d\n",fd);

	char flag[256]="1";
	int Flag;
	int byte_count;
	char buffer[256];
	
	

	
	while(1){
		
	
	//send message
	send(fd, flag, sizeof(flag), 0 );
	
	sleep(1);
	//接收回傳訊息
	bzero(buffer, 256); //清除
	byte_count=recv(fd, buffer, sizeof(buffer), 0 ); //接收

	Flag = atoi(buffer); //字串轉數字

	//identify primary system keep alive or not.
	if ( Flag ==1 ){
		printf("primary system keep alive %s\n", buffer);
	
		}
	else if(byte_count == 0 || byte_count < 0){
	
		perror("primary system is die!");
		//send chdflag to parent , chdflag is used to identify old child program.
		close(pipe_fd[0]); 
		chdflag = 1;
		write(pipe_fd[1], &chdflag, sizeof(int));

		// execute external program
		int ret;       
		//ret =execl("./dht11_example.py", "hello", NULL);
		ret = execl(programpath,"hello",NULL);
    		if(ret == -1)
        	perror("execl error");
		
		
		//system("./dht11_example.py");


		
	}
	
		sleep(3);
	}
	
    close(fd);
}

void *mode2_child( void *ptr, int *chdpid)//, int *chdflag) 
{
   	int fd = *(int *)ptr;
	int cd = *(int *)chdpid;
    printf("this is a new thread,you got connected\n");
   	printf("fd=%d\n",fd);

	char flag[256]="1";
	int Flag;
	int byte_count;
	char buffer[256];
	
	

	
	while(1){
		
	
	//發送訊息
	send(fd, flag, sizeof(flag), 0 );
	
	sleep(1);
	//接收回傳訊息
	bzero(buffer, 256); //清除
	byte_count=recv(fd, buffer, sizeof(buffer), 0 ); //接收
	Flag = atoi(buffer); //字串轉數字
	
	//identify primary system keep alive or not.
	if ( Flag ==1 ){
		printf("primary system keep alive %s\n", buffer);
	
		}
	else if ( Flag ==2 ){
		printf("primary system keep transport %d\n", Flag);
	
		}
	else if(byte_count == 0 || byte_count < 0){
	
		perror("primary system is die!");
		//send chdflag to parent , chdflag is used to identify old child program.
		close(pipe_fd[0]); 
		chdflag = 1;
		write(pipe_fd[1], &chdflag, sizeof(int));
		
		printf("primary system keep alive %s\n", buffer);
		
		system("~/build-Redundant_system_UI_v2-Desktop-Release/mv_ckpt_from_gluster.sh");
		// execute external program
		system("~/build-Redundant_system_UI_v2-Desktop-Release/auto_remove_file.sh");
		system("/dmtcp/dmtcp_restart_script_12085f9b-40000*.sh --restartdir /dmtcp/ --no-strict-checking -h localhost");
		
		
	}
	
		sleep(3);
	}
	
    close(fd);
}

int main(int argc, char *argv[ ]){

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	char *type = argv[1];
	char *programpath = argv[2];
 
 
	//建立
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	//printf("[+]Server Socket is created.\n");
	printf("---initial!---\n");
	
	
	//memset(&serverAddr, '\0', sizeof(serverAddr));
	//初始化
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8888);
	//serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	
	//綁定
	// 設置socket選項避免address使用錯誤
	int on=1;
	if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)
	{
		perror("setsockopt failed!\n");
		exit(2);
	}

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		perror("error: ");
		exit(1);
	}
	
	/*ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	*/
	listen(sockfd, 20);
	printf("wait..\n");
	
	
	//creat pipe is used to communicat with child process.
	if (pipe(pipe_fd) == -1){
		fprintf(stderr, "Error: Unable to create pipe.\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid;
	int stat;
	
	//用來處理當child process 比parent process 早結束, 而產生的zombie process
	signal(SIGCHLD, signal_handler); 
	
	while(1){
		
		int chdpid;
		
		//等待連線
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);

		//sleep(1);
		
		if(newSocket < 0){
			exit(1);
		}
		//printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		printf("---connection success---\n");
		
	
	
		//transfer msg to slave
		sprintf (reconnection, "%d", reconnection_count); // 字串格式化 (數字轉字串)
		printf("reconnection:%s\n",reconnection);
		send(newSocket, reconnection, sizeof(reconnection), 0);
		//reconnection_count++;
		//sleep(1);
	
		//detect old chlid program
		if ( reconnection_count != 0 ){
			
			//chdflag = 0;
	
			printf("No kill Child %d\n", pid);
			/*
			char k[50]="kill ";
			char tmp[50];
			sprintf(tmp,"%d",pid);
			strcat(k, tmp);
			printf("\t\t\t\tCMD:%s\n",k);
			system(k);
			
			printf("kill dmtcp \n");
			system("pkill -9 dmtcp \n");
			*/
			printf("I'm working, flag:%s\n",flag);
			//send(sockfd, flag, sizeof(flag), 0);
		}
		
		reconnection_count++;
		
		if ( *type == '1'){
				//create child process
				if((pid = fork()) == 0){

							close(sockfd);
					while(1){
				
							chdpid=getpid();
							printf("child process chdpid: %d\n",chdpid);			
							mode1_child((void *)&newSocket,&chdpid, programpath);//, &chdflag);
							waitpid(-getpid(), &stat, 0);
							exit(EXIT_SUCCESS);
					}
				}
			
		
		}
		else if ( *type =='2'){
		//create child process
		if((pid = fork()) == 0){

			close(sockfd);
			while(1){
				
					chdpid=getpid();
					printf("child process chdpid: %d\n",chdpid);			
					mode2_child((void *)&newSocket,&chdpid);//, &chdflag);
					waitpid(-getpid(), &stat, 0);
					exit(EXIT_SUCCESS);
				}
			}
		}
			
			printf("---Child Process:%4d\n", pid);
			
			close(pipe_fd[1]); 
			sleep(1);  //確保關閉寫端
			read(pipe_fd[0],  &chdflag, sizeof(int));
			//printf("parent process chdflag: %d\n",chdflag);
			close(pipe_fd[0]); //close read end
			
		}
		

	close(newSocket);
	
	return 0;
}
