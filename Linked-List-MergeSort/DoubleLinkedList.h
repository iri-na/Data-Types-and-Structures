#pragma once

#include <stdexcept> // for std::runtime_error
#include <iostream> // for std::cerr, std::cout
#include <ostream> // for std::ostream

// Header

template <typename T>
class LinkedList {
public:

    class Node {
    public:
        Node* next;
        Node* prev;
        T data;

        Node() : next(nullptr), prev(nullptr) {}
        Node(const T& dataArg) : next(nullptr), prev(nullptr), data(dataArg) {}

        Node(const Node& other) : next(other.next), prev(other.prev), data(other.data) {}

        Node& operator=(const Node& other) {
            next = other.next;
            prev = other.prev;
            data = other.data;
            return *this;
        }

        ~Node() {}
    };

private:
    Node* head_;
    Node* tail_;
    int size_;

public:
    Node* getHeadPtr() { return head_; }
    Node* getTailPtr() { return tail_; }

    int size() const { return size_; }
    bool empty() const { return !head_; }

    T& front(){
        if (!head_) {
            throw std::runtime_error("front() called on empty LinkedList");
        }
        else {
            return head_->data;
        }
    }
    const T& front() const {
        if (!head_) {
            throw std::runtime_error("front() called on empty LinkedList");
        }
        else {
            return head_->data;
        }
    }

    T& back(){
        if (!tail_) {
            throw std::runtime_error("back() called on empty LinkedList");
        }
        else {
            return tail_->data;
        }
    }
    const T& back() const {
        if (!tail_) {
            throw std::runtime_error("back() called on empty LinkedList");
        }
        else {
            return tail_->data;
        }
    }

    void pushFront(const T& newData);
    void pushBack(const T& newData);
    void popFront();
    void popBack();

    // Delete all items in the list
    void clear() {
        // As long as there are items left in the list, remove the tail item.
        while (head_) {
            popBack();
        }

        if (0 != size_) throw std::runtime_error(std::string("Error in clear()"));
    }

    bool equals(const LinkedList<T>& other) const;
    bool operator==(const LinkedList<T>& other) const {
        return equals(other);
    }
    bool operator!=(const LinkedList<T>& other) const {
        return !equals(other);
    }

    std::ostream& print(std::ostream& os) const;

    void insertOrdered(const T& newData);
    bool isSorted() const;

    LinkedList<LinkedList<T>> splitHalves() const;
    LinkedList<LinkedList<T>> explode() const;
    LinkedList<T> merge(const LinkedList<T>& other) const;
    LinkedList<T> mergeSort() const;
    LinkedList<T> mergeSortRecursive() const;
    LinkedList<T> mergeSortIterative() const;

    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    LinkedList(const LinkedList<T>& other) : LinkedList() {
        *this = other;
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        // Clear the current list.
        clear();

        const Node* cur = other.head_;
        while (cur) {
            pushBack(cur->data);
            cur = cur->next;
        }

        return *this;
    }

    ~LinkedList() {
        clear();
    }
};

///////////////////////// Implementation ////////////////////////////////////

template <typename T>
void LinkedList<T>::pushFront(const T& newData) {
    Node* newNode = new Node(newData);

    if (!head_) {
        head_ = newNode;
        tail_ = newNode;
    }
    else {
        Node* oldHead = head_;
        oldHead->prev = newNode;
        newNode->next = oldHead;
        head_ = newNode;
    }

    size_++;
}

template <typename T>
void LinkedList<T>::pushBack(const T& newData) {
    Node* newNode = new Node(newData);

    if (!head_) {
        head_ = newNode;
        tail_ = newNode;
    }
    else {
        Node* oldTail = tail_;
        oldTail->next = newNode;
        newNode->prev = oldTail;
        tail_ = newNode;
    }

    size_++;
}

template <typename T>
void LinkedList<T>::popFront() {

    if (!head_) return;

    if (!head_->next) {
        delete head_;
        head_ = nullptr;
        tail_ = nullptr;
        size_--;
        if (0 != size_) throw std::runtime_error(std::string("Error in popFront()"));
        return;
    }

    Node* oldHead = head_;
    head_ = head_->next;
    head_->prev = nullptr;
    delete oldHead;
    oldHead = nullptr;
    size_--;
}

template <typename T>
void LinkedList<T>::popBack() {

    if (!head_) return;

    if (!tail_->prev) {
        delete tail_;
        head_ = nullptr;
        tail_ = nullptr;
        size_--;
        if (0 != size_) throw std::runtime_error(std::string("Error in popBack()"));
        return;
    }

    Node* oldTail = tail_;
    tail_ = tail_->prev;
    tail_->next = nullptr;
    delete oldTail;
    oldTail = nullptr;
    size_--;
}

template <typename T>
bool LinkedList<T>::equals(const LinkedList<T>& other) const {

    if (size_ != other.size_) {
        return false;
    }

    const Node* thisCur = head_;
    const Node* otherCur = other.head_;

    while (thisCur) {
        if (!otherCur) {
            throw std::runtime_error(std::string("Error in equals: ") + "other missing a node or size is wrong");
        }
        if (thisCur->data != otherCur->data) {
            return false;
        }
        thisCur = thisCur->next;
        otherCur = otherCur->next;
    }

    return true;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
    return list.print(os);
}

template <typename T>
std::ostream& LinkedList<T>::print(std::ostream& os) const {
    // List format will be [(1)(2)(3)], etc.
    os << "[";

    // Note that this works correctly for an empty list.
    Node* cur = head_;
    while (cur) {
        os << "(" << cur->data << ")";
        cur = cur->next;
    }

    os << "]";

    return os;
}

template <typename T>
void LinkedList<T>::insertOrdered(const T& newData) {

    Node* newp = new Node(newData);
    Node* cur = head_;

    if (!head_) {
        if (0 != size_) throw std::runtime_error(std::string("Error in insertOrdered: wrong list size"));
        head_ = newp;
        tail_ = newp;
        size_++;
        return;
    }

    if (newp->data <= head_->data) {
        cur->prev = newp;
        newp->next = cur;
        head_ = newp;
        size_++;
        return;
    }

    for (cur = head_->next; cur != nullptr; cur = cur->next) {
        if (newp->data <= cur->data) {
            newp->prev = cur->prev;
            cur->prev->next = newp;
            cur->prev = newp;
            newp->next = cur;
            size_++;
            return;
        }
    }

    if (!cur) {
        tail_->next = newp;
        newp->prev = tail_;
        tail_ = newp;
        size_++;
        return;
    }
}

template <typename T>
bool LinkedList<T>::isSorted() const {
    if (size_ < 2) return true;

    if (!head_) throw std::runtime_error(std::string("Error in isSorted()"));

    const Node* prev = nullptr;
    const Node* cur = head_;
    while (cur->next) {
        prev = cur;
        cur = cur->next;
        if (!(prev->data <= cur->data)) {
            return false;
        }
    }

    return true;
}

template <typename T>
LinkedList<LinkedList<T>> LinkedList<T>::splitHalves() const {
    LinkedList<LinkedList<T>> halves;
    LinkedList<T> leftHalf = *this;
    LinkedList<T> rightHalf;

    if (size_ < 2) {
        halves.pushBack(leftHalf);
        halves.pushBack(rightHalf);
        return halves;
    }

    int rightHalfLength = size_ / 2;
    for (int i=0; i<rightHalfLength; i++) {
        T dataToCopy = leftHalf.back();
        rightHalf.pushFront(dataToCopy);
        leftHalf.popBack();
    }

    halves.pushBack(leftHalf);
    halves.pushBack(rightHalf);

    return halves;
}

template <typename T>
LinkedList<LinkedList<T>> LinkedList<T>::explode() const {
    LinkedList<T> workingCopy = *this;
    LinkedList< LinkedList<T> > lists;

    while (!workingCopy.empty()) {
        LinkedList<T> singletonList;
        singletonList.pushBack(workingCopy.front());
        workingCopy.popFront();
        lists.pushBack(singletonList);
    }

    return lists;
}

template <typename T>
LinkedList<T> LinkedList<T>::merge(const LinkedList<T>& other) const {
    LinkedList<T> left = *this;
    LinkedList<T> right = other;
    LinkedList<T> merged;

    Node* templ = left.head_;
    Node* tempr = right.head_;

    if (left.empty()) {
        merged = right;
    }
    else if (right.empty()) {
        merged = left;
    }

    else {
        while (templ || tempr) {
            if (!templ) {
                merged.pushBack(tempr->data);
                tempr = tempr->next;
            }
            else if (!tempr) {
                merged.pushBack(templ->data);
                templ = templ->next;
            }
            else if (templ->data <= tempr->data) {
                merged.pushBack(templ->data);
                templ = templ->next;
            }
            else {
                merged.pushBack(tempr->data);
                tempr = tempr->next;
            }
        }
    }

    return merged;
}

template <typename T>
LinkedList<T> LinkedList<T>::mergeSort() const {
    return mergeSortRecursive();
    // return mergeSortIterative();
}

template <typename T>
LinkedList<T> LinkedList<T>::mergeSortRecursive() const {
    if (size_ < 2) {
        return *this;
    }

    LinkedList<LinkedList<T>> halves = splitHalves();
    LinkedList<T>& left = halves.front();
    LinkedList<T>& right = halves.back();

    left = left.mergeSortRecursive();
    right = right.mergeSortRecursive();

    return left.merge(right);
}

template <typename T>
LinkedList<T> LinkedList<T>::mergeSortIterative() const {
    if (size_ < 2) {
        return *this;
    }

    LinkedList< LinkedList<T> > workQueue = explode();

    while (workQueue.size() > 1) {
        LinkedList<T> left = workQueue.front();
        workQueue.popFront();
        LinkedList<T> right = workQueue.front();
        workQueue.popFront();

        LinkedList<T> merged = left.merge(right);
        workQueue.pushBack(merged);
    }
    return workQueue.front();
}