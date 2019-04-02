build () {

	clear
	clean
	
	# build functions here

	buildLinux
	buildWin

}

buildLinux () {

	cp -a Assets/. Output/Linux/Assets/

	make linux

}

buildWin () {
	
	cp Lib/SDL/lib/win32/*.dll Output/Windows/
	cp Lib/SDL/lib/win32/LICENSE*.* Output/Windows/
	cp -a Assets/. Output/Windows/Assets/

	make win

}

clean () {

	rm -r Output/Linux
	rm -r Output/Windows
	mkdir Output/Linux
	mkdir Output/Windows

}

run () {

	Output/Linux/briscola

	wine Output/Windows/Briscola.exe

}

buildrun () {

	build
	run

}
