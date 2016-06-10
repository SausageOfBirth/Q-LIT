def identity4 = qgate(identity # identity # identity # identity);
def u_s = qgate(2 * (|0000> * <0000|) - identity4);

def not2 = qgate([1,0;0,1]);

def q = qbit(|00001>);

def c = qcirc(11,5);

c.attach(q);

c.add(0,[0], hadamard);
c.add(0,[1], hadamard);
c.add(0,[2], hadamard);
c.add(0,[3], hadamard);
c.add(0,[4], hadamard);

-- black box of magic goes here

c.add(1,[0], hadamard);
c.add(1,[1], hadamard);
c.add(1,[2], hadamard);
c.add(1,[3], hadamard);

c.add(2,[0,1,2,3], u_s);

c.add(3,[0], hadamard);
c.add(3,[1], hadamard);
c.add(3,[2], hadamard);
c.add(3,[3], hadamard);

-- black box of magic goes here

c.add(4,[0], hadamard);
c.add(4,[1], hadamard);
c.add(4,[2], hadamard);
c.add(4,[3], hadamard);

c.add(5,[0,1,2,3], u_s);

c.add(6,[0], hadamard);
c.add(6,[1], hadamard);
c.add(6,[2], hadamard);
c.add(6,[3], hadamard);

-- black box of magic goes here

c.add(7,[0], hadamard);
c.add(7,[1], hadamard);
c.add(7,[2], hadamard);
c.add(7,[3], hadamard);

c.add(8,[0,1,2,3], u_s);

c.add(9,[0], hadamard);
c.add(9,[1], hadamard);
c.add(9,[2], hadamard);
c.add(9,[3], hadamard);

c.add(10,[2;4], not);

-- let's just say that's enough

-- c.add(10,[0], measurement);
