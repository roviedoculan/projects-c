/********************************************************************
Finding the Convex Hull
PROGRAM DESCRIPTION: Given a set of n points, print the vertices 
of the smallest convex polygon that will enclose the set of n points.  
BY: Rovie Jerahmeel T. Doculan 
DATE: December 12, 2011
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#define NEG_INF -100000

int comp = 0, pointcomp = 0;
int straight_line_ba(int *, int *, int);
int count_cases();
int count_points(int);
void input_to_arrays(int *, int *, int);
void insert_element(int *, int *, int, int, int);
int quickhull_driver (int *, int *, int *, int *, int);
int quickhull(int, int, int, int, int *, int *, int *, int *, int, int);
void find_maxPt(int *, int *, int, int, int, int, int *, int *, int);

int main() {

	int *xFinArray, *yFinArray, *xArray, *yArray, currCase = 0, n, p, ctr;
	int cases = count_cases();
	
	while (currCase < cases) {
		
		ctr = 0; comp = 0; pointcomp = 0;
		
		n = count_points(currCase);
		xArray = (int *)calloc(n, sizeof(int));
		yArray = (int *)calloc(n, sizeof(int));
		xFinArray = (int *)calloc(n, sizeof(int));
		yFinArray = (int *)calloc(n, sizeof(int));	
		for (p = 0; p < n; ++p) {
			xArray[p] = NEG_INF;	yArray[p] = NEG_INF;
			xFinArray[p] = NEG_INF; yFinArray[p] = NEG_INF;
		}
			
		input_to_arrays(xArray, yArray, currCase);
		ctr = quickhull_driver (xFinArray, yFinArray, xArray, yArray, n);
			
		for (p = 0; p <= ctr; ++p)
			printf("(%d, %d)\n", xFinArray[p], yFinArray[p]);
		printf("Number of Comparisons: %d\n", comp);	
		printf("Number of Times checked if a point is in a line: %d\n", pointcomp);				
		++currCase;		
	}
}

/*	Subprogram name: count_cases
	Description: Returns number of test cases in input file. */
	
int count_cases() {
	FILE *iPtr;
	char buffer[30];	
	int cases = 1;

	if ((iPtr = fopen("test.txt", "r")) == NULL) 
			printf("File could not be opened.\n");
	else {
		while (fgets(buffer, sizeof(buffer), iPtr)!=NULL) {
			if (buffer[0]=='*') ++cases;
	}}	
	fclose(iPtr);
	return cases;		
}		

/*	Subprogram name: count_points
	Description: Return number of points in current test case. */
	
int count_points(int currCase) {
	FILE *iPtr;
	char buffer[30];
	int n = 0, temp = 0;
	
	if ((iPtr = fopen("test.txt", "r")) == NULL) 
			printf("File could not be opened.\n");
	else {
		while (fgets(buffer, sizeof(buffer), iPtr)!=NULL) {
			if (buffer[0]=='*') ++temp;
			if ((temp==currCase) && (buffer[0]!='E') && (buffer[0]!='\n') && (buffer[0]!='*')) {
				++n;	
			}	
	}}	
	fclose(iPtr);
	return n;
}	

/*	Subprogram name: input_to_arrays
	Description: Stores x- and y- coordinates of the current test 
		case in their respective arrays. */
	
void input_to_arrays(int *xArray, int *yArray, int currCase) {
	FILE *iPtr;
	char buffer[30];
	int x = 0, y = 0, ctr = 0, temp = 0;        
	
	if ((iPtr = fopen("test.txt", "r")) == NULL) 
			printf("File could not be opened.\n");
	else {
		while ((fgets(buffer, sizeof(buffer), iPtr)!=NULL)&&(temp<=currCase)) {
			if (temp==currCase) {
				if (sscanf(buffer, "(%d,%d)", &x, &y)==2) {
					insert_element(xArray, yArray, x, y, ctr);
					++ctr;	
			    } 		                                                                                                                                                                                                         
			}
			if (buffer[0] == '*') ++temp;	
		}
	}
	fclose(iPtr);	
}	

/*	Subprogram name: insert_element
	Description: Insert a vertex to a sorted array. */
		
void insert_element (int *xArray, int *yArray, int x, int y, int ctr) {
	
	int a = ctr;
	if (xArray[0]!=NEG_INF) {
		while (((xArray[a-1] > x)&&(a>0))||((xArray[a-1]==x)&&(yArray[a-1] >= y))) {	//
			xArray[a] = xArray[a-1];
			yArray[a] = yArray[a-1];
			--a;
		}
		xArray[a] = x;
		yArray[a] = y;			
	}	
	else {
		xArray[ctr] = x;
		yArray[ctr] = y;		
	}							
}		

/*	Subprogram name: quickhull_driver
	Description: Executes first division of set of points into upper and lower hulls.
		Driver of subprogram quickhull. */
	
int quickhull_driver (int *xFinArray, int *yFinArray, int *xArray, int *yArray, int n) {
	
	int r, a, b, c, sum, S1ctr = 0, S2ctr = 0, finctr = 0, straight = 0;
	
	int x1 = xArray[0], y1 = yArray[0], x2 = xArray[n-1], y2 = yArray[n-1];
	int *xUpper = (int *)calloc(n, sizeof(int)), *xLower = (int *)calloc(n, sizeof(int));	
	int *yUpper = (int *)calloc(n, sizeof(int)), *yLower = (int *)calloc(n, sizeof(int));		
	for (r = 0; r < n; ++r) {
		xUpper[r] = NEG_INF;	yUpper[r] = NEG_INF;	
		xLower[r] = NEG_INF;	yLower[r] = NEG_INF;			
	}

	insert_element(xFinArray, yFinArray, x1, y1, finctr);
	insert_element(xFinArray, yFinArray, x2, y2, ++finctr);
	straight = straight_line_ba(xArray, yArray, n);
	
	a = y2 - y1; b = x1 - x2; c = ((x1*y2)-(y1*x2));
	for (r = 0; r < n; ++r) {
		if (((xArray[r]!=x1)||(yArray[r]!=y1)) && ((xArray[r]!=x2)||(yArray[r]!=y2))) {
                
			sum = (a * xArray[r]) + (b * yArray[r]) - c;
			if (sum < 0) {
				insert_element(xUpper, yUpper, xArray[r], yArray[r], S1ctr); ++S1ctr;
			}
			else if (sum > 0) {
				insert_element(xLower, yLower, xArray[r], yArray[r], S2ctr); ++S2ctr;				
			}
			else if ((sum == 0) && (straight))
				insert_element(xFinArray, yFinArray, xArray[r], yArray[r], ++finctr);
		}
	}

	finctr = quickhull(x1, y1, x2, y2, xUpper, yUpper, xFinArray, yFinArray, S1ctr, finctr);
	finctr = quickhull(x2, y2, x1, y1, xLower, yLower, xFinArray, yFinArray, S2ctr, finctr);
	return finctr;	
}	

/*	Subprogram name: quickhull
	Description: Recursively determines upper and lower convex hulls.  */
		
int quickhull(int x1, int y1, int x2, int y2, int *xSet, int *ySet, int *xFinArray, int *yFinArray, int ctr, int finctr) {
	
	int x3, y3, a, b, c, r, sum, S1ctr = 0, S2ctr = 0;
	int *xUpper = (int *)calloc(ctr, sizeof(int)), *yUpper = (int *)calloc(ctr, sizeof(int));
	int *xLower = (int *)calloc(ctr, sizeof(int)), *yLower = (int *)calloc(ctr, sizeof(int));
	for (r = 0; r < ctr; ++r) {
		xUpper[r] = NEG_INF;	yUpper[r] = NEG_INF;	
		xLower[r] = NEG_INF;	yLower[r] = NEG_INF;					
	}
	if ((ctr!=0)&&(xSet[0]!=NEG_INF))	{                 
		find_maxPt(&x3, &y3, x1, y1, x2, y2, xSet, ySet, ctr);
		++comp;
		insert_element(xFinArray, yFinArray, x3, y3, ++finctr);		
		a = y3 - y1; b = x1 - x3; c = ((x1*y3)-(y1*x3));
		for (r = 0; r < ctr; ++r) {	
			if (((xSet[r]!=x1)||(ySet[r]!=y1)) && ((xSet[r]!=x3)||(ySet[r]!=y3))) {       			
				sum = (a * xSet[r]) + (b * ySet[r]) - c;
				if ((sum < 0)) {
					insert_element(xUpper, yUpper, xSet[r], ySet[r], S1ctr); ++S1ctr;
				}
			}
		}
					
		a = y2 - y3; b = x3 - x2; c = ((x3*y2)-(y3*x2));
		for (r = 0; r < ctr; ++r) {
			if (((xSet[r]!=x3)||(ySet[r]!=y3)) && ((xSet[r]!=x2)||(ySet[r]!=y2))) {                
				sum = (a * xSet[r]) + (b * ySet[r]) - c;
				if ((sum < 0)) {
					insert_element(xLower, yLower, xSet[r], ySet[r], S2ctr); ++S2ctr;
				}
			}
		}	
					
		finctr = quickhull(x1, y1, x3, y3, xUpper, yUpper, xFinArray, yFinArray, S1ctr, finctr);
		finctr = quickhull(x3, y3, x2, y2, xLower, yLower, xFinArray, yFinArray, S2ctr, finctr);		
	}	
			
	return finctr;
}	

/*	Subprogram name: find_maxPt
	Description: Determines the point that is furthest away from the current line segment
		dividing the set of points.  */

void find_maxPt(int *x, int *y, int x1, int y1, int x2, int y2, int *xSet, int *ySet, int ctr) {
	
	int temp, aMax, xMax, yMax, temp1 = 1;
	for (int a = 0; a < ctr; a++) {
         ++pointcomp;                                                                           
			temp = (x1*y2) + (xSet[a]*y1) + (x2*ySet[a]) - (xSet[a]*y2) - (x2*y1) - (x1*ySet[a]);	
			if (temp1) {
				aMax = temp; temp1 = 0;
			}						
			if (temp >= aMax) {
				aMax = temp;
				xMax = xSet[a];
				yMax = ySet[a];
			}	
	}	
	
	*x = xMax;
	*y = yMax;
}	

/*	Subprogram name: straight_line_ba
	Description: Tests if input set of points forms a straight line. */
		
int straight_line_ba(int *xSet, int *ySet, int n) {
	
	int temp = 1, r = 0;
	double slope, prevSlope = 1;
	while ((r < (n-1)) && (temp)) {
		slope = (ySet[r+1] - ySet[r]); slope /= (xSet[r+1] - xSet[r]);
		if ((prevSlope!=slope)&&(r!=0)) temp = 0;
		else prevSlope = slope;
		++r;			
	}	
	return temp;
	
}	