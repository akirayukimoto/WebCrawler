
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "webcrawler.h"

// Add your implementation here

char *word;
char *description;

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
		int len = strlen(urlRoots[i]);
		char *url = new char[len];
		//char *url = new char [strlen(urlRoots[i])];
		strcpy(url, urlRoots[i]);
		if (url[len - 1] != '/') url[len - 1] = '/';
		_urlArray[i]._url = strdup(url);
		_urlArray[i]._description = NULL;
	
	}
	_urlToUrlRecord = new HashTableTemplate<int>();
	_urlToUrlRecordList = new HashTableTemplate<URLRecordList*>();
}

void 
WebCrawler::onContentFound(char c) {
}

void
WebCrawler::onAnchorFound(char *url) {
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

	}
}

void
WebCrawler::writeURLFile(const char *urlFileName)
{
	FILE *f = fopen(urlFileName, "r");
	for (int i = 0; i < _tailURL; i++) {
		fprintf(f, "%d %s\n",i, _urlArray[i]._url);
		if (_urlArray[i]._description != NULL) 
			fprintf(f, "%s\n", _urlArray[i]._description);
		else fprintf(f, "%s\n", "");
		fprintf(f, "%s\n", "");
	}
	fclose(f);
}

void
WebCrawler::writeWordFile(const char *wordFileName)
{
	FILE *file = fopen(wordFileName, "w");
	for (int i = 0; i < _tailURL; i++) {
		fprintf(file, "%s", _wordToURLRecordList->_buckets[i]._key);
		
	}
	fclose(file);
}

int main() {
	return 0;
}
