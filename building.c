#include "defs.h"
/*
  Function: initBuilding
  Purpose:  initializes fields of the BuildingType
   in/out:  A pointer to a BuildingType which gets initialized 
   return:  void
*/
void initBuilding(BuildingType *b){
	int a = randInt(0,3);
	initGhost(a,NULL,&b->ghost); 
	initRoomList(&b->rooms);
	initHunterArray(&b->hunters);
	initEvidenceArray(&b->evidences);
}

/*
  Function: populateRooms
  Purpose:  Adds rooms to the buildings room list and connects the rooms
   in/out:  A pointer to a BuildingType 
   return:  void
*/
void populateRooms(BuildingType* building){
    RoomType* van = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(van, "Van");

    RoomType* hallway = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(hallway, "Hallway");
   
    RoomType* master_bedroom = calloc(1, sizeof(RoomType));
    initRoom(master_bedroom, "Master Bedroom");
    RoomType* boys_bedroom = calloc(1, sizeof(RoomType));
    initRoom(boys_bedroom, "Boy's Bedroom");
    RoomType* bathroom = calloc(1, sizeof(RoomType));
    initRoom(bathroom, "Bathroom");
    RoomType* basement = calloc(1, sizeof(RoomType));
    initRoom(basement, "Basement");
    RoomType* basement_hallway = calloc(1, sizeof(RoomType));
    initRoom(basement_hallway, "Basement Hallway");
    RoomType* right_storage_room = calloc(1, sizeof(RoomType));
    initRoom(right_storage_room, "Right Storage Room");
    RoomType* left_storage_room = calloc(1, sizeof(RoomType));
    initRoom(left_storage_room, "Left Storage Room");
    RoomType* kitchen = calloc(1, sizeof(RoomType));
    initRoom(kitchen, "Kitchen");
    RoomType* living_room = calloc(1, sizeof(RoomType));
    initRoom(living_room, "Living Room");
    RoomType* garage = calloc(1, sizeof(RoomType));
    initRoom(garage, "Garage");
    RoomType* utility_room = calloc(1, sizeof(RoomType));
    initRoom(utility_room, "Utility Room");
    initRoomList(&building->rooms);

    appendRoom(&building->rooms, van);
  
    appendRoom(&building->rooms, hallway);
   
    appendRoom(&building->rooms, master_bedroom);
    appendRoom(&building->rooms, boys_bedroom);
    appendRoom(&building->rooms, bathroom);
    appendRoom(&building->rooms, basement);
    appendRoom(&building->rooms, basement_hallway);
    appendRoom(&building->rooms, right_storage_room);
    appendRoom(&building->rooms, left_storage_room);
    appendRoom(&building->rooms, kitchen);
    appendRoom(&building->rooms, living_room);
    appendRoom(&building->rooms, garage);
    appendRoom(&building->rooms, utility_room);


   
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);
    
}

/*
  Function: cleanupBuilding
  Purpose:  Deallocates memory in the buildings room list, the buildings evidence list, and the hunters array
   in/out:  A pointer to a BuildingType 
   return:  void
*/
void cleanUpBuilding(BuildingType *b){
	cleanUpRoomList(&b->rooms);
	cleanUpRoomData(&b->rooms);
	cleanUpRoomNode(&b->rooms);
	cleanUpEvidenceArray(&b->evidences);
	cleanUpHunterArray(&b->hunters);
}
