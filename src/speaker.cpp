//
// Created by Stefan Schokker on 31/10/2016.
//

#include "speaker.h"
#include <stdlib.h>

int speaker::playsound(char* file)
{
	int pid;
	pid=fork();
	if(pid==0)
	{
		execlp("/usr/bin/omxplayer --vol 900", " ", file, NULL);
		_exit(0);
	}
	else
	{
		wait();
	}
}