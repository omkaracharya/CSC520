/*
Name: Omkar Pradeep Acharya
Student ID: 200155609
Course: CSC520 - Artificial Intelligence
Assignment 2 - Q.1

Problem Statement: Implement the Uniform Cost Search, Greedy Best First Search and A* algorithms.

How to run:
$ g++ SearchUSA.cpp -o SearchUSA

$ ./SearchUSA searchtype srccityname destcityname

searchtype - uniform or greedy or astar
cityname - lowercase letters
*/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <list>
#include <math.h>
using namespace std;

//Data structure for storing all the neighbors of a city in the map.
struct Neighbor_City{
	string name;	//city name
	int distance;	//distance from a city
};

class SearchUSA;

//Data structure representing a city and its details
class Node{
	friend class SearchUSA;	//Friend class SearchUSA can access the private data members of class Node
	string name;
	string nodes_expanded;
	double latitude,longitude;
	int nodes_cost;
	double heuristic_value;
	double total_cost;
	bool visited;
	vector<Neighbor_City>neighbors;		//collection of all the neighbors of the instance of this class
	vector<Neighbor_City>::iterator neighbor_iterator;
public:
	Node(){//default constructor
		this->name = "";
		this->latitude = 0;
		this->longitude = 0;
		this->nodes_expanded = "";
		this->nodes_cost = 0;
		this->total_cost = 0;
		this->visited = false;
		this->neighbor_iterator = neighbors.begin();
	}
	Node(string name, double latitude, double longitude){//parameterized constructor
		this->name = name;
		this->latitude = latitude;
		this->longitude = longitude;
		this->nodes_expanded = name;
		this->nodes_cost = 0;
		this->total_cost = 0;
		this->visited = false;
		this->neighbor_iterator = neighbors.begin();
	}
	void add_neighbor_city(string, int);
};

class SearchUSA{//class containing search methods and USA map
	string source_city;	//source city name
	string destination_city;	//destination city name
	Node destination;	//destination city node
	vector<Node>cities;	//list of cities
	vector<Node>frontier;	//cureent frontier
public:
	SearchUSA(string src, string dest){
		source_city = src;
		destination_city = dest;
	}
	bool create_usa_map();
	void find_heuristic_value();
	void get_node(string,Node*);
	bool get_next_node(Neighbor_City,Node*);
	Node shortest(double*,string);
	void search(string);
	void print_cities(vector<Node>&);
	void greedy();
	void mark_visited(string);
	int total_visited();
	void nodes_visited();
	int get_cost(string);
};

//returns the cost of all the nodes
int SearchUSA::get_cost(string s){
	vector<Node>::iterator it;
	for(it = frontier.begin(); it != frontier.end();it++){
		if((*it).name == s){
			return (*it).nodes_cost;
		}
	}
	return 0;
}

//prints all the cities in the map with their g() and h() values
void SearchUSA::print_cities(vector<Node>&cities){
	for(vector<Node>::iterator i = cities.begin(); i < cities.end(); i++){
		cout<<(*i).name<<":"<<(*i).heuristic_value<<"+"<<(*i).nodes_cost<<"="<<(*i).heuristic_value+(*i).nodes_cost<<'\n';
	}
	cout<<'\n'<<cities.size()<<'\n';
}

//marks the expanded node as visited
void SearchUSA::mark_visited(string s){
	for(vector<Node>::iterator i = cities.begin(); i < cities.end(); i++){
		if(s == (*i).name){
			(*i).visited = true;
		}
	}	
}

//counts the total number of visited nodes
int SearchUSA::total_visited(){
	int count = 0;
	for(vector<Node>::iterator i = cities.begin(); i < cities.end(); i++){
		if((*i).visited)
			count++;
	}
	return count;	
}

//prints which nodes are visited
void SearchUSA::nodes_visited(){
	for(vector<Node>::iterator i = cities.begin(); i < cities.end(); i++){
		if((*i).visited)
			cout<<(*i).name<<',';
	}
}

//gets the node for a particular city
void SearchUSA::get_node(string name, Node *node){
	vector<Node>::iterator i;
	for(i = cities.begin(); i < cities.end(); i++){
		if(name == (*i).name){
			*node = (*i);
			break;
		}
	}
}

//gets the neighbor node for a particular city
bool SearchUSA::get_next_node(Neighbor_City neighbor, Node *child){
	for(vector<Node>::iterator i = cities.begin(); i < cities.end(); i++){
		*child = *i;
		if(i->name == neighbor.name){
			return true;
		}
	}
	return false;
}

//calculates the heuristic value for each city
void SearchUSA::find_heuristic_value(){
	double pi = 3.14159265358979323846;
	get_node(destination_city,&destination);
	for(vector<Node>::iterator i = cities.begin(); i < cities.end(); i++){
		(*i).heuristic_value = sqrt(pow(69.5*((*i).latitude - destination.latitude),2) + pow((69.5 * cos(((*i).latitude + destination.latitude)/360 * pi) * ((*i).longitude - destination.longitude)),2));
	}
}

//adds the neighbor cities to the data structure of a particular city
void Node::add_neighbor_city(string name, int distance){
	Neighbor_City new_neighbor;
	new_neighbor.name = name;
	new_neighbor.distance = distance;
	neighbors.push_back(new_neighbor);
	neighbor_iterator = neighbors.begin();
}

//creates a data structure for the given map;
bool SearchUSA::create_usa_map(){
	ifstream file("cities.txt");
	string name, latitude, longitude;
	while(getline(file,name,',')){
		getline(file,latitude,',');
		getline(file,longitude);
		Node new_city(name,atof(latitude.c_str()),atof(longitude.c_str()));
		new_city.neighbors.clear();
		cities.push_back(new_city);
	}
	find_heuristic_value();

	ifstream file2("neighbors.txt");
	string source, destination, distance;
	vector<Node>::iterator i;
	while(getline(file2,source,',')){
		getline(file2,destination,',');
		getline(file2,distance);
		for(i = cities.begin() ;i<cities.end(); i++){
			if((*i).name == source){		//edge from source to destination
				(*i).add_neighbor_city(destination,atoi(distance.c_str()));
			}
			if((*i).name == destination){	//edge from destination to source
				(*i).add_neighbor_city(source,atoi(distance.c_str()));	
			}
		}
	}	
	Node s,d;
	get_node(source_city,&s);
	get_node(destination_city,&d);
	if(s.name == "" || d.name == ""){
		return false;
	}
	return true;
}

//finds the shortest node on the current frontier
Node SearchUSA::shortest(double *dist,string algorithm){
	vector<Node>::iterator frontier_iterator = frontier.begin();
	vector<Node>::iterator shortest_so_far_iterator = frontier.begin();
	Node city = *frontier_iterator;
	if(algorithm == "astar")
		*dist = city.nodes_cost + city.heuristic_value;
	else if(algorithm == "uniform")
		*dist = city.nodes_cost;
	else if(algorithm == "greedy")
		*dist = city.heuristic_value;

	shortest_so_far_iterator = frontier_iterator;
	frontier_iterator++;
	while(frontier_iterator < frontier.end()){
		city = *frontier_iterator;
		if(*dist > city.nodes_cost + city.heuristic_value && algorithm == "astar"){
			*dist = city.nodes_cost + city.heuristic_value;
			shortest_so_far_iterator = frontier_iterator;
		}else if(*dist > city.nodes_cost && algorithm == "uniform"){
			*dist = city.nodes_cost;
			shortest_so_far_iterator = frontier_iterator;
		}else if(*dist > city.heuristic_value && algorithm == "greedy"){
			*dist = city.heuristic_value;
			shortest_so_far_iterator = frontier_iterator;
		}
		frontier_iterator++;
	}
	city = *shortest_so_far_iterator;
	frontier.erase(shortest_so_far_iterator);
	mark_visited(city.name);
	return city;
}

//search algorithm function
void SearchUSA::search(string algorithm){
	Node start,current,child;
	Neighbor_City current_neighbor;
	double best_possible=0, current_distance_from_goal=0;
	bool found = false;
	string best_possible_path;
	int flag = 0;

	get_node(source_city,&start);
	frontier.push_back(start);
	mark_visited(start.name);
	while(!frontier.empty() && (best_possible < current_distance_from_goal || !found)){
		//find the best possible node according to f() from the frontier
		current = shortest(&best_possible,algorithm);
		//iterate over all of its neighbors
		for(current.neighbor_iterator = current.neighbors.begin(); current.neighbor_iterator < current.neighbors.end(); current.neighbor_iterator++){
			current_neighbor = *(current.neighbor_iterator);
			//terminates when there is no node to expand
			if(!get_next_node(current_neighbor,&child))
				return;
			//take it out if the node is in the closed list and the new f( ) is better than previous
			if(child.visited && get_cost(child.name) > current.nodes_cost + current_neighbor.distance){
				child.visited = false;
			}else if(child.visited && get_cost(child.name) <= current.nodes_cost + current_neighbor.distance){
				continue;
			}

			//update the cost for the expanded node
			vector<Node>::iterator it;
			for(it = frontier.begin(); it != frontier.end();it++){
				if((*it).name == child.name){
					if((*it).nodes_cost > current.nodes_cost + current_neighbor.distance){
						frontier.erase(it);	
						flag = 1;
					}
					break;
				}
			}

			//store the path uptil now and check whether the last node is destination node or not
			child.nodes_expanded = current.nodes_expanded + "->" + child.name;
			child.nodes_cost = current.nodes_cost + current_neighbor.distance;

			//if we find the goal, still we need to check that the path we got is the shortest one
			if(child.name == destination_city){
				current_distance_from_goal = child.nodes_cost;
				best_possible_path = child.nodes_expanded;
				found = true;
				mark_visited(child.name);
			}

			if(it == frontier.end() || flag ){
				frontier.push_back(child);
				flag = 0;
			}
		}
	}	
	cout<<"\nExpanded nodes: {";nodes_visited();cout<<"\b}"<<endl<<endl
		<<"Number of nodes expanded: "<<total_visited()<<endl<<endl
		<<"Solution Path: "<<best_possible_path<<endl<<endl
		<<"Path length from "<<source_city<<" to "<<destination_city<<": "<<current_distance_from_goal<<endl<<endl;
}

void SearchUSA::greedy(){
	Node start,current,child;
	bool found = false;
	string path;
	double distance=0;
	Neighbor_City current_neighbor;

	get_node(source_city,&start);
	frontier.push_back(start);
	mark_visited(start.name);
	while(!found && !frontier.empty()){
		//find the best possible node according to f() from the frontier
		current = shortest(&distance,"greedy");

		//if the frontier is cleared, then the algorithm doesn't use backtracking. This will give the incomplete algorithm
//		frontier.clear();

		//iterate over all of its neighbors
		for(current.neighbor_iterator = current.neighbors.begin(); current.neighbor_iterator < current.neighbors.end(); current.neighbor_iterator++){
			current_neighbor = *(current.neighbor_iterator);
			//no possible expansion for the leaf node
			if(!get_next_node(current_neighbor,&child))
				return;

			//store the path uptil now and update the cost for the node
			child.nodes_expanded = current.nodes_expanded + "->" + child.name;
			child.nodes_cost = current.nodes_cost + current_neighbor.distance;
			if(child.name == destination_city){
				distance = child.nodes_cost;
				path = child.nodes_expanded;
				found = true;
				mark_visited(child.name);
			}
			if(!child.visited){
				frontier.push_back(child);
			}
		}
	}
	cout<<"\nExpanded nodes: {";nodes_visited();cout<<"\b}"<<endl<<endl
		<<"Number of nodes expanded: "<<total_visited()<<endl<<endl;
	if(path != ""){
		cout<<"Solution Path: "<<path<<endl<<endl 
		<<"Path length from "<<source_city<<" to "<<destination_city<<": "<<distance<<endl<<endl;
	}else{
		cout<<"Solution path not found"<<endl;
	}
}

int main(int argc, char *argv[]){
	//The second command line argument is the name of algorithm.
    string algorithm = argv[1];
    if(argc<4){
        cout<<"Invalid command line arguments."<<'\n';
        return 0;
    }
    //Create the given USA map.
    SearchUSA usamap(argv[2],argv[3]);
    bool success = usamap.create_usa_map();
    if(!success){
    	cout<<"Incorrect city names."<<endl;
    	return 0;
    }
    if(algorithm == "uniform" || algorithm == "astar")
		usamap.search(algorithm);
	else if(algorithm == "greedy")
		usamap.greedy();
	else
		cout<<"Incorrect algorithm type specified"<<endl;
	return 0;
}