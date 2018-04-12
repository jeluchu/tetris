

portaudio_files = /home/student/jcalderon/include/portaudio/portaudio_files

all:
		g++ -c interfaz.cpp -lncursesw
		g++ -c main.cpp -lncursesw -I$(portaudio_files)/include -L$(portaudio_files)/lib -libportaudio
		g++ -o tetris interfaz.o main.o -lncursesw -L$(portaudio_files)/lib -L$(portaudio_files)/lib/.libs -lportaudio
