--Haskell implementation of a scanner (Lexical Analyser) for the programming language COOL (01-2016)--
import System.Environment
import System.IO
import Data.Char

--the tokenset--
data Token = EOF |PLUS |MINUS |AST |FSLASH |LT |LTEQ |EQ |DO |LPAR |RPAR |LCBR |RCBR |PERIOD|COMMA |COLON |SCOLON |ASSIGN| NEG |KCASE |KCLASS |KDEF |KELSE |KEXT |KIF |KMATCH |KNAT |KNEW |KNULL |KOVER |KSUPER |KTHIS |KVAR |KWHILE |STRING|BOOLEAN|INTEGER|OBJID|TYPEID|UNKNOWN deriving (Show, Eq)

--START--
main = do 
	 files <- getArgs
	 scanFiles files

--scan cool files--
scanFiles :: [String] -> IO()
scanFiles [] = putStr ""
scanFiles (x:xs) = 	do
				putStr (x ++ ":")
				content <- readFile x
				putStrLn $ show (scan 1 1 content) --SCAN FILE AND PRINT TOKENSTREAM--
				scanFiles xs --scan rest of files (if any)

--valid characters in an id token--
isIdChar :: Char -> Bool
isIdChar x = isLetter x || isDigit x || x == '_'

--comment--
scanComment :: Int -> Int -> String -> [(Token,String,(Int,Int))]
scanComment l c ('\n':r) = scan (l+1) 1 r
scanComment l c (x:xs) = scanComment l (c+1) xs
scanComment l c [] = [(EOF,[],(l,c))]

--multiline comment--
scanMLC :: Int -> Int -> String -> [(Token,String,(Int,Int))]
scanMLC l c ('*':'/':r) = scan l (c+2) r
scanMLC l c ('\n':r) = scanMLC (l+1) 1 r
scanMLC l c (x:xs) = scanMLC l (c+1) xs
scanMLC l c [] = [(EOF,[],(l,c))]

--string--
scanString :: Int -> Int -> Int -> Int -> String -> String -> [(Token,String,(Int,Int))]
scanString sl sc l c b ('"':r) = (STRING, b,(sl,sc)) : scan l (c+1) r 
scanString sl sc l c b (x:xs) = scanString sl sc l (c+1) (b ++ [x]) xs
scanString sl sc l c b [] = [(STRING,b,(sl,sc)),(EOF,[],(l,c))]

--multiline string--
scanMString :: Int -> Int -> Int -> Int -> String -> String ->[(Token,String,(Int,Int))]
scanMString sl sc l c b ('\n':r) = scanMString sl sc (l+1) 1 b r
scanMString sl sc l c b ('\"':'\"':'\"':'\"':'\"':r) = (STRING, (b ++ "\"\""),(sl,sc)) : scan l (c+5) r
scanMString sl sc l c b ('\"':'\"':'\"':'\"':r) = (STRING, (b ++ "\""),(sl,sc)) : scan l (c+4) r
scanMString sl sc l c b ('\"':'\"':'\"':r) = (STRING, b,(sl,sc)) : scan l (c+3) r
scanMString sl sc l c b [] = [(STRING,b,(sl,sc)),(EOF,[],(l,c))]
scanMString sl sc l c b (x:xs) = scanMString sl sc l (c+1) (b ++ [x]) xs

--integer--
scanInt :: Int -> Int -> Int -> Int -> String -> String -> [(Token,String,(Int,Int))]
scanInt sl sc l c b (x:xs) = if isDigit x then scanInt sl sc l (c+1) (b ++ [x]) xs else (INTEGER, b,(sl,sc)) : scan l c ([x] ++ xs)
scanInt sl sc l c b [] = [(INTEGER,b,(sl,sc)),(EOF,[],(l,c))]

--type id--
scanType :: Int -> Int -> Int -> Int -> String -> String -> [(Token,String,(Int,Int))]
scanType sl sc l c b (x:xs) = if isIdChar x then scanType sl sc l (c+1) (b ++ [x]) xs else (TYPEID, b,(sl,sc)) : scan l c ([x] ++ xs)  
scanType sl sc l c b [] = [(TYPEID,b,(sl,sc)),(EOF,[],(l,c))]

--object id--
scanObjId :: Int -> Int -> Int -> Int -> String-> String -> [(Token,String,(Int,Int))]
scanObjId sl sc l c b (x:xs) = if isIdChar x then scanObjId sl sc l (c+1) (b ++ [x]) xs else (OBJID,b,(sl,sc)) : scan l c ([x] ++ xs)
scanObjId sl sc l c b [] = [(OBJID,b,(sl,sc)),(EOF,[],(l,c))]

--intermediate check called after a keyword match--
checkId ::  Int -> Int -> String -> String -> Token -> [(Token,String,(Int,Int))]
checkId l c [] match token = (token,[],(l,c)) : scan l c []
checkId l c r match token = let char = (r !! 0 ) in if (isIdChar char) then scanObjId l c l (c+(length match)) match r else (token,[],(l,c)) : scan l (c+(length match)) r  

--main function, args: line, column, input
scan :: Int -> Int -> String -> [(Token,String,(Int,Int))]
scan l c ('c':'l':'a':'s':'s':r) = checkId l c r "class" KCLASS
scan l c ('e':'x':'t':'e':'n':'d':'s':r) = checkId l c r "extends" KEXT
scan l c ('d':'e':'f':r) = checkId l c r "def" KDEF
scan l c ('o':'v':'e':'r':'r':'i':'d':'e':r) = checkId l c r "override" KOVER
scan l c ('m':'a':'t':'c':'h':r) = checkId l c r "match" KMATCH
scan l c ('c':'a':'s':'e':r) =  checkId l c r "case" KCASE
scan l c ('v':'a':'r':r) = checkId l c r "var" KVAR
scan l c ('i':'f':r) = checkId l c r "if" KIF
scan l c ('e':'l':'s':'e':r) = checkId l c r "else" KELSE
scan l c ('w':'h':'i':'l':'e':r) = checkId l c r "while" KWHILE
scan l c ('n':'e':'w':r) = checkId l c r "new" KNEW
scan l c ('n':'a':'t':'i':'v':'e':r) = checkId l c r "native" KNAT
scan l c ('s':'u':'p':'e':'r':r) = checkId l c r "super" KSUPER
scan l c ('t':'h':'i':'s':r) = checkId l c r "this" KTHIS
scan l c ('n':'u':'l':'l':r) = checkId l c r "null" KNULL
scan l c ('d':'o':r) = checkId l c r "do" DO
scan l c ('.':r) = (PERIOD,[],(l,c)) : scan l (c+1) r
scan l c (',':r) = (COMMA,[],(l,c)) : scan l (c+1) r
scan l c ('+':r) = (PLUS,[],(l,c)) : scan l (c+1) r
scan l c ('-':r) = (MINUS,[],(l,c)) : scan l (c+1) r
scan l c ('*':r) = (AST,[],(l,c)) : scan l (c+1) r
scan l c ('/':'*':r) = scanMLC l (c+2) r
scan l c ('/':'/':r) = scanComment l (c+2) r
scan l c ('/':r) = (FSLASH,[],(l,c)) : scan l (c+1) r
scan l c ('<':'=':r) = (Main.LTEQ,[],(l,c)) : scan l (c+2) r
scan l c ('<':r) = (Main.LT,[],(l,c)) : scan l (c+1) r
scan l c ('=':'=':r) = (Main.EQ,[],(l,c)) : scan l (c+2) r
scan l c ('=':r) = (ASSIGN,[],(l,c)) : scan l (c+1) r
scan l c ('(':r) = (LPAR,[],(l,c)) : scan l (c+1) r
scan l c (')':r) = (RPAR,[],(l,c)) : scan l (c+1) r
scan l c ('{':r) = (LCBR,[],(l,c)) : scan l (c+1) r
scan l c ('}':r) = (RCBR,[],(l,c)) : scan l (c+1) r
scan l c ('\n':r) = scan (l+1) 1 r
scan l c ('\t':r) = scan l (c+4) r 
scan l c (' ':r) = scan l (c+1) r  
scan l c (';':r) = (SCOLON,[],(l,c)) : scan l (c+1) r
scan l c (':':r) = (COLON,[],(l,c)) : scan l (c+1) r
scan l c ('\"':'\"':'\"':r) = scanMString l c l (c+3) [] r
scan l c ('\"':r) = scanString l c l (c+1) [] r 
scan l c (x:xs) = if isDigit x then scanInt l c l (c+1) [x] xs else if isUpper x then scanType l c l (c+1) [x] xs  else if isIdChar x then scanObjId l c l (c+1) [x] xs else (UNKNOWN,[],(l,c)) : scan l (c+1) xs
scan l c [] = [(EOF,[],(l,c))]
