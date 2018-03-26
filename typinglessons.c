/*                 *******************************************               */
/*                 *                                         *               */
/*                 *    CREATED BY SIMON IYAMU PERISANIDIS   *               */
/*                 *                                         *               */
/*                 *******************************************               */
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>

int getchar_silent(){
    int ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}	

						
int main(int argc,char* argv[]){

	printf("\033[2J\033[1;1H");
	FILE *fp;
	char *filename,screen[64];
	int i,b,j=0,totalletters=0,			/*to sq metraei posa stoixeia yparxoun sto screen*/
	    sq=0,ch,faults=0,letters=0;		/*to j einai enas int pou leei poioi einai to epomeno stoixeio tou buffer pou prepei na antigrafei*/
	struct timeval  tv1, tv2;

	filename=argv[1];
	fp=fopen(filename,"rb");		 /*or rb?*/

	if(fp==NULL){
		printf("The file does not exist in this directory.\n");
		return 1;
	}


	while(!feof(fp)){
		fgetc(fp);
		totalletters++;
	}
	totalletters--;

	fp=fopen(filename,"rb");
	b = fread(screen, sizeof(char), sizeof(screen), fp);		/*gemizei to prwto buffer kai to prwto screen*/
	sq=b;

	gettimeofday(&tv1, NULL);

	while(sq>1){
		printf("\033[2J\033[1;1H");
		printf("                              %.0f",(double)(letters*100)/(double)totalletters);
		putchar('%');
		putchar(10);
		for(i=0;i<sq;i++){
			if(screen[i]!=10)
				putchar(screen[i]);
			else
				putchar(' ');
		}
		putchar(10);
		do{
			ch=getchar_silent();
			if(ch!=screen[0]){
				if(screen[0]==10 && ch==' ')
					break;
				faults++;
				printf("\033[2J\033[1;1H");
				printf("                              %.0f",(double)(letters*100)/(double)totalletters);
				putchar('%');
				putchar(10);
				for(i=0;i<sq;i++){
					if(screen[i]!=10)
						putchar(screen[i]);
					else
						putchar(' ');
				}
				printf("\nYou typed  %c !",ch);
			}
		}while(ch!=screen[0]);
		letters++;
		/*olis8hsh pros ta aristera*/
		for(i=1;i<64;i++)
			screen[i-1]=screen[i];

		if(feof(fp)){
			screen[63]=' ';
			sq--;
		}else
			b=fread(&screen[63],sizeof(char),1,fp);
	}


	gettimeofday(&tv2, NULL);
	double seconds=(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
	double minutes = seconds/60.0;
	int words;
	double acc=100-(double)faults*100/(double)letters;
	words=letters/5;


	printf("\033[2J\033[1;1H");
	
	if(acc>=98){
		fp=fopen("a","rb");
		ch=10;
		while(!feof(fp)){
			if(ch!=EOF){
				if(ch==10){
					putchar(ch);
					printf("                                                    ");
				}else
					putchar(ch);
			}
			ch=fgetc(fp);

		}
		putchar(10);
		printf("                                            Congratulations! You the best typer in the whole universe\n");
	}else if(acc>=93){
		fp=fopen("b","rb");
		ch=10;
		while(!feof(fp)){
			if(ch!=EOF){
				if(ch==10){
					putchar(ch);
					printf("                                                           ");
				}else
					putchar(ch);
			}
			ch=fgetc(fp);
		}
		putchar(10);
		printf("                                            Good job! You've completed the lesson successfully\n");

	}else{
		fp=fopen("c","rb");
		ch=10;
		while(!feof(fp)){
			if(ch!=EOF){
				if(ch==10){
					putchar(ch);
					printf("                                                ");
				}else
					putchar(ch);
			}
			ch=fgetc(fp);
		}
		putchar(10);
		printf("                                                          Too many faults :/ Try again\n");
	}

	putchar(10);
	printf("                                                                  Faults : %d\n",faults);
	printf("                                                                Accuracy : %.0f ",acc);
	putchar('%');
	printf("\n\n                                                                     %.0f WPS\n",(double)words/minutes);
	return 0;
}
