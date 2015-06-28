#ifndef __URL_H__
#define __URL_H__

#include <string>
using namespace std;

static int hextoi(char *s);

string urldecode(const string &str_source);

string urlencode(string &str_source);


#endif
