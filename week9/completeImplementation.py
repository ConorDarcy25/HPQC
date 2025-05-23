# DIY quantum computer simulator
# Martin Nilsson, RISE, 2023-11-26

import numpy as np

def pushQubit(name,weights):
    global workspace
    global namestack
    if workspace.shape == (1,1): namestack = [] 
    namestack.append(name)
    weights = np.array(weights/np.linalg.norm(weights),
                       dtype=workspace[0,0].dtype)
    workspace = np.kron(np.reshape(workspace,(1,-1)),weights)

def tosQubit(name):
    global workspace
    global namestack
    k = len(namestack)-namestack.index(name) 
    if k > 1:
        namestack.append(namestack.pop(-k))
        workspace = np.reshape(workspace,(-1,2,2**(k-1)))
        workspace = np.swapaxes(workspace,-2,-1)

def applyGate(gate,*names): 
    global workspace
    if list(names) != namestack[-len(names):]: 
        [tosQubit(name) for name in names]
    workspace = np.reshape(workspace,(-1,2**(len(names))))
    subworkspace = workspace[:,-gate.shape[0]:]
    np.matmul(subworkspace,gate.T,out=subworkspace)
    
def probQubit(name):
    global workspace
    tosQubit(name)
    workspace = np.reshape(workspace,(-1,2)) 
    prob = np.linalg.norm(workspace,axis=0)**2 
    return prob/prob.sum()
    
def measureQubit(name):
    global workspace
    global namestack
    prob = probQubit(name)
    measurement = np.random.choice(2,p=prob) 
    workspace = (workspace[:,[measurement]]/
                 np.sqrt(prob[measurement]))
    namestack.pop()
    return str(measurement)
    
# ---------- Grover search example

X_gate = np.array([[0, 1],[1, 0]])
H_gate = np.array([[1, 1],[1,-1]])*np.sqrt(1/2)
Z_gate = H_gate @ X_gate @ H_gate

def sample_phaseOracle(qubits): 
    applyGate(X_gate,qubits[1]) 
    applyGate(Z_gate,*namestack) 
    applyGate(X_gate,qubits[1])

def zero_phaseOracle(qubits): 
[applyGate(X_gate,q) for q in qubits] 
applyGate(Z_gate,*namestack) 
[applyGate(X_gate,q) for q in qubits]

def groverSearch(n, printProb=True):
    qubits = list(range(n))
    [pushQubit(q,[1,1]) for q in qubits]
    for k in range(int(np.pi/4*np.sqrt(2**n)-1/2)):
        sample_phaseOracle(qubits)
        [applyGate(H_gate,q) for q in qubits] 
        zero_phaseOracle(qubits) 
        [applyGate(H_gate,q) for q in qubits]
        if printProb: print(probQubit(qubits[0]))
    [print(measureQubit(q),end="") for q in reversed(qubits)]

workspace = np.array([[1.]])
groverSearch(8)
