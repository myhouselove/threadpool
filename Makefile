server:
	g++ -o server.bin -g src/main.cpp src/Server.cpp src/thread.cpp src/cpu_info.c \
	-I include  -lpthread;rm core -rf 
client:
	g++ -o client.bin -g src/main_client.cpp src/Client.cpp src/thread.cpp src/cpu_info.c \
	-I include -lpthread;rm core -rf
clean:
	rm *.bin -rf
