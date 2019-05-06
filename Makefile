scc: scc.c

test: scc
	./scc -test
	./test.sh

clean:
	rm -f scc *.o *~ tmp*
