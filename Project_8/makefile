PROJNAME = treasurehunt

all:$(PROJNAME)

$(PROJNAME):src/$(PROJNAME).c
	mkdir -p ./bin/
	gcc -std=c99 -o ./bin/$(PROJNAME) ./src/$(PROJNAME).c

test:$(PROJNAME)
	./bin/$(PROJNAME) < ./test_input.txt

clean:
	rm ./bin/$(PROJNAME)
