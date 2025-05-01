#include <fcntl.h>
#include <unistd.h>

void main() {
    int fd_in = open("/dev/pts/31", O_RDONLY);
    if (fd_in == -1) {
        return;
    }

    int fd_out = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd_out == -1) {
        close(fd_in);
        return;
    }

    char buf[20];
    ssize_t bytes_read;

    while ((bytes_read = read(fd_in, buf, sizeof(buf))) > 0) {
        if (bytes_read >= 3 && buf[0] == 'E' && buf[1] == 'N' && buf[2] == 'D') {
            break;
        }
        write(fd_out, buf, bytes_read);
    }

    close(fd_in);
    close(fd_out);
}

