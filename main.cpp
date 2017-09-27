
/* 
 * File:   main.cpp
 * Author: azori
 *
 * Created on 4 сентября 2017 г., 17:43
 */

#include <cstdlib>
#include <array>
#include <stdio.h>
#include <random>
#include "Unit.h"
#include "Generators.h"
#include <queue>
#include <iostream>
#include <time.h>

using namespace std;

#define true false
#define f first
#define s second
#define mp make_pair

//Environment
int map[9][9];                                          //Main map
int dist[9][9];                                         //Shortest path to the cell
int b[9][9];                                            //Amount of berries

Coordinates from[9][9];                         

//Agents
Unit units[5];
redHood player;

//Creating environment and agents
void createVision(int x, int y, int id)
{
    if (id == 1)
    {
        int start_i = x-1, end_i = x+2, start_j = y-1, end_j = y+2;
        if (x-1 < 0) start_i = x;
        if (x+2 > 9) end_i = x+1;
        if (y-1 < 0) start_j = y;
        if (y+2 > 9) end_j = y+1;
        
        for (int i = start_i; i < end_i; i++)
            for (int j = start_j; j < end_j; j++) map[i][j] = id;
    } else
    if (id == 2)
    {
        int start_i = x-1, end_i = x+2, start_j = y-1, end_j = y+2;
        if (x-1 < 0) start_i = x;
        if (x+2 > 9) end_i = x+1;
        if (y-1 < 0) start_j = y;
        if (y+2 > 9) end_j = y+1;
        for (int i = start_i; i < end_i; i++) map[i][y] = id;
        for (int i = start_j; i < end_j; i++) map[x][i] = id;
    }
}

bool free(Coordinates a)
{
    if (map[a.x][a.y] != 0) return false;
    return true;
}

void generateUnits()
{
    string names[5] = {"Bear", "Wolf", "Grandma", "Limberjack_1", "Limberjack_2"};
    int i = 0;
    while (i < 5)
    {
        Coordinates a;
        a = genCoordinates();
        if (free(a)) 
        {
            Unit temp(names[i], a.x, a.y);
            units[i] = temp;
            map[a.x][a.y] = ++i;
            createVision(a.x, a.y, i);
        }
    }
    random_device generator;
    uniform_int_distribution<int> distribution(0, 1);
    int check = distribution(generator);
    if (check) units[3].active = true; else units[4].active = true;
}

//Checking if direction is valid
bool valid(int x, int y)
{
    if (x >= 0 && x < 9 && y >= 0 && y < 9) return true;
    return false;
}

//Marker of success
bool done;

//Setters
void setBer(int& x, int& y, int ber, int dis)
{
    b[x][y] = ber;
    dist[x][y] = dis;
}

//Main code with movements
bool go(int x, int y)
{
    if (done) return true;
    
    //List which contains unit in a pair of cell and it's coordinate
    vector <pair< int,pair<int, int> > > next;
    
    //Find the correct directions
    if (valid(x, y+1)) next.push_back(mp(map[x][y+1], mp(x, y+1)));
    if (valid(x+1, y)) next.push_back(mp(map[x+1][y], mp(x+1, y)));
    if (valid(x, y-1)) next.push_back(mp(map[x][y-1], mp(x, y-1)));
    if (valid(x-1, y)) next.push_back(mp(map[x-1][y], mp(x-1, y)));
    
    //Checking whether should we go there or not. If all are bad, we go back
    for (int i = 0; i<next.size(); i++)
    {
        int _x = next[i].s.f;
        int _y = next[i].s.s;
        if (next[i].f == 0 && b[_x][_y] < b[x][y])
        {
            setBer(_x, _y, b[x][y], dist[x][y]+1);
            go(_x, _y);
        } 
        else if(next[i].f == 1 && b[_x][_y] < b[x][y] - 2)
        {
            setBer(_x, _y, b[x][y] - 2, dist[x][y]+1);
            go(_x, _y);
        }
        else if(next[i].f == 4 && b[_x][_y] < 6 && units[3].active)
        {
            setBer(_x, _y, 6, dist[x][y]+1);
            go(_x, _y);
        }
        else if(next[i].f == 5 && b[_x][_y] < 6&& units[4].active )
        {
            setBer(_x, _y, 6, dist[x][y]+1);
            go(_x, _y);
        }
        else if(next[i].f == 3 && b[x][y] == 6)
        {
            setBer(_x, _y, b[x][y], dist[x][y]+1);
            done = true;
        }
        
    }
    return false;
}

int main(int argc, char** argv) {
    float sumtime, succesrate, sumSteps;
    
    //Running code 100 times
    for (int qwe = 0; qwe < 100; qwe++)
    {
        const clock_t begin_time = clock();
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                map[i][j] = 0;
                dist[i][j] = 0;
                b[i][j] = 0;
            }
        }
        done = false;
        srand(time(0));
        generateUnits();
        b[0][0] = 6;
        go(0, 0);
            
        
        if (dist[units[2].x][units[2].y] != 0 && b[units[2].x][units[2].y] == 6)
        {
            succesrate ++;
            sumSteps += dist[units[2].x][units[2].y];
        }
        
        sumtime += float(clock() - begin_time)/CLOCKS_PER_SEC;
    }
    
    //Print averages
    cout << "Success rate: " << succesrate << "%\n";
    cout << "Average time: " << sumtime*10 << " msec\n";
    cout << "Average steps: " << float(sumSteps)/100 << "\n";
    return 0;
}

