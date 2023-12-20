//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"



//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;
static int player_nr;
static int player;
typedef struct player{
	int energy;
	int position;
	char name[MAX_CHARNAME];
	int accumCredit;
	int flag_graduate;
	int success; 
}player_t;

static player_t *cur_player;

//function prototypes
void generatePlayers(int n, int initEnergy);
void printGrades(int player);
void printPlayerStatus(void);
int rolldie(int player);
void actionNode(int player);
void goForward(int player, int step);
smmGrade_e takeLecture(int player, char *lectureName, int credit);
void* findGrade(int player, char *lectureName);
float calcAverageGrade(int player);
int isGraduated(void);
void printPlayerInfo(void);


// Function to generate new players with specified initial energy
void generatePlayers(int n, int initEnergy){
    int i;
    
    // Loop to input player information
    for(i = 0; i < n; i++){
        // Input player's name
        printf("Input player %i's name: ", i); 
        scanf("%s", cur_player[i].name);
        fflush(stdin);
        
        // Set player's position
        cur_player[i].position = 0;  
        
        // Set player's energy
        cur_player[i].energy = initEnergy; 
        
        // Set player's accumulated credit
        cur_player[i].accumCredit = 0;
        
        // Set player's experiment flag
        cur_player[i].flag_graduate = 0;
        
        // Set player's experiment success value
        cur_player[i].success = 0;
    } 
}

// Function to print grade history of a player
void printGrades(int player){
    int i;
    void *gradePtr;
    
    printf("Player: %i, Database Length: %i\n", player, smmdb_len(LISTNO_OFFSET_GRADE + player));
    
    // Loop to print each grade entry
    for(i = 0; i < smmdb_len(LISTNO_OFFSET_GRADE + player); i++){
        gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i); 
        printf("%s: %s\n", smmObj_getNodeName(gradePtr), smmObj_getGradeName(smmObj_getNodeGrade(gradePtr)));    
    }
}

// Function to print all player statuses at the beginning of each turn
void printPlayerStatus(void){
    int i;
    
    // Loop to print each player's status
    for (i = 0; i < player_nr; i++){
        printf("%s: Credit %i, Energy %i, Position %i, Experiment %i\n", cur_player[i].name, cur_player[i].accumCredit, cur_player[i].energy, cur_player[i].position, cur_player[i].flag_graduate);    
    }
}

// Function to roll a die and optionally display grades
int rolldie(int player){
    char c;
    printf("Press any key to roll a die (press 'g' to see grade): ");
    c = getchar();
    fflush(stdin);
    
    // If 'g' is pressed, display player's grade history
    if (c == 'g')
        printGrades(player);
    // Return a random number between 1 and MAX_DIE
    return (rand() % MAX_DIE + 1);
}

// Action code when a player stays at a node
void actionNode(int player)
{
    // Retrieve information about the current board node
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    int type = smmObj_getNodeType(boardPtr);
    void *gradePtr;
    char *name = smmObj_getNodeName(boardPtr);
    int grade, success;

    switch (type)
    {
        case SMMNODE_TYPE_LECTURE:
        {
            int ox;
            // Prompt the player to choose whether to take the lecture or not
            printf("To take the %s lecture, enter 1; to skip, enter 0: ", name);
            scanf("%d", &ox);
            fflush(stdin);

            if (ox == 1)
            {
                 // Call takeLecture to handle the lecture registration
                int result = takeLecture(player, name, smmObj_getNodeCredit(boardPtr));

                // Check if the result is valid (If not taken the lecture before, result value is 1)
                if (cur_player[player].energy >= smmObj_getNodeEnergy(boardPtr)&& result==1)
                {
                	// Assign a random grade
                    int grade = rand() % smmObjGrade_max;
                    void *gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, (smmObjGrade_e)grade);
                    // Add the grade to the player's grade history
					smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
					// Update player's accumulated credit and energy
                    cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
                    cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);

                }
                else
                {
                    printf("Not enough energy to take the %s lecture.\n", name);
                }
            }
            break;
        }

        case SMMNODE_TYPE_RESTAURANT:
        {
            // Increase player's energy by the energy value of the restaurant node
            cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
            break;
        }

        case SMMNODE_TYPE_FOODCHANCE:
        {
            if (food_nr > 0)
            {
                // Draw a random food card and increase player's energy accordingly
                int randomFoodCard = rand() % food_nr;
                void *foodCardObj = smmdb_getData(LISTNO_FOODCARD, randomFoodCard);
                printf("%s drew a food card: %s, energy %i\n", cur_player[player].name, smmObj_getNodeName(foodCardObj), smmObj_getNodeEnergy(foodCardObj));
                cur_player[player].energy += smmObj_getNodeEnergy(foodCardObj);
            }
            break;
        }

        case SMMNODE_TYPE_FESTIVAL:
        {
            if (festival_nr > 0)
            {
                // Draw a random festival card and print a message
                int randomFestCard = rand() % festival_nr;
                void *festCardObj = smmdb_getData(LISTNO_FESTCARD, randomFestCard);
                printf("%s drew a festival card: %s\n", cur_player[player].name, smmObj_getNodeName(festCardObj));
            }
            break;
        }

        case SMMNODE_TYPE_GOTOLAB:
        {
            // Set the flag indicating the player is in an experiment
            cur_player[player].flag_graduate = 1;
            printf("In the Experiment State.\n");

            // Generate a random success value for the experiment
            cur_player[player].success = rand() % MAX_DIE + 1;
            printf("Experiment Success Value is %d\n", cur_player[player].success);

            // Move the player to the laboratory node (node number 8)
            cur_player[player].position = 8; 
            printf("Go to the laboratory.\n");
    	}
    	
        case SMMNODE_TYPE_LABORATORY:{
        	if(cur_player[player].flag_graduate == 1){
        		// Generate a random challenge value
        		int challenge = rand() % MAX_DIE + 1;
                if (challenge < cur_player[player].success)
                {
                    // If challenge is less than success, allow the player to try again
                    printf("Challenge: %d < Success: %d, try again\n", challenge, cur_player[player].success);
                    cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
                    
                }
                else
                {
                    // If challenge is greater than or equal to success, end the experiment
                    printf("Challenge: %d >= Success: %d, Ends the experiment.\n", challenge, cur_player[player].success);
                    cur_player[player].flag_graduate = 0;
                }
			}
			break;
		}
    }
}


//make player go "step" steps on the board (check if player is graduated)
void goForward(int player, int step)
{
    if (cur_player[player].flag_graduate != 1) {
        // Update player position based on the number of steps
        cur_player[player].position += step;

        // Check if the player has moved beyond the board boundary
        if (cur_player[player].position >= board_nr) {
            // Adjust player position and replenish energy if passed the last node on the board
            void *firstBoardNode = smmdb_getData(LISTNO_NODE, 0);
            cur_player[player].energy += smmObj_getNodeEnergy(firstBoardNode);

            // Wrap around the board
            cur_player[player].position %= board_nr;

            // Check if the player's accumulated credits meet the graduation criteria
            if (cur_player[player].accumCredit >= GRADUATE_CREDIT) {
                // Move to HOME node (position 0)
                cur_player[player].position = 0;
            }
        }

        // Retrieve information about the current board node
        void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);

        // Print the player's movement to the console
        printf("%s goes to node %i (name: %s)\n", cur_player[player].name, cur_player[player].position, smmObj_getNodeName(boardPtr));
    }
}


// Function to register a grade for a lecture taken by a player
smmGrade_e takeLecture(int player, char *lectureName, int credit) {
    // Find the grade information for the given lecture name
    void *lecturePtr = findGrade(player, lectureName);

    // If the grade for the lecture already exists
    if (lecturePtr != NULL) {
        printf("Player %s already has a grade for the lecture %s\n", cur_player[player].name, lectureName);
        return smmObj_getNodeGrade(lecturePtr); // Return the current grade
    }
    // If not taken the lecture before, return 1
    return 1;
}

// Function to find the grade for a specific lecture in a player's grade history
void* findGrade(int player, char *lectureName) {
    
	int i;
    // Iterate through the player's grade history
    for (i = 0; i < smmdb_len(LISTNO_OFFSET_GRADE + player); i++) {
        // Get the pointer to the grade record
        void *gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        
        // Compare the lecture name with the name in the grade record
        if (strcmp(smmObj_getNodeName(gradePtr), lectureName) == 0) {
            // Return the grade record for the specified lecture
            return gradePtr;
        }
    }

    // Return NULL if the grade for the given lecture is not found
    return NULL;
} 
// Function to check if any player has graduated
int isGraduated(void) {
    int i;
    for (i = 0; i < player_nr; i++) {
        // Check the current position while wrapping around the board
        int currentPosition = cur_player[i].position % (board_nr + 1);
        // If the credits are fulfilled and the current position is home
        if (cur_player[i].accumCredit >= GRADUATE_CREDIT && currentPosition == 0) {
            // Print the graduation message and return the graduation status
            printf("%s has graduated!\n", cur_player[i].name);
            return 1;
        }
    }
    // If the graduation condition is not satisfied for all players
    return 0;
}

// Function to print information about the lectures taken by the winning player
void printPlayerInfo(void) {    
    printf("Lectures Taken:\n");

    // Traverse the winner's grade records and print course information
    int winnerIndex =0;
    int i;
    for (i = 0; i < smmdb_len(LISTNO_OFFSET_GRADE + winnerIndex); i++) {
        void* gradePtr = findGrade(winnerIndex, smmdb_getData(LISTNO_OFFSET_GRADE + winnerIndex, i));
        // Print lecture information
        printf("- Lecture: %s | Credits: %d | Grade: %s\n", smmObj_getNodeName(gradePtr), smmObj_getNodeCredit(gradePtr), smmObj_getGradeName(smmObj_getNodeGrade(gradePtr)));
    }
    return;
}

int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn=0;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set
        //char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj); 
        
		if (type==SMMNODE_TYPE_HOME)
        	initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
    for (i = 0;i<board_nr;i++)
    {
        void *boardObj=smmdb_getData(LISTNO_NODE, i);
		printf("node %i : %s, %i(%s), credit %i, energy %i\n", i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
    }
    
    //printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    

    //2. food card config   
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set
    {
        //store the parameter set 
        void *boardObj = smmObj_genObject(name, 0, 0, 0, energy, 0);
        smmdb_addTail(LISTNO_FOODCARD, boardObj);
        food_nr++;
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    
    for (i = 0; i < food_nr; i++)
	{
		void *boardObj=smmdb_getData(LISTNO_FOODCARD, i);
    	printf("card %i: %s, energy %i\n", i, smmObj_getNodeName(boardObj), smmObj_getNodeEnergy(boardObj));
	}
    

    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name)==1) //read a festival card string
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, 0, 0, 0, 0, 0);
        smmdb_addTail(LISTNO_FESTCARD, boardObj);
        festival_nr++;
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    
    for(i=0; i< festival_nr; i++){
    	void *boardObj = smmdb_getData(LISTNO_FESTCARD,i);
    	printf("card %i : %s\n", i, smmObj_getNodeName(boardObj));
	}

    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("\n\ninput player no.:");
    	scanf("%d", &player_nr);
    	fflush(stdin);
    }
    while (player_nr <0 || player_nr > MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    
    generatePlayers(player_nr, initEnergy);
    
    
    

    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (!isGraduated()) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        die_result= rolldie(turn);
        printf("die result : %i\n", die_result);
        
		//4-3. go forward
        goForward(turn, die_result);
        printf("player position : %i\n", cur_player[turn].position);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn+1)%player_nr;
    }

	// Game over, print player information
    printPlayerInfo();
	
    free(cur_player);
    system("PAUSE");
    return 0;
}
