def q = qbit(|11111111111>);

def c = qcirc(1,11);

c.attach(q);

c.add(0,[0],hadamard);
c.add(0,[1],hadamard);
c.add(0,[2],hadamard);
c.add(0,[3],hadamard);
c.add(0,[4],hadamard);
c.add(0,[5],hadamard);
c.add(0,[6],hadamard);
c.add(0,[7],hadamard);
c.add(0,[8],hadamard);
c.add(0,[9],hadamard);
c.add(0,[10],hadamard);