#pragma once
/*=======================================================================
 * DataStructures.h — Custom Container Implementations
 * OceanRoute Nav — Maritime Navigation Optimizer
 * 
 * ACADEMIC CONSTRAINT: No STL containers allowed.
 * These custom implementations replace std::vector, std::queue,
 * and std::priority_queue respectively.
 * 
 * Authors: Shahzad Ahmad (24i-3090), Syed Muhammad Rayyan Hassan (24i-3000)
 *=======================================================================*/

#include <iostream>
#include <string>
using namespace std;

// ─────────────────────────────────────────────────────────────────────
// Vector<T> — Dynamic Array (replaces std::vector)
// Amortized O(1) push_back, O(1) random access
// ─────────────────────────────────────────────────────────────────────
template <typename T>
class Vector {
private:
    T* arr;
    unsigned int size, capacity;

public:
    /// Default constructor — starts with capacity 2
    Vector() {
        size = 0;
        capacity = 2;
        arr = new T[capacity];
    }

    /// Copy constructor — deep copy
    Vector(const Vector& other) {
        size = other.size;
        capacity = other.capacity;
        arr = new T[capacity];
        for (unsigned int i = 0; i < size; i++)
            arr[i] = other.arr[i];
    }

    /// Destructor — frees heap memory
    ~Vector() {
        delete[] arr;
    }

    /// Copy assignment operator — deep copy with self-check
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] arr;
            capacity = other.capacity;
            size = other.size;
            arr = new T[capacity];
            for (unsigned int i = 0; i < size; i++)
                arr[i] = other.arr[i];
        }
        return *this;
    }

    /// Bounds-checked element access
    T& operator[](int index) {
        if (index < 0 || index >= (int)size) {
            cout << "Index out of range! (" << index << " / " << size << ")" << endl;
            exit(1);
        }
        return arr[index];
    }

    /// Const bounds-checked element access
    const T& operator[](int index) const {
        if (index < 0 || index >= (int)size) {
            cout << "Index out of range! (" << index << " / " << size << ")" << endl;
            exit(1);
        }
        return arr[index];
    }

    /// Append element — doubles capacity when full (amortized O(1))
    void push_back(T value) {
        if (size == capacity) {
            capacity *= 2;
            T* temp = new T[capacity];
            for (unsigned int i = 0; i < size; i++)
                temp[i] = arr[i];
            delete[] arr;
            arr = temp;
        }
        arr[size++] = value;
    }

    /// Remove last element
    void pop_back() {
        if (size == 0) {
            cout << "Vector is empty! Cannot pop.\n";
            exit(1);
        }
        size--;
    }

    /// Number of elements currently stored
    int getSize() const {
        return (int)size;
    }

    /// Check if empty
    bool empty() const {
        return size == 0;
    }

    /// Reset size to 0 (does not free memory)
    void clear() {
        size = 0;
    }

    /// Iterator support — pointer to first element
    T* begin() { return arr; }
    const T* begin() const { return arr; }

    /// Iterator support — pointer past last element
    T* end() { return arr + size; }
    const T* end() const { return arr + size; }

    /// Debug display
    void display() const {
        for (unsigned int i = 0; i < size; i++)
            cout << arr[i] << " ";
        cout << endl;
    }
};

// ─────────────────────────────────────────────────────────────────────
// Queue<T> — Circular Array Queue (replaces std::queue)
// O(1) enqueue and dequeue
// ─────────────────────────────────────────────────────────────────────
template <typename T>
class Queue {
private:
    T* arr;
    int front, rear, capacity, size;

public:
    /// Default constructor — initial capacity 10
    Queue() {
        capacity = 10;
        arr = new T[capacity];
        front = 0;
        rear = -1;
        size = 0;
    }

    /// Copy constructor
    Queue(const Queue& other) {
        capacity = other.capacity;
        size = other.size;
        front = other.front;
        rear = other.rear;
        arr = new T[capacity];
        for (int i = 0; i < capacity; i++)
            arr[i] = other.arr[i];
    }

    /// Copy assignment operator
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            delete[] arr;
            capacity = other.capacity;
            size = other.size;
            front = other.front;
            rear = other.rear;
            arr = new T[capacity];
            for (int i = 0; i < capacity; i++)
                arr[i] = other.arr[i];
        }
        return *this;
    }

    /// Destructor
    ~Queue() {
        delete[] arr;
    }

    /// Add element to rear — doubles capacity when full
    void enqueue(T value) {
        if (size == capacity) {
            int oldCapacity = capacity;
            capacity *= 2;
            T* temp = new T[capacity];
            for (int i = 0; i < size; i++)
                temp[i] = arr[(front + i) % oldCapacity];
            delete[] arr;
            arr = temp;
            front = 0;
            rear = size - 1;
        }
        rear = (rear + 1) % capacity;
        arr[rear] = value;
        size++;
    }

    /// Remove and return element from front
    T dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty!" << endl;
            exit(1);
        }
        T value = arr[front];
        front = (front + 1) % capacity;
        size--;
        return value;
    }

    /// View front element without removing
    T peek() {
        if (isEmpty()) {
            cout << "Queue is empty!" << endl;
            exit(1);
        }
        return arr[front];
    }

    /// Check if queue has no elements
    bool isEmpty() const {
        return size == 0;
    }

    /// Number of elements in queue
    int getSize() const {
        return size;
    }
};

// ─────────────────────────────────────────────────────────────────────
// HeapNode — Element stored in MinHeap
// ─────────────────────────────────────────────────────────────────────
struct HeapNode {
    int priority;   ///< Sort key (lower = higher priority)
    int index;      ///< Associated graph node index
};

// ─────────────────────────────────────────────────────────────────────
// MinHeap — Binary Min-Heap (replaces std::priority_queue)
// O(log n) push/pop, used for Dijkstra and A* algorithms
// ─────────────────────────────────────────────────────────────────────
class MinHeap {
private:
    Vector<HeapNode> arr;

    /// Restore heap property downward from index i
    void heapifyDown(int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < arr.getSize() && arr[left].priority < arr[smallest].priority)
            smallest = left;
        if (right < arr.getSize() && arr[right].priority < arr[smallest].priority)
            smallest = right;
        if (smallest != i) {
            HeapNode temp = arr[i];
            arr[i] = arr[smallest];
            arr[smallest] = temp;
            heapifyDown(smallest);
        }
    }

    /// Restore heap property upward from index i
    void heapifyUp(int i) {
        int parent = (i - 1) / 2;
        while (i > 0 && arr[i].priority < arr[parent].priority) {
            HeapNode temp = arr[i];
            arr[i] = arr[parent];
            arr[parent] = temp;
            i = parent;
            parent = (i - 1) / 2;
        }
    }

public:
    /// Check if heap is empty
    bool empty() {
        return arr.getSize() == 0;
    }

    /// Insert node maintaining heap property
    void push(HeapNode node) {
        arr.push_back(node);
        heapifyUp(arr.getSize() - 1);
    }

    /// Remove and return minimum element
    HeapNode pop() {
        if (arr.getSize() == 0) {
            cout << "Heap is empty!!\n";
            exit(1);
        }
        HeapNode root = arr[0];
        arr[0] = arr[arr.getSize() - 1];
        arr.pop_back();
        if (!empty())
            heapifyDown(0);
        return root;
    }

    /// View minimum element without removing
    HeapNode top() {
        return arr[0];
    }
};
