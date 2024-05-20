#include <ncurses.h>

#include <algorithm>
#include <iostream>
#include <limits>
#include <stack>

typedef struct Node {
    int number;
    struct Node *left;
    struct Node *right;
    int height;
} stNode;

// Global variables, it will help us out to avoid
bool flag = false;
int counter;
stNode *ptrNew, *ptrAux, *mainRoot = nullptr;

int getRightInput();
void menu();

// Functions to create a new node, insert and show the AVL TREE
stNode *createNode(int number);
void insertNode(stNode *&root, int number);
void searchNode(stNode *root, int number);
void showTree(stNode *root);
stNode *getNode_Right_SI(stNode *root);
void deleteNode(stNode *&root, int number);

// Functions to balance the AVL TREE
int getHeight(stNode *root);
void updateHeight(stNode *&root);
int getBalance(stNode *root);
stNode *getRightRotate(stNode *&root);
stNode *getLeftRotate(stNode *&root);
stNode *getDoubleRightRotate(stNode *&root);
stNode *getDoubleLeftRotate(stNode *&root);
stNode *balanceTree(stNode *&root);

int main(int argc, char const *argv[]) {
    stNode *ptrRoot = nullptr;

    menu();
    return 0;
}

// this function will help us out to get the right input from the user
int getRightInput() {
    int option;

    while (true) {
        std::cout << "\nIngrese Un Numero: ";
        std::cin >> option;

        if (std::cin.fail()) {
            std::cerr << "\n\tDato Invalido. Por Favor Ingrese Un Dato Valido.\n\n";
            std::cin.clear();
            // we are going to ignore the rest of the characters in the buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    return option;
}

void menu() {
    int option;
    stNode *root = nullptr;
    while (true) {
        std::cout << "\t\t\t ARBOL BINARIO DE BUSQUEDA (AVL) \n";
        std::cout << "\n[1]. Insertar Nodo.";
        std::cout << "\n[2]. Buscar Nodo. ";
        std::cout << "\n[3]. Eliminar Nodo. ";
        std::cout << "\n[4]. Visualizar El Arbol AVL.";
        std::cout << "\n[5]. Salir. \n";
        std::cout << "\n Escoge una opcion: ";
        // we use a try-catch block to handle the exception, the exception is going to be thrown when the user enters a letter or a character
        try {
            std::cin >> option;
            if (std::cin.fail()) {
                throw std::invalid_argument("Dato Invalido. Por Favor Ingrese Un Dato Valido.\n");
            }
            switch (option) {
                case 1:
                    // system("clear");
                    std::cout << "\t\t\t INSERTAR UN NODO EN EL ARBOL AVL \n\n";
                    option = getRightInput();
                    insertNode(root, option);
                    std::cin.get();
                    std::cin.get();
                    break;
                case 2:
                    // system("clear");
                    std::cout << "\t\t\t BUSCAR UN NODO EN EL ARBOL AVL \n\n";
                    if (root == nullptr) {
                        std::cout << "\n\t El Arbol AVL Esta Vacio. Inserte Un Nodo\n";
                    } else {
                        option = getRightInput();
                        flag = false;
                        searchNode(root, option);
                        if (!flag) {
                            std::cout << "\n\t El Nodo No Existe En El Arbol AVL \n";
                        }
                    }
                    std::cin.get();
                    std::cin.get();
                    break;
                case 3:
                    // system("clear");
                    std::cout << "\t\t\t ELIMINAR NODO EN EL ARBOL AVL \n\n";
                    if (root == nullptr) {
                        std::cout << "\n\t El Arbol AVL Esta Vacio.\n";
                    } else {
                        option = getRightInput();
                        flag = false;
                        deleteNode(root, option);
                        if (flag) {
                            std::cout << "\n\t Nodo Eliminado Exitosamente \n";
                        } else {
                            std::cout << "\n\t El Nodo No Existe En El Arbol AVL \n";
                        }
                    }
                    std::cin.get();
                    std::cin.get();
                    break;
                case 4:
                    // system("clear");
                    std::cout << "\t\t\t VISUALIZAR EL ARBOL AVL \n\n";
                    if (root == nullptr) {
                        std::cout << "\n\t El Arbol AVL Esta Vacio. \n";
                    } else {
                        showTree(root);
                    }
                    std::cin.get();
                    std::cin.get();
                    break;
                case 5:
                    exit(0);
                    break;
                default:
                    std::cout << "\nOpcion Invalida, [1-5]. \n";
                    break;
            }
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// We are going to create all of the information for a new node
stNode *createNode(int number) {
    // we are going to ask a memory space for the new node
    // as a good practice we are going to add all of the information before to make any operation
    ptrNew = (stNode *)malloc(sizeof(stNode));
    ptrNew->number = number;
    ptrNew->left = nullptr;
    ptrNew->right = nullptr;
    return ptrNew;
}

void insertNode(stNode *&root, int number) {
    // if the root is null, we are going to create a new node.
    if (root == nullptr) {
        // we are going to create a new node
        root = createNode(number);
        // we are going to set the mainRoot with the root, with purpose to have a reference to the root and not include the same value at root
        if (mainRoot == nullptr) {
            mainRoot = root;
        }
        std::cout << "\n\t Nodo Insertado. \n";
        // we are going to update the height of the root
        updateHeight(root);
        root = balanceTree(root);
        counter++;
        return;
    }

    if (number == mainRoot->number) {
        std::cout << "\t\nEl Nodo Es Igual Al Valor De La Raiz, Nodo Rechazado. \n";
        return;
    }

    // ahora hacemos las dos validaciones, si # <= root->number, si es menor o igual, se va a insertar a la izquierda
    //  si es mayor se va a insertar a la derecha
    (number <= root->number) ? insertNode(root->left, number) : insertNode(root->right, number);
    updateHeight(root);
    root = balanceTree(root);
}

void searchNode(stNode *root, int number) {
    if (root == nullptr)
        return;

    // else

    if (root->number == number) {
        std::cout << "\t Nodo Encontrado: " << root->number << " En La Direccion: " << root << std::endl;
        flag = true;
    }

    (number <= root->number) ? searchNode(root->left, number) : searchNode(root->right, number);
}

// This function will help us out to find out where and what is the maximum value of Node more right in left subtree (Node+D: SI)
stNode *getNode_Right_SI(stNode *root) {
    while (root->right != nullptr) {
        root = root->right;
    }
    return root;
}

void deleteNode(stNode *&root, int number) {
    /*
        Case 1:  when there is a leaf in the binary tree
        Case 2:  when the father only has a child
        Case 3:  when the father has two children
    */

    if (root == nullptr) {
        return;
    }

    // once we already checked the root is not nullptr, we are going to move on to the next step

    if (number < root->number) {
        deleteNode(root->left, number);
    } else if (number > root->number) {
        deleteNode(root->right, number);
    } else {
        // we found the node to delete, so we are going to check the cases and change the flag to true
        flag = true;

        // case 1: when there is a leaf in the binary tree
        if (root->left == nullptr && root->right == nullptr) {
            free(root);
            root = nullptr;
        }

        // case 2: when the father only has a child
        else if (root->left == nullptr || root->right == nullptr) {
            ptrAux = root;
            root = (root->left == nullptr) ? root->right : root->left;
            free(ptrAux);
        }
        // case 3: when the father has two children
        else {
            ptrAux = getNode_Right_SI(root->left);
            root->number = ptrAux->number;
            deleteNode(root->left, ptrAux->number);
        }
    }
    updateHeight(root);
    root = balanceTree(root);
}

void showTree(stNode *root) {
    if (root != nullptr) {
        std::cout << "\t" << root->number;
        showTree(root->left);
        showTree(root->right);
    }
}

int getHeight(stNode *root) {
    return (root == nullptr) ? 0 : root->height;
}

void updateHeight(stNode *&root) {
    if (root != nullptr) {
        // height(root->left) = 1
        // height(root->right) = 0
        // root->height = 2
        root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    }
}
/*
levels:
    20 -> height 1

levels:
    20 -> height 3
    10 -> height 2
    5 -> height 1



*/

int getBalance(stNode *root) {
    return (root == nullptr) ? 0 : getHeight(root->left) - getHeight(root->right);
}
// this function is the same as we learned in class, we know it as "Left-Left CASE"
stNode *getRightRotate(stNode *&root) {
    ptrAux = root->left;
    root->left = ptrAux->right;
    ptrAux->right = root;
    // we are going to update the height of the root
    updateHeight(root);
    updateHeight(ptrAux);

    return ptrAux;
}

// this function is the same as we learned in class, we know it as "Right-Right CASE"
stNode *getLeftRotate(stNode *&root) {
    ptrAux = root->right;
    root->right = ptrAux->left;
    ptrAux->left = root;

    // we are going to update the height of the root
    updateHeight(root);
    updateHeight(ptrAux);

    return ptrAux;
}
// this function is the same as we learned in class, we know it as "Left-Right CASE"
stNode *getDoubleRightRotate(stNode *&root) {
    root->left = getLeftRotate(root->left);
    return getRightRotate(root);
}
// this function is the same as we learned in class, we know it as "Right-Left CASE"
stNode *getDoubleLeftRotate(stNode *&root) {
    root->right = getRightRotate(root->right);
    return getLeftRotate(root);
}

stNode *balanceTree(stNode *&root) {
    // we are going to get the balance of the root
    int balance = getBalance(root);

    // Left-Left CASE
    if (balance > 1 && getBalance(root->left) > 0) {
        return getRightRotate(root);
    }
    // Right-Right CASE
    if (balance < -1 && getBalance(root->right) < 0) {
        return getLeftRotate(root);
    }

    // Left-Right CASE
    if (balance > 1 && getBalance(root->left) < 0) {
        return getDoubleRightRotate(root);
    }

    // Right-Left CASE
    if (balance < -1 && getBalance(root->right) > 0) {
        return getDoubleLeftRotate(root);
    }

    return root;
}