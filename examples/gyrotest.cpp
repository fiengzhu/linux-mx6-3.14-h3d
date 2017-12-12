#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdarg.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char **argv)
{
    int keys_fd; //Button handler
    int ret[4];
    struct input_event t;
    int i;
    bool bX = false; 
    bool bY = false; 
    bool bZ = false;

    if (argc < 2) { 
	keys_fd = open("/dev/input/event0",O_RDONLY);}
    else {
	keys_fd = open(argv[1], O_RDONLY);}
    if(keys_fd<=0)
    {
        printf("open /dev/input/event0 device error!\n");
        return 0;
    }

    memset(ret, 0, 4*sizeof(int));
    for (i=0; i<3*10; i++)
    {
	if (read(keys_fd, &t, sizeof(t)) == sizeof(t))
        {
            //printf("event code %d, expecting %d\n", t.type, EV_ABS);
	    if(t.type==EV_ABS) 
            {
		switch(t.code)
                {
                    case ABS_X:
                        ret[0] = t.value;
			bX = true;
                        //printf("X: key %d %d\n", t.code, t.value);
                        break;
                    case ABS_Y:
                        ret[1] = t.value;
			bY = true;
                        //printf("Y: key %d %d\n", t.code, t.value);
			break;
                    case ABS_Z:
                        ret[2] = t.value;
			bZ = true;
			//printf("Z: key %d %d\n", t.code, t.value);
                        break;
		    case ABS_MISC:
			ret[3] = t.value;
			break;	
		    default:
			printf("other: key %d %d\n", t.code, t.value);	
			//ret[3] = t.value;
			break;
                }
            }
        }

	if (bX && bY && bZ)
	{
	    printf("mag output is %d, %d, %d (%x) \n", ret[0], ret[1], ret[2],ret[3]);
	    bX = false; bY = false; bZ = false;
	}
    }

    close(keys_fd);
    return 0;
}
