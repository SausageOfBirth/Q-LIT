def q = qbit(|111>);

def c = qcirc(1,3);

c.attach(q);

c.add(0,[0],hadamard);
c.add(0,[1],hadamard);
c.add(0,[2],hadamard);