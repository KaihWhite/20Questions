/*
	animal.cpp

	author: Kaih White

	Animal
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <queue>

#define print(x) cout << x ;

using namespace std;

class node {
public:
    string data;
    node* left;
    node* right;
};

void play_game(node*, string &h);
node* read_game_tree(ifstream &f);
void write_game_tree(node*);
void delete_game_tree(node*);
void expand_game_tree(node*, string h);

bool answer(string inp){
    if(inp == "yes" || inp == "y"){
        return true;
    }
    else{
        return false;
    }
}

void savefile(node* root, ofstream &f){
    if( root == nullptr){
        return;
    }
    if( root -> left == nullptr){
        f << "#A  " << root -> data << endl;
        return;
    }
    f << "#Q  " << root -> data << endl;
    savefile(root ->left, f);
    savefile(root -> right, f);
}

/**
 * Handles showing the main menu/basic UI
 */
int main() {
    print("Please enter the file you want to open: ")
    string name;
    getline(cin, name);
    ifstream f(name);
	node* root = read_game_tree(f);
	if (root == NULL) return -1;

	while (true) {
		string tmp;
		string history = "";
		int choice;

		cout << "Welcome to 20 questions!" << endl;
		cout << "  1) Play the game" << endl;
		cout << "  2) Save the game file" << endl;
		cout << "  3) Quit" << endl;
		cout << "Please make your selection: ";
		getline(cin, tmp);
		choice = atoi(tmp.c_str());

		switch (choice) {
    		case 1:
    			play_game(root, history);
    			break;
    		case 2:
    			write_game_tree(root);
    			break;
    		case 3:
    			break;
    		default:
    			cout << "Sorry, I don't understand." << endl << endl;
		}
		if (choice == 3) break;
	}

	//delete_game_tree(root);
	return 0;
}

/**
 * Sets up the recursive call to the read_preorder
 * @return root of the tree
 */
node* read_game_tree(ifstream &f) {
    string item;
    node *n = new node;
    while (getline(f, item)) {
        if(item.find("#A") != string::npos){
            n -> data = item.substr(2, item.size());
            n -> left = nullptr;
            n -> right = nullptr;
            return n;
        }
        else if(item.find("#Q") != string::npos){
            n -> data = item.substr(2, item.size());
            n -> left = read_game_tree(f);
            n -> right = read_game_tree(f);
            return n;
        }
        else{
            cout << "what" << endl;
        }
    }
    return n;
}

/**
 * Plays the game
 * @param root Root of the game tree
 */
void play_game(node* root, string &h) {
    string input;
    string temp = root -> data + " (y/n lowercase): ";
    print(temp)
    getline(cin, input);
    h += (temp + " " + input + "\n");
    if (answer(input)){
        if(root -> left != nullptr) {
            play_game(root->left, h);
        }
        else{
            cout << "Yay! I guessed your animal!" << endl;
        }
    }
    else{
        if(root -> right != nullptr) {
            play_game(root->right, h);
        }
        else{
            cout << "Dang! I guessed wrong." << endl;
            cout << "Would you like to expand the game tree? : ";
            string expand;
            getline(cin, expand);
            if(answer(expand)){
                expand_game_tree(root, h);
            }
        }
    }
}

void expand_game_tree(node* root, string h){
    string inp;
    string animal;
    print("\nHere are the questions I asked:\n" << h)
    cout << "\nEnter a new animal in the form of a question: ";
    getline(cin,animal);
    node* n = new node;
    root -> left = n;
    root -> left -> data = animal;
    root -> left -> left = nullptr;
    root -> left -> right = nullptr;
    cout << "Enter a question in which the answer yes leads to the new animal: ";
    // not waiting for both inputs
    getline(cin, inp);
    node* p = new node;
    root -> right = p;
    root -> right -> data = root -> data;
    root -> right -> left = nullptr;
    root -> right -> right = nullptr;
    root -> data = inp;
}

/**
 * Writes the game tree, sets up a recursive call to write_preorder();
 * @param root The root of the tree
 */
void write_game_tree(node* root) {
    print("Please enter the name of your file save: ")
    string name;
    getline(cin, name);
    ofstream save(name);
    savefile(root, save);
    save.close();
}