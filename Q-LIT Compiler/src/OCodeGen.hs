module OCodeGen( hadamardG,
  main_codegen,
  notG,
  cnotG,
  identityG,
  headGen,
  tailGen,
  makeCirc,
  customGate,
  qubitDec,
  qubitProd,
  gateDec
) where

import CircuitAnalysis
import DefAnalysis
import QuantumParser
import Data.List
import QSyntax

main_codegen prog = 
  let mat_defs = filter_mat . filter_def $ prog
      bit_defs = filter_bit . filter_def $ prog
      add_defs = map (\stmt -> let Insert s _ _ _ _ = stmt in (s,stmt)) 
                 $ filter_insert prog
      att_defs = filter_attach prog
  in concatMap (\x -> dis_code prog x) [mat_defs,bit_defs,att_defs,add_defs]  

-- call code gen functions for each data type 
dis_code prog con = 
  if length con > 0
  then let (s,stmt) = (head con) in case stmt of 
    Def _ qexpr -> case qexpr of
                     QBit m -> qubitDec $ map (\(s',stmt') ->
                               let Def _ q = stmt' in
                                 let QBit mexpr = q in 
                                   (s',mexpr)) con
                     QGate m -> gateDec $ map (\(s',stmt') -> 
                                let Def _ q = stmt' in 
                                  let QGate mexpr = q in
                                    (s',control mexpr)) con
    Insert _ _ _ _ _ ->
      let ins_st = filter_insert prog in
      concatMap (\ins ->
      let (Insert c _ _ _ _) = head ins
          dim = filter_dim c (filter_dcirc . filter_def $ prog) in case dim of
        Just (slots, wires) -> 
          concat . makeCirc slots $ sorts slots $ plot_identity slots wires 
          $ plot_circuit $ ins
        Nothing -> error $ "no circuit c, so no dimensions found" 
      ) (reverse $ segrIns (uniqIns [] (plot_circuit ins_st)) ins_st)
    Attach _ _ -> concatMap 
                    (\(s,(Attach s' c')) -> "Qubit* "++s'++" = "++c'++";\n ") 
                  con
  else ""

uniqIns :: [String] -> [CQGate] -> [String]
uniqIns cs [] = cs
uniqIns cs (i:ins)
  | elem c cs = uniqIns cs ins
  | otherwise = uniqIns (c:cs) ins
  where (_,_,_,c,_) = i

segrIns uniqL ins = map (\u -> filter(\(Insert c _ _ _ _) -> c == u) ins) uniqL

customGateAssign :: String -> Integer -> Integer -> Integer -> String -> [[CExpr]] -> (Integer,String)
customGateAssign name _ _ n str [] = (n, str)
customGateAssign name col row n str ([c]:ys) = 
  case c of
    Complex r i ->
      if r /= 0.0 || i /= 0.0 
      then if i >= 0 
           then customGateAssign name 0 (row+1) (n+1) 
                (str++name++"_data["++show n++"] = genTuple("++show row++","
                ++show col++","++show r++"+"++show i++" * I);\n ") ys
           else customGateAssign name 0 (row+1) (n+1) 
                (str++name++"_data["++show n++"] = genTuple("++show row++","
                ++show col++","++show r++show i++" * I);\n ") ys
      else customGateAssign name 0 (row+1) n str ys
    Constant c -> customGateAssign name 0 (row+1) (n+1) (pickCon c) ys
      where pickCon c
             | c == "e"  = str++name++"_data["++show n++"] = genTuple("++
                          show row++","++show col++",M_E + 0.0 * I);\n"
             | c == "pi" = str++name++"_data["++show n++"] = genTuple("++
                          show row++","++show col++",M_PI + 0.0 * I);\n"
customGateAssign name col row n str ((c:xs):ys) = 
  case c of
    Complex r i ->
      if r /= 0.0 || i /= 0.0 
      then if i >= 0 
           then customGateAssign name (col+1) row (n+1) 
                (str++name++"_data["++show n++"] = genTuple("++show row++","
                ++show col++","++show r++"+"++show i++" * I);\n ") (xs:ys)
           else customGateAssign name (col+1) row (n+1) 
                (str++name++"_data["++show n++"] = genTuple("++show row++","
                ++show col++","++show r++show i++" * I);\n ") (xs:ys)
      else customGateAssign name (col+1) row n str (xs:ys)
    Constant c -> customGateAssign name (col+1) row (n+1) (pickCon c) ys
      where pickCon c
             | c == "e"  = str++name++"_data["++show n++"] = genTuple("++
                          show row++","++show col++",M_E + 0.0 * I);\n"
             | c == "pi" = str++name++"_data["++show n++"] = genTuple("++
                          show row++","++show col++",M_PI + 0.0 * I);\n"

customGate :: String  -> [[CExpr]] -> String
customGate name n = 
  let (len, strn) = customGateAssign name 0 0 0 "" n in 
    "Matrix* "++name++" = (Matrix*)malloc(sizeof(Matrix));\n"++name++"->row = "++show (length n)++";\n"++name++"->col = "++show (length n)++";\n "++"tuple** "++name++"_data = (tuple**)malloc(sizeof(tuple*)*"++show len++");\n"++strn++name++"->data = "++name++"_data;\n"++name++"->valsize = "++show len++";\n "

-- generates a tensor product between 
makeCircSlot :: [CQGate] -> Integer -> String -> String
makeCircSlot [] it str = str
makeCircSlot [x] it str =
  let (x',n,m,qubit,c) = x in
    if x' == "measurement"
    then "measurement(&"++qubit++","++show (m!!0)++");\n "
    else if c
    then str++"slot_"++qubit++show n++"["++show it++"] = controlGen("++show(head m)++","++
        show(last m)++","++x'++");\n "++"calcSlot(&"++qubit++", slot_"++qubit++show n++");\n"
    else str++"slot_"++qubit++show n++"["++show it++"] = "++x'++";\n "++
         "calcSlot(&"++qubit++", slot_"++qubit++show n++");\n "
makeCircSlot (x:xs) it str = 
  let (x',n,m,qubit,c) = x in 
    if x' == "measurement" 
    then "measurement(&"++qubit++","++show (m!!0)++");\n "
    else if c
    then makeCircSlot xs (it+1) (str++"slot_"++qubit++show n++"["++show it++"] = "++
         "controlGen("++show(head m)++","++show(last m)++","++x'++");\n")
    else makeCircSlot xs(it+1)(str++"slot_"++qubit++show n++"["++show it++"] = "++x'++";\n")

-- main fuction to generate the major circuit matrix 
makeCirc :: Integer -> [CQGate] -> [String]
makeCirc dim cs = 
  let slots = map (\d -> filter(\(_,s,_,_,_) -> s == d ) cs) [0..(dim-1)]
      (_,_,_,circ,_) = head $ head  slots
  in (map (\(x:xs) -> 
    let (_,n,_,qubit,_) = x
    in makeCircSlot (x:xs) 0 ("Matrix** slot_"++qubit++show n++" = (Matrix**)malloc(sizeof(Matrix*)*"++(show (length (x:xs)))++");\n")) slots)
    ++["outputQ(\"qubitoutput_"++circ++"\","++circ++");\n"]

-- get index for the 1 in a qubit 
getQubitVal :: String -> Int -> Int -> Int
getQubitVal content index val = 
  if index == (length content)
    then val
    else if (content !! index) == '1'
      then getQubitVal content (index+1) (val+(2^index))
      else getQubitVal content (index+1) val

-- initialize qubit 
qubitProd :: String -> String
qubitProd content = "qubitInit("++show (getQubitVal (reverse content) 0 0)++","++show (length content)++")"

-- main function to generate a qubit from definition
qubitDec :: [(String, MExpr)] -> String
qubitDec [] = ""
qubitDec ((name,Bra content):xs) = "Qubit* "++name++" = "++qubitProd content++";\n "++(qubitDec xs)
qubitDec ((name,Ket content):xs) = "Qubit* "++name++" = "++qubitProd content++";\n "++(qubitDec xs)

-- main function to generate a qubit from definition
gateDecIn :: MExprA -> String -> Integer -> String
gateDecIn (Atom aType (IntConst (Complex r i))) abc matName = 
  (show r)++" + "++(show i)++" * I"
gateDecIn (Atom aType (IntConst (Constant c))) abc matName 
  | c == "e"  = "M_E + 0.0 * I"
  | c == "pi" = "M_PI + 0.0 * I"
gateDecIn (Atom aType (GateIdf content)) abc matName = content
gateDecIn (Atom aType (Matrix content)) abc matName = "_"++abc++(show matName)
gateDecIn (Atom aType (Ket content)) abc matName = qubitProd content
gateDecIn (Atom aType (Bra content)) abc matName = qubitProd content
gateDecIn (Comp (func, resType) m1 m2) abc matName = 
  func++"("++(gateDecIn m1 "b" (matName+1))++","++(gateDecIn m2 "a" (matName+1))++")"

gateMatDec :: MExprA -> String -> Integer -> String
gateMatDec (Atom aType (Ket content)) abc _ = ""
gateMatDec (Atom aType (Bra content)) abc _ = ""
gateMatDec (Atom aType (IntConst content)) abc _ = ""
gateMatDec (Atom aType (GateIdf content)) abc matName = ""
gateMatDec (Atom aType (Matrix content)) abc matName = 
  customGate ("_"++abc++(show matName)) content
gateMatDec (Comp (func, resType) m1 m2) abc matName = 
  (gateMatDec m1 "b" (matName+1))++(gateMatDec m2 "a" (matName+1))++";\n "

-- gives c equivalent struct names for quantum constructs as denoted in haskell
handleType :: String -> String
handleType n
  | n == "bra" = "Qubit* "
  | n == "ket" = "Qubit* "
  | n == "scalar" = "double complex "
  | n == "matrix" = "Matrix* "

-- main function to generate a matrix from definition
gateDec :: [(String, MExprA)] -> String
gateDec [] = ""
gateDec ((name, (Atom "matrix" (Matrix content))):xs) = 
  (customGate name content)++(gateDec xs)
gateDec ((name, (Atom _ (GateIdf content))):xs) = 
  content++(gateDec xs)
gateDec ((name, (Comp (func, resType) m1 m2)):xs) = 
  (gateMatDec (Comp (func, resType) m1 m2) "a" 0)++
  (handleType resType)++name++" = "++
  (gateDecIn (Comp (func, resType) m1 m2) "a" 0)++";\n "
  ++(gateDec xs)

hadamardG = "Matrix* hadamard = hadamardG();\n "
notG = "Matrix* not = notG();\n "
cnotG = "Matrix* cnot = cnotG();\n "
identityG = "Matrix* identity = identityG();\n "

headGen n = "#include \"QLIT_lib.h\"\n#include <sys/time.h>\n\n"++
            "#define M_PI 3.14159265358979323846\n#define M_E  "++
            "2.71828182845904523536\n\nint main(){\n #ifdef RUNTIME\n  struct timeval t1, t2;\n  double elapsedTime;\n  gettimeofday(&t1, NULL);\n #endif\n "
            ++hadamardG++notG++cnotG++identityG++n

tailGen n = n++"#ifdef RUNTIME\n  gettimeofday(&t2, NULL);\n  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;\n  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;\n  printf(\"delta time for the total run %f ms.\\n\", elapsedTime);\n #endif\n return 0;\n}"
