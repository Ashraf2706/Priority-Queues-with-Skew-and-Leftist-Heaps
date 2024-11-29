// UMBC - CMSC 341 - Spring 2024 - Proj3

#ifndef _RQUEUE_H
#define _RQUEUE_H

#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;
using std::ostream;
using std::string;
using std::out_of_range;

class Grader;   // forward declaration (for grading purposes)
class Tester;   // forward declaration (for testing purposes)
class Student;  // forward declaration
class RQueue;   // forward declaration

// Constant parameters for scenario 1 (used for MAX heap)
enum Level {FRESH, SOPH, JUNI, SENI};//freshman, sophomore, junior, senior
enum Major {BIO, CHE, MEC, EEN, CSC};//bioinformatics, chemical eng, mechanial eng, electrical eng, computer science
enum Group {REGU, HONO, BSMS, RESE}; // regular, honored, accelerated Bs/Ms, funded research
// Constant parameters for scenario 2 (used for MIN heap)
enum Race   {MINORITY, MIX, MAJORITY};//
enum Gender {NONE, FEMALE, MALE};     //
enum Income {TIER1, TIER2, TIER3, TIER4, TIER5};//Tier1 is the lowest income
enum Highschool {LOW, MEDIUM, HIGH};//this defines the school rank
const int MIN = 0;  // this is a min value for a priority
const int MAX = 10; // this is a max value for a priority

enum HEAPTYPE {MINHEAP, MAXHEAP};
enum STRUCTURE {SKEW, LEFTIST};
// Priority function pointer type
typedef int (*prifn_t)(const Student&);

class Student{
public:
    friend class Grader; // for grading purposes
    friend class Tester; // for testing purposes
    friend class RQueue;
    Student(){
        m_name="";m_level=0;m_major=0;m_group=0;m_race=0;m_gender=0;m_income=0;m_highschool=0;
    }
    Student(string name, int level, int major, int group, int race,
            int gender, int income, int high){
        m_name = name; m_level = level; m_major = major; m_group = group;
        m_race = race; m_gender = gender; m_income = income; m_highschool = high;
    }
    // Overloaded assignment operator
    const Student& operator=(const Student& rhs){
        if (this != &rhs){
            m_name = rhs.m_name; m_level = rhs.m_level;
            m_major = rhs.m_major; m_group = rhs.m_group;
            m_race = rhs.m_race; m_gender = rhs.m_gender;
            m_income = rhs.m_income; m_highschool = rhs.m_highschool;
        }
        return *this;
    }
    // Overloaded equality operator
    bool operator==(const Student & rhs) const {
        return ((m_name == rhs.m_name) && (m_level == rhs.m_level) &&
                (m_major == rhs.m_major) && (m_group == rhs.m_group) &&
                (m_race == rhs.m_race) && (m_gender == rhs.m_gender) &&
                (m_income == rhs.m_income) && (m_highschool == rhs.m_highschool));
    }
    int getLevel() const {return m_level;}
    int getMajor() const {return m_major;}
    int getGroup() const {return m_group;}
    int getRace() const {return m_race;}
    int getGender() const {return m_gender;}
    int getIncome() const {return m_income;}
    int getHighschool() const {return m_highschool;}
    string getLevelStr() const{
        string result = "UNKNOWN";
        switch (m_level)
        {
            case FRESH: result = "Freshman"; break;
            case SOPH: result = "Sophomore"; break;
            case JUNI: result = "Junior"; break;
            case SENI: result = "Senior"; break;
            default: break;
        }
        return result;
    }
    string getGenderStr() const{
        string result = "UNKNOWN";
        switch (m_gender)
        {
            case NONE: result = "Not specified"; break;
            case FEMALE: result = "Female"; break;
            case MALE: result = "Male"; break;
            default: break;
        }
        return result;
    }
    string getMajorStr() const{
        string result = "UNKNOWN";
        switch (m_major)
        {
            case BIO: result = "Bioinformatics"; break;
            case CHE: result = "Chemical Engineering"; break;
            case MEC: result = "Mechanical Engineering"; break;
            case EEN: result = "Electrical Engineering"; break;
            case CSC: result = "Computer Science"; break;
            default: break;
        }
        return result;
    }
    // Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Student& student);

private:
    string m_name;        // student name
    int m_level;          // for valid values refer to enum type Level
    int m_major;          // for valid values refer to enum type Major
    int m_group;          // for valid values refer to enum type Group
    int m_race;           // for valid values refer to enum type Race
    int m_gender;         // for valid values refer to enum type Gender
    int m_income;         // for valid values refer to enum type Income
    int m_highschool;     // for valid values refer to enum type Highschool
};
class Node{
public:
    friend class Grader; // for grading purposes
    friend class Tester; // for testing purposes
    friend class RQueue;
    Node(Student student) {
        m_student = student;
        m_right = nullptr;
        m_left = nullptr;
        m_npl = 0;
    }
    void setNPL(int npl) {m_npl = npl;}
    int getNPL() const {return m_npl;}
    Student getStudent() const {return m_student;}
    // Overloaded insertion operators for Student and Node
    friend ostream& operator<<(ostream& sout, const Node& node);
private:
    Student m_student;    // student information
    Node * m_right;       // right child
    Node * m_left;        // left child
    int m_npl;            // null path length for leftist heap
};

class RQueue {
    // stores the skew/leftist heap, minheap/maxheap
public:
    friend class Grader; // for grading purposes
    friend class Tester; // for testing purposes
    RQueue(){}
    RQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure);
    ~RQueue();
    RQueue(const RQueue& rhs);
    RQueue& operator=(const RQueue& rhs);
    void insertStudent(const Student& student);
    Student getNextStudent(); // Return the highest priority student
    void mergeWithQueue(RQueue& rhs);
    void clear();
    int numStudents() const; // Return number of orders in queue
    void printStudentsQueue() const; // Print the queue using preorder traversal
    prifn_t getPriorityFn() const;
    // Set a new priority function. Must rebuild the heap!!!
    void setPriorityFn(prifn_t priFn, HEAPTYPE heapType);
    HEAPTYPE getHeapType() const;
    STRUCTURE getStructure() const;
    // Set a new data structure (skew/leftist). Must rebuild the heap!!!
    void setStructure(STRUCTURE structure);
    void dump() const; // For debugging purposes
private:
    Node * m_heap;          // Pointer to root of skew heap
    int m_size;             // Current size of the heap
    prifn_t m_priorFunc;    // Function to compute priority
    HEAPTYPE m_heapType;    // MINHEAP or MAXHEAP
    STRUCTURE m_structure;  // skew heap or leftist heap

    void dump(Node *pos) const; // helper function for dump

    /******************************************
     * Private function declarations go here! *
     ******************************************/
    void clearHelper(Node*& aNode);
    Node* copyHelper(Node* aNode);
    Node* mergeHeap(Node* heap1, Node* heap2);;
    void rebuildSkewHeap();
    void rebuildLeftistHeap();
    void insertNodes(Node* node);
    int getNPL(Node* node) const;
    void printHelper(Node* node) const;
    Node* getRoot() const;

};
#endif