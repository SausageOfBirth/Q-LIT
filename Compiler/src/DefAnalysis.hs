module DefAnalysis(
  filter_def,
  filter_bit,
  filter_mat,
  filter_dcirc,
  control,
  MExprA(..)
) where

import QuantumParser
import Data.List
import QSyntax 

-- is needed for when we must determine the correct functions between constructs
data MExprA = Atom String MExpr
            | Comp (String,String) MExprA MExprA
            deriving (Show,Eq)

-- take Def statements out of list of all statements
filter_def (Right (Seq stmt_list)) = [ i | i@Def {} <- stmt_list]

-- take Matrix statements out of list of Insert statements
filter_mat :: [Stmt] -> [(String,Stmt)]
filter_mat []     = []
filter_mat (d:ds) = let Def s q = d in
                      case q of
                        QGate mat -> (s, d) : filter_mat ds
                        _         -> filter_mat ds

-- take Qubit statements out of list of Insert statements 
filter_bit :: [Stmt] -> [(String,Stmt)] 
filter_bit []     = []
filter_bit (d:ds) = let Def s q = d in
                      case q of
                        QBit bit -> (s, d) : filter_bit ds
                        _        -> filter_bit ds

-- take Circuit statements out of list of Insert statements 
filter_dcirc :: [Stmt] -> [(String,Stmt)] 
filter_dcirc []     = []
filter_dcirc (d:ds) = let Def s q = d in
                      case q of
                        c@QCirc {} -> (s, d) : filter_dcirc ds
                        _          -> filter_dcirc ds

-- control takes a matrix expression and returns a structure for codegen
control :: MExpr -> MExprA
control mexpr = case mexpr of
  i@IntConst {}    -> Atom "scalar" i
  b@Bra {}         -> Atom "bra" b
  k@Ket {}         -> Atom "ket" k
  m@Matrix {}      -> Atom "matrix" m
  s@GateIdf {}     -> Atom "matrix" s
  MBinary op m1 m2 -> dispatch_wrapper op (control m1) (control m2)

-- Exhausts the possibilities for binary expressions in new structure
dispatch_wrapper :: MBinOp -> MExprA -> MExprA -> MExprA
dispatch_wrapper op m1 m2 = 
  case (m1,m2) of 
    (Atom s1 _, Atom s2 _)             -> f s1 s2 m1 m2
    (Comp (_,s1) _ _, Atom s2 _)       -> f s1 s2 m1 m2
    (Atom s1 _, Comp (_,s2) _ _)       -> f s1 s2 m1 m2
    (Comp (_,s1) _ _, Comp (_,s2) _ _) -> f s1 s2 m1 m2
    where f s1 s2 m1 m2 = let (fn,rt,p) = (dispatch_op op s1 s2) in
                          if p == 't'
                          then Comp (fn,rt) m2 m1
                          else Comp (fn,rt) m1 m2

-- Returns the correct pair of (function,return values) to use in C-code
dispatch_op :: MBinOp -> String -> String -> (String,String,Char)
dispatch_op op m1 m2 = 
  case op of
  Multiply  -> mult_return m1 m2
  Add       -> add_return m1 m2
  Subtract  -> sub_return m1 m2
  Kronecker -> kron_return m1 m2
  Exponent  -> exp_return m1 m2

-- matches functions for multiplication
mult_return :: String -> String -> (String,String,Char)
mult_return s1 s2
  | s1 == "bra" && s2 == "ket" = ("innerProduct","scalar",'f')
  | s1 == "ket" && s2 == "bra" = ("outerProduct","matrix",'f')
  | s1 == "bra" && s2 == "bra" = ("multQ","bra",'f')
  | s1 == "ket" && s2 == "ket" = ("multQ","ket",'f')
  | s1 == "scalar" && s2 == "bra" = ("scalarQ","bra",'t')
  | s1 == "scalar" && s2 == "ket" = ("scalarQ","ket",'t')
  | s1 == "bra" && s2 == "scalar" = ("scalarQ","bra",'f')
  | s1 == "ket" && s2 == "scalar" = ("scalarQ","ket",'f')
  | s1 == "matrix" && s2 == "bra" = ("multGateBit","bra",'f')
  | s1 == "matrix" && s2 == "ket" = ("multGateBit","ket",'f')
  | s1 == "bra" && s2 == "matrix" = ("multGateBit","bra",'t')
  | s1 == "ket" && s2 == "matrix" = ("multGateBit","ket",'t')
  | s1 == "matrix" && s2 == "matrix" = ("multG","matrix",'f')
  | s1 == "scalar" && s2 == "matrix" = ("scalarG","matrix",'t')
  | s1 == "matrix" && s2 == "scalar" = ("scalarG","matrix",'f')
  | otherwise = error $ "some multiplication not supported"

-- matches functions for addition
add_return :: String -> String -> (String,String,Char)
add_return s1 s2
  | s1 == "bra" = ("plusQ","bra",'f')
  | s1 == "ket" = ("plusQ","ket",'f')
  | s1 == "matrix" = ("plusG","matrix",'f')
  | s1 == "scalar" && s2 == "matrix" = ("plusG","matrix",'t')
  | s1 == "scalar" = ("+","scalar",'f')

-- matches functions for negation 
sub_return :: String -> String -> (String,String,Char)
sub_return s1 s2
  | s1 == "bra" = ("minusQ","bra",'f')
  | s1 == "ket" = ("minusQ","ket",'f')
  | s1 == "matrix" = ("minusG","matrix",'f')
  | s1 == "scalar" && s2 == "matrix" = ("minusG","matrix",'t')
  | s1 == "scalar" = ("-","scalar",'f')

-- matches functions for kronecker
kron_return :: String -> String -> (String,String,Char)
kron_return s1 s2
  | s1 == "scalar" && s2 == "scalar" = ("*","scalar",'f')
  | s1 == "scalar" && s2 == "matrix" = ("scalarG","matrix",'f')
  | s1 == "matrix" && s2 == "scalar" = ("scalarG","matrix",'t')
  | s1 == "matrix" && s2 == "matrix" = ("tensor","matrix",'f')

exp_return :: String -> String -> (String,String,Char)
exp_return s1 s2 
 | s1 == "scalar" = ("cpow","scalar",'f')
 | s1 == "matrix" = ("expG","matrix",'f')
 | s1 == "bra"    = ("expQ","bra",'f')
 | s1 == "ket"    = ("expQ","ket",'f')
