/*
 * I.h
 *
 *  Created on: 10-Oct-2013
 *      Author: 123
 */

 /**<
	I.h is the header file which parse the details of universities structure and store the details in their respective structures
 */

#ifndef I_H_
#define I_H_

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <map>
#include <queue>
#include <random>
using namespace std;
struct Student;
struct Faculty;
struct Department;
/**<
	Structure to store students of a particular interest
*/
struct globalInterest{
	vector<Student*> glo_in;
	globalInterest(){}
};

/**<
	Structure storing different randomseed
*/
struct Randomseed
{
public:
	float faculty_random;
	float student_random;
	float course_random;
	float friend_random;
	Randomseed(){}
	Randomseed(float a,float b,float c,float d)
	{
		this->faculty_random = a;
		this->student_random = b;
		this->course_random = c;
		this->friend_random = d;
	}
};

/**<
	Structure storing students of a Hostel
*/
struct HOSTEL
{
	string name;
	vector<Student*> stu_hostel;
	HOSTEL(string n)
	{
		this->name = n;
	}
};

/**<
	Structure representing Courses having specifications
*/
struct Courses
{
public :
	string name;
	string dep_offer;
	Department* depp_offer;
	float freq_peryr;
	Faculty* prof;
	vector<Student*> students;
	Courses(){}
	Courses(string n,string d,float f)
	{
		this->name = n;
		this->freq_peryr = f;
		this->dep_offer = d;
	}
};

/**<
	Structure storing Interest with its name and popularity
*/
struct Interest
{
public:
	string name;
	float popularity;
	//vector<Student*> interest_student;
	//vector<Faculty*> interest_faculty;
	Interest(string a,float b)
	{
		this->name = a;
		this->popularity = b;
	}
};

/**<
	Structure of Department storing its name, number of faculty, number of students per year , Courses under the department and faculty under the department
*/
struct Department
{
public:
	string name;
	int num_faculty;
	int num_student_peryr;
	float sem_deptc;
	float sem_nondeptc;
	vector<Courses*> floatedCourses;
	vector<Faculty*> dep_faculty;
	Department(){}
	Department(string v,int n_f,int n_s,float s_d,float s_n)
	{
		this->name = v;
		this->num_faculty = n_f;
		this->num_student_peryr = n_s;
		this->sem_deptc = s_d;
		this->sem_nondeptc = s_n;
	}
};

/**<
	Class of university having objects Courses,departments,Hostels,Houselocality,Students,Interest
*/
class University
{
public:
	vector<Courses*> Course;
	vector<Department*> dept;
	vector<string> Hostel;
	vector<HOSTEL*> hostels_univ;
	vector<string> HouseLocality;
	vector<Student*> students;
	vector<Faculty*> profs;
	vector<Interest> Interests_univ;
	string name;
	float openness;
	float friendliness;
	float friendshiprate;
	University(string a){
		this->name = a;
	}
};

/**<
	Structure of node to define general charachteristic of student and faculty
*/
struct node
{
public:
	string name;
	Department* dept;
	string home;
	vector<Interest> interest;
	vector<string> friends;
	vector<Courses*> courses;
	University* univ;
	node(){}
	node(string n,Department* d,string h,vector<Interest> i,University* u)
	{
		this->univ= u;
		this->dept = d;
		this->name = n;
		this->home = h;
		this->interest = i;
	}
};

/**<
	Structure of faculty
*/
struct Faculty
{
public:
	string employecode;
	node* info;
	Faculty(){}
	Faculty(string e,node* p)
	{
		this->employecode = e;
		this->info = p;
	}
};

/**<
 	Structure of Student
 */
struct Student
{
public:
	string entrynumber;
	HOSTEL* home;
	char floorno;
	int room;
	node* info;
	Student(){}
	Student(string e,node* p,HOSTEL* h,char a,int b){
			this->entrynumber = e;
			this->info = p;
			this-> home = h;
			this->floorno = a;
			this->room = b;
		}
};

/**<
	Function to parse input file and storing data
*/
void create(map<int,University*> *Univs,Randomseed *randomseed,map<string,globalInterest*> *In)
{
	string line;
	int i=-1;
		ifstream input ("input.txt");
		if (input.is_open())
		{
			while (input.good())
			{
				getline(input,line);
				string x;
				stringstream s(line);
				while (s >> x){
 						if(x == "UNIVERSITY")
						{
 							i++;
							s >> x;
							University *U1 = new University(x);
							(*Univs)[i] = U1;
						}
						else if(x == "DEPARTMENT")
						{
							s>>x;
							string a = x;
							s>>x;
							float b = atof(x.c_str());
							s>>x;
							float c = atof(x.c_str());
							s>>x;
							float d = atof(x.c_str());
							s>>x;
							float e = atof(x.c_str());
							//cout<<a<<" "<<b<<" "<<c<<" "<<d<<" "<<e<<endl;
							Department *d1 = new Department(a,b,c,d,e);
							Univs->at(i)->dept.push_back(d1);
						}
						else if(x == "COURSE")
						{
							s>>x;
							string a = x;
							s>>x;
							string b = x;
							s>>x;
							float c = atof(x.c_str());
							Courses *c1 = new Courses(a,b,c);
							Univs->at(i)->Course.push_back(c1);
						}
						else if(x == "INTEREST")
						{
							s >> x;
							string tempname = x;
							s >> x;
							Interest in = Interest(tempname,atof(x.c_str()));
							Univs->at(i)->Interests_univ.push_back(in);
							if(In->find(tempname) == In->end()){
								globalInterest *g = new globalInterest();
								(*In)[tempname] = g;
						//		cout << "\n inserted " << tempname;
							} 

						}
						else if(x == "HOSTEL")
						{
							s>>x;
							Univs->at(i)->Hostel.push_back(x);
							HOSTEL* h = new HOSTEL(x);
							Univs->at(i)->hostels_univ.push_back(h);
						}
						else if(x == "HOUSELOCALITY")
						{
							s>>x;
							Univs->at(i)->HouseLocality.push_back(x);
						}
						else if(x == "FRIENDSHIPRATE")
						{
							s>>x;
							Univs->at(i)->friendshiprate = atof(x.c_str());
						}
						else if(x == "OPENNESS")
						{
							s>>x;
							Univs->at(i)->openness = atof(x.c_str());
						}
						else if(x == "FRIENDLINESS")
						{
							s>>x;
							Univs->at(i)->friendliness = atof(x.c_str());
						}
						else if(x == "RANDOMSEED")
						{
							s>>x;
							float a = atof(x.c_str());
							s>>x;
							float b = atof(x.c_str());
							s>>x;
							float c = atof(x.c_str());
							s>>x;
							float d = atof(x.c_str());
							(*randomseed) = Randomseed(a,b,c,d);
						}
				}
			}

			input.close();
		}

}
#endif /* I_H_ */