#include </home/aketcham0691/Documents/CSE321/Lab 1/Drone/drone.h>
using namespace std;
std::mutex locks[8][8];
std::mutex maplock;
std::mutex setlock;
void printMap(std::vector<std::vector<int> > m){
  for (int i = 0; i < m.size(); i++){
    std::cout << " " << std::endl;
    for (int j = 0; j <m.size(); j++){
      std::cout << m[i][j] << "  ";
    }
  }
  std::cout << std::endl;
  std::cout << std::endl;
}

void printSet(set<int> s){
  cout << "[";
  int count = 1;
  for (int i : s){
    if (count == s.size()){
      cout << i;
    }
    else{
      cout << i << ",";
    }
    count++;

  }
  cout<<"]"<<std::endl;
}

std::vector<std::vector<int> > drone::getMap(){
  return *(this->map);
}

void drone::spawn(int x, int y){
    locks[x][y].lock();
    this->pos[0] = x;
    this->pos[1] = y;
    this->map[0][x][y] = this->id;
}

void drone::setPos(int x, int y){
  int a = this->pos[0];
  int b = this->pos[1];
  this->map[0][a][b] = 0;
  locks[a][b].unlock();
  setlock.lock();
  this->motion[0].insert(this->id);
  setlock.unlock();
  locks[x][y].lock();
  setlock.lock();
  this->motion[0].erase(this->id);
  setlock.unlock();
  this->map[0][x][y] = this->id;
    this->pos[0] = x;
    this->pos[1] = y;
}

void drone::setGoal(int x, int y){
  this->goal[0] = x;
  this->goal[1] = y;
}

void drone::setHome(int x, int y){
  this->homeloc[0] = x;
  this->homeloc[1] = y;
}

std::vector<std::vector<int> > drone::applicable(){
  std::vector<std::vector<int> > retVal;
  int xPos = this->pos[0];
  int yPos = this->pos[1];
  std::vector<int> pos;
  for (int i = -2; i < 3; i += 1){
    if (i == 0){
      continue;
    }
    pos.clear();
    pos.push_back(xPos + i);
    pos.push_back(yPos);
    if (pos[0] >= 0 && pos[0] <= 7 && pos[1] >= 0 && pos[1] <= 7){
      retVal.push_back(pos);
    }
    pos.clear();
    pos.push_back(xPos);
    pos.push_back(yPos + i);
    if (pos[0] >= 0 && pos[0] <= 7 && pos[1] >= 0 && pos[1] <= 7){
      retVal.push_back(pos);
    }
  }
  return retVal;
}

int drone::goalDistance(int x, int y){
  return abs(x - this->goal[0]) + abs(y - this->goal[1]);
}

int drone::homeDistance(int x, int y){
  return abs(x - this->homeloc[0]) + abs(y - this->homeloc[1]);
}

void drone::move(){
  std::vector<std::vector<int> > applic = this->applicable();
  int d;
  while(applic.size() != 0){
    int r = rand() % applic.size();
    int check;
    if (this->complete == false){
      check = this->goalDistance(applic[r][0], applic[r][1]);
      d = this->goalDistance(this->pos[0], this->pos[1]);
    }
    else{
      check = this->homeDistance(applic[r][0], applic[r][1]);
      d = this->homeDistance(this->pos[0], this->pos[1]);
      if (check == 0){
        this->setPos(3,3);
        maplock.lock();
        setlock.lock();
        cout << "DRONE " << this->id << " MISSION COMPLETE" << endl;
        int x = 3;
        int y = 3;
        while (x == 3 && y == 3){
          x = rand()%8;
          y = rand()%8;
        }
        this->setGoal(x,y);
        cout << "DRONE " << this->id << " NEW TARGET: (" << x << "," << y <<")" << std::endl;
        if (!(*(this->motion)).empty()){
          cout<<"OTHER DRONES IN MOTION/WAITING ON FREE LOCATION: ";
          printSet(*(this->motion));
        }
        printMap(*(this->map));
        setlock.unlock();
        maplock.unlock();
        this->complete = false;
        return;
      }
    }
    if (check < d){

      this->setPos(applic[r][0],applic[r][1]);
      if (check == 0){
        maplock.lock();
        cout << "DRONE " << this->id << " TARGET ACQUIRED" << endl;
        maplock.unlock();
        this->complete = true;
      }
      break;
    }
  }
}
