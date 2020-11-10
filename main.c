#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

extern float *newton_cotes_1(float (*p)(float), int a, int b);
extern double *newton_cotes_2(double (*p)(double), int a, int b);


static inline double curtime(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1e9 + t.tv_nsec;
}

int main(int argc, char *argv[]) {
	
	if (argc != 4 && argc != 5){
		printf("Incorrect amount of arguments entered. Expected 3 or 4, got %d\n", argc-1);
		return EXIT_FAILURE;
	}
	

	const char *a_ptr = argv[2];
	char *a_end_ptr = NULL;
	
	const char *b_ptr = argv[3];
	char *b_end_ptr = NULL;
	
	errno = 0;
	long a_long = strtol (a_ptr, &a_end_ptr, 10);
	
	
	if (a_ptr == a_end_ptr) { printf ("Integration limit 'a' has invalid characters\n"); return EXIT_FAILURE; }
	if (errno == 0 && a_ptr && *a_end_ptr != 0) { printf("Invalid characters after integration limit 'a'\n"); return EXIT_FAILURE; }
	if ((errno == ERANGE && a_long == LONG_MAX) || (errno == ERANGE && a_long == LONG_MIN)) { printf("Integration limit 'a' too large\n"); return EXIT_FAILURE; }
	if (errno != 0 && a_long == 0) { printf ("Invalid entry for integration limit 'a'\n"); return EXIT_FAILURE; }
	
	errno = 0;
	long b_long = strtol (b_ptr, &b_end_ptr, 10);
	
	if (b_ptr == b_end_ptr) { printf ("Integration limit 'b' has invalid characters\n"); return EXIT_FAILURE; }
	if (errno == 0 && b_ptr && *b_end_ptr != 0) { printf("Invalid characters after integration limit 'b'\n"); return EXIT_FAILURE; }
	if ((errno == ERANGE && b_long == LONG_MAX) || (errno == ERANGE && b_long == LONG_MIN)) { printf("Integration limit 'b' is too large\n"); return EXIT_FAILURE; }
	if (errno != 0 && b_long == 0) { printf ("Invalid entry for integration limit 'b'\n"); return EXIT_FAILURE; }
	if (argc == 5 && strcmp(argv[4],"-t") != 0){printf("Invalid Flag\n"); return EXIT_FAILURE; }


	if(a_long > INT_MAX || a_long < INT_MIN || b_long > INT_MAX || b_long < INT_MIN) {
		printf("Integration limits have values outside the representation range of an integer\n");
		return EXIT_FAILURE;
	}
	
	int a = (int) a_long;
	int b = (int) b_long;
	

	float (*p_nc1)(float);
	double (*p_nc2)(double);
	void * handle;
	char * error;
	

	if (strcmp(argv[1], "42") == 0){
		handle = dlopen("./functions/42.so", RTLD_LAZY);
	}
	else if (strcmp(argv[1], "min2x") == 0){
		handle = dlopen("./functions/min2x.so", RTLD_LAZY);
	}
	else if (strcmp(argv[1], "3x-1") == 0){
		handle = dlopen("./functions/3x-1.so", RTLD_LAZY);
	}
	else if (strcmp(argv[1], "x^2+1") == 0){
		handle = dlopen("./functions/x^2+1.so", RTLD_LAZY);
	}
	else if (strcmp(argv[1], "x^3-25x") == 0){
		handle = dlopen("./functions/x^3-25x.so", RTLD_LAZY);
	}
	else if (strcmp(argv[1], "x^4-21x^3+52x^2+480x-512") == 0){
		handle = dlopen("./functions/x^4-21x^3+52x^2+480x-512.so", RTLD_LAZY);
	}
	else if (strcmp(argv[1], "x^5-21x^4+52x^3+480x^2-512x") == 0){
		handle = dlopen("./functions/x^5-21x^4+52x^3+480x^2-512x.so", RTLD_LAZY);
	}
	else if (strcmp(argv[1], "e^x") == 0){
		handle = dlopen("./functions/e^x.so", RTLD_LAZY);
	}
	else {
		printf("The entered function does not exist\n");
		return EXIT_FAILURE;
	}
	

	if(!handle){
		fputs(dlerror(), stderr);
		printf("\n");
		return EXIT_FAILURE;
	}
	
    if ((error = dlerror()) != NULL) {
        fputs(error, stderr);
		printf("\n");
        return EXIT_FAILURE;
    }
	

	p_nc1 = dlsym(handle, "f");
	p_nc2 = dlsym(handle, "f");	
	

	float * result_asm_1 = newton_cotes_1(p_nc1, a, b);
	double * result_asm_2 = newton_cotes_2(p_nc2, a, b);


	float * num_qdrt1_ptr = ((float *) &result_asm_1);
	float num_qdrt1_result = *num_qdrt1_ptr; 
	
	double * num_qdrt2_ptr = ((double *) &result_asm_2);
	double num_qdrt2_result = *num_qdrt2_ptr;
	
	printf("Results with Newton Cotes 1 = %f\n", num_qdrt1_result);
	printf("Results with Newton Cotes 2 = %f\n", num_qdrt2_result);
	

	float superPrecise_nc1 = 0;
	double superPrecise_nc2 = 0;
	
	float * sp_asm_1;
	double * sp_asm_2;
	int swapped = false;

	if (a > b) {
		swapped = true;
		int swap = b;
		b = a;
		a = swap;
	}

	for (int i = a; i < b; i++) {
		sp_asm_1 = newton_cotes_1(p_nc1, i, (i+1));
		sp_asm_2 = newton_cotes_2(p_nc2, i, (i+1));

		float * num_qdrt1_ptr_sp = ((float *) &sp_asm_1);
		superPrecise_nc1 += *num_qdrt1_ptr_sp; 
	
		double * num_qdrt2_ptr_sp = ((double *) &sp_asm_2);
		superPrecise_nc2 += *num_qdrt2_ptr_sp; 
	}

	if (swapped == false) {
		printf("Result of the summed numerical quadrature with Newton Cotes 1 = %f\n", superPrecise_nc1);
		printf("Result of the summed numerical quadrature with Newton Cotes 2 = %f\n", superPrecise_nc2);
	} else {
		printf("Result of the summed numerical quadrature with Newton Cotes 1 = %f\n", -1*superPrecise_nc1);
		printf("Result of the summed numerical quadrature with Newton Cotes 2 = %f\n", -1*superPrecise_nc2);
	}

	if(argc == 5){
		printf("\n");

		double start_time;
		double end_time;
		double newton_cotes_1_time = 0;
		double newton_cotes_2_time = 0;
		
		for(int i = 0; i < 5; i++){
			start_time = curtime();
			for(int i = 0; i < 1000000; i++){
				newton_cotes_1(p_nc1, a, b);
			}
			end_time = curtime();
			newton_cotes_1_time += end_time - start_time;
			start_time = curtime();
			for(int i = 0; i < 1000000; i++){
				newton_cotes_2(p_nc2, a, b);
			}
			end_time = curtime();
			newton_cotes_2_time += end_time - start_time;

			sleep(1);
		}


		newton_cotes_1_time /= 5000000;
		newton_cotes_2_time /= 5000000;

		printf("Calculation time for Newton Cotes 1 = %f Nanoseconds\n", newton_cotes_1_time);
		printf("Calculation time for Newton Cotes 2= %f Nanoseconds\n", newton_cotes_2_time);
		printf("\n");
		}

	return EXIT_SUCCESS;
}