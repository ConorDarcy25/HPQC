This assignment deals with the implementation of Grover's algorithm for quantum computing. Grover's algorithm is used for searching through unsorted databases or solve an unknown function faster than a classical computer can. A claissical search through N elements would take a time proportional to O(2^N) but Grover's algorithm is much quicker, making it an extremely importnant quantum algorithm.

# Subroutines

A basic quantum environment is set up using NumPy arrays. The workspace starts off in the 0 state (default). An X_gate, Z_gate and H_gate are defined, and a namestack keeps track of which qubits are in the work space

The pushQubit function adds a new qubit to the system and its weight is normalised using a Kronecker product to comibine it with the exiating quantum state. This allows one to build multi-qubit states.

The applyGate function is responsible for applying gates to specfific qubits. This functino first checks if the qubits are at the top of the stack. If it is not, it reorders them using the tosQubit function. This function swaps the position of a qubit in the internal matrix structure so that the correct operation can be applied. This is necessary as you need to rearrange the matrix so the corret qubit is active.

zero_booleanOracle checks if all qubits are 0. It does this by applying X gates, then using a Toffoli gate
