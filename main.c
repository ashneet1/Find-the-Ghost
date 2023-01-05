#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/*
  Function: main
  Purpose:  runs the program
   return:  0 if the program ran successfully
*/
int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	int x =0;
	int no = 0;
	
	BuildingType building;
   	initBuilding(&building);
        populateRooms(&building);
	HunterType* hunters[MAX_HUNTERS];
	pthread_t   threads[2];
	
	char name1;
	
	
 	printf("Name for Hunter %d\n: ", 1);
	scanf("%s",&name1);
	initHunter(&name1,EMF,(&building)->rooms.head->data,&building,&hunters[0]);
	addToHunterArray(&(&building)->hunters, hunters[0]);

	setGhost(&(&building)->ghost, &(&building)->rooms); 
	while(x!=2){
		x = 0;
		for(int i = 0;i<MAX_HUNTERS;i++){
			if(hunters[i]->boredom > 0 && hunters[i]->fear < 100 && findGhost(hunters[i]) == -1){
				checkForGhost(hunters[i]);
				no = randInt(0,3);
				if(no == 0){
					pthread_create(&threads[i], NULL, hunterMoveRoom, hunters[i]);
					printf("Hunter %s is in %s \n",hunters[i]->name, hunters[i]->room->name);
				}else if(no == 1){
					pthread_create(&threads[i], NULL, switchDevice, hunters[i]);
				}else{
					printf("The hunter will now read evidence\n");
					pthread_create(&threads[i], NULL, readEvidence, hunters[i]);
				}
			}else{
				exitBuilding(hunters[i]);
				x+=1;
			}
		}
		
		if(building.ghost.boredom >=0){
			if(checkForHunter(&(&building)->ghost) == 1){
				no = randInt(0,1);
				if(no == 0){
					pthread_create(&threads[1], NULL, generateEvidence, &(&building)->ghost);
				}
			}else{
				no  = randInt(0,2);
				if(no == 0){
					pthread_create(&threads[1], NULL, generateEvidence, &(&building)->ghost);
				}else if(no == 1){
					pthread_create(&threads[1], NULL, ghostMoveRoom, &(&building)->ghost);
				}
			}
			
		
		}else{
			x+=1;
		} 
		for (int i = 0; i < 2; ++i) {
			pthread_join(threads[i], NULL);
		}
	}
		
		int k = 0;
		for (int i = 0; i < MAX_HUNTERS; ++i) {
			if(hunters[i]->fear >=100){
				printf("Hunter %s has fear >= 100\n", hunters[i]->name);
				printf("The hunters ran away in fear!\n");
				printGhost(&(&building)->ghost);
				printf("The ghost has won!");
			}else{
				k = findGhost(hunters[i]);
				if(k == 0){
					printf("The speculated ghost is a Poltergeist\n");
					printGhost(&(&building)->ghost);
					printf("The hunters have won!\n");
				}else if (k == 1){
					printf("The speculated ghost is a Banshee\n");
					printGhost(&(&building)->ghost);
					printf("The hunters have won!\n");
				}else if(k == 2){
					printf("The speculated ghost is a Bully\n");
					printGhost(&(&building)->ghost);
					printf("The hunters have won!\n");
				}else if(k == 3){
					printf("The speculated ghost is a Phantom\n");
					printGhost(&(&building)->ghost);
					printf("The hunters have won!\n");
				}else{
					printf("The hunters didn't collect enough evidence\n");
					printGhost(&(&building)->ghost);
					printf("The ghost has won!");
				}
				break;
			}
		}
		
    cleanUpBuilding(&building);
    return 0;
}


/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max)
{
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1) 
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}
