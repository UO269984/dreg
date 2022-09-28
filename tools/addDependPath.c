#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 7000
#define LINE_BREAK '\n'

static char* insertDependsFolders(char* buffer, const char* dependsStr, const char* folderStr) {
	char* newLinePos;
	size_t folderStrLen = strlen(folderStr);
	
	memcpy(buffer, folderStr, folderStrLen);
	buffer += folderStrLen;
	*(buffer++) = '/';
	
	while ((newLinePos = strchr(dependsStr, LINE_BREAK)) != NULL) {
		newLinePos++; //Also copy the new line character
		
		memcpy(buffer, dependsStr, newLinePos - dependsStr);
		buffer += newLinePos - dependsStr;
		dependsStr = newLinePos;
		
		if (newLinePos[-2] != '\\') {
			memcpy(buffer, folderStr, folderStrLen);
			buffer += folderStrLen;
			*(buffer++) = '/';
		}
	}
	if (*dependsStr == 0) //Last line is empty
		buffer -= folderStrLen + 1; //Delete the last added folderStr
	
	size_t lastLineSize = strlen(dependsStr);
	memcpy(buffer, dependsStr, lastLineSize + 1);
	buffer += lastLineSize;
	
	return buffer;
}

int readStdin(char* buffer, char* bufferEnd) {
	char* curPosBuffer = buffer;
	
	ssize_t readSize;
	while (curPosBuffer < bufferEnd && (readSize = read(STDIN_FILENO, curPosBuffer, bufferEnd - curPosBuffer)) > 0)
		curPosBuffer += readSize;
	
	*curPosBuffer = 0;
	return curPosBuffer < bufferEnd ? curPosBuffer - buffer : -1;
}

int main(int argc, const char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Build folder para required\n");
		return -1;
	}
	
	char stdinBuffer[BUF_SIZE];
	int stdinSize = readStdin(stdinBuffer, stdinBuffer + BUF_SIZE);
	if (stdinSize < 0) {
		fprintf(stderr, "Buffer size exceeded\n");
		return -1;
	}
	
	size_t outputSize = (stdinSize + 2000) * (argc - 1) * sizeof(char);
	char* output = malloc(outputSize);
	char* curBufferPos = output;
	
	for (size_t i = 1; i < argc; i++) {
		curBufferPos = insertDependsFolders(curBufferPos, stdinBuffer, argv[i]);
		
		if (curBufferPos > output + outputSize) {
			fprintf(stderr, "Output buffer size exceeded\n");
			return -1;
		}
	}
	printf("%s", output);
	
	free(output);
	return 0;
}