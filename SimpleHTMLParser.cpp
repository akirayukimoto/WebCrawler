
#include "SimpleHTMLParser.h"
#include "openhttp.h"
#include <string.h>

int i = 0;
char *c = new char[500];
int ind;

SimpleHTMLParser::SimpleHTMLParser()
{
}

bool
SimpleHTMLParser::match(char **b, const char *m) {
	int len = strlen(m);
	if ( !strncasecmp(*b, m, len)) {
		(*b)+=len;
		return true;
	}
	return false;
}

bool
SimpleHTMLParser::parse(char * buffer, int n)
{
	enum { START, TAG, SCRIPT, ANCHOR, HREF,
	       COMMENT, FRAME, SRC, TITLE, METAKEY, METACF, METANF} state;

	state = START;

	bool hasDocument = false;
	
	char * bufferEnd = buffer + n;
	char * b = buffer;
	bool lastCharSpace = false;
	while (b < bufferEnd) {
		//printf("<%c,%d,%d>", *b, *b,state);
		switch (state) {
		case START: {
			if (match(&b,"<SCRIPT")) {
				state = SCRIPT;
			}
			else if (match(&b,"<!--")) {
				state = COMMENT;
			}
			else if (match(&b,"<A ")) {
				state = ANCHOR;
			}
			else if (match(&b,"<FRAME ")) {
				state = FRAME;
			}
			else if (match(&b, "<TITLE ")) {
				state = TITLE;
				i++;
				onContentFound(' ');
			}
			else if(match(&b, "<META CONTENT=\"")) {
				state = METACF;
				memset(c, 0, 500*sizeof(char));
				ind = 0;
			}
			else if(match(&b, "<META NAME=\"DESCRIPTION\" CONTENT=\"")) {
				state = METANF;
				onContentFound(' ');
			}
			else if(match(&b, "<META NAME=\"KEYWORD\" CONTENT=\"")) {
				state = METANF;
				onContentFound(' ');
			}
			else if (match(&b, "</HEAD>")) {
				i = 0;
				onContentFound('*');
			}
			else if	(match(&b,"<")) {
				state = TAG;
			}
			else {
				char c = *b;
				//Substitute one or more blank chars with a single space
				//if (c=='\n'||c=='\r'||c=='\t'||c==' ') {
				//	if (!lastCharSpace) {
				//		if (hasDocument)
				//			onContentFound(' ');
				//	}
				//	lastCharSpace = true;
				//}
				//else {
				//	if (hasDocument)
				//		onContentFound(c);
				//	lastCharSpace = false;
				//}
				
				b++;
			}
			break;
		}
		case ANCHOR: {
			if (match(&b,"href=\"")) {
				state = HREF;
				urlAnchorLength=0;
				//printf("href=");
			}
			else if (match(&b,">")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
				
		}
		case HREF: {
			if (match(&b,"\"")) {
				// Found ending "
				state = ANCHOR;
				urlAnchor[urlAnchorLength] = '\0';
				onAnchorFound(urlAnchor);
				//printf("\n");
			}
			else {
				if ( urlAnchorLength < MaxURLLength-1) {
					urlAnchor[urlAnchorLength] = *b;
					urlAnchorLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
		}
		case FRAME: {
			if (match(&b,"src=\"")) {
				state = SRC;
				urlAnchorLength=0;
				//printf("href=");
			}
			else if (match(&b,">")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
				
		}
		case SRC: {
			if (match(&b,"\"")) {
				// Found ending "
				state = FRAME;
				urlAnchor[urlAnchorLength] = '\0';
				onAnchorFound(urlAnchor);
				//printf("\n");
			}
			else {
				if ( urlAnchorLength < MaxURLLength-1) {
					urlAnchor[urlAnchorLength] = *b;
					urlAnchorLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
		}
		case SCRIPT: {
			if (match(&b,"/SCRIPT>")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case COMMENT: {
			if (match(&b,"-->")) {
				// End comments
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case METANF: {
			if (match(&b,"\" />")) {
				state = START;
			}
			else if(match(&b,"\"/>")) {
				state = START;
				
			}
			else if(match(&b,"/>")) {
				state = START;
				
			}
			else if(match(&b,"\" >")) {
				state = START;
				//onContentFound('*');
			}
			else if(match(&b,"\">")) {
				state = START;
			}
			else if(match(&b,">")) {
				state = START;
			}			
			else {
				char c = *b;
				if (!(('a'<= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '0'))) {
					if (!lastCharSpace) {
						onContentFound(' ');
					}
					lastCharSpace = true;
				}
				else {
					onContentFound(c);
					lastCharSpace = false;
				}
				b++;
			}
			break;
		}
		case METACF: {
			if (match(&b, "\" name=\"description\"")||match(&b, "\" name=\"keywords\"")) {
				onContentFound(' ');
				for(int i = 0; i < 400; i++) {
					if(c[i]==0) break;
					else	onContentFound(c[i]);
				}
				state = START;
			}
			else if (match(&b,">")) {
				state = START;
			}			
			else {
				char d = *b;
				if (!(('a'<= d && d <= 'z') || ('A' <= d && d <= 'Z') || ('0' <= d && d <= '9'))) {
					if (!lastCharSpace) {
						c[ind++] = ' ';
					}
					lastCharSpace = true;
				}
				else {
					c[ind++] = *b;
					lastCharSpace = false;
				}
				b++;
			}
			break;

		}
		case TAG: {
			if (match(&b, ">")) {
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		default:;
		}
		
	}
}

void
SimpleHTMLParser::onContentFound(char c) {
}

void
SimpleHTMLParser::onAnchorFound(char * url) {
}
