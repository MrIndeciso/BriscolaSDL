build () {

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
	
	cp Lib/SDL/lib/win32/SDL2.dll Output/Windows/SDL2.dll
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
