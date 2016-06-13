CHARMDIR = [put Charm++ install directory here]
CHARMC = $(CHARMDIR)/bin/charmc $(OPTS)


default: all
all: prefix


prefix : main.o prefix.o
	$(CHARMC) -language charm++ -o prefix main.o prefix.o

main.o : main.C main.h main.decl.h main.def.h prefix.decl.h
	$(CHARMC) -o main.o main.C

main.decl.h main.def.h : main.ci
	$(CHARMC) main.ci

main.h : prefix.decl.h

prefix.o : prefix.C prefix.h prefix.decl.h prefix.def.h main.decl.h
	$(CHARMC) -o prefix.o prefix.C

prefix.decl.h prefix.def.h : prefix.ci
	$(CHARMC) prefix.ci


clean:
	rm -f main.decl.h main.def.h main.o
	rm -f prefix.decl.h prefix.def.h prefix.o
	rm -f prefix charmrun
