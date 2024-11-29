
# Priority Queues with Skew and Leftist Heaps

## Overview
This project implements a priority queue system using skew and leftist heaps. It manages student data with customizable priority functions and heap structures. The system supports dynamic merging, efficient priority-based operations, and reconstruction of heaps for various scenarios.

## Features

### Core Functionality
1. **Priority Queue Operations**:
   - Insert students into the queue.
   - Retrieve and remove the highest-priority student.
   - Merge two queues dynamically while maintaining heap properties.

2. **Heap Structures**:
   - **Leftist Heap**: Balances based on null path length (NPL).
   - **Skew Heap**: Balances using subtree swapping.

3. **Customizable Priority Functions**:
   - Priority can be determined by various criteria such as academic level, major, and demographics.

4. **Dynamic Reconstruction**:
   - Rebuild heaps when priority functions or heap structures change.

### Testing Framework
1. Validates heap properties such as:
   - Min-Heap and Max-Heap priority rules.
   - Leftist Heap NPL consistency.
2. Checks copy constructor, assignment operator, and merging behavior.

## Classes and Functions

### `Student` Class
Represents student data with attributes such as name, academic level, major, and demographic details.
- **Attributes**:
  - `Name`: Name of the student.
  - `Level`: Freshman, Sophomore, Junior, or Senior.
  - `Major`: Bioinformatics, Engineering, Computer Science, etc.
  - `Group`: Regular, Honors, Research, etc.
  - `Race`, `Gender`, `Income`, `Highschool`: Demographic attributes.
- **Methods**:
  - Accessors for all attributes.
  - Overloaded operators (`<<`, `=`) for output and assignment.

### `Node` Class
Represents a node in the heap with a `Student` object and pointers to left and right subtrees.

### `RQueue` Class
Implements the priority queue with skew and leftist heaps.
- **Constructors**:
  - `RQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)`.
- **Core Methods**:
  - `insertStudent(const Student& student)`: Inserts a student into the queue.
  - `getNextStudent()`: Retrieves and removes the highest-priority student.
  - `mergeWithQueue(RQueue& rhs)`: Merges two queues.
  - `setPriorityFn(prifn_t priFn, HEAPTYPE heapType)`: Changes priority function and heap type.
  - `setStructure(STRUCTURE structure)`: Changes heap structure.
  - `clear()`: Clears the queue.
  - `dump()`: Displays the heap for debugging.

## Installation and Execution

1. Compile the program:
   ```bash
   g++ -o mytest mytest.cpp rqueue.cpp
   ```
2. Run the program:
   ```bash
   ./mytest
   ```

## Testing

### Example Tests
1. **Heap Properties**:
   - Verifies Min-Heap and Max-Heap rules.
   - Ensures leftist heaps maintain correct NPL values.
2. **Queue Operations**:
   - Validates insertion, removal, and merging of queues.
   - Tests behavior with empty and non-empty queues.
3. **Copy Constructor and Assignment Operator**:
   - Ensures deep copies of queues.
4. **Priority Function Changes**:
   - Rebuilds heaps correctly after priority function updates.

## Example Code

### Inserting Students
```cpp
Student student("John Doe", FRESH, CSC, REGU, MAJORITY, MALE, TIER2, HIGH);
RQueue queue(priorityFn1, MINHEAP, LEFTIST);
queue.insertStudent(student);
```

### Retrieving the Highest-Priority Student
```cpp
Student next = queue.getNextStudent();
cout << next << endl;
```

### Merging Queues
```cpp
RQueue queue1(priorityFn1, MINHEAP, LEFTIST);
RQueue queue2(priorityFn1, MINHEAP, LEFTIST);
queue1.mergeWithQueue(queue2);
```

## Dependencies
- **Standard C++ Libraries**:
  - `<iostream>`
  - `<stdexcept>`
  - `<string>`

## Authors
- **CMSC 341 - Spring 2024**

## License
This project is for educational purposes and does not include warranties or guarantees.
