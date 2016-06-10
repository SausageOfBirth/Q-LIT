def q = qbit(|1111111>);

def c = qcirc(1,7);

c.attach(q);

c.add(0,[0],not);
c.add(0,[1],not);
c.add(0,[2],not);
c.add(0,[3],not);
c.add(0,[4],not);
c.add(0,[5],not);
c.add(0,[6],not);