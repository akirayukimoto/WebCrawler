
#include "webcrawler.h"

// Add your implementation here

char *word;
//char *description;

int inserted;


int e = 0;

char *description = (char*)malloc(500*sizeof(char));
char *getWord = new char[50];
int ccount=0;
int count = 0;

//call of duty modern warfare mac

WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char **urlRoots)
{
	// Allocate space for _urlArray
	// insert the initiaLURLs
	// Update _maxUrls, _headURL, and _tailURL
	_maxUrls = maxUrls;
	_headURL = 0;
	_tailURL = nurlRoots;
	_urlArray = new URLRecord[maxUrls + nurlRoots];
	_urlToUrlRecord = new HashTableTemplate<int>();
	_wordToURLRecordList = new HashTableTemplate<URLRecordList*>();

	for (int i = 0; i < nurlRoots; i++) {
		_urlArray[i]._url = strdup(urlRoots[i]);
		//_urlArray[i]._description = NULL;
		_urlToUrlRecord->insertItem(_urlArray[i]._url, i);
	
	}

}

/*
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
*/
void
WebCrawler::writeURLFile(const char *urlFileName)
{
	FILE *f = fopen(urlFileName, "w+");
	if (f == NULL) return;
	for (int i = 0; i < _tailURL; i++) {
		fprintf(f, "%d %s\n",i, _urlArray[i]._url);
		//if (_urlArray[i]._description != NULL) {
		if (_urlArray[i]._description[0] != '\0') {
			fprintf(f, "%s\n", _urlArray[i]._description);
			//fprintf(f, "%s\n", _urlArray[i]._description);
		}
		else fprintf(f, "%s\n", "No descriotion");
		//}
		//else fprintf(f, "%s\n", "No description");
		//fprintf(f, "%s\n", "");
		fprintf(f, "\n");
	}
	//fclose(f);
}

void
WebCrawler::writeWordFile(const char *wordFileName)
{
	FILE *file = fopen(wordFileName, "w+");
	if (file == NULL) return;
	HashTableTemplateIterator<URLRecordList *> it(_wordToURLRecordList);
	const char *key;
	URLRecordList *data;
	//int j = -1;

	while(it.next(key, data)) {
		fprintf(file, "%s ", key);
		URLRecordList *curr = data;
		while (curr != NULL) {
			//if (j != curr->_urlRecordIndex) {
			fprintf(file, "%d ", curr->_urlRecordIndex);
			//}
			//j = curr->_urlRecordIndex;
			curr = curr->_next;
		}
		fprintf(file, "\n");
		
	}
	//fclose(file);
}

void WebCrawler::onContentFound(char c)
{ 	
	if ('A' <= c && c <= 'Z')
		c = c + 32;
	if(c != '*'){
		description[count] = c;
		count++;
		if(c!= '\t' && c!=' ' && c!= '\n' && c!= '\0') {
			getWord[ccount] = c;
			ccount++;
		}		
		else {
			getWord[ccount] = '\0';
			word = strdup(getWord);
			ccount = 0;
			URLRecordList *prev = NULL;    
			if (strcmp(word,"")) {    
        			if (!_wordToURLRecordList->find(word, &prev)) {
            				URLRecordList *data = new URLRecordList();
         				data->_urlRecordIndex = _headURL;
            				data->_next = NULL;
            				_wordToURLRecordList->insertItem(word, data);
        			}
        			else {	
					int flag = 0;
					URLRecordList *tmp = prev;
					while (tmp != NULL) {
						if(tmp->_urlRecordIndex == _headURL) {
							flag = 1;
							break;
						}
						tmp = tmp->_next;
					}
					if (flag == 0) {
            					URLRecordList *data = new URLRecordList();
            					data->_urlRecordIndex = _headURL;
            					data->_next = prev;
						_wordToURLRecordList->insertItem(word, data);
            				}
				}
			}		

		}

	}
	else {
		description[count] = '\0';
		count = 0;
		if(description != NULL) {
			_urlArray[_headURL]._description = strdup(description);

		}
	}
}

void WebCrawler::onAnchorFound(char * url){
	char *finalurl;	
	char *temp = (char *)malloc(1000*sizeof(char));
	memset(temp,0,1000*sizeof(char));
	bool flag = true;
	const char *http = "http://";
	const char *https = "https://";
	if(_tailURL >= _maxUrls) return;
	else if(!strncmp(url, http, strlen("http://"))) {
		strcpy(temp,url);
	}
	else if (!strncmp(url, https, strlen("https://"))) {
		strcpy(temp,url);
	}
	else if(!strncmp(url,"//",strlen("//"))) {
		strcpy(temp,"http:");
		strcat(temp,url);
	}
	else if(!strncmp(url,"/",strlen("/"))) {
		strcpy(temp, _urlArray[_headURL]._url);
		for(int i = 10; i < strlen(temp); i++) {
			if(temp[i] == '/') {
				temp[i] = '\0';
				break;
			}
		}
		strcat(temp,url);
	}
	for (int i = 0; i< _tailURL; i++) {
		//checking if the URL already exists in URL array
		if(temp[strlen(temp)-1] =='/')
			temp[strlen(temp) - 1] = '\0';
			finalurl = strdup(temp);
			if(!strcmp(finalurl, _urlArray[i]._url)) {
				flag = false;
				break;
			}
	}
	if(flag && strcmp(finalurl, "")) {
		_urlArray[_tailURL]._url = finalurl;
	//	_urlArray[_tailURL]._description = "";
		_urlToUrlRecord -> insertItem(finalurl, _tailURL);
		//count = 0;
		_tailURL++;
	}
	free(temp);
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

		//_urlArray[_headURL]._description = description;

		_headURL++;

		//insertURL();
		//insertWord();

		//_urlArray[_headURL]._description = strdup(description);
	}
	//insertURL();
	//for (int i = 0; i < _tailURL; i++) {
	//	printf("%i %s\n",i, _urlArray[i]._description); 
	//}
	//insertWord();
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
