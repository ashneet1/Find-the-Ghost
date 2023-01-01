#include "defs.h"

/*
  Function: initEvidence
  Purpose:  initializes a EvidenceType
       in:  integer value of the evidence
       in:  A EvidenceClassType which is what sort of evidence it is
   in/out:  A pointer to a pointer of EvidenceType
   return:  void
*/
void initEvidence(EvidenceClassType typeEvidence, int value, EvidenceType** theEvidence){
	(*theEvidence) = (EvidenceType*) calloc(1,sizeof(EvidenceType));
	(*theEvidence)->evidenceType = typeEvidence;
	(*theEvidence)->value = value;
}

/*
  Function: initEvidenceList
  Purpose:  initializes the head and tail EvidencelistType
   in/out:  A pointer to a EvidenceListType which gets initialized 
   return:  void
*/
void initEvidenceList(EvidenceListType* list){
	list->head = NULL;
	list->tail=NULL;
}

/*
  Function: initEvidenceArray
  Purpose:  initializes fields of the EvidenceArrayType
   in/out:  A pointer to a EvidenceArrayType which gets initialized 
   return:  void
*/
void initEvidenceArray(EvidenceArrayType* arr){
	arr->size = 0;
}

/*
  Function: generateEvidence
  Purpose:  add evidences to the room the ghost is in
       in:  A void pointer to a GhostType
   return:  void
*/
void* generateEvidence(void* ghostArg){
	GhostType* ghost = (GhostType*) ghostArg;
	if (sem_wait(&ghost->room->mutex) < 0) {
		      printf("semaphore wait error\n");
		      exit(1);
	}
	EvidenceType* evidence;
	int type = randInt(0,2);
	int value;
	if(ghost->ghostType == POLTERGEIST){
		if(type == 0){
			value = randFloat(0,5);
			initEvidence(EMF,value,&evidence);
		}else if(type == 2){
			value = randFloat(-10,27);
			initEvidence(TEMPERATURE,value,&evidence);
		}else{
			value = randFloat(0,1);
			initEvidence(FINGERPRINTS,value,&evidence);
		}
		
	}else if (ghost->ghostType == BANSHEE){
		if(type == 0){
			value = randFloat(0,5);
			initEvidence(EMF,value,&evidence);
		}else if(type == 2){
			value = randFloat(-10,27);
			initEvidence(TEMPERATURE,value,&evidence);
		}else{
			value = randFloat(40,75);
			initEvidence(SOUND,value,&evidence);
		}
	}else if(ghost->ghostType == BULLIES){
		if(type == 0){
			value = randFloat(0,5);
			initEvidence(EMF,value,&evidence);
		}else if(type == 2){
			value = randFloat(0,1);
			initEvidence(FINGERPRINTS,value,&evidence);
		}else{
			value = randFloat(40,75);
			initEvidence(SOUND,value,&evidence);
		}
	}else{
		if(type == 0){
			value = randFloat(-10,27); 
			initEvidence(TEMPERATURE,value,&evidence);
		}else if(type == 2){
			value = randFloat(0,1);
			initEvidence(FINGERPRINTS,value,&evidence);
		}else{
			value = randFloat(40,75);
			initEvidence(SOUND,value,&evidence);
		}
	}
	addToEvidenceList(ghost->room->evidences, evidence);
	printf("Ghost has added evidence to the room\n");
	if (sem_post(&ghost->room->mutex) < 0) {
		      printf("semaphore wait error\n");
		      exit(1);
	}
}

/*
  Function: addToEvidenceList
  Purpose:  add evidences to an EvidenceListType
   in\out:  A pointer to a EvidenceListType
       in:  A pointer to a EvidenceType
   return:  void
*/
void addToEvidenceList(EvidenceListType* list, EvidenceType* evidence){
	EvidenceNode* head = list->head;
	EvidenceNode* tail = list->tail;
	EvidenceNode* node = (EvidenceNode*) calloc(1,sizeof(EvidenceNode));
	node->data = evidence;
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
  Function: addToEvidenceArray
  Purpose:  add evidences to an EvidenceArrayType
   in\out:  A pointer to a EvidenceArrayType
       in:  A pointer to a EvidenceType
   return:  void
*/
void addToEvidenceArray(EvidenceArrayType* arr, EvidenceType* evidence){
	arr->elements[arr->size] = evidence;
	arr->size = arr->size + 1;
}

/*
  Function: cleanUpEvidenceList
  Purpose:  free the nodes and their data in the evidence list
   in/out:  A pointer to a EvidenceListType 
   return:  void
*/
void cleanUpEvidenceList(EvidenceListType* list){
	EvidenceNode* curr = list->head;
	if(curr !=NULL){
		EvidenceNode* theNext = curr->next;
		while(curr->next != NULL){
			free(curr->data);
			free(curr);
			curr = theNext;
			theNext = theNext->next;
		}
		free(curr->data);
		free(curr);
	}
	free(list);
}

/*
  Function: cleanUpEvidenceArray
  Purpose:  free the data in the evidence array
   in/out:  A pointer to a EvidenceArrayType 
   return:  void
*/
void cleanUpEvidenceArray(EvidenceArrayType* arr){
	for(int i = 0;i<arr->size;i++){
		free(arr->elements[i]);
	}
}

/*
  Function: checkForGhostly
  Purpose:  checks if the evidence is ghostly or not
       in:  A pointer to a EvidenceArrayType 
   return:  an intger 1 is the evidence is ghostly, or an integer 0 if the evidence is not ghostly
*/
int checkForGhostly(EvidenceType* evidence){
	if(evidence->evidenceType == EMF){
		if(4.90 < evidence->value&&evidence->value <= 5.00){
			return 1;
		}else{
			return 0;
		}
	}else if(evidence->evidenceType == TEMPERATURE){
		if(-10.00<=evidence->value&&evidence->value<0.00){
			return 1;
		}else{
			return 0;
		}
	}else if(evidence->evidenceType == FINGERPRINTS){
		if(evidence->value==1.00){
			return 1;
		}else{
			return 0;
		}
	}else{
		if(70.00<=evidence->value&&evidence->value<75.00){
			return 1;
		}else{
			return 0;
		}
	}
}

/*
  Function: printEvidence
  Purpose:  prints the EvidenceClassType
       in:  A pointer to a EvidenceType 
   return:  void
*/
void printEvidence(EvidenceType* evidence){
	if(evidence->evidenceType == EMF){
		printf("EMF\n");
	}else if(evidence->evidenceType == TEMPERATURE){
		printf("TEMPERATURE\n");
	}else if(evidence->evidenceType == FINGERPRINTS){
		printf("FINGERPRINTS\n");
	}else{
		printf("SOUND");
	}
}
