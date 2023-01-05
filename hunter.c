#include "defs.h"

/*
  Function: initHunter
  Purpose:  initializes a HunterType
       in:  name of the hunter
       in:  A EvidenceClassType which is what sort of evidence they can collect
   in/out:  A roomType of the room the hunter is in
   in/out:  A pointer to a pointer of HunterType
   return:  void
*/
void initHunter(char *name, EvidenceClassType typeEvidence, RoomType* room, BuildingType* building, HunterType** hunter){
	(*hunter) = (HunterType*) calloc(1,sizeof(HunterType)); 
	strcpy((*hunter)->name,name);
	(*hunter)->evidenceType = typeEvidence;
	(*hunter)->room = room; 
	addHunter(room->hunters,(*hunter));
	(*hunter)->fear = 0;
	(*hunter)->boredom = BOREDOM_MAX;
	(*hunter)->b = building; 
	EvidenceArrayType* evidenceArray = (EvidenceArrayType*) calloc(1,sizeof(EvidenceArrayType));
	initEvidenceArray(evidenceArray);
	(*hunter)->evidences = evidenceArray;
}

/*
  Function: initHunterArray
  Purpose:  initializes fields of the HunterArrayType
   in/out:  A pointer to a HunterArrayType which gets initialized 
   return:  void
*/
void initHunterArray(HunterArrayType *arr){
	arr->size = 0;
}

/*
  Function: initHunterList
  Purpose:  initializes the head and tail HunterlistType
   in/out:  A pointer to a HunterListType which gets initialized 
   return:  void
*/
void initHunterList(HunterListType* list){
	list->head = NULL;
	list->tail = NULL;
}

/*
  Function: cleanupHunterList
  Purpose:  go through each nodeto cleanup the hunter list
   in/out:  A pointer to a hunterListType 
   return:  void
*/
void cleanUpHunterList(HunterListType* list){
	HunterNode* curr = list->head;
	if(curr !=NULL){
		HunterNode* theNext = curr->next;
		while(curr->next != NULL){
			free(curr);
			curr = theNext;
			theNext = theNext->next;
		}
		free(curr);
	}
	free(list);
}

/*
  Function: cleanupHunterArray
  Purpose:  freeing HunterType data
   in/out:  A pointer to a hunterArraType
   return:  void
*/
void cleanUpHunterArray(HunterArrayType* arr){
	for(int i = 0;i<arr->size;i++){
		free(arr->huntersArray[i]->evidences);
		free(arr->huntersArray[i]);
	}
}

/*
  Function: checkForGhost
  Purpose:  checks if a ghost is in the room
   in/out:  A pointer to a HunterType, whos boredom and fear level changes
   return:  void
*/
void checkForGhost(HunterType* hunter){
	if(hunter->room->ghost != NULL){
		hunter->fear += 1;
		hunter->boredom = BOREDOM_MAX;
		printf("HUNTER HAS ENCOUNTERED A GHOST!\n");
	}
}

/*
  Function: readEvidence
  Purpose:  Reads evidence in a room 
   in/out:  A pointer to a HunterType
   return:  void
*/
void* readEvidence(void* hunterArg){
	HunterType* hunter = (HunterType*) hunterArg;
	
	if (sem_wait(&hunter->room->mutex) < 0) {
		      printf("semaphore wait error\n");
		      exit(1);
	}
	
	EvidenceNode* curr = hunter->room->evidences->head;
	EvidenceNode* prev = hunter->room->evidences->head;
		while(curr != NULL){
			//printf("current is: \n");
			//printEvidence(curr->data);
			if(curr->data->evidenceType == hunter->evidenceType){
				if(checkForGhostly(curr->data) == 1){
					hunter->boredom = BOREDOM_MAX;
				}else{
					hunter->boredom = hunter->boredom - 1;
				}
				if(curr == hunter->room->evidences->head){
					addToEvidenceArray(&hunter->b->evidences,curr->data);
					addToEvidenceArray(hunter->evidences, curr->data);
					hunter->room->evidences->head = curr->next;
					printf("Hunter %s has read this type of evidence: \n",hunter->name);
					printEvidence(curr->data);
					free(curr);
				}
				break;
			}
			prev = curr;
			curr = curr->next;
		}
	if (sem_post(&hunter->room->mutex) < 0) {
		      printf("semaphore wait error\n");
		      exit(1);
	}
}

/*
  Function: switchDevice
  Purpose:  Changes the type of evidence the hunter can read
   in/out:  A void pointer to a HunterType
   return:  void
*/
void* switchDevice(void* hunterArg){
	HunterType* hunter = (HunterType*) hunterArg;
	int rand;
	rand = randInt(0,4);
	while((rand == 0 && hunter->evidenceType == EMF) || (rand == 1 && hunter->evidenceType == TEMPERATURE) || (rand == 2 && hunter->evidenceType == SOUND) || (rand == 3 && hunter->evidenceType == FINGERPRINTS)){
		rand = randInt(0,3);
	}
	if(rand == 0){
		hunter->evidenceType = EMF;
		printf("The hunter has swtiched their device type to EMF\n");
	}else if(rand == 1){
		hunter->evidenceType = TEMPERATURE;
		printf("The hunter has swtiched their device type to TEMPERATURE\n");
	}else if(rand == 2){
		hunter->evidenceType = SOUND;
		printf("The hunter has swtiched their device type to SOUND\n");
	}else if(rand == 3){
		hunter->evidenceType = FINGERPRINTS;
		printf("The hunter has swtiched their device type to FINGERPRINTS\n");
	}
	
}
/*
  Function: hunterMoveRoom
  Purpose:  Moves a hunter to a room
   in/out:  A void pointer to a HunterType
   return:  void
*/
void* hunterMoveRoom(void* hunterArg){
	HunterType* hunter = (HunterType*) hunterArg;
	if (sem_wait(&hunter->room->mutex) < 0) {
		      printf("semaphore wait error\n");
		      exit(1);
	}
	int theRoom = randInt(0,hunter->room->rooms->size);
	if(theRoom == hunter->room->rooms->size){
		theRoom = hunter->room->rooms->size -1;
	}
	RoomNode *curr = hunter->room->rooms->head;
	RoomType *prev = hunter->room;
	for(int i=0; i<hunter->room->rooms->size;i++){
		if(i == theRoom){
			addHunter(curr->data->hunters,hunter);
			removeHunter(hunter->room->hunters, hunter);
			hunter->room = curr->data;
			break;
		}
		curr = curr->next;
	}
	hunter->boredom = hunter->boredom - 1;
	if (sem_post(&prev->mutex) < 0) {
		printf("semaphore wait error\n");
		exit(1);
	}
}
/*
  Function: removeHunter
  Purpose:  removes a hunter from a HunterListType
   in/out:  A pointer to a HunterTypeList
       in:  A pointer to a hunter
   return:  void
*/
void removeHunter(HunterListType* list, HunterType* hunter){
	HunterNode* curr = list->head;
	HunterNode* prev = NULL;
	while(curr!=NULL){
		if(hunter == curr->data){
			if(curr == list->head){
				list->head = curr->next;
				free(curr);
			}else{
				prev->next = curr->next;
				free(curr);
			}
			break;
		}
		prev = curr;
		curr = curr->next;
	}
}

/*
  Function: addHunter
  Purpose:  adds a hunter to a HunterListType
   in/out:  A pointer to a HunterTypeList
       in:  A pointer to a hunter
   return:  void
*/
void addHunter(HunterListType* list, HunterType* hunter){
	HunterNode* head = list->head;
	HunterNode* tail = list->tail;
	HunterNode* node = (HunterNode*) calloc(1,sizeof(HunterNode));
	node->data = hunter;
	if(head == NULL){
		list->head = node;
		list->tail = node;
		node->next = NULL;
	}else{
		node->next = NULL;
		tail->next = node;
		list->tail = node;
	}

}

/*
  Function: addToHunterArray
  Purpose:  adds a hunter to a HunterArrayType
   in/out:  A pointer to a HunterArrayType
       in:  A pointer to a hunter
   return:  void
*/
void addToHunterArray(HunterArrayType* arr, HunterType* hunter){
	arr->huntersArray[arr->size] = hunter;
	arr->size = arr->size + 1;
}
	
/*
  Function: exitBuilding
  Purpose:  removes the hunter from its room list and makes the pointer to the room null
   in/out:  A pointer to a Huntertype
       in:  A pointer to a hunter
   return:  void
*/
void exitBuilding(HunterType* hunter){
	if(hunter->room != NULL){
		printf("Hunter %s has exited the building\n", hunter->name);
		removeHunter(hunter->room->hunters,hunter);
		hunter->room = NULL;
	}
}

/*
  Function: findGhost
  Purpose:  checks the hunters evidence list, to determine which ghost is in the building
       in:  A pointer to a Huntertype
   return:  An integer to which represents the enum type or -1 if the ghost can't be identified
*/
int findGhost(HunterType* hunter){
	EvidenceClassType find[3];
	find[0] = -1;
	find[1] = -1;
	find[2] = -1;
	int m = 0;
	int a = 1;
	for(int i = EMF;i <= SOUND;i++){
		a = 1;
		for(int j = 0; j < hunter->evidences->size;j++){
			if(hunter->evidences->elements[j]->evidenceType == i && a == 1 && checkForGhostly(hunter->evidences->elements[j]) == 1){
				find[m] = i;
				m++;
				a = 0;
			}
		}	
	
	}
	if(find[0] != -1 && find[1] != -1 && find[2] != -1){
		if(find[0] == TEMPERATURE || find[1] == TEMPERATURE || find[2] == TEMPERATURE){
			if(find[0] == EMF || find[1] == EMF || find[2] == EMF){
				if(find[0] == SOUND || find[1] == SOUND || find[2] == SOUND){
					return 1;
				}else{
					return 0;
				}
			}else{
				return 3;
			}
		}else{
			return 2;
		}
	}else{
		return -1;
	}
}
