module Controller where
import Commands
import Commutator

type ID = String
data Controller = Controller ID Commutator

getController :: String -> String -> ID ->IO Controller
getController ip port id = do
						 comm <- getCommutator ip port
						 return $ Controller id comm

ready :: Controller -> IO Controller
ready ctrl = do
	comm' <- connectCommutator comm  
	handShake (id ++ "\r\n") comm'
	return $ Controller id comm'
	where Controller id comm  = ctrl

endGame :: Controller -> IO Controller
endGame ctrl = do
	comm' <- disconnectCommutator "EOG\r\n" comm
	return $ Controller id comm'
	where Controller id comm  = ctrl

--------------- Command functions ---------------  
produceCommand cmdName argument controller = do
						 let request = produceRequest id cmdName argument
						 response <- messageExchange request comm
						 return $ parseResponse response cmdName 
						 where 
						 	   Controller id comm = controller

move destination controller = produceCommand "MOV" (CommandArgument destination) controller
						 
rotate angle controller = produceCommand "ROT" (CommandArgument angle) controller

fire controller = produceCommand "FR" Nil controller

scan controller = produceCommand "SC" Nil controller
--------------- ---------------- ---------------
--------------- ---------------- ---------------
