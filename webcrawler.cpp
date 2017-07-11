
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "webcrawler.h"

// Add your implementation here

char *word;
char *description;

int inserted;

//call of duty modern warfare mac

int
strlength(const char *str) {
	int len = 0;
	while (str[len]) len++;
	return len;
}

void

WebCrawler::onContentFound(char c) {
    if (word == NULL) {
        word = new char[1];
        word[0] = '\0';
    }
    if (description == NULL) {
        description = new char[1];
        description[0] = '\0';
    }

    if (('A' <= c && c <= 'Z') ||
        ('a' <= c && c <= 'z') ||
        ('0' <= c && c <= '9')) {
        char *single = new char[2];
        single[0] = c;
        single[1] = '\0';
        char *buffer;
        buffer = strcat(word, single);
        strcpy(word, buffer);
    }
    else {
        if (word == NULL || strlength(word) <= 0)
            return;
        URLRecordList *tmp = NULL;        
        if (_wordToURLRecordList->find(word, &tmp) == false) {
            URLRecordList *data = new URLRecordList();
            data->_urlRecordIndex = _headURL;
            data->_next = NULL;
            _wordToURLRecordList->insertItem(word, data);
        }
        else {
            URLRecordList *data = new URLRecordList();
            data->_urlRecordIndex = _headURL;
            data->_next = tmp;
            _wordToURLRecordList->insertItem(word, data);

        }
        word = NULL;

    }
    if (strlength(description) < 500) {
        char *single = new char[2];
        single[0] = c;
        single[1] = '\0';
        char *buffer;
        buffer = strcat(description, single);
        strcpy(description, buffer);
        delete [] single;
    }
}



void

WebCrawler::onAnchorFound(char *url) {
    if (inserted >= _maxUrls) return;
    char *root = _urlArray[_headURL]._url;

    if (root[strlength(root) - 1] != '/') {
        root = strcat(root, "/");
    }

    if (url[0] == '/') {
        int count = 0;
        int i;
        for (i = 0; i < strlength(root); i++) {
            if (root[i] == '/')
                count = count + 1;
            else continue;
            if (count >= 3) {
                root[i] = '\0';
                break;
            }
        }
        url = strcat(root, url);
    }
    else {
        const char *http = "http://";
        const char *https = "https://";
        if (strncmp(url, http, 7) != 0 && strncmp(url, https, 8) != 0) {
            url = strcat(root, url);
        }
    }
    char *absoluteURL = new char[strlength(url) + 1];
    strcpy(absoluteURL, url);
    int tmp;
    if (_urlToUrlRecord->find(absoluteURL, &tmp) == false) {
        _urlArray[_tailURL]._url = absoluteURL;
        _urlArray[_tailURL]._description = NULL;
        _tailURL = _tailURL + 1;
        _urlToUrlRecord->insertItem(absoluteURL, _headURL);
        inserted = inserted + 1;
    }

}

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
		//_urlArray[i]._description = (char *)malloc(1000);
		_urlArray[i]._description = NULL;
	
	}
	//for (int i = 0; i < (nurlRoots + maxUrls); i++) {
	//	_urlArray[i]._description = (char *)malloc(1000);
	//}
	_urlToUrlRecord = new HashTableTemplate<int>();
	_wordToURLRecordList = new HashTableTemplate<URLRecordList*>();
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
		
		int n;
		char *buff = fetchHTML(curr, &n);
		if (buff == NULL) {
			_headURL = _headURL + 1;
			continue;
		}

		parse(buff, n);

		_urlArray[_headURL]._description = description;

		_headURL = _headURL +1;

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
	//crawler->writeURLFile("url.txt");
	//crawler->writeWordFile("word.txt");
	
}
