#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define MAX_SAMPLES 8000 * 2

int main (int argc, char *argv[])
{
    int fd;
    char *myfifo;

    if (argc < 2) {
        fprintf (stderr, "Usage: %s /tmp/audio_fifo\n", argv[0]);
        exit (EXIT_FAILURE);
    }

    myfifo = argv[1];
    mkfifo (myfifo, 0666);
    fd = open (myfifo, O_RDONLY | O_NONBLOCK);

    while (1) {
        fd_set read_fd_set;
        struct timeval tv;
        int rv;

        FD_ZERO (&read_fd_set);
        FD_SET (fd, &read_fd_set);

        /* TODO mark based on bytes send how far we are ahead of time */
        tv.tv_sec = 0;
        tv.tv_usec = 980000;

        rv = select (fd + 1, &read_fd_set, NULL, NULL, &tv);

        if (rv == -1) {
            exit (EXIT_FAILURE);

        } else if (rv == 0) {
            char buffer[MAX_SAMPLES];
            memset (&buffer, 0, MAX_SAMPLES);
            write (STDOUT_FILENO, buffer, MAX_SAMPLES);

        } else {
            if (FD_ISSET (fd, &read_fd_set)) {
                char buffer[MAX_SAMPLES];
                int nbytes;

                nbytes = read (fd, buffer, MAX_SAMPLES);
                if (nbytes < 0) {

                } else if (nbytes == 0) {
                    close (fd);
                    fd = open(myfifo, O_RDONLY | O_NONBLOCK);

                } else {
                    write (STDOUT_FILENO, buffer, nbytes);
                }
            }
        }
    }

    close (fd);
    unlink (myfifo);
    exit (0);
}
