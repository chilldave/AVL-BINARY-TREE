#include <algorithm>
#include <iostream>
#include <limits>
#include <stack>
#include <conio.h>
#include <windows.h>

#define WIDTH 70
#define WEIGHT 35




void SetConsoleWindowSize(int width, int height) {
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, width, height, TRUE);
}


void gotoxy(int x, int y){

    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}


void setConsoleSize(int cols, int rows){

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOut == INVALID_HANDLE_VALUE)
        return; 

    SMALL_RECT r;

    r.Left = 0;
    r.Top = 0;
    r.Right = cols -1;
    r.Bottom = rows -1;
    SetConsoleWindowInfo(hOut, TRUE, &r);

    COORD c;

    c.X = cols;
    c.Y = rows;

    SetConsoleScreenBufferSize(hOut, c);

}
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
    system("cls");
    setConsoleSize(100,30);
    // SetConsoleWindowSize(400,600);
    stNode *ptrRoot = nullptr;  
    menu();
    return 0;
}

// this function will help us out to get the right input from the user
int getRightInput() {
    int option;

    while (true) {
        // system("cls");
        gotoxy(20,7); std::cout << "                                         ";
        gotoxy(20,7); std::cout << "Ingrese Un Numero: ";
        std::cin >> option;

        if (std::cin.fail()) {
            gotoxy(20,9); std::cerr << "Dato Invalido. Por Favor Ingrese Un Dato Valido.";
            std::cin.clear();
            // we are going to ignore the rest of the characters in the buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            gotoxy(20,7); std::cout << "                                             ";
            gotoxy(20,9); std::cout << "                                                                  "; 
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
        system("cls");

        gotoxy(35,5); std::cout << "ARBOL BINARIO DE BUSQUEDA (AVL)";
        gotoxy(25,8); std::cout << "[1]. Insertar Nodo.";
        gotoxy(25,9); std::cout << "[2]. Buscar Nodo. ";
        gotoxy(25,10); std::cout << "[3]. Eliminar Nodo. ";
        gotoxy(25,11); std::cout << "[4]. Visualizar El Arbol AVL.";
        gotoxy(25,12); std::cout << "[5]. Salir.";
        gotoxy(24,17); std::cout << "Escoge una opcion: ";

        // we use a try-catch block to handle the exception, the exception is going to be thrown when the user enters a letter or a character
        try {
            std::cin >> option;
            if (std::cin.fail()) {
                gotoxy(24,19); throw std::invalid_argument("Dato Invalido. Por Favor Ingrese Un Dato Valido.");
            }
            switch (option) {
                case 1:
                    system("cls");
                    gotoxy(33,5); std::cout << "INSERTAR UN NODO EN EL ARBOL AVL.";
                    option = getRightInput();
                    insertNode(root, option);
                    gotoxy(33,24); std::cout<<"mainRoot: " << mainRoot->number;
                    std::cin.get();
                    std::cin.get();
                    break;
                case 2:
                    system("cls");
                    gotoxy(33,5); std::cout << "BUSCAR UN NODO EN EL ARBOL AVL";
                    if (root == nullptr) {
                        gotoxy(20,8);std::cout << "El Arbol AVL Esta Vacio. Inserte Un Nodo";
                    } else {
                        option = getRightInput();
                        flag = false;
                        searchNode(root, option);
                        if (!flag) {
                            gotoxy(20,10); std::cout << "El Nodo No Existe En El Arbol AVL.";
                        }
                    }
                    std::cin.get();
                    std::cin.get();
                    break;
                case 3:
                    system("cls");
                    gotoxy(33,5); std::cout << "ELIMINAR NODO EN EL ARBOL AVL";
                    if (root == nullptr) {
                        gotoxy(20,8); std::cout << "El Arbol AVL Esta Vacio.";
                    } else {
                        option = getRightInput();
                        flag = false;
                        deleteNode(root, option);
                        if (flag) {
                            gotoxy(20,10); std::cout << "Nodo Eliminado Exitosamente.";
                        } else {
                            gotoxy(20,10); std::cout << "El Nodo No Existe En El Arbol AVL.";
                        }
                    }
                    std::cin.get();
                    std::cin.get();
                    break;
                case 4:
                    system("cls");
                    gotoxy(33,5); std::cout << "VISUALIZAR EL ARBOL AVL";
                    if (root == nullptr) {
                        gotoxy(20,8); std::cout << "El Arbol AVL Esta Vacio.";
                    } else {
                        showTree(root);
                    }
                    std::cin.get();
                    std::cin.get();
                    break;
                case 5:
                    system("cls");
                    exit(0);
                    break;
                default:
                    gotoxy(24,20); std::cout << "Opcion Invalida, [1-5]. Intentelo De Nuevo";
                    gotoxy(24,22); std::cout << "Presione enter para continuar...";
                    std::cin.get();
                    std::cin.get();
                    break;  
            }
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            // std::cout << "Presione Enter para continuar...";
            std::cin.get(); // Esperar a que el usuario presione Enter
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
        gotoxy(20,10); std::cout << "Nodo Insertado.";
        // we are going to update the height of the root
        updateHeight(root);
        root = balanceTree(root);
        counter++;
        mainRoot = root;
        return;
    }

    if (number == mainRoot->number) {
        gotoxy(20,9); std::cout << "El Nodo Es Igual Al Valor De La Raiz, Nodo Rechazado.";
        return;
    }

    // ahora hacemos las dos validaciones, si # <= root->number, si es menor o igual, se va a insertar a la izquierda
    //  si es mayor se va a insertar a la derecha
    (number <= root->number) ? insertNode(root->left, number) : insertNode(root->right, number);
    updateHeight(root);
    mainRoot = root;
    root = balanceTree(root);
}

void searchNode(stNode *root, int number) {
    if (root == nullptr)
        return;

    // else

    if (root->number == number) {
        gotoxy(20,10); std::cout << "Nodo Encontrado: " << root->number << " En La Direccion: " << root;
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
    //we need to update the mainRoot in order to update always the root 
    mainRoot = root;
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