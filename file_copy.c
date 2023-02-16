#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int input_fd, output_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    struct stat input_stat;

    // Check for the correct number of command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the input file for reading
    input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Open the output file for writing
    output_fd = open(argv[2], O_CREAT | O_WRONLY, 0644);
    if (output_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Get the permissions of the input file
    if (fstat(input_fd, &input_stat) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    // Read from the input file and write to the output file
    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(output_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Set the permissions of the output file to match the input file
    if (fchmod(output_fd, input_stat.st_mode) == -1) {
        perror("fchmod");
        exit(EXIT_FAILURE);
    }

    // Close the input and output files
    if (close(input_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    if (close(output_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}
