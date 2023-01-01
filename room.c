#include "defs.h"

/*
  Function: initRoom
  Purpose:  initializes a room which is a RoomType
   in/out:  A pointer to a RoomType which gets initialized 
       in:  A char value of the name of the room
   return:  void
*/
void initRoom(RoomType *room, char *name){
	if (sem_init(&room->mutex, 0, 1) < 0) {
	    printf("semaphore initialization error\n");
	    exit(1);
	  }
	room->ghost = NULL;
	
	strcpy(room->name,name);
	
	RoomListType* roomL = (RoomListType*) calloc(1,sizeof(RoomListType)); //allocating space for its roomlist
	initRoomList(roomL);
	room->rooms = roomL;
	
	EvidenceListType* evidenceList = (EvidenceListType*) calloc(1,sizeof(EvidenceListType)); //allocating space for its evidence list
	initEvidenceList(evidenceList);
	room->evidences = evidenceList;
	
	HunterListType* hunterList = (HunterListType*) calloc(1,sizeof(HunterListType));
	initHunterList(hunterList); 
	room->hunters = hunterList;
	
}

/*
  Function: initRoomList
  Purpose:  initializes the head,tail and size of the RoomlistType
   in/out:  A pointer to a RoomListType which gets initialized 
   return:  void
*/
void initRoomList(RoomListType *list){
	list->head = NULL;
	list->tail = NULL;
	list->size = 0; 
}

/*
  Function: appendRoom
  Purpose:  Adds a RoomType pointer to the RoomListType 
   in/out:  A pointer to a RoomListType 
       in:  A pointer to a RoomType which we are adding to the list
   return:  void
*/
void appendRoom(RoomListType *list, RoomType* room){
	RoomNode* head = list->head;
	RoomNode* tail = list->tail;
	RoomNode* node = (RoomNode*) calloc(1,sizeof(RoomNode));
	node->data = room; 
	if(head == NULL){
		list->head = node;
		list->tail = node;
		node->next = NULL;
	}else{
		node->next = NULL;
		tail->next = node;
		list->tail = node;
	}
	list->size = list->size + 1;
}

/*
  Function: connectRoom
  Purpose:  Connects two rooms, adding them to each others list
   in\out:  A pointer to a RoomType which we are adding to the list
   in\out:  A pointer to a RoomType which we are adding to the list
   return:  void
*/
void connectRooms(RoomType* firstRoom, RoomType* secondRoom){
	appendRoom(firstRoom->rooms,secondRoom);
	appendRoom(secondRoom->rooms, firstRoom);
}

/*
  Function: cleanupRoomData
  Purpose:  Deallocates a Room lists data 
   in/out:  A pointer to a RoomListType which contains a list of rooms
   return:  void
*/
void cleanUpRoomData(RoomListType *list){
	RoomNode *curr = list->head;
	if(curr!=NULL){
		while(curr->next != NULL){
			free(curr->data);
			curr = curr->next;
		}
		free(curr->data);
	}

}

/*
  Function: cleanupRoomNode
  Purpose:  Deallocates a Room lists node 
   in/out:  A pointer to a RoomListType 
   return:  void
*/
void cleanUpRoomNode(RoomListType *list){
	RoomNode *curr = list->head;
	if(curr!=NULL){
		RoomNode* theNext = curr->next;
		while(theNext!= NULL){
			free(curr);
			curr = theNext;
			theNext = theNext->next;
		}
		free(curr);
	}
}

/*
  Function: cleanupRoomList
  Purpose:  go through each node, go through its nodes, then seperately deallocate the node and its data to cleanup the room list in the building
   in/out:  A pointer to a RoomListType 
   return:  void
*/
void cleanUpRoomList(RoomListType *list){
	RoomNode* curr = list->head;
	if(curr!=NULL){
		while(curr->next != NULL){
			cleanUpRoomNode(curr->data->rooms);
			free(curr->data->rooms);//deallocating the list in the rooms itself
			cleanUpEvidenceList(curr->data->evidences); //deallocating the nodes in and the evidence list
			cleanUpHunterList(curr->data->hunters);//deallocating the nodes in and the hunter list
			curr = curr->next;
		}
		cleanUpRoomNode(curr->data->rooms);
		cleanUpEvidenceList(curr->data->evidences);//deallocating the nodes in and the evidence list
		cleanUpHunterList(curr->data->hunters);
		free(curr->data->rooms); //deallocating the list in the rooms itself
	}
}
