/**********************************************************************
Pathfinding
PROGRAM DESCRIPTION: Use greedy and A* search to find a path from start
to goal in a given map. FUNCTION DESCRIPTIONS & SEARCH ALGORITHMS ARE
EXPLAINED IN COMMENTS BELOW.
BY: Rovie Jerahmeel T. Doculan
DATE: July 19, 2012
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct CANDIDATE {
	int node;
	int prednode; 
	float value;
	float path;
	struct CANDIDATE *next;
};
struct CANDIDATE *root, *conductor, *newnode, *temp, *q;

struct FINAL {
	int x;
	int y;
	struct FINAL *next;
};
struct FINAL *froot, *fconductor, *n;
	
float **matrix, **HEURISTIC;
float SIDE = 1;
float DIAGONAL = 1.5;
int WHITE = 0, GRAY = 1, BLACK = 2;
int *color, *pred, NCOUNT, NROW = 0, NCOL = 0, ctr = 40, StartX, StartY, EndX, EndY;

//General Methods
void set_init_values(FILE *);
void set_adjacency();
int array_to_node(int, int);
int node_to_x(int node);
int node_to_y(int node);

//For Greedy Search
void greedy(int, int);
void greedy_driver();

//For A* Search
void astar(CANDIDATE *, int, int);
void astar_driver();
void enqueue(int n, int p, float v, float a);
void dequeue(int, int, float);
void retrace(int startNode, int goalNode);


int main(int argc, char *argv[]) {
	
	int a = 0, b = 0;
	FILE *iPtr;
	
	if ((iPtr = fopen(argv[1], "r")) == NULL)
		printf("File can't be opened.\n");
		
	else {	
		set_init_values(iPtr);
		set_adjacency();
		greedy_driver();
		astar_driver();
		for (a = 0; a<NCOUNT; a++)
			printf("pred[%d] = %d\n", a, pred[a]);
	}
	
}	

/* FUNCTION NAME: set_init_values
DESCRIPTION: Sets values of global variables and arrays. */

void set_init_values(FILE *iPtr) {
	
	int ctr = 0, a = 0, b = 0, c = 0;
	float d = 0, temp = 0;
	char buffer[30];
   
   while ((fgets(buffer, sizeof(buffer), iPtr))!=NULL) {
	    
		if (ctr == 0) {	//set NROW, NCOL, NCOUNT, HEURISTIC, matrix, pred, color
			sscanf(buffer, "%d,%d", &NROW, &NCOL); 	
			NCOUNT = NROW * NCOL;
			pred = (int *)calloc(NCOUNT, sizeof(int));
			color = (int *)calloc(NCOUNT, sizeof(int));
			for (a = 0; a < NCOUNT; a++)
				pred[a] = -1;
			matrix = (float **)calloc(NCOUNT, sizeof(float));
			for (a = 0; a < NCOUNT; a++) 
				matrix[a] = (float *)calloc(NCOUNT, sizeof(float));		
			for (a = 0; a < NCOUNT; a++) 
				for (b = 0; b < NCOUNT; b++) 
					matrix[a][b] = -1;
			HEURISTIC = (float **)calloc(NROW, sizeof(float));
			for (a = 0; a < NROW; a++) 
				HEURISTIC[a] = (float *)calloc(NCOL, sizeof(float));
		}	
	
		else if (ctr == 1) {	//Set coordinates of start point
			sscanf(buffer, "%d,%d", &StartX, &StartY);	
			--StartX; --StartY;

		}
			
		else if (ctr == 2) {	//Set coordinates of end point
			sscanf(buffer, "%d,%d", &EndX, &EndY);	
			--EndX; --EndY;

		}
		
		else {	//Set values of HEURISTIC table
			c = ctr - 3;
			for (a = 0; a < NCOL; a++) {
				if (buffer[a] == '*')
					HEURISTIC[c][a] = -1;
				else {
					temp = pow((EndY-a), 2) + pow((EndX-c), 2);	
					d = sqrt(temp);
					HEURISTIC[c][a] = d;
					
				}			
			}		
		}		
		++ctr;
	} 
	
   	fclose(iPtr);
    
}	

/* FUNCTION NAME: set_adjacency
DESCRIPTION: Sets values of adjacency matrix. */

void set_adjacency() {

	int x = 0, y = 0, xnode = 0, ynode = 0;
	
	for (x = 0; x < NROW; x++) 
		for (y = 0; y < NCOL; y++) {
		
			if (HEURISTIC[x][y] >= 0) {
				
				if ((y-1) >= 0) {	//left, NW, SW
					if (HEURISTIC[x][y-1] >= 0) {
						xnode = array_to_node(y,x);	
						ynode = array_to_node(y-1,x);
						matrix[xnode][ynode] = HEURISTIC[x][y-1];
					}
					if ((x-1) >= 0)
						if (HEURISTIC[x-1][y-1] >= 0) {
							xnode = array_to_node(y,x);	
							ynode = array_to_node(y-1,x-1);
							matrix[xnode][ynode] = HEURISTIC[x-1][y-1];
						}		
					if ((x+1) < NROW)
						if (HEURISTIC[x+1][y-1] >= 0) {
							xnode = array_to_node(y,x);	
							ynode = array_to_node(y-1,x+1);
							matrix[xnode][ynode] = HEURISTIC[x+1][y-1];
						}				
				}		
				if ((x-1) >= 0) {	//above, NE
					if (HEURISTIC[x-1][y] >= 0) {
						xnode = array_to_node(y,x);	
						ynode = array_to_node(y,x-1);
						matrix[xnode][ynode] = HEURISTIC[x-1][y];
					}		
					if ((y+1) < NCOL)
						if (HEURISTIC[x-1][y+1] >= 0) {
							xnode = array_to_node(y,x);	
							ynode = array_to_node(y+1,x-1);
							matrix[xnode][ynode] = HEURISTIC[x-1][y+1];
						}				
				}
				if ((y+1) < NCOL) {	//right, SE
					if (HEURISTIC[x][y+1] >= 0) {
						xnode = array_to_node(y,x);	
						ynode = array_to_node(y+1,x);
						matrix[xnode][ynode] = HEURISTIC[x][y+1];
					}		
					if ((x+1) < NROW)
						if (HEURISTIC[x+1][y+1] >= 0) {
							xnode = array_to_node(y,x);	
							ynode = array_to_node(y+1,x+1);
							matrix[xnode][ynode] = HEURISTIC[x+1][y+1];
						}				
				}			
				if ((x+1) < NROW)	//bottom
					if (HEURISTIC[x+1][y] >= 0) {
						xnode = array_to_node(y,x);	
						ynode = array_to_node(y,x+1);
						matrix[xnode][ynode] = HEURISTIC[x+1][y];
					}			
			}
		}
	
}	

/* FUNCTION NAME: array_to_node
DESCRIPTION: Converts (x,y) coordinate of an entry in the integer grid to its equivalent 
node. For example, (0,0) corresponds to node 0 & (0,4) corresponds to node 4. */

int array_to_node(int col, int row) {

	int a = ((col+1) + (row*NCOL)) - 1;
	return a;

}

/* FUNCTION NAME: node_to_x
DESCRIPTION: Converts a node to its equivalent x-coordinate. */

int node_to_x(int node) {
	return (node/NCOL);
}

/* FUNCTION NAME: node_to_y
DESCRIPTION: Converts a node to its equivalent y-coordinate. */

int node_to_y(int node){
	return node%NCOL;
}

/* FUNCTION NAME: greedy_driver
DESCRIPTION: Sets initial values and initiates Greedy Search. 
Traces back the final path using the PRED array. */

void greedy_driver() {

	int startNode = array_to_node(StartY, StartX), goalNode = array_to_node(EndY, EndX), start_okay = 0, goal_okay = 0;
	for (int a = 0; a < NCOUNT; a++) {
		pred[a] = -1;
		if (matrix[startNode][a]!=-1)
			start_okay = 1;
		if (matrix[goalNode][a]!=-1)
			goal_okay = 1;
	}
	
	if (start_okay && goal_okay) {
		printf("\nGREEDY: ");
		greedy(startNode, goalNode);
		retrace(startNode, goalNode);
	}
	
	else printf("GREEDY: No possible solution.\n");
	
}

/* FUNCTION NAME: greedy
DESCRIPTION: Greedy search. Performs a graph traversal similar to DFS, only that it
recursively chooses the unvisited, adjacent node with the lowest heuristic in an iteration. */

void greedy(int startNode, int goalNode) {

	color[startNode] = GRAY;

	int y = -1, lowestNode = -1;
	float temp = 1000000;
	
	for (y = 0; y < NCOUNT; y++)
		if ((temp > matrix[startNode][y]) && (matrix[startNode][y]!=-1) && (color[y]==WHITE)) {
			temp = matrix[startNode][y];
			lowestNode = y;
		}		
	
	if (lowestNode == -1 && color[startNode]!=BLACK) { 
		color[startNode] = BLACK;
		lowestNode = pred[startNode];
	}
	else if (lowestNode!=-1) 
		pred[lowestNode] = startNode;
	
	if (lowestNode!=goalNode && (!(color[startNode]==BLACK && lowestNode==-1)))
		greedy(lowestNode, goalNode);
	
}

/* FUNCTION NAME: astar_driver
DESCRIPTION: Sets initial values and initiates A* search. 
Traces back the final path basing on the PRED array. */
 
void astar_driver() {

	int goalNode = array_to_node(EndY, EndX), startNode = array_to_node(StartY, StartX), start_okay = 0, goal_okay = 0;
	for (int a = 0; a < NCOUNT; a++) {
		pred[a] = -1;
		if (matrix[startNode][a]!=-1)
			start_okay = 1;
		if (matrix[goalNode][a]!=-1)
			goal_okay = 1;			
	}	
	
	froot = NULL; fconductor = NULL;
	
	CANDIDATE *curr = (CANDIDATE *)malloc(sizeof(struct CANDIDATE));
	curr -> node = array_to_node(StartY, StartX);
	curr -> prednode = -1;
	curr -> value = -1;
	curr -> next = NULL;
	curr -> path = 0;
			
	if (start_okay && goal_okay) {
		printf("A*: ");
		astar(curr, startNode, goalNode);
		retrace(startNode, goalNode);
	}

	else printf("A*: No possible solution.\n");
	
}

/* FUNCTION NAME: astar
DESCRIPTION: A* search. Adds adjacent nodes to a list of candidate nodes. Then, recursively
chooses the node with the lowest pathcost+heuristic from the CANDIDATE list, while marking their
predecessors in PRED array. */

void astar(CANDIDATE *curr, int startNode, int goalNode) {
	
int x = curr->node, y = 0, lowestnode = 0, tempx = 0, tempy = 0;
	float pathvalue = curr->path, lowestvalue = 1000000;
	
	for (y = 0; y < NCOUNT; y++) {

		if (matrix[x][y]!=-1)
			pathvalue += 1;
		if ((node_to_x(x) != node_to_x(y)) && (node_to_y(x) != node_to_y(y)))
			pathvalue += 0.5;

		if (matrix[x][y]!=-1) 
			enqueue(y, x, matrix[x][y]+pathvalue, pathvalue);

		pathvalue = curr->path;		
	}	
	
	conductor = root;
	while (conductor) {
		if (lowestvalue > conductor->value) {
			temp = conductor;
			lowestvalue = temp->value;
			lowestnode = temp->node;
		}	
		conductor = conductor->next;
	}

	q = (CANDIDATE *)malloc(sizeof(struct CANDIDATE));
	q -> node = temp->node;
	q -> prednode = temp->prednode;
	q -> value = temp->value;
	q -> path = temp->path;
	q -> next = NULL;
	
	pred[lowestnode] = q->prednode;
	
	dequeue(lowestnode, temp->prednode, temp->value);

	if (lowestnode != goalNode && q->path<ctr) astar(q, startNode, goalNode);
	
}

/* FUNCTION NAME: Enqueue
DESCRIPTION: Enqueues a node. */

void enqueue(int n, int p, float v, float a) {

	if (root == NULL) {
		root = (CANDIDATE *)malloc(sizeof(struct CANDIDATE));
		root -> node = n;
		root -> prednode = p;
		root -> value = v;
		root -> path = a;
		root -> next = NULL;
	}
	
	else {
		conductor = root;
		while (conductor->next)
			conductor = conductor->next;
		
		newnode = (CANDIDATE *)malloc(sizeof(struct CANDIDATE));
		newnode -> node = n;
		newnode -> prednode = p;
		newnode -> value = v;
		newnode -> path = a;
		newnode -> next = NULL;
		conductor->next = newnode;
	}
	
}

/* FUNCTION NAME: dequeue
DESCRIPTION: Dequeues a node. */

void dequeue(int n, int p, float a) {
	
	CANDIDATE *r = NULL;
	
	conductor = root;
	while (conductor->next) {
		if ((n==root->node) && (p==root->prednode) && (a==root->value)) {
			r = root->next;
			free(root);
			root = r;
		}
		else if ((n==conductor->next->node) && (p==conductor->next->prednode) && (a==conductor->next->value)) {
			r = conductor->next;
			conductor->next = r->next;
			free(r);
		}
		else conductor = conductor->next;
	}
		
}

/* FUNCTION NAME: retrace
DESCRIPTION: Retraces the final path basing on the PRED array. */

void retrace(int startNode, int goalNode) {

	int a = goalNode, pnode = -2;
	
	froot = (FINAL *)malloc(sizeof(struct FINAL));
	froot->x = node_to_x(goalNode);
	froot->y = node_to_y(goalNode);
	froot->next = NULL;
	
	while (pred[a]!=-1 && pred[startNode]==-1) {
		pnode = pred[a];
		n = (FINAL *)malloc(sizeof(struct FINAL));
		n->x = node_to_x(pnode);
		n->y = node_to_y(pnode);
		n->next = froot;
		froot = n;
		a = pnode;
	}
	
	fconductor = froot;

	if(froot->x == node_to_x(startNode) && froot->y == node_to_y(startNode)){
		while (fconductor) {
			printf("(%d,%d)", fconductor->x+1, fconductor->y+1);
			fconductor = fconductor->next;
		}
		printf("\n");
	}
	
	else printf("No possible solution.\n");
}

