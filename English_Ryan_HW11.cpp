#include <fstream>
#include <iostream>
#include <cstddef>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <chrono>
#include <pthread.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;

// The number of ints wer are summing
#define NUM_COUNT 100000
// The number of threads to test
#define NUM_THREADS 10
// The scale of which we are summing in threads
#define SCALE 10000

/**
 * A struct of thread data in order to get the sum
 * of the threaded values
 */
typedef struct thread_data
{
    // Where to start the sum
    int start;
    // How are to sum to
    int end;
    // The number array
    int numbers[NUM_COUNT];
    // The results of the threaded sum
    int result;
} thread_data;

/**
 * The sum function from a start point to an end point
 * of the numbers in the array
 */
int sum(int numbers[], int start, int end)
{
    int sum;
    for (int i = start; i < end; i++)
    {
        sum += numbers[i];
    }
    return sum;
}

/**
 * Threaded sum function. The param is thread data
 * in order to get the parameters needed to sum 
 */
void *sumThreaded(void *param)
{
    thread_data *tdata = (thread_data *)param;
    tdata->result = sum(tdata->numbers, tdata->start, tdata->end);
    pthread_exit(NULL);
}

/**
 * Ryan English
 * Homework 11
 * 
 * Read 100,000 integers and sum in a single thread vs 10 threads
 * determine the deference of time it takes between the two.
 * 
 * Did not see any reference to the time sys call mentioned in the
 * instructions, so I looked on online:
 * stack overflow: https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
 * stack overflow: https://stackoverflow.com/questions/41077377/how-to-get-current-time-in-milliseconds
 * cppref: https://en.cppreference.com/w/cpp/chrono/duration/duration_cast
 */
int main(int argc, const char *argv[])
{
    // The numbers we are goin to sum
    int numbers[NUM_COUNT];

    // Open the file
    std::ifstream file("./input.txt");

    // Go line by line and insert into the trie
    if (file.is_open())
    {
        int index = 0;
        std::string line;
        while (std::getline(file, line))
        {
            // The number
            int num = std::stoi(line);
            // Add to the arra
            numbers[index++] = num;
        }
    }

    // Part One
    // Use a single thread to sum the array
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    int s = sum(numbers, 0, NUM_COUNT);
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Output results and time
    cout << "Single Thread: ";
    cout << s;
    cout << " Took: ";
    cout << elapsed.count();
    cout << " microseconds" << endl;

    // Part Two
    // Create 10 threads sum, then sum those ten
    // Store the threads for the join
    pthread_t threads[NUM_THREADS];
    // We need to store the thread data to ensure its not collected
    thread_data threadData[NUM_THREADS];

    std::chrono::high_resolution_clock::time_point start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Create the thread data
        threadData[i].start = i * SCALE;
        // Add the scale to get the end point
        threadData[i].end = threadData[i].start + SCALE;
        // Copy over the array, should be quicker than resetting. If we don't do this
        // Threads can be waiting to access the array
        memcpy(&threadData[i].numbers, &numbers, sizeof(numbers));
        ;

        // Create the thread with the thread data and call the sumThreaded function
        pthread_create(&threads[i], NULL, sumThreaded, &threadData[i]);
    }

    int s2 = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Wait for thread
        pthread_join(threads[i], NULL);
        // Add the results to the sum
        s2 += threadData[i].result;
    }

    // Output the results and time
    std::chrono::high_resolution_clock::time_point end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> elapsed2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    cout << "Multi Thread: ";
    cout << s2;
    cout << " Took: ";
    cout << elapsed2.count();
    cout << " microseconds" << endl;
}