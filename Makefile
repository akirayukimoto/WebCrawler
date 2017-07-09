
all: git-commit gethttp

gethttp: gethttp.cpp openhttp-old.cpp SimpleHTMLParser.cpp
	g++ -o gethttp -g gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp -lcurl

webcrawler: webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp HashTableTamplate.h
	g++ -o webcrawler -g webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp HashTableTemplate.h

git-commit:
	git add Makefile *.h *.cpp  .local.git.out >> .local.git.out
	git commit -a -m "Commit web-crawler" >> .local.git.out

clean:
	rm -f *.o gethttp core
