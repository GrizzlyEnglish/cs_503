#include <iostream>
#include <cstddef>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// The amount of random integers we want
#define LENGTH 1000
// The largest number we can have in the array, assume 0 lowest
#define MAX_INT 100

using std::string;
using std::cout;
using std::cin;
using std::endl;

/**
 * The child process of counting the amount of numbers
 * in the search array
 */
void countAndOutput(int searchArray[], int number)
{
    // Count how many ints
    int count = 0;
    for (int i = 0; i < LENGTH; i++) {
        int ai = searchArray[i];
        count += ai == number ? 1 : 0;
    }

    // Out put based on the HW sheet
    cout << "query: ";
    cout << number;
    cout << " count: ";
    cout << count;
    cout << " pid: ";
    cout << getpid() << endl;

    // Exit the child process
    exit(0);
}

/**
 * Homework #9
 * Ryan English
 * 10-15-2020
 * 
 * Generates an array of 1,000 random integers
 * between 0-10, loads file for querying how many are
 * in the array while forking the process of counting and outputting
 * then waiting for all to complete before exiting
 * 
 * NOTE: unistd.h is not part of MinGW so I could not compile and run
 * this program without the help of https://repl.it/languages/cpp
 * What a pain.
 * 
 */
int main(int argc, const char *argv[])
{
    // Construct an array of 1000 random [0,100] integers
    int arr[LENGTH];

    // Loop and add random numbers
    for (int i = 0; i < LENGTH; i++) {
        arr[i] = rand() % MAX_INT;
    }

    // Open the file
    std::ifstream file("./input.txt");

    // Go line by line and insert into the trie
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // Fork a new process for counting
            pid_t pid = fork();
            // The number
            int num = std::stoi(line);

            // Failed to fork just output the error
            if (pid < 0){
              cout << "Failed to fork for query: ";
              cout << num << endl;
            }
            else if (pid == 0) {
                // The child process
                countAndOutput(arr, num);
            }
        }

        // We need to keep pinging wait until ALL processes are done
        int status;
        // The pid that is completing
        pid_t done;
        // Wait for the results from done, until we get a -1 indicating all are complet
        while ((done = wait(&status)))
        {
          // Nothing came back lets break out
          if (done == -1) {
              cout << "All child processes are complete" << endl;
              // Break out we are done
              break;
            }
        }

        file.close();
    }

    return 0;
}