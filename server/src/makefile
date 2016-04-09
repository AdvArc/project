ELF:=../bin/server
OBJ:=main.c server.c packet.c que.c pool.c control.c
$(ELF):$(OBJ)
	gcc -g -Wall $^ -o $@ -I../include -lpthread
.PHONY:clean
clean:
	rm -rf ../bin/server
	
