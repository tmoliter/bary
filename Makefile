timtest:
	g++ -g -o bary -I . *.cpp components/*.cpp things/*.cpp gui/*.cpp events/*.cpp editor/*.cpp -I include -L lib -l SDL2-2.0.0 -l SDL2_image -l SDL2_mixer -llua -ldl -std=c++17