module Protocol where
import Data.String
import Tools

type Head = String
type RawCommand = String
type ArgsLine = String

data Message = Message (Head) (RawCommand) (ArgsLine)
instance Show (Message) where show (Message head rawcommand argsline) = head ++ " " ++ rawcommand ++ " " ++ argsline

endLine :: String
endLine = "\r\n"


getMessageByLine :: String -> Message
getMessageByLine strline = Message head rawcommand argsline
	where 
		  [head,rawcommand,argsline] = parseWords $ split clearline ' ' 2
		  clearline = erase strline endLine

parseWords :: [String] -> [String]
parseWords words = parseWords' (reverse words) ["","",""]

parseWords' :: [String] -> [String] -> [String]
parseWords' [] list = list
parseWords' (word:words) list = parseWords' words (word : (init list)) 


getLineByMessage :: Message -> String
getLineByMessage message = show message ++ endLine

getLineByFields :: Head -> RawCommand -> ArgsLine -> String
getLineByFields head rawcommand argsline = head ++ " " ++ rawcommand ++ " " ++ argsline ++ endLine


getSimpleMessageLine :: String -> String
getSimpleMessageLine str = str ++ endLine

--main = do
--	putStrLn head
--	where Message head _ _  = getMessageByLine "ACK"