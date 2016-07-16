def q = qbit(|11>);

def c = qcirc(2,2);

c.attach(q);

c.add(0,[0],not);
c.add(0,[1],not);
c.add(1,[1],not);
c.add(1,[0],not);