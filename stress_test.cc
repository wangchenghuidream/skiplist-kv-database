#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <map>
#include<unordered_map>
#include "skiplist.h"

#define NUM_THREADS 1
#define TEST_COUNT 100000
SkipList<int, std::string> skipList(18);

std::map<int, std::string> mp1;
std::unordered_map<int, std::string> mp2;
void *insertElement_map(void* threadid) 
{
    long tid; 
    tid = (long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; ++i) 
	{
        ++count;
		mp1[rand() % TEST_COUNT] = "a";
		//mp1.insert(rand() % TEST_COUNT, "a")
	}
	std::cout<<"count:"<<mp1.size();
    pthread_exit(NULL);
}

void *insertElement_umap(void* threadid) 
{
    long tid; 
    tid = (long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; ++i) 
	{
        ++count;
		mp2[rand() % TEST_COUNT] = "a";
		//mp1.insert(rand() % TEST_COUNT, "a")
	}
	std::cout<<"count:"<<mp1.size();
    pthread_exit(NULL);
}

void *insertElement(void* threadid) 
{
    long tid; 
    tid = (long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; ++i) 
	{
        ++count;
		skipList.insert_element(rand() % TEST_COUNT, "a"); 
	}
	std::cout<<"count:"<<skipList.size();
    pthread_exit(NULL);
}

void *getElement(void* threadid) 
{
    long tid; 
    tid = (long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; ++i) 
	{
        ++count;
		skipList.search_element(rand() % TEST_COUNT); 
	}
    pthread_exit(NULL);
}

void *getElement_map(void* threadid) 
{
    long tid; 
    tid = (long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; ++i) 
	{
        ++count;
		mp1.find(rand() % TEST_COUNT);
	}
    pthread_exit(NULL);
}

void *getElement_umap(void* threadid) 
{
    long tid; 
    tid = (long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; ++i) 
	{
        ++count;
		mp2.find(rand() % TEST_COUNT);
	}
    pthread_exit(NULL);
}

void *delElement(void* threadid) 
{
    long tid; 
    tid = (long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; ++i) 
	{
        ++count;
		skipList.delete_element(rand() % TEST_COUNT); 
	}
    pthread_exit(NULL);
}

void *delElement_map(void* threadid) 
{
    long tid; 
    tid = (long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; ++i) 
	{
        ++count;
		auto it1 = mp1.find(rand() % TEST_COUNT);
		if(it1 != mp1.end())
			mp1.erase(it1);
	}
    pthread_exit(NULL);
}

void *delElement_umap(void* threadid) 
{
    long tid; 
    tid = (long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for (int i=tid*tmp, count=0; count<tmp; ++i) 
	{
        ++count;
		auto it1 = mp2.find(rand() % TEST_COUNT);
		if(it1 != mp2.end())
			mp2.erase(it1);
	}
    pthread_exit(NULL);
}


int main() {
    srand (time(NULL));  
    {

        pthread_t threads[NUM_THREADS];
        int rc;
        int i;
		int k = 0;
		for(int k =0; k<100000; ++k)
		{
			//skipList.insert_element(k, "a");
			//mp1[k] = "a";
			mp2[k] = "a";
		}
		
        auto start = std::chrono::high_resolution_clock::now();

        for( i = 0; i < NUM_THREADS; i++ ) {
            std::cout << "main() : creating thread, " << i << std::endl;
            //rc = pthread_create(&threads[i], NULL, insertElement, (void *)i);
			//rc = pthread_create(&threads[i], NULL, insertElement_map, (void *)i);
			//rc = pthread_create(&threads[i], NULL, insertElement_umap, (void *)i);
			
			//rc = pthread_create(&threads[i], NULL, getElement, (void *)i);
			//rc = pthread_create(&threads[i], NULL, getElement_map, (void *)i);
			//rc = pthread_create(&threads[i], NULL, getElement_umap, (void *)i);
			
			//rc = pthread_create(&threads[i], NULL, delElement, (void *)i);
			//rc = pthread_create(&threads[i], NULL, delElement_map, (void *)i);
			rc = pthread_create(&threads[i], NULL, delElement_umap, (void *)i);
			
            if (rc) {
                std::cout << "Error:unable to create thread," << rc << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for( i = 0; i < NUM_THREADS; i++ ) {
            if (pthread_join(threads[i], &ret) !=0 )  {
                perror("pthread_create() error"); 
                exit(3);
            }
        }
        auto finish = std::chrono::high_resolution_clock::now(); 
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "insert elapsed:" << elapsed.count() << std::endl;
    }
	pthread_exit(NULL);
    return 0;

}
