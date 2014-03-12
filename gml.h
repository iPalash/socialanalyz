#ifndef GML_H
#define GML_H
/**<
	Gml.h is a header file to create graph after reading from graphml file and save the nodes and content in graph
*/
#include "I.h"
#include <fstream>
#include <cstring>

/**<
	Class Graphm creates the graph using map 
*/
class Graphm{
	public:
		map<Student*,int> studId;
		map<Faculty*,int> facId;
		map<string,Student*> studd;
		map<int,vector<int> > edges;
		void insert(map<int,University*> Univs){

			typedef map<string,Student*>::iterator iter_stud;
			typedef map<Student*,int>::iterator iter3;
			typedef map<int,University*>::iterator iter1;
			typedef map<int,Student*>::iterator iter2;
			int j=0;
			for(iter1 i=Univs.begin();i!=Univs.end();i++){
				University* uuu=i->second;
                for(int xx=0;xx<uuu->students.size();xx++){
                	studId[uuu->students[xx]]=j;
                	studd[uuu->students[xx]->entrynumber]=uuu->students[xx];
                	j++;
                }
			}
			
			
			for(iter3 i=studId.begin();i!=studId.end();i++){
				vector<int> cc;
				cc.clear();
				
					//cout<<i->second->info->friends.size()<<" "<<i->second->info->name<<endl;
				int jj=0;
				int x=0;
				while(jj<i->first->info->friends.size()){

					x=studId.at(studd[i->first->info->friends[jj]]);
					cc.push_back(x);
					jj++;
				}
				edges[studId.at(i->first)]=cc;
				//cout<<cc.size()<<"inde"<<endl;
				
			}
			
		}
};

/**<
	Function saveGraph take content fron graphml file and save the content in the object of graph	
*/
void saveGraph(Graphm *g)	
{
	ofstream f("Graph.graphml");
	typedef map<Student*,int>::iterator iter_stud;
	typedef map<Faculty*,int>::iterator iter_fac;
	if(f.is_open())
	{
		f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
		f << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"" << endl;
		f << "\txmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << endl;
		f << "\txsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n\t   http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">" << endl;
		f << "		  <graph id=\"G\" edgedefault=\"undirected\">"<<endl;
		for(iter_stud i=g->studId.begin();i !=g->studId.end();i++)
		{
			f << "<node id=\""<<i->second<<"\" entrynumber=\""<<i->first->entrynumber<<"\" univ=\""<<i->first->info->univ->name<<"\" />" << endl;
		}
		//for(iter_fac i=g->facId.begin();i !=g->facId.end();i++)
		//{
		//	f << "          <node id=\""<<i->second<<"\" empcode=\""<<i->first->employecode<<"\" univ=\""<<i->first->info->univ->name<<"\" />" << endl;
		//}
		
		typedef map<int,vector<int> >::iterator iter_edges;
		int index=0;
		cout<<"\nEdges:"<<g->edges.size();
		for(iter_edges i=g->edges.begin();i!=g->edges.end();i++){
			for(int k=0;k<i->second.size();k++){
				f << "<edge id = \"e" << index << "\" source = \""<< i->first <<"\" target = \"" <<i->second[k]<< "\"/>" << endl;
				index++;
			}
		}
		f << "   </graph>" << endl;
		f << "</graphml>" << endl;
	}
	f.close();

}

/**<
	Function returnId return Id of the node	
*/
string returnId(string line){
	int i=0;
	string temp="";
	while(line[i]!=' '){
		temp=temp+line[i++];
	}
	return temp;
}
/**<
	Function returnEntry return the entrynumber of the node	
*/
string returnEntry(string line){
	int i=0;
	string temp="";
	while(line[i]!=' '){
		i++;
	}
	i++;
	while(line[i]!=' '){
		temp=temp+line[i++];
	}
	return temp;
}

/**<
	Function returnid2
*/
string returnid2(string line){
	int i=0;
	string temp="";
	while(line[i]!=' '){
		i++;
	}
	i++;
	while(i<line.size()){
		temp=temp+line[i++];
	}
	return temp;
}
/**<
	Function returnUniv return the university of the node	
*/
string returnUniv(string line){
	int i=0;
	string temp="";
	while(line[i]!=' '){
		i++;
	}
	i++;
	while(line[i]!=' '){
		i++;
	}
	i++;
	while(i<line.size()){
		temp=temp+line[i++];
	}
	return temp; 
}

/**<
	Struct ssnode to store the data of the node of the graph	
*/
struct ssnode{
	string entrynumber;
	string id;
	string university;
	string parent;
	vector<string> friends;
	ssnode(){}
	
};

/**<
		
*/
class grph{
public:
    map<string,ssnode> student;
    map<string,string> studentIndex;
};
grph readGraph(string filename){
	grph g;
	stringstream ss2;
	//ss2<<"python read.py ";
	ss2 << "python read.py ";
	ss2<<filename;
	system(ss2.str().c_str());
typedef map<string,ssnode> :: iterator it;
	map<int,string> vertex;
	
	ifstream ver("vertex.txt");
	ifstream edg("edges.txt");
	string line;
	char* pch;
	if (ver.is_open()){
		while(getline(ver,line)){
				ssnode ss;
				ss.entrynumber=returnEntry(line);
				ss.id=returnId(line);
				ss.university=returnUniv(line);
				g.studentIndex[returnId(line)]=returnEntry(line);
				g.student[ss.entrynumber]=ss;
				//cout<<ss.entrynumber<<" "<<ss.id<<" "<<ss.university<<endl;
		}
		ver.close();
	}
	
	if (edg.is_open()){
		while(getline(edg,line)){
				string e1=g.studentIndex[returnId(line)];
				string e2=g.studentIndex[returnid2(line)];
				g.student[e1].friends.push_back(e2);
				g.student[e2]. friends.push_back(e1);
			
		}
		edg.close();
	}
	return g;
}

#endif
