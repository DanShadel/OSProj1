/*
TO COMPILE: g++ -std=c++11 proj1.cpp
PART ONE: Suppose that all 5 processors are identical (i.e., same speed and memory), develop and implement a
scheduling algorithm that assigns the set 200 processes to the 5 processors such that the total turnaround
time to complete all 200 processes is minimized. 
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
        unsigned long int cycles; //# of cycles
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
        CPU(int x) // MEM, 
        { 
            mem = x; //MB
            speed = 3; //GHz
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
                auto end = std::chrono::system_clock::now(); //get end time
                std::chrono::duration<double> elapsed_seconds = end-start; //compute the total time
                cout << "\nElapsed time to sort processes: " << elapsed_seconds.count() << "s\n\n";
               // sort(processes.begin()->getcycles(), processes.end()->getcycles()); // maybe?????  
                for(int i=0; i<200; i++) //generate 200 processess
                {
                    cout << "process #" << processes.at(i)->get_pid() << ":    " <<  processes.at(i)->getmem() << endl;
                }     
             return;
          } //end gen_processes()
          
          void gen_CPU()
          {
            //P1 and P2 = 2gb    
            cpu = new CPU(2048);
            cpus.push_back(cpu);
            cpu = new CPU(2048);
            cpus.push_back(cpu);

            //P3 and P4 = 4gb
            cpu = new CPU(4096);
            cpus.push_back(cpu);
            cpu = new CPU(4096);
            cpus.push_back(cpu);

            //P5 = 8gb
            cpu = new CPU(8192);
            cpus.push_back(cpu);


         
              return;
              
          }
          void assign()//will change this function
          {
              int i;
              int len = processes.size()-1;

              for(i=len; processes.at(i)->getmem() > 4096; i--) //assign processes that have to go on the biggest processor first
              {
                  cpus.at(4)->assigned.push_back(processes.at(i));
                  processes.pop_back();
              }

              len = processes.size()-1; //remaining unassigned processes
              for(i = len; processes.at(i)->getmem() > 2048; i--)
              {
                cpus.at((i%2)+2)->assigned.push_back(processes.at(i));
                processes.pop_back();
              }

              len = processes.size()-1; //remaining unassigned processes
              for(i = len; i >= 1 ; i--)
              {
                cpus.at((i%2))->assigned.push_back(processes.at(i));
                processes.pop_back();
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
                  cout <<  endl << endl << "processor " << i << " :" <<endl;
                  for(j=0;j<size;j++)
                  {
                      cout <<"process id " << cpus.at(i)->assigned.at(j)->get_pid() << ":     " <<   cpus.at(i)->assigned.at(j)->getmem() <<endl; // print out the list
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
                      while(processes.at(i)->getmem() > processes.at(j)->getmem()) //find the first element with bigger cycles than 
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
    mon->assign();
    auto end = std::chrono::system_clock::now(); //get end time
    mon->print_lists();
    
    std::chrono::duration<double> elapsed_seconds = end-start; //compute the total time
    cout << "\nElapsed time to sort processes: " << elapsed_seconds.count() << "s\n\n";
    //dispatch SJF to free processors

    return 0;
}