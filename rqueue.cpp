// UMBC - CMSC 341 - Spring 2024 - Proj3
#include "rqueue.h"
RQueue::RQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) {
    m_heapType = heapType;
    m_structure = structure;
    m_priorFunc = priFn;
    m_heap = nullptr;
    m_size = 0;
}
RQueue::~RQueue() {
    clear(); //clear the queue
}
void RQueue::clear() {
    clearHelper(m_heap);
    m_heap = nullptr;
    m_size = 0;
}
RQueue::RQueue(const RQueue& rhs) {
    //Deep copy of the rhs object
    m_heap = copyHelper(rhs.m_heap);
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_priorFunc = rhs.m_priorFunc;
    m_size = rhs.m_size;
}
HEAPTYPE RQueue::getHeapType() const {
    return m_heapType;
}
RQueue& RQueue::operator=(const RQueue& rhs) {
    if(this != &rhs) {
        clear();
        m_priorFunc = rhs.m_priorFunc;
        m_heapType = rhs.m_heapType;
        m_structure = rhs.m_structure;
        m_heap = copyHelper(rhs.m_heap);
        m_size = rhs.m_size;
    }
    return *this;
}
void RQueue::mergeWithQueue(RQueue& rhs) {
    //Check if merging with itself
    if(this == &rhs){
        throw domain_error("Cannot merge with itself");
    }
    // Check if priority functions and structures are the same
    if(m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure){
        throw domain_error("Do not match");
    }
    m_heap = mergeHeap(m_heap, rhs.m_heap);
    rhs.m_heap = nullptr;
    m_size += rhs.m_size;
    //rhs.m_size = 0;

}
void RQueue::insertStudent(const Student& student) {
    Node* newStudent = new Node(student);

    RQueue tempQueue(getPriorityFn(), getHeapType(), getStructure());

    tempQueue.m_heap = newStudent;
    tempQueue.m_size = 1;

    mergeWithQueue(tempQueue);
}
int RQueue::numStudents() const{
    return m_size;
}
prifn_t RQueue::getPriorityFn() const {
    return m_priorFunc;
}
Student RQueue::getNextStudent() {
     if(m_size == 0){
         throw out_of_range("Queue is empty");
     }
     Node* root = m_heap;
     Student highestPriority = root->getStudent();
     Node* left = m_heap->m_left;
     Node* right = m_heap->m_right;

     // Create a temporary queue and merge left and right subtrees
     RQueue tempQueue(getPriorityFn(), getHeapType(), getStructure());
     tempQueue.m_heap = left;

     RQueue rightQueue(getPriorityFn(), getHeapType(), getStructure());
     rightQueue.m_heap = right;

     tempQueue.mergeWithQueue(rightQueue);

     delete root;
     m_heap = copyHelper(tempQueue.m_heap);
     m_size--;
     return highestPriority;
}

void RQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    m_priorFunc = priFn;
    m_heapType = heapType;
    if(m_structure == LEFTIST){
        rebuildLeftistHeap();
    }else if (m_structure == SKEW){
        rebuildSkewHeap();
    }
}

void RQueue::setStructure(STRUCTURE structure){
    if (m_structure != structure) {
        m_structure = structure;
        Node* newHeap = copyHelper(m_heap); // Deep copy existing heap
        clear(); // Clear current heap
        m_heap = newHeap; // Assign copied heap
        if(m_structure == SKEW){
            rebuildLeftistHeap();
            m_structure = LEFTIST;
            // rebuildSkewHeap();
        }else if(m_structure == LEFTIST){
            rebuildSkewHeap();
            m_structure = SKEW;
            // rebuildLeftistHeap();
        }
    }
}

STRUCTURE RQueue::getStructure() const {
    return m_structure;
}
void RQueue::printStudentsQueue() const {
    printHelper(m_heap);
}

void RQueue::dump() const {
    if (m_size == 0) {
        cout << "Empty heap.\n" ;
    } else {
        dump(m_heap);
    }
    cout << endl;
}
void RQueue::dump(Node *pos) const {
    if ( pos != nullptr ) {
        cout << "(";
        dump(pos->m_left);
        if (m_structure == SKEW)
            cout << m_priorFunc(pos->m_student) << ":" << pos->m_student.m_name;
        else
            cout << m_priorFunc(pos->m_student) << ":" << pos->m_student.m_name << ":" << pos->m_npl;
        dump(pos->m_right);
        cout << ")";
    }
}

ostream& operator<<(ostream& sout, const Student& student) {
    sout << "Student name: " << student.m_name
         << ", Major: " << student.getMajorStr()
         << ", Gender: " << student.getGenderStr()
         << ", Level: " << student.getLevelStr();
    return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
    sout << node.m_student;
    return sout;
}

void RQueue::clearHelper(Node *&aNode) {
    if(aNode != nullptr){
        clearHelper(aNode->m_left);
        clearHelper(aNode->m_right);
        delete aNode;
        aNode = nullptr;
        m_size--;
    }
}

Node* RQueue::copyHelper(Node *aNode) {
    if(aNode == nullptr){
        return nullptr;
    }
    //Create a new node with same student data
    Node* newNode = new Node(aNode->m_student);
    newNode->m_left = copyHelper(aNode->m_left);
    newNode->m_right = copyHelper(aNode->m_right);
    newNode->m_npl = aNode->m_npl;
    return newNode;
}

Node* RQueue::mergeHeap(Node* heap1, Node* heap2){
    //Check if any of heaps are empty
    if(heap1 == nullptr){
        return heap2;
    }
    if(heap2 == nullptr){
        return heap1;
    }
    //if root of heap 1 is higher priority
    if(m_heapType == MINHEAP){
        if(m_priorFunc(heap1->m_student) > m_priorFunc(heap2->m_student)) {
            swap(heap1, heap2);
        }
    }else{
        if(m_priorFunc(heap1->m_student) < m_priorFunc(heap2->m_student)){
            swap(heap1, heap2);
        }
    }
    if(m_structure == LEFTIST){
        // for leftist heap merge and then check for higher npl
        heap1->m_right = mergeHeap(heap1->m_right, heap2);
        if(getNPL(heap1->m_left) < getNPL(heap1->m_right)){
            swap(heap1->m_left, heap1->m_right);
        }
        heap1->m_npl = getNPL(heap1->m_right) + 1;
    }else{
        //for skew heap, you merge right child of heap 1 with heap 2 then swap
        heap1->m_right = mergeHeap(heap1->m_right, heap2);
        swap(heap1->m_left, heap1->m_right);
    }
    return heap1;

}
void RQueue::rebuildSkewHeap(){
    Node* newHeap = copyHelper(m_heap);
    clear();
    m_structure = SKEW; //set structure to skew
    insertNodes(newHeap);
}
void RQueue::rebuildLeftistHeap(){
    Node* newHeap = copyHelper(m_heap);
    clear();
    m_structure = LEFTIST; //set structure to leftist
    insertNodes(newHeap);
}

int RQueue::getNPL(Node *aNode) const {
    if(aNode == nullptr){
        return -1;
    }
    if(aNode->m_right == nullptr){
        return 0;
    }
    return aNode->m_right->m_npl + 1;
}

Node* RQueue::getRoot() const {
    return m_heap;
}

void RQueue::insertNodes(Node *node) {
    if(node == nullptr){
        return;
    }
    Node* left = node->m_left;
    Node* right = node->m_right;

    node->m_left = nullptr;
    node ->m_right = nullptr;
    //Merge current node to skew heap
    m_heap = mergeHeap(m_heap, node);
    m_size++;

    //Recursively reinsert left and right
    insertNodes(left);
    insertNodes(right);
}

void RQueue::printHelper(Node *node) const {
    if(node != nullptr){
        cout << "[" << m_priorFunc(node->m_student) << "] " << node->m_student << endl;
        printHelper(node->m_left);
        printHelper(node->m_right);
    }
}


