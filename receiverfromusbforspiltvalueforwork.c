#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>

// function to sort the array in ascending order
void Array_sort(int *array , int n)
{
    // declare some local variables
    int i=0 , j=0 , temp=0;
    for(i=0 ; i<n ; i++)
    {
        for(j=0 ; j<n-1 ; j++)
        {
            if(array[j]>array[j+1])
            {
                temp        = array[j];
                array[j]    = array[j+1];
                array[j+1]  = temp;
            }
        }
    }
    //printf("\nThe array after sorting is..\n");
    for(i=0 ; i<n ; i++)
    {
        //printf("\narray_1[%d] : %d",i,array[i]);
    }
}
// function to calculate the median of the array
float Find_median(int array[] , int n)
{
    float median=0;
    // if number of elements are even
    if(n%2 == 0)
        median = (array[(n-1)/2] + array[n/2])/2.0;
    // if number of elements are odd
    else
        median = array[n/2];
    return median;
}

//int open_port(void)
int open_port(int fd)
 {
    //int fd;                                   /* File descriptor for the port */
    //fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);
    //fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1)
    {                                              /* Could not open the port */
        fprintf(stderr, "open_port: Unable to open /dev/ttyUSB0 - %s\n",strerror(errno));
    }

    return (fd);
 }

int main(int argc,char *argv[])
{
    int mainfd=0,fd;                                            /* File descriptor */
    struct termios options;
    char read_buffer[40];   /* Buffer to store the data received              */
    char *delimito;
    char *token;
    int  bytes_read = 0;    /* Number of bytes read by the read() system call */
    int i = 0;


    fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);
    mainfd = open_port(fd);

    fcntl(mainfd, F_SETFL, FNDELAY);                  /* Configure port reading */
                                     /* Get the current options for the port */
    tcgetattr(mainfd, &options);
    cfsetispeed(&options, B2400);                 /* Set the baud rates to 2400 9600 19200 etc.*/
    cfsetospeed(&options, B2400);
                                   /* Enable the receiver and set local mode */
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag |= PARENB;
    options.c_cflag |= PARODD;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS7;
    options.c_cflag &= ~CRTSCTS;               /* Disable hardware flow control */

                                 /* Enable data to be processed as raw input */
    options.c_lflag &= ~(ICANON | ECHO | ISIG);
                                        /* Set the new options for the port */
    tcsetattr(mainfd, TCSANOW, &options);
    usleep(1000000);      /* wait for 5 second*/
    bytes_read = read(mainfd,&read_buffer,40); /* Read the data                   */
    //printf("\n\n Bytes Rxed -%d", bytes_read); /* Print the number of bytes read */
    //printf("\n\n  ");
    for(i=0;i<bytes_read;i++)
    {	 /*printing only the received characters*/
        //printf("%c",read_buffer[i]);
    }
    delimito = "+P";
    token = strtok(read_buffer,delimito);
    int cnt = 0;
    char *f[4];
    while(token != NULL){
        //printf("\nToken(%d): %s\n",cnt,token);
        //cnt++;
        f[cnt] = token;
        cnt++;
        token = strtok(NULL,delimito);
    }
    //printf("\nread is \n%s \n%s \n%s \n%s \n%s \n",f[0],f[1],f[2],f[3],f[4]);
    int d[4];
    int cnt2 =0;
    char *test;
    for(int a = 0;a < 5; ++a){
        //int cnt2;
        test = f[a];
        char *s = test;
        int c = atoi(s);
        d[cnt2] = c;
        cnt2++;
    }
    //printf("\nread is \n%d \n%d \n%d \n%d \n%d \n%d",d[0],d[1],d[2],d[3],d[4],cnt2);
                                        // Sort the array in ascending order
    Array_sort(d,cnt2);
                                        // Now pass the sorted array to calculate
                                        // the median of your array.
    int median = Find_median(d,cnt2);
    //int z = 5.5;
    char w[6] = "";
    sprintf(w,"%d",median);
    //printf("weight = %s kg\n",w);
    printf("%s\n",w);
    //printf("\n\nThe median is : %d\n ",median);
                                                    /* Close the serial port */
    close(mainfd);
    return 0;
 }



