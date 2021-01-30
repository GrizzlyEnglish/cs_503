#include <iostream>
#include <fstream>
#include <cstddef>
#include <string>
#include <algorithm>
#include <sstream>
#include <queue>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::stringstream;
using std::queue;

// The amount of allotted time per cpu thread
#define QUANTUM_MS 5
// The amount of jobs to read from file
#define JOB_COUNT 1001

/*
* The ScheduledJob class houses all the information
* read from the file to mock a cpu process, like a job id
* time requested at, and how long the job should take
* ScheduledJob also houses a run job function that outputs
* the job information as per instructions
*/
class ScheduledJob {

private:

    /*
    * How long the job needs to run
    */
    int duration;
    /*
    * The current amount of the job completed
    */
    int duration_ran;
    /*
    * The time the job wants to start
    * since the start of the CPU
    */
    int requesting_time_ms;
    /*
    * The id of the job
    */
    int job_id;

public:

    /*
    * Parameterless constructor for generation in the array
    */
    ScheduledJob() {
    }

    /*
    * Nothing to garbage collect on deletion
    */
    ~ScheduledJob() {
    }

    /*
    * Sets all the member variables of the job after init
    */
    void memberInit(int id, int requesting_time, int duration) {
        this->job_id = id;
        this->requesting_time_ms = requesting_time;
        this->duration = duration;
        this->duration_ran = 0;
    }

    /*
    * Returns the time that the job was requested for
    */
    int get_requested_time(){
        return this->requesting_time_ms;
    }

    /*
    * Determines if the job is complete if the duration ran
    * is greater than or equal to the duration of the job
    */
    bool is_completed(){
        return this->duration <= this->duration_ran;
    }

    /*
    * "Runs" the job for the given quantum time
    * returns the amount of time used
    */
    int run_job() {
        cout << "Job ";
        cout << this->job_id;
        cout << ", scheduled for ";

        int time_ran;

        // Determines how much time is left in the job, so if it doesn't use all qms we can display
        int time_diff = this->duration - this->duration_ran;
        // If it is greater than qms use qms
        if (time_diff >= QUANTUM_MS) {
            time_ran = QUANTUM_MS;
        // Otherwise use the diff we have
        } else {
            time_ran = time_diff;
        }

        // Add to duration ran
        this->duration_ran += time_ran;

        cout << time_ran;
        cout << "ms";

        // If we are complete output
        if (this->is_completed()) {
            cout << ", job completed";
        }

        cout << endl;

        return time_ran;
    }

    /*
    * Returns how long the job has been running for
    */
    int get_running_time() {
        return this->duration_ran;
    }

    /*
    * Resets the job for reprocessing
    */
    void reset_job() {
        this->duration_ran = 0;
    }

};

/*
* Project 2
* Ryan English
*
* Mock a CPU Round Robin Scheduling 
* Reads jobs from a file to mock generating cpu
* requests at a given time frame, and runs the jobs
* to completion
* Part One: Do a normal 1 CPU Round Robin Scheduling
* Part Two: Do 4 CPU Round Robin Scheduling
*/
int main(int argc, const char *argv[])
{
    // Open the file
    std::ifstream file("./job.txt");

    // Houses all the jobs
    ScheduledJob jobs[JOB_COUNT];

    if (file.is_open()) {
        // The current line we are reading
        std::string line;
        // A ticker for the slot in the jobs array
        int job_count = 0;
        // Read all lines
        while (std::getline(file, line)) {
            /*
            * Borrowed from:
            *   - https://stackoverflow.com/questions/20755140/split-string-by-a-character
            *   - http://www.cplusplus.com/reference/sstream/stringstream/stringstream/
            */
            std::replace(line.begin(), line.end(), ',', ' '); 
            // Setup the string stream to read each string split by the space
            stringstream split(line);

            int id;
            int request_time;
            int duration;

            // First is id
            split >> id;
            // Next is request time since CPU started
            split >> request_time;
            // Last is te duration
            split >> duration;

            // Set these values in the array of jobs
            jobs[job_count++].memberInit(id, request_time, duration);
        }

        file.close();
    }

    /*
    * Part One
    * Round Robin the jobs, schedule, run, and complete all jobs
    */
    queue<ScheduledJob> *waitingQueue = new queue<ScheduledJob>();

    // Put all jobs in the waiting queueue
    for (int i = 0; i < JOB_COUNT; i++) {
        ScheduledJob job = jobs[i];
        waitingQueue->push(job);
    }

    // These are the jobs that are ready to run
    queue<ScheduledJob> *readyQueue = new queue<ScheduledJob>();

    // The current time of the CPU
    int current_ms = 0;
    // How many cycles the CPU has ran
    int cpu_cycles = 0;

    do {

        // First set new jobs in the ready queue the match the current ms
        while (waitingQueue->front().get_requested_time() == current_ms) {
            ScheduledJob job = waitingQueue->front();
            readyQueue->push(job);
            waitingQueue->pop();
        }

        // Next schedule and run job
        ScheduledJob runningJob = readyQueue->front();
        // Remove it
        readyQueue->pop();
        // Run the job
        int time_ran = runningJob.run_job();

        // If the job is not completed return it to the back of the queue
        if (!runningJob.is_completed()) {
            readyQueue->push(runningJob);
        }

        // Add the time we ran to the current time
        current_ms += QUANTUM_MS;
        cpu_cycles++;

    } while (!readyQueue->empty());


    // Reset the array for Part Two
    for (int i = 0; i < JOB_COUNT; i++) {
        ScheduledJob job = jobs[i];
        job.reset_job();
    }

    // Empty for Part Two
    delete waitingQueue;
    delete readyQueue;

    // Prompt user of Part One Completion, so they can see the results
    cout << "Part One Completed in ";
    cout << cpu_cycles;
    cout << " cpu cycles, press any key to continue (not space!).";

    cin.get();

    /*
    * Part Two
    * Round Robin the jobs, schedule, run, and complete all jobs except
    * with 4 CPUs instead of 1. This means we need a common waiting queue
    * and 4 seperate ready queues
    */

    waitingQueue = new queue<ScheduledJob>();

    // Put all jobs in the waiting queueue
    for (int i = 0; i < JOB_COUNT; i++) {
        ScheduledJob job = jobs[i];
        waitingQueue->push(job);
    }

    // Unlike part one we now have 4 ready queues for 4 CPUs
    queue<ScheduledJob> *readyQueue1 = new queue<ScheduledJob>();
    queue<ScheduledJob> *readyQueue2 = new queue<ScheduledJob>();
    queue<ScheduledJob> *readyQueue3 = new queue<ScheduledJob>();
    queue<ScheduledJob> *readyQueue4 = new queue<ScheduledJob>();

    current_ms = 0;
    cpu_cycles = 0;

    do {

        // First set new jobs in the ready queue the match the current ms
        int thread_index = 0;
        while (waitingQueue->front().get_requested_time() == current_ms) {
            ScheduledJob job = waitingQueue->front();
            waitingQueue->pop();

            // Determine CPU to queue up
            if (thread_index++ % 2 == 0) {
                readyQueue1->push(job);
            } else {
                readyQueue2->push(job);
            }
        }

        // Next we need to schedule and run the jobs on each CPU
        for (int i = 0; i < 4; i++) {
            queue<ScheduledJob> *q;
            switch (i) {
                case 0:
                    q = readyQueue1;
                    break;
                case 1:
                    q = readyQueue2;
                    break;
                case 2:
                    q = readyQueue3;
                    break;
                case 3:
                    q = readyQueue4;
                    break;
            }

            // Get the job from the CPU and run it
            if (!q->empty()) {
                ScheduledJob runningJob = q->front();
                q->pop();

                // Run the job
                int time_ran = runningJob.run_job();

                // If the job is not completed return it to the back of the queue
                if (!runningJob.is_completed()) {
                    // Cycically add to 3 and 4
                    if (readyQueue3->size() > readyQueue4->size()) {
                        readyQueue4->push(runningJob);
                    } else {
                        readyQueue3->push(runningJob);
                    }
                }
            }
        }

        // Add the time we ran to the current time
        current_ms += QUANTUM_MS;
        cpu_cycles++;

    // Keep going till ALL CPUs are done
    } while (!readyQueue1->empty() || !readyQueue2->empty() || !readyQueue3->empty() || !readyQueue4->empty());

    // Notify user of Part Two Completion
    cout << "Part Two Completed in ";
    cout << cpu_cycles;
    cout << " cpu cycles." << endl;

    return 0;
}