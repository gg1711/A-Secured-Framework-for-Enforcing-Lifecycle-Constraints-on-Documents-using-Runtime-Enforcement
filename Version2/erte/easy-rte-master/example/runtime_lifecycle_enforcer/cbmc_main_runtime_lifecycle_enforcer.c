
//This file should be called cbmc_main_runtime_lifecycle_enforcer.c
//This is autogenerated code. Edit by hand at your peril!

//It can be used with the cbmc model checker
//Call it using the following command: 
//$ cbmc cbmc_main_runtime_lifecycle_enforcer.c F_runtime_lifecycle_enforcer.c


#include "F_runtime_lifecycle_enforcer.h"
#include <stdio.h>
#include <stdint.h>

int main() {


	//I/O and state for runtime_lifecycle_enforcer
	enforcervars_runtime_lifecycle_enforcer_t enf_runtime_lifecycle_enforcer;
    inputs_runtime_lifecycle_enforcer_t inputs_runtime_lifecycle_enforcer;
    outputs_runtime_lifecycle_enforcer_t outputs_runtime_lifecycle_enforcer;

	//set values to known state
    runtime_lifecycle_enforcer_init_all_vars(&enf_runtime_lifecycle_enforcer, &inputs_runtime_lifecycle_enforcer, &outputs_runtime_lifecycle_enforcer);

	//introduce nondeterminism
    //a nondet_xxxxx function name tells cbmc that it could be anything, but must be unique
    //randomise inputs
	inputs_runtime_lifecycle_enforcer.W_A_G1 = nondet_runtime_lifecycle_enforcer_input_0();
	inputs_runtime_lifecycle_enforcer.W_B_G1 = nondet_runtime_lifecycle_enforcer_input_1();
	inputs_runtime_lifecycle_enforcer.W_C_G1 = nondet_runtime_lifecycle_enforcer_input_2();
	inputs_runtime_lifecycle_enforcer.W_C_G2 = nondet_runtime_lifecycle_enforcer_input_3();
	inputs_runtime_lifecycle_enforcer.W_D_G2 = nondet_runtime_lifecycle_enforcer_input_4();
	inputs_runtime_lifecycle_enforcer.W_E_G2 = nondet_runtime_lifecycle_enforcer_input_5();
	

	//randomise enforcer state, i.e. clock values and position (excepting violation state)
	enf_runtime_lifecycle_enforcer.A_G1_p1 = nondet_runtime_lifecycle_enforcer_enf_p1_0();
	enf_runtime_lifecycle_enforcer.B_G1_p1 = nondet_runtime_lifecycle_enforcer_enf_p1_1();
	enf_runtime_lifecycle_enforcer.C_G1_p1 = nondet_runtime_lifecycle_enforcer_enf_p1_2();
	
	enf_runtime_lifecycle_enforcer._policy_p1_state = nondet_runtime_lifecycle_enforcer_enf_p1_state() % 2;
	enf_runtime_lifecycle_enforcer.E_G2_p2 = nondet_runtime_lifecycle_enforcer_enf_p2_0();
	enf_runtime_lifecycle_enforcer.D_G2_p2 = nondet_runtime_lifecycle_enforcer_enf_p2_1();
	
	enf_runtime_lifecycle_enforcer._policy_p2_state = nondet_runtime_lifecycle_enforcer_enf_p2_state() % 2;
	enf_runtime_lifecycle_enforcer.B_G1_p3 = nondet_runtime_lifecycle_enforcer_enf_p3_0();
	enf_runtime_lifecycle_enforcer.D_G2_p3 = nondet_runtime_lifecycle_enforcer_enf_p3_1();
	
	enf_runtime_lifecycle_enforcer._policy_p3_state = nondet_runtime_lifecycle_enforcer_enf_p3_state() % 2;
	

    //run the enforcer (i.e. tell CBMC to check this out)
	runtime_lifecycle_enforcer_run_via_enforcer(&enf_runtime_lifecycle_enforcer, &inputs_runtime_lifecycle_enforcer, &outputs_runtime_lifecycle_enforcer);

}


void runtime_lifecycle_enforcer_run(inputs_runtime_lifecycle_enforcer_t *inputs, outputs_runtime_lifecycle_enforcer_t *outputs) {
    //randomise controller

     
}

