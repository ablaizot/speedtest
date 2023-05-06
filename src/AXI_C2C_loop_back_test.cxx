#include "uhalspeedtest.hh"
#include <chrono>
#include <random>

int MDTTP::AXI_C2C_loop_back_test(string node, uint32_t loops)
{
  uint32_t write_mem;
  uint32_t read_mem;

  double speed;

  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<unsigned int> distrib(0, 0xFFFFFFFF);

  auto begin = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
    
  cout << endl << "Loop test of AXI C2C data integrity and speed" << endl 
       << std::dec << loops << " loops doing write-read of random 32-bit words to " << node 
	    << endl << endl; 

  for(uint32_t i = 0; i < loops; ++i) {

    write_mem = distrib(gen);
    SM->WriteRegister(node,write_mem);
    read_mem = SM->ReadRegister(node);

    if (write_mem != read_mem) {
      cout << "R/W error: loop " << i << ", write_mem = " << std::hex << write_mem 
		<< ", read_mem = " << read_mem << endl << endl;
      return -1;
    }

    if (i < 10) {
      cout << "write_mem = " << std::hex << write_mem << ", read_mem = " << read_mem << endl;
    }
       
    if (i%100000 == 0 && i != 0) {
      end = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
      speed = 2.*32.*i/duration;
      cout << std::dec << i << " reads done, speed = " << speed <<  " Mbps" << endl;
    }

  }
   
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();

  cout << endl << "Speed test: " << std::dec << loops << " write-reads of " << node << endl;
  cout << duration << " us total, average : " << duration / loops << " us." << endl;

  speed = 2.*32.*loops/duration;
  cout << "Speed = " << speed << " Mbps" << endl;

  return 0;
}

