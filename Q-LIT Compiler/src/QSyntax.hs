module QSyntax where

data Stmt = Empty
          | Seq [Stmt]
          | Def String QExpr
          | Attach String String  
          | Insert String String Integer [Integer] Bool
          deriving (Show,Eq)

-- quantum element assignment expression
data QExpr = QBit MExpr
           | QGate MExpr
           | QCirc Integer Integer
           deriving (Show,Eq)

-- matrix arithmetic expression
data MExpr = IntConst CExpr
           | MBinary MBinOp MExpr MExpr
           | Bra String
           | Ket String
           | Matrix [[CExpr]]
           | GateIdf String
           deriving (Show,Eq)

data CExpr = Complex Double Double
           | Constant String
           deriving (Show, Eq)

-- matrix operators 
data MBinOp = Add
            | Subtract
            | Multiply
            | Divide
            | Kronecker
            | Exponent
            deriving (Show,Eq)
