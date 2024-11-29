// UMBC - CMSC 341 - Spring 2024 - Proj3
#include "rqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>     //used to get the current time
using namespace std;
// We can use the Random class to generate the test data randomly!
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
// Priority functions compute an integer priority for a student.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Student &student);
int priorityFn2(const Student &student);
// a name database for testing purposes
// we can use this DB to generate student names randomly instead of generating
// random strings as names (from Random class)
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
        "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
        "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green",
        "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong",
        "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
        "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};
class Tester{
    // implement your test functions in this class
public:
    bool testNPLProperty(RQueue& queue);
    bool testLeftistProperty(RQueue& queue);
    bool testInsertionMinHeap(RQueue& queue, int numInserts, prifn_t priFn);
    bool testInsertionMaxHeap(RQueue& queue, int numInserts, prifn_t priFn);
    bool testPriorityFunctionChange(RQueue& queue);
    bool testRemovalMinHeap(RQueue& queue);
    bool testRemovalMaxHeap(RQueue& queue);
    bool testMergeEmptyWithNormal(RQueue& emptyQueue, RQueue& normalQueue);
    bool testCopyConstructorNormalCase(RQueue& queue1, RQueue& queue2);
    bool testCopyConstructorEdgeCase(RQueue& queue1, RQueue& queue2);
    bool testAssignmentOperatorNormalCase(RQueue& queue1, RQueue& queue2);
    bool testAssignmentOperatorEdgeCase(RQueue& queue1, RQueue& queue2);
    bool testEmptyDequeue();
    bool testMergeDifferentPriorityFunctions(RQueue& queue1, RQueue& queue2);

private:
    bool isMinHeap(Node* node, prifn_t priorityFn);
    bool isMaxHeap(Node* node, prifn_t priorityFn);
    bool checkNPLRecursive(Node* node);
    bool checkLeftistRecursive(Node* node);
    bool areHeapsSame(Node* node1, Node* node2);

};

int main(){
    // Creating an instance of RQueue for testing
    RQueue minQueue(priorityFn2, MINHEAP, LEFTIST);  // Assuming priorityFn2 is designed for MINHEAP
    RQueue maxQueue(priorityFn2, MAXHEAP, LEFTIST);  // Assuming priorityFn2 is designed for MINHEAP
    RQueue nplQueue(priorityFn2, MINHEAP, LEFTIST); //for npl values
    RQueue leftistQueue(priorityFn2, MINHEAP, LEFTIST); //for leftist queue
    RQueue emptyQueue(priorityFn2, MINHEAP, LEFTIST); //for empty queue
    RQueue filledQueue(priorityFn2, MINHEAP, LEFTIST); //for non empty queue
    RQueue originalQueue(priorityFn2, MINHEAP, LEFTIST); //for original queue (normal)
    RQueue originalEdgeQueue(priorityFn2, MINHEAP, LEFTIST); //for original queue(edge)
    RQueue priorityQueue(priorityFn2, MINHEAP, LEFTIST); //for priority queue
    RQueue priorityQueue1(priorityFn2, MINHEAP, LEFTIST); //for priority two
    RQueue priorityQueue2(priorityFn1, MAXHEAP, LEFTIST); //for priority one
    RQueue assignQueue1(priorityFn2, MINHEAP, LEFTIST); //for assignment operator 1 (normal)
    RQueue assignQueue2(priorityFn2, MINHEAP, LEFTIST); //for assignment operator 2 (normal)
    RQueue assignEdgeQueue(priorityFn2, MINHEAP, LEFTIST);//for assignment 1 edge case
    RQueue assignEdgeQueue2(priorityFn2, MINHEAP, LEFTIST); //for assignment 2 edge case
    RQueue removalMaxQueue(priorityFn1, MAXHEAP, LEFTIST); //for max heap remove
    RQueue removalMinQueue(priorityFn1, MINHEAP, LEFTIST); //for min heap remove

    Random nameGen(97,122);Random levelGen(0,3);
    Random majorGen(0,4);Random groupGen(0,3);
    Random raceGen(0,2);Random genderGen(0,2);
    Random incomeGen(0,4);Random highschoolGen(0,2);
    for (int i = 0; i < 10; ++i) {
        Student student(nameGen.getRandString(5), levelGen.getRandNum(),
                        majorGen.getRandNum(), groupGen.getRandNum(),
                        raceGen.getRandNum(), genderGen.getRandNum(),
                        incomeGen.getRandNum(), highschoolGen.getRandNum());
        filledQueue.insertStudent(student);
        originalQueue.insertStudent(student);
        priorityQueue.insertStudent(student);
        assignQueue1.insertStudent(student);
        nplQueue.insertStudent(student);
        leftistQueue.insertStudent(student);
        removalMinQueue.insertStudent(student);
        removalMaxQueue.insertStudent(student);
    }

    RQueue copyQueue(originalQueue); //for copy constructor normal case
    RQueue copyEdgeQueue(originalEdgeQueue); //for copy constructor edge case
    assignQueue2 = assignQueue1; //for assignment operator normal case
    assignEdgeQueue2 = assignEdgeQueue;  //for assignment operator edge case

    Tester tester;
    {
        cout << "Testing insertion for a normal case of min-heap: ";
        if(tester.testInsertionMinHeap(minQueue, 300, priorityFn2)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing insertion for a normal case of max-heap: ";
        if(tester.testInsertionMaxHeap(maxQueue, 300, priorityFn2)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing removal for a normal case of min-heap: ";
        if(tester.testRemovalMinHeap(removalMinQueue)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing removal for a normal case of max-heap: ";
        if(tester.testRemovalMaxHeap(removalMaxQueue)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing all nodes in a leftist heap have the correct NPL value: ";
        if (tester.testNPLProperty(nplQueue)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing leftist heap preserves the property: ";
        if (tester.testLeftistProperty(leftistQueue)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing correct heap rebuilt after priority function change with same date and different priority: ";
        if(tester.testPriorityFunctionChange(priorityQueue)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing merge of an empty queue with a normal queue: ";
        if(tester.testMergeEmptyWithNormal(emptyQueue, filledQueue)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing copy constructor for a normal case: ";
        if(tester.testCopyConstructorNormalCase(originalQueue, copyQueue)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing copy constructor for edge case: ";
        if(tester.testCopyConstructorEdgeCase(originalEdgeQueue, copyEdgeQueue)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing class assignment operator for normal case: ";
        if(tester.testAssignmentOperatorNormalCase(assignQueue1, assignQueue2)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing class assignment operator for edge case: ";
        if(tester.testAssignmentOperatorEdgeCase(assignEdgeQueue, assignEdgeQueue2)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing dequeue an empty queue: ";
        if(tester.testEmptyDequeue()){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }

        cout << "Testing merge queues with different priority functions: ";
        if(tester.testMergeDifferentPriorityFunctions(priorityQueue1, priorityQueue2)){
            cout << "PASSED!" << endl;
        }else{
            cout << "FAILED!" << endl;
        }
    }
    return 0;
}
int priorityFn1(const Student & student) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-10]
    //level + major + group
    //the highest priority would be 3+4+3 = 10
    //the lowest priority would be 0+0+0 = 0
    //the larger value means the higher priority
    int priority = student.getLevel() + student.getMajor() + student.getGroup();
    return priority;
}

int priorityFn2(const Student & student) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-10]
    //race + gender + income + highschool
    //the highest priority would be 0+0+0+0 = 0
    //the lowest priority would be 2+2+4+2 = 10
    //the smaller value means the higher priority
    int priority = student.getRace() + student.getGender() + student.getIncome() + student.getHighschool();
    return priority;
}

//Testing for normal case of min heap
bool Tester::testInsertionMinHeap(RQueue& queue, int insertions, prifn_t priFn){
    Random nameGen(97,122);Random levelGen(0,3);
    Random majorGen(0,4);Random groupGen(0,3);
    Random raceGen(0,2);Random genderGen(0,2);
    Random incomeGen(0,4);Random highschoolGen(0,2);
    for (int i = 0; i < insertions; ++i) {
        Student student(nameGen.getRandString(5), levelGen.getRandNum(),
                        majorGen.getRandNum(), groupGen.getRandNum(),
                        raceGen.getRandNum(), genderGen.getRandNum(),
                        incomeGen.getRandNum(), highschoolGen.getRandNum());
        queue.insertStudent(student);
    }
    return isMinHeap(queue.getRoot(), priFn);
}

//Testing for normal case of max heap
bool Tester::testInsertionMaxHeap(RQueue& queue, int insertions, prifn_t priFn){
    Random nameGen(97,122);Random levelGen(0,3);
    Random majorGen(0,4);Random groupGen(0,3);
    Random raceGen(0,2);Random genderGen(0,2);
    Random incomeGen(0,4);Random highschoolGen(0,2);
    for (int i = 0; i < insertions; ++i) {
        Student student(nameGen.getRandString(5), levelGen.getRandNum(),
                        majorGen.getRandNum(), groupGen.getRandNum(),
                        raceGen.getRandNum(), genderGen.getRandNum(),
                        incomeGen.getRandNum(), highschoolGen.getRandNum());
        queue.insertStudent(student);
    }
    return isMaxHeap(queue.getRoot(), priFn);
}

//Check if the tree maintains max-heap property
bool Tester::isMaxHeap(Node* node, prifn_t priorityFn){
    if(node == nullptr){
        return true;
    }
    if(node->m_left && priorityFn(node->m_left->m_student) > priorityFn(node->m_student)){
        return false;
    }
    if(node->m_right && priorityFn(node->m_right->m_student) > priorityFn(node->m_student)){
        return false;
    }
    return isMaxHeap(node->m_left, priorityFn) && isMaxHeap(node->m_right, priorityFn);
}

//Check if the tree maintains min-heap property
bool Tester::isMinHeap(Node* node, prifn_t priorityFn){
    if(node == nullptr){
        return true;
    }
    if(node->m_left && priorityFn(node->m_left->m_student) < priorityFn(node->m_student)){
        return false;
    }
    if(node->m_right && priorityFn(node->m_right->m_student) < priorityFn(node->m_student)){
        return false;
    }
    return isMinHeap(node->m_left, priorityFn) && isMinHeap(node->m_right, priorityFn);
}

//Recursive helper function to check NPL properties
bool Tester::checkNPLRecursive(Node* node){
    if(node == nullptr){
        return true;
    }
    int rightNPL;

    if(node->m_right != nullptr){
        rightNPL = node->m_right->m_npl;
    }else{
        rightNPL = -1;
    }

    return checkNPLRecursive(node->m_left) && checkNPLRecursive(node->m_right) && (node->m_npl == rightNPL + 1);
}

//Check if tree maintains npl property
bool Tester::testNPLProperty(RQueue& queue){
    return checkNPLRecursive(queue.getRoot());
}

//recursive helper function to check leftist property
bool Tester::checkLeftistRecursive(Node* node){
    if(node == nullptr){
        return true;
    }
    int leftNPL;
    int rightNPL;

    if(node->m_left != nullptr){
        leftNPL = node->m_left->m_npl;
    }else{
        leftNPL = -1;
    }

    if(node->m_right != nullptr){
        rightNPL = node->m_right->m_npl;
    }else{
        rightNPL = -1;
    }
    if(leftNPL < rightNPL){
        return false;
    }
    return checkLeftistRecursive(node->m_left) && checkLeftistRecursive(node->m_right);
}

//Check if tree maintains leftist property
bool Tester::testLeftistProperty(RQueue& queue){
    return checkLeftistRecursive(queue.getRoot());
}

//Check if the tree succesfully changes priority
bool Tester::testPriorityFunctionChange(RQueue& queue) {
    bool result = true;
    try {
        Random nameGen(97, 122);
        Random levelGen(0, 3);
        Random majorGen(0, 4);
        Random groupGen(0, 3);
        Random raceGen(0, 2);
        Random genderGen(0, 2);
        Random incomeGen(0, 4);
        Random highschoolGen(0, 2);
        for (int i = 0; i < 10; ++i) {
            Student student(nameGen.getRandString(5), levelGen.getRandNum(),
                            majorGen.getRandNum(), groupGen.getRandNum(),
                            raceGen.getRandNum(), genderGen.getRandNum(),
                            incomeGen.getRandNum(), highschoolGen.getRandNum());
            queue.insertStudent(student);//Insert students in random order
        }
        int size = queue.m_size;//Save current number of students
        result = result && (isMinHeap(queue.m_heap, queue.getPriorityFn()));
        queue.setPriorityFn(priorityFn2, queue.m_heapType);
        result = result && (size == queue.m_size);
        result = result && (isMinHeap(queue.m_heap, queue.getPriorityFn()));//Now it should be a min heap
        queue.setPriorityFn(priorityFn1, MAXHEAP); //set it to max heap
        result = result && (isMaxHeap(queue.m_heap, queue.getPriorityFn())); //Now it should be max heap
    }catch(out_of_range &e){//Catch out of range exceptions that may occur from
        result = false;
    }catch(domain_error &e){//Catch domain error exceptions which are thrown when trying
        result = false;
    }
    return result;
}

//test whether empty heap merges well with full heap
bool Tester::testMergeEmptyWithNormal(RQueue &emptyQueue, RQueue &normalQueue) {
    int sizeBeforeMerge = normalQueue.numStudents();
    emptyQueue.mergeWithQueue(normalQueue);
    int sizeAfterMerge = normalQueue.numStudents();
    if(emptyQueue.numStudents() == 0) { //if the size is still 0
        return false;
    }
    return sizeAfterMerge == sizeBeforeMerge;
}

//test copy constructor for normal case
bool Tester::testCopyConstructorNormalCase(RQueue &queue1, RQueue &queue2) {
    return areHeapsSame(queue1.getRoot(), queue2.getRoot());
}

//test copy constructor for edge case
bool Tester::testCopyConstructorEdgeCase(RQueue &queue1, RQueue &queue2) {
    return areHeapsSame(queue1.getRoot(), queue2.getRoot());
}

//test assignment operator for normal case
bool Tester::testAssignmentOperatorNormalCase(RQueue &queue1, RQueue &queue2) {
    return areHeapsSame(queue1.getRoot(), queue2.getRoot());
}

//test assignment operator for edge case
bool Tester::testAssignmentOperatorEdgeCase(RQueue &queue1, RQueue &queue2) {
    return areHeapsSame(queue1.getRoot(), queue2.getRoot());
}

//test dequeue for empty heap
bool Tester::testEmptyDequeue() {
    RQueue emptyQueue(priorityFn2, MINHEAP, SKEW);
    try{
        emptyQueue.getNextStudent();
        return false;
    }catch(const out_of_range& e){
        return true; //if it throws an exception
    }
}

//test merging heaps with different priority types
bool Tester::testMergeDifferentPriorityFunctions(RQueue &queue1, RQueue &queue2) {
    try{
        queue1.setPriorityFn(priorityFn1, queue1.getHeapType()); //set one heap to priority one
        queue2.setPriorityFn(priorityFn2, queue2.getHeapType()); // set other heap to priority two
        queue1.mergeWithQueue(queue2); //merge both heaps
        return false;
    }catch(const domain_error& e){
        return true;
    }
}

//test if the heaps of trees are same
bool Tester::areHeapsSame(Node *node1, Node *node2) {
    if(node1 == nullptr && node2 == nullptr) //both nodes are null
        return true;

    if(node1 == nullptr || node2 == nullptr) //one is null and the other isn't
        return false;

    if(node1->m_student == node2->m_student){//students match
        return areHeapsSame(node1->m_left, node2->m_left) &&
               areHeapsSame(node1->m_right, node2->m_right);
    }
    return false;
}

//test removal for min heap
bool Tester::testRemovalMinHeap(RQueue& queue) {
    bool result = true;
    try {
        Random nameGen(97, 122);
        Random levelGen(0, 3);
        Random majorGen(0, 4);
        Random groupGen(0, 3);
        Random raceGen(0, 2);
        Random genderGen(0, 2);
        Random incomeGen(0, 4);
        Random highschoolGen(0, 2);
        for (int i = 0; i < 10; ++i) {
            Student student(nameGen.getRandString(5), levelGen.getRandNum(),
                            majorGen.getRandNum(), groupGen.getRandNum(),
                            raceGen.getRandNum(), genderGen.getRandNum(),
                            incomeGen.getRandNum(), highschoolGen.getRandNum());
            queue.insertStudent(student);
        }
        Node *temp = queue.m_heap;
        int size = queue.m_size;
        result = result && (queue.m_size == size);
        result = result && (temp == queue.m_heap);
        queue.getNextStudent();
        result = result && !(temp == queue.m_heap);
        result = result && (isMinHeap(queue.m_heap, queue.getPriorityFn()));
    }catch (out_of_range &e){
        result = false;
    }catch (domain_error &e){
        result = false;
    }
    return result;
}

//test removal for max heap
bool Tester::testRemovalMaxHeap(RQueue& queue) {
    bool result = true;
    try {
        Random nameGen(97, 122);
        Random levelGen(0, 3);
        Random majorGen(0, 4);
        Random groupGen(0, 3);
        Random raceGen(0, 2);
        Random genderGen(0, 2);
        Random incomeGen(0, 4);
        Random highschoolGen(0, 2);
        for (int i = 0; i < 10; ++i) {
            Student student(nameGen.getRandString(5), levelGen.getRandNum(),
                            majorGen.getRandNum(), groupGen.getRandNum(),
                            raceGen.getRandNum(), genderGen.getRandNum(),
                            incomeGen.getRandNum(), highschoolGen.getRandNum());
            queue.insertStudent(student);
        }
        Node *temp = queue.m_heap;
        int size = queue.m_size;
        result = result && (queue.m_size == size);
        result = result && (temp == queue.m_heap);
        queue.getNextStudent();
        result = result && !(temp == queue.m_heap);
        result = result && (isMaxHeap(queue.m_heap, queue.getPriorityFn()));
    }catch (out_of_range &e){
        result = false;
    }catch (domain_error &e){
        result = false;
    }
    return result;
}
