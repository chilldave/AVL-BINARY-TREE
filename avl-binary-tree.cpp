#include <algorithm>
#include <iostream>
#include <limits>
#include <stack>
#include <conio.h>
#include <windows.h>

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
//We have the Node structure 
typedef struct Node {
    int number;
    struct Node *left;
    struct Node *right;
    int height;
} stNode;

// Global variables, it will help us out to avoid
bool flag = false; stNode *ptrNew, *ptrAux, *mainRoot = nullptr; int i = 10;

// menu and getRightInput is really important to manage the input
int getRightInput();
void menu();

/*   Functions to create a new node, insert, search, show, delete  Nodo
     Thre is another function named getNode_Right_SI, it means we are going to get the Node+Right -> left->subtree
*/
stNode *createNode(int number);
void insertNode(stNode *&root, int number);
void searchNode(stNode *root, int number);
void showTree(stNode *root, int x, int y, int offset);
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
    SetConsoleWindowSize(1650,600);
    setConsoleSize(180,35);
    menu();
    return 0;
}

// this function will help us out to get the right input from the user
int getRightInput() {
    int option;
    /*
        We are going to ask a number, cin.fail() helps to verify whether the input was certainly well or not 
        if cin.fail() returns true, we are going to show a message, we are going to clear the cin input
        cin.ignore -> reject the wrong input
        but it returns false, the code will end the while loop
    */
    while (true) {
        gotoxy(20,7); std::cout << "                                         ";
        gotoxy(20,7); std::cout << "Ingrese Un Numero: ";
        std::cin >> option;

        if (std::cin.fail()) {
            gotoxy(20,9); std::cerr << "Dato Invalido. Por Favor Ingrese Un Dato Valido.";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); // we are going to ignore the rest of the characters in the buffer
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
    int option; stNode *root = nullptr; //define local variables

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
                case 1: //Insert a Node
                    system("cls");
                    gotoxy(33,5); std::cout << "INSERTAR UN NODO EN EL ARBOL AVL.";
                    gotoxy(90,5); std::cout << "mainRoot -> " << mainRoot;
                    option = getRightInput();
                    insertNode(root, option);
                    gotoxy(24,22); std::cout << "Presione enter para continuar...";
                    std::cin.get();
                    std::cin.get();
                    break;
                case 2: // Search a Node and we are going to verify if the tree is empty or not
                    system("cls");
                    gotoxy(33,5); std::cout << "BUSCAR UN NODO EN EL ARBOL AVL";
                    if (root == nullptr) {
                        gotoxy(20,8);std::cout << "El Arbol AVL Esta Vacio.";
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
                case 3: // Delete a nodo and verify if the tree is empty or not
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
                case 4: // Show AVL Tree with Format
                    system("cls");
                    gotoxy(40,5); std::cout << "VISUALIZAR EL ARBOL AVL";
                    if (root == nullptr) {
                        gotoxy(171,8); std::cout << "*";

                        gotoxy(0,8); std::cout << "*";
                        gotoxy(25,8); std::cout << "El Arbol AVL Esta Vacio.";
                    } else {
                        gotoxy(179,8); std::cout << "*";
                        gotoxy(0,8); std::cout << "*";
                        showTree(root,87,8,44); 
                        gotoxy(24,22); std::cout << "Presione enter para continuar...";
                    }
                    std::cin.get();
                    std::cin.get();
                    break;
                case 5: //End the while loop
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
            std::cerr << e.what() << std::endl; //It will display a error message 
            std::cin.clear(); // it will clear the input 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // it will ignore and reject the information 
            gotoxy(72,19);std::cin.get(); // Esperar a que el usuario presione Enter
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
        root = createNode(number);   // we are going to create a new node
        if (mainRoot == nullptr) {   // we are going to set the mainRoot with the root, with purpose to have a reference to the root and not include the same value at root
            mainRoot = root;
        }
        gotoxy(20,10); std::cout << "Nodo Insertado.";
        updateHeight(root);           // we are going to update the height of the root
        root = balanceTree(root);     // we are going to balance the tree   
        return;
    }
    // if the nuber is the same as root, it will be rejected 
    if (number == mainRoot->number) {
        gotoxy(20,9); std::cout << "El Numero Es Igual Al Valor De La Raiz, Nodo Rechazado.";
        return;
    }

    // ahora hacemos las dos validaciones, si # <= root->number, si es menor o igual, se va a insertar a la izquierda
    //  si es mayor se va a insertar a la derecha
    (number <= root->number) ? insertNode(root->left, number) : insertNode(root->right, number);
    updateHeight(root);
    root = balanceTree(root);
    mainRoot =  root;  // we are going to update the mainRoot pointer in case the root has change
}

void searchNode(stNode *root, int number) {
    if (root == nullptr)  return;

    if (root->number == number) {
        gotoxy(20,i); std::cout << "Nodo Encontrado: " << root->number << " En La Direccion: " << root;
        flag = true;
        i++;
    }
    // (number <= root->number) ? searchNode(root->left, number) : searchNode(root->right, number);
    searchNode(root->left, number); searchNode(root->right, number);
}

// This function will help us out to find out where and what is the maximum value of Node more right in left subtree (Node+D: SI)
stNode *getNode_Right_SI(stNode *root) {
    while (root->right != nullptr) 
        root = root->right;
    return root;
}

void deleteNode(stNode *&root, int number) {
    /*
        Case 1:  when there is a leaf in the binary tree
        Case 2:  when the father only has a child
        Case 3:  when the father has two children
    */
    if (root == nullptr)  return;

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
        } else if (root->left == nullptr || root->right == nullptr) { // case 2: when the father only has a child
            ptrAux = root;
            root = (root->left == nullptr) ? root->right : root->left;
            free(ptrAux);
        } else {   // case 3: when the father has two children
            ptrAux = getNode_Right_SI(root->left); 
            root->number = ptrAux->number;
            deleteNode(root->left, ptrAux->number);
        }
    }
    updateHeight(root);
    root = balanceTree(root);
    mainRoot = root; //we need to update the mainRoot in order to update always the root 
}

void showTree(stNode* root, int x, int y, int offset) {
    if (root) {
        gotoxy(x, y);
        std::cout <<"["<< root->number <<"]";
        if (root->left) {
            gotoxy(x - offset / 2, y + 1);
            std::cout << "* ";
            showTree(root->left, x - offset, y + 2, offset / 2);
        }   
        if (root->right) {
            gotoxy(x + offset / 2, y + 1);
            std::cout << "   *";
            showTree(root->right, (x+1) + offset, y + 2, offset / 2);
        }
    }
}

int getHeight(stNode *root) { 
    return (root == nullptr) ? 0 : root->height;
}

void updateHeight(stNode *&root) {
    if (root != nullptr) {
        //we are going to update the height in each node
        root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    }
}

int getBalance(stNode *root) {
    //Balnace the tree, we are going to make sure the result could be -1,0,1
    return (root == nullptr) ? 0 : getHeight(root->left) - getHeight(root->right);
}
// this function is the same as we learned in class, we know it as "Left-Left CASE"
stNode *getRightRotate(stNode *&root) {
    ptrAux = root->left;
    root->left = ptrAux->right;
    ptrAux->right = root;
    updateHeight(root);  // we are going to update the height of the root
    updateHeight(ptrAux);

    return ptrAux;
}

// this function is the same as we learned in class, we know it as "Right-Right CASE"
stNode *getLeftRotate(stNode *&root) {
    ptrAux = root->right;
    root->right = ptrAux->left;
    ptrAux->left = root;
    updateHeight(root); // we are going to update the height of the root
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
    int balance = getBalance(root); // we are going to get the balance of the root

    if (balance > 1 && getBalance(root->left) > 0) {     // Left-Left CASE
        return getRightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) < 0) {    // Right-Right CASE
        return getLeftRotate(root);
    }

    if (balance > 1 && getBalance(root->left) < 0) {    // Left-Right CASE
        return getDoubleRightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) > 0) {  // Right-Left CASE
        return getDoubleLeftRotate(root);
    }
    return root;
}