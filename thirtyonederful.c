/*******************************
31-Derful
BY: Rovie Jerahmeel T. Doculan
DATE: October 15, 2010
*******************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N_SUITS 4
#define N_COLS 5
#define N_ROWS 5

void game_play();
int generate_card();
void print_tables(int a[][N_COLS], int b[], int c[]);
void view_scores();
void add_highscore(int);
void delete_scores();


int main() {

	int option;
	
	printf("\n\n\t\t    *****************************************\n"
				 "\t\t    *                                       *\n"
				 "\t\t    *          Welcome to 31-Derful!        *\n"
				 "\t\t    *                                       *\n"
				 "\t\t    *****************************************\n");
		
	while (option!=4) {
		
		printf("\n\n\nWhat do you want to do?\n"
			 "  (1) Start the game! :D\n"
			 "  (2) View high scores. I need to see first whom shall I defeat. >:]\n"
			 "  (3) Delete high scores. There are just too much for me to defeat. :O\n"
			 "  (4) Quit the game. *NOSEBLEED*\n"
			 "  Enter Option: ");
		scanf(" %d", &option);
		while (option < 1 || option > 4) {
			printf("  Oops. Enter 1 to 4 only: ");
			scanf(" %d", &option);
		}
		
		switch (option) {
			case 1: 
				game_play();
				break;
			case 2:
				view_scores();
				break;
			case 3:
				delete_scores();
				break;
		}

	}
		
	exit(1);
	
}


/* Subprogram name: game_play
	Description: Executes the game proper */
	
	void game_play() {
	
		int cardsleft, extra = 0, row, col, m, n, totalpts, score = 0, temp, temp1 = 0;
		char z;
		int gameboard[N_ROWS][N_COLS] = {{0}, {0}};
		int rowtotal[N_ROWS] = {0};
		int coltotal[N_COLS] = {0};
	
		printf("\n\n\nRules:\n\n"
			"  1. Deal out a deck of cards onto the game board to try to make the rows\n   and columns total to 31.\n"
			"  2. There are 30 cards that will be randomly generated and 25 spaces on\n   the game board. You can place cards over cards you have already put down.\n"
			"  3. Aces are worth 11 pts; Faces are worth 10 pts; Numbers are worth their face  values.\n"
			"  4. You begin with 310 pts. Each time you place a card that raises your\n   total toward 31, that card is subtracted from your score. "
			"But if \n   you go over 31, that value is added.\n"
			"  5. You win if your total points become less than 12. The more cards\n  are left, the higher your score is.\n");
			
		do {
		
			cardsleft = 30, totalpts = 310; 
		
			while (cardsleft > 0 && totalpts > 0) {
			
				printf("\nGenerating card..\n");
				n = generate_card();
				if (n==1) m = 11;
					else if (n>10) m = 10;
					else m = n;
				
				printf("\n  Cards left: %d", cardsleft);
				printf("\n  Points left: %d\n", totalpts);
			
				print_tables(gameboard, rowtotal, coltotal);
			
				printf("Enter row where you want to put the card: ");
				scanf("%d", &row);
				while (row > 5 || row < 1) {
					printf("Oops. Enter 1 to 5 only. Enter row where you want to put the card: ");
					scanf("%d", &row);
				}
				printf("Enter column where you want to put the card: ");
				scanf("%d", &col);
				while (col > 5 || col < 1) {
					printf("Oops. Enter 1 to 5 only. Enter column where you want to put the card: ");
					scanf("%d", &col);
				}
				--row;
				--col;
			
				if (gameboard[row][col]==0) {
					gameboard[row][col] = m;
					rowtotal[row]+=m;
					coltotal[col]+=m;
					totalpts -= (m*2);
				}
			
				else {
					rowtotal[row] = (rowtotal[row] - gameboard[row][col]) + m;
					coltotal[col] = (coltotal[col] - gameboard[row][col]) + m;
					temp = m - gameboard[row][col];
					gameboard[row][col]=m;
					totalpts -= (temp*2);
				}

				for (row = 0; row < N_ROWS; row++) {
					if (rowtotal[row] > 31)
						extra += (rowtotal[row]-31);
					for (col = 0; col < N_COLS; col++) 
						if (coltotal[col] > 31)
							extra += (coltotal[col]-31);
				}
			
				totalpts += (extra - temp1);
				temp1 = extra;
				extra = 0;
				cardsleft--;
				
				print_tables(gameboard, rowtotal, coltotal);

			}
		
		if (cardsleft > 0 || totalpts > 12)
			printf("Awwww sorry, you lost. :(\nScore so far: %d", score);
		else {
			score += 20 + cardsleft;
			printf("Congratulations! You won! :D\nScore so far: %d", score);
		}
	
		printf("\nProceed to next round? (Y/N) ");
		scanf(" %c", &z);
		while (z!='Y' && z!='N' && z!='y' && z!='n') {
			printf("Error. Proceed to next round? (Y/N) ");
			scanf(" %c", &z);
			} 
	
		} while (z=='y'|| z=='Y');

		if (score!=0)
			add_highscore(score);
		return;
	}


/* Subprogram name: generate_card
	Description: Randomly generates the next card to be placed */

	int generate_card() {

		int a, b;
		char *suits[N_SUITS] = {"Hearts", "Diamonds", "Spades", "Clubs"};
	 
		srand(time(NULL));
		a = (rand()%3) + 1;
		b = (rand()%13) + 1;
	
		if (b < 2 || b > 10) 
			switch (b) {
				case 1:
					printf("CARD: Ace of %s\n", suits[a]);
					break;
				case 11:
					printf("CARD: Jack of %s\n", suits[a]);
					break;
				case 12:
					printf("CARD: Queen of %s\n", suits[a]);
					break;	
				case 13:
					printf("CARD: King of %s\n", suits[a]);
					break;	
			}
		
		else printf("CARD: %d of %s\n", b, suits[a]); 
		return b;
	
	}


/* Subprogram name: print_tables
	Description: Prints the game board */
	
	void print_tables(int a[][N_COLS], int b[], int c[]) {
	
		int row, col;
	
		printf("\n");
		for (row = 0; row < N_ROWS; row++) {
			for (col = 0; col < N_COLS; col++)
				printf("%6d", a[row][col]);
			printf("   | %5d\n", b[row]);
		}
		printf("   ----------------------------------------\n");
		for (col = 0; col < N_COLS; col++)
			printf("%6d", c[col]);
		printf("\n\n");

	}


/* Subprogram name: view_scores
	Description: Displays high scores */
	
	void view_scores() {
	
		FILE *sp;
		char name[80];
		int score;

		sp = fopen("scores.txt", "r");
		printf("\n\nHigh Scores:\n");
		
		if ((fgetc(sp))!='0') {	
			while (!feof(sp)) {
				fscanf(sp, "%s%d", name, &score);
				printf("%s %d\n", name, score);
			}	
		}
	
		printf("\nNo more high scores to display. :)");
		fclose (sp);
		
	}


/* Subprogram name: add_highscore
	Description: Adds new high score */
	
	void add_highscore(int score) {

		FILE *sp;
		int min = 0, temp;
		char tempname[80];
		char user[80];
	
		sp = fopen("scores.txt", "r");
	
		while (fscanf(sp, "%s%d\n", tempname, &temp)) {
		if (temp < min || temp == min)
				min = temp;
		}
	
		fclose(sp);
	
		if (score > min) {
			sp = fopen("scores.txt", "a");
			printf("New high score! Please enter name: ");
			scanf("%s", user);
			fprintf(sp, "\n%s %d", user, score);
			fclose (sp);
		}
		return;
	}
	
/* Subprogram name: delete_scores
	Description: Deletes list of high scores */
	
	void delete_scores() {
	
		FILE *sp;
		sp = fopen("scores.txt", "w");
		fprintf(sp, "0");
		fclose(sp);
		printf("\n\nHigh scores deleted.");	
		
	}