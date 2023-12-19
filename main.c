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

typedef struct player{
	int energy;
	int position;
	char name[MAX_CHARNAME];
	int accumCredit;
	int flag_graduate;
}player_t;

static player_t *cur_player;

//function prototypes
#if 0
int isGraduated(void) //check if any player is graduated
float calcAverageGrade(int player) //calculate average grade of the player
void* findGrade(int player, char *lectureName) //find the grade from the player's grade history
smmGrade_e takeLecture(int player, char *lectureName, int credit)	//take the lecture (insert a grade of the player)

#endif


//generate a new player
void generatePlayers(int n, int initEnergy){
	int i;
	//n time loop
	  
	for(i=0; i<n; i++){
		//input name
		printf("input player %i's name :",i); 
		scanf("%s", cur_player[i].name);
		fflush(stdin);
	
		
		//set position
		cur_player[i].position=0;  
		//set energy
		cur_player[i].energy=initEnergy; 
		//set credit
		cur_player[i].accumCredit=0;
		//set experiment
		cur_player[i].flag_graduate=0;
		
	} 
}

//print grade history of the player
//강의를 들었는지의 유무 , printgrade응용해서 함수 만들기 (성적이력 검색함수정의) 
void printGrades(int player){
	int i;
	void *gradePtr;
	
	printf("player : %i, db len : %i\n", player, smmdb_len(LISTNO_OFFSET_GRADE+player));
	
	for(i=0;i<smmdb_len(LISTNO_OFFSET_GRADE+player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE+player, i); 
		printf("%s : %s\n", smmObj_getNodeName(gradePtr), smmObj_getGradeName(smmObj_getNodeGrade(gradePtr)));    
	}
}

//print all player status at the beginning of each turn
void printPlayerStatus(void){
	int i;
	for (i=0;i<player_nr;i++){
		printf("%s : credit %i, energy %i, position %i, experiment %i\n", cur_player[i].name, cur_player[i].accumCredit, cur_player[i].energy, cur_player[i].position, cur_player[i].flag_graduate);	
	}
}

int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
    if (c == 'g')
        printGrades(player);
    
    return (rand()%MAX_DIE + 1);
    
}

//action code when a player stays at a node
void actionNode(int player)
{
	void *boardPtr= smmdb_getData(LISTNO_NODE, cur_player[player].position);
	int type = smmObj_getNodeType(boardPtr);
	void *gradePtr;
	char *name = smmObj_getNodeName(boardPtr);
	int grade, success;
	switch(type)
	{
    case SMMNODE_TYPE_LECTURE:
        {
            int ox;
            printf("%s강의를 들으려면 1, 듣지 않으려면 0을 입력하세요: ", name);
            scanf("%d", &ox);
            fflush(stdin);

            if (ox == 1) {
                if (cur_player[player].energy >= smmObj_getNodeEnergy(boardPtr)) {
                    cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
                    cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);	
				
                    grade = rand() % smmObjGrade_max;
                    gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, (smmObjGrade_e)grade);
                    smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
				}else{
					printf("에너지가 부족하여 강의를 들을 수 없습니다.\n");
				}
            }
        }
        break;
    default:

    case SMMNODE_TYPE_RESTAURANT:
	{
		cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
		break;
	} 
	
	case SMMNODE_TYPE_FOODCHANCE:
	{	
		if (food_nr >0){
			int randomFoodCard = rand()%food_nr;
			void *foodCardObj = smmdb_getData(LISTNO_FOODCARD, randomFoodCard);
			printf("%s drew a food card : %s, energy %i\n", cur_player[player].name, smmObj_getNodeName(foodCardObj), smmObj_getNodeEnergy(foodCardObj));
			cur_player[player].energy += smmObj_getNodeEnergy(foodCardObj); 
		}
		break;
	}
	
	case SMMNODE_TYPE_FESTIVAL:
	{
		if (festival_nr >0){
			int randomFestCard = rand()%festival_nr;
			void *festCardObj = smmdb_getData(LISTNO_FESTCARD, randomFestCard);
			printf("%s drew a festival card : %s\n", cur_player[player].name, smmObj_getNodeName(festCardObj));
		}
		break; 
	}
	
	case SMMNODE_TYPE_GOTOLAB:
	{			
		cur_player[player].flag_graduate=1;
		printf("State in Experiment.\n");
		int success = rand()%MAX_DIE +1;
		printf("Experiment Success Value is %d\n", success);
		printf("go to the laboratory.\n");
		cur_player[player].position=SMMNODE_TYPE_LABORATORY;
		case SMMNODE_TYPE_LABORATORY:
		{		
			if(cur_player[player].flag_graduate==1){
				int challenge = rand()%MAX_DIE +1;
				if(challenge < success){
					printf("challenge : %d < success : %d,  try again\n", challenge, success);
					cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
				}
				else{
					printf("challenge : %d >= success : %d,  Ends the experiment.\n", challenge, success);
					cur_player[player].flag_graduate=0;
				}
			}
			else{
				break;
			}
		break;
		}
	break;
	}
	#if 0
	case SMMNODE_TYPE_LABORATORY:
	{		
		if(cur_player[player].flag_graduate==1){
			int challenge = rand()%MAX_DIE +1;
			if(challenge < success){
				printf("challenge : %d < success : %d,  try again\n", challenge, success);
				cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
			}
			else{
				printf("challenge : %d >= success : %d,  Ends the experiment.\n", challenge, success);
				cur_player[player].flag_graduate=0;
			}
		}
		else{
			break;
		}
	break;
	}
	#endif	
	    
	break;
	} 
}

//make player go "step" steps on the board (check if player is graduated)
void goForward(int player, int step)
{
	cur_player[player].position += step;
	if (cur_player[player].position >= board_nr){
		if (cur_player[player].position > board_nr){
			void *firstBoardNode = smmdb_getData(LISTNO_NODE, 0);
			cur_player[player].energy += smmObj_getNodeEnergy(firstBoardNode);
		}
		cur_player[player].position %= board_nr;
	}
	void *boardPtr= smmdb_getData(LISTNO_NODE, cur_player[player].position);
	printf("%s go to node %i (name : %s)\n", cur_player[player].name, cur_player[player].position, smmObj_getNodeName(boardPtr));
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
    while (1) //is anybody graduated?
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
    
    free(cur_player);
    system("PAUSE");
    return 0;
}
