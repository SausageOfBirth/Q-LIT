import SyntaxAnalysis
import QuantumParser
import CircuitAnalysis
import DefAnalysis
import NCodeGen
import CustomError
import QSyntax

import Control.Monad
import System.IO
import System.Environment
import Data.List.Split

main = do
  (program:args) <- getArgs
  contents <- readFile program
  case syntax_check contents of
    Left e  -> print (e :: CustomError)
    Right e -> 
      do f <- openFile ((head $ splitOn "." program)++".c") WriteMode
         hPutStr f (tailGen $ headGen $ main_codegen $ parseExpr contents) 
         hClose f
         putStrLn "Haskell Compilation successful"

syntax_check prog = do
  let init = initialize (parseExpr prog)
  case init of
    Right init_state -> pass init_state
    Left e -> Left $ ParseError (show e)
