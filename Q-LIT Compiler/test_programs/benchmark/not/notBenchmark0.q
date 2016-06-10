def q = qbit(|1>);

def c = qcirc(1,1);

c.attach(q);

c.add(0,[0],not);