#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct Movie{
    char hallName[50];
    char movieName[50];
    int studentTicketNumb;
    int fullTicketNumb;
    int **hall ;
    int hallWidth;
    int hallHeight;
    int control;
} movie;

char* readLine(FILE *);
void closeFiles(FILE *,FILE *);
int getHallNumber(FILE*);
void processCommand (char*,FILE *,movie*,int);
void createHall (char*,char*,char*,char*,FILE*,movie*,int);
void buyTicket (char**,char*,char*,char*,FILE*,movie*,int);
void cancelTicket(char**,char*,FILE*,movie*,int);
void showHall(char**,FILE*,movie*,int);
void statistics (FILE*,movie*,int);


int main(int argc, char *argv[])
{
	/* open files */
    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen("output.txt","w");

    char *line=NULL;
    int hallNumber=0,i=0;
    hallNumber = getHallNumber(inputFile); /* get create hall command number */
    movie *Movies = (movie*)malloc((hallNumber)*sizeof(movie));

    for (i=0 ; i<hallNumber ; i++) /* all movies free in movies array */
    	Movies[i].control = -1;

    if (inputFile == NULL)
        perror("Error opening input file");

    while(1) {  /* read input file and calling functions in processCommand */
    	line = readLine(inputFile);
        if (line == NULL)
            break;
         processCommand(line,outputFile,Movies,hallNumber);
    }
    free(Movies);
    closeFiles(inputFile,outputFile);
    return 1;
}

void closeFiles(FILE *input,FILE *output) {
	fclose(input);
	fclose(output);
}

int getHallNumber(FILE *input) {

	char buff [500] ;
	char *line=NULL ,*command=NULL;
	int number = 0;

	line = fgets(buff,500,input);
    while(line != NULL ) {
    	command = strtok(line," ");
		if(command != NULL) {
			if (strcmp(command,"CREATEHALL") == 0)
				number++;
		}
		line = fgets(buff,500,input);
    }
    rewind(input); /* fp start to file */
    return number;
}

char* readLine(FILE *input) { /*read input file line by line */

    char buff [500];
    char *line;

    line = fgets(buff,sizeof(buff),input);

    if (line == NULL )
        return NULL;

    return line;
}

void processCommand(char *line,FILE* output,movie Movies[],int hallNumber) { /*determine command and process(send essential function) */

    char *hallName=NULL ,*movieName=NULL,*hallWidth=NULL,*hallHeight=NULL;
    char *seatLabel = NULL,*ticketType = NULL,*ofSeatsRequested = NULL;
    char *command = NULL;

    command = strtok(line," "); /*split command according to " " */

    if(command != NULL) {
		if (strcmp(command,"CREATEHALL") == 0) {
			hallName = strtok(NULL," ");
			movieName = strtok(NULL," ");
			hallWidth= strtok(NULL," ");
			hallHeight = strtok(NULL," ");
			if (hallName && movieName && hallWidth && hallHeight ) {
				 createHall(hallName,movieName,hallWidth,hallHeight,output,Movies,hallNumber);
			 }

		}
		else if (strcmp(command,"BUYTICKET") == 0 ) {

			movieName = strtok(NULL," ");
			seatLabel = strtok(NULL," ");
			ticketType = strtok(NULL," ");
			ofSeatsRequested = strtok(NULL," ");

			buyTicket(&movieName,seatLabel,ticketType,ofSeatsRequested,output,Movies,hallNumber);
		}
		else if (strcmp(command,"CANCELTICKET") == 0 ){

			movieName = strtok(NULL," ");
			seatLabel = strtok(NULL," ");

			cancelTicket(&movieName,seatLabel,output,Movies,hallNumber);
		}
		else if (strcmp(command,"SHOWHALL") == 0 ) {

			hallName = strtok(NULL," ");

			showHall(&hallName,output,Movies,hallNumber);
		}
		else if (strcmp(command,"STATISTICS") == 0 ){

			statistics(output,Movies,hallNumber);
		}
    }
    /*else*/
}

void createHall(char* hallName,char* movieName,char* width,char *height,FILE *output,movie Movies[],int hallNumber) {
    int i = 0;
    int hallWidth = atoi(width);
    int hallHeight = atoi(height);
    movie Movie;

    if (strcmp(hallName,"\"\"") == 0){
    	printf("ERROR: Hall name cannot be empty.\n");
        fprintf(output, "ERROR: Hall name cannot be empty.\n");
        return;
    }
    else if(strcmp(movieName,"\"\"") == 0){
    	printf("ERROR: Movie name cannot be empty.\n");
        fprintf(output,"ERROR: Movie name cannot be empty.\n");
        return;
    }
    else if(hallWidth>26){
    	printf("ERROR: Hall width cannot bigger than 26.\n");
    	fprintf(output,"ERROR: Hall width cannot bigger than 26.\n");
    	return;
    }
    else{
    	movieName = strtok(movieName, "\"");
        hallName = strtok(hallName, "\"");

        if (hallName)
        	strcpy(Movie.hallName, hallName);
        if (movieName)
        	strcpy(Movie.movieName, movieName);

        Movie.hallWidth = hallWidth;
        Movie.hallHeight = hallHeight;
        Movie.studentTicketNumb = 0; Movie.fullTicketNumb = 0;

        Movie.hall = (int **) malloc(sizeof(int*)*hallWidth ) ;
        for(i = 0 ;i<hallWidth ; i++)
        	Movie.hall[i] =  (int*) calloc(hallHeight, sizeof(int));
        Movie.control = 1;

        for (i = 0 ; i<hallNumber+1; i++) {
        	if(Movies[i].control == -1) {
        		Movies[i] = Movie;
        		break;
        	}
        }
    }
}

void buyTicket (char **movieName, char *seatLabel ,char* ticketType,char *requested,FILE *output,movie Movies[],int hallNumber){

	int requestNumber=0,x=0,width=0,width2=0,width3=0,height=0,i=0,j=0,y=0;
    char widthValue;
    char a;
    widthValue = toupper(seatLabel[0]);
    x =(int)widthValue; /*get ASCII code for char */
    width = (x-65);         /* get in alphabet position for char.requested column */
    height = atoi(seatLabel+sizeof(char));
    width2 = width;
    width3=width;

    if(strcmp(*movieName,"\"\"") == 0){     /* movie name control */
    	printf("ERROR: Movie name cannot be empty\n");
    	fprintf(output,"ERROR: Movie name cannot be empty\n");
        return;
    }
    *movieName = strtok(*movieName, "\"");
    requestNumber = atoi(requested); /* requested seat number */

    for(i=0 ; i<hallNumber+1 ; i++){

    	if(Movies[i].control == 1) {
    		if(strcmp(Movies[i].movieName,*movieName) == 0){ /* if finding movie */
    			if(Movies[i].hallHeight<height || Movies[i].hallWidth<width){ /*seat is not defined this movie's hall */
    				printf("ERROR: Seat %s is not defined at %s\n" , seatLabel,Movies[i].hallName);
    				fprintf(output,"ERROR: Seat %s is not defined at %s\n" , seatLabel,Movies[i].hallName);
    				return;
    			}
    			else {  /*if seat is defined */
    				if (requestNumber == 1){
    					if(Movies[i].hall[width][height] == 1 || Movies[i].hall[width][height] == 2 ){
    						printf("ERROR: Seat %s in Red-Hall was not sold.",seatLabel);
    						fprintf(output,"ERROR: Seat %s in Red-Hall was not sold.",seatLabel);
    					}
    				}
    				for(j=0 ; j< requestNumber ; j++) { /*control request seats full or empty */
    					if(Movies[i].hall[width][height] == 1 || Movies[i].hall[width][height] == 2) { /*if already taken */
    						printf("ERROR: Specified seat(s) in %s are not available! They have been already taken.\n",Movies[i].hallName);
    						fprintf(output,"ERROR: Specified seat(s) in %s are not available! They have been already taken.\n",Movies[i].hallName);
    						return;
    					}
    					width++;
    				}
    				printf("%s [%s] Seat(s) ",Movies[i].hallName,Movies[i].movieName);
    				fprintf(output,"%s [%s] Seat(s) ",Movies[i].hallName,Movies[i].movieName);

    				for (j=0 ; j< requestNumber; j++) { /* if all the seat free add the movie hall as the student or full */

    					if(strcmp(ticketType,"Student") == 0)
    						Movies[i].hall[width2++][height] = 1;

    					 else if(strcmp(ticketType,"FullFare") == 0)
    						 Movies[i].hall[width2++][height] = 2;

    					y= width3+j+65; /* ASCII to char */
    					a = y;
    					if (j == requestNumber-1) {
    						printf("%c%d ",a,height);
    						fprintf(output,"%c%d ",a,height);
    						break;
    					}
    					printf("%c%d,",a,height);
    					fprintf(output,"%c%d,",a,height);
    			    }
    				if(strcmp(ticketType,"Student") == 0)
    					Movies[i].studentTicketNumb += requestNumber;
    				else if(strcmp(ticketType,"FullFare") == 0)
    					Movies[i].fullTicketNumb += requestNumber;
    				printf("successfully sold.\n");
    				fprintf(output,"successfully sold.\n");

    				return;
    			}
    		} /*end find movie */
    	}
    } /* end for loop */
    printf("ERROR : Movie [%s] does not exist.\n" , *movieName);
	fprintf(output,"ERROR : Movie [%s] does not exist.\n" , *movieName);
	return;

} /*end buy ticket */

void cancelTicket (char **movieName,char *seatLabel,FILE *output,movie Movies[],int hallNumber){

	int width,height,i,x;
    char widthValue;
    widthValue = toupper(seatLabel[0]);
    x =(int)widthValue; /*get ASCII code for char */
    width = (x-65);         /* get in alphabet position for char.requested column */
    height = atoi(seatLabel+sizeof(char));

    if(strcmp(*movieName,"\"\"") == 0){     /* movie name control */
    	printf("ERROR: Movie name cannot be empty\n");
    	fprintf(output,"ERROR: Movie name cannot be empty\n");
        return;
    }
    *movieName = strtok(*movieName, "\"");

    for(i=0 ; i<hallNumber+1 ; i++){

    	if(Movies[i].control == 1) {
    		if(strcmp(Movies[i].movieName,*movieName) == 0){ /* if finding movie */
    			if(Movies[i].hallHeight<height || Movies[i].hallWidth<width){ /*seat is not defined this movie's hall */
    				printf("ERROR: Seat %s is not defined at %s\n" , seatLabel,Movies[i].hallName);
    				fprintf(output,"ERROR: Seat %s is not defined at %s\n" , seatLabel,Movies[i].hallName);
    				return;
    			}
    			else if (Movies[i].hall[width][height] == 1) {
    				Movies[i].hall[width][height] = 0;
    				Movies[i].studentTicketNumb--;
    				printf("%s [%s] purchase is cancelled. Seat %c%d is now free.\n",Movies[i].hallName,Movies[i].movieName,seatLabel[0],height);
    				fprintf(output,"%s [%s] purchase is cancelled. Seat %c%d is now free.\n",Movies[i].hallName,Movies[i].movieName,seatLabel[0],height);
    				return;
    			}
    			else if (Movies[i].hall[width][height] == 2) {
    				Movies[i].hall[width][height] = 0;
    				Movies[i].fullTicketNumb--;
    				printf("%s [%s] purchase is cancelled. Seat %c%d is now free.\n",Movies[i].hallName,Movies[i].movieName,seatLabel[0],height);
    				fprintf(output,"%s [%s] purchase is cancelled. Seat %c%d is now free.\n",Movies[i].hallName,Movies[i].movieName,seatLabel[0],height);
    				return;
    			}
    			else{
    				printf("ERROR: Seat %c%d in %s was not sold.\n",seatLabel[0],height,Movies[i].hallName);
    				fprintf(output,"ERROR: Seat %c%d in %s was not sold.\n",seatLabel[0],height,Movies[i].hallName);
    				return;
    			}
    		}
    	}
    }
    printf("ERROR : Movie [%s] does not exist.\n" , *movieName);
    fprintf(output,"ERROR : Movie [%s] does not exist.\n" , *movieName);
    return;

} /* end cancelticket function */

void showHall(char **hallName,FILE *output,movie Movies[],int hallNumber) {

	int i,j,k,l;
	char ticketType;

	if(strcmp(*hallName,"\"\"") == 0){     /* movie name control */
		printf("ERROR: Hall name cannot be empty\n");
    	fprintf(output,"ERROR: Hall name cannot be empty\n");
        return;
    }
	*hallName = strtok(*hallName, "\"");

	for(i=0 ; i<hallNumber+1 ; i++) {
		if(Movies[i].control == 1){ /* control movies list */

			if(strcmp(Movies[i].hallName,*hallName) == 0) { /*finding hall*/
				printf("%s sitting plan\n",Movies[i].hallName);
				fprintf(output,"%s sitting plan\n",Movies[i].hallName);
				for(k =0 ; k<Movies[i].hallHeight;k++) {
					printf("  ");
					fprintf(output,"  ");
					for(j=0 ; j<2*(Movies[i].hallWidth)+1 ; j++){
						printf("-");
						fprintf(output,"-");
					}
					if(k+1>9) {
						printf("\n%d",k+1);
						fprintf(output,"\n%d",k+1);
					}
					else{
						printf("\n%d ",k+1);
						fprintf(output,"\n%d ",k+1);
					}
					for(l=0; l<Movies[i].hallWidth+1 ; l++) {
						if( l == Movies[i].hallWidth){
							printf("|");
							fprintf(output,"|"); break;
						}
						if(Movies[i].hall[l][k+1] == 0 )
							ticketType = ' ';
						else if(Movies[i].hall[l][k+1] == 1)
							ticketType = 's';
						else if(Movies[i].hall[l][k+1] == 2)
							ticketType = 'f';
						printf("|%c",ticketType);
						fprintf(output,"|%c",ticketType);

					}
					printf("\n");
					fprintf(output,"\n");
				}
				fprintf(output,"  ");
				for(k=0 ; k<2*(Movies[i].hallWidth)+1 ; k++){
					printf("-");
					fprintf(output,"-");
				}
				printf("\n   ");
				fprintf(output,"\n   ");
				for(k=0 ; k<Movies[i].hallWidth ; k++) {
					printf("%c ",k+65);
					fprintf(output,"%c ",k+65);
				}
				printf("\n");
				fprintf(output,"\n");
				for (k=0 ; k<Movies[i].hallWidth ; k++) {
					printf("  ");
					fprintf(output,"  ");
					if(k>=Movies[i].hallWidth/3) {
						printf("C U R T A I N\n");
						fprintf(output,"C U R T A I N\n");
						break;

					}
				}
				return;
			}
		}
	}
}

void statistics(FILE *output,movie Movies[],int hallNumber){

	int i,totalPrice;
	printf("Statistics\n");
	fprintf(output,"Statistics\n");
	for(i = 0 ; i<hallNumber+1 ; i++) {
		if(Movies[i].control == 1 ) {
			totalPrice = Movies[i].fullTicketNumb * 10 + Movies[i].studentTicketNumb * 7; /*calculate total price */
			printf("%s %d student(s) %d full fare(s), sum:%d TL\n",Movies[i].movieName,Movies[i].studentTicketNumb,Movies[i].fullTicketNumb,totalPrice);
			fprintf(output,"%s %d student(s) %d full fare(s), sum:%d TL\n",Movies[i].movieName,Movies[i].studentTicketNumb,Movies[i].fullTicketNumb,totalPrice);

		}
	}
}
