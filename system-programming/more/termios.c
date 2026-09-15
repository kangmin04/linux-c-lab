#include<stdio.h> 
#include<termios.h> 
#include<unistd.h> 

int main(void){
	struct termios ttystate; // struct variable that contains terminal configure. 
	tcgetattr(0, &ttystate); // read current configure. (fd = 0 's configute) 
	ttystate.c_lflag &= ~ICANON; // c_lflag : local mode flag's bitmask. ICANON bit determines canonical mode. 
					// And Not ( &= ~) : turn off the ICANON bits. --> without enter! 
	ttystate.c_cc[VMIN] =1; 
	tcsetattr(0, TCSANOW, &ttystate); 
	return 0; 
}
