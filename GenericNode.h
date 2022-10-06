#ifndef WET1_GENERICNODE_H
#define WET1_GENERICNODE_H

#include "exceptions.h"

using namespace std;

template<class T>
class GenericNode {
public:
    T *data;
    GenericNode *father;
    GenericNode *left_son;
    GenericNode *right_son;
    int height;
    int bf;

    GenericNode() : data(nullptr), father(nullptr), left_son(nullptr), right_son(nullptr), height(0), bf(0) {}

    GenericNode(const GenericNode &copyNode) = default;

    ~GenericNode() {
        delete data;
    }


    GenericNode<T> *createNode(T val) {
        GenericNode<T> *node = new GenericNode<T>;
        node->data = new T(val);
        node->height = 0;
        node->left_son = nullptr;
        node->right_son = nullptr;
        node->father = nullptr;
        return node;
    }


    GenericNode &operator=(const GenericNode &node) {
        if (this == &node) {
            return *this;
        }
        data = node->data;
        height = node->height;
        left_son = node->left_son;
        right_son = node->right_son;
        father = node->father;

        return *this;
    }


    bool operator==(const T &data1) {
        if (data == data1) {
            return true;
        }
        return false;
    }

    bool operator>(const T &data1) {
        if (data > data1) {
            return true;
        }
        return false;
    }

    void updateHeight() {
        if (this == nullptr) {
            return;
        }
        int rightH, leftH;
        //calc right son height
        if (this->right_son == nullptr) {
            rightH = -1;
        } else {
            rightH = right_son->height;
        }
        //calc left son height
        if (this->left_son == nullptr) {
            leftH = -1;
        } else {
            leftH = left_son->height;
        }
        //updates the height
        if (rightH > leftH) {
            this->height = rightH + 1;
        } else {
            this->height = leftH + 1;
        }
    }

    int NodeGetBF() const {
        if (this == nullptr)
            return 0;
        if (this->left_son && this->right_son)
            return (left_son->height - right_son->height);
        if (this->left_son && this->right_son == nullptr)
            return this->left_son->height + 1;
        if (this->left_son == nullptr && this->right_son)
            return -1 - (this->right_son->height);
        return 0;
    }

    GenericNode<T> *nodeGetRight() {
        if (this == nullptr)
            return nullptr;
        return this->right_son;
    }

    GenericNode<T> *nodeGetLeft() {
        if (this == nullptr)
            return nullptr;
        return this->left_son;
    }


    //returns if the balance factor of the node is valid
    bool isValidBF() {
        return (this->NodeGetBF() < 2 && this->NodeGetBF() > -2);
    }

    bool isLeftSon() {
        if (father == nullptr) {
            return false;
        }
        return father->left_son == this;
    }

    bool isRightSon() {
        if (father == nullptr) {
            return false;
        }
        return father->right_son == this;
    }
};


#endif //WET1_GENERICNODE_H
