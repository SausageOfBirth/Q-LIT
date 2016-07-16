module SyntaxAnalysis where

-- libraries
import Control.Monad.Except 
import Data.Either

-- data types
import CustomError
import QSyntax

-- source files
import QuantumParser
import DefAnalysis

-- type aliases for syntax structures (to be thrown away postsuccesful analysis)
type Gate = (String,Integer)
type Qubit = (String,String)
type Circuit = (String,Integer,Integer,Objects,String) -- slot/wires
type Objects = [(Integer,Integer)]
type State = ([Gate],[Qubit],[Circuit])

-- type alias for possible error 
type StateMonad = Either CustomError

-- standard gates
std = [("hadamard",2),("not",2),("cnot",4),("identity",2),("measurement",2)]

-- Extract statement from parseExpr Either and initialize state
initialize parseRes = case parseRes of
  Right stmt -> Right (stmt, ([],[],[]))
  Left e     -> Left e

-- Pass through AST and register statements (identifers, inserts etc.)
pass :: (Stmt,State) -> StateMonad (Stmt,State)
pass (stmt, state) = case stmt of
  Empty -> Right (stmt,state)
  Seq (s:[]) -> pass (s,state)
  Seq (s:stmtlist) ->
    let new_state = pass (s,state)
    in case new_state of 
      Right (_,ns) -> pass (Seq stmtlist, ns)
      Left e       -> Left e
  Def idf qexpr    -> 
    let def' = def idf qexpr state in case def' of
      Right st -> Right (Empty,st)
      Left  e  -> Left e
  Attach idf qubit -> 
    let attach' = attach idf qubit state in case attach' of
      Right st -> Right (Empty,st)
      Left e   -> Left e
  Insert circ gate slot wires control -> 
    let insert' = insert circ gate slot wires state control in case insert' of
      Right st -> Right (Empty,st)
      Left e   -> Left e

-- getter for defined circuits
find_c :: String -> String -> [Circuit] -> StateMonad Circuit
find_c fun circ c = let c'  = [(i,x,y,o,q) | (i,x,y,o,q) <- c, i==circ]
                    in case c' of
                      []     -> throwError $ CircuitNotDef circ fun
                      (x:xs) -> return x

-- getter for defined gates
find_g :: String -> String -> [Gate] -> StateMonad Gate
find_g fun gate g = let g' = [x | x@(i,dim) <- g, i==gate]
                          ++ [x | x@(i,dim) <- std, i==gate]
                    in case g' of
                      []     -> throwError $ GateNotDef gate fun
                      (x:xs) -> return x

-- getter for qubits
find_q :: String -> String -> [Qubit] -> StateMonad Qubit
find_q fun qubit q = let q' = [ bit | bit@(i,val) <- q, i==qubit]
                     in case q' of
                       []     -> throwError $ QubitNotDef qubit fun
                       (x:xs) -> return x

-- wrapper for find functions 
find_w f fun i con = (f fun i con) `catchError` Left 

-- registers a definition
def :: String -> QExpr -> State -> StateMonad State
def idf qexpr (gates, qubits, circuits) =
  let fun = "Definition"
      avail = check_avail idf gates qubits circuits
  in case avail of
    Right _ ->
      case qexpr of
        QGate expr -> 
          let checks = map (\f -> mat_w f fun idf expr) 
                      [unit_mat_m, final_return, check_reg_m, check_power_m,
                       check_exp_m]
                   ++ [(subgate_def_m fun idf expr gates) `catchError` Left,
                       (check_dim_m fun idf expr gates) `catchError` Left]
          in case lefts checks of
                        (x:xs) -> Left x
                        [] -> Right ((idf,rd expr gates):gates,qubits,circuits)
        QBit expr  -> case expr of
                        Bra s -> Right (gates,(idf,s):qubits,circuits)
                        Ket s -> Right (gates,(idf,s):qubits,circuits)
                        _     -> Left (IllegalQubit idf)
        QCirc x y -> Right (gates,qubits,(idf,x,y,[],""):circuits)
    Left e -> Left e

-- Assigns a dimension to custom defined gate
rd :: MExpr -> [Gate] -> Integer
rd (Matrix c_list) gates    = (toInteger . length) c_list
rd (GateIdf idf) gates      = let f = find_g "" idf gates
                              in case f of Right res -> snd res
rd (MBinary op m1 m2) gates = case op of
  Kronecker -> (rd m1 gates) * (rd m2 gates)
  Multiply  -> case (m1,m2) of
    (Ket q1, Bra q2) -> let x = (toInteger . length) q1 in x - 2
    (Bra g1, Ket q2) -> 1
    _                -> max (rd m1 gates) (rd m2 gates)
  _         -> max (rd m1 gates) (rd m2 gates)
rd _ _ = 1
                                   
-- wrapper for availability function
check_avail_w f idf g q c = (check_avail idf g q c) `catchError` Left

-- functions to check if identifiers are available
check_avail idf g q c = 
  let g' = [i | (i,dim) <- g, i==idf]
        ++ [i | (i,dim) <- std, i==idf]
      q' = [i | (i,_) <- q, i==idf]
      c' = [i | (i,_,_,_,_) <- c, i==idf]
  in case g' of
    (x:xs) -> throwError $ GateExists idf
    []     -> 
      case q' of
        (x:xs) -> throwError $ QubitExists idf
        []     ->
          case c' of
            (x:xs) -> throwError $ CircuitExists idf
            []     -> return idf

-- check if final return type for function is matrix 
final_return :: String -> String -> MExpr -> StateMonad String
final_return fun idf mat = 
  let ret = control mat
    in case ret of
      Comp (_,cret) _ _ -> if cret /= "matrix"
                           then throwError $ ReturnNotUnit idf fun
                           else return idf
      Atom cret _       -> if cret /= "matrix"
                           then throwError $ ReturnNotUnit idf fun
                           else return idf

-- check only if atomary matrix is of same length and width
unit_mat_m :: String -> String -> MExpr -> StateMonad String
unit_mat_m fun idf mat = if unit_mat mat
                         then throwError $ MatNotSquare idf fun
                         else return idf

unit_mat :: MExpr -> Bool
unit_mat (Matrix mat) = unit_mat' mat
unit_mat (MBinary _ m1 m2) = (unit_mat m1) || (unit_mat m2)
unit_mat _ = False

unit_mat' :: [[CExpr]] -> Bool
unit_mat' mat = let wid = toInteger $ length mat
                    len = toInteger . length $ head mat 
                in wid /= len

-- find all atomary matrices and check if columns and rows are regular
check_reg_m :: String -> String -> MExpr -> StateMonad String
check_reg_m fun idf mat = if check_reg mat
                          then throwError $ MatNotReg idf fun
                          else return idf

check_reg :: MExpr -> Bool
check_reg (Matrix mat) = check_reg' mat
check_reg (MBinary _ m1 m2) = check_reg m1 || check_reg m2 
check_reg _ = False

check_reg' :: [[CExpr]] -> Bool
check_reg' mat = 
  let dim = toInteger . length $ head mat
  in foldl (\acc x -> toInteger (length x) /= dim || acc) False mat

-- check that the size of handcrafted matrices is a power of 2
check_power_m :: String -> String -> MExpr -> StateMonad String
check_power_m fun idf mat = if check_power mat
                            then throwError $ MatNotPower idf fun
                            else return idf

check_power :: MExpr -> Bool
check_power (Matrix mat) = check_power' mat
check_power (MBinary _ m1 m2) = check_power m1 || check_power m2 
check_power _ = False

check_power' :: [[CExpr]] -> Bool
check_power' mat = let wid = toInteger $ length mat
                   in notElem wid $ map (2^) [1..wid]

-- check that all predefined gates used are actually defined
subgate_def_m :: String -> String -> MExpr -> [Gate] -> StateMonad String
subgate_def_m fun idf mat g = if subgate_def mat g
                              then throwError $ SubGateNotDef idf fun
                              else return idf

subgate_def :: MExpr -> [Gate] -> Bool
subgate_def (GateIdf mat) g = subgate_def' mat g
subgate_def (MBinary _ m1 m2) g = (subgate_def m1 g) || (subgate_def m2 g)
subgate_def _ g = False

subgate_def' :: String -> [Gate] -> Bool
subgate_def' idf g = case [i | x@(i,dim) <- g, i==idf]
                       ++ [i | x@(i,dim) <- std, i==idf] of
                     []     -> True
                     (x:xs) -> False

check_dim_m fun idf mat g = if check_dim' idf mat g == (-1)
                            then throwError $ GateWrongDim idf fun
                            else return idf

-- check that no operations are performed with illegal dimensions 
check_dim' :: String -> MExpr -> [Gate] -> Int
check_dim' idf expr@(GateIdf m) gates = 
  let f = find_g "" m gates in case f of Right res -> fromIntegral $ snd res
check_dim' idf expr@(Bra m) gates = length m
check_dim' idf expr@(Ket m) gates = length m
check_dim' idf expr@(Matrix m) gates = fromIntegral . length $ head m
check_dim' idf (MBinary op m1 m2) gates = let m1res = check_dim' idf m1 gates
                                              m2res = check_dim' idf m2 gates
                                          in case op of
  Kronecker -> m1res * m2res
  _         -> if m1res `deq` m2res
               then -1
               else max m1res m2res
               where deq m1res m2mres
                       | m1res == -1 || m2res == -1 = True
                       | m1res == m2res             = False
                       | m1res == 1 || m2res == 1   = False
                       | otherwise                  = True
check_dim' idf expr@IntConst {} gates = 1

check_exp_m fun idf mat = if check_exp' mat
                          then throwError $ ScalarOnlyExp idf fun
                          else return idf

check_exp' (MBinary Exponent m1 m2) = case (m1,m2) of
                                       (e1@IntConst {},e2@IntConst {}) -> False
                                       _                         -> True
check_exp' (MBinary _ m1 m2)        = (check_exp' m1) || (check_exp' m2)
check_exp' _                        = False

-- catch function for matrix definition checks
mat_w :: (String -> String -> MExpr -> StateMonad String) -> 
          String -> String -> MExpr -> StateMonad String
mat_w f fun idf m = (f fun idf m) `catchError` Left

-- attach qubit to circuit 
attach :: String -> String -> State -> StateMonad State
attach circ q_nam state = 
  let fun = "Attach" 
      (gates,qubits,circuits) = state 
      c = find_w find_c fun circ circuits 
      q = find_w find_q fun q_nam qubits
  -- check if circuit was found
  in case c of
      Left e  -> Left e
      Right c ->
        -- check if qubit was found
        case q of
          Left e  -> Left e
          Right q ->
            -- vailididy checks for attaching qubits
            let qubit_checks = map (\f -> attach_w f fun q_nam q c) 
                                   [qubit_len, qubit_assign]
            in case lefts qubit_checks of
              (x:xs) -> Left x
              []     -> return (gates,qubits,attach' circ q circuits)

attach_w :: (String -> String -> Qubit -> Circuit -> StateMonad String) ->
             String -> String -> Qubit -> Circuit -> StateMonad String
attach_w f fun idf q c = (f fun idf q c) `catchError` Left

-- check if qubit and circuit match in number of bits
qubit_len :: String -> String -> Qubit -> Circuit -> StateMonad String
qubit_len fun idf (_,q) (s,_,w,_,_) = 
  if (toInteger $ length q) == w
  then return idf
  else throwError $ BadQubitLength idf s fun

-- check if there is already a qubit assigned
qubit_assign :: String -> String -> Qubit -> Circuit -> StateMonad String
qubit_assign fun idf q (s,_,_,_,cq) =
  if cq == ""
  then return idf
  else throwError $ BadQubitAssign idf s fun

-- helper function for attacking qubit to correct circuit
attach' :: String -> Qubit -> [Circuit] -> [Circuit]
attach' cn qubit c = 
  let (q_name,q_val) = qubit in 
  [ if i == cn
    then (i,s,w,o,q_name)
    else (i,s,w,o,q) | (i,s,w,o,q) <- c ]

-- inserting gate on circuit
insert :: String -> String -> Integer -> [Integer] -> State -> Bool ->
          StateMonad State
insert cidf gidf slot wires state cont = 
  let fun = "Insert" 
      (gates,qubits,circuits)= state
      c = find_w find_c fun cidf circuits in
    case c of 
      Left e   -> Left e
      Right c' ->
        case find_w find_g fun gidf gates of
          Left e   -> Left e
          Right g' ->
            let checks = map (\f -> insert_w f fun cidf gidf slot wires cont c') 
                             [out_of_bounds,occupied,check_cont,adj_wires]
                       ++ [(cor_nw fun cidf gidf wires cont gates)`catchError`Left]
            in case lefts checks of
              (x:xs) -> Left x
              []     -> return (gates,qubits,insert' cidf slot wires circuits)

-- insert help function. Occupies slots/wires on circuit
insert' :: String -> Integer -> [Integer] -> [Circuit] -> [Circuit]
insert' cidf slot wires c =
  [ if i == cidf
    then (i,x,y,(foldl (\o w -> (slot,w):o) objs wires),q)
    else (i,x,y,objs,q) | (i,x,y,objs,q) <- c ]

-- wrapper function to catch insert errors
insert_w f fun cidf gidf slot wires cont c = 
  (f fun cidf gidf slot wires cont c) `catchError` Left

-- check if a request to add to circuit is in bound
out_of_bounds :: String -> String -> String -> Integer -> [Integer] -> 
                 Bool -> Circuit -> StateMonad String
out_of_bounds fun cidf gidf slot wires cont (_,x,y,_,_) =
    let pred = x <= slot || foldl (\acc w -> acc || y <= w) False wires
    in if pred
       then throwError $ OutOfBounds gidf cidf fun
       else return cidf

-- check if a request to add to circuit targets an occupied slot/wire
occupied :: String -> String -> String -> Integer -> [Integer] -> Bool ->
            Circuit -> StateMonad String
occupied fun cidf gidf slot wires cont (_,_,_,o,_) =
  let pred = foldl (\acc (x,y) -> acc ||
               foldl (\acc w -> acc || (x,y) == (slot,w)) False wires)
             False o
  in if pred
     then throwError $ SlotOccupied gidf cidf fun
     else return cidf

-- checks if an inserted gate is applied to the correct number of wires
cor_nw :: String -> String -> String -> [Integer] -> Bool -> [Gate] ->
          StateMonad String
cor_nw fun cidf gidf wires cont gates = 
  let dim = case find_g "" gidf gates of Right res -> snd res
  in if cont
     then if dim /= 2
          then throwError $ GateNotFit gidf cidf fun
          else return cidf
     else if (toInteger . floor . (logBase 2) . fromIntegral) dim 
             /= (toInteger $ length wires)
     then throwError $ GateNotFit gidf cidf fun
     else return cidf

-- checks if an inserted gate is applied to fully adjacent wires
adj_wires :: String -> String -> String -> Integer -> [Integer] -> Bool -> 
             Circuit -> StateMonad String
adj_wires fun cidf gidf slot wires cont c = 
  let pred = wires /= [head wires .. last wires]
  in if pred
     then throwError $ WiresNotAdj gidf cidf fun
     else return cidf

-- check if control bit is correct
check_cont fun cidf gidf slot wires cont c =
  if cont && length wires < 2
  then throwError $ ControlBitsWrong gidf cidf fun
  else return cidf

-- checks if there are circuits in State that have no qubit attached after pass
nattaches :: State -> State 
nattaches state = 
  let (g,q,c) = state
  in case filter (\c' -> let (i,x,y,o,q) = c' in q == "") c of
    [] -> state
    _  -> error $ "there was a circuit without an attached qubit"

