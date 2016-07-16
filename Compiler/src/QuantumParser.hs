module QuantumParser (
  parseExpr
) where

-- AST is defined in exported from the QSyntax module
import QSyntax

import Text.Parsec
import Text.ParserCombinators.Parsec.Number
import Text.Parsec.String (Parser)
import Text.Parsec.Language (emptyDef)

import qualified Text.Parsec.Expr as Ex
import qualified Text.Parsec.Token as Tok

import Data.Functor.Identity
import Control.Monad (liftM)

langDef :: Tok.LanguageDef ()
langDef = Tok.LanguageDef
  { Tok.commentStart = "{-"
  , Tok.commentEnd = "-}"
  , Tok.commentLine = "--"
  , Tok.nestedComments = True
  , Tok.identStart = letter
  , Tok.identLetter = alphaNum <|> oneOf "_'"
  , Tok.opStart = oneOf ":!#$%&*+./<=>?@\\^|-~"
  , Tok.opLetter = oneOf ":!#$%&*+./<=>?@\\^|-~"
  , Tok.reservedNames = ["def","new","qcirc","qbit","qgate","add","attach","i","e","pi"]
  , Tok.reservedOpNames = ["+","-","*","#","=",".","^"]
  , Tok.caseSensitive = True
  }

-- create lexer from definition
lexer = Tok.makeTokenParser langDef

identifier = Tok.identifier lexer -- parses an identifier
reserved   = Tok.reserved   lexer -- parses a reserved name
reservedOp = Tok.reservedOp lexer -- parses an operator
parens     = Tok.parens     lexer -- parses surrounding paranthesis
integer    = Tok.integer    lexer -- parses an integer
semi       = Tok.semi       lexer -- parses a semicolon
whiteSpace = Tok.whiteSpace lexer -- parses a whitespace
float      = Tok.float      lexer -- parses a float

-- parse the expressions
expr :: Parser Stmt
expr = Ex.buildExpressionParser [[]] sequenceofStmt

-- builds up a list of statements (needs at least one)
sequenceofStmt =
  do list <- many1 termSemi
     return $ Seq list
  where
    termSemi =
      do t <- term
         semi
         return t

-- function to choose between statements.
term :: Parser Stmt
term =
      defStmt
  <|> try attachqubit
  <|> insertStmt
  <|> parens expr

defStmt =
  do reserved "def"
     var <- identifier
     reservedOp "="
     qexpr <- qExpression
     return $ Def var qexpr

attachqubit =
  do var <- identifier
     reservedOp ".attach("
     qubit <- identifier
     reservedOp ")"
     return $ Attach var qubit

insertStmt =
  do var <- identifier
     reservedOp ".add("
     slot <- integer
     reservedOp ","
     wires <- insertList
     reservedOp ","
     gate <- identifier
     reservedOp ")"
     return $ case wires of
                (-1,w) -> Insert var gate slot w False
                (c,w) -> Insert var gate slot [c..head w] True

insertList =
   try (reservedOp "[" >> (do cc <- integer
                              reservedOp ";"
                              list <- (sepBy integer (char ','))
                              reservedOp "]"
                              return (cc,list)
                           )
       )
   <|> (reservedOp "[" >> (do list <- (sepBy integer (char ','))
                              reservedOp "]"
                              return (-1,list)
                          )
      )

qExpression =
      qcirc
  <|> qcons

qcons =
      (reserved "qbit" >> (do{expr <- mExpression; return $ QBit expr }))
  <|> (reserved "qgate" >> (do{expr <- mExpression; return $ QGate expr }))

qcirc =
  do reserved "qcirc"
     reservedOp "("
     slots <- integer
     reservedOp ","
     wires <- integer
     reservedOp ")"
     return $ QCirc slots wires

-- set up matrix arithmetics
mExpression :: Parser MExpr
mExpression = Ex.buildExpressionParser mOperators mTerm

-- contains a list of operators with necessary info
mOperators = [ [Ex.Infix (reservedOp "^" >>
                return (MBinary Exponent  )) Ex.AssocLeft]
             , [Ex.Infix (reservedOp "*" >>
                return (MBinary Multiply  )) Ex.AssocLeft,
                Ex.Infix (reservedOp "/" >>
                return (MBinary Divide    )) Ex.AssocLeft]
             , [Ex.Infix (reservedOp "+" >>
                return (MBinary Add       )) Ex.AssocLeft,
                Ex.Infix (reservedOp "-" >>
                return (MBinary Subtract  )) Ex.AssocLeft]
             , [Ex.Infix (reservedOp "#" >>
                return (MBinary Kronecker )) Ex.AssocLeft]

             ]

cTerm =
      try cimag
  <|> try ( do ni <- sign
               i <- imag
               return $ Complex (0 :: Double) (ni i)
          )
  <|> try ( do cons <- many1 letter
               if cons /= "e" && cons /= "pi"
               then unexpected $ cons
               else return $ Constant cons
          )
  <|> ( do nr <- sign
           r <- generic_num
           return $ Complex (nr r) (0 :: Double)
      )

cimag =
  do nr <- sign
     r <- generic_num
     ni <- sign
     i <- imag
     return $ Complex (nr r) (ni i)

imag = try ( do char 'i'
                return $ (1 :: Double)
           )
   <|> try ( do f <- generic_num
                char 'i'
                return f
           )

generic_num = try float
          <|> try ( do f <- integer
                       return $ (fromIntegral f :: Double)
                  )

mTerm =
     parens mExpression
 <|> try (fmap GateIdf identifier)
 <|> (do { reservedOp "|"; ket <- many1 $ satisfy (\x -> x == '0' || x == '1'); reservedOp ">"; return $ Ket ket })
 <|> (do { reservedOp "<"; bra <- many1 $ satisfy (\x -> x == '0' || x == '1'); reservedOp "|"; return $ Bra bra })
 <|> do outer <- mMatrix 
        return $ Matrix outer
 <|> fmap IntConst cTerm

mMatrix =
  do skipMany space
     char '['
     let column = sepBy cTerm (char ',')
     outer <- (sepBy column semi)
     char ']'
     skipMany space
     return outer

-- gets rid of leading whitespaces and then parses the input
contents :: Parser a -> Parser a
contents p = do
  whiteSpace
  r <- p
  eof
  return r

-- final function
parseExpr :: String -> Either ParseError Stmt
parseExpr = parse (contents expr) "<stdin>"
