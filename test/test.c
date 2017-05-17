#include <DANG/vector.h>
#include <assert.h>
#include <stdlib.h>
int put_get_test(){
	vector_t * test = vector_init(sizeof(int),1);
	int i;
	for(i=0;i<25;i++){
		vector_put(test,&i);
	}
	int value;
	for(;i>0;i--){
		value = *(int*)vector_get(test,i-1);
		if(value!=i-1) return -1;
	}
	vector_free(test);
	return 0;
}

int contains_test(){
	vector_t *test = vector_init(sizeof(int),1);
	int i;
	for(i=0;i<50;i++){
		vector_put(test,&i);
	}
	i = 38;
	if(vector_contains(test,&i) !=-1){
		vector_free(test);
		return 0;
	}
	vector_free(test);
	return -1;
}

int head_tail_zip_test(){
	vector_t *test = vector_init(sizeof(int),1);
	int i;
	for(i=0;i<50;i++){
		vector_put(test,&i);
	}
	vector_zip(test);
	return !((*(int*)vector_head(test)) == 0 && (*(int*)vector_tail(test))==49);
}

int (*tests[])(void) = {
	&put_get_test,
	&contains_test,
	&head_tail_zip_test,
	NULL
};


int test_all(){
	int i = 0;
	int result;
	int (*tmp)();
	tmp = tests[i];
	while(tmp!=NULL){
		printf("Running test: %d\t",i);
		result = tmp();
		printf("%s\n",result?"Failed":"Success");
		assert(result==0);
		i++;
		tmp = tests[i];
	}
	return 0;	
}

int main(){
	test_all();
	return 0;
}
