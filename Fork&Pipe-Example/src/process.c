#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int createMatrix();
void squareMatrix();
void writepipe();
void readpipe();
void writefile();

void  main(int argc, char **argv)
{

	int i, pid,n,j,size,k;
	int p[2];
   	pipe(p);
	int **matrix;
	
	n = atoi(argv[1]);   	      /* determine process count */
	size = createMatrix(&matrix); /* create matrix in file and return matrix size */
	
	writepipe(&p,matrix,size);    /* write main process matrix to pipe */
	
	for(i = 0; i < n; i++) {
		
		pid = fork();
		if(pid < 0) {
			printf("Error");
			exit(1);
		} 
		else if (pid == 0) {  /* child process */
			
			for (k = 0; k < size; k++) { 		/* read from pipe */
				for (j = 0; j < size ; j++) {
					read(p[0], &(matrix[k][j]), sizeof(int));
				}
			}
			close(p[0]);
			squareMatrix(matrix,&matrix,size);	/* calculate new matrix */

			writefile(i+1,size,matrix);
			writepipe(&p,matrix,size);		/* write to pipe */
			close (p[1]);			/* close read an write and of the process */
			exit(0);
			
		} 
		else  {		/* main process wait all childs */
			wait(NULL);
		}
		
	}
}
void writefile(int processNum,int size,int **matrix) {
	
	FILE *fp;
	int i,j;
	char *name = malloc(7*sizeof(char));
	sprintf(name, "%d.txt", processNum);
	fp = fopen (name, "w");
	fprintf(fp,"Process-%d %d\n",processNum, getpid());
	for (i = 0; i <size; i++) {
		for (j = 0; j <size; j++) {
			if(j != 0) 
				fprintf(fp, "       ");
			fprintf(fp, "%d", matrix[i][j]);
			
		}

		fprintf(fp, "\n");
	}
	fclose(fp);
}

void writepipe (int *p,int **matrix,int size) {
	
	int i,j; 
	for (i = 0; i < size; i++) { 
		for (j = 0; j < size ; j++) {
			write(p[1], &(matrix[i][j]), sizeof(int));
		}
	}
	
}

void readpipe (int *p,int ***matrix,int size) {
	int i,j; 
	for (i = 0; i < size; i++) { 
		for (j = 0; j < size ; j++) {
			read(p[0], &(*(matrix)[i][j]), sizeof(int));
		}
	}
	
}	

void squareMatrix(int** matrix, int***mult, int size ) {	
	
	int i,j,k, sum = 0;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				sum += (matrix[i][k] * matrix[k][j]);
			}
			(*mult)[i][j] = sum;
			sum = 0;
		}
	}
}

int createMatrix (int *** matrix) {
	
	FILE *inputFile = fopen("matrix.txt", "r");
	char buff[40];
    	char *line,*val;
	int size,i,j,a; 
	
	line = fgets(buff,sizeof(buff),inputFile);  /* get an matrix size */
	size = atoi (line);
	
	*matrix = (int**)malloc(sizeof(int*) * size) ;		/* memory allocate for matrix */
	for(i=0; i<size;i++)  
    		(*matrix)[i] = (int*)malloc(size* sizeof(int*));
    
	for (i = 0; i<size ; i++) {			/* add elements to matrix */			
		fgets(buff,sizeof(buff),inputFile);

		for(j=0;j<size;j++) {
			if (j == 0 )
				val = strtok(buff,",");
			else 
				val = strtok(NULL,",");
			a = atoi(val);
			(*matrix)[i][j] = a;
		}
	}
	
	return size;
	
}


	
