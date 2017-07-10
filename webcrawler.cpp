
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "webcrawler.h"

// Add your implementation here

char *word;
char *description;

int inserted;

WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char **urlRoots)
{
	// Allocate space for _urlArray
	// insert the initiaLURLs
	// Update _maxUrls, _headURL, and _tailURL
	_maxUrls = maxUrls;
	_headURL = 0;
	_tailURL = nurlRoots;
	_urlArray = new URLRecord[maxUrls + nurlRoots];

	for (int i = 0; i < nurlRoots; i++) {
		//char *url = strdup(urlRoots[i]);
		//int len = strlen(urlRoots[i]);
		//char *url = new char[len];
		//char *url = new char [strlen(urlRoots[i])];
		//strcpy(url, urlRoots[i]);
		//if (url[len - 1] != '/') url[len - 1] = '/';
		_urlArray[i]._url = strdup(urlRoots[i]);
		_urlArray[i]._description = (char *)malloc(1000);
		_urlArray[i]._description = NULL;
	
	}
	for (int i = 0; i < (nurlRoots + maxUrls); i++) {
		_urlArray[i]._description = (char *)malloc(1000);
	}
	_urlToUrlRecord = new HashTableTemplate<int>();
	_wordToURLRecordList = new HashTableTemplate<URLRecordList*>();
}

//char *buffer = new char[10000];
char *buffer = (char *)malloc(sizeof(char) * 10000);
char *ptr = buffer;

void 
WebCrawler::onContentFound(char c) {
	
	if (c == '[') {
		*ptr = '\0';
		ptr = buffer;
		if (_urlArray[_headURL]._description == NULL) 
			_urlArray[_headURL]._description = strdup(ptr);
		else {
			strcat(_urlArray[_headURL]._description, " ");
			strcat(_urlArray[_headURL]._description, ptr);
		}
		
	}
	else if (c == ']') {
		memset(buffer, 0, strlen(buffer));
		ptr = buffer;
	}
	else if (c == '"') {
		//return;
	}
	else {
		*ptr = c;
		ptr++;
	}
}

char *bss;
int flag1 = 0;
int flag2 = 0;

void
WebCrawler::onAnchorFound(char *url) {
	if (_tailURL >= _maxUrls) return;
	const char *http = "http://";
	const char *https = "https://";
	const char *slash = "//";
	const char *hashtag = "#";
	bss = new char[200];

	if (!strncmp(http, url, 7)) {
		for (int i = 0; i < _tailURL; i++) {
			if (!strcmp(url, _urlArray[i]._url)) {
				flag1 = 1;
				break;
			}
		}
		if (flag1 == 1) {
			flag1 = 0;
			return;
		}
		else {
			_urlArray[_tailURL]._url = strdup(url);
			_tailURL++;
		}
	}
	else if (!strncmp(slash, url, 2)) {
		//return;
	}
	else {
		if (strncmp(https, url, 8) && strncmp(hashtag, url, 1)) {
			bss = strdup(_urlArray[_headURL]._url);
			if (strncmp(url, "/", 1)) bss = strcat(bss, "/");
			bss = strcat(bss, url);
			_urlArray[_tailURL]._url = strdup(bss);
			_urlArray[_tailURL]._description = NULL;
			_tailURL++;
		}
	}



}

void
WebCrawler::insertURL() {
	for (int i = 0; i < _tailURL; i++) {
		int temp;
		if (!_urlToUrlRecord->find(_urlArray[i]._url, &temp)) {
			_urlToUrlRecord->insertItem(_urlArray[i]._url, i);
		}
	}
}

char *nw = (char *)malloc(sizeof(char) * 1000);
char*
WebCrawler::nextWord(char * &c) {
	//char * nw = new char[1000];
	int i = 0;
	while (*c != '\0') {
		if (*c == ' ' || *c == '.' || *c == ',' || *c == '-') {
			if (i == 0) {
				c++;
				continue;
			}
			else {
				nw[i] == '\0';
				return nw;
			}
		}
		else {
			nw[i++] = *c;
			//i++;
			c++;
		}
	}
	if (i > 0) {
		nw[i] = '\0';
		return nw;
	}
	return NULL;
}

void
WebCrawler::insertWord() {
	for (int i = 0; i < _tailURL; i++) {
		if (_urlArray[i]._description != NULL) {
			//char *c;
			char *des = _urlArray[i]._description;
			char *c = nextWord(des);
			URLRecordList *tmp = NULL;
			URLRecordList *curr;
			if (c != NULL) {
				URLRecordList *data = new URLRecordList();
				if (!_wordToURLRecordList->find(c, &tmp)) {
					//URLRecordList *data = new URLRecordList();
					data->_urlRecordIndex = i;
					data->_next = tmp;
					_wordToURLRecordList->insertItem(c, data);
				}
				else {
					URLRecordList *tmp2 = tmp;
					int flag = 0;
					while (tmp2 != NULL) {
						if (tmp2->_urlRecordIndex == i) {
							flag = 1;
							break;
						}
						tmp2 = tmp2->_next;
					}
					if (flag == 1) {
						flag = 0;
						continue;
					}
					else {
						//URLRecordList *data = new URLRecordList();
						data->_urlRecordIndex = i;
						data->_next = tmp;
						_wordToURLRecordList->insertItem(c, data);
					}

				}
			}

		}
	}
}

void
WebCrawler::writeURLFile(const char *urlFileName)
{
	FILE *f = fopen(urlFileName, "w");
	for (int i = 0; i < _tailURL; i++) {
		fprintf(f, "%d %s\n",i, _urlArray[i]._url);
		if (_urlArray[i]._description != NULL) {
			printf("%s\n", _urlArray[i]._description);
			fprintf(f, "%s\n", _urlArray[i]._description);
		}
		else fprintf(f, "%s\n", "");
		//fprintf(f, "%s\n", "");
		fprintf(f, "\n");
	}
	fclose(f);
}

void
WebCrawler::writeWordFile(const char *wordFileName)
{
	FILE *file = fopen(wordFileName, "w");

	HashTableTemplateIterator<URLRecordList *> it(_wordToURLRecordList);
	const char *key;
	URLRecordList *data;
	int j = -1;

	while(it.next(key, data)) {
		fprintf(file, "%s ", key);
		URLRecordList *curr = data;
		while (curr != NULL) {
			if (j != curr->_urlRecordIndex) {
				fprintf(file, "%d ", curr->_urlRecordIndex);
			}
			j = curr->_urlRecordIndex;
			curr = curr->_next;
		}
		fprintf(file, "\n");
		
	}
	fclose(file);
}

void
WebCrawler::crawl()
{
	while (_headURL < _tailURL) {
		// Fetch the next URL in _headURL
		//
		// increment _headURL
		// if the document is not text/html
		// countinue;
		// Get the first 500 charactoers (at most) of the document without tags.
		// Add this description to the URL record for this URL
		//
		// Find all the hyperlinks of this document
		// Add them to the _urlArray and _urlToRecord 
		// if they are not already in the _urlToUrlRecord. Only insert up to _maxURL entries
		//
		// For each word in the document without tags, add the index of this URL to
		// a URLRecordList in the _wordToURLRecordList table if the URL is not already there
		char *curr = _urlArray[_headURL]._url;
		
		int n = (int *)malloc(sizeof(int));
		char *buffer = fetchHTML(buffer, &n);
		if (buffer == NULL) {
			_headURL++;
			continue;
		}

		parse(buffer, n);

		_headURL++;

		//insertURL();
		//insertWord();

		//_urlArray[_headURL]._description = strdup(description);
	}
	insertURL();
	//for (int i = 0; i < _tailURL; i++) {
	//	printf("%i %s\n",i, _urlArray[i]._description); 
	//}
	insertWord();
	//_urlArray[_headURL]._description = strdup(description);
	//const char *urlFile = "url.txt";
	//writeURLFile(urlFile);
	//writeWordFile("word.txt");

}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: webcrawler [-u <maxurls>] url-list\n");
		exit(1);
	}
	int maxUrls = 1000;
	int inserted = 0;
	int start = 1;
	const char **initialUrls;
	if (!strcmp(argv[1], "-u")) {
		maxUrls = atoi(argv[2]);
		start = 3;
	}
	initialUrls = new const char *[(argc - start)];

	for (int i = start; i < argc; i++) {
		initialUrls[(i - start)] = argv[i];
	}

	WebCrawler *crawler = new WebCrawler(maxUrls, (argc - start), initialUrls);
	crawler->crawl();
	crawler->writeURLFile("url.txt");
	crawler->writeWordFile("word.txt");
	
}
