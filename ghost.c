#include "defs.h"

/*
  Function: initGhost
  Purpose:  initializes a GhostType
       in:  A GhostClassType which is what type the ghost is
       in:  A roomType of the room the ghost is in 
   in/out:  A pointer to a GhostType which gets initialized 
   return:  void
*/
void initGhost(GhostClassType ghostType, RoomType* room, GhostType* ghost){
	ghost->ghostType = ghostType;
	ghost->room = room;
	ghost->boredom = BOREDOM_MAX;
}

/*
  Function: checkForHunter
  Purpose:  checks if a hunter is in the room
   in/out:  A pointer to a GhostType, whos boredom level changes
   return:  an Integer, 1 if the hunter is in the same room as the ghost, 0 otherwise
*/
int checkForHunter(GhostType* ghost){
	if(ghost->room->hunters->head != NULL){
		ghost->boredom = BOREDOM_MAX;
		return 1;
	}else{
		ghost->boredom = ghost->boredom - 1;
		return 0;
	}
}

/*
  Function: ghostMoveRoom
  Purpose:  Moves a ghost to a room
   in/out:  A void pointer to a ghostType
   return:  void pointer
*/
void* ghostMoveRoom(void* ghostArg){
	GhostType* ghost = (GhostType*) ghostArg;
	
	if (sem_wait(&ghost->room->mutex) < 0) {
		      printf("semaphore wait error\n");
		      exit(1);
	}
	int theRoom = randInt(0,ghost->room->rooms->size);
	if(theRoom == ghost->room->rooms->size){
		theRoom = ghost->room->rooms->size -1;
	}
	RoomNode *curr = ghost->room->rooms->head;
	RoomType *prev = ghost->room;
	for(int i=0; i<ghost->room->rooms->size;i++){
		if(i == theRoom){
			ghost->room->ghost = NULL;
			ghost->room = curr->data; 
			break;
		}
		curr = curr->next;
	}
	printf("This ghost is now in the %s\n", ghost->room->name);
	if (sem_post(&prev->mutex) < 0) {
		printf("semaphore wait error\n");
	        exit(1);
	}
}

/*
  Function: setGhost
  Purpose:  places the ghost somewhere at the very beginnning
   in/out:  A pointer to a ghostType
       in:  A pointer to a RoomList
   return:  void
*/
void setGhost(GhostType* ghost, RoomListType* rooms){
	int theRoom = randInt(1,rooms->size-1);
	RoomNode *curr = rooms->head;
	for(int i=0; i<rooms->size;i++){
		if(i == theRoom){ 
			ghost->room = curr->data;
			break;
		}
		curr = curr->next;
	}
	printf("The ghost has been placed in the %s\n", ghost->room->name);
}

/*
  Function: setGhost
  Purpose:  prints the type of ghost
       in:  A pointer to a ghostType
   return:  void
*/
void printGhost(GhostType* ghost){
	if(ghost->ghostType == POLTERGEIST){
		printf("The ghost is a Poltergeist\n");
	}else if (ghost->ghostType == BANSHEE){
		printf("The ghost is a Banshee\n");
	}else if(ghost->ghostType == BULLIES){
		printf("The ghost is a Bully\n");
	}else if(ghost->ghostType == PHANTOM){
		printf("The ghost is a Phantom\n");
	}
}
