#include <iostream>
#include <cstddef>
#include <queue>
#include <list>
#include <fstream>
#include <string>
#include <cstring>

using std::string;
using std::queue;
using std::cout;
using std::cin;
using std::endl;
using std::list;

/*
* ASSUME we are only using this for 
* ASCII letters
*/
static unsigned short ASCII_SIZE = 128;

class Trie {
    private:
        /*
        * Each child of the current trie node
        */
        Trie** children;

        /*
        * The current nodes inserted value
        */
        string value;
    public:

    /*
    * News a trie node with default values
    */
    Trie() {
        // Default value
        this->value = "";
        // Set children array
        this->children = new Trie*[ASCII_SIZE];
        // Set all to null for now
        for (int i = 0; i < ASCII_SIZE; i++) {
            children[i] = NULL;
        }
    }

    /*
    * Deletes the trie
    */
    ~Trie() {
        for (int i = 0; i < ASCII_SIZE; i++) {
            children[i] = NULL;
            delete children[i];
        }
    }

    /*
    * Returns a queue of words that match the 
    * current search and/or have additional letters
    * after. Returns NULL if it fails to find words
    */
    queue<string>* completion(string search) {
        // Set this as current
        Trie* current_trie = this;

        // Possible matches
        queue<string> *words = new queue<string>();

        for (int i = 0; i < search.length(); i++) {
            // Get the letter
            char c = search[i];

            // If the trie node is null exit and send back what we have
            if (current_trie->children[c] == NULL) {
                return NULL;
            }

            current_trie = current_trie->children[c];
        }

        // Now we got all the way down, we need to find all the leafs
        return current_trie->collect();
    };

    /*
    * Traverse all the children and get all the possible words
    */
    queue<string>* collect() {
        // The collection
        queue<string> *words = new queue<string>();

        // First see if we have a value
        if (this->value != "") {
            words->push(this->value);
        }

        // Loop every child and collect their leafs
        for (int i = 0; i < ASCII_SIZE; i++) {
            // If its not null we can go in and collect
            if (this->children[i] != NULL) {
                Trie* child = this->children[i];
                queue<string>* children_words = child->collect();
                // Move the chidren to the current collection
                while (!children_words->empty()) {
                    string child_word = children_words->front();
                    words->push(child_word);
                    children_words->pop();
                }
            }
        }

        return words;
    }

    /*
    * Inserts a string into the trie, once it gets
    * to the end of the string it marks it as a leaf
    * in order to get collected later
    */
    void insert(string value) {
        // The current node
        Trie* current_trie = this;
        
        // Loop the letters and insert into their children
        for (int i = 0; i < value.length(); i++) {
            // Get the letter
            char c = value[i];

            // If there is not an entry we need to add one
            if (current_trie->children[c] == NULL) {
                current_trie->children[c] = new Trie();
            }

            // Set the current trie to the child letter and continue
            current_trie = current_trie->children[c];
        }

        current_trie->value = value;
    };

    /*
    * Removes a letter at each position and researches the
    * query. This assumes that the user incorrectly typed
    * a letter, does not swap letter locations for sake of simplicity
    */
    queue<string>* possible_matchs(string search) {
        // Use the list in order to sort easily
        list<string>* possible = new list<string>();
        queue<string>* suggestions = new queue<string>();
        // Loop all the characters and remove said character and attempt search
        for (int i = 0; i < search.length(); i++){
            string altered = search.substr(0,i) + search.substr(i + 1);

            queue<string>* matches = this->completion(altered);
            if  (matches != NULL) {
                while (!matches->empty()) {
                    string child_word = matches->front();

                    // The word actually meets the altered string, this is one the biggest potential of what the user meant
                    if (child_word == altered) {
                        suggestions->push(child_word);
                    }
                    // Next we put at the front of the list words that match length and with a letter dropped
                    else if (child_word.length() == search.length() || child_word.length() == altered.length()) {
                        possible->push_front(child_word);
                    // And finally well take all other words and put at the end
                    } else {
                        possible->push_back(child_word);
                    }

                    matches->pop();
                }
            }

            // If we end up with three matches lets just return those
            if (suggestions->size() == 3) {
                return suggestions;
            }
        }

        // If we don't have three and we have possible matches lets move over till we have 3
        while (suggestions->size() < 3 && possible->size() > 0) {
            suggestions->push(possible->front());
            possible->pop_front();
        }

        return suggestions->size() == 0 ? NULL : suggestions;
    };

};

/*
* Project One
* Ryan English
* CSE 503 - 09/18/2020
* Trie structure for search completion
*/
int main(int argc, const char *argv[])
{
    /*
    * Problem 1
    * Read a file and create trie of words
    */

    // Open the file
    std::ifstream file("./dictonary.txt");

    // Start the tree root
    Trie* root = new Trie();

    // Go line by line and insert into the trie
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            root->insert(line);
        }
        file.close();
    }


    /*
    * Problem Two
    * Have the user enter a search term and get the possible
    * matches of the string given
    * Problem Three
    * If the user enters something is not found then output
    * the get words the are a close match
    */

    string query;
    while (true) {
        cout << "Please type a search or exit to quit: " << endl;
        cin >> query;

        // Allows user to terminate
        if (query == "exit") {
            break;
        }

        /*
        * Problem Two:
        * This tries to find possible words that complete the query
        */
        queue<string>* possible_words = root->completion(query);

        if (possible_words != NULL) {
            // If we have possible words list them out
            cout << "Possible options: " << endl;
        } 
        /*
        * Problem Three:
        * No matches were found, try to get possible matches
        * of the query.
        */
        else {
            // If there is no match get possible matches
            possible_words = root->possible_matchs(query);
            if (possible_words != NULL) {
                cout << "Did you mean: " << endl;
            }
        }

        // If we still failed to find anything just notify the user
        if (possible_words == NULL) {
            cout << "Unable to find any match or suggestions" << endl;
        } 
        // Output the found words
        else {
            // Output the possible words
            while (!possible_words->empty()) { 
                cout << possible_words->front() << endl;
                possible_words->pop(); 
            } 
        }
    }

    return 0;
}