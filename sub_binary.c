
void parallel_binary_search_function();
void* parallel_binary_search(void*);
void insert(int);
void printList();
void serial_version(int[]);
void parallel_version();
void printArray(int[],int);
int* serial_search(int[],int[]);
int* sortArray(int[],int);
void generalization();
int get_gpu_count();


//generalization for the main function
void generalization(){
	printf("Enter the size of the array (will be multiplied by the number of your computer processors):\t");
	scanf("%d",&N);
	
	p = get_gpu_count();
	N = N*p;

	//dynamic allocation 
	array = (int*)malloc(sizeof(int)*N);
	int s = (N/2)*(-1);
	int f = (N/2);
	for(int i=0;i<N;i++){
		int num = s+ rand()%(f-s+1);
		array[i] = num;
	}
	sortedarray = sortArray(array,N);
	arr_parallel = sortArray(array,N);
	printArray(sortedarray,N);

	printf("\nEnter the target value:\t");
	scanf("%d",&target);

}


//delete list from the linked list
void deleteList(struct node** head_ref){
	struct node* current = *head_ref;
	struct node* next;

	while(current!=NULL){
		next = current->next;
		free(current);
		current = next;
	}

	*head_ref = NULL;
}

//insert numbers on the linked list
void insert(int data){
	//creating a link
	struct node *link = (struct node*)malloc(sizeof(struct node));

	int flagger = 0;
	struct node *potr = head;

	//checks if the data is already existing in the linked list
	while(potr!=NULL){
		if (potr->data == data){
			flagger = 1;
		}
		potr=potr->next;
	}

	if (!flagger){
		link-> data = data;
		link->next = head;
		head = link;

	}
}

//prints the list from the linked list
void printList(){
	struct node *ptr = head;

	while(ptr!=NULL){
		printf(" %d\t ",ptr->data);
		ptr=ptr->next;
	}
}

//gets number of processors. can be used for MACOS, Windows, and other OS
int get_gpu_count(){
	#ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
	#elif MACOS
	    int nm[2];
	    size_t len = 4;
	    uint32_t count;
	 
	    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
	    sysctl(nm, 2, &count, &len, NULL, 0);
	 
	    if(count < 1) {
	    nm[1] = HW_NCPU;
	    sysctl(nm, 2, &count, &len, NULL, 0);
	    if(count < 1) { count = 1; }
	    }
	    return count;
	#else
	    return sysconf(_SC_NPROCESSORS_ONLN);
	#endif
}


void parallel_binary_search_function(){

	//sets up core affinity, and computes for the subarray size
	int subarray_size = N/p;


	pthread_t binary_array[p];
	BINARIES binary_structure[p];

	//set affinity variables
	pthread_attr_t attr;
	cpu_set_t cpus;
	pthread_attr_init(&attr);

	//does the loop for each thread
	int marker = (-1);
	for(int i = 0; i<p; i++){
		CPU_ZERO(&cpus);
		CPU_SET(i, &cpus);
		pthread_attr_setaffinity_np(&attr,sizeof(cpu_set_t),&cpus);

		binary_structure[i].s = (marker+1);
		binary_structure[i].f = (marker+subarray_size);

		pthread_create(&binary_array[i],&attr,parallel_binary_search,&binary_structure[i]);
		pthread_join(binary_array[i],NULL);

		marker = marker+subarray_size;
	}	

	//parameters to pass: starting index and ending index 


}


//function for the parallel binary search
void *parallel_binary_search(void *arg){

	//setting up index to be left and right
	BINARIES * temp;
	temp = (BINARIES *) arg;

	int left = temp->s;
	int right = temp->f;
	int mid;
	while(left<=right){
		mid = (left+right)/2;
		if (arr_parallel[mid]==target){
			//checks every part to check if there is anything that is equal to the target value
			counter=0;
			int answers[N];
			for(int i=mid-1;i>=0;i--){
				if (target == arr_parallel[i]){
					answers[counter] = i;
					counter++;
				}
			}
			answers[counter] = mid;
			counter++;
			for (int i=mid+1;i<N;i++){	
				if(target==arr_parallel[i]){
					answers[counter] = i;
					counter++;
				}
			}

			for (int i=0;i<counter;i++){
				insert(answers[i]);
			}

			break;
		}else if(target<arr_parallel[mid]){
			right = mid-1;
		}else{
			left = mid+1;
		}
	}	
}

//serial search version for the binary search
int* serial_search(int arr[],int arryA[]){
	//defining the first left and right index value
	int left = 0;
	int right = N-1;
	int mid;
	while(left<=right){
		mid = (left+right)/2;
		if (arr[mid]==target){
			//checks almost every part to check if the target value is in that part
			counter=0;
			int answers[N];
			for(int i=mid-1;i>=0;i--){
				if (target == arr[i]){
					answers[counter] = i;
					counter++;
				}
			}
			answers[counter] = mid;
			counter++;
			for (int i=mid+1;i<N;i++){	
				if(target==arr[i]){
					answers[counter] = i;
					counter++;
				}
			}

			for (int i=0;i<counter;i++){
				arryA[i] = answers[i];
			}
			arryA[counter+1] = counter;
			return arryA;
		}else if(target<arr[mid]){
			right = mid-1;
		}else{
			left = mid+1;
		}
	}	
	//returns -1 if target value is not in the array
	arryA[0] = -1;
	arryA[1] = 0;
	return arryA;
}


//collates the functions needed for the serial version
void serial_version(int arryA[]){
	printf("===SERIAL VERSION===\n");
	int *answer = serial_search(sortedarray,arryA);
	if(counter>0){
		printf("Target: \t %d\n",target);
		printf("Indexes:\t");
		printArray(answer,counter);
	}else{
		printf("[!] Target is not in the array\n");
	}printf("\n");
}

//collates the functions needed for the parallel version
void parallel_version(){
	printf("===PARALLEL VERSION===\n");
	parallel_binary_search_function();

	printf("Target: \t %d\n",target);
	printf("Indexes:\t");
	if(head == NULL){
		printf("[!] Target is not in the array\n");
	}else{
		printList();
	}
	printf("\n");
	deleteList(&head);
}

//prints the array needed
void printArray(int arr[], int N){
	for(int i=0;i<N;i++){
		printf(" %d\t ",arr[i]);
	}
}

//sorts the array (to ready for the binary search)
int* sortArray(int arr[],int N){
	int min,temp;
	for(int i=0;i<N-1;i++){
		min = i;	
		for (int j=i;j<N;j++){
			if(arr[j]<arr[min]){
				min = j;
			}
		}
		//swapping for the first element
		temp = arr[min];
		arr[min] = arr[i];
		arr[i] = temp;
	}

	return arr;
}