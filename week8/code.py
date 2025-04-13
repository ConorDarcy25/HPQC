import numpy as np

# Initialize the quantum workspace
workspace = np.array([[1.]])  # Represents the |0⟩ state

# Define quantum gates
X_gate = np.array([[0, 1], [1, 0]])  # Pauli-X (NOT) gate
Z_gate = np.array([[1, 0], [0, -1]])  # Pauli-Z gate
H_gate = np.array([[1, 1], [1, -1]]) * np.sqrt(0.5)  # Hadamard gate

# Stack to keep track of qubit names
namestack = []

TOFF_gate = np.array([[1, 0, 0, 0, 0, 0, 0, 0], # Toffoli gate
                     [0, 1, 0, 0, 0, 0, 0, 0],
                     [0, 0, 1, 0, 0, 0, 0, 0],
                     [0, 0, 0, 1, 0, 0, 0, 0],
                     [0, 0, 0, 0, 1, 0, 0, 0],
                     [0, 0, 0, 0, 0, 1, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0, 1],
                     [0, 0, 0, 0, 0, 0, 1, 0]])
# Function to push a qubit onto the stack with given weights
def pushQubit(name, weights):
    global workspace, namestack
    if workspace.shape == (1, 1):
        namestack = []
    namestack.append(name)
    weights = np.array(weights, dtype=complex)
    weights /= np.linalg.norm(weights)  # Normalize the weights
    workspace = np.kron(np.reshape(workspace, (1, -1)), weights)

# Function to apply a gate to specific qubits
def applyGate(gate, *names):
    global workspace, namestack
    if list(names) != namestack[-len(names):]:
        [tosQubit(name) for name in names]
    workspace = np.reshape(workspace, (-1, 2**len(names)))
    subworkspace = workspace[:, -gate.shape[0]:]
    np.matmul(subworkspace, gate.T, out=subworkspace)

# Function to swap qubit positions
def tosQubit(name):
    global workspace, namestack
    k = len(namestack) - namestack.index(name)
    if k > 1:
        namestack.append(namestack.pop(-k))
        workspace = np.reshape(workspace, (-1, 2, 2**(k-1)))
        workspace = np.swapaxes(workspace, -2, -1)

# Function to calculate the probability of measuring a qubit
def probQubit(name):
    global workspace
    tosQubit(name)
    workspace = np.reshape(workspace, (-1, 2))
    prob = np.linalg.norm(workspace, axis=0)**2
    return prob / prob.sum()

# Function to measure a qubit
def measureQubit(name):
    global workspace, namestack
    prob = probQubit(name)
    measurement = np.random.choice(2, p=prob)
    workspace = (workspace[:, [measurement]] / np.sqrt(prob[measurement]))
    namestack.pop()
    return str(measurement)

# Grover's search

# Subroutines:

def zero_booleanOracle(qubits,result): # all qubits zero? 
     # if all qubits==0 return 1 else return 0
     for qubit in qubits:             # negate all inputs
         applyGate(X_gate,qubit)
     TOFFn_gate(qubits,result)        # compute AND
     for qubit in qubits:             # restore inputs
         applyGate(X_gate,qubit)

def zero_phaseOracle(qubits):            # all qubits zero? 
    # if all qubits==0 return -weight else return weight
    for qubit in qubits:                 # negate all inputs
        applyGate(X_gate,qubit)
    applyGate(Z_gate,*namestack)         # controlled Z gate
    for qubit in qubits:                 # restore inputs
        applyGate(X_gate,qubit)

def sample_phaseOracle(qubits):          # sample function 
        # if all f(x)==1 return -weight else return weight
    applyGate(X_gate,qubits[1])          # negate qubit 1
    applyGate(Z_gate,*namestack)         # controlled Z gate
    applyGate(X_gate,qubits[1])          # restore qubit 1

# The Main Loop:

def groverSearch(n, printProb=True):
    optimalTurns = int(np.pi/4*np.sqrt(2**n)-1/2)   # iterations 
    qubits = list(range(n))                         # generate qubit names
    for qubit in qubits:                            # initialize qubits
        pushQubit(qubit,[1,1])
    for k in range(optimalTurns):                   # Grover iterations:
        sample_phaseOracle(qubits)                  # apply phase oracle
        for qubit in qubits:                        # H-gate all qubits
            applyGate(H_gate,qubit) 
        zero_phaseOracle(qubits)                    # apply 0 phase oracle
        for qubit in qubits:                        # H-gate all qubits
            applyGate(H_gate,qubit) 
        if printProb:                               # peek probabilities
            print(probQubit(qubits[0]))             # to show convergence
    for qubit in reversed(qubits):                  # print result 
        print(measureQubit(qubit),end="")

# Run Grover's search for 6 qubits
workspace = np.array([[1.]])  # Reset workspace
groverSearch(6)
