# INSTRUCTION 1: Push a Qubit onto the stack

import numpy as np

def pushQubit(weights):
    global workspace
    workspace = np.reshape(workspace,(1,-1)) workspace = np.kron(workspace,weights)

workspace = np.array([[1.]])       # create empty qubit stack pushQubit([1,0])
pushQubit([1,0])
print(workspace)
pushQubit([3/5,4/5])               # push a 2nd qubit print(workspace)
print(workspace)

# INSTRUCTION 2: Perform a gate operation

# Example of applyGate

def applyGate(gate):
    global workspace
    workspace = np.reshape(workspace,(-1,gate.shape[0]))     
    np.matmul(workspace,gate.T,out=workspace)

X_gate = np.array([[0, 1],   # Pauli X gate 
                  [1, 0]])   # = NOT gate

# Qubit of 0

 np.matmul(X_gate,[1,0])

# New instructions 

workspace = np.array([[1.]])       # reset workspace 
pushQubit([1,0])
print("input",workspace)
applyGate(X_gate)                  # = NOT 
print("output",workspace)

# Hadamard Gate

H_gate = np.array([[1, 1],                         # Hadamard gate 
                  [1,-1]]) * np.sqrt(1/2)

# Code examples 

workspace = np.array([[1.]])
pushQubit([1,0])
print("input",workspace)
applyGate(H_gate)
print("output",workspace)

# Initialise with a complete data set

T_gate = np.array([[1,                0],
                   [0,np.exp(np.pi/-4j)]])
workspace = np.array([[1.+0j]])       # set complex workspace pushQubit([.6,.8])
print("input",workspace)
applyGate(T_gate)
print("output",workspace)

# Swap gate 

SWAP_gate = np.array([[1, 0, 0, 0], # Swap gate [0, 0, 1, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1]])

workspace = np.array([[1.]])
pushQubit([1,0])                          # qubit 1
pushQubit([0.6,0.8])                      # qubit 2
print(workspace)
applyGate(SWAP_gate)
print(workspace)

# List of common gates

X_gate = np.array([[0, 1],                      # Pauli X gate
                   [1, 0]])                     # = NOT gate

Y_gate = np.array([[ 0,-1j],                    # Pauli Y gate
                   [1j,  0]])                   # = SHZHZS
  
Z_gate = np.array([[1, 0],                      # Pauli Z gate
                   [0,-1]])                     # = P(pi) = S^2
                                                # = HXH

H_gate = np.array([[1, 1],                      # Hadamard gate 
                   [1,-1]]) * np.sqrt(1/2)

S_gate = np.array([[1, 0],                      # Phase gate
                   [0,1j]])                     # = P(pi/2) = T^2
                   
T_gate = np.array([[1,                0],       # = P(pi/4)
                   [0,np.exp(np.pi/-4j)]])
                   
Tinv_gate = np.array([[1, 0],                   # = P(-pi/4) 
                      [0,np.exp(np.pi/4j)]])    # = T^-1
                      
def P_gate(phi):                                # Phase shift gate
    return np.array([[1,             0],
                     [0,np.exp(phi*1j)]])
                     
def Rx_gate(theta):                             # Y rotation gate
    return np.array([[np.cos(theta/2),-1j*np.sin(theta/2)],
                     [-1j*np.sin(theta/2),np.cos(theta/2)]])
                     
def Ry_gate(theta):                             # Y rotation gate return 
    np.array([[np.cos(theta/2),-np.sin(theta/2)],
              [np.sin(theta/2), np.cos(theta/2)]])
              
def Rz_gate(theta):                             # Z rotation gate 
    return np.array([[np.exp(-1j*theta/2),                0],
                     [                  0,np.exp(1j*theta/2]])
                     
CNOT_gate = np.array([[1, 0, 0, 0],             # Ctled NOT gate
                      [0, 1, 0, 0],             #=XORgate
                      [0, 0, 0, 1],
                      [0, 0, 1, 0]])
                      
CZ_gate = np.array([[1, 0, 0, 0],               # Ctled Z gate
                    [0, 1, 0, 0],
                    [0, 0, 1, 0],
                    [0, 0, 0,-1]])
                    
SWAP_gate = np.array([[1, 0, 0, 0],             # Swap gate
                      [0, 0, 1, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1]])
                      
TOFF_gate = np.array([[1, 0, 0, 0, 0, 0, 0, 0], # Toffoli gate
                     [0, 1, 0, 0, 0, 0, 0, 0],
                     [0, 0, 1, 0, 0, 0, 0, 0],
                     [0, 0, 0, 1, 0, 0, 0, 0],
                     [0, 0, 0, 0, 1, 0, 0, 0],
                     [0, 0, 0, 0, 0, 1, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0, 1],
                     [0, 0, 0, 0, 0, 0, 1, 0]])

# INSTRUCTION 3: Move a Qubit to the Top of the Stack

def tosQubit(k):
global workspace
if k > 1:                                               # if non-trivial
    workspace = np.reshape(workspace,(-1,2,2**(k-1)))
    workspace = np.swapaxes(workspace,-2,-1)

# tosQubit

workspace = np.array([[1.]])
pushQubit([1,0])
pushQubit([0.6,0.8])
print(workspace)
tosQubit(2)
print(workspace)

# INSTRUCTION 4: Measure a Qubit

def probQubit():
    global workspace
    workspace = np.reshape(workspace,(-1,2)) 
    return np.linalg.norm(workspace,axis=0)**2
def measureQubit():
    global workspace
    prob = probQubit()
    measurement = np.random.choice(2,p=prob)         # select 0 or 1 
    workspace = (workspace[:,[measurement]]/
    np.sqrt(prob[measurement])) 
    return str(measurement)

# Example of measureQubit

workspace = np.array([[1. ]])
for n in range(30):
    pushQubit([0.6,0.8])
    print(measureQubit(), end="")

# Advanced example

workspace = np.array([[1.]]) 
for i in range(16):
    pushQubit([1,0])                      # push a zero qubit
    applyGate(H_gate)                     # set equal 0 and 1 probability
    pushQubit([1,0])                      # push a 2nd zero qubit
    applyGate(H_gate)                     # set equal 0 and 1 probability
    pushQubit([1,0])                      # push a dummy zero qubit
    applyGate(TOFF_gate)                  # compute Q3 = Q1 AND Q2
    q3 = measureQubit()                   # pop qubit 3
    q2 = measureQubit()                   # pop qubit 2
    q1 = measureQubit()                   # pop qubit 1
    print(q1+q2+q3,end=",")

# CODE IMPROVEMENTS:

# Improved pushQubit

def pushQubit(name,weights):
    global workspace
    global namestack
    if workspace.shape == (1,1):                  # if workspace empty
        namestack = []                            # then reset
    namestack.append(name)                        # push name
    weights = weights/np.linalg.norm(weights)     # normalize 
    weights = np.array(weights,dtype=workspace[0,0].dtype) 
    workspace = np.reshape(workspace,(1,-1))      # to row vector 
    workspace = np.kron(workspace,weights)

# Improved tosQubit

def tosQubit(name):
global workspace
global namestack
k = len(namestack)-namestack.index(name)    # qubit pos
if k > 1:                                   # if non-trivial
namestack.append(namestack.pop(-k))         # rotate name stack 
workspace = np.reshape(workspace,(-1,2,2**(k-1))) 
workspace = np.swapaxes(workspace,-2,-1)

# Improved applyGate

def applyGate(gate,*names):
global workspace
for name in names:                   # move qubits to TOS
        tosQubit(name)
    workspace = np.reshape(workspace,(-1,gate.shape[0]))
    np.matmul(workspace,gate.T,out=workspace)

# Imrpoved measureQubit

def probQubit(name):
    global workspace
    tosQubit(name)
    workspace = np.reshape(workspace,(-1,2))
    prob = np.linalg.norm(workspace,axis=0)**2
    return prob/prob.sum()                 # make sure sum is one

def measureQubit(name): 
    global workspace 
    global namestack
    prob = probQubit(name)
    measurement = np.random.choice(2,p=prob)
    workspace = (workspace[:,[measurement]]/
                 np.sqrt(prob[measurement]))
    namestack.pop()
    return str(measurement)

# Controlled Gates and Recycling of Qubits

def TOFF3_gate(q1,q2,q3,q4):
    pushQubit("temp",[1,0]) applyGate(TOFF_gate,q1,q2,"temp")         
    applyGate(TOFF_gate,"temp",q3,q4) 
    applyGate(TOFF_gate,q1,q2,"temp")         # restore temp
    measureQubit("temp")                      # t is surely zero
    
workspace = np.array([[1.]])                  # test!
for i in range(20):                           # generate truth table
pushQubit("Q1",[1,1]) 
pushQubit("Q2",[1,1]) 
pushQubit("Q3",[1,1]) 
pushQubit("Q4",[1,0])                         # Q4 starts at zero so
TOFF3_gate("Q1","Q2","Q3","Q4")               # Q4 = AND of Q1 thru Q3
print("".join([measureQubit(q) for q in
                   ["Q1","Q2","Q3","Q4"]]), end=",")

# Generalised Toffi gate 

def TOFFn_gate(ctl,result): # result = result XOR AND(qubits) 
    n = len(ctl)
    if n == 0: 
        applyGate(X_gate,result)
    if n == 1: 
        applyGate(CNOT_gate,ctl[0],result)
    elif n == 2: 
        applyGate(TOFF_gate,ctl[0],ctl[1],result)
    elif n > 2: 
        k=0
        while "temp"+str(k) in namestack: 
            k=k+1
        temp = "temp"+str(k)        # generate unique name 
        pushQubit(temp,[1,0])       # push zero temp qubit 
        applyGate(TOFF_gate,ctl[0],ctl[1],temp) # apply TOFF 
        ctl.append(temp)            # add temp to controls 
        TOFFn_gate(ctl[2:],result)  # recursion 
        applyGate(TOFF_gate,ctl[0],ctl[1],temp) # uncompute temp 
        measureQubit(temp)          # pop temp
workspace = np.array([[1]],dtype=np.single)     # test!
for i in range(20):                 # generate truth table
    pushQubit("Q1",[1,1])
    pushQubit("Q2",[1,1])
    pushQubit("Q3",[1,1])
    pushQubit("Q4",[1,0])               # Q4 starts at zero, becomes 
    TOFFn_gate(["Q1","Q2","Q3"],"Q4")   # AND of Q1 thru Q3 
    print("".join([measureQubit(q) for q in
               ["Q1","Q2","Q3","Q4"]]),end=",")
