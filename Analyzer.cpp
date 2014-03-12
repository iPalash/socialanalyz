#include "gml.h"
#include "network.h"
#include <random>
grph g;
typedef map<string,ssnode> :: iterator it;

/**
	Function query calling functions of network.h to answer the query
*/
void query(int q, char* e1, char* e2){
	int variable = rand()%g.student.size();
	vector<string> answer;	
	switch(q){

		case 1: 
			cout << sizeofclique(e1,&g,&g.student.at(e1).friends).size()+1 << endl;
			break;
		case 2: cout << lengthsp(e1,e2,&g) << endl;
			break;
		case 3: answer = listsp(e1,e2,&g);
			for(int k=0 ;k<answer.size() ; k++){
				cout  << answer.at(k) << "to " << endl;	
			}
            		break;
		case 4: answer = shortestpath(&g);
				for(int k=0 ;k<answer.size() ; k++){
				cout  << answer.at(k) << endl;	
			}
			break;
		case 5: cout << floyd2(e1,&g) << endl;
			break;
		case 6: cout << moreimp(e1,&g) <<endl;
			break;
	}
}

int main(int argc,char** argv){
	g=readGraph("Graph.graphml");
	query(atoi(argv[1]),argv[2],argv[3]);
}
