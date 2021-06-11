#define _GNU_SOURCE

#ifdef _WIN32
#include <windows.h>
#elif MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

#include <sched.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
int counter;
int target;
int* array;
int* sortedarray;
int* arr_parallel;
int p=0;
int N=0;




//importing functions

typedef struct binaryindex{
	int s;
	int f;
}BINARIES;

//linked list for all indexes
struct node{
	int data;
	struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

#include "sub_binary.c"



int main(){

	generalization();
	int arryA[N];

	

	//=== [START] === Comment/Uncomment This Part

	// serial_version(arryA);
	parallel_version();

	//=== [END] === Comment/Uncomment This Part

	return 0;
}



//REFERENCES:

//REFERENCES:
//https://stackoverflow.com/questions/33150887/generate-adjacency-matrix-of-undirected-graph
//https://www.programiz.com/dsa/graph-dfs
//https://scanftree.com/programs/c/c-program-to-implement-depth-first-search-dfs_serial/
//Function to get number of CPU Cores: http://www.cprogramming.com/snippets/source-code/find-the-number-of-cpu-cores-for-windows-mac-or-linux
//Setting Core Affinities: https://stackoverflow.com/questions/25472441/pthread-affinity-before-create-threads
//https://www.geeksforgeeks.org/complexity-analysis-of-binary-search/
//https://www.geeksforgeeks.org/introduction-to-parallel-computing/
// https://www.geeksforgeeks.org/c-program-to-sort-an-array-in-ascending-order/