//#pragma once
//
//#ifndef DOUBLYLINKEDLIST_HPP
//#define DOUBLYLINKEDLIST_HPP
//
//#include <iostream>
//#include <string>
//
//struct Node {
//    std::string word;
//    Node* next;
//
//    Node(const std::string& word) : word(word), next(nullptr) {}
//};
//
//class LinkedList {
//private:
//    Node* head;
//    size_t size;
//
//public:
//    LinkedList() : head(nullptr), size(0) {}
//
//    ~LinkedList() {
//        Node* current = head;
//        while (current) {
//            Node* next = current->next;
//            delete current;
//            current = next;
//        }
//    }
//
//    void append(const std::string& word) {
//        Node* newNode = new Node(word);
//        if (!head) {
//            head = newNode;
//        }
//        else {
//            Node* current = head;
//            while (current->next) {
//                current = current->next;
//            }
//            current->next = newNode;
//        }
//        size++;
//    }
//
//    bool contains(const std::string& word) const {
//        Node* current = head;
//        while (current) {
//            if (current->word == word) {
//                return true;
//            }
//            current = current->next;
//        }
//        return false;
//    }
//
//    size_t getSize() const {
//        return size;
//    }
//};
//
//#endif // LINKED_LIST_H
