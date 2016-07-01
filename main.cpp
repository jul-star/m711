#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //sleep
#include <string.h>
#include <pthread.h>

void *pthrd(void *id)
{
	int  *tid;
	tid = (int*)id;
	printf("Thread:PID=%d, TID=%d\n",getpid(),tid);

	pthread_testcancel();

	int i=1;
	while(1){
		printf("Wait to cancel:%d\n",i);
		++i;
		sleep(1);
	}
	pthread_testcancel();

	pthread_exit(NULL);
	return NULL;
}

int main(int c, char**v)
{
	int pid = getpid();
	FILE *fs;
	char name[L_cuserid];
	cuserid(name);
	if (strcmp(name,"jstar")==0)
		fs= fopen("/home/jstar/Stepic/Multi/m711/main.pid","w");
	else 
		fs= fopen("/home/box/main.pid","w");

	if (fs == NULL) perror("fopen");
	if(fprintf(fs,"%d",pid)<0) perror("write_pid");
	if(fclose(fs)<0) perror("fclose");


	int rt;
	pthread_t tid;
	rt = pthread_create(&tid, NULL, pthrd,(void*)1);
	if (rt != 0) perror("pthread");
	printf("Base:Pid=%d, Tid=%d\n",getpid(), tid);
//	if (pthread_join(tid,NULL) != 0) perror("join");
	sleep(5);
	rt = pthread_cancel(tid);
	if (rt != 0) perror("phread_cancel");
	pthread_exit(NULL);	
	return 0;
}
