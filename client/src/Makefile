ELF:=../bin/client
OBJ:=main.c  packet.c control.c 
$(ELF):$(OBJ)
	gcc $^ -o $@ -I../include -lpthread
.PHONY:clean
clean:
	  rm -rf ../bin/server
