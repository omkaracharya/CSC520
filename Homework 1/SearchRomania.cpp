/*
Name: Omkar Pradeep Acharya
Student ID: 200155609
Course: CSC520 - Artificial Intelligence
Assignment 1 - Q.5A

Problem Statement: Implement the Depth-First Search and Breadth-First Search algorithms.

How to run:
./a.out searchtype srccityname destcityname

searchtype - BFS or DFS
cityname - lowercase letters
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

//Class representing the Romania Map
class Romania{
    int no_of_nodes;
    vector<pair<string,int> >numbers;
    list<int> *neighbors;
public:
    Romania();
    void create_romania_map();
    void source_to_destinations(string,string);
    void breadth_first_search(string, string, int, int);
    void depth_first_search_helper(string,string, int, int);
    void depth_first_search(int,int,bool[],bool&,int&);
    void assign_city_number();
    int get_city_number(string);
    string get_city_name(int);
};

//Constructor
Romania::Romania(){
    no_of_nodes = 20;

/*  It creates an adjacency list of 20 nodes as follows:

     ___    ___    ___    ___
    |   |  |   |  |   |  |   |
    | 0 |->| 15|->| 16|->| 19|
     ---    ---    ---    ---
      |
      V
     ___    ___    ___    ___
    |   |  |   |  |   |  |   |
    | 1 |->| 6 |->| 13|->| 17|
     ---    ---    ---    ---
      |
      V
      .
      .
      .
      |
      V
     ___    ___    ___
    |   |  |   |  |   |
    | 19|->| 0 |->| 12|
     ---    ---    ---

*/
    neighbors = new list<int>[no_of_nodes];
    assign_city_number();
    create_romania_map();
}

void Romania::create_romania_map(){
    //Nodes from A->B
    neighbors[get_city_number("oradea")].push_back(get_city_number("zerind"));
    neighbors[get_city_number("zerind")].push_back(get_city_number("arad"));
    neighbors[get_city_number("arad")].push_back(get_city_number("timisoara"));
    neighbors[get_city_number("timisoara")].push_back(get_city_number("lugoj"));
    neighbors[get_city_number("lugoj")].push_back(get_city_number("mehadia"));
    neighbors[get_city_number("dobreta")].push_back(get_city_number("mehadia"));
    neighbors[get_city_number("oradea")].push_back(get_city_number("sibiu"));
    neighbors[get_city_number("arad")].push_back(get_city_number("sibiu"));
    neighbors[get_city_number("dobreta")].push_back(get_city_number("craiova"));
    neighbors[get_city_number("sibiu")].push_back(get_city_number("rimnicu_vilcea"));
    neighbors[get_city_number("sibiu")].push_back(get_city_number("fagaras"));
    neighbors[get_city_number("rimnicu_vilcea")].push_back(get_city_number("craiova"));
    neighbors[get_city_number("pitesti")].push_back(get_city_number("craiova"));
    neighbors[get_city_number("rimnicu_vilcea")].push_back(get_city_number("pitesti"));
    neighbors[get_city_number("bucharest")].push_back(get_city_number("pitesti"));
    neighbors[get_city_number("bucharest")].push_back(get_city_number("fagaras"));
    neighbors[get_city_number("bucharest")].push_back(get_city_number("giurgiu"));
    neighbors[get_city_number("bucharest")].push_back(get_city_number("urziceni"));
    neighbors[get_city_number("vaslui")].push_back(get_city_number("urziceni"));
    neighbors[get_city_number("hirsova")].push_back(get_city_number("urziceni"));
    neighbors[get_city_number("hirsova")].push_back(get_city_number("eforie"));
    neighbors[get_city_number("vaslui")].push_back(get_city_number("iasi"));
    neighbors[get_city_number("neamt")].push_back(get_city_number("iasi"));

    //Nodes from B->A
    neighbors[get_city_number("zerind")].push_back(get_city_number("oradea"));
    neighbors[get_city_number("arad")].push_back(get_city_number("zerind"));
    neighbors[get_city_number("timisoara")].push_back(get_city_number("arad"));
    neighbors[get_city_number("lugoj")].push_back(get_city_number("timisoara"));
    neighbors[get_city_number("mehadia")].push_back(get_city_number("lugoj"));
    neighbors[get_city_number("mehadia")].push_back(get_city_number("dobreta"));
    neighbors[get_city_number("sibiu")].push_back(get_city_number("oradea"));
    neighbors[get_city_number("sibiu")].push_back(get_city_number("arad"));
    neighbors[get_city_number("craiova")].push_back(get_city_number("dobreta"));
    neighbors[get_city_number("rimnicu_vilcea")].push_back(get_city_number("sibiu"));
    neighbors[get_city_number("fagaras")].push_back(get_city_number("sibiu"));
    neighbors[get_city_number("craiova")].push_back(get_city_number("rimnicu_vilcea"));
    neighbors[get_city_number("craiova")].push_back(get_city_number("pitesti"));
    neighbors[get_city_number("pitesti")].push_back(get_city_number("rimnicu_vilcea"));
    neighbors[get_city_number("pitesti")].push_back(get_city_number("bucharest"));
    neighbors[get_city_number("fagaras")].push_back(get_city_number("bucharest"));
    neighbors[get_city_number("giurgiu")].push_back(get_city_number("bucharest"));
    neighbors[get_city_number("urziceni")].push_back(get_city_number("bucharest"));
    neighbors[get_city_number("urziceni")].push_back(get_city_number("vaslui"));
    neighbors[get_city_number("urziceni")].push_back(get_city_number("hirsova"));
    neighbors[get_city_number("eforie")].push_back(get_city_number("hirsova"));
    neighbors[get_city_number("iasi")].push_back(get_city_number("vaslui"));
    neighbors[get_city_number("iasi")].push_back(get_city_number("neamt"));

    for(int i=0;i<no_of_nodes;i++)
        neighbors[i].sort();
}

//Assigns city numbers alphabetically
/*
0 - arad,1 - bucharest,2 - craiova,3 - dobreta,4 - eforie,5 - fagaras,6 - giurgiu,7 - hirsova,8 - iasi,
9 - lugoj,10 - mehadia,11 - neamt,12 - oradea,13 - pitesti,14 - rimnicu_vilcea,15 - sibiu,16 - timisoara,
17 - urziceni,18 - vaslui,19 - zerind
*/
void Romania::assign_city_number(){
    string cities[] = {"arad","bucharest","craiova","dobreta","eforie","fagaras","giurgiu","hirsova","iasi","lugoj","mehadia","neamt","oradea","pitesti","rimnicu_vilcea","sibiu","timisoara","urziceni","vaslui","zerind"};
    for(int i=0;i<no_of_nodes;i++)
        numbers.push_back(make_pair(cities[i],i));
}

//Returns city number for a city name
int Romania::get_city_number(string name){
    for(int i=0;i<no_of_nodes;i++)
        if(numbers[i].first == name)
            return i;
    return -1;
}

//Returns city name for a city number
string Romania::get_city_name(int n){
    for(int i=0;i<no_of_nodes;i++)
        if(i == n)
            return numbers[i].first;
    return 0;
}

//BFS
void Romania::breadth_first_search(string start_city, string end_city, int s, int d){
    int total=0;
    list<int>q;
    bool visited_nodes[no_of_nodes];
    for(int i=0;i<no_of_nodes;i++)
        visited_nodes[i] = false;
    visited_nodes[s] = true;
    q.push_back(s);
    list<int>::iterator n;
    while(q.empty() == false){
        s = q.front();
        total++;
        if(s == d)
            break;
        cout<<get_city_name(s)<<"->";
        q.pop_front();
        for(n=neighbors[s].begin();n!=neighbors[s].end();n++){
            if(visited_nodes[*n] == false){
                visited_nodes[*n] = true;
                q.push_back(*n);
            }
        }
    }
    cout<<get_city_name(d)<<" ";
    cout<<"\nTotal number of nodes expanded:"<<total;
}

//DFS
void Romania::depth_first_search(int v, int d, bool visited[],bool& found,int& total){
    visited[v] = true;
    if(v==d) found = true;
    if(found)
        return;
    cout<<get_city_name(v)<<"->";
    total++;
    list<int>::iterator n;
    for(n=neighbors[v].begin();n!=neighbors[v].end();n++)
        if(visited[*n] == false)
            //Recursive Call
            depth_first_search(*n,d,visited,found,total);
}

//DFS Helper function
void Romania::depth_first_search_helper(string start_city, string end_city, int s, int d){
    int total = 0;
    bool *visited = new bool[no_of_nodes];
    for(int i=0; i<no_of_nodes;i++)
        visited[i] = false;
    bool found=false;
    depth_first_search(s,d,visited,found,total);
    if(!found) cout<<"Not Found";
    else cout<<get_city_name(d);
    cout<<"\nTotal number of nodes expanded:"<<total+1;
}

//Main function
int main(int argc, char *argv[]){
    if(argc<4){
        cout<<"Invalid command line arguments."<<'\n';
        return 0;
    }
    Romania graph1;
    string start_city = argv[2];
    string end_city = argv[3];
    int s = graph1.get_city_number(start_city);
    int d = graph1.get_city_number(end_city);
    if(s==-1||d==-1){
        if(s==-1&&d==-1)
            cout<<"Invalid start city and end city names";
        else if(s==-1)
            cout<<"Invalid start city";
        else if(d==-1)
            cout<<"Invalid end city";
        cout<<endl;
        return 0;
    }
    if((string)argv[1]=="BFS")
        graph1.breadth_first_search(start_city,end_city,s,d);
    else
        graph1.depth_first_search_helper(start_city,end_city,s,d);
    cout<<endl;
    return 0;
}