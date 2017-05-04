--Lexical analyser written in Haskell--
import System.Environment
import System.Exit
import System.IO
import Data.List.Split
import Text.Regex
import Text.Regex.Base

data Token =	EOF |
				EOL |
				PLUS |
				MINUS |
				AST |
				FSLASH |
				LT |
				LTEQ |
				EQ |
				DO |
				LPAR |
				RPAR |
				LCBR |
				RCBR |
				PERIOD |
				COMMA |
				COLON |
				SCOLON |
				ASSIGN |
				NEG |
				KCASE |
				KCLASS |
				KDEF |
				KELSE |
				KEXT |
				KIF |
				KMATCH |
				KNAT |
				KNEW |
			 	KNULL |
				KOVER |
				KSUPER |
				KTHIS |
				KVAR |
				KWHILE |
				INTEGER |
				STRING |
				BOOLEAN |
				VARID |
				TYPEID |
				UNDEFINED
				deriving (Show, Eq)

scanFiles :: [String] -> IO()
scanFiles (x:xs) = 	do
		  			content <- readFile x
		  			putStr content
		  			let tokens = parseBegin content
					putStrLn $ show tokens
		  			scanFiles xs
scanFiles  []    =  putStr ""


main =  do
	 	args <- getArgs
	 	scanFiles args

--base parse function
parseBegin :: String -> [(Token, String)]
parseBegin [] =	[(EOF, [])]
parseBegin (x:xs) =	case x of
						--whitespaces
						' ' -> parseBegin xs
						'\t' -> parseBegin xs
						'\n' -> parseBegin xs
						--single-character tokens
						'+' -> [(PLUS, [x])] ++ parseBegin xs
						'-' -> [(MINUS, [x])] ++ parseBegin xs
						'*' -> [(AST, [x])] ++ parseBegin xs
						'/' -> [(FSLASH, [x])] ++ parseBegin xs
						'.' -> [(PERIOD, [x])] ++ parseBegin xs
						',' -> [(COMMA, [x])] ++ parseBegin xs
						'(' -> [(LPAR, [x])] ++ parseBegin xs
						')' -> [(RPAR, [x])] ++ parseBegin xs
						'{' -> [(LCBR, [x])] ++ parseBegin xs
						'}' -> [(RCBR, [x])] ++ parseBegin xs
						';' -> [(SCOLON, [x])] ++ parseBegin xs
						':' -> [(COLON, [x])] ++ parseBegin xs
						'!' -> [(NEG, [x])] ++ parseBegin xs
						'=' -> [(ASSIGN, [x])] ++ parseBegin xs
						--TODO: go to other function on comment and string signatures
						_ -> parseKeyw ([x] ++ xs)

parseKeyw :: String -> [(Token, String)]
parseKeyw [] =	[(EOF, [])]
parseKeyw (x:xs) =	let w = getWord ([x] ++ xs) in
					let xs2 = drop ((length w) - 1) (xs) in
					case w of
						"if" -> [(KIF, w)] ++ parseBegin xs2
						"else" -> [(KELSE, w)] ++ parseBegin xs2
						"while" -> [(KWHILE, w)] ++ parseBegin xs2
						"match" -> [(KMATCH, w)] ++ parseBegin xs2
						"case" -> [(KCASE, w)] ++ parseBegin xs2
						"class" -> [(KCLASS, w)] ++ parseBegin xs2
						"def" -> [(KDEF, w)] ++ parseBegin xs2
						"var" -> [(KVAR, w)] ++ parseBegin xs2
						"extends" -> [(KEXT, w)] ++ parseBegin xs2
						"override" -> [(KOVER, w)] ++ parseBegin xs2
						"native" -> [(KNAT, w)] ++ parseBegin xs2
						"new" -> [(KNEW, w)] ++ parseBegin xs2
						"null" -> [(KNULL, w)] ++ parseBegin xs2
						"super" -> [(KSUPER, w)] ++ parseBegin xs2
						"this" -> [(KTHIS, w)] ++ parseBegin xs2
						"true" -> [(BOOLEAN, w)] ++ parseBegin xs2
						"false" -> [(BOOLEAN, w)] ++ parseBegin xs2
						_ -> parseOther ([x] ++ xs)

parseOther :: String -> [(Token, String)]
parseOther [] = 	[(UNDEFINED, [])]
parseOther (x:xs) =	if (elem x ['A'..'Z']) then
						parseType xs [x]
					else if (elem x ['a'..'z'] || x == '_') then
						parseVar xs [x]
					else if (elem x ['0'..'9']) then
						parseInt xs [x]
					else
						[(UNDEFINED, [x])]
						
parseType :: String -> String -> [(Token, String)]
parseType [] s =		[(TYPEID, s)]
parseType (x:xs) s =	if (elem x ['a'..'z'] || elem x ['A'..'Z'] ||  elem x ['0'..'9']) then
							parseType xs (s ++ [x])
						else
							[(TYPEID, s)] ++ parseBegin xs

parseVar :: String -> String -> [(Token, String)]
parseVar [] s =		[(VARID, s)]
parseVar (x:xs) s =	if (elem x ['a'..'z'] || elem x ['A'..'Z'] ||  elem x ['0'..'9'] || x == '_') then
						parseVar xs (s ++ [x])
					else
						[(VARID, s)] ++ parseBegin xs

parseInt :: String -> String -> [(Token, String)]
parseInt [] s =		[(INTEGER, s)]
parseInt (x:xs) s =	if (elem x ['0'..'9']) then
						parseInt xs (s ++ [x])
					else
						[(INTEGER, s)] ++ parseBegin xs
						

--read until whitespace, single-character token or comment/string signature
getWord :: String -> String
getWord [] =	[]
getWord s =		getWordH s 0

--helper function
getWordH :: String -> Int -> String 
getWordH s i =	let c = s !! i in
				if (i == (length s) || elem c " \n\t+-*/(){}:;!=\\\"") then
					take (i) s
				else
					getWordH s (i + 1)
