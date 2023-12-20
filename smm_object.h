//
//  smm_object.h
//  SMMarble object
//
//  Created by Juyeop Kim on 2023/11/05.
//

#ifndef smm_object_h
#define smm_object_h

#define SMMNODE_TYPE_LECTURE	0
#define SMMNODE_TYPE_RESTAURANT	1
#define SMMNODE_TYPE_LABORATORY	2
#define SMMNODE_TYPE_HOME	3
#define SMMNODE_TYPE_GOTOLAB	4
#define SMMNODE_TYPE_FOODCHANCE	5
#define SMMNODE_TYPE_FESTIVAL	6

#define SMMNODE_TYPE_MAX	7
// Enumeration for object types
typedef enum smmObjType{
	smmObjType_board =0,
	smmObjType_card,
	smmObjType_grade
}smmObjType_e;
// Enumeration for object grades
typedef enum smmObjGrade{
	smmObjGrade_Ap =0,
	smmObjGrade_A0,
	smmObjGrade_Am,
	smmObjGrade_BP,
	smmObjGrade_B0,
	smmObjGrade_Bm,
	smmObjGrade_Cp,
	smmObjGrade_C0,
	smmObjGrade_Cm,
	smmObjGrade_max
}smmObjGrade_e;

typedef enum smmObjGrade smmGrade_e;

// Function prototypes
smmGrade_e takeLecture(int player, char *lectureName, int credit);
void* findGrade(int player, char *lectureName);
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade);

//member retrieving functions
char* smmObj_getNodeName(void*);
int smmObj_getNodeType(void*);
int smmObj_getNodeCredit(void*);
int smmObj_getNodeEnergy(void*);
int smmObj_getNodeGrade(void*);


//element to string functions
char* smmObj_getTypeName(int type);
char* smmObj_getGradeName(smmObjGrade_e grade);

#endif /* smm_object_h */
