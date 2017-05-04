--Lexical analyser written in Haskell--
import System.Environment
import System.Exit
import System.IO

data Token =	EOF |
				EOL |
				PLUS |
				MINUS |
				AST |
				FSLASH |
				LESSTHAN |
				LTEQ |
				EQUALS |
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
		  			let tokens = parseBegin content 1 1
					putStrLn $ show tokens
		  			scanFiles xs
scanFiles  []    =  putStr ""


main =  do
	 	args <- getArgs
	 	scanFiles args

--base parse function
parseBegin :: String -> Int -> Int -> [(Token, String, (Int, Int))]
parseBegin [] li col =	[(EOF, [], (li, col))]
parseBegin (x:xs) li col =	case x of
							--whitespaces
							' ' -> parseBegin xs li (col + 1)
							'\t' -> parseBegin xs li (col + 4)
							'\n' -> parseBegin xs (li + 1) 1
							--single-character tokens
							'+' -> [(PLUS, [x], (li, col))] ++ parseBegin xs li (col + 1)
							'-' -> [(MINUS, [x], (li, col))] ++ parseBegin xs li (col + 1)
							'*' -> [(AST, [x], (li, col))] ++ parseBegin xs li (col + 1)
							--handle comments and comment blocks
							'/' -> parseComment xs li (col + 1)
							'.' -> [(PERIOD, [x], (li, col))] ++ parseBegin xs li (col + 1)
							',' -> [(COMMA, [x], (li, col))] ++ parseBegin xs li (col + 1)
							'(' -> [(LPAR, [x], (li, col))] ++ parseBegin xs li (col + 1)
							')' -> [(RPAR, [x], (li, col))] ++ parseBegin xs li (col + 1)
							'{' -> [(LCBR, [x], (li, col))] ++ parseBegin xs li (col + 1)
							'}' -> [(RCBR, [x], (li, col))] ++ parseBegin xs li (col + 1)
							';' -> [(SCOLON, [x], (li, col))] ++ parseBegin xs li (col + 1)
							':' -> [(COLON, [x], (li, col))] ++ parseBegin xs li (col + 1)
							'!' -> [(NEG, [x], (li, col))] ++ parseBegin xs li (col + 1)
							--check if =, == or =>
							'=' -> parseEq xs li (col + 1)
							--check if < or <=
							'<' -> parseLt xs li (col + 1)
							'\"'	-> parseStr xs li (col + 1)
							--everything else
							_ -> parseKeyw ([x] ++ xs) li col

parseEq :: String -> Int -> Int -> [(Token, String, (Int, Int))]
parseEq [] li col = 		[(EOF, [], (li, col))]
parseEq ('=':xs) li col =	[(EQUALS, "==", (li, col - 1))] ++ parseBegin xs li (col + 1)
parseEq ('>':xs) li col =	[(DO, "=>", (li, col - 1))] ++ parseBegin xs li (col + 1)
parseEq xs li col =			[(ASSIGN, "=", (li, col - 1))] ++ parseBegin xs li col

parseLt :: String -> Int -> Int -> [(Token, String, (Int, Int))]
parseLt [] li col = 		[(EOF, [], (li, col))]
parseLt ('=':xs) li col =	[(LTEQ, "<=", (li, col))] ++ parseBegin xs li (col + 1)
parseLt xs li col =			[(LESSTHAN, "<", (li, col))] ++ parseBegin xs li col

parseKeyw :: String -> Int -> Int -> [(Token, String, (Int, Int))]
parseKeyw [] li col =		[(EOF, [], (li, col))]
parseKeyw (x:xs) li col =	let w = getWord ([x] ++ xs) in
					let xs2 = drop ((length w)) (xs) in
					case w of
						"if" -> [(KIF, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"else" -> [(KELSE, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"while" -> [(KWHILE, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"match" -> [(KMATCH, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"case" -> [(KCASE, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"class" -> [(KCLASS, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"def" -> [(KDEF, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"var" -> [(KVAR, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"extends" -> [(KEXT, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"override" -> [(KOVER, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"native" -> [(KNAT, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"new" -> [(KNEW, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"null" -> [(KNULL, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"super" -> [(KSUPER, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"this" -> [(KTHIS, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"true" -> [(BOOLEAN, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						"false" -> [(BOOLEAN, w, (li, col))] ++ parseBegin xs2 li (col + (length w) + 1)
						_ -> parseOther ([x] ++ xs) li col

parseOther :: String -> Int -> Int -> [(Token, String, (Int, Int))]
parseOther [] li col = 		[(UNDEFINED, [], (li, col))]
parseOther (x:xs) li col =	if (elem x ['A'..'Z']) then
								parseType xs [x] li col
							else if (elem x ['a'..'z'] || x == '_') then
								parseVar xs [x] li col
							else if (elem x ['0'..'9']) then
								parseInt xs [x] li col
							else
								[(UNDEFINED, [x], (li, col))]
						
parseType :: String -> String -> Int -> Int  -> [(Token, String, (Int, Int))]
parseType [] s li col =			[(TYPEID, s, (li, col))]
parseType (x:xs) s li col =		if (elem x ['a'..'z'] || elem x ['A'..'Z'] ||  elem x ['0'..'9']) then
									parseType xs (s ++ [x]) li col
								else
									[(TYPEID, s, (li, col))] ++ parseBegin ([x] ++ xs) li (col + (length s))

parseVar :: String -> String -> Int -> Int -> [(Token, String, (Int, Int))]
parseVar [] s li col =		[(VARID, s, (li, col))]
parseVar (x:xs) s li col =	if (elem x ['a'..'z'] || elem x ['A'..'Z'] ||  elem x ['0'..'9'] || x == '_') then
								parseVar xs (s ++ [x]) li col
							else
								[(VARID, s, (li, col))] ++ parseBegin ([x] ++ xs) li (col + (length s))

parseInt :: String -> String -> Int -> Int -> [(Token, String, (Int, Int))]
parseInt [] s li col =		[(INTEGER, s, (li, col))]
parseInt (x:xs) s li col =	if (elem x ['0'..'9']) then
								parseInt xs (s ++ [x]) li col
							else
								[(INTEGER, s, (li, col))] ++ parseBegin ([x] ++ xs) li (col + (length s))
						

--read until whitespace, single-character token or comment/string signature
getWord :: String -> String
getWord [] =	[]
getWord s =		getWordH s 0

--helper function
getWordH :: String -> Int -> String 
getWordH s i =	let c = s !! i in
				if (i == (length s) || elem c " \n\t+-*/(){}:;!=<\\\"") then
					take (i) s
				else
					getWordH s (i + 1)

parseComment :: String -> Int -> Int -> [(Token, String, (Int, Int))]
parseComment [] li col =		[(EOF, [], (li, col))]
parseComment ('*':xs) li col =	parseCBlock xs li (col + 1)
parseComment ('/':xs) li col =	parseCLine xs li (col + 1)
parseComment xs li col = [(FSLASH, "/", (li, col - 1))] ++ parseBegin xs li col

parseCBlock :: String -> Int -> Int -> [(Token, String, (Int, Int))]
parseCBlock [] li col =			[(EOF, [], (li, col))]
parseCBlock (x:xs) li col = 	if (x == '*' && length xs > 0 && (head xs) == '/') then
									parseBegin (tail xs) li (col + 1)
								else if (x == '\n') then
									parseCBlock xs (li + 1 ) 1
								else
									parseCBlock xs li (col + 1)
							

parseCLine :: String -> Int -> Int -> [(Token, String, (Int, Int))]
parseCLine [] li col =			[(EOF, [], (li, col))]
parseCLine ('\n':xs) li col =	parseBegin xs (li + 1) 1
parseCLine xs li col =			parseCLine (tail xs) li (col + 1)

parseStr :: String -> Int -> Int -> [(Token, String, (Int, Int))]
parseStr [] li col = 		[(EOF, [], (li, col))]
parseStr (x:xs) li col =	if (x == '\"' && length xs > 1 && (head xs) == '\"') then
								parseCStr (drop 1 xs) [] li (col + 3) 0
							else
								parseNStr ([x] ++ xs) [] li (col + 1)

parseNStr :: String -> String -> Int -> Int -> [(Token, String, (Int, Int))]
parseNStr [] s li col = 		[(EOF, [], (li, col))]
parseNStr ('\"':xs) s li col =	if ((length s) > 0 && (last s) /= '\\') then
									[(STRING, s, (li, col - (length s) - 1))] ++ parseBegin xs li (col + 1)
								else
									parseNStr xs (s ++ [head xs]) li (col + 1)
parseNStr ('\\':xs) s li col =	if ((length s) > 1 && elem (head xs) "0\n\r\b\f ") then
									[(UNDEFINED, s, (li, col))]
								else
									parseNStr xs (s ++ ['\\']) li (col + 1)
parseNStr (x:xs) s li col =		parseNStr xs (s ++ [x]) li (col + 1)

parseCStr :: String -> String -> Int -> Int -> Int -> [(Token, String, (Int, Int))]
parseCStr [] s li col d = 			[(EOF, [],(li, col))]
parseCStr ('\"':xs) s li col d =	if ((length xs) > 1 && (last s) /= '\\' && (xs !! 0) == '\"' && (xs !! 1) == '\"') then
										[(STRING, s, (li, col - (length s) - 3))] ++ parseBegin (drop 2 xs) (li + d) (col + 3)
									else
										parseCStr xs (s ++ ['\"']) li (col + 1) d
parseCStr ('\n':xs) s li col d =	parseCStr xs (s ++ ['\n']) li (col + 1) (d + 1)
parseCStr (x:xs) s li col d =		parseCStr xs (s ++ [x]) li (col + 1) d
