def q = qbit(|00>);

def c = qcirc(3,2);

c.attach(q);

c.add(0,[0],hadamard);

c.add(1,[0;1],not);

c.add(2,[0],measurement);
