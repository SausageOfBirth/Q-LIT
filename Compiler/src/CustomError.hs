module CustomError where

-- datatype with constructors for each unique error
data CustomError = GateNotDef String String
                 | QubitNotDef String String
                 | CircuitNotDef String String
                 | GateExists String
                 | QubitExists String
                 | CircuitExists String
                 | IllegalQubit String
                 | ReturnNotUnit String String
                 | MatNotReg String String
                 | MatNotSquare String String
                 | BadQubitAssign String String String
                 | BadQubitLength String String String
                 | OutOfBounds String String String 
                 | SlotOccupied String String String 
                 | SubGateNotDef String String
                 | MatNotPower String String
                 | GateWrongDim String String
                 | GateNotFit String String String
                 | WiresNotAdj String String String
                 | ParseError String
                 | ScalarOnlyExp String String
                 | ControlNotMatrix String String
                 | ControlBitsWrong String String String

-- error messages for each particular custom error
instance Show CustomError where
  show (GateNotDef idf fun) =
    "Gate not found named "++idf++" during an operation of "++fun
  show (QubitNotDef idf fun) =
    "Qubit not found named "++idf++" during an operation of "++fun
  show (CircuitNotDef idf fun) =
    "Circuit not found named "++idf++" during an operation of "++fun
  show (GateExists idf) = 
    "Can not define gate "++idf++" because it already exists"
  show (QubitExists idf) = 
    "Can not define qubit "++idf++" because it already exists"
  show (CircuitExists idf) = 
    "Can not define circuit "++idf++" because it already exists"
  show (IllegalQubit idf) =
    "Qubit "++idf++" was not defined with bra or ket" 
  show (ReturnNotUnit idf fun) =
    "Matrix definition for "++idf++" did not result in a matrix type "++
    "during an operation of "++fun
  show (MatNotSquare idf fun) =
    "Matrix definition for "++idf++" contained a not square component "++
    "during an operation of "++fun
  show (MatNotReg idf fun) = 
    "Matrix definition for "++idf++" contained a freeform matrix component "++
    "which did not have regular rows and columns"
  show (SubGateNotDef idf fun) =
    "Matrix definition for "++idf++" attempted to use previously defined "++
    "gate in matrix expression that does not exist during an operation of "++fun
  show (MatNotPower idf fun) =
    "Matrix definition for "++idf++" has a handcrafted submatrix that does "++
    "not have dimensions that are a power of 2 during an operation of "++fun
  show (BadQubitAssign idf circ fun) =
    "Attempted to assign qubit "++idf++" to circuit "++circ++" when that "++
    "circuit already has an attached qubit during an operation of "++fun
  show (BadQubitLength idf circ fun) =
    "Attempted to assign qubit "++idf++" to circuit "++circ++" when length "++
    "of qubit does not fit on circuit during an operation of "++fun
  show (OutOfBounds idf circ fun) =
    "Can not insert gate "++idf++" on slot or wire outside the bounds of "++
    "circuit "++circ++" during an operation of "++fun
  show (SlotOccupied idf circ fun) =
    "Can not insert gate "++idf++" on position that is already occupied on "++
    "circuit "++circ++" during an operation of "++fun
  show (GateNotFit idf circ fun) =
    "Attempted to insert gate "++idf++" on circuit "++circ++" with too few "++
     "too many wires specified during an operation of "++fun
  show (GateWrongDim idf fun) =
    "Some subgates did not match in dimensions for gate "++idf++" during "++
    "an operation of "++fun
  show (WiresNotAdj idf circ fun) =
    "Attempted to insert gate "++idf++" on wires that are not adjacent on "++
    "circuit "++circ++" during an operation of "++fun
  show (ScalarOnlyExp idf fun) =
    "Attempted to use non-scalar as exponent during definition of "++idf++
    " during an operation of "++fun
  show (ControlNotMatrix idf fun) =
    "Operation for a control gate was not a one-bit handcrafted matrix for "++
    "self-defined gate "++idf++" during an operation of "++fun
  show (ControlBitsWrong idf circ fun) =
    "Control bit was larger or equal too target bit during insertion of "++
    idf++" on"++" circuit "++circ++" during an operation of "++fun

  show (ParseError s) = s

instance Eq CustomError where
  x == y = checkEqError x y

-- eq function that allows us to only check and test against error type
checkEqError x y = case (x,y) of
  (GateNotDef {}, GateNotDef {})            -> True
  (QubitNotDef {}, QubitNotDef {})          -> True
  (CircuitNotDef {}, CircuitNotDef {})      -> True
  (GateExists {}, GateExists {})            -> True
  (QubitExists {}, QubitExists {})          -> True
  (CircuitExists {}, CircuitExists {})      -> True
  (IllegalQubit {}, IllegalQubit {})        -> True
  (ReturnNotUnit {}, ReturnNotUnit {})      -> True
  (MatNotReg {}, MatNotReg {})              -> True
  (MatNotSquare {}, MatNotSquare {})        -> True
  (BadQubitAssign {}, BadQubitAssign {})    -> True
  (BadQubitLength {}, BadQubitLength {})    -> True
  (OutOfBounds {}, OutOfBounds {})          -> True
  (SlotOccupied {}, SlotOccupied {})        -> True
  (SubGateNotDef{}, SubGateNotDef{})        -> True
  (MatNotPower {}, MatNotPower {})          -> True
  (GateNotFit {}, GateNotFit {})            -> True
  (GateWrongDim {}, GateWrongDim{})         -> True
  (WiresNotAdj {}, WiresNotAdj{})           -> True
  (ParseError {}, ParseError{})             -> True
  (ScalarOnlyExp {}, ScalarOnlyExp{})       -> True
  (ControlNotMatrix {}, ControlNotMatrix{}) -> True
  (ControlBitsWrong {}, ControlBitsWrong{}) -> True
  _                                         -> False
