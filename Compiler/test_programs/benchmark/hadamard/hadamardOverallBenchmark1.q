def q = qbit(|11>);

def c = qcirc(2,2);

c.attach(q);

c.add(0,[0],hadamard);
c.add(0,[1],hadamard);
c.add(1,[1],hadamard);
c.add(1,[0],hadamard);