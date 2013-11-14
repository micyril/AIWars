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

move destination controller = do
						 let request = produceRequest id cmdName (CommandArgument destination) 
						 response <- messageExchange request comm
						 return $ parseResponse response cmdName 
						 where 
						 	   Controller id comm = controller
						 	   cmdName = "MOV"

rotate angle controller = do
						 let request = produceRequest id cmdName (CommandArgument angle) 
						 response <- messageExchange request comm
						 return $ parseResponse response cmdName 
						 where 
						 	   Controller id comm = controller
						 	   cmdName = "ROT"

fire controller = do
						 let request = produceRequest id cmdName Nil 
						 response <- messageExchange request comm
						 return $ parseResponse response cmdName 
						 where 
						 	   Controller id comm = controller
						 	   cmdName = "FR"

scan controller = do
						 let request = produceRequest id cmdName Nil 
						 response <- messageExchange request comm
						 return $ parseResponse response cmdName 
						 where 
						 	   Controller id comm = controller
						 	   cmdName = "SC"

test ctrl = do
	ans <- move 1.2 ctrl
	print ans
	ans <- rotate 10.1 ctrl
	print ans
	ans <- fire ctrl
	print ans
	ans <- scan ctrl
	print ans


main = do
	ctrl <- getController "192.168.0.2" "2001" "0"
	ctrl' <- ready ctrl
	mapM_ (\x -> test ctrl') [0..1000]
	endGame ctrl'
	print "OK"
