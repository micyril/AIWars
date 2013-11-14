module Commands where
import Protocol
import Tools

type State = Int


------------------- command answer -------------------
--------- for scan -----------
type ObjectType = String
type Destination = Float
type Angle = Float

data WorldObject = WorldObject ObjectType Destination Angle
instance Show WorldObject where
	show (WorldObject objecttype destination angle) = "{ " ++ objecttype ++ " | destination: " ++ (show destination) ++ " | angle: " ++ (show angle) ++ " }" 
--------- -------- -----------


data CommandAnswer = SimpleAnswer State | ScanAnswer State [WorldObject]
instance Show CommandAnswer where
	show (SimpleAnswer state) = show state
	show (ScanAnswer state arr) = (show state) ++ " " ++ (show arr) 

-------------------------------------------------------


data CommandArgument a = Nil | CommandArgument (a)


produceRequest :: String -> String -> CommandArgument Float -> String

produceRequest id cmd Nil = getLineByFields id cmd ""
produceRequest id cmd (CommandArgument arg) = getLineByFields  id cmd  $ floatToFixedDecimals arg 4

checkState :: String -> Int

checkState "ACK" = 1
checkState "NAK" = 0
checkState "EOG" = -1
checkState _ = 0

parseScanResponse :: [String] -> [WorldObject]
parseScanResponse words = map3 (\t d a -> WorldObject (t) (strToFloat d) (strToFloat a)) words


parseResponse :: String -> String -> CommandAnswer

parseResponse [] cmd = SimpleAnswer (-1)

parseResponse response "SC" = ScanAnswer (checkState head) (parseScanResponse $ words args) 
	where Message head _ args = getMessageByLine response

parseResponse response cmd = SimpleAnswer (checkState head)
	where Message head _ _ = getMessageByLine response


--main = do
--	 putStrLn $ show $ (parseResponse "ACK RET DOG 1.0 1.0 CAT 2.1 2.1" "MOV") 
