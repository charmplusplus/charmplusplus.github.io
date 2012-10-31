CHARMDIR = [put Charm++ install directory here]
CHARMC = $(CHARMDIR)/bin/charmc $(OPTS)


default: all
all: bubble


bubble : main.o bubble.o
	$(CHARMC) -language charm++ -o bubble main.o bubble.o

main.o : main.C main.h main.decl.h main.def.h bubble.decl.h
	$(CHARMC) -o main.o main.C

main.decl.h main.def.h : main.ci
	$(CHARMC) main.ci

main.h : bubble.decl.h

bubble.o : bubble.C bubble.h bubble.decl.h bubble.def.h main.decl.h
	$(CHARMC) -o bubble.o bubble.C

bubble.decl.h bubble.def.h : bubble.ci
	$(CHARMC) bubble.ci


clean:
	rm -f main.decl.h main.def.h main.o
	rm -f bubble.decl.h bubble.def.h bubble.o
	rm -f bubble charmrun
