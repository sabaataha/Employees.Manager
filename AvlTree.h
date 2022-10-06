#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H

#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include "GenericNode.h"

using namespace std;

template<class T, class Comp, class Equal>
class AVLtree {
public:
    typedef GenericNode<T> *Node;
    Node max;
    int num_of_nodes;
    Node root;
    Comp compare;
    Equal equal;

    AVLtree() : max(nullptr), num_of_nodes(0), root(nullptr) {};

    AVLtree(Node max, int num_of_nodes, Node root) : max(max), num_of_nodes(num_of_nodes), root(root) {};

    ~AVLtree() {
        clear(root);
    }

    AVLtree(T arr[], int size) {
        root = sortedArrayToAVL(arr, 0, size - 1);
        max = maximumNode(root);
        num_of_nodes = size;
    }

    void connectSubTreeToTree(Node originalRoot, Node newRoot) {
        if (originalRoot->father == NULL) {
            root = newRoot;
        }

        if (originalRoot->isLeftSon()) {
            originalRoot->father->left_son = newRoot;
        }

        if (originalRoot->isRightSon()) {
            originalRoot->father->right_son = newRoot;
        }

        newRoot->father = originalRoot->father;
        originalRoot->father = newRoot;
    }

    void rollLeftLeft(Node currRoot) {
        Node originalRoot = currRoot;
        Node originalRootLeftTree = currRoot->left_son;
        Node originalRootLeftTreeRightSon = originalRootLeftTree->right_son;

        connectSubTreeToTree(originalRoot, originalRootLeftTree);
        currRoot = originalRootLeftTree;
        currRoot->right_son = originalRoot;
        currRoot->right_son->left_son = originalRootLeftTreeRightSon;
        if (currRoot->right_son->left_son != NULL) {
            currRoot->right_son->left_son->father = currRoot->right_son;
        }

        currRoot->right_son->updateHeight();
        currRoot->updateHeight();
    }

    void rollLeftRight(Node currRoot) {
        Node originalRoot = currRoot;
        Node originalRootLeftTree = currRoot->left_son;
        Node originalRootLeftTreeRightSon = originalRootLeftTree->right_son;
        Node originalRootLeftTreeRightSonLeftSon = originalRootLeftTreeRightSon->left_son;
        Node originalRootLeftTreeRightSonRightSon = originalRootLeftTreeRightSon->right_son;

        connectSubTreeToTree(originalRoot, originalRootLeftTreeRightSon);
        currRoot = originalRootLeftTreeRightSon;
        currRoot->right_son = originalRoot;
        currRoot->right_son->left_son = originalRootLeftTreeRightSonRightSon;
        if (currRoot->right_son->left_son != NULL) {
            currRoot->right_son->left_son->father = currRoot->right_son;
        }
        currRoot->left_son = originalRootLeftTree;
        if (currRoot->left_son != NULL) {
            currRoot->left_son->father = currRoot;
        }
        currRoot->left_son->right_son = originalRootLeftTreeRightSonLeftSon;
        if (currRoot->left_son->right_son != NULL) {
            currRoot->left_son->right_son->father = currRoot->left_son;
        }

        currRoot->right_son->updateHeight();
        currRoot->left_son->updateHeight();
        currRoot->updateHeight();
    }

    void rollRightRight(Node currRoot) {
        Node originalRoot = currRoot;
        Node originalRootRightTree = currRoot->right_son;
        Node originalRootRightTreeLeftSon = originalRootRightTree->left_son;

        connectSubTreeToTree(originalRoot, originalRootRightTree);
        currRoot = originalRootRightTree;
        currRoot->left_son = originalRoot;
        currRoot->left_son->right_son = originalRootRightTreeLeftSon;
        if (currRoot->left_son->right_son != NULL) {
            currRoot->left_son->right_son->father = currRoot->left_son;
        }

        currRoot->left_son->updateHeight();
        currRoot->updateHeight();
    }

    void rollRightLeft(Node currRoot) {
        Node originalRoot = currRoot;
        Node originalRootRightTree = currRoot->right_son;
        Node originalRootRightTreeLeftSon = originalRootRightTree->left_son;
        Node originalRootRightTreeLeftSonRightSon = originalRootRightTreeLeftSon->right_son;
        Node originalRootRightTreeLeftSonLeftSon = originalRootRightTreeLeftSon->left_son;

        connectSubTreeToTree(originalRoot, originalRootRightTreeLeftSon);
        currRoot = originalRootRightTreeLeftSon;
        currRoot->left_son = originalRoot;
        currRoot->left_son->right_son = originalRootRightTreeLeftSonLeftSon;
        if (currRoot->left_son->right_son != NULL) {
            currRoot->left_son->right_son->father = currRoot->left_son;
        }
        currRoot->right_son = originalRootRightTree;
        if (currRoot->right_son) {
            currRoot->right_son->father = currRoot;
        }
        currRoot->right_son->left_son = originalRootRightTreeLeftSonRightSon;
        if (currRoot->right_son->left_son != NULL) {
            currRoot->right_son->left_son->father = currRoot->right_son;
        }

        currRoot->right_son->updateHeight();
        currRoot->left_son->updateHeight();
        currRoot->updateHeight();
    }

    void roll(Node node) {
        if (node->NodeGetBF() == 2) {
            if (node->nodeGetLeft()->NodeGetBF() >= 0)
                rollLeftLeft(node);
            else if (node->nodeGetLeft()->NodeGetBF() == -1)
                rollLeftRight(node);
        } else if (node->NodeGetBF() == -2) {
            if (node->nodeGetRight()->NodeGetBF() <= 0)
                rollRightRight(node);
            else if (node->nodeGetRight()->NodeGetBF() == 1)
                rollRightLeft(node);
        }
    }

    void clear(Node &root) {
        if (root == nullptr)
            return;
        if (root != nullptr) {
            clear(root->left_son);
            clear(root->right_son);
            delete root;
        }
        max = nullptr;
        root = nullptr;
        num_of_nodes = 0;
    }

    void clear_pointer(Node &root) {
        if (root == NULL) {
            return;
        }
        if (root->left_son) {
            clear_pointer(root->left_son);
        }
        if (root->right_son) {
            clear_pointer(root->right_son);
        }
        delete root;
        max = NULL;
        root = NULL;
        num_of_nodes = 0;
    }

    T *findData(const T &data) {
        if (findNode(data) == nullptr) {
            return nullptr;
        }
        return findNode(data)->data;
    }

    Node findNode(const T &data) {
        return findNodeHelper(this->root, data);
    }

    Node findNodeHelper(Node node, const T &data) {
        if (node == nullptr)
            return nullptr;
        if (equal(*(node->data), data)) {
            return node;
        } else {
            if (compare(*(node->data), data)) {
                return findNodeHelper(node->right_son, data);
            } else {
                return findNodeHelper(node->left_son, data);
            }
        }
    }

    void insert(T data) {
        if (&data == nullptr) {
            return;
        }

        if (findNode(data) != nullptr) {
            throw NodeAlreadyExists();
            return;
        }
        Node node = nullptr;
        node = node->createNode(data);

        insertHelper(node, root);
        max = maximumNode(root);
        num_of_nodes++;
    }

    void insertHelper(Node nodeToAdd, Node currRoot) {
        //the tree is empty
        if (currRoot == nullptr) {
            nodeToAdd->father = nullptr;
            root = nodeToAdd;
            root->height = 0;
            return;
        }

        //add the node to the left sub-tree if data is smaller
        //returns true if the first is smaller
        if (compare(*(nodeToAdd->data), *(currRoot->data))) {
            if (currRoot->left_son != nullptr) {
                insertHelper(nodeToAdd, currRoot->left_son);
            } else {
                currRoot->left_son = nodeToAdd;
                nodeToAdd->father = currRoot;
            }
        }

            //add the node to the right sub-tree if data is bigger
        else {
            if (currRoot->right_son != nullptr) {
                insertHelper(nodeToAdd, currRoot->right_son);
            } else {
                currRoot->right_son = nodeToAdd;
                nodeToAdd->father = currRoot;
            }
        }
        currRoot->updateHeight();
        if (!currRoot->isValidBF()) {
            roll(currRoot);
        }
    }

    void remove(const T &data) {
        if (&data == nullptr || num_of_nodes == 0)
            return;

        if (num_of_nodes == 0)
            return;
        Node node = findNode(data);
        if (node == nullptr) {
            throw NodeDoesntExists();
            return;
        }
        removeHelper(node, root);
        num_of_nodes--;
        if (num_of_nodes != 0) {
            max = maximumNode(root);
        } else {
            max = nullptr;
        }
    }

    void removeHelper(Node node_to_delete, Node currNode) {
        if (equal(*(node_to_delete->data), *(currNode->data))) {
            if (currNode->left_son == nullptr && currNode->right_son == nullptr) {
                Node to_remove = currNode;
                currNode = currNode->father;
                if (to_remove->isLeftSon()) {
                    currNode->left_son = nullptr;
                }
                if (to_remove->isRightSon()) {
                    currNode->right_son = nullptr;
                }
                delete to_remove;
                if (num_of_nodes == 1) {
                    this->root = nullptr;
                }
            }

                // the root has only left son
            else if (currNode->left_son && currNode->right_son == nullptr) {
                Node left = currNode->left_son;
                delete currNode->data;
                currNode->data = new T(*(left->data));
                currNode->left_son = left->left_son;
                currNode->right_son = left->right_son;
                delete left;
            }

                // the root has only right son
            else if (currNode->left_son == nullptr && currNode->right_son) {
                Node right = currNode->right_son;
                delete currNode->data;
                currNode->data = new T(*(right->data));
                currNode->left_son = right->left_son;
                currNode->right_son = right->right_son;
                delete right;
            } else {
                // left and right exists
                Node min_node = getMinNode(currNode->right_son);
                delete currNode->data;
                currNode->data = new T(*(min_node->data));
                removeHelper(min_node, currNode->right_son);

            }
        } else if (compare(*(node_to_delete->data), *(currNode->data))) {

            removeHelper(node_to_delete, currNode->left_son);
        } else {
            removeHelper(node_to_delete, currNode->right_son);
        }
        if (num_of_nodes - 1 != 0) {
            currNode->updateHeight();
            if (!currNode->isValidBF()) {
                roll(currNode);
            }
        }
    }

    GenericNode<T> *maximumNode(Node node) {
        if (node == NULL) {
            return NULL;
        }
        if (node->right_son == NULL) {
            return node;
        }
        return maximumNode(node->right_son);
    }

    GenericNode<T> *getMinNode(Node node) {
        if (node == NULL) {
            return NULL;
        }
        if (node->left_son == NULL) {
            return node;
        }
        return getMinNode(node->left_son);
    }

    void preorder(Node root) {
        if (root != nullptr) {
            cout << *(root->data) << " ";
            preorder(root->left_son);
            preorder(root->right_son);
        }
    }

    void inorder(Node root) {
        if (root != nullptr) {
            inorder(root->left_son);
            std::cout << *(root->data) << " ";
            inorder(root->right_son);
        }
    }

    void postorder(Node root) {
        if (root != nullptr) {
            postorder(root->left_son);
            postorder(root->right_son);
            std::cout << *(root->data) << " ";
        }
    }

    T *maxData() {
        if (max == nullptr) {
            return nullptr;
        }
        return max->data;
    }

    void storeInOrderToArray(T inorder[]) {
        int i = 0;
        storeInorderNodes(root, inorder, &i);
    }

    void storeInorderNodes(Node node, T inorder[], int *index) {
        if (node == nullptr)
            return;
        storeInorderNodes(node->left_son, inorder, index);
        inorder[*index] = *(node->data);
        (*index)++;
        storeInorderNodes(node->right_son, inorder, index);
    }

    void storeInOrderToArray2(T inorder[], int size) {
        int i = 0;
        storeInorderNodes2(root, inorder, &i, size);
    }

    void storeInorderNodes2(Node node, T inorder[], int *index, int size) {
        if (node == nullptr)
            return;
        storeInorderNodes2(node->left_son, inorder, index, size);
        if (*index < size) {
            inorder[*index] = *(node->data);
            (*index)++;
        }
        storeInorderNodes2(node->right_son, inorder, index, size);
    }

    Node sortedArrayToAVL(T arr[], int start, int end) {
        if (start > end) {
            return nullptr;
        }

        int mid = (start + end) / 2;
        Node node = nullptr;
        node = node->createNode(arr[mid]);

        node->left_son = sortedArrayToAVL(arr, start, mid - 1);
        node->right_son = sortedArrayToAVL(arr, mid + 1, end);
        if (node->left_son != nullptr)
            node->left_son->father = node;
        if (node->right_son != nullptr)
            node->right_son->father = node;
        node->updateHeight();
        return node;
    }

    void avlToSortedArrayBackwards(T inorder[], int size) {
        int i = size;
        avlToSortedArrayBackwardsHelper(root, inorder, &i);
    }

    void avlToSortedArrayBackwardsHelper(Node currRoot, T arr[], int *index) {
        if (currRoot != nullptr) {
            avlToSortedArrayBackwardsHelper(currRoot->left_son, arr, index);
            arr[*index] = *(currRoot->data);
            (*index)--;
            avlToSortedArrayBackwardsHelper(currRoot->right_son, arr, index);
        }
    }

    int getNodesNumInRange(Node root, const T &low, const T &high) {
        // Base case
        if (!root) return 0;

        if (equal(*root->data, high) && equal(*root->data, low))
            return 1;

        if (!compare(high, *(root->data)) && !compare(*(root->data), low))
            return 1 + getNodesNumInRange(root->left_son, low, high) +
                   getNodesNumInRange(root->right_son, low, high);


        else if (compare(*(root->data), low))
            return getNodesNumInRange(root->right_son, low, high);

        else return getNodesNumInRange(root->left_son, low, high);
    }

    void getNodesInRangeToArr(Node root, const T &low, const T &high, T arr[], int *index) {
        if (!root) return;

        if (equal(*root->data, high) && equal(*root->data, low)) {
            arr[*index] = *(root->data);
        }
        if (!compare(high, *(root->data)) && !compare(*(root->data), low)) {
            arr[*index] = *(root->data);
            (*index)--;
            getNodesInRangeToArr(root->left_son, low, high, arr, index);
            getNodesInRangeToArr(root->right_son, low, high, arr, index);
        } else if (compare(*(root->data), low))
            getNodesInRangeToArr(root->right_son, low, high, arr, index);
        else
            getNodesInRangeToArr(root->left_son, low, high, arr, index);
    }

    void getArr(const T &low, const T &high, T arr[]) {
        int numOfNodes = getNodesNumInRange(this->root, low, high);
        //T** arr= new T*[numOfNodes];
        int newNum = numOfNodes - 1;
        int *size = &newNum;
        getNodesInRangeToArr(this->root, low, high, arr, size);
    }
};


#endif //WET1_AVLTREE_H