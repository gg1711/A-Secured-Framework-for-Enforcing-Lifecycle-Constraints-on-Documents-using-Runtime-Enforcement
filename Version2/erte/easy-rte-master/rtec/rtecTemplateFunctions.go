package rtec

import (
	"regexp"
	"strings"

	"github.com/PRETgroup/easy-rte/rtedef"
)

//CECCTransition is used with getCECCTransitionCondition to return results to the template
type CECCTransition struct {
	IfCond    string
	AssEvents []string
}

//getCECCTransitionCondition returns the C "if" condition to use in state machine next state logic and associated events
// returns "full condition", "associated events"
func getCECCTransitionCondition(function rtedef.EnforcedFunction, trans string) CECCTransition {
	var events []string

	re1 := regexp.MustCompile("([<>=!]+)")          //for capturing operators
	re2 := regexp.MustCompile("([a-zA-Z0-9_<>=]+)") //for capturing variable and event names and operators
	isNum := regexp.MustCompile("^[0-9.]+$")

	retVal := trans

	//rename AND and OR
	retVal = strings.Replace(retVal, "AND", "&&", -1)
	retVal = strings.Replace(retVal, "OR", "||", -1)

	//re1: add whitespace around operators
	retVal = re1.ReplaceAllStringFunc(retVal, func(in string) string {
		if in != "!" {
			return " " + in + " "
		}
		return " !"
	})

	//re2: add "me->" where appropriate
	retVal = re2.ReplaceAllStringFunc(retVal, func(in string) string {
		if strings.ToLower(in) == "and" || strings.ToLower(in) == "or" || strings.ContainsAny(in, "!><=") || strings.ToLower(in) == "true" || strings.ToLower(in) == "false" {
			//no need to make changes, these aren't variables or events
			return in
		}

		if isNum.MatchString(in) {
			//no need to make changes, it is a numerical value of some sort
			return in
		}

		//check to see if it is input data
		if function.InputVars != nil {
			for _, Var := range function.InputVars {
				if Var.Name == in {
					return "inputs->" + in
				}
			}
		}

		//check to see if it is output data
		if function.OutputVars != nil {
			for _, Var := range function.OutputVars {
				if Var.Name == in {
					return "outputs->" + in
				}
			}
		}

		//check to see if it is a policy internal var
		for i := 0; i < len(function.Policies); i++ {
			for _, Var := range function.Policies[i].InternalVars {
				if Var.Name == in || Var.Name+"_i" == in {
					if Var.Constant {
						return "CONST_" + function.Policies[i].Name + "_" + in
					}
					return "me->" + in
				}
			}
		}

		//else, return it (no idea what else to do!) - it might be a function call or strange text constant
		return in
	})

	//tidy the whitespace
	retVal = strings.Replace(retVal, "  ", " ", -1)

	return CECCTransition{IfCond: retVal, AssEvents: events}
}

//getPolicyEnfInfo will get a PEnforcer for a given policy
func getPolicyEnfInfo(function rtedef.EnforcedFunction, policyIndex int) *rtedef.PEnforcer {
	enfPol, err := rtedef.MakePEnforcer(function.InterfaceList, function.Policies[policyIndex])
	if err != nil {
		return nil
	}
	//Uncomment these two lines to generate the intermediate enforcer JSON file
	//bytes, _ := json.MarshalIndent(enfPol, "", "\t")
	//ioutil.WriteFile(function.Name+".json", bytes, 0644)
	return enfPol
}

//getAllPolicyEnfInfo will get a PEnforcer for a given policy
func getAllPolicyEnfInfo(function rtedef.EnforcedFunction) []rtedef.PEnforcer {
	pols := make([]rtedef.PEnforcer, 0, len(function.Policies))
	for i := 0; i < len(function.Policies); i++ {
		enfPol, err := rtedef.MakePEnforcer(function.InterfaceList, function.Policies[i])
		if err != nil {
			return nil
		}
		pols = append(pols, *enfPol)
	}

	//Uncomment these two lines to generate the intermediate enforcer JSON file
	//bytes, _ := json.MarshalIndent(enfPol, "", "\t")
	//ioutil.WriteFile(function.Name+".json", bytes, 0644)
	return pols
}

func sub(a, b int) int {
	return a - b
}
