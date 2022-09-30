
//This file should be called F_runtime_lifecycle_enforcer.h
//This is autogenerated code. Edit by hand at your peril!

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

//the dtimer_t type
typedef uint64_t dtimer_t;

//For each policy, we need an enum type for the state machine

enum runtime_lifecycle_enforcer_policy_p1_states { 
	POLICY_STATE_runtime_lifecycle_enforcer_p1_s0, 
	POLICY_STATE_runtime_lifecycle_enforcer_p1_s1,
	POLICY_STATE_runtime_lifecycle_enforcer_p1_violation 
};

enum runtime_lifecycle_enforcer_policy_p2_states { 
	POLICY_STATE_runtime_lifecycle_enforcer_p2_s0, 
	POLICY_STATE_runtime_lifecycle_enforcer_p2_s1,
	POLICY_STATE_runtime_lifecycle_enforcer_p2_violation 
};

enum runtime_lifecycle_enforcer_policy_p3_states { 
	POLICY_STATE_runtime_lifecycle_enforcer_p3_s0, 
	POLICY_STATE_runtime_lifecycle_enforcer_p3_s1,
	POLICY_STATE_runtime_lifecycle_enforcer_p3_violation 
};


//Inputs to the function runtime_lifecycle_enforcer
typedef struct {
	int16_t W_A_G1;
	int16_t W_B_G1;
	int16_t W_C_G1;
	int16_t W_C_G2;
	int16_t W_D_G2;
	int16_t W_E_G2;
	
} inputs_runtime_lifecycle_enforcer_t;

//Outputs from the function runtime_lifecycle_enforcer
typedef struct {
	
} outputs_runtime_lifecycle_enforcer_t;

//enforcer state and vars:
typedef struct {
	enum runtime_lifecycle_enforcer_policy_p1_states _policy_p1_state;
	//internal vars
	int16_t A_G1_p1;
	int16_t B_G1_p1;
	int16_t C_G1_p1;
	
	
	enum runtime_lifecycle_enforcer_policy_p2_states _policy_p2_state;
	//internal vars
	int16_t E_G2_p2;
	int16_t D_G2_p2;
	
	
	enum runtime_lifecycle_enforcer_policy_p3_states _policy_p3_state;
	//internal vars
	int16_t B_G1_p3;
	int16_t D_G2_p3;
	
	
	
} enforcervars_runtime_lifecycle_enforcer_t;










//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It sets up the variable structures to their initial values
void runtime_lifecycle_enforcer_init_all_vars(enforcervars_runtime_lifecycle_enforcer_t* me, inputs_runtime_lifecycle_enforcer_t* inputs, outputs_runtime_lifecycle_enforcer_t* outputs);

//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It will run the synthesised enforcer and call the controller function
void runtime_lifecycle_enforcer_run_via_enforcer(enforcervars_runtime_lifecycle_enforcer_t* me, inputs_runtime_lifecycle_enforcer_t* inputs, outputs_runtime_lifecycle_enforcer_t* outputs);

//This function is provided from the user
//It is the controller function
extern void runtime_lifecycle_enforcer_run(inputs_runtime_lifecycle_enforcer_t* inputs, outputs_runtime_lifecycle_enforcer_t* outputs);

//enforcer functions


//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It will run the input enforcer for runtime_lifecycle_enforcer's policy p1
void runtime_lifecycle_enforcer_run_input_enforcer_p1(enforcervars_runtime_lifecycle_enforcer_t* me, inputs_runtime_lifecycle_enforcer_t* inputs);

//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It will run the input enforcer for runtime_lifecycle_enforcer's policy p1
void runtime_lifecycle_enforcer_run_output_enforcer_p1(enforcervars_runtime_lifecycle_enforcer_t* me, inputs_runtime_lifecycle_enforcer_t* inputs, outputs_runtime_lifecycle_enforcer_t* outputs);

//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It will check the state of the enforcer monitor code
//It returns one of the following:
//0: currently true (safe)
//1: always true (safe)
//-1: currently false (unsafe)
//-2: always false (unsafe)
//It will need to do some reachability analysis to achieve this
int runtime_lifecycle_enforcer_check_rv_status_p1(enforcervars_runtime_lifecycle_enforcer_t* me);


//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It will run the input enforcer for runtime_lifecycle_enforcer's policy p2
void runtime_lifecycle_enforcer_run_input_enforcer_p2(enforcervars_runtime_lifecycle_enforcer_t* me, inputs_runtime_lifecycle_enforcer_t* inputs);

//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It will run the input enforcer for runtime_lifecycle_enforcer's policy p2
void runtime_lifecycle_enforcer_run_output_enforcer_p2(enforcervars_runtime_lifecycle_enforcer_t* me, inputs_runtime_lifecycle_enforcer_t* inputs, outputs_runtime_lifecycle_enforcer_t* outputs);

//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It will check the state of the enforcer monitor code
//It returns one of the following:
//0: currently true (safe)
//1: always true (safe)
//-1: currently false (unsafe)
//-2: always false (unsafe)
//It will need to do some reachability analysis to achieve this
int runtime_lifecycle_enforcer_check_rv_status_p2(enforcervars_runtime_lifecycle_enforcer_t* me);


//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It will run the input enforcer for runtime_lifecycle_enforcer's policy p3
void runtime_lifecycle_enforcer_run_input_enforcer_p3(enforcervars_runtime_lifecycle_enforcer_t* me, inputs_runtime_lifecycle_enforcer_t* inputs);

//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It will run the input enforcer for runtime_lifecycle_enforcer's policy p3
void runtime_lifecycle_enforcer_run_output_enforcer_p3(enforcervars_runtime_lifecycle_enforcer_t* me, inputs_runtime_lifecycle_enforcer_t* inputs, outputs_runtime_lifecycle_enforcer_t* outputs);

//This function is provided in "F_runtime_lifecycle_enforcer.c"
//It will check the state of the enforcer monitor code
//It returns one of the following:
//0: currently true (safe)
//1: always true (safe)
//-1: currently false (unsafe)
//-2: always false (unsafe)
//It will need to do some reachability analysis to achieve this
int runtime_lifecycle_enforcer_check_rv_status_p3(enforcervars_runtime_lifecycle_enforcer_t* me);


