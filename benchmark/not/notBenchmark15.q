def q = qbit(|1111111111111111>);

def c = qcirc(1,16);

c.attach(q);

c.add(0,[0],not);
c.add(0,[1],not);
c.add(0,[2],not);
c.add(0,[3],not);
c.add(0,[4],not);
c.add(0,[5],not);
c.add(0,[6],not);
c.add(0,[7],not);
c.add(0,[8],not);
c.add(0,[9],not);
c.add(0,[10],not);
c.add(0,[11],not);
c.add(0,[12],not);
c.add(0,[13],not);
c.add(0,[14],not);
c.add(0,[15],not);
