module Tools where
import Data.String
import Numeric 


split :: String -> Char -> Int -> [String]
split str separator count = reverse $ split' str separator count [] []

split' :: String -> Char -> Int -> String -> [String] -> [String]
split' [] _ _ buf words = (buf:words)
split' str _ 0 [] words = (str:words)
split' str _ 0 buf words = (str:buf:words)
split' (ch:str) separator count buf words = if ch == separator then split' str separator (count-1) [] (buf:words)
										    else split' str separator count (buf++[ch]) words



erase :: String -> String -> String
erase str pattern = filter (\ch -> not $ elem ch pattern) str 


------ float to str and conversely --------
floatToFixedDecimals floatNum numOfDecimals = showFFloat (Just numOfDecimals) floatNum ""
strToFloat str = read str :: Float
------ --------------------------- ---------

------       3 element map         ---------   
map3 func list = if (length list > 2) then reverse $ map3' func list []
				 else []
map3' _ [] buf = buf
map3' func (el1:el2:el3:list) buf = if (length list > 2) then map3' func list ((func el1 el2 el3):buf)
									else ((func el1 el2 el3):buf)
------ --------------------------- ---------


--main = do
--	a <- return $ strToFloat "123.144124124"
--	print a