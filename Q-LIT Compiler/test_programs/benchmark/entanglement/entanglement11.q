def q = qbit(|00000000000>);

def c = qcirc(12,11);

c.attach(q);

c.add(0,[0],hadamard);

c.add(1,[0;1],not);
c.add(2,[0;2],not);
c.add(3,[0;3],not);
c.add(4,[0;4],not);
c.add(5,[0;5],not);
c.add(6,[0;6],not);
c.add(7,[0;7],not);
c.add(8,[0;8],not);
c.add(9,[0;9],not);
c.add(10,[0;10],not);

c.add(11,[0],measurement);
