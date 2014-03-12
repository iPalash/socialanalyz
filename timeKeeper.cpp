/* 
 * File:   timeKeeper.cpp
 * Author: atul
 *
 * Created on 11 November, 2013, 10:35 PM
 */
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <random>
#include <queue>
#include <sys/msg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>


using namespace std;

struct univslen{
    long mtype;
    int lenUnivs;
};

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

class CompareTime {
    public:
    bool operator()(ids& t1, ids& t2) // Returns true if t1 is earlier than t2
    {
       if (t1.time_id < t2.time_id) return false;
       if (t1.time_id == t2.time_id && t1.thread_id < t2.thread_id) return false;
       return true;
    }
};

float currTime=0;
priority_queue<ids, vector<ids>, CompareTime> times;

int main(int argc, char** argv) {
    long total_simulation_time=atoi(*(argv+1))*24*60;
    long current_time=0;
    
    bool skip=false;
    
    int msqid;
    key_t key;
    
    univslen ul;
    ul.mtype  =3;
    
    key= ftok("graph.cpp", 'A');
    if(key==-1){
        perror("ftok");
        exit(1);
    }
    
    msqid= msgget(key, 0666);
    if(msqid==-1){
        perror("msggt");
        exit(1);
    }
    
    msgrcv(msqid, &ul, sizeof(ul)- sizeof(long), 3, 0);
    
    for(int i=0; i< ul.lenUnivs+2; i++){
        ids *temp= (ids*) malloc(sizeof(ids*));
        if(i==0){
            temp->mtype=2;
            temp->thread_id=0;
            temp->time_id=0;
            temp->univ_id=-1;
            
            msgsnd(msqid, temp, sizeof(*temp)- sizeof(long), 0);
        }
        else if(i==1){
            temp->mtype=2;
            temp->thread_id=1;
            temp->time_id=0;
            temp->univ_id=-1;

            msgsnd(msqid, temp, sizeof(*temp)- sizeof(long), 0);
        }
        else{
            temp->mtype=2;
            temp->thread_id=2;
            temp->time_id=0;
            temp->univ_id= i-2;

            msgsnd(msqid, temp, sizeof(*temp)- sizeof(long), 0);
        }
    }
    
    for(int i=0; i< ul.lenUnivs+2; i++){
        ids temp;
        msgrcv(msqid, &temp, sizeof(ids)- sizeof(long), 1, 0);
        times.push(temp);
    }
    
    for(int i=0;!skip;i++){
        i--;
        ids *temp1= (ids*) malloc(sizeof(ids*));
        
        if(! skip && !times.empty()){
            ids temp2;
            temp2=times.top();
            temp1=&temp2;
            times.pop();
            current_time= temp1->time_id;
        }
        
        if(current_time > total_simulation_time) skip= true;
        if(! skip && !times.empty()){
            temp1->mtype=2;
            temp1->time_id=currTime;
            msgsnd(msqid, temp1, sizeof(*temp1)- sizeof(long), 0);
        }
        
        if(!skip){
            ids temp;
            msgrcv(msqid, &temp, sizeof(ids)- sizeof(long), 1,0);
            temp.time_id=current_time+temp.time_id;
            times.push(temp);
        }
    }
    
    msgctl(msqid, IPC_RMID, NULL);
    return 0;
}
