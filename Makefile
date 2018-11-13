projet : projet.c projet.h
	gcc $< -o $@

clean:
	rm -rf *.o
