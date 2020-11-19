/****************************************************************
Graphs
PROGRAM DESCRIPTION: Gets an input file containing coordinate 
points that represent a texting network between n
people, and solves for the respective graph elements. 
BY: Rovie Jerahmeel T. Doculan 201046535
DATE: August 26, 2011
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 100

int time, *color;
int count_cases();
double input_to_vArray(int vertexArray[][2], int currCase);
void init_matrix(int vertexArray[][2], double matrix[][SIZE], double maxCov);
int count_vertices(int vertexArray[][2]);
void DFS_driver(double matrix[][SIZE], int *pred, int *d, int *f, int vcount, int orig[][2]);
void DFS(int i, double matrix[][SIZE], int *pred, int *d, int *f, int vcount, int orig[][2]);
int connected_ba(int *d, int *f, int vcount);
int minimum(int x, int y);
void artPts_driver(int *tag, int *pred, int *d, int *low, double matrix[][SIZE], int vcount);
void artPts(int *d, int *low, double matrix[][SIZE], int *pred, int *tag, int vcount, int i);
int start_vertex(int vertexArray[][2], int vcount);
int isEmptyPQ (int *heap, int *index, double *key, int vcount);
int initPQ (int vcount, int *heap, int *index, double *key);
int extract_min (int *heap, int *index, double *key, int j, int size);
void heapify(int *heap, int *index, double *key, int r, int size);
void decrease_key (int *heap, int *index, double *key, int l, double newkey);
double prim (double matrix[][SIZE], int s, int *pred, int *heap, int *index, double *key, int vcount);
void output(int numberOne, double numberThree, int *tag, int vertexArray[][2], int currCase, int cases, int vcount);

int main() {
	
	int a = 0, b = 0, c = 0, vcount, numberOne, cases, currCase = 0;
	double maxCov = 0.0, *key, numberThree;
	int vertexArray[SIZE][2], orig[SIZE][2], *pred, *d, *f, *tag, *low, *heap, *index;
	double matrix[SIZE][SIZE];
	
	//init vertexArray and matrix
	for (a = 0; a < SIZE; a++) {
		for (b = 0; b < 2; b++) {
			vertexArray[a][b] = -1;
			orig[a][b] = -1;
		}	
		for (c = 0; c < SIZE; c++)
			matrix[a][c] = -1;
	}		

	cases = count_cases();

	while (currCase < cases) {
		maxCov = input_to_vArray(vertexArray, currCase);
		vcount = count_vertices(vertexArray);
		//init dynamic arrays
		color = (int *) calloc(vcount, sizeof(int));
		pred = (int *) calloc(vcount, sizeof(int));
		d = (int *) calloc(vcount, sizeof(int));
		f  = (int *) calloc(vcount, sizeof(int));
		tag = (int *) calloc(vcount, sizeof(int));
		low = (int *) calloc(vcount, sizeof(int));
		heap = (int *) calloc(vcount+1, sizeof(int));
		index = (int *) calloc(vcount+1, sizeof(int));
		key  = (double *) calloc(vcount+1, sizeof(double));
		key[0] = -1000; index[0] = -1000; heap[0] = -1000;
		init_matrix(vertexArray, matrix, maxCov);
	
		//answer number one	
		DFS_driver(matrix, pred, d, f, vcount, orig);
		numberOne = connected_ba(d, f, vcount);
	
		//answer number two
		artPts_driver(tag, pred, d, low, matrix, vcount);
		
		//answer number three
		for (a = 0; a < vcount; a++)
			pred[a] = 0;
		int s = start_vertex(vertexArray, vcount);	
		if (numberOne==1) //possible
			numberThree = prim (matrix, s, pred, heap, index, key, vcount);
		else 
			numberThree = -1;
		
		//output
		output(numberOne, numberThree, tag, vertexArray, currCase, cases, vcount);
	
		//clear memory
		for (a = 0; a < SIZE; a++) {
			for (b = 0; b < 2; b++) {
				vertexArray[a][b] = -1;
				orig[a][b] = -1;
   		}		
   		for (c = 0; c < SIZE; c++)
				matrix[a][c] = -1;
		}						
		for (a = 0; a < vcount; a++) {
			pred[a] = 0; d[a] = 0; f[a] = 0; tag[a] = 0; low[a] = 0; color[a] = 0;
		}	
		for (a = 0; a <= vcount; a++) {
			heap[a] = 0; index[a] = 0; key[a] = 0;	
		}		
		time = 0; numberOne = 0; numberThree = 0; vcount = 0;
	
	++currCase;	
	}			
}

/* Subprogram name: count_cases
	Description: counts number of test cases */
	 
int count_cases() {
	FILE *iPtr;
	char buffer[15] = "";	
	int cases = 1;
		
	if ((iPtr = fopen("test.txt", "r")) == NULL) 
			printf("File could not be opened.\n");
	else {
		while (fgets(buffer, sizeof(buffer), iPtr)!=NULL) {
			if (buffer[0]=='\n')
				if (buffer[1]=='\0') {
					++cases;
				}	
	}}	
	fclose(iPtr);
	return cases;
}	

/* Subprogram name: input_to_vArray
	Description: converts coordinates of input file to vertexArray[n][2] */
	
double input_to_vArray(int vertexArray[][2], int currCase) {
	FILE *iPtr;
	char buffer[15] = "";
	int x = 0, y = 0, z = 0, a = 0, temp = 0;
	double maxCov = 0;
	
	if ((iPtr = fopen("test.txt", "r")) == NULL) 
			printf("File could not be opened.\n");
	else {
		while ((fgets(buffer, sizeof(buffer), iPtr)!=NULL)&&(temp<=currCase)) {
			if (temp==currCase) {
				if (sscanf(buffer, "(%d,%d), %d", &x, &y, &z)==3) {	
					vertexArray[a][0] = x;
					vertexArray[a][1] = y;
					++a;
				}
				if (sscanf(buffer, "%lf", &maxCov)==1);
				if (sscanf(buffer, "(%d,%d)", &x, &y)==2){
					vertexArray[a][0] = x;
					vertexArray[a][1] = y;	
				}
				if (buffer[0] == '\n') temp = currCase + 1;
			}
			else if (buffer[0] == '\n') 
				++temp;	
		}
	}
	fclose(iPtr);
	return maxCov;
}		

/* Subprogram name: count_vertices
	Description: counts number of vertices of graph */
	
int count_vertices(int vertexArray[][2]) {
	int a = 0, count = 0;
	while (vertexArray[a+1][0]!=-1) {
		++count;	++a;
	}	
	return count;
}	

/* Subprogram name: init_matrix
	Description: initializes adjacency matrix */
	
void init_matrix(int vertexArray[][2], double matrix[][SIZE], double maxCov) {
	
	int currX = 0, currY = 0, othrX = 0, othrY = 0, a = 0, b = 0;
	double t1, t2, d, rem, cost, ten, value = 0;
	int count = count_vertices(vertexArray);
	
	for (a = 0; a < count; a++) {
		currX = vertexArray[a][0];
		currY = vertexArray[a][1];
		for(b = 0; b < count; b++) {
			t1 = 0, t2 = 0, d = 0, cost = 0, ten = 0, rem = 0;
			othrX = vertexArray[b][0];
			othrY = vertexArray[b][1];
			if ((othrX==currX) && (othrY==currY)) //if equal current vertex
				value = 0;	
			else {
			//get distance by distance formula
				t1 = abs(othrX-currX);
				t2 = abs(othrY-currY);
				t1 = pow(t1, 2);
				t2 = pow(t2, 2);
				d = t1 + t2;
				d = sqrt(d);
				
				if (d < maxCov) {
					if (d <= 10) value = 5;
					else {
					//if vertex within max coverage area, get cost	
					ten = floor(d/10);
					rem = d - (ten*10);
					cost = (ten*5) + rem;
					value = cost;
					}		
				}
				else value = -1;
			}	
			
			matrix[a][b] = value;
			matrix[b][a] = value;	
		}
}}	

/* Subprogram name: DFS_driver
	Description: calling function of subprogram DFS */
	
void DFS_driver(double matrix[][SIZE], int *pred, int *d, int *f, int vcount, int orig[][2]) {
	int i = 0;
	for (i = 0; i < vcount; i++) {
		if (color[i]==0) {
			DFS(i, matrix, pred, d, f, vcount, orig);	
		}	
}}	

/* Subprogram name: DFS
	Description: depth first search */

void DFS(int i, double matrix[][SIZE], int *pred, int *d, int *f, int vcount, int orig[][2]) {
	color[i] = 1;
	++time;
	d[i] = time;
	int r = 0;

	for (r = 0; r < vcount; r++) {	//scan per row
		if ((matrix[i][r]>0))	{//if adjacent		
			if (color[r] == 0) {	//if current vertex = white
				pred[r] = i+1;
				DFS(r, matrix, pred, d, f, vcount, orig);
			}	
		}
	}				
	color[i] = 2;
	++time;
	f[i] = time;		
}

/* Subprogram name: connected_ba
	Description: checks if graph is connected, answers number 1 */
	
int connected_ba(int *d, int *f, int vcount) {

	int connected = 1, dstart = d[0], fstart = f[0], a = 1, currD = 0, currF = 0;	
	while ((a < vcount) && (connected == 1)) {
		currD = d[a];
		currF = f[a];
		if ((dstart < currD) && (currD < currF)) {
			if (currF < fstart);
			else connected = 0;	
		}		
		else connected = 0;
		++a;
	}	
	return connected;
}	

/* Subprogram name: minimum
	Description: returns the minimum of two numbers */
	
int minimum (int x, int y) {
	if (x < y)
		return x;
	else return y;	
	
}	

/* Subprogram name: artPts_driver
	Description: calling function of subprogram artPts */
	
void artPts_driver(int *tag, int *pred, int *d, int *low, double matrix[][SIZE], int vcount) {
	int a = 0, i, m = 0;
	time = 0;
	for (a = 0; a < vcount; a++) {
		color[a] = 0;	//color <- white	
		pred[a] = 0;	//pred <- 0
		d[a] = 0;
	}	
	artPts(d, low, matrix, pred, tag, vcount, 0);
	m = 0;
	for (i = 1; i < vcount; i++)
		if (pred[i] == 1) ++m;
	if (m >= 2) tag[0] = 1;
	else tag[0] = 0;	
}	

/* Subprogram name: artPts
	Description: searches for articulation points of graph */
	
void artPts(int *d, int *low, double matrix[][SIZE], int *pred, int *tag, int vcount, int i) {
	
	int j = 0;
	color[i] = 1;
	++time;
	d[i] = time;
	low[i] = d[i];
	
	for (j = 0; j < vcount; j++) {
		if (matrix[i][j] > 0) {
			if (color[j] == 0) {	//if vertex = white
				pred[j] = i+1;
				artPts(d, low, matrix, pred, tag, vcount, j);
				if (low[j] >= d[i]) 
					tag[i] = 1;
				low[i] = minimum(low[i], low[j]);	
			}		
			if (color[j] == 1)
				if (pred[i] != j) 
					low[i] = minimum(low[i], d[j]);	
		}
}}	

/* Subprogram name: start_vertex
	Description: returns the starting vertex for generating MST */

int start_vertex(int vertexArray[][2], int vcount) {
	int a = 0, s = 0, x = vertexArray[vcount][0], y = vertexArray[vcount][1], currX = 0, currY = 0;
	double t1 = 0, t2 = 0, d = 0, min = 0;

	currX = vertexArray[0][0]; currY = vertexArray[0][1];
	t1 = abs(currX-x); t2 = abs(currY-y);
	t1 = pow(t1, 2); t2 = pow(t2, 2);
	d = t1 + t2;
	d = sqrt(d);
	min = d;
	
	for (a = 1; a < vcount; a++) {
		currX = vertexArray[a][0]; currY = vertexArray[a][1];
		t1 = abs(currX-x); t2 = abs(currY-y);
		t1 = pow(t1, 2); t2 = pow(t2, 2);
		d = t1 + t2;
		d = sqrt(d);	
		if (d < min) {
			min = d;
			s = a;
		}	
   }	  
   ++s;                       
	return s;
}	

/* Subprogram name: isEmptyPQ
	Description: determines if priority queue is empty */

int isEmptyPQ (int *heap, int *index, double *key, int vcount) {
	int isEmpty = 1, a = 1;
	while ((a <= vcount) && (isEmpty == 1)) {
		if (key[a]!=-2) isEmpty = 0;
		++a;
	}	
	return isEmpty;
}	

/* Subprogram name: init_PQ
	Description: initializes priority queue */
	
void initPQ (int vcount, int *heap, int *index, double *key, int s) {
	int i = 1, l = 1;
	for (l = 1; l <= vcount; l++) {
		if (l==s) {
			heap[1] = s;
			index[s] = 1;
			key[s] = -2;	//-infinity = -2 	
		}
		else {
			++i;
			heap[i] = l;	
			index[l] = i;
			key[l] = 1000;	//infinity = 1000
		}	
	}
}	

/* Subprogram name: heapify
	Description: restore heap-order property of priority queue */
	
void heapify(int *heap, int *index, double *key, int r, int size) {
	double k = key[(heap[r])];
	int l = heap[r];
	int i = r, j = 2 * i;
	int exit = 0;
	while ((j <= size) && (exit == 0)) {
		if ((j < size) && (key[(heap[j+1])] < key[(heap[j])])) ++j;
		if (key[(heap[j])] < k) {
			heap[i] = heap[j];	
			index[(heap[j])] = i;
			i = j; j = 2 * i;
		}
		else exit = 1;
	}	
	heap[i] = l;
	index[l] = i;
}	

/* Subprogram name: extract_min
	Description: returns vertex at the root of heap */
	
int extract_min (int *heap, int *index, double *key, int j, int size) {
	if (size==0);
	else {

		j = heap[1]; heap[1] = heap[size];
		index[(heap[1])] = 1;
		size = size - 1;	
		heapify(heap, index, key, 1, size);
	}	
	return j;
}	

/* Subprogram name: decrease_key
	Description: adjusts priority of vertex l */
	
void decrease_key (int *heap, int *index, double *key, int l, double newkey) {
	key[l] = newkey;
	int i = index[l];
	int j = i/2;
	while ((i>1) && (key[(heap[j])] > newkey)) {
		heap[i] = heap[j];
		index[(heap[j])] = i;
		i = j; j = i/2;
	}	
	heap[i] = l;
	index[l] = i;	
}	

/* Subprogram name: prim
	Description: main function for implementing Prim's Algorithm */

double prim (double matrix[][SIZE], int s, int *pred, int *heap, int *index, double *key, int vcount) {
	initPQ (vcount, heap, index, key, s);
	int size = vcount;
	pred[s-1] = 0;
	int j = s,  a = 0, l = 0;                                                                                                                                                                                                                                                                                                                                                                                                  	
		
	while (!(isEmptyPQ (heap, index, key, vcount))) {
		j = extract_min(heap, index, key, j, size);
		--size;
		key[j] = -2;
		for (a = 0; a < vcount; a++) {
			if (matrix[j-1][a] > 0) {       
				if (matrix[j-1][a] < key[a+1]) {
					l = a + 1;
					pred[a] = j;
					decrease_key(heap, index, key, l, matrix[j-1][a]);
				}		
			}
		}
	}	
	double totcost = 0;
	for (a = 0; a < vcount; a++)
		if ((matrix[a][(pred[a])-1])>0)
		totcost = totcost + ceil(matrix[a][(pred[a])-1]);			
	return totcost; 
}

/* Subprogram name: output
	Description: prints results to output file */
	
void output(int numberOne, double numberThree, int *tag, int vertexArray[][2], int currCase, int cases, int vcount) {
	FILE *iPtr;
	int a = 0, temp = 0, x = 0, y = 0;
	
	if ((iPtr = fopen("201046535.txt", "a")) == NULL) 
			printf("File could not be opened.\n");
	else {
		//for number 1
		if (numberOne==1) fprintf(iPtr, "YES\n");
		else fprintf(iPtr, "NOT POSSIBLE\n");
		
		//for number 2		
		if (numberOne==0) fprintf(iPtr, "NONE\n");
		else { 
			for (a = 0; a < vcount; a++) {
				if (tag[a]>0) ++temp;
			}
			if (temp == 0) fprintf(iPtr, "NONE\n");
			else {
				fprintf(iPtr, "YES ");
				for (a = 0; a < vcount; a++) {
					if (tag[a]>0) {	//if AP
						x = vertexArray[a][0];	
						y = vertexArray[a][1];
						if (temp == 1) { 
							fprintf(iPtr, "(%d,%d)\n", x, y); 
							--temp; 
						}	// if last AP
						else if (temp > 1) { 
							fprintf(iPtr, "(%d,%d),", x, y); 
							--temp;		
						}							 
					}
				}	
			}	
	}
		
		//for number 3
		if ((numberThree <= 0) || (numberOne == 0)) fprintf(iPtr, "NOT POSSIBLE");
		else fprintf(iPtr, "%d", (int)numberThree);
		
		//for newline	
		if (currCase!=(cases-1)) fprintf(iPtr, "\n\n"); 
	}
	fclose(iPtr);	
}	