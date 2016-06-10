def q = qbit(|1111>);

def c = qcirc(4,4);

c.attach(q);

c.add(0,[0],not);
c.add(0,[1],not);
c.add(0,[2],not);
c.add(0,[3],not);
c.add(1,[0],not);
c.add(1,[1],not);
c.add(1,[2],not);
c.add(1,[3],not);
c.add(2,[0],not);
c.add(2,[1],not);
c.add(2,[2],not);
c.add(2,[3],not);
c.add(3,[0],not);
c.add(3,[1],not);
c.add(3,[2],not);
c.add(3,[3],not);