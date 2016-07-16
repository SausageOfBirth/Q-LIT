def q = qbit(|1111111>);

def c = qcirc(1,7);

c.attach(q);

c.add(0,[0],hadamard);
c.add(0,[1],hadamard);
c.add(0,[2],hadamard);
c.add(0,[3],hadamard);
c.add(0,[4],hadamard);
c.add(0,[5],hadamard);
c.add(0,[6],hadamard);