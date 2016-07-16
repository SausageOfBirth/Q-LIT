def q = qbit(|1>);

def c = qcirc(2,1);

c.attach(q);

c.add(0,[0],not);
c.add(1,[0],not);