# QC Using GPU

This code is a basic quantum simulator written in PyTorch. It simulates how qubits behave and lets you apply quantum gates, measure qubits and do things like Grover's search. It can run on both CPU and GPU depending on what's available

Gradient calculations are turned off as we are not training a model. Then the code checks to see if a GPU is available.

pushQubit adds a new qubit to the quantum state and normalises it. It is then converted into a PyTorch sesnor and Kronecker products it with the current state

tosQubit moves the qubit to the top of the stack.

applyGate applies a quantum gate as seen before

probQubit calculates the probability that a given qubit is measured as 0 or 1

measureQubit measures and removes a qubit from the system.

# Complete Implementation

This a DIY version of the previous code, and follows the same method as seen previously

# Minimalist QC

This code is a tiny quantum computer simulator using NumPy and is built to run Grover's search over 2^10 in under 50 lines, and works using the same algorithms as previously, just in a shoter block of code.

