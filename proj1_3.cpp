/*
TO COMPILE: g++ -std=c++11 proj1_3.cpp
PART THREE: Modify your scheduling algorithm developed in Q1 and Q2 and
show the best assignment of processes if the processors are identical,
i.e., 8GB memory available but differ in speed. Specifically: PA= PB = 2 GHz,
PC= PD = 3 GHz and PE= 4 GHz
*/
#include <iostream>
#include <vector>
#include <algorithm> //sort
#include <time.h>
#include <stdlib.h>
#include <chrono>
#include <ctime>

using namespace std;

class process
{
    private:
        int pid;
        unsigned int cycles; //# of cycles
        float mem; //memory requirement in MB
        time_t arrival, completion; //time process arrived and time process left queue

    public:
        int getmem() { return mem; }
        int getcycles() { return cycles; }
        int get_pid() { return pid; }

        process (int x, int y, float z) // PID, CYCLES, MEM
        {
            pid = x;
            cycles = y;
            mem = z;
        }

};
class CPU
{
    private:
        int mem; //MB
        int speed; //GHz
        bool occupied; //is there a process here

    public:
        bool gettaken() { return occupied; } // returns if a processor is occupied
        vector <process*> assigned;
        CPU(int speed_in)
        {
            mem = 2048; //MB
            speed = speed_in; //GHz
            occupied = false;
        }

        void add_process(process *P)
        {
            assigned.push_back(P);
        }

};

class monitor
{

    private:
        process* P;
        CPU* cpu;
        vector<CPU*> cpus;
        vector<process*> processes;

    public:

          void gen_processes()
          {
              srand((unsigned) time(NULL)); //seed random number generator
              unsigned int cycles;
              float mem;
              for(int i=0; i<200; i++) //generate 200 processess
                {
                    cycles = rand() % (50000000000000 - 10000000) + 10000000; // % (5x10^13 - 1x10^7) + 1x10^7
                    mem = rand() % (8192-1) + 1;
                    P = new process(i, cycles, mem);
                    processes.push_back(P);
                }
                auto start = std::chrono::system_clock::now(); //get start time
                //do insertion sort here
                insertion_sort(processes);
                auto end = std::chrono::system_clock::now(); //get start time
                 std::chrono::duration<double> elapsed_seconds = end-start; //compute the total time
                 cout << "\nElapsed time to sort processes: " << elapsed_seconds.count() << "s\n\n";

                for(int i=0; i<200; i++) //generate 200 processess
                {
                    cout << "process # " << processes.at(i)->get_pid() << ": " <<  processes.at(i)->getcycles() << endl;
                }
             return;
          } //end gen_processes()

          void gen_CPU()
          {
              int i = 0;
		int speed = 3; //GHz
              for (i=0; i<5; i++)
              {
		  if(i == 0 || i == 1)
			speed = 2;
		  if(i == 2 || i == 3)
			speed = 3;
		  if(i == 4)
			speed = 4;
                  cpu = new CPU(speed);
                  cpus.push_back(cpu);
              }

              return;
          }

          void assign()
          {
		//Longer processes go to faster processors without violating SJF property.
		int len = processes.size();
		int increment = len/5; //5 processors

		for(int i = 0; i < 2*increment; i++) //PA = PB = 2GHz, slow
		{
			cpus.at(i%2)->assigned.push_back(processes.at(i));
		}
		for(int i = 2*increment; i < 4*increment; i++) //PC = PD = 3GHz, medium
		{
			cpus.at(i%2 + 2)->assigned.push_back(processes.at(i));
		}
		for(int i = 4*increment; i < len; i++) //PE = 4GHz, fast
		{
			cpus.at(4)->assigned.push_back(processes.at(i));
		}


              return;
          }

          void print_lists()
          {
              int i = 0, j=0;

              int size;

              for(i=0;i<5;i++)
              {
                  size = cpus.at(i)->assigned.size(); // get size of the list of the processess assigned to each processor
                  cout << endl << "processor " << i << "    :" << endl << endl << endl;
                  for(j=0;j<size;j++)
                  {
                      cout <<"process id " << cpus.at(i)->assigned.at(j)->get_pid() << ": " <<   cpus.at(i)->assigned.at(j)->getcycles() <<endl; // print out the list
                  }
              }
              return;
          }

            void insertion_sort(vector<process*> &processes) //https://stackoverflow.com/questions/5709637/c-vector-insertion-sort-algorithm-method-pass-vector-into-method
            {

                  int i = 0, j = 0;
                  process *temp;
                  int n = processes.size(); //https://www.geeksforgeeks.org/vector-in-cpp-stl/

                  for(i=0;i<n;i++)
                  {
                      j=0;
                      while(processes.at(i)->getcycles() > processes.at(j)->getcycles()) //find the first element with bigger cycles than 
                      {
                          j++;
                      }
                      processes.insert(processes.begin()+j,processes.at(i)); //put it right before that element
                      processes.erase(processes.begin()+i+1);//remove the duplicate element

                  }

                return;
            }




};

int main()
{
    int i = 0;
    monitor* mon;
    mon = new monitor;
    //generate list of processes & sort them in increasing order
    mon->gen_processes();
    //Make processors

    mon->gen_CPU();

    auto start = std::chrono::system_clock::now(); //get start time
	//dispatch short processes to slow processors and long processes to fast processors
    mon->assign();
    auto end = std::chrono::system_clock::now(); //get end time

    mon->print_lists();

    std::chrono::duration<double> elapsed_seconds = end-start; //compute the total time
    cout << "\nElapsed time to assign processes: " << elapsed_seconds.count() << "s\n";
    return 0;
}
