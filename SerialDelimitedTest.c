#include <stdio.h> 
#include <wiringPi.h> 
#include <wiringSerial.h> 
#include <string.h> 
#include <errno.h> 
#include <termios.h> 
#include <unistd.h> 

int main() {
int serial_port; 

if((serial_port = serialOpen("/dev/ttyACM0", 9600)) < 0) {
	//fprintf(stderr, "Unable to open serial device : %s\n", strerror(errno)); 
	return 1; 
}
if (wiringPiSetup() == -1) {
	//fprintf(stderr, "Unable to start wiringPi: %s\n", stderr(errno)); 
	return 1; 
}
printf("Serial port opened successfully. Reading data ... \n"); 

char buf[100]; 
int i = 0; 

while(1) { 

		while(serialDataAvail(serial_port)) {
			char c = serialGetchar(serial_port);
			if(c == '\n') break; 
			buf[i++] = c; 
		}
	buf[i] = '\0'; 
	printf("Received: %s\n", buf); 

	char *tokens[20]; 
	int q = 0; 
	char *token; 
	
	token = strtok(buf, "!"); // note this function is not ideal for threaded programs 
	
	while( token != NULL) {
		tokens[q] = token; 
		q++; 
		printf("%d\n", q); 
		
		token = strtok(NULL, "!"); 
	}
	if(q == 3){
		printf("\n \n Start button press detected \n \n"); 
	}
	
	printf("\nFirst token: %s\n", tokens[0]); 
	printf("\nSecond tokens %s\n", tokens[1]); 

	
	delay(1000); 
	serialPuts(serial_port, "Hi"); 
	delay(100); 
	

	
	buf[0] = '\0'; // clears the buffer. need to do this at end 
	tcflush(serial_port, TCIOFLUSH); 
	delay(1000); 
	q=0; 
	i = 0; 

}

return 0; 

}

