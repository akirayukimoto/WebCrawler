
all: git-commit gethttp webcrawler

gethttp: gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o gethttp -g gethttp.cpp openhttp.cpp SimpleHTMLParser2.cpp -lcurl

webcrawler: webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp gethttp.cpp
	g++ -o webcrawler -g openhttp.cpp SimpleHTMLParser2.cpp webcrawler2.cpp HashTableTemplate.h -lcurl

git-commit:
	git add Makefile *.h *.cpp  .local.git.out >> .local.git.out
	git commit -a -m "Commit web-crawler" >> .local.git.out

clean:
	rm -f *.o gethttp core
	rm -f *.o webcrawler core
