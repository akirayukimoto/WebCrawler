#include "webcrawler.h"

#include "SimpleHTMLParser.h"

int e = 0;

char *desc = (char*)malloc(10000*sizeof(char));

char *getWord = new char[20];

char *word;

int ccount=0;

int count = 0;

// Add your implementation here

WebCrawler::WebCrawler(int maxUrls, int nInitialURls,  const char ** initialURLs)

{

	// Allocate space for _urlArray

	_headURL = 0;

    _urlToUrlRecord = new HashTableTemplate<int>();

    _wordToURLRecordList = new HashTableTemplate<URLRecordList*>();

	_urlArray = new URLRecord[maxUrls];

	// insert the initialURls

	//printf("%d\n", nInitialURls);

	for (int i = 0; i < nInitialURls; i++) {

		_urlArray[i]._url = strdup(initialURLs[i]);

		//_urlArray[i]._description = "";

		_urlToUrlRecord -> insertItem(_urlArray[i]._url, i);

	}

	// Update _maxUrls, _headURL and _tailURL

	_maxUrls = maxUrls;

	_tailURL = nInitialURls;

	

}

void WebCrawler::writeURLFile(const char * urlFileName){

	remove(urlFileName);

	FILE *fd = fopen(urlFileName, "a");

	for(int i = 0; i < _tailURL;i++) {

			fprintf(fd, "%d ",e++);

			fprintf(fd, "%s\n", _urlArray[i]._url);

			fprintf(fd, "%s\n", _urlArray[i]._description);

		}



}



void WebCrawler::writeWordFile(const char *wordFileName) {

	remove(wordFileName);

	FILE *fd = fopen(wordFileName, "a");

	HashTableTemplateIterator<URLRecordList*> iterator(_wordToURLRecordList);

	const char *key;

	URLRecordList *e;

	while (iterator.next(key, e)) {

    	fprintf(fd, "%s ",key);

		while(e!= NULL) {

			fprintf(fd, "%d ", e-> _urlRecordIndex);

			e = e -> _next;

		}

		fprintf(fd,"\n"); 

    

  }



}

void WebCrawler::onContentFound(char c)

{ 	

	if ('A' <= c && c <= 'Z')

		c = c + 32;

	if(c != '*'){

		desc[count] = c;

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

        	if (_wordToURLRecordList->find(word, &prev) == false)

        	{

            	URLRecordList *e = new URLRecordList();

            	e -> _urlRecordIndex = _headURL;

            	e -> _next = NULL;

            	_wordToURLRecordList->insertItem(word, e);

        	}

        	else

        	{	

				int flag = 0;

				URLRecordList *tmp = prev;

				while (tmp!=NULL) {

					if(tmp -> _urlRecordIndex == _headURL) {

						flag = 1;						

						break;

					}

				tmp = tmp -> _next;

				}

					if (flag == 0) {

            			URLRecordList *e = new URLRecordList();

            			e -> _urlRecordIndex = _headURL;

            			e -> _next = prev;

						_wordToURLRecordList->insertItem(word, e);

            		}

				}}		

		}

	}

	else {

		desc[count] = '\0';

		count = 0;

		if(desc!=NULL) {

			//printf("abhiga\n");

			_urlArray[_headURL]._description = strdup(desc);

		}

	}

}

void WebCrawler::onAnchorFound(char * url){

	char *finalurl;	

	char *temp = (char *)malloc(1000*sizeof(char));

	memset(temp,0,1000*sizeof(char));

	bool flag = true;	

	if(_tailURL >= _maxUrls)

		return;

	//check if the absolute URL starts with http

	else if(strncmp(url,"http://", strlen("http://")) == 0) {

		strcpy(temp,url);

	}

	else if(strncmp(url,"//",strlen("//")) == 0) {

		strcpy(temp,"http:");

		strcat(temp,url);

	}

	else if(strncmp(url,"/",strlen("/")) == 0) {

		strcpy(temp, _urlArray[_headURL]._url);

		for(int i = 10; i < strlen(temp); i++) {

			if(temp[i] == '/') {

				temp[i] = '\0';

				break;

			}

		}

		strcat(temp,url);

		

	}

	//previous comments

	/*else if((('a' <= url[0] && url[0] <= 'z')||('A' <= url[0] && url[0] <= 'Z')) && (strncmp(url,"https://",(strlen("https://"))) !=0) && (strncmp(url,"ftp://",(strlen("ftp://"))) !=0)&& (strncmp(url,"mailto:",(strlen("mailto:"))) !=0)) {

		strcpy(temp, _urlArray[_headURL]._url);

		for(int i = strlen(temp)-1; i > 8; i--) {

			if(temp[i] =='/') {

				temp[i+1] = '\0';

				//strcat(temp,"/");

				strcat(temp,url);

				//printf("abhiga\n");	

				break;

			}

		}

	}*/

	for (int i = 0; i< _tailURL; i++) {

			//checking if the URL already exists in URL array

			if(temp[strlen(temp)-1] =='/')

				temp[strlen(temp) - 1] = '\0';

			finalurl = strdup(temp);

			if(strcmp(finalurl, _urlArray[i]._url)==0) {

				flag = false;

				break;

			}

		}

		if(flag && strcmp(finalurl,"")) {

			//inserting this absolute URL

			_urlArray[_tailURL]._url = finalurl;

		//	_urlArray[_tailURL]._description = "";

			_urlToUrlRecord -> insertItem(finalurl,_tailURL);

				//memset(desc,0,400*sizeof(char));

			//count = 0;

			_tailURL++;

		}

		free(temp);

}

void WebCrawler::crawl()



{	

	int n;

	//char * buffer;

	while (_headURL <_tailURL) {

		//Fetch the next URL in _headURL

		char* buffer = fetchHTML(_urlArray[_headURL]._url, &n);

		

		//Increment _headURL

		

		//If the document is not text/html 

		if(buffer == NULL) {

			_headURL++;

			continue;

		}

		parse(buffer,n);

		

		//Get the first 500 characters (at most) of the document without tags. Add this 

		//  description to theURL record for this URL.

		//_urlArray[_headURL]._description = "empty";

		_headURL++;

		//Find all the hyperlinks of this document and add them to the

		//_urlArray and _urlToUrlRecord if they are not already in the

		//_urlToUrlRecord. Only insert up to _maxURL entries.

		//if(_tailURL == _maxUrls - 1)

			//break;

		//For each word in the document without tags, add the index of this URL to

		//a URLRecordList in the _wordToURLRecordList table if the URL is not already there.

		//while

		//}

	}

	/*for(int i = 0; i < _tailURL;i++) {

			printf("%d\n",e++);

			printf("%s\n", _urlArray[i]._url);

			printf("%s\n", _urlArray[i]._description);

		}*/

}



int main (int argc, char** argv) {

	if(argc<2)

		fprintf(stderr,"Invalid Format\n");

	else {

		int count = 0;

		int maxURLs;

		const char **initialURLs;

		if(strcmp(argv[1],"-u") == 0) {

			maxURLs = atoi(argv[2]);

			initialURLs = new const char*[argc - 3];

			for (int i = 0 ; i < argc - 3; i++) {

				initialURLs[i] = argv[i+3];

				count++;

			}

		}

		else {

			maxURLs = 1000;

			initialURLs = new const char*[argc - 1];

			for (int i = 0 ; i < argc -1; i++) {

				initialURLs[i] = argv[i+1];

				count++;

			}

		}

		WebCrawler *w = new WebCrawler(maxURLs,count,initialURLs);

		w -> crawl();

		w -> writeURLFile("url.txt");

		w -> writeWordFile("word.txt");

	}

	return 0;

}