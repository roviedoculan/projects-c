/*******************************
The Perpetual Calendar
BY: Rovie Jerahmeel T. Doculan
DATE: August 22, 2010
*******************************/

#include <stdio.h>

void input_output();

main() {	
	
	printf("\n\n\t\t    *****************************************\n"
				 "\t\t    *                                       *\n"
				 "\t\t    *   Welcome to My Calendar Generator!   *\n"
				 "\t\t    *                                       *\n"
				 "\t\t    *****************************************\n\n\n");
	
	printf("%28cLegend For Input Format:\n"								
				 "%31cMonth: xx (01 - 12)\n"
			    "%30cYear: xxxx (1 - 9999)\n\n\n",' ',' ',' ');
	
	input_output();
	
}	

/* Subprogram name: input_output
	Description: Uses input from user and outputs calendar; contains the bulk of computations */
	
	void input_output() {												
		void header (int, int);
		int normal_year (int, int);
		int leap_year (int, int);
		int days_of_month (int, int);
		int first_day (int, int);
		void repeat_prog();

		int month, year, cont;
	
		printf("\nPlease enter the month: ");								
		scanf("%d", &month);
		while (month<1 || month >12) {
			printf("Oops! Invalid input. Please enter the month: ");
			scanf("%d", &month);
			}
		printf("Please enter the year: ");								
		scanf("%d", &year);
		while (year<1 || year>9999) {
			printf("Oops! Invalid input. Please enter the year: ");
			scanf("%d", &year);
			}
		
		printf("\n%12c",' ');
		for (cont = 58; cont > 0; --cont) 
				printf("_"); 
				
		header(month,year);
				
		printf("%12c",' ');
		for (cont = 58; cont > 0; --cont) 
			printf("_"); 
		printf("\n");													
		printf("\n\t\tSun\tMon\tTue\tWed\tThur\tFri\tSat\n");						 
	
		int daysMonth = days_of_month (month, year);				
  		int firstDay = first_day(month, year);								
   
		printf("\t\t");
		for (cont = 1; cont <= firstDay; cont++)							
				printf("\t");
				
		for (cont = 1; cont <= daysMonth; cont++) {					
			printf("%2d", cont);
			if ((cont + firstDay) % 7 > 0)							
				printf("\t");
			else 
				printf("\n\t\t");
		}
		
		printf("\n%12c",' ');
		for (cont = 58; cont > 0; --cont) 
			printf("_"); 
		printf("\n\n\n");

		repeat_prog();
}

/* Subprogram name: header
	Description: Prints calendar header */
	
	void header (int a, int b) {										
		printf("\n\n%34c",' ');													
		switch (a) {
			case 01:
				printf("January %d\n", b);
				break;
			case 02:
				printf("February %d\n", b);
				break;
			case 03:
				printf("March %d\n", b);
				break;
			case 04:
				printf("April %d\n", b);
				break;		
			case 05:
				printf("May %d\n", b);
				break;
			case 06:
				printf("June %d\n", b);
				break;		
			case 07:
				printf("July %d\n", b);
				break;	
			case 8:
				printf("August %d\n", b);
				break;	
			case 9:
				printf("September %d\n", b);
				break;	
			case 10:
				printf("October %d\n", b);
				break;	
			case 11:
				printf("November %d\n", b);
				break;	
			case 12:
				printf("December %d\n", b);
				break;
			}
		}

/* Subprogram name: days_of_month
	Description: Returns number of days of a specific month */
	
	int days_of_month (int c, int d) {								
		switch (c) {
			case 01:
				return 31;
				break;
			case 02:
				if (d%4 || (!d%400)) 
					return 28;
				else 
					return 29; 
				break;
			case 03:
				return 31;
				break;
			case 04:
				return 30;
				break;
			case 05:
				return 31;
				break;
			case 06:
				return 30;
				break;
			case 07:
				return 31;
				break;
			case 8:
				return 31;
				break;
			case 9:
				return 30;
				break;
			case 10:
				return 31;
				break;
			case 11:
				return 30;
				break;
			case 12:
				return 31;
				break;
		}
	}		
	
/* Subprogram name: first_day
	Description: Computes the position of the first day of the month in the calendar */	
	
	int first_day (int e, int f) {									
		int days_of_month(int,int);
		int temp, temp1;				
		if (e == 1) {		
			int leap_year(int), normal_year(int);					
			int temp = normal_year(f) + leap_year(f) * 2;
			int temp1 = (temp + 31) % 7;
			return temp1;
			}
		else {
			for (temp = 0; e>1; e--) 
				temp = temp + (days_of_month(e-1, f));
			temp1 = (first_day(1, f) + temp) % 7;
			return temp1;
		}
	}

/* Subprogram name: leap_year
	Description: Computes number of leap years bet. a given year and 1582 */
	
	int leap_year (int g) {											
		if (g >= 1582) {
			int temp = g - 1582;
			int temp1 = temp / 4;
			if (temp1%400) {
				int leap = temp1 - temp / 400; 
				return leap;
				}
			else {
			   int leap = temp1;
			   return leap;
			   }
			}
		else {
			int leap = (1582 - g)/4;
			return leap;
			}
		}

/* Subprogram name: normal_year
	Description: Computes number of normal years bet. a given year and 1582 */	
	
	int normal_year (int h) {										
		int leap_year(int);												
		if (h >= 1582) {
			int temp = h - 1582;
			int normal = temp - leap_year(h); 
			return normal;
			}
		else {
			int temp = 1582 - h;
			int normal = temp - leap_year(h);
			return normal;
			}
		}
	
/* Subprogram name: repeat_prog
	Description: Asks user if he/she wants to see calendar of another month; If yes, program control proceeds to input_output; 
	If no, program is terminated; Otherwise, gets input from user again */
		
	void repeat_prog() {												
		char i;
		printf("Do you want to see the calendar of another month? (y/n) ");
		scanf(" %c", &i);
		if (i=='y')	
			input_output();
		else if (i=='n') {
			printf("\n\n%18cThank you for using my calendar generator! ;D",' ');
			exit(1);
			}		
		else 
			while (!(i=='y') && !(i=='n')) {
				printf("Error. ");
				repeat_prog(); 
			}
	}