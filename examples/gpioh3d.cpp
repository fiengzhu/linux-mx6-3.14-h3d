// Borrowed from Radek "Mrkva" Pilar @ http://www.raspberrypi.org/phpBB3/viewtopic.php?f=44&t=7509
// Modified to only print 'interrupt' as output, increase timeout to 10 minutes and not buffer the output.
// Compile with gcc gpio-irq-demo.c -o gpio-irq
#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define GPIO_FN_MAXLEN	32
#define POLL_TIMEOUT	600000
#define RDBUF_LEN	5

int main(int argc, char **argv) {
	char fn[GPIO_FN_MAXLEN];
	int fdc,fd,ret;
	struct pollfd pfd[4];
	char rdbuf[RDBUF_LEN];
	char buf[100];

	memset(rdbuf, 0x00, RDBUF_LEN);
	memset(fn, 0x00, GPIO_FN_MAXLEN);
	
	// export gpio 248 to 251
	sprintf(fn, "/sys/class/gpio/export");
	fdc = open(fn, O_WRONLY);
	for (int ig=248; ig<252; ig++)
	{
		sprintf(buf, "%d", ig);
		write(fdc, buf, 3);
	}
	close(fdc);

	// configure gpios
	for (int ig=248; ig<252; ig++)
	{
		sprintf(fn, "/sys/class/gpio/gpio%d/edge", ig);
		fdc= open(fn, O_WRONLY);
		sprintf(buf, "both");
		write(fdc, buf, 6);
		close(fdc);
	}

	// setup interrupts
	for (int i=0; i<4; i++)
	{
		snprintf(fn, GPIO_FN_MAXLEN-1, "/sys/class/gpio/gpio%d/value", 248+i);
		fd=open(fn, O_RDONLY);
		if(fd<0) { perror(fn); return 2; }
		pfd[i].fd=fd;
		pfd[i].events=POLLPRI;
		pfd[i].revents = 0;
	}
	
/*
	ret=read(fd, rdbuf, RDBUF_LEN-1);
	if(ret<0) {
		perror("read()");
		return 4;
	}
	printf("value is: %s\n", rdbuf);
*/
	
	while(1) {
		memset(rdbuf, 0x00, RDBUF_LEN);
		
		for (int i=0; i<4; i++)
                {
			lseek(pfd[i].fd, 0, SEEK_SET);
		}
		ret=poll(pfd, 4, POLL_TIMEOUT);
		if(ret<0) {
			perror("poll()");
			close(fd);
			return 3;
		}
		if(ret==0) {
			printf("timeout\n");
			continue;
		}

		printf("interrupt detected!\n");

		for (int i=0; i<4; i++)
		{
			if (pfd[i].revents & POLLPRI)
			{
				ret=read(pfd[i].fd, rdbuf, RDBUF_LEN-1);
				if(ret<0) {
					perror("read()");
					return 4;
				}		
				printf("interrupt %s\n", rdbuf);
			
			}
			pfd[i].revents = 0;
		}	
	
		//fflush(stdout);
	}

	for (int i=0; i<4; i++)
	{
		close(pfd[i].fd);
	}
	return 0;
}
