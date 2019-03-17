/*
TO COMPILE: g++ -std=c++11 proj1.cpp
PART FOUR: Last, but not least, develop a scheduling methodology that can deal with the sequential arrival of the 200
processes. That is, the scheduler cannot inspect the entire set of processes but must schedule them one by
one in the order that they arrive. What is the best turnaround time that you can achieve?
*/
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

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
        CPU() 
        { 
            mem = 2048; //MB
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

          } //end gen_processes()
          
          void gen_CPU()
          {
          
              int i = 0;
              for (i=0; i<5; i++)
              {
                  cpu = new(CPU);
                  cpus.push_back(cpu);
              }
             
              return;
              
          }
          void assign()
          {
              int len = processes.size();
              for(int i=0; i< len; i++)
              {
                  cpus.at(i%5)->assigned.push_back(processes.at(i));
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
  
};

int main()
{
    int i = 0;
    monitor* mon;
    mon = new monitor;
    //generate list of processes
    mon->gen_processes();
    //Make processors
    mon->gen_CPU();
   //assign processes to processors using FIFO
    mon->assign();
    //print lists
    mon->print_lists();

    return 0;
}