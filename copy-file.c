/* Aim of this program: 'Copying a file' */
#include<fcntl.h>
char buffer[2048];
int version=1;
main(argc,argv)
int argc;
char *argv[];
{
	int ExistingFileDescriptor, NewFileDescriptor;
	if(argc!=3)
	{
		printf("Requires 2 more arguments to copy a program \n");
		exit(1);
	}
	ExistingFileDescriptor=open(argv[1],O_RDONLY);
	if(ExistingFileDescriptor==-1)
	{
		printf("unable to open a file %s \n", argv[1]);
		exit(1);
	}
	NewFileDescriptor=creat(argv[2], 0666); /*creating target file*/
	if(NewFileDescriptor==-1)
	{
		printf("unable to create file %s \n", argv[2]);
		exit(1);
	}
	copy(ExistingFileDescriptor, NewFileDescriptor);
	exit(0);
}
copy(OldFile, NewFile)
	int OldFile, NewFile;
{
	int count;
	while((count=read(OldFile,buffer,sizeof(buffer))) > 0)
		write(NewFile, buffer, count);
}
