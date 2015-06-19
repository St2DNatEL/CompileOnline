#include "Http.h"

Http::Http():strBuf(),requestHead(),method(),path(),protocolVersoin(),
	msgHead(),mapMsgHead(),msgBody(),mapQueryString()
{
}
Http::~Http(void)
{
}