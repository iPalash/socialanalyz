#include "gml.h"
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>

using namespace std;

int year = 2013;
string name[]={"Aastha","Atul","Abhishek","Anand","Abhijeet","Aditya","Akash","Ashish","Arvind","Aarti","Akon"};
string dept[]={"d1","d2","d3","d4","d5","d6","d7","d8","d9","d10","d11"};
string home[]={"h1","h2","h3","h4","h5","h6","h7","h8","h9","h10","h11"};
char floors[] = { 'a','b','c','d'};
typedef map<int,University*>::iterator it_type;
ofstream logfile("Friendship Log.txt");
//stringstream logfile;
class Graph;
struct ids;

default_random_engine sr;
default_random_engine fcr;
default_random_engine cr;
default_random_engine fr;

struct univslen{
    long mtype;
    int lenUnivs;
};
/**
    Structure of ids defining identity to different threads
*/
struct ids {
    long mtype;
    int thread_id;
    int univ_id;
    float time_id;
    ids(){}
    ids(long mt, int a,int b,int c){
        this->mtype=mt;
        this->thread_id = a;
        this->univ_id = b;
        this->time_id = c;
    }
};
/**
    Class of graph and it stores the students in map
*/
class Graph
{
public:
	map<string,Student*> student;
	
	Graph(){}
        
	void addStudent(Graph *g,Student* s)
	{
		(*g).student[s->entrynumber] = s;
	}
};
/**
    Function generateFaculty generates the faculty to create nodes and store information of faculty in their respective attributes
*/
void generateFaculty(Graph *g,map<int,University*> *Univs)
{   
	for(it_type iterator = Univs->begin(); iterator != Univs->end(); iterator++)
    {
    	University* u=iterator->second;
    Department *d;
    node* p;
    Faculty* f;
    vector<Interest> i;


    for(int k = 0; k<u->dept.size() ;k++){
#pragma omp parallel
#pragma omp for schedule(dynamic) private(d,p,f,i) nowait
        for(int j = 0; j<u->dept.at(k)->num_faculty ;j++)
        {
            stringstream a;
            a<<j;
            string x = a.str();
            string e = u->name+u->dept.at(0)->name+x;
            string n = name[fcr()%11];
            int xx;
#pragma omp critical
            {
                xx=fcr()%u->dept.size();
                d = (u->dept[xx]);
            }
            string h = u->HouseLocality[fcr()%u->HouseLocality.size()];
				vector<Interest> i;
				int uu = u->Interests_univ.size();
				int count = 0;
				for(int j = 0; j < uu ; j++)
				{
					int r = fcr()%101;
					if(r < (u->Interests_univ.at(j).popularity)*100){
						i.push_back(u->Interests_univ.at(j)); //interest[(19*j+23)%3];
						count++;
					}
				}

            p = new node(n,d,h,i,u);
            f = new Faculty(e,p);
            
#pragma omp critical
            {
                u->dept[xx]->dep_faculty.push_back(f);
                u->profs.push_back(f);
            }

        }
    }
        }
}
/**
    Function generateStudent generates the Student and store their information in their attributes and in the graph  
*/
float generateStudent(Graph* g,map<int,University*> *Univs, map<string,globalInterest*> *gIT){
    int proc= omp_get_num_procs();
#pragma omp parallel num_threads(proc)
    for(it_type iterator = Univs->begin(); iterator != Univs->end(); iterator++){
        int o =1;
	University* u=iterator->second;
#pragma omp for schedule(dynamic) nowait
	for(int k = 0; k<u->dept.size() ;k++){
            for(int j = 0; j<u->dept.at(k)->num_student_peryr ;j++){
                stringstream a;
		a<<year;
                string y = a.str();
		stringstream b;
                b<<j;
		string x = b.str();
                string e = y+u->name+u->dept.at(0)->name+x;
		string n = name[sr()%11];
                Department* d = (u->dept[sr()%u->dept.size()]);
		string h = home[sr()%11];
                int t = sr()%u->hostels_univ.size();
		HOSTEL* H = u->hostels_univ.at(t);
                char floorno = floors[year%4];
		int roomno = j;
                vector<Interest> i;
		int uu = u->Interests_univ.size();
                int count = 0;
		for(int j = 0; j < uu ; j++){
                    int r = sr()%101;
                    if(r < (u->Interests_univ.at(j).popularity)*100){
#pragma omp critical
                        i.push_back(u->Interests_univ.at(j)); //interest[(19*j+23)%3];
			count++;
                    }
		}
                node* p = new node(n,d,h,i,u);
		Student* s = new Student(e,p,H,floorno,roomno);
#pragma omp critical
		for(int i = 0; i < count ; i++){
                    gIT->at(s->info->interest.at(i).name)->glo_in.push_back(s);
                }
#pragma omp critical
		{
                    u->hostels_univ.at(t)->stu_hostel.push_back(s);
                    u->students.push_back(s);
                    (*g).addStudent(g,s);
                }
            }
            o += u->dept.at(k)->num_student_peryr;
        }
    }
year+=1;
    return (float)12*30*24;
}
/**
    Function floatCourseUniv float Courses in the university
*/
void floatCourseUniv(University* u){
    int proc=omp_get_num_procs();
#pragma omp parallel num_threads(proc)
#pragma omp for schedule(dynamic) nowait
	for(int i=0;i<u->Course.size();i++){
		if (1+cr()%100<(float)u->Course[i]->freq_peryr*75)
#pragma omp critical
			u->Course[i]->depp_offer->floatedCourses.push_back(u->Course[i]);
	}
}
/**
    Function assigns the Courses to professors
*/
void AssignCourseProf(University* u){
    int proc=omp_get_num_procs();
#pragma omp parallel num_threads(proc)
#pragma omp for schedule(dynamic) nowait
	for(int i=0;i<u->dept.size();i++){
		for(int j=0;j<u->dept[i]->floatedCourses.size();j++){
#pragma omp critical
                    {
			u->dept[i]->floatedCourses[j]->prof=u->dept[i]->dep_faculty[j % (u->dept[i]->dep_faculty.size())];
			u->dept[i]->floatedCourses[j]->prof->info->courses.push_back(u->dept[i]->floatedCourses[j]);
                    }
		}
	}
}
/**
    Function AssignStudent assigns Courses to students
*/
void AssignStudent(University* u){	
	Student* stud;
        int proc=omp_get_num_procs();
        
#pragma omp parallel num_threads(proc)
#pragma omp for schedule(dynamic) private(stud) nowait
        for(int i=0;i<u->students.size(); i++){
            stud=u->students[i];
            for(int j=0;j<=(int)stud->info->dept->sem_deptc;j++){
		int xx=cr()%stud->info->dept->floatedCourses.size();
#pragma omp critical
                {
                    stud->info->courses.push_back(stud->info->dept->floatedCourses[xx]);
                    stud->info->dept->floatedCourses[xx]->students.push_back(stud);
                }
            }
            if (1+cr()%100<((float)stud->info->dept->sem_deptc-(int)stud->info->dept->sem_deptc)*100){
		int xx=cr()%stud->info->dept->floatedCourses.size();
#pragma omp critical
                {
                    stud->info->courses.push_back(stud->info->dept->floatedCourses[xx]);
                    stud->info->dept->floatedCourses[xx]->students.push_back(stud);
                }
            }
            for(int ji=0;ji<=(int)stud->info->dept->sem_nondeptc;ji++){
		int xx=cr()%u->dept.size();
                while(u->dept[xx]==stud->info->dept)
                    xx=cr()%u->dept.size();
                int yy=cr()%u->dept[xx]->floatedCourses.size();
#pragma omp critical
                {
                    stud->info->courses.push_back(u->dept[xx]->floatedCourses[yy]);
                    u->dept[xx]->floatedCourses[yy]->students.push_back(stud);
                }
            }
            if (1+cr()%100<((float)stud->info->dept->sem_nondeptc-(int)stud->info->dept->sem_nondeptc)*100){
		int xx=cr()%u->dept.size();
                while(u->dept[xx]==stud->info->dept)
                    xx=cr()%u->dept.size();
                int yy=cr()%u->dept[xx]->floatedCourses.size();
#pragma omp critical
                {
                    stud->info->courses.push_back(u->dept[xx]->floatedCourses[yy]);
                    u->dept[xx]->floatedCourses[yy]->students.push_back(stud);
                }
            }
        }
}

/**
    Function generates the Courses 
*/
float generateCourses(map<int,University*> *Univs){
    for(it_type iterator = Univs->begin(); iterator != Univs->end(); iterator++)
    {
    	University* u=iterator->second;
	floatCourseUniv(u);
	AssignCourseProf(u);
	AssignStudent(u);
    }
        return (float)6*30*24;
}

bool contains(vector<string> *vec, string toSearch){
    int proc=omp_get_num_procs();
    bool toReturn= false;
#pragma omp parallel num_threads(proc)
#pragma omp for schedule(dynamic) nowait
    for(int i=0; i< vec->size(); i++){
        if(vec->at(i).compare(toSearch)==0){
            toReturn= true;
            i=vec->size();
        }
    }
    return toReturn;
}

/**
    Function commonInterests return bool on comparing two students whether they have common Interest
*/
bool commonInterests(vector<Interest> *vec1, vector<Interest> *vec2){
    int proc=omp_get_num_procs();
    bool toReturn=false;
#pragma omp parallel num_threads(proc)
#pragma omp for schedule(dynamic) nowait
    for(int i=0; i< vec1->size(); i++){
        for(int j=0; j< vec2->size(); j++){
            if(vec1->at(i).name.compare(vec2->at(j).name)==0){
                toReturn=true;
                j=vec2->size();
                i=vec1->size();
            }
        }
    }
    return toReturn;
}
/**
    Function commonCourses return bool on comparing two students whether they have common Courses
*/
bool commonCourses(vector<Courses*>*vec1, vector<Courses*>*vec2){
        int proc=omp_get_num_procs();
        bool toReturn=false;
#pragma omp parallel num_threads(proc)
#pragma omp for schedule(dynamic) nowait
    for(int i=0; i< vec1->size(); i++){
        for(int j=0; j<vec2->size();j++){
            if(vec1->at(i)->name.compare(vec2->at(j)->name)==0){
                toReturn= true;
                j=vec2->size();
                i=vec1->size();
            }
        }
    }
    return toReturn;
}

/**
    Function generateFriends generate friends , create edge betwwen two students in graph
*/
float generateFriends(map<int, University*> *uni, map<string,globalInterest*> *allInterest, int uid){
    
    exponential_distribution<float> distribution(uni->at(uid)->friendshiprate);
    
    float tempo= distribution(fr)*60;
    University *iter=uni->at(uid);
    
    int proc= omp_get_num_procs();
    bool picked=true;
    
    {
        vector<Student*> *stud=&iter->students;
        float outp=iter->openness;
        
        int i= fr()%(stud->size());
        logfile<<"----------------------------------------------------------------------------"<<endl;
        logfile<< "REQUESTOR NAME: "<< stud->at(i)->entrynumber<<endl;
        
        {
            if(fr()%100 < outp*100){
                int temp=fr()%2;
                bool alreadypicked=false;
                if(temp==0){
                    logfile << "Request Type: SEARCHING FRIENDS IN OTHER UNIVERSITIES\n";
                    int uniselector=fr()%uni->size();
                    
                    while(uni->at(uniselector)->name.compare(iter->name)==0){
                        uniselector=fr()%uni->size();
                    }
                    
                        vector<Student*> *tempstud=&uni->at(uniselector)->students;
#pragma omp parallel num_threads(proc)
#pragma omp for schedule(dynamic) nowait
                        for(int k=0; k<75; k++){
                            int tmp=fr()%tempstud->size();
                            vector<string> *frnd=&stud->at(i)->info->friends;
                            
                            if(contains(frnd, tempstud->at(tmp)->entrynumber)){
                                if(k==74 && ! alreadypicked) picked=false;
                                continue;
                            }
                            else if(commonInterests(&stud->at(i)->info->interest, &tempstud->at(tmp)->info->interest)){
                                if(k==74 && !alreadypicked) picked=false;
                                continue;
                            }
                            else {
#pragma omp critical        
                                {
                                    alreadypicked=true;
                                    temp=tmp;
                                }
                            }
                        }
                        
                        if(fr()%100 < uni->at(uniselector)->friendliness*100 && picked){
                            tempstud->at(temp)->info->friends.push_back(stud->at(i)->entrynumber);
                            stud->at(i)->info->friends.push_back(tempstud->at(temp)->entrynumber);
                            
                            logfile<< "REQUEST STATUS: "<< "ACCEPTED"<<endl;
                            logfile<< "FRIEND CHOOSED: "<< tempstud->at(temp)<<endl;
                            logfile<< "UNIVERSITY OF NEW FRIEND: "<< uni->at(uniselector)<< endl;
                            
                            logfile.flush();
                            return tempo;
                        }
                        else if(picked){
                            logfile<< "REQUEST STATUS: "<< "DENIED"<< endl;
                            logfile<< "DENYING PERSON: "<< tempstud->at(temp)->entrynumber<<endl;
                            logfile<< "UNIVERSITY OF DENYING PERSON: "<< uni->at(uniselector)->name<< endl;

                            logfile.flush();
                            return tempo;
                        }
                        else{
                            logfile<< "SEARCH STATUS :"<< "ABANDONED"<<endl;
                            logfile<< "SEARCHED UNIVERSITY :"<< uni->at(uniselector)->name<< endl;
                            logfile<< "seems already have too many friends in the university"<<endl;
                            
                            logfile.flush();
                            return tempo;
                        }
                }
                
                else{
                    logfile << "Request Type: SEARCHING FRIENDS HAVING UNCOMMON PROFILE\n";
#pragma omp parallel num_threads(proc)
#pragma omp for schedule(dynamic) nowait
                    for(int j=0; j<75; j++){
                        int tmp= fr()%stud->size();
                        if(stud->at(tmp)->info->dept->name.compare(stud->at(i)->info->dept->name)==0){
                            if(j==74 && !alreadypicked) picked=false;
                            continue;
                        }
                        else if(stud->at(tmp)->info->home.compare(stud->at(i)->info->home)==0){
                            if(j==74 && !alreadypicked) picked=false;
                            continue;
                        }
                        else if(commonCourses(&stud->at(tmp)->info->courses, &stud->at(i)->info->courses)){
                            if(j==74 && !alreadypicked) picked=false;
                            continue;
                        }
                        else if(commonInterests(&stud->at(i)->info->interest, &stud->at(tmp)->info->interest)){
                            if(j==74 && !alreadypicked) picked=false;
                            continue;
                        }
                        else if(contains(&stud->at(i)->info->friends, stud->at(tmp)->entrynumber)){
                            if(j==74 && !alreadypicked) picked=false;
                            continue;
                        }
                        else{
#pragma omp critical
                            {
                                alreadypicked=true;
                                temp=tmp;
                            }
                        }
                    }
                        
                    if(fr()%100 < iter->friendliness*100 && picked){
                        stud->at(i)->info->friends.push_back(stud->at(temp)->entrynumber);
                        stud->at(temp)->info->friends.push_back(stud->at(i)->entrynumber);
                        
                        logfile<< "REQUEST STATUS: "<< "ACCEPTED"<<endl;
                        logfile<< "FRIEND CHOOSED: "<< stud->at(temp)<<endl;
                        
                        logfile.flush();
                        return tempo;
                    }
                    else if(picked){
                        logfile << "REQUEST STATUS: "<< "DENIED"<<endl;
                        logfile<< "DENYING PERSON: "<< stud->at(temp)->entrynumber<<endl;
                        
                        logfile.flush();
                        return tempo;
                    }
                    else{
                        logfile<< "SEARCH STATUS :"<< "ABANDONED"<<endl;
                        logfile<< "seems already have too many friends having uncommon profile"<<endl;
                            
                        logfile.flush();
                        return tempo;
                    }
                }
            }
            else{
                
                int choose=fr()%4;
                if(choose==0){
                    int k=0,j=0;
                    vector<Student*> *tempstud;
                    vector<Student*> *ttempstud;
                    logfile << "Request Type: SEARCHING FRIENDS HAVING COMMON COURSES IN THE SEMESTER\n";
                    
#pragma omp parallel num_threads(proc)
#pragma omp for private(j, ttempstud) schedule(dynamic) nowait
                    for(int l=0; l<75; l++){
                        j=fr()%stud->at(i)->info->courses.size();
                        ttempstud=&stud->at(i)->info->courses[j]->students;
                        for(int m=0; m<75; m++){
                            int kk=fr()%ttempstud->size();
                            if(contains(&stud->at(i)->info->friends, ttempstud->at(kk)->entrynumber)){
                                if(m==74) picked=false;
                                continue;
                            }
                            else {
#pragma omp critical
                                tempstud=ttempstud;
                                k=kk;
                            }
                        }
                    }
                    
                    if(fr()%100 < iter->friendliness*100 && picked){
                        
                        stud->at(i)->info->friends.push_back(tempstud->at(k)->entrynumber);
                        tempstud->at(k)->info->friends.push_back(stud->at(i)->entrynumber);
                        
                        logfile<< "REQUEST STATUS: "<< "ACCEPTED"<<endl;
                        logfile<< "FRIEND CHOOSED: "<< tempstud->at(k)->entrynumber<<endl;
                        
                        logfile.flush();
                        
                        return tempo;
                    }
                    else if(picked){
                        logfile << "REQUEST STATUS: "<< "DENIED"<<endl;
                        logfile<< "DENYING PERSON: "<< tempstud->at(k)->entrynumber<<endl;
                        
                        logfile.flush();
                        
                        return tempo;
                    }
                    else{
                        logfile<< "SEARCH STATUS :"<< "ABANDONED"<<endl;
                        logfile<< "seems already have too many friends having common courses in the semester"<<endl;
                        
                        logfile.flush();
                    
                        return tempo;
                    }
                }
                else if(choose==1){
                    int j=0,k=0;
                    vector<Student*> *tempstud;
                    vector<Student*> *ttempstud;
                    logfile << "Request Type: SEARCHING FRIENDS IN SAME UNIVERSITY\n";
                    vector<Courses*> *flc=&stud->at(i)->info->dept->floatedCourses;
#pragma omp parallel num_threads(proc)
#pragma omp for schedule(dynamic) private(j, ttempstud) nowait
                    for(int l=0; l<75; l++){
                        j=fr()%flc->size();
                        ttempstud=&flc->at(j)->students;
                        for(int m=0; m<75; m++){
                            int kk=fr()%ttempstud->size();
                            if(contains(&stud->at(i)->info->friends, ttempstud->at(kk)->entrynumber)){
                                if(m==74) picked=false;
                                continue;
                            }
                            else{
#pragma omp critical
                                k=kk;
                                tempstud=ttempstud;
                            }
                        }
                    }
                    

                    if(fr()%100 < iter->friendliness*100 && picked){
                        stud->at(i)->info->friends.push_back(tempstud->at(k)->entrynumber);
                        tempstud->at(k)->info->friends.push_back(stud->at(i)->entrynumber);
                        
                        logfile<< "REQUEST STATUS: "<< "ACCEPTED"<<endl;
                        logfile<< "FRIEND CHOOSED: "<< tempstud->at(k)->entrynumber<<endl;
                        
                        logfile.flush();
                        
                        return tempo;
                    }
                    else if(picked){
                        logfile << "REQUEST STATUS: "<< "DENIED"<<endl;
                        logfile<< "DENYING PERSON: "<< tempstud->at(k)->entrynumber<<endl;
                        
                        logfile.flush();
                        
                        return tempo;
                    }
                    else{
                        logfile<< "SEARCH STATUS :"<< "ABANDONED"<<endl;
                        logfile<< "seems already have too many friends in his/her university"<<endl;
                        
                        logfile.flush();

                        return tempo;
                    }
                }
                else if(choose==2){
                    logfile << "Request Type: SEARCHING FRIENDS IN SAME HOSTEL\n";
                    vector<Student*> *tempstud= &stud->at(i)->home->stu_hostel;
                    int cran=0;
#pragma omp parallel num_threads(proc)
#pragma omp schedule(dynamic) nowait
                    for(int j=0; j<75; j++){
                        int tcran=fr()%tempstud->size();
                        if(contains(&stud->at(i)->info->friends, tempstud->at(tcran)->entrynumber)){
                            if(j==74) picked=false;
                            continue;
                        }
                        else{
#pragma omp critical
                            cran=tcran;
                        }
                    }
                        if(fr()%100 < iter->friendliness*100 && picked){
                            stud->at(i)->info->friends.push_back(tempstud->at(cran)->entrynumber);
                            tempstud->at(cran)->info->friends.push_back(stud->at(i)->entrynumber);
                            
                            logfile<< "REQUEST STATUS: "<< "ACCEPTED"<<endl;
                            logfile<< "FRIEND CHOOSED: "<< tempstud->at(cran)->entrynumber<<endl;
                            
                            logfile.flush();

                            return tempo;
                        }
                        else if(picked){
                            logfile << "REQUEST STATUS: "<< "DENIED"<<endl;
                            logfile<< "DENYING PERSON: "<< tempstud->at(cran)->entrynumber<<endl;
                            
                            logfile.flush();

                            return tempo;
                        }
                        else{
                            logfile<< "SEARCH STATUS :"<< "ABANDONED"<<endl;
                            logfile<< "seems already have too many friends in his/her hostel"<<endl;
                            
                            logfile.flush();

                                return tempo;
                        }
                }
                else{
                    logfile << "Request Type: SEARCHING FRIENDS HAVING COMMON INTERESTS\n";
                    int crand=fr()%stud->at(i)->info->interest.size();
                    vector<Student*> *tempstud= &(allInterest->at(stud->at(i)->info->interest.at(crand).name)->glo_in);
#pragma omp parallel num_threads(proc)
#pragma omp for schedule(dynamic) nowait
                    for(int j=0; j<75; j++){
                        int ccrand=fr()%tempstud->size();
                        if(contains(&stud->at(i)->info->friends, tempstud->at(ccrand)->entrynumber)){
                            if(j==74) picked=false;
                            continue;
                        }
                        else{
#pragma omp critical
                            crand=ccrand;
                        }
                    }
                        
                        if(fr()%100 < iter->friendliness*100 && picked){
                            stud->at(i)->info->friends.push_back(tempstud->at(crand)->entrynumber);
                            tempstud->at(crand)->info->friends.push_back(stud->at(i)->entrynumber);
                            
                            logfile<< "REQUEST STATUS: "<< "ACCEPTED"<<endl;
                            logfile<< "FRIEND CHOOSED: "<< tempstud->at(crand)->entrynumber<<endl;
                            
                            logfile.flush();

                            return tempo;
                        }
                        else if(picked){
                            logfile << "REQUEST STATUS: "<< "DENIED"<<endl;
                            logfile<< "DENYING PERSON: "<< tempstud->at(crand)->entrynumber<<endl;
                            
                            logfile.flush();

                            return tempo;
                        }
                        else{
                            logfile<< "SEARCH STATUS :"<< "ABANDONED"<<endl;
                            logfile<< "seems already have too many friends in having common interests"<<endl;
                            
                            logfile.flush();
                            return tempo;   
                        }
                }
            }
        }
    }
}

int main()
{
	map<int,University*> Univs;
        
	map<string,globalInterest*> allInterest;

	Randomseed randomseed = Randomseed();
	create(&Univs,&randomseed,&allInterest);
        
	sr.seed(randomseed.student_random);
	fcr.seed(randomseed.faculty_random);
	cr.seed(randomseed.course_random);
	fr.seed(randomseed.friend_random);

	Graph network = Graph();
        int lenUnivs=0;
        
	for(it_type iterator = Univs.begin(); iterator != Univs.end(); iterator++){
            lenUnivs++;
            University* uuu=iterator->second;
		for(int i=0;i<uuu->Course.size();i++){
			string dp=uuu->Course[i]->dep_offer;
			int j=0;
			for(j=0;j<uuu->dept.size();j++){
				if (uuu->dept[j]->name==dp) break;
			}
			uuu->Course[i]->depp_offer=uuu->dept[j];
		}
	}
        
	generateFaculty(&network, &Univs);
        
        int msqid;
        key_t key;
        
        key= ftok("graph.cpp", 'A');
        if(key==-1){
            perror("ftok");
            exit(1);
        }
        msqid= msgget(key, 0666| IPC_CREAT);
        if(msqid==-1){
            perror("msqid");
            exit(1);
        }
        
        univslen ul;
	ul.mtype = 3;
        ul.lenUnivs=lenUnivs;
        
        msgsnd(msqid, &ul, sizeof(int), 0);
        
        bool skip=false;
        for(int i=0;!skip;i++){
            i--;
            ids temp;
            if(msgrcv(msqid, &temp, sizeof(temp)-sizeof(long), 2, 0)==-1){
                perror("msgrcv");
                logfile.close();
		Graphm gm;
		gm.insert(Univs);
		saveGraph(&gm);

                exit(1);
            }
            if(temp.thread_id==0){
                float returned= generateStudent(&network, &Univs,& allInterest);
                ids *temp2= (ids*) malloc(sizeof(ids*));
                temp2->mtype=1;
                temp2->thread_id=0;
                temp2->time_id=returned;
                temp2->univ_id=-1;
               
               msgsnd(msqid, temp2, sizeof(*temp2)- sizeof(long), 0);
            }
            else if(temp.thread_id==1){
                
                float returned= generateCourses(&Univs);
                ids *temp2= (ids*) malloc(sizeof(ids*));
                temp2->mtype=1;
                temp2->thread_id=1;
                temp2->time_id=returned;
                temp2->univ_id=-1;
                
                msgsnd(msqid, temp2, sizeof(*temp2)- sizeof(long), 0);
            }
            else if(temp.thread_id==2){
                
                float returned = generateFriends(&Univs,&allInterest, temp.univ_id);
                
                ids *temp2= (ids*) malloc(sizeof(ids*));
                temp2->mtype=1;
                temp2->thread_id=2;
                temp2->time_id=returned;
                temp2->univ_id=temp.univ_id;
                
                msgsnd(msqid, temp2, sizeof(*temp2)- sizeof(long), 0);
            }
        }
        logfile.close();
	Graphm gm;
	gm.insert(Univs);
	saveGraph(&gm);
        return 0;
}
