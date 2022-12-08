#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int inputFile;
int outputFile;

char charLib[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', '\\', ',', '.', '/', '"', '(', ')', '!', '?', '@', '#', '$', '%', '^', '&', '*', '[', ']', '{', '}', '|', ':', ';', '<', '>', '~', '`', 39, '-', '_', '=', '+'};

void addWheel();
void showWheels();
void addBuffer();
void showBuffers();
int writeToFile();
int readFromFile();
void rotate();
void reverse();
char getChar(char input);
void encrypt();
void decrypt(int key);

struct wheel {
	struct wheel* next;
	struct wheel* prev;
	int pos;
	int data[sizeof(charLib)];
};

struct wheel* wHead;
struct wheel* wTail;
struct wheel* wPtr;

void addWheel() {
	struct wheel* newWheel = malloc(sizeof(struct wheel));
	newWheel->pos = 0;
	int wheelSet;
	for (wheelSet = 0; wheelSet < sizeof(charLib); wheelSet++) {
		newWheel->data[wheelSet] = -1;
	}
	for (wheelSet = 0; wheelSet < sizeof(charLib); wheelSet++) {
		int newValPos = rand() % sizeof(charLib);
		while (newWheel->data[newValPos] != -1) {
			newValPos = rand() % sizeof(charLib);
		}
		newWheel->data[newValPos] = wheelSet;
	}
	if (wHead == NULL) {
		wHead = newWheel;
		wTail = wHead;
		wPtr = wTail;
		return;
	}
	wTail->next = newWheel;
	newWheel->prev = wTail;
	wTail = newWheel;
	return;
}

void showWheels() {
	if (wHead == NULL) {
		printf("\nNo Wheels Shown\n\n");
		return;
	}
	printf("\nCharacter Library:\n");
	int dataPos;
	for (dataPos = 0; dataPos < sizeof(charLib) - 1; dataPos++) {
		printf("%c, ", charLib[dataPos]);
	}
	printf("%c\n\n", charLib[dataPos]);
	int currentWheel = 1;
	wPtr = wHead;
	while (wPtr != NULL) {
		printf("Wheel %d:\n", currentWheel);
		currentWheel++;
		for (dataPos = 0; dataPos < sizeof(charLib) - 1; dataPos++) {
			printf("%c, ", charLib[wPtr->data[dataPos]]);
		}
		printf("%c\n\n", charLib[wPtr->data[dataPos]]);
		wPtr = wPtr->next;
	}
	wPtr = wHead;
	return;
}

struct buffer {
	struct buffer* next;
	struct buffer* prev;
	char data[2 * 1024];
};

struct buffer* bHead;
struct buffer* bTail;
struct buffer* bPtr;

void addBuffer() {
	struct buffer* newBuffer = malloc(sizeof(struct buffer));
	if (bHead == NULL) {
		bHead = newBuffer;
		bTail = bHead;
		bPtr = bTail;
		return;
	}
	bTail->next = newBuffer;
	newBuffer->prev = bTail;
	bTail = newBuffer;
	return;
}

void showBuffers() {
	if (bHead == NULL) {
		printf("\nNo Data Stored\n\n");
		return;
	}
	int dataPos;
	printf("\nSaved Data:\n");
	bPtr = bHead;
	while (bPtr != NULL) {
		for (dataPos = 0; dataPos < sizeof(bPtr->data); dataPos++) {
			printf("%c", bPtr->data[dataPos]);
		}
		bPtr = bPtr->next;
	}
	printf("\n\n");
	bPtr = bHead;
	return;
}

int writeToFile() {
	int writeCount = 0;
	bPtr = bHead;
	while (bPtr != NULL) {
		writeCount += write(outputFile, bPtr->data, sizeof(bPtr->data));
		bPtr = bPtr->next;
	}
	bPtr = bHead;
	return writeCount;
}

int readFromFile() {
	int readCount = 0;
	addBuffer();
	int currentCount = read(inputFile, bPtr->data, sizeof(bPtr->data));
	readCount += currentCount;
	while (currentCount == sizeof(bPtr->data)) {
		printf("HI");
		addBuffer();
		bPtr = bTail;
		currentCount = read(inputFile, bPtr->data, sizeof(bPtr->data));
		readCount += currentCount;
	}
	bPtr = bHead;
	return readCount;
}

void rotate() {
	return;
}

void reverse() {
	return;
}

char getChar(char input) {
	if (input == '\0') {
		return '\0';
	}
	if (input == '\n') {
		return '\n';
	}
	wPtr = wHead;
	int location = 0;
	int found = 0;
	while (found == 0 && location < sizeof(charLib)) {
		if (input == charLib[location]) {
			found = 1;
		}
		location++;
	}
	if (found == 0) {
		return '\0';
	}
	while (wPtr != NULL) {
		location = wPtr->data[location];
		wPtr = wPtr->next;
	}
	return charLib[location];
}

void encrypt() {
	int bufPos;
	bPtr = bHead;
	while (bPtr != NULL) {
		for (bufPos = 0; bufPos < sizeof(bPtr->data); bufPos++) {
			bPtr->data[bufPos] = getChar(bPtr->data[bufPos]);
			rotate();
		}
		bPtr = bPtr->next;
	}	
	return;
}

void decrypt(int key) {
	int bufPos;
	for (bufPos = 0; bufPos < key; bufPos++) {
		rotate;
	}
	return;
}

int main(int argc, char** argv) {
	if (argc == 1) {
		printf("\nInput Organization:\n[Input File] [Output File] [Wheel Count] [Random Seed] [Encrypt or Decrypt]\n\n");
		return 0;
	}
	if (argc != 6) {
		printf("\nInvalid Argument Count\n\n");
		return 1;
	}
	srand(atoi(argv[4]));
	inputFile = open(argv[1], O_RDONLY);
	outputFile = open(argv[2], O_WRONLY);
	int mainLoop;
	for (mainLoop = 0; mainLoop < atoi(argv[3]); mainLoop++) {
		addWheel();
	}
	int encryption = 0;
	if (argv[5][0] == 'E' || argv[5][0] == 'e') {
		encryption = 1;
	} else if (argv[5][0] == 'D' || argv[5][0] == 'd') {
		encryption = 0;
	} else {
		printf("\nInvalid Encryption State\n\n");
		return 2;
	}
	showWheels();
	int readCount = readFromFile();
	printf("\n%d Bytes Read\n\n", readCount);
	showBuffers();
	if (encryption == 1) {
		encrypt();
	} else {
		decrypt(readCount);
	}
	printf("\n%d Bytes Written\n\n", writeToFile());
	showBuffers();
	inputFile = close(inputFile);
	outputFile = close(outputFile);
	return 0;
}
