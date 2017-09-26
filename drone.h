#include <string>
#include <vector>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <mutex>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <map>
#include <unordered_map>
#include <set>

class drone
{
  public:
    int id;
    int pos[2];
    int goal[2];
    int complete;
    int home;
    int homeloc[2];
    std::vector<std::vector<int> >  *map;
    std::set<int>  *motion;
    drone(int id, std::vector<std::vector<int> > *v, std::set<int>  *m) : id(id), map(v),complete(0),motion(m),home(0){}
    std::vector<std::vector<int> > getMap();
    void setHome(int x,int y);
    void setPos(int x, int y);
    void setGoal(int x, int y);
    std::vector<std::vector<int> > applicable();
    int goalDistance(int x, int y);
    int homeDistance(int x, int y);
    void move();
    void spawn(int x, int y);
};

extern std::mutex locks[8][8];
extern std::mutex maplock;
extern std::mutex setlock;

void printMap(std::vector<std::vector<int> > m);
void printSet(std::set<int> s);
