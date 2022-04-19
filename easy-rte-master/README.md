# easy-rte

## About
This project provides an easy-to-use implementation of _bi-directional Runtime Enforcement_. It is the toolchain used in the paper [Smart I/O Modules for Mitigating Cyber-Physical Attacks on Industrial Control Systems](https://ieeexplore.ieee.org/abstract/document/8859335) (IEEE Transactions on Industrial Informatics (TII) 2020). 

We ensure we are correct via formal proof and via the use of the CBMC and EBMC model checkers.

Easy-rte extends the boolean semantics originally presented in [Runtime Enforcement of Cyber-Physical Systems](https://dl-acm-org.ezproxy.auckland.ac.nz/citation.cfm?id=3126500) (ACM Transactions on Embedded Computing Systems (TECS) 2017). 
While the original implementation was restricted to simple _boolean_ arguments only, and was implemented in Python for use 
with SCCharts, this project presents a more generalised any-type enforcement system, which can be used with any C project. 

_easy-rte_ was ported from [goFB](https://github.com/PRETgroup/goFB).

## What is Runtime Enforcement?

Runtime Enforcement is a subset of Run-time Assurance (RA) which focuses on formal semantics for blocking, delaying, modifying, or re-ordering events. 
* Enforcement/assurance mechanisms aim to bridge the growing gap between system capabilites and certification abilities.
* They shift the analysis/test burden from a complicated control system to a simpler assurance/enforcement mechanism.
* They are already popular in several domains, for instance in operating systems, web browsers, spam filters, intrusion-detection systems, firewalls, and so on.

This project aims to join the growing chorus calling for the use of Runtime Enforcement mechanisms for use within the real-world cyber-physical systems domain. Using Runtime Enforcement, we can bound the behaviour of _unpredictable_ and _untrustworthy_ processes, ensuring that they obey desired policies.

## How does Runtime Enforcement work?

Runtime Enforcement works by placing a special execution module _between_ your plant and your controller.

```
+-------+                +------------+               
|       | ---inputs--->  |            |
| Plant |                | Controller |
|       | <--outputs---  |            |
+-------+                +------------+
```
becomes
```
                           Policies
                              \/
+-------+                +----------+               +------------+
|       | ---inputs--->  |          | ---inputs'--> |            |
| Plant |                | Enforcer |               | Controller |
|       | <--outputs'--  |          | <--outputs--- |            |
+-------+                +----------+               +------------+
```

In our case, we can compile our policies to either *C* or *Verilog* code. 

The C enforcers are designed to be composed with your software in a system such as a microcontroller or an Arduino.

However, software enforcers cannot by their nature enforce the behaviour of the hardware that they run upon. 
So, in this case you may want to build your enforcer to Verilog, and then compose your microcontroller with custom hardware (such as on an FPGA or ASIC) to ensure system correctness.

## Build instructions

Download and install the latest version of [Go](https://golang.org/doc/install).

Then, download this repository, and run `make` or `make default`, which will generate the tools. 

* The ab5 example can be generated using `make c_enf PROJECT=ab5`.
* The robotable example can be generated using `make c_enf PROJECT=robotable`.
* You can also generate Verilog enforcers by using `make verilog_enf PROJECT=ab5`, for example. The Verilog enforcers are a little trickier to use, and require I/O connections to be provided to them. They do not embed a function call to the controller inside.
* If you are interested in using the model checkers:
  * Obtain CBMC (C model checker) by running `sudo apt install cbmc`. Website here: https://www.cprover.org/cbmc/
  * Obtain EBMC (Verilog model checker) by going to website https://www.cprover.org/ebmc/

## A note on Easy-rte language

Easy-rte is based on Structured Text (ST) operators and syntax. When making guards, ensure that you adhere to the following operators:

| Operator       |     Code    |
| -------------- | ----------- |
| Assignment     |  `:=`       |
| Equality       |  `=`        |
| Inequality     |  `<>`       |
| Addition       |  `+`        |
| Subtraction    |  `-`        |
| Multiplication |  `*`        |
| Division       |  `/`        |
| Not            | `!` or NOT  |
| And            | `&&` or AND |
| Or             | `\|\|` or OR  |
| Brackets       | `(` and `)` |

## Example of Use (AB5)

Imagine a function which inputs boolean `A` and outputs boolean `B`. 
In _easy-rte_, we present this function with the following _erte_ syntax:
```
function ab5Function;
interface of ab5Function {
	in bool A;  //in here means that they're going from PLANT to CONTROLLER
	out bool B; //out here means that they're going from CONTROLLER to PLANT
}
```

This is equivalent to the following C code, which is autogenerated, so that the function ab5Function_run can be provided by the user:
```c
//Inputs to the function ab5Function
typedef struct {
	bool A;
} inputs_ab5Function_t;

//Outputs from the function ab5Function
typedef struct {
	bool B;
} outputs_ab5Function_t;

void ab5Function_run(inputs_ab5Function_t inputs, outputs_ab5Function_t *outputs);
```

Let's now give our function the following I/O properties:
1. A and B cannot happen simultaneously.
2. A and B alternate starting with an A. 
3. B should be true within 5 ticks after an occurance of A.

We can present this as the following _easy-rte_ policy format:
```
policy ab5 of ab5Function {
	internals {
		dtimer_t v;
	}

	states {

		//first state is initial, and represents "We're waiting for an A"
		s0 {
			//if we receive neither A nor B, do nothing	
			-> s0 on (!A and !B): v := 0;

			//if we receive an A only, head to state s1							
			-> s1 on (A and !B): v := 0;
			
			//if we receive a B, or an A and a B (i.e. if we receive a B) then VIOLATION
			-> violation on ((!A and B) or (A and B));	
		}

		//s1 is "we're waiting for a B, and it needs to get here within 5 ticks"
		s1 {
			//if we receive nothing, and we aren't over-time, then we do nothing
			-> s1 on (!A and !B and v < 5);	

			//if we receive a B only, head to state s0
			-> s0 on (!A and B);

			//if we go overtime, or we receive another A, then VIOLATION
			-> violation on ((v >= 5) or (A and B) or (A and !B));	
		}
	}
}
```

As can be seen, this can be thought of as a simple mealy finite state machine, which provides the rules for correct operation.
A transition which goes from any state to a violation state is an error, and should be prevented.
So, we shall convert the _policy_ to an _enforcer_, which is capable of modifying the I/O such that a violation cannot occur.

The theory of converting policies to enforcers is covered in the companion paper [Runtime Enforcement of Cyber-Physical Systems](https://dl-acm-org.ezproxy.auckland.ac.nz/citation.cfm?id=3126500).

With _easy-rte_, this process is completed automatically in two steps. Firstly, we convert the _erte_ file into an equivalent policy XML file (which makes it easier to understand, and allows portability between tools).
* `./easy-rte-parser -i example/ab5 -o example/ab5`

Then, we convert this policy XML file into an _enforcer_, which is written in C. 
* `./easy-rte-c -i example/ab5 -o example/ab5`

Now, we can provide a `main.c` file which has our controller function in it, and then compile the project together.

This entire example is provided in the `/example/ab5` folder of this repository, including an example top level file, and can be built from the root directory using `make c_enf PROJECT=ab5`.

To compile it together with the example main file, run `make c_enf c_build PROJECT=ab5`

### Using CBMC on AB5

Remember to install CBMC with either `sudo apt install cbmc` or from https://www.cprover.org/cbmc/

Short command: you can run CBMC automatically on build with the command `make c_enf run_cbmc PROJECT=ab5`.

For more details:

Once you have compiled AB5 using `make c_enf PROJECT=ab5`, you can navigate to `/example/ab5` directory.

You will see that a file `cbmc_main_ab5.c` is automatically generated for you. You may open this file to inspect it.

This file is a CBMC-ready main file for you to ensure that entering a violation state of a policy is impossible (i.e. your recover transitions were correctly specified). 
It makes all the environmental inputs, the controller outputs, and the enforcer internal state and data vars _nondeterministic_.
This ensures that _all operating cases_ are checked.
See http://www.cprover.org/cprover-manual/modeling-nondet.html for an explanation.

In the top comments of the file it will describe the CBMC command required to run it. In the case of ab5, this is `$ cbmc cbmc_main_ab5.c F_ab5.c`. 

Once run, this will tell you `VERIFICATION SUCCESSFUL`, i.e. this policy can not enter a violation state.

### Using EBMC on AB5

Remember to install EBMC from https://www.cprover.org/ebmc/

Short command: You can run EBMC automatically on build of a Verilog Enforcer with the command `make verilog_enf run_ebmc PROJECT=ab5`.

For more details:

You can also compile ab5 to Verilog by using `make verilog_enf PROJECT=ab5`.

Now, if you navigate to the `/example/ab5` directory you will see there is an `F_ab5.v` file as well as a `ebmc_F_ab5.sv` file.

We are interested in verifying the `embc_F_ab5.sv` file, as this exposes sources of nondeterminism to the model checker (just as we did earlier with the CBMC top level file for the C compilation).
It also includes the assertion command, which is not present in normal verilog.

If you open up the generated `embc_F_ab5.sv` file, you will see in the top comments an EMBC command that can be used to run the EBMC
model checker. For AB5, this is `$ ebmc ebmc_F_ab5.sv`.

## Example of Use (robotable)

Consider a 2-d flat surface (a table) with -10 to +10 in each dimension.

We place an omnidirectional robot into this table, and tell it to go to a set position.

There is thus a current position `(x,y)` and a desired position `(x,y)`. The robot can output a drive command which is of the form `(x,y)`.

The robot's drive motors can only handle integer commands in `{-1, 0, 1}`. If the robot tries to provide larger commands, the motors will burn out.

The robot should never be instructed to drive off the table, nor informed it is currently not on the table.

We can represent the safety policies for this robot with the following `erte` specification:

```
function Robotable;
interface of Robotable {
	//in here means that they're going from PLANT to CONTROLLER
	//The inputs to the controller is the requested X, Y location, as well as the sensed X, Y location
	in int16_t reqLocX;  
	in int16_t reqLocY;	
	in int16_t curLocX := 0; //0, 0 is the middle of the table
	in int16_t curLocY := 0;

	//out here means that they're going from CONTROLLER to PLANT
	//The outputs from the controller are the direction controllers for X and Y.
	//These are limited to {-1, 0, 1}
	out int16_t driveX := 0;
	out int16_t driveY := 0;
}

policy stayOnTable of Robotable {
	internals {
		constant int16_t minX := -10; //use of "constant" for constant variables allows for compiler 
		constant int16_t maxX := 10;  //to make smarter decisions around these internal variables
		constant int16_t minY := -10;
		constant int16_t maxY := 10;

		constant int16_t maxSpeed := 1;
		constant int16_t minSpeed := -1;
	}

	states {

		s0 {
			//INPUT: ensure requested location is on the table
			-> violation on ((reqLocX < minX) or (reqLocX > maxX) or (reqLocY < minY) or (reqLocY > maxY));
			
			//INPUT: ensure sensed location is on the table
			-> violation on ((curLocX < minX) or (curLocX > maxX) or (curLocY < minY) or (curLocY > maxY));

			//OUTPUT: ensure that the drive command does not exceed a safe value
			-> violation on ((driveX < minSpeed) or (driveX > maxSpeed) or (driveY < minSpeed) or (driveY > maxSpeed));

			//OUTPUT: ensure that the current sense location augmented with the drive command does not push us off the table
			-> violation on ((curLocX + driveX) < minX) recover driveX := 0;
			-> violation on ((curLocX + driveX) > maxX) recover driveX := 0;
			-> violation on ((curLocY + driveY) < minY) recover driveY := 0;
			-> violation on ((curLocY + driveY) > maxY) recover driveY := 0;
			
			//SAFE: everything is ok.
			-> s0 on (
				(reqLocX >= minX) and (reqLocX <= maxX) and (reqLocY >= minY) and (reqLocY <= maxY) and 
				(curLocX >= minX) and (curLocX <= maxX) and (curLocY >= minY) and (curLocY <= maxY) and
				((curLocX + driveX) >= minX) and ((curLocX + driveX) <= maxX) and ((curLocY + driveY) >= minY) and ((curLocY + driveY) <= maxY)) and
				(driveX >= minSpeed) and (driveX <= maxSpeed) and (driveY >= minSpeed) and (driveY <= maxSpeed)
			);
		}
	}
}
```

You will notice in this example, unlike _ab5_, that we have used the `recover` keyword in the policy. This allows us to manually specify the recovery instruction, rather than having the tool try and autogenerate it (the tool is not perfect!).

When compiling this policy, data like the following is presented to the console:
```
...
Automatically deriving a solution for violation transition
        s0 -> violation on (driveY < minSpeed)
        (If this is undesirable behaviour, use a 'recover' keyword in the erte file to manually specify solution)
        NOTE: (Guess) Solution found, and edits required! (I have selected a safe transition, and edited the I/O so that it can be taken)
        Selected transition: "s0 -> s0 on reqLocX >= minX && reqLocX <= maxX && reqLocY >= minY && reqLocY <= maxY && curLocX >= minX && curLocX <= maxX && curLocY >= minY && curLocY <= maxY && curLocX + driveX >= minX && curLocX + driveX <= maxX && curLocY + driveY >= minY && curLocY + driveY <= maxY && driveX >= minSpeed && driveX <= maxSpeed && driveY >= minSpeed && driveY <= maxSpeed"
        NOTE: I will perform the following edits:
                driveY = minSpeed;
...
```

We can see then that the autogeneration of solutions is correct here, as it has decided that if `driveY < minSpeed` then the correct rectifying behaviour is to make `driveY = minSpeed` (which is correct).

Running this example will show that the robot eventually drives its way to the desired location eventually, despite the controller presenting unsafe outputs (drive values too large) and the desired location being set off the table.

## Combining policies (Experimental)

`make verilog_enf run_ebmc PROJECT=pacemaker FILE=p1_and_p2 PARSEARGS=-product`
