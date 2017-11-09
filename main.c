#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>

void list(const char*);

int main(int argc, char **argv)
{
	if(argc!=2)
	{
		printf("Usage :./a.out path\n");
		return;
	}
	if(!opendir(argv[1]))
	{
		perror("opendir");
		return;
	}
	char ch;
	do
	{
		list(argv[1]);
		printf("do you want to continue?(y/n) : ");
		scanf(" %c",&ch);
	}while(ch=='y' ||ch=='Y'); 
	return 0;
}

void list(const char *path)
{

	system("clear");
	DIR *dp;
	printf("\t\t%s\n\n",path);
	dp=opendir(path);
	struct dirent *v;
	char **con,*new_path;
	int i=0;

	new_path=malloc(500);
	con=malloc(sizeof(char*)*100);
	for(i=0;i<100;i++)
		con[i]=malloc(150);
	i=0;
	while(v=readdir(dp))
	{
		if(v->d_name[0]=='.')
			continue;
		strcpy(con[i++],v->d_name); 
		printf("%d. %s\n",i,v->d_name);
	}
	printf("Select the File/Directory number : ");
	scanf("%d",&i);

	strcpy(new_path,path);
	strcat(new_path,"/");
	strcat(new_path,con[i-1]);
	struct stat p;
	stat(new_path,&p);
	if(S_ISFIFO(p.st_mode))
		printf("cannot open pipe file\n");
	else if(S_ISDIR(p.st_mode))
		list(new_path);
	else
	{
		if(fork()==0)
		{
			execlp("cat","cat",new_path,NULL);
			exit(0);
		}
		wait(0);
	}
	closedir(dp);
	free(new_path);
	free(con);
	free(v);
}




