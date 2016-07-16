def q = qbit(|1111111111111111>);

def c = qcirc(1,16);

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
c.add(0,[11],hadamard);
c.add(0,[12],hadamard);
c.add(0,[13],hadamard);
c.add(0,[14],hadamard);
c.add(0,[15],hadamard);
