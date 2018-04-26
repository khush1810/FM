//============================================================================
// Name        : partitioning.cpp
// Version     : 1.8
//===========================================================================
#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include<map>
#include<set>
#include<algorithm>
using namespace std;



void printVector(std::vector<string> vect)
{
	for (int i = 0; i < vect.size(); i++)
	{
		cout<<vect[i]<<" ";
	}
	cout<<endl;
}





std::vector<string> remove_duplicate2(std::vector<string> vec) {
  std::set<string> s(vec.begin(), vec.end());
  vec.assign(s.begin(), s.end());
  return vec;
}

bool isneighbour(std::map<string, vector<string> > m, string a, string b)
{
	vector<string> v = m[a];

	for(int i = 0; i < v.size(); i++)
	{
		if(v[i] == b)
		{
			return true;
		}
	}

	return false;
}





class node
{
public:
	std::string id;
	std::vector<std::string> neighbours;
	float area;
	int lock;

	// constructor


	void display()
	{

		 cout<<"ID: "<<id<<endl;

		cout<<"With neighbours : ";
		for (int i =0 ; i < neighbours.size(); i++)
		{
			cout<<neighbours[i]<<" ";
		}
		cout<<endl;
	}

};

float getArea(std::vector<node> g, string id)
{
	for(int i = 0; i< g.size(); i++)
	{
		if(g[i].id == id)
		{
			return g[i].area;
		}
	}

	return 1;
}

float getPartitionArea(std::vector<node> p)
{
	float tot = 0;

	for (int i = 0 ; i < p.size(); i++)
	{

		tot = tot+ p[i].area;

	}
return tot;

}



template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

float getDegrees(node n , std::map<std::string, int> me, std::map<std::string, int >other, std::map<std::string,std::vector<std::string> > adjMap )
{

	int external = 0;
	int internal = 0;

	std::vector<string> neighbours;

	neighbours = adjMap[n.id];

	for(int i = 0; i< neighbours.size(); i++)
	{
		string curr = neighbours[i];

		if(other[curr] == 1)
		{
			external++;
		}


		if(me[curr] == 1)
		{
			internal++;
		}


	}

	int result = external - internal;
	return result ;
}



std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


int main()
{
    std::ifstream file("/home/manix025/Downloads/ibm01/ibm01.net");
    std::string str;

    int count = 0;

    std::vector<node> graph;

    std::map<int, std::string> nodemap;





    //FIRST PASS TO GET THE NODES IN THE GRAPH
    int nCount = 0;
    node curr;

    while (std::getline(file, str))
    {

    //FIRST FIVE LINES ARE UNRELATED TO CIRCUIT DESIGN
        count = count+1;

      if (count<6)
      {
    	  continue;
      }

      // ACTUAL BUSINESS BEGINS
      vector<std::string> lineByDelim;

      lineByDelim = split(str,' ');

      int ls = lineByDelim.size();
      //std::cout<<"Line : "<<count<<" "<< ls <<endl;


      // NODES CREATED EVERY TIME A 3 LENGTH LINE IS ENCOUNTERED
      if(ls == 3)
      {	  nCount = nCount+1;

	  	  graph.push_back(curr);
	  	  curr.neighbours.clear();
      	  curr.id = lineByDelim[0];
    	  //cout<<"The curr's id is "<<curr.id;
    	  nodemap[nCount] = lineByDelim[0] ;
      }

      else if(ls == 2)
      {
    	  //cout<<" I am here "<<curr.id<<endl

    	  curr.neighbours.push_back(lineByDelim[0]);

      }


    }


    std::map<std::string, float> areaMap;

 //Read Areas
    string st;
    float no;
    std::ifstream infile("/home/manix025/Downloads/ibm01/ibm01.are");
    while (infile >> st >> no)
    {
        areaMap[st] = no;
    }


 for(int i = 0; i< graph.size(); i++)
 {
	 graph[i].area = areaMap[graph[i].id];
 }







std::map<string,vector<string> > nMap;

for(int i = 0; i < graph.size(); i++)
	{
		vector<string> temp;

		nMap[graph[i].id] = temp;
	}



//EFFICIENT WAY TO STORE THE SPARSE ADJACENCY MATRIX IS THROUGH A HASH MAP WHICH BEGINS HERE.

for(int i = 0; i < graph.size(); i++)

{
		//graph[i] is the current object that is being iterated over.

		for(int j = 0; j < graph[i].neighbours.size(); j++)
		{
			//graph[i].neighbours[j] will return a node object which is the neighbour of graph[i]. Now a two way referencing must take place. Simply go into the vector and push back to each of the keys. we will deal with duplicate edges later.

				nMap[graph[i].id].push_back(graph[i].neighbours[j]);

				nMap[graph[i].neighbours[j]].push_back(graph[i].id); // Two way referencing fulfilled
		}

}




/*
cout<<"Before removal of Duplicates: ";
printVector(nMap["a8418"]);
*/



//REMOVES DUPLICATES

std::map<std::string, std::vector<string> >::iterator it;
std::vector<std::string> nodes;

for (it = nMap.begin(); it != nMap.end(); it++)
{
	string key = it->first;
/*	cout<<key<<endl;*/
	std::vector<string> val = it->second;

	val = remove_duplicate2(val);

	nMap[key] = val;
	nodes.push_back(key);

}



/* trial
cout<<"After removal of duplicates: ";
printVector(nMap["a8418"]);
*/

/*
cout<<graph[14].id<<" "<<graph[14].area<<endl;
cout<<"This map is nMap: "<<nMap.size()<<" chars long"<<endl;
*/


std::map<std::string, bool> allo;

for(int i = 0; i< graph.size(); i++)
{
	allo[graph[i].id] = false;

}


int osci = -1;

int c1  = 0;
int c2 = 0;


std::vector<node> cGraph;

for(int i = 0; i < nodes.size(); i++)
{
	node n1;

	n1.id = nodes[i];
	n1.neighbours = nMap[nodes[i]];
	n1.area = areaMap[nodes[i]];

	cGraph.push_back(n1);
}


std::map<std::string,int> pa1;
std::map<std::string,int> pa2;
osci = -1;

for(int i = 0; i < cGraph.size(); i++)
{
	if (osci == -1)
	{
		pa1[cGraph[i].id] =1;
	}

	else
	{
		pa2[cGraph[i].id] = 1;
	}

	osci = osci*-1;
}
std::map<std::string,float> gainMap;

std::map<float,vector<std::string> > gains;



for(int i = 0; i < cGraph.size(); i++)
{
	cGraph[i].lock = 0; // setting everything to unlock.
	if(i == cGraph.size()/4)
	{
		cout<<"Quarter way there"<<"\n";
	}

	if(i == cGraph.size()/2)
	{
		cout<<"Half way there "<<"\n";

	}

	if(pa1[cGraph[i].id] == 1)
	{
		float ans = getDegrees(cGraph[i],pa1,pa2,nMap);

		gainMap[cGraph[i].id] = ans;

		gains[ans].push_back(cGraph[i].id);
	}

	else if(pa2[cGraph[i].id] == 1)
	{
		float ans = getDegrees(cGraph[i], pa2, pa1, nMap);
		gainMap[cGraph[i].id] = ans;

		gains[ans].push_back(cGraph[i].id);
	}




}

cout<<gains.size();


for(int i = 0; i < cGraph.size()/2 ; i++)
{
graph[i].display();
}
}
