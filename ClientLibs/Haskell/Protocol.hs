module Protocol where
import Data.String
import Tools

type Head = String
type RawCommand = String
type ArgsLine = String

data Message = Message (Head) (RawCommand) (ArgsLine)
instance Show (Message) where show (Message head rawcommand argsline) = head ++ " " ++ rawcommand ++ " " ++ argsline


getMessageByLine :: String -> Message
getMessageByLine strline = Message head rawcommand argsline
	where 
		  [head,rawcommand,argsline] = parseWords $ split strline ' ' 2

parseWords :: [String] -> [String]
parseWords words = parseWords' (reverse words) ["","",""]

parseWords' :: [String] -> [String] -> [String]
parseWords' [] list = list
parseWords' (word:words) list = parseWords' words (word : (init list)) 


getLineByMessage :: Message -> String
getLineByMessage message = show message

getLineByFields :: Head -> RawCommand -> ArgsLine -> String
getLineByFields head rawcommand argsline = head ++ " " ++ rawcommand ++ " " ++ argsline


--main = do
--	putStrLn head
--	where Message head _ _  = getMessageByLine "ACK"