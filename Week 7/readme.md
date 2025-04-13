The code used throughout simulates a basic quantum circuit using numPy. It includes psuhing quits, using quantum gates, measures qubits and uses Toffoli gates.

# Instruction 1
This part of the program reshapes the workspace into a row vector and uses a Kroneckher product to add new qubits.

pushQubit(name, weights) pushes a qubit (defined by a 2-element vector weights) onto the stack.

np.reshape reshapes the workspace into a 1-row matrix so it can be used in matrix operations. np.kron(workspace, weights) expands the workspace by multiplying each element of the workspace with the weights vector. Each qubit doubles the size of the workspace, mimicking how the quantum state grows with more qubits. The input weights are then normalised so the qubit is a valid state.

# Instruction 2
Quantum gates are used to manipulate qubits. These are implemented as matrices that operate on the state vector.

The applyGate(gate, *names) function applies a gate to one or more qubits in the workspace.

X gate: Acts like a NOT gate

H gate (Hadamard): Puts the qubit into a equal superposition of 0 and 1

T gate: Introduces a complex phase

Swap gate: Changes the order of the two qubits in the stack

All of these gates are matrix multiplications of the current state. The function reshapes the workspace to match the size of the gate

Toffoli gates are used to simulate classical logic in quantum circuits. a TOFF gate is a basic 3 qubit gates with two controls and one target. The target qubit is flipped only if the both controls are 1

# Instruction 3
tosQubit(name) moves a named qubit to the top of the stack. This is needed before applying a gate or measuring. It reshapes and reorders the workspace and the name list automatically

# Instruction 4
measureQubit(name) simulates the measurement of a qubit. probQubit is used the calculate the probability of getting 0 or 1. The result is picked using np.random.choice based on the calculated probabilities. The workspace is then updated to reflect this measurement. The resulting state is normalised so that the total probability remains 1.

Advnaced locig gates are used too.

TOFF3_gate(q1, q2, q3, q4)
Combines three control qubits to compute an AND result stored in q4

TOFFn_gate(ctl_list, result)
A recursive fate that applies the logic of a multi-control Toffoli gate

Temporary qubits are created to help with intermediate steps but are removed after use.
