def q = qbit(|1>);

def c = qcirc(7,1);

c.attach(q);

c.add(0,[0],not);
c.add(1,[0],not);
c.add(2,[0],not);
c.add(3,[0],not);
c.add(4,[0],not);
c.add(5,[0],not);
c.add(6,[0],not);