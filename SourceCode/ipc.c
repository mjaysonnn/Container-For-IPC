#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>

//#define simple macro

#define Q_NAME    "/ch6_ipc"
#define MAX_SIZE  1024
#define M_EXIT    "done"
#define SRV_FLAG  "-producer"

int main(int argc, char *argv[]) {

    if (argc < 2)   // default would be producer
    {
        producer();
    }
        // -producer -> argument would be producer
    else if (argc >= 2 && 0 == strncmp(argv[1], SRV_FLAG, strlen(SRV_FLAG))) {
        producer();
    }
        // otherwise, consumer
    else {
        consumer();
    }
}

int producer() {
    /* message queue description */
    mqd_t mq;

    /* mq_setattr() sets message queue attributes using information supplied in the mq_attr structure pointed to by newattr. */
    struct mq_attr attr;

    /* Char buffers can be created either by allocation , which allocates space for the buffer's content */
    char buffer[MAX_SIZE];

    /* attribute
    Flags: 0 or O_NONBLOCK
    Max. # of messages on queue
    Max. message size (bytes) *
    # of messages currently in queue
    */
    int msg, i;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    /*
    creates a new POSIX message queue or opens an existing queue

    Q_NAME - Queue Name
    O_CREAT (of-flag) - Create the message queue if it does not exist
    O_WRONLY
              Open the queue to send messages only.

    If O_CREAT is specified in oflag, then two additional arguments
       must be supplied.  The mode argument specifies the permissions to
       be placed on the new queue, as for open(2).
    */
    mq = mq_open(Q_NAME, O_CREAT | O_WRONLY, 0644, &attr);

    /* seed random */
    srand(time(NULL));

    i = 0;
    while (i < 5) {
        /*
        random message
        returns a pseudo-random number in the range of 0 to RAND_MAX.
        */
        msg = rand() % 256;

        /*
        The C library function void *memset(void *str, int c, size_t n) copies the character c (an unsigned char) to the first n characters of the string pointed to, by the argument str.

        copies 0 to the first MAX_SIZE charatcetes of the string by the argument buffer
        fill buffer with all 0
        it's just merely buffer with all 0
        */
        memset(buffer, 0, MAX_SIZE);

        /*
        char str[80];

        sprintf(str, "Value of Pi = %f", M_PI);
        puts(str);

        Value of Pi = 3.141593

        Sends msg to buffer
         */
        sprintf(buffer, "%x", msg);

        /* Print buffer in integer form */
        printf("Produced: %s\n", buffer);

        /* flushes the output buffer of a stream. */
        fflush(stdout);

        /*
        send a message to a message queue
        adds the message pointed to by buffer to the message queue mq (message queue descriptor)
        MAX_SIZE -> length of the messages pointed by buffer
        0 -> priority
        */
        mq_send(mq, buffer, MAX_SIZE, 0);

        i = i + 1;
    }

    /* send M_EXIT(done) message to message queue */
    memset(buffer, 0, MAX_SIZE);
    sprintf(buffer, M_EXIT);
    printf("Produced: %s\n", buffer);
    mq_send(mq, buffer, MAX_SIZE, 0);

    /*
    close a message queue descriptor
    remove a message queue
    */
//    mq_close(mq);
//    mq_unlink(Q_NAME);
    return 0;
}

int consumer() {
    /* message queue attribute would be default */
    struct mq_attr attr;

    char buffer[MAX_SIZE + 1];

    /*
    represent the size of objects in memory.
    size_t is an unsigned integer type of at least 16 bit
    */
    ssize_t bytes_read;

    /* read message queue only for reading */
    mqd_t mq = mq_open(Q_NAME, O_RDONLY);

    if ((mqd_t) - 1 == mq) {
        printf("Either the producer has not been started or maybe I cannot access the same memory...\n");
        exit(1);
    }
    /* Before exit message comes in print buffer from message queue */
    do {
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
        buffer[bytes_read] = '\0';
        printf("Consumed: %s\n", buffer);
    } while (0 != strncmp(buffer, M_EXIT, strlen(M_EXIT)));

//    mq_close(mq);
    return 0;
}
