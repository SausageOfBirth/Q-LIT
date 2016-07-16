def q = qbit(|11111>);

def c = qcirc(5,5);

c.attach(q);

c.add(0,[0],hadamard);
c.add(0,[1],hadamard);
c.add(0,[2],hadamard);
c.add(0,[3],hadamard);
c.add(0,[4],hadamard);
c.add(1,[0],hadamard);
c.add(1,[1],hadamard);
c.add(1,[2],hadamard);
c.add(1,[3],hadamard);
c.add(1,[4],hadamard);
c.add(2,[0],hadamard);
c.add(2,[1],hadamard);
c.add(2,[2],hadamard);
c.add(2,[3],hadamard);
c.add(2,[4],hadamard);
c.add(3,[0],hadamard);
c.add(3,[1],hadamard);
c.add(3,[2],hadamard);
c.add(3,[3],hadamard);
c.add(3,[4],hadamard);
c.add(4,[0],hadamard);
c.add(4,[1],hadamard);
c.add(4,[2],hadamard);
c.add(4,[3],hadamard);
c.add(4,[4],hadamard);