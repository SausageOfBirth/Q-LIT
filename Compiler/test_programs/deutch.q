def uf_constant1 = qgate([1,0,0,0;0,0,0,1;0,1,0,0;0,0,1,0]);

def q = qbit(|01>);

def c = qcirc(4,2);

c.attach(q);

c.add(0,[1],hadamard);
c.add(0,[0],hadamard);
c.add(1,[0,1],uf_constant1);
c.add(2,[0],hadamard);
c.add(3,[0],measurement);
c.add(3,[1],not);
