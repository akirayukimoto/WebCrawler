
all: git-commit gethttp webcrawler

gethttp: gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o gethttp -g gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp -lcurl

webcrawler: webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp HashTableTemplate.h
	g++ -o webcrawler -g openhttp.cpp SimpleHTMLParser.cpp webcrawler.cpp HashTableTemplate.h -lcurl

git-commit:
	git add Makefile *.h *.cpp  .local.git.out >> .local.git.out
	git commit -a -m "Commit web-crawler" >> .local.git.out

clean:
	rm -f *.o gethttp core
	rm -f *.o webcrawler core
