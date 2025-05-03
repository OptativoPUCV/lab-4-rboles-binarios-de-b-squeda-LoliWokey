#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL) return;
    
    TreeNode* parent = NULL;
    TreeNode* current = tree->root;
    
    while (current != NULL) {
        parent = current;
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current;
            return;
        }
        else if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    TreeNode* new_node = createTreeNode(key, value);
    if (new_node == NULL) return;
    
    new_node->parent = parent;
    
    if (parent == NULL) {
        tree->root = new_node;
    } else if (tree->lower_than(key, parent->pair->key)) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
    
    tree->current = new_node;
}

TreeNode * minimum(TreeNode * x){
    
    TreeNode* minimo = x;
    
    while (minimo->left != NULL) {
        minimo = minimo->left;
    }
    
    return minimo;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            tree->root = NULL;
        } else {
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        }
        free(node->pair);
        free(node);
    }
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* hijo = (node->left != NULL) ? node->left : node->right;
        
        hijo->parent = node->parent;
        
        if (node->parent == NULL) {
            tree->root = hijo;
        } else {
            if (node->parent->left == node) {
                node->parent->left = hijo;
            } else {
                node->parent->right = hijo;
            }
        }
        free(node->pair);
        free(node);
    }
    else {
        TreeNode* minimo = minimum(node->right);
        free(node->pair);
        node->pair = minimo->pair;
        removeNode(tree, minimo);
    }
    
    if (tree->current == node) {
        tree->current = NULL;
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) {
        return NULL; 
    }

    TreeNode* nodo = tree->root;
    
    while (nodo != NULL) {
        if (is_equal(tree, key, nodo->pair->key)) {
            tree->current = nodo;
            return nodo->pair;
        }
        else if (tree->lower_than(key, nodo->pair->key)) {
            nodo = nodo->left;
        }
        else {
            nodo = nodo->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* node = tree->root;
    while (node->left != NULL) {
        node = node->left;
    }
    
    tree->current = node;
    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* current = tree->current;
    
    if (current->right != NULL) {
        current = current->right;
        while (current->left != NULL) {
            current = current->left;
        }
        tree->current = current;
        return current->pair;
    }
    
    TreeNode* parent = current->parent;
    while (parent != NULL && current == parent->right) {
        current = parent;
        parent = parent->parent;
    }
    
    tree->current = parent;
    return (parent != NULL) ? parent->pair : NULL;
}
