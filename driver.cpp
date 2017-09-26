
#include </home/aketcham0691/Documents/CSE321/Lab 1/Drone/drone.h>

struct thread_data {
   int  id;
   std::vector<std::vector<int> >  *map;
   std::set<int>  *motion;
};

void *startDrone(void* threadarg){
  struct thread_data *droneData;
  droneData = (struct thread_data *) threadarg;
  drone* d = new drone(droneData->id, droneData->map, droneData->motion);
  int x = 3;
  int y = 3;
  while (x == 3 && y == 3){
    x = rand()%8;
    y = rand()%8;
  }
  d->setGoal(x,y);
  d->setHome(3,3);
  d->spawn(3,3);
  maplock.lock();
  std::cout<<"DRONE ID TAKEOFF: "<<d->id<<std::endl;
  std::cout<<"DRONE "<<d->id<<" TARGET: "<< "("<< d->goal[0]<< ","<<d->goal[1]<< ")"<<std::endl;
  setlock.lock();
  if (!(*(d->motion)).empty()){
    std::cout<<"OTHER DRONES IN MOTION/WAITING ON FREE LOCATION: ";
    printSet(*(d->motion));
  }
  printMap(*(d->map));
  setlock.unlock();
  maplock.unlock();
  sleep(3);
  while(true){
    d->move();
    maplock.lock();
    if (d-> home == false){
      std::cout<<"DRONE ID MOVING: "<<d->id<<std::endl;;
      setlock.lock();
      if (!(*(d->motion)).empty()){
        std::cout<<"OTHER DRONES IN MOTION/WAITING ON FREE LOCATION: ";
        printSet(*(d->motion));
      }
      setlock.unlock();
      printMap(*(d->map));
    }

    maplock.unlock();
    sleep(3);

  }
  pthread_exit(NULL);

}

int main()
{
  pthread_attr_t attr;
   void *status;

   // Initialize and set thread joinable
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  srand ( time(NULL) );
  std::set<int> inMotion;
  std::vector<std::vector<int> >  v(8);
  for (int i = 0; i <8;++i){
    for (int j = 0; j < 8; ++j){
      v[i].push_back(0);
    }
  }
  std::cout << "DCAS LOCATION: (3,3)"<<std::endl;
  pthread_t drones[10];
  struct thread_data droneData[9];
  int rc;
  for (int i = 0; i < 9; ++i){
    droneData[i].id = i + 1;
    droneData[i].map = &v;
    droneData[i].motion = &inMotion;
    rc = pthread_create(&drones[i],NULL,startDrone,(void*)&droneData[i]);
    if (rc) {
      std::cout << "UNABLE TO CREATE THREAD";
      exit(-1);
    }
    sleep(5);
  }
  // free attribute and wait for the other threads
   pthread_attr_destroy(&attr);
   for(int i = 0; i < 10; i++ ) {
      rc = pthread_join(drones[i], &status);
      if (rc) {
         std::cout << "Error:unable to join," << rc << std::endl;
         exit(-1);
      }
   }

   std::cout << "All drone missions complete." << std::endl;
  pthread_exit(NULL);

  return 0;
}
