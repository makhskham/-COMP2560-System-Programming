#include <fcntl.h>
#include <unistd.h>

void main() {
    int fd_in = open("/dev/pts/29", O_RDONLY);
    if (fd_in == -1) {
        write(2, "Error opening tty device\n", 25);
        return;
    }

    int fd_out = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        write(2, "Error opening log file\n", 23);
        close(fd_in);
        return;
    }

    char buf[20];
    ssize_t bytes_read;

    while ((bytes_read = read(fd_in, buf, sizeof(buf) - 1)) > 0) {
        buf[bytes_read] = '\0'; // Null-terminate for comparison

        if (bytes_read >= 3 && buf[0] == 'E' && buf[1] == 'N' && buf[2] == 'D') {
            break;
        }

        ssize_t bytes_written = write(fd_out, buf, bytes_read);
        if (bytes_written == -1) {
            write(2, "Error writing to log file\n", 26);
            break;
        }
    }

    if (bytes_read == -1) {
        write(2, "Error reading from tty device\n", 30);
    }

    if (close(fd_in) == -1) {
        write(2, "Error closing tty device\n", 25);
    }
    if (close(fd_out) == -1) {
        write(2, "Error closing log file\n", 23);
    }
}
