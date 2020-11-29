//#include <iostream>  <<C++寫法, gcc無法編譯
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <netinet/in.h>
//using namespace std; <<C++寫法, gcc無法編譯
int reconnection_count;
int original_reconnection_count=0;
struct sockaddr_in dest;
int sockfd;
char flag[256] = "1";
char flagT[256] = "2";
pid_t pid;
int Flag=0;
int ret;
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

void mode1_child(void* programpath){

	system(programpath);
	
}


void mode2_child(void *programpath, void *checkpointTime){
	//char *str = (char*)data; //get data
	//execute detect temperature program
	//system("python ./simpletest.py");
	char buffer[256];
	int ret=0;
	if( reconnection_count == 0){
		printf("reconnection_count!reconnection_count!reconnection_count!\n");
		printf("\n");
		printf("\n");
		if(original_reconnection_count>0)
		{
			system("~/build-Redundant_system_UI_v2-Desktop-Release/mv_ckpt_from_gluster.sh");
		
			// execute external program
			system("~/build-Redundant_system_UI_v2-Desktop-Release/auto_remove_file.sh");
			system("/dmtcp/dmtcp_restart_script_12085f9b-40000*.sh --restartdir /dmtcp/ --no-strict-checking -h localhost");
		}
		else 
		{
		char str1[256] = "dmtcp_launch -i ";
		strcat(str1,checkpointTime);
		char str2[256] = " --with-plugin ~/build-Redundant_system_UI_v2-Desktop-Release/libdmtcp_mv_ckpt_to_gluster.so --ckptdir /dmtcp " ;
		strcat(str1,str2);
		strcat(str1,programpath);
		//printf("%s\n",str1);
		system(str1);
		//system("dmtcp_launch -i 5 --with-plugin ./libdmtcp_mv_ckpt_to_gluster.so --ckptdir /dmtcp ./test");
		}


	}
	else{
		
			//system("~/build-Redundant_system_UI_v2-Desktop-Release/mv_ckpt_from_gluster.sh");
		
			// execute external program
			//system("/dmtcp/dmtcp_restart_script.sh --restartdir /dmtcp/ --no-strict-checking -h localhost");
	
	}
	
}

void reconnection_server(void *ip){
	
	//char ip[32] = "10.0.0.4";
	int connectnum;
	char reconnection[256];
	
	//初始
	bzero(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(8888);
	dest.sin_addr.s_addr = inet_addr(ip); //ip
	printf("---initial!---\n");
	
	//建立
	sockfd = socket(PF_INET, SOCK_STREAM,0);
	
	/*if(sockfd == INVALID_SOCKET)
    {
        printf("invalid socket !");
        //system("pause");
        return;
    }
	*/
	//連線
	 while(connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) == -1)
    {
        //printf("connection failed，reconnection after 3 seconds。\n");
        sleep(3);
    }

	
	//connectnum=connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));
	
	printf("\n---connection success---\n");
	//send(sockfd, flag, sizeof(flag), 0);
	
	//receive msg from monitor
	recv(sockfd, reconnection, sizeof(reconnection), 0);
	reconnection_count = atoi(reconnection);
	printf("reconnection: %d\n",reconnection_count);
	//sleep(1);
	
	

}


int main(int argc, char *argv[])
{
	//char ip[32] = "10.0.0.4";
	//struct sockaddr_in dest;
	char buffer[256];
	char res[256] = "client";
	//char flag[256] = "1";
	int connectnum;
	

	char *type=argv[1];
	char *ip=argv[2];
	char *programpath=argv[3];
	char *checkpointTime=argv[4];
	
	printf("I'm working, flag:%s\n",flag);
	
	
	reconnection_server(ip);

	signal(SIGCHLD,signal_handler);

	printf("%s\n",type);

	if(*type == '1'){
	
		//create child process
		if((pid = fork()) == 0 ){
		close(sockfd);
		mode1_child(programpath);
		exit(EXIT_SUCCESS);
		
	}
	
	}
	else if (*type == '2') {
		printf("\n");
		printf("\n");
		//create child process
		if((pid = fork()) == 0 ){
			close(sockfd);
			mode2_child(programpath,checkpointTime);
			exit(EXIT_SUCCESS);
		
		}
	}

	while(1){


		
	ret = recv(sockfd, buffer, sizeof(buffer), 0);
	
	if(ret > 0)
        {
		
		//printf("primary system ret alive %d\n", reconnection_count);
			if(reconnection_count>0)
			{
				original_reconnection_count=reconnection_count;
				send(sockfd, "2", sizeof(flag), 0);
				//sleep(2);
				//接收回傳訊息
				bzero(buffer, 256); //清除
				Flag = atoi(buffer); //字串轉數字
				printf("redundant system   alive\n");
			}
			else 
			{
				//printf("I'm working, flag:%s\n",buffer);
				send(sockfd, flag, sizeof(flag), 0);
			}

			
			

        }
        else if(ret == 0)
        {
			
            //當ret == 0 代表server lost connection
            printf("\n------server lost connection------ \n");
            close(sockfd);
            if(reconnection_count ==0)
            {
				
			}
			else if(reconnection_count >0)
			{
				reconnection_count =0;
				if (*type == '2') {
				//create child process
					if((pid = fork()) == 0 ){
						close(sockfd);
						mode2_child(programpath,checkpointTime);
						exit(EXIT_SUCCESS);
					}
				}
				
			}
			

            reconnection_server(ip);//重连
	    
            //break;這裡不要跳出loop,否則process會停止
        }
        else//ret < 0
        {
            //當ret < 0 代表出現異常,例如阻塞狀態解除,或者讀取數據時出現pointer錯誤等.
            //所以這裡要主動斷開連接
            printf("\n something wrong of %s\n", inet_ntoa(dest.sin_addr));
            close(sockfd);
            if(reconnection_count ==0)
            {
				
			}
			else if(reconnection_count >0)
			{
				reconnection_count =0;
				if (*type == '2') {
				//create child process
					if((pid = fork()) == 0 ){
						close(sockfd);
						mode2_child(programpath,checkpointTime);
						exit(EXIT_SUCCESS);
					}
				}
				
			}
            reconnection_server(ip);//重連
            //break;這裡不要跳出loop,否則process會停止 
        }
    }
	

	close(sockfd);
	return 0;
	
}
