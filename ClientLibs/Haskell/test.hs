import Controller

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
	ctrl <- ready ctrl
	mapM_ (\x -> test ctrl) [0..1000]
	endGame ctrl
	print "OK"