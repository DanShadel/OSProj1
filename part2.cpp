/*
TO COMPILE: g++ -std=c++11 part2.cpp
PART TWO: Develop your own memory management system with corresponding function calls my_malloc() and
my_free() to manage a pre-defined block of memory of size 10MB. The function my_malloc() will try to
find the required chunk of memory within the pre-defined 10MB block and allocates it to an arriving
process. Upon completion, the memory partition is returned to the memory pool via the function
my_free(), to be used by other processes. Compare the performance of your memory manager to the
performance of malloc() and free(). Note that your memory manager only calls the system’s malloc()
once to request the initial 10MB block, hence you are reducing the number of context switches.
*/

#include <iostream>
#include <vector>
#include <algorithm> //sort
#include <time.h>
#include <stdlib.h>
using namespace std;
class Process // taken from project 1
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
        
        Process (int x, int y, float z) // PID, CYCLES, MEM
        {
            pid = x;
            cycles = y;
            mem = z;
        }
        void set_index(int index)
        {
        	mem_index = index;
        }
        int cycle() //"processing" one cycle
        {
        	cycles--;
        	if(cycles > 0)
        	{
        		
        		return 0;
        	}
        	else if (cycles <=0 )
        	{
        		cout << "P " << get_pid() << " complete" << endl;
        		return 1; //process complete, call my_free
        	}
   
        }
    
};
class Monitor{

	private:
		vector<int> memory;
		vector<int> chunk_size;
		vector<int> chunk_index;
		Process* P;
        vector<Process*> processes;

	public:
		void init() // init vectors and gen processes.
		{
			//gen 50 processes
			memory.resize(10000,0); // malloc(10000), init all to 0
			get_chunks();//find contiguous blocks of memory
			gen_processes();
			
		}
		void run()
		{
			int i;
			int p_counter = 50;
			int finished;
			while(p_counter)
			{
				for (i= 0;i<p_counter;i++)
				{
					finished = processes.at(i)->cycle();
					if(finished)
					{
						my_free(processes.at(i));
						p_counter--;
						processes.erase(processes.begin() + i);
					
					}
				}
			}
			return;
		}
		void get_chunks() // call after every malloc and free
		{
			int index = 0;
			int counter = 0;
			int i= 0;
			chunk_index.clear();
			chunk_size.clear();

			while(i < 9999) // check all memory
			{
				if(memory.at(i) != 0)
				{
					i += memory.at(i); // get the index for the block of mem right after this process
				}
				else
				{
					index = i; // first index of contiguous block
					counter = 0;
					while(memory.at(i) == 0)
					{
						if(i >= 9999)
						{
							break;
						}
						i++;
						counter++;
					}
					chunk_index.push_back(index); // saves the first index of the contigous chunk
					chunk_size.push_back(counter);//saves the size of the contiguous chunk
				}
			}
		
		}//end get_chunks
		void print_chunks()
		{
			for(int i = 0; i < chunk_size.size();i++)
			{
				cout << "Chunk index: " << chunk_index.at(i) << ", size: " << chunk_size.at(i) << endl;
			}
		}//end print_chunks
		void my_free(Process *p) // freeing up data
		{
			int index = p->get_index();
			memory.at(index) = 0;
			get_chunks();
			print_chunks();

		} //end my_free
		void my_malloc(Process *p) // assigning data
		{
			int size = p->getmem(); //get the amount of memory needed for a process
			int best = 100000; // best fit block
			int index;
			for(int i = 0; i < chunk_size.size(); i++) // find a block that fits
			{
				if( size <= chunk_size.at(i))
				{
					index = chunk_index.at(i); //get the index of the best fit block
				}
				else
				{
					cout << "No block of memory could be found for process " << p->get_pid() << endl;
					return;
				}
			}
			memory.at(index) = size; //allocate size for the process in memory
			p->set_index(index); //tell the process where its memory is
			get_chunks();
			return;
		} //end my_malloc

		void gen_processes() //taken from Project 1
		{
		  srand((unsigned) time(NULL)); //seed random number generator
		  unsigned int cycles;
		  float mem;
		  for(int i=0; i<50; i++) //generate 50 processess
		    {
		        cycles = rand() % (100 - 10) + 10; // min 10 cycles max 100
		        mem = rand() % (400-10) + 10; // # of kB
		        P = new Process(i, cycles, mem);
		        processes.push_back(P);
		        my_malloc(P);//give it memory
		        cout << "P #" << i << " created. Size:" << mem << ", first memory index:" << P->get_index() << endl;
		    }
		    	

		 return;
		} //end gen_processes() 
};
int main()
{

    Monitor* mon;
    mon = new Monitor;
    mon->init();
    mon->run();

	return 0;
}
