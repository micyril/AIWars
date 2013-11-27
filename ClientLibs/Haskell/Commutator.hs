module Commutator where
import Network.Socket hiding (send, sendTo, recv, recvFrom)
import Network.Socket.ByteString (send, recv)
import Tools
import qualified Data.ByteString.Char8 as B8


----- State =  Connected or not ? -----
type State = Int
type ServerInfo = AddrInfo
data Commutator  = Commutator Socket ServerInfo State 

endLine :: String
endLine = "\r\n"

getCommutator :: String -> String -> IO Commutator
getCommutator host port = withSocketsDo $ do 
			addrInfo <- getAddrInfo Nothing (Just host) (Just port)
			let server = head addrInfo
			sock <- socket (addrFamily server) Stream defaultProtocol
			return $ Commutator sock server 0

connectCommutator :: Commutator -> IO Commutator
connectCommutator comm = do
			let Commutator socket server _ = comm
			connect socket (addrAddress server)
			return $ Commutator socket server 1

sendAll :: Socket -> String -> IO Int
sendAll socket message = do
			send socket $ B8.pack (message ++ endLine)

recvAll :: Socket -> IO String
recvAll socket = do
			out <- recv socket 1024
			return $  erase (B8.unpack out) endLine

messageExchange :: String -> Commutator -> IO String
messageExchange message comm =  if (state == 1) then do
									sendAll socket message
									recvAll socket
							    else return ""
							    where Commutator socket _ state  =  comm


disconnectCommutator :: String -> Commutator -> IO Commutator
disconnectCommutator lastmessage comm = if (state == 1) then do
											sendAll socket lastmessage
											sClose socket
											return $ Commutator socket server 0
									    else return comm
							    		where Commutator socket server state  =  comm

handShake :: String ->  Commutator -> IO String
handShake hshkmsg comm = if(state == 1) then do
							sendAll socket hshkmsg
							recvAll socket
						 else return ""
						where Commutator socket _ state  =  comm

--main = do
--	comm <- connectCommutator $ getCommutator "192.168.0.2" "2001"
--	msg <- handShake "HI" comm
--	print msg