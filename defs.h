#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_ARR             3 
#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         1
#define USLEEP_TIME     50000
#define BOREDOM_MAX        99

typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;


typedef struct{
	char name[MAX_STR];
	struct Ghost *ghost;
	struct RoomList *rooms;
	struct EvidenceList *evidences;
	struct HunterList *hunters; 
	sem_t mutex;
}RoomType;

typedef struct{
	EvidenceClassType evidenceType;
	float value;
}EvidenceType;


typedef struct{
	EvidenceType *elements[10000]; //array of pointers to evidence types
	int size;
}EvidenceArrayType;


typedef struct{
	RoomType *room;
	char name[MAX_STR];
	int fear;
	int boredom;
	EvidenceArrayType *evidences;
	EvidenceClassType evidenceType;//what evidence they can read
	struct building *b;
}HunterType;

typedef struct HunterArray{
	HunterType *huntersArray[MAX_HUNTERS]; //array of pointers to huntertypes
	int size;
}HunterArrayType;

typedef struct HNode{
	HunterType* data;
	struct HNode* next;
}HunterNode;

typedef struct HunterList{
	HunterNode* head;
	HunterNode* tail;
}HunterListType;

typedef struct Node{
	RoomType* data;
	struct Node* next;
}RoomNode;

typedef struct RoomList{
	RoomNode* head;
	RoomNode* tail;
	int size; 
}RoomListType;

typedef struct ENode{
	EvidenceType* data; //a pointer to the address of the evidencetype
	struct ENode* next;
}EvidenceNode;

typedef struct EvidenceList{
	EvidenceNode* head;
	EvidenceNode* tail;
}EvidenceListType;

typedef struct Ghost{
	GhostClassType ghostType;
	int boredom;
	RoomType *room;
}GhostType;

typedef struct building{
	RoomListType rooms;
	GhostType ghost;
	HunterArrayType hunters;
	EvidenceArrayType evidences;
}BuildingType;

//forward declerations
int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

void populateRooms(BuildingType*);   // Populates the building with sample data for rooms
void cleanUpRoomData(RoomListType *);
void cleanUpRoomNode(RoomListType *);
void cleanUpRoomList(RoomListType *);
void initRoomList(RoomListType *);
void initRoom(RoomType *, char *);

void addToEvidenceList(EvidenceListType*,EvidenceType*);
void initEvidence(EvidenceClassType , int , EvidenceType** );
void initEvidenceList(EvidenceListType*);
void initEvidenceArray(EvidenceArrayType*);
void* generateEvidence(void*);
void addToEvidenceList(EvidenceListType*, EvidenceType*);
void cleanUpEvidenceList(EvidenceListType*);
void addToEvidenceArray(EvidenceArrayType*, EvidenceType*);
void cleanUpEvidenceArray(EvidenceArrayType*);
int checkForGhostly(EvidenceType*);
void* readEvidence(void*);
void printEvidence(EvidenceType*);


void initGhost(GhostClassType, RoomType*, GhostType*);
int checkForHunter(GhostType*);
void* ghostMoveRoom(void*);
void setGhost(GhostType*, RoomListType*);
void printGhost(GhostType*);

void initRoom(RoomType *, char *);
void initRoomList(RoomListType *);
void appendRoom(RoomListType *, RoomType*);
void connectRooms(RoomType*, RoomType*);
void cleanUpRoomData(RoomListType *);
void cleanUpRoomNode(RoomListType *);
void cleanUpRoomList(RoomListType *);

void initHunter(char *, EvidenceClassType , RoomType* , BuildingType* ,HunterType** );
void initHunterArray(HunterArrayType *);
void initHunterList(HunterListType*);
void cleanUpHunterList(HunterListType*);
void* hunterMoveRoom(void*);
void removeHunter(HunterListType*, HunterType*);
void addHunter(HunterListType* , HunterType*);
void cleanUpHunterArray(HunterArrayType*);
void addToHunterArray(HunterArrayType*, HunterType*);
void printHunterList(HunterListType*);
void checkForGhost(HunterType*);
int findGhost(HunterType*);
void exitBuilding(HunterType*);
void* switchDevice(void*);

void initBuilding(BuildingType *);
void cleanUpBuilding(BuildingType *);
