This assignment deals with the implementation of Grover's algorithm for quantum computing. Grover's algorithm is used for searching through unsorted databases or solve an unknown function faster than a classical computer can. A claissical search through N elements would take a time proportional to O(2^N) but Grover's algorithm is much quicker, making it an extremely importnant quantum algorithm.

# Subroutines

A basic quantum environment is set up using NumPy arrays. The workspace starts off in the 0 state (default). An X_gate, Z_gate and H_gate are defined, and a namestack keeps track of which qubits are in the work space

The pushQubit function adds a new qubit to the system and its weight is normalised using a Kronecker product to comibine it with the exiating quantum state. This allows one to build multi-qubit states.

The applyGate function is responsible for applying gates to specfific qubits. This functino first checks if the qubits are at the top of the stack. If it is not, it reorders them using the tosQubit function. This function swaps the position of a qubit in the internal matrix structure so that the correct operation can be applied. This is necessary as you need to rearrange the matrix so the corret qubit is active.

zero_booleanOracle checks if all qubits are 0. It does this by applying X gates, then using a Toffoli gate and then flips them back to restore the original state

zero_paseOracle works in a similar way to the boolean version, but instead of storing a result it changes the sign of the entire state if all qubits are 0. This is done by surrounding a Z_gate with X_gates 

sample_phaseOracle represents the actual problem. In this case, it flips the phase only if a specific combination of qubit value matches (e.g 111101). It works by flippinh one qubit with x, applying the Z gate and then flipping it back

# Main Loop

All n qubits are pushed in superposition using the Hadamard operation 

It runs a loop for a number of steps based on the size of the input. Each loop performs the oracle, a layer of Hadamard gates to spread amplitudes, the zero_phaseOracle to amplify the marked state and finally another layer of Hadamards. This process is called inversion about the mean. This increases the chance of measuring the correct result.

After the loop it measures each qubit from last to first. This result is printed as a bitstring representing the best guess at the correct input
