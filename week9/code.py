import torch as pt
pt.autograd.set_grad_enabled(False)      # disable autogradients
if pt.cuda.is_available():               # check if GPU is available
    print("GPU available") 
else:
    print("Sorry, only CPU available")

def pushQubit(name,weights):
    global workspace
    global namestack
    if (workspace.shape[0],workspace.shape[1]) == (1,1): #!
        namestack = []                                   # reset if workspace empty 
    namestack.append(name)
    weights = weights/np.linalg.norm(weights)            # normalize 
    weights = pt.tensor(weights,device=workspace.device, #!
                        dtype=workspace[0,0].dtype)      #! 
    workspace = pt.reshape(workspace,(1,-1))             #! 
    workspace = pt.kron(workspace,weights)               #!
    
def tosQubit(name):
    global workspace
    global namestack
    k = len(namestack)-namestack.index(name)                  # position of qubit 
    if k > 1:                                                 # if non-trivial
        namestack.append(namestack.pop(-k))
        workspace = pt.reshape(workspace,(-1,2,2**(k-1)))     #! 
        workspace = pt.swapaxes(workspace,-2,-1)              #!
        
def applyGate(gate,*names):
    global workspace
    if list(names) != namestack[-len(names):]:                # reorder stack
        for name in names:                                    # if necessary 
            tosQubit(name)
    workspace = pt.reshape(workspace,(-1,2**len(names)))      #!
    subworkspace = workspace[:,-gate.shape[0]:]
    gate = pt.tensor(gate.T,device=workspace.device,          #! 
                     dtype=workspace[0,0].dtype)              #! 
    if workspace.device.type == 'cuda':                       #! 
        pt.matmul(subworkspace,gate,out=subworkspace)         #!
    else:    #! workaround for issue #114350 in torch.matmul 
        subworkspace[:,:]=pt.matmul(subworkspace,gate) #!
        
def probQubit(name):                             # Check probabilities
    global workspace                             # of qubit being 0 or 1
    tosQubit(name)                               # qubit to TOS
    workspace = pt.reshape(workspace,(-1,2))     #! to 2 cols
    prob = pt.linalg.norm(workspace,axis=0)**2   #! compute prob 
    prob = pt.Tensor.cpu(prob).numpy()           #! convert to numpy
    return prob/prob.sum()                       # make sure sum is one
    
def measureQubit(name):                          # Measure and pop qubit
    global workspace
    global namestack
    prob = probQubit(name)                      # Compute probabilities
    measurement = np.random.choice(2,p=prob)    # 0 or 1 
    workspace = (workspace[:,[measurement]]/    # extract col
                 np.sqrt(prob[measurement])) 
    namestack.pop()                             # pop stacks
    return measurement
    
import time
workspace = pt.tensor([[1.]],device=pt.device('cuda'),
                             dtype=pt.float32) 
t = time.process_time()                               # with GPU
groverSearch(16, printProb=False)                     # skip prob printouts 
print("\nWith GPU:", time.process_time() - t, "s")
workspace = pt.tensor([[1.]],device=pt.device('cpu'),
                             dtype=pt.float32)
t = time.process_time()                               # with CPU
groverSearch(16, printProb=False)                     # skip prob printouts 
print("\nWith CPU (single-core):", time.process_time() - t, "s")
