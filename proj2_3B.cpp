/*
TO COMPILE: g++ -std=c++11 proj2_A.cpp
PART ONE: Use the system calls malloc() and free() to dynamically allocate and de-allocate memory to your
processes upon their arrival and departure. Measure the total system time that is required to simulate the
execution of your 50 processes.
*/
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm> //sort
#include <time.h>
#include <chrono>
#include <ctime>
#include <queue>          // std::queue
#include <stdlib.h>     // malloc, free, rand */
using namespace std;

class Process
{
    private:
        int pid;
        unsigned long int cycles; //# of cycles
        float mem; //memory requirement in MB
        time_t arrival, completion; //time process arrived and time process left queue
		int mem_index;
		
    public:
        int getmem() { return mem; }
        int getcycles() { return cycles; }
        int get_pid() { return pid; }
		int get_index() {return mem_index;}
        
        Process (int x, int y, int z) // PID, CYCLES, MEM
        {
            pid = x;
            cycles = y;
            mem = z;
        }
		void set_index(int index)
		{
			mem_index = index;
		}
		int cycle()
		{
			cycles --;
			if(cycles > 0)
				return 0;
			else
			{
				cout <<"P " << get_pid() << " complete" <<endl;
				return 1;
			}
		}
};
      
class Monitor
{
  
    private:
        Process* P;
        vector<Process*> processes;
		vector<Process*> running;
		vector<Process*> wait;
		vector<int> memory50;
		vector<int> memory10;
		vector<int> chunk_index;
		vector<int> chunk_size; 
    public:
		void init()
		{
			//gen 50 processes
			memory10.resize(1000,0); //10% of memory
			get_chunks(); // find contigious blocks of memory
			gen_processes();
			print_wait();
		}
		~Monitor()
		{
			for(int i = 0;i< processes.size();i++)
			{
				delete processes.at(i);
			}

		}
		void run()
		{
			int i, p_counter=50-1, finished;
			while(p_counter)
			{
				//cout << p_counter << endl;
				print_wait();
				print_chunks();
				cout << "\n" << endl;
				for(int i = 0; i < running.size(); i++)
				{
					finished = running.at(i)->cycle();
					if(finished) //if 1
					{
						//take from the wait queue and add it to the process queue
						//if the memory of the first process in the queue is bigger then the current process
						int temp = wait.size();
						my_free(running.at(i));
						for(int j=0; j < temp; j++)
						{
							my_malloc(wait.at(j));
							wait.erase(wait.begin()+j);
							break;
						}

						cout << "Removed process: " << running.at(i)->get_pid() << endl; 
						running.erase(running.begin() + i);	
						print_running();
						p_counter--;
						break;
					}
				}
				//print_running();
			}
			return;
		}
		void get_chunks() // call after every malloc and free
		{
			int index=0, counter =0, i=0;
			chunk_index.clear();
			chunk_size.clear();
			
			while(i < 999) //check all memeory
			{
				if(memory10.at(i) != 0)
				{
					i += memory10.at(i); //get the index for the block of mem right after this process
				}
				else
				{
					index = i; // first index of contigious block
					counter =0;
					while(memory10.at(i) == 0)
					{
						if(i >= 999)
							break;
						i++;
						counter++;
					}
					chunk_index.push_back(index); //saves the first index of the contigous chunk
					chunk_size.push_back(counter); // saves the size of the contigious chunk
				}
				
			}
		}
		void print_chunks()
		{
			for(int i = 0; i < chunk_size.size(); i++)
			{
				cout << "Chunk index: " << chunk_index.at(i) << ", size: " << chunk_size.at(i) << endl;
			}
		}
		void print_wait()
		{
			cout <<"\nsize of wait: " << wait.size() << endl;
			cout << "Waiting Queue:\n";
			for(int i =0; i < wait.size(); i++)
				cout << "Process PID: "<< wait.at(i)->get_pid() << " cycles: "<< wait.at(i)->getcycles() << " memory: " << wait.at(i)->getmem() << endl;
		}
		void print_running()
		{
			cout <<"\nsize of running: " << running.size() << endl;
			cout << "Running Queue:\n";
			for(int i =0; i < running.size(); i++)
				cout << "Process PID: "<< running.at(i)->get_pid() << " cycles: "<< running.at(i)->getcycles() << " memory: " << running.at(i)->getmem() << endl;
		}
		void print_processes()
		{
			cout <<"\nsize of processes: " << processes.size() << endl;
			cout << "Process Queue:\n";
			for(int i =0; i < processes.size(); i++)
				cout << "Process PID: "<< processes.at(i)->get_pid() << " cycles: "<< processes.at(i)->getcycles() << " memory: " << processes.at(i)->getmem() << endl;
		}
		void my_malloc(Process *p) //assigning data
		{
			int size = p->getmem(); //get amount of memory needed
			int best = 1000; //best fit block
			int index=0;
			for(int i =0; i < chunk_size.size(); i++)
			{
				//if the amount of memory needed is avaliable then get the index
				if(size <= chunk_size.at(i))
				{
					index = chunk_index.at(i); //get index of best fit block
				}
				else
				{
					cout << "No block of memory could be found for process " << p->get_pid() << " put on waiting queue"<<endl;
					p->set_index(0);
					wait.push_back(p);
					print_wait();
					//processes.erase(processes.begin() + i);
					return;
				}
			}
			memory10.at(index) = size; //allocate size for the process in mem
			p->set_index(index); // tell the process where it's memory is
			get_chunks();
			if(index > 0)
				running.push_back(p);
			print_running();
			return;
		}
		void my_free(Process *p)
		{
			int index = p->get_index();
			memory10.at(index)=0;
			get_chunks();
			//print_chunks();
			//print_processes();
			//print_running();
			//print_wait();
		}
        void gen_processes()
        {
			srand((unsigned) time(NULL)); //seed random number generator
            unsigned int cycles;
            float mem;
			  
            for(int i=0; i<50; i++) //generate 50 processess
            {
				cycles = rand() % (100-10) + 10; // min 10 cycles max 100
				mem = rand() % (400-10) +10; //# of KB
				P = new Process(i, cycles, mem);
				processes.push_back(P);
				my_malloc(P); //give it memory
				cout << "P #" << i << " created. Size:" << mem << ", first memory index:" << P->get_index() << endl;
				//delete P;
			}
			//delete P;
            return;
        } //end gen_processes()
};



int main()
{
	Monitor* mon;
	mon = new Monitor;
	mon->init();
	auto start = std::chrono::system_clock::now(); //get start time
	mon->run();
	auto end = std::chrono::system_clock::now(); //get end time
	
	std::chrono::duration<double> elapsed_seconds = end-start; //compute the total time
	cout << "\nElapsed time to run program: " << elapsed_seconds.count() << "s\n\n";
	
	delete mon;
	return 0;
}