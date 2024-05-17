#include <iostream>
#include <stack>
#include <algorithm>

typedef struct Node{
    int number;
    struct Node *left;
    struct Node *right;
    int height;    
}stNode;

// Global variables, it will help us out to avoid 
int counter;
stNode* ptrNew, *ptrAux, *mainRoot=nullptr;


void menu();
// Functions to create a new node, insert and show the AVL TREE
stNode* createNode(int number);
void insertNode(stNode*& root, int number);
void showTree(stNode* root);

// Functions to balance the AVL TREE
int height(stNode* root);
void updateHeight(stNode*& root);



int main(int argc, char const *argv[])
{
    stNode* ptrRoot = nullptr;

    menu();

    std::cout << "\t DISPLAYING AVL BINARY TREE \n";    
    showTree(ptrRoot);

    return 0;
}



void menu(){
    int option;

    stNode* root = nullptr;

    while(true){
        std::cout << "\t\t\t AVL BINARY TREE \n";
        std::cout << "\n[1]. Insert Node \n";
        std::cout << "\n[2]. Show Tree \n";
        std::cout << "\n[3]. Exit \n";
        std::cout << "\nChoose an option: ";
        std::cin >> option;

        switch(option){
            case 1:
                // system("clear");
                std::cout << "\t\t\t INSERT NODE \n\n";
                std::cout << "\t Enter a number: ";
                std::cin >> option;
                insertNode(root, option);
                std::cin.get();
                std::cin.get();
                break;
            case 2:
                showTree(root);
                std::cin.get();
                std::cin.get();

                break;
            case 3:
                exit(0);
                break;
            default:
                std::cout << "Invalid option \n";
                break;
        }
    }
}

// We are going to create all of the information for a new node
stNode* createNode(int number){
    // we are going to ask a memory space for the new node
    // as a good practice we are going to add all of the information before to make any operation 
    ptrNew = (stNode*)malloc(sizeof(stNode));
    ptrNew->number = number;
    ptrNew->left = nullptr;
    ptrNew->right = nullptr;
    // ptrNew->height = 0;

    return ptrNew;
}


void insertNode(stNode*& root, int number ){
        
    // if the root is null, we are going to create a new node.
    if(root == nullptr){
        root = createNode(number);
        // vamos a asignarle el valor del nodo de primer nivel ( nodo raiz )
        if(mainRoot == nullptr){
            mainRoot = root;
        }
        std::cout << "\n\t Nodo insertado. \n" ;
        //we are going to update the height of the root
        updateHeight(root);
        counter++;
        return;
    }
    if(number == mainRoot->number){
    std::cout << "\t\nEl nodo es igual al valor de la raiz, no insertardo. \n";
        return;
    }
        
    //ahora hacemos las dos validaciones, si # <= root->number, si es menor o igual, se va a insertar a la izquierda
    // si es mayor se va a insertar a la derecha
    if(number <= root->number){
        insertNode(root->left, number);   
    }else {
        insertNode(root->right, number);
    }
    updateHeight(root);
}

void showTree(stNode* root){
    if(root != nullptr){
        std::cout << root->number << " - ";
        // std::cout << "direction: " << root << " - \n";
        // std::cout << "Left: " << root->left << " - \n";
        // std::cout << "Right: " << root->right << " - \n";
        
        std::cout << "Height: " << root->height << std::endl;
        showTree(root->left);
        showTree(root->right);  
    }
}

int height(stNode* root){   
    return (root == nullptr) ? root->height : 0;
}



void updateHeight(stNode*& root){
    if(root != nullptr){
        // height(root->left) = 1
        // height(root->right) = 0
        //root->height = 2
        root->height = 1 + std::max(height(root->left), height(root->right));
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