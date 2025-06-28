#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd = open("/dev/virtual_sensor", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
}

	char buffer[128];
	int bytes = read(fd, buffer, sizeof(buffer) - 1);
	if (bytes >= 0) {
		buffer[bytes] = '\0';
		printf("Read from kernel: %s", buffer);
	} else {
		perror("read");
	}

	close(fd);
	return 0;
}
