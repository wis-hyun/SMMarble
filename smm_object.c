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

static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
	"강의",
	"식당",
	"실험실",
	"집",
	"실험실로 이동",
	"음식찬스",
	"축제시간"
};

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


char* smmObj_getGradeName(smmObjGrade_e grade){
	return (char*)smmGradeName[grade];
} 


char* smmObj_getTypeName(int type){
	return (char*)smmNodeName[type];
} 


typedef struct smmObject
{
	char name[MAX_CHARNAME];
	smmObjType_e objType;
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
} smmObject_t;

//static smmObject_t smm_node[MAX_NODE];
//static int smmObj_noNode =0;

#if 0
static char smmObj_name[MAX_NODE][MAX_CHARNAME];
static int smmObj_type[MAX_NODE];
static int smmObj_credit[MAX_NODE];
static int smmObj_energy[MAX_NODE];
#endif

//object generation
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

char* smmObj_getNodeName(void* obj)
{
	smmObject_t* ptr= (smmObject_t*)obj;
    return ptr->name;
}

int smmObj_getNodeType(void* obj)
{
    smmObject_t* ptr= (smmObject_t*)obj;
    return ptr->type;
	//return smm_node[node_nr].type;
}

int smmObj_getNodeCredit(void* obj)
{
    smmObject_t* ptr= (smmObject_t*)obj;
    return ptr->credit;
	//return smm_node[node_nr].credit;
}

int smmObj_getNodeEnergy(void* obj)
{
    smmObject_t* ptr= (smmObject_t*)obj;
    return ptr->energy;
	//return smm_node[node_nr].energy;
}

int smmObj_getNodeGrade(void* obj)
{
	smmObject_t* ptr= (smmObject_t*)obj;
    return ptr->grade;
}


#if 0
//member retrieving

//element to string
char* smmObj_getNodeName(smmNode_e type)
{
    return smmNodeName[type];
}

char* smmObj_getGradeName(smmGrade_e grade)
{
    return smmGradeName[grade];
}
#endif

