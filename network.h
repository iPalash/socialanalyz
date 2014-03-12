#include "I.h"
#include <map>
#include <unistd.h>
#include <cstdlib>
struct vertex;
typedef map<string,ssnode>::iterator iter;
//int 10000000 = 10000000;
using namespace std;
int global = 0;

/**<
    network.h is the header file having functions to calculate shortest path and importance of a given person
*/
/**<
    Structure to define a vertex storing entrynumber and properties 
*/

struct vertex
{
public:
	string en;
	ssnode value;
	int distance;
    	bool visited;
    	vertex(){}
	vertex(string a, ssnode b,int c,bool f)
	{
		this->en = a;
		this->value = b;
		this->distance = c;
        	this->visited = f;
	}
};

/**<
    CompareDist function comparator function for priority queue
*/
class CompareDist
{
    public:
        bool operator()(vertex& n1, vertex& n2)
        {
           if (n1.distance > n2.distance) 
                return true;
           else
                return false;
        }
};
	
/**<
    function sizeofclique gives the clique of the given person
*/
vector<string> sizeofclique(string a,grph *g,vector<string>* f)
{
    vector<string> ans;
    if(f->size() == 0) {
        return ans;
    }
    else if(f->size() == 1){
        ans.push_back(f->at(0));
        return ans;
    }
    else
    {
       for(int i = 0 ;i <f->size() ; i++)
        {
            string x = f->at(i);
            f->erase(f->begin()+i);
            vector<string> var = sizeofclique(a,g,f);
	        bool t = false;
	        int count = 0;
            for(int j = 0; j<var.size() ; j++)
            {
                string y = var.at(j);
                for(int k = 0; k< g->student.at(y).friends.size() ; k++)
                {
                    if(g->student.at(y).friends.at(k) == x) {
                        count++;
                        break;
                    }
                }
		       if(count == var.size()){
		        var.push_back(x);
		        t = true;
		        }	
                if(ans.size() < var.size())
                {
                    ans = var;
                }
            }
        } 
    } 
    return ans;
}

/**<
    Function lengthsp gives the lenght of the Shortest distance between the two specified person
*/
	int lengthsp(string a,string b,grph *g)
	{
        int size = g->student.size();
        map<string,vertex> data;
        for(iter iterator = g->student.begin(); iterator != g->student.end(); iterator++)
        {
            if(iterator->second.entrynumber == a){
                vertex x = vertex(iterator->first,iterator->second,0,true);
                data[iterator->first] = x;
            }
            else{
                vertex x = vertex(iterator->first,iterator->second,10000000,false);
                data[iterator->first] = x;
            }
        }
        
	   priority_queue< vertex, vector<vertex>, CompareDist> pq;
    	vertex first = vertex(a,g->student.at(a),0,true);
    	pq.push(first);
    	while((pq.top().en != b) && (!pq.empty())) 
    	{

        vertex temp = pq.top();
        pq.pop();
        string node= temp.en;
        size = g->student.at(node).friends.size();
            for(int i=0;i <size;i++ )
            {
                if(data[g->student.at(node).friends.at(i)].distance > data[node].distance + 1){
                    data[g->student.at(node).friends.at(i)].distance = data[node].distance + 1;
                }
                    
                if(!data[g->student.at(node).friends.at(i)].visited)
                {
                    g->student.at(g->student.at(node).friends.at(i)).parent = node;
                    data[g->student.at(node).friends.at(i)].visited = true;
                    vertex newNode = vertex(g->student.at(node).friends.at(i),g->student.at(g->student.at(node).friends.at(i)),data[g->student.at(node).friends.at(i)].distance,true);
                    pq.push(newNode);
                }
            }
        }
        vertex temp = pq.top();
        int ans;
        if(temp.en == b){
            ans = temp.distance;
        } 
        else ans = data[b].distance;
        return ans;
	}

/**<
    Function shortestpath gives the shortest path in the graph
*/
    vector<string> shortestpath(grph *g)
    {
        vector<string> ans;
        ans.push_back("no shortest path");
        for(iter iterator = g->student.begin(); iterator != g->student.end(); iterator++)
        {
           if(iterator->second.friends.size() != 0)
           {
                ans.pop_back();
                ans.push_back(iterator->first);
                ans.push_back(iterator->second.friends.at(0));
                break;
           }
        }
        return ans;
    }
/**<
    Function listsp gives the list of person in the shortest path between two given person
*/

    vector<string> listsp(string a,string b,grph *g){
        int d =lengthsp(a,b,g);
        vector<string> list;
        list.push_back(b);
        string x = b;
        if(d == 10000000)
        {
            list.push_back("no path");
        }
        else if(d == 1)
        {
        }
        else {
            while(g->student.at(x).parent != a)
            {
                list.push_back(g->student.at(x).parent);
                x = g->student.at(x).parent;
            }
        }
        
        list.push_back(a);
        return list;
    }

// struct input
// {
//     string ini;
//     string final;
//     input(string a,string b)
//     {
//         this->ini = a;
//         this->final = b;
//     }
// };

// struct inputCompare
// {
// public:
//     bool operator()(const input &a, const input &b) const
//     {
//         if (a.ini == b.ini) return false;
//         else return true;
//     }
// };
/**<
    Function floyd2 to calculate importance of a given person
*/

int floyd2(string person,grph *g)\
{

    int size = g->student.size();
    int distp[size][size];
    int nextp[size][size];
    int initial = 0;
    int final = 0;
    vector<string> nodes;
    //nodes.push_back("a");
    int c = 0;
    for(iter iterator = g->student.begin(); iterator != g->student.end(); iterator++)
    {
        nodes.push_back(iterator->first);
        //cout << nodes.at(c) << endl;
        c++;
    }
        int count =0;
            for(int i = 0; i<size ; i++)
            {
               for(int j = 0;j <size ; j++)
               {
                
                    if(i == j){
                        nextp[i][j] = size+2;
                        distp[i][j] = 0;
                    } 
                    else
                    {
                        bool check = false;
                        string x = g->student.at(nodes[j]).entrynumber;
                        //cout << "here " << x << endl;
                        for(int w = 0 ; w<g->student.at(nodes[i]).friends.size() ; w++)
                        {
                            if(g->student.at(nodes[i]).friends.at(w) == x)
                            {
                                check = true;
                                distp[i][j] = 1;
                                nextp[i][j] = size+2;
                            }
                        }
                        if(!check)
                        {
                            distp[i][j] = 10000000;
                             nextp[i][j] = size+2;
                        }
                    }
               }
            }
           
            for (int k = 0 ;k <size ; k++)
            {
                for (int i = 0 ;i <size ; i++)
                {
                    for (int j = 0 ;j <size ; j++)
                    {
                        if (distp[i][k] + distp[k][j] < distp[i][j])
                        {
                            distp[i][j] = distp[i][k] + distp[k][j];
                            if( nextp[i][j] < size && nodes[nextp[i][j]]== person){
                                if(count > 0) count -= 1;
                            }
                            nextp[i][j] = k;
                            //cout << nodes[k] << " " << person << endl;
                            if(nodes[nextp[i][j]] == person){
                                 count += 1;
                            }
                            //cout << " in between " << i << " and " << j << " is " << nextp[i][j] << " " << nodes[k]  << " " <<count<< endl;
                        } 
                    }
                        
                }
         
            }
return count;
}


/**<
    Function boolimp gives the boolean if some friend is having more importance than the given person
*/
    string moreimp(string a,grph *g)
    {
        string ans;
        int value = floyd2(a,g);
        int size = g->student.at(a).friends.size();
        for(int i = 0; i<size ;i++)
        {
            int temp = floyd2(g->student.at(a).friends.at(i),g);
            cout << value << " compared to " << temp << endl;
            if(temp > value){
                ans = g->student.at(a).friends.at(i);
                break;
            }
        } 
        cout << ans << endl;
        return ans;
    }


