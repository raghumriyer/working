#include "httpd.h"
#include "RequestWorkerThread.h"


// The main file, which run the http server

extern "C" void serve_forever(const char*);

int main(int c, char** v)
{
    mins();
    serve_forever("12914");
    return 0;
}
