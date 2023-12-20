//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE      100
// Array containing names for each node type
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
	"강의",
	"식당",
	"실험실",
	"집",
	"실험실로 이동",
	"음식찬스",
	"축제시간"
};
// Array containing grade names
static char smmGradeName[9][MAX_CHARNAME] = {
	"A+",
	"A0",
	"A-",
	"B+",
	"B0",
	"B-",
	"C+",
	"C0",
	"C-"
};

// Function to get the grade name based on the grade enumeration
char* smmObj_getGradeName(smmObjGrade_e grade){
	return (char*)smmGradeName[grade];
} 


// Function to get the node type name based on the node type enumeration
char* smmObj_getTypeName(int type){
	return (char*)smmNodeName[type];
} 

// Structure representing a SM Marble object
typedef struct smmObject
{
	char name[MAX_CHARNAME];
	smmObjType_e objType;
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
} smmObject_t;

// Function to generate a SM Marble object with specified parameters
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{
	smmObject_t* ptr;
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
}
// Function to get the name of a SM Marble object
char* smmObj_getNodeName(void* obj)
{
	smmObject_t* ptr= (smmObject_t*)obj;
    return ptr->name;
}
// Function to get the type of a SM Marble object
int smmObj_getNodeType(void* obj)
{
    smmObject_t* ptr= (smmObject_t*)obj;
    return ptr->type;
}
// Function to get the credit value of a SM Marble object
int smmObj_getNodeCredit(void* obj)
{
    smmObject_t* ptr= (smmObject_t*)obj;
    return ptr->credit;
}
// Function to get the energy value of a SM Marble object
int smmObj_getNodeEnergy(void* obj)
{
    smmObject_t* ptr= (smmObject_t*)obj;
    return ptr->energy;
}
// Function to get the grade value of a SM Marble object
int smmObj_getNodeGrade(void* obj)
{
	smmObject_t* ptr= (smmObject_t*)obj;
    return ptr->grade;
}
