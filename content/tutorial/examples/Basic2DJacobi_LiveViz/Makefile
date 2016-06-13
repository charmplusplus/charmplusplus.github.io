CHARMDIR = ~/charm/net-linux-x86_64
CHARMC = $(CHARMDIR)/bin/charmc $(OPTS)


default: all
all: jacobi


jacobi : main.o jacobi.o
	$(CHARMC) -language charm++ -module liveViz -o jacobi main.o jacobi.o

main.o : main.C main.h main.decl.h main.def.h jacobi.decl.h common.h
	$(CHARMC) -o main.o main.C

main.decl.h main.def.h : main.ci
	$(CHARMC) main.ci

main.h : jacobi.decl.h

jacobi.o : jacobi.C jacobi.h jacobi.decl.h jacobi.def.h main.decl.h common.h
	$(CHARMC) -o jacobi.o jacobi.C

jacobi.decl.h jacobi.def.h : jacobi.ci
	$(CHARMC) jacobi.ci


clean:
	rm -f main.decl.h main.def.h main.o
	rm -f jacobi.decl.h jacobi.def.h jacobi.o
	rm -f jacobi charmrun
