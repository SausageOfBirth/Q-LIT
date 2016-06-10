def q = qbit(|1111>);

def c = qcirc(1,4);

c.attach(q);

c.add(0,[0],not);
c.add(0,[1],not);
c.add(0,[2],not);
c.add(0,[3],not);