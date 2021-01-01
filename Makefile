CC = gcc
C_ARGS = -g

build : main.exe clear_objs

main.exe : main.o modules/fmap_win/fmap_win.o
	${CC} $^ -L./ C:/msys64/mingw64/lib/libz.a -o $@

main.o : main.c inc/ispsoft_file_def.h
	${CC} ${C_ARGS} -c $< -o $@

modules/fmap_win/fmap_win.o : modules/fmap_win/fmap_win.c modules/fmap_win/fmap_win.h 
	${CC} ${C_ARGS} -c $< -o $@

clear : 
	@rm -f main.exe
	@rm -f main.o
	@rm -f modules/fmap_win/fmap_win.o

clear_objs : 
	@rm -f main.o
	@rm -f modules/fmap_win/fmap_win.o