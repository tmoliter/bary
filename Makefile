timtest:
	g++ -o bary -I . -I ./components *.cpp components/*.cpp things/*.cpp gui/*.cpp events/*.cpp editor/*.cpp   -I include -L lib -l SDL2-2.0.0 -lSDL2_image -std=c++17