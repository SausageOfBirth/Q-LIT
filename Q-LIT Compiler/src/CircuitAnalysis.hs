module CircuitAnalysis(
  filter_insert,
  filter_attach,
  plot_circuit,
  plot_identity,
  sorts,
  findw,
  filter_dim,
  CQGate(..),
) where

import QuantumParser
import Data.List
import QSyntax 

-- tuple to hold values for an Insert statement. Might just want to pass Insert?
type CQGate = (String,Integer,[Integer],String,Bool)

-- take Insert statements out of list of all statements
filter_insert (Right (Seq stmt_list)) = [ i | i@Insert {} <- stmt_list]

-- take Attach statements out of list of all statements
filter_attach (Right (Seq stmt_list)) = [ let Attach s c = a in 
                                          (s,a) | a@Attach {} <- stmt_list]

-- find dimensions for particular circuit c 
filter_dim :: String -> [(String,Stmt)] -> Maybe (Integer,Integer)
filter_dim c [] = Nothing
filter_dim c ((s,Def name (QCirc n m)):xs) = if c == name 
                                             then Just (n,m)
                                             else filter_dim c xs
filter_dim c (_:xs) = filter_dim c xs 

-- build a list of tuples with coordinates for plotted gates
plot_circuit :: [Stmt] -> [CQGate]
plot_circuit is = [ (g,x,y,n,c) | Insert n g x y c <- is ]

plot_identity :: Integer -> Integer -> [CQGate] -> [CQGate]
plot_identity x y cs = 
  let pos_list = concatMap(\x' -> map(\y' -> (x',[y'])) [0..(y-1)]) [0..(x-1)]
      (_,_,_,q,_) = head cs
  in
    foldl (\acc (x',y') -> 
      let pred = find (\(g,n,m,s,c) -> n == x' && y' `isInfixOf` m) cs
        in case pred of 
          Just (g,n,m,s,c) -> if (g,n,m,s,c) `elem` acc 
                              then acc else (g,n,m,s,c):acc
          Nothing        -> ("identity",x',y',q,False) : acc
    ) [] pos_list 

-- helper function for finding gate with greatest wire position
findw :: CQGate -> [CQGate] -> CQGate
findw i [] = i
findw i (c:cs) = let (_,_,m1,_,_) = i
                     (_,_,m2,_,_) = c
                 in if m1 > m2
                 then findw i cs
                 else findw c cs

-- sorts according to wire
sortw :: [CQGate] -> [CQGate]
sortw [] = []
sortw cs = let elem = findw ("",0,[0],"",False) cs in
               (sortw $ delete elem cs) ++ [elem]

-- sorts gate according slots and rearranges their gates according to wires 
sorts :: Integer -> [CQGate] -> [CQGate]
sorts (-1) cs = []
sorts dim cs = sorts (dim-1) cs ++ 
               sortw (filter (\(_,s,_,_,_) -> s == dim) cs) 
