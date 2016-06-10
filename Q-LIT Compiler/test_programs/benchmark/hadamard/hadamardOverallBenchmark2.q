def q = qbit(|111>);

def c = qcirc(3,3);

c.attach(q);

c.add(0,[0],hadamard);
c.add(0,[1],hadamard);
c.add(0,[2],hadamard);
c.add(1,[0],hadamard);
c.add(1,[1],hadamard);
c.add(1,[2],hadamard);
c.add(2,[0],hadamard);
c.add(2,[1],hadamard);
c.add(2,[2],hadamard);