#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 5000
#define LINE_BREAK '\n'

char* insert(char* str, const char* replaceStr, size_t moveStrSize) {
	size_t replaceLen = strlen(replaceStr);
	
	memcpy(str + replaceLen, str, moveStrSize);
	memcpy(str, replaceStr, replaceLen);
	
	return str + (replaceLen + moveStrSize);
}

int main(int argc, char const *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Build folder para required\n");
		return -1;
	}
	
	const char* replaceStr = argv[1];
	
	char buffer[BUF_SIZE] = {0};
	char* curBufferPos = insert(buffer, replaceStr, 0);
	ssize_t readSize;
	
	while ((readSize = read(STDIN_FILENO, curBufferPos, 50)) > 0 && curBufferPos < buffer + BUF_SIZE) {
		char found = 0;
		char* newLinePos;
		
		while ((newLinePos = strchr(curBufferPos, LINE_BREAK)) != NULL) {
			curBufferPos = insert(newLinePos + 1, replaceStr, readSize - (newLinePos - curBufferPos + 1));
			found = 1;
		}
		
		if (! found)
			curBufferPos += readSize;
	}
	
	if (curBufferPos < buffer + BUF_SIZE) {
		*(curBufferPos - strlen(replaceStr)) = 0;
		printf("%s", buffer);
		
		return 0;
	}
	else
		fprintf(stderr, "Buffer size exceeded\n");
}