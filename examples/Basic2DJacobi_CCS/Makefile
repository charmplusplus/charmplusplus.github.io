CHARMDIR = /expand/home/gioachin/charm#[put Charm++ install directory here]
CHARMC = $(CHARMDIR)/bin/charmc $(OPTS)


default: all
all: jacobi client


##--- Executable ---##

jacobi : main.o jacobi.o
	$(CHARMC) -language charm++ -o jacobi main.o jacobi.o

client : client.o
	$(CHARMC) -language c++ -seq -o client client.o -lreadline -lccs-client


##--- Main Chare ---##

main.o : main.C main.h main.decl.h main.def.h jacobi.decl.h common.h
	$(CHARMC) -o main.o main.C

main.h : jacobi.decl.h main.decl.h common.h

main.decl.h main.def.h : main.ci
	$(CHARMC) main.ci


##--- Jacobi Chare ---##

jacobi.o : jacobi.C jacobi.h main.h common.h jacobi.def.h 
	$(CHARMC) -o jacobi.o jacobi.C

jacobi.h : jacobi.decl.h common.h

jacobi.decl.h jacobi.def.h : jacobi.ci
	$(CHARMC) jacobi.ci


##--- Client ---##

client.o : client.C
	$(CHARMC) -o client.o client.C

##--- Clean-up ---##

clean:
	rm -f main.decl.h main.def.h main.o
	rm -f jacobi.decl.h jacobi.def.h jacobi.o
	rm -f jacobi charmrun client
