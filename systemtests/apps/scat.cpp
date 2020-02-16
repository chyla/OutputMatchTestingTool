#include <iostream>
#include <unistd.h>
#include <cstring>

int
main()
{
    char *buf[1024];
    int c;

    while ( (c = read(0, buf, 1024)) > 0) {
        int x = write(1, buf, c);
        if (x < 0) {
            std::cerr << "scat: write:" << strerror(errno) << '\n';
        }
    }

    if (c < 0) {
      std::cerr << "scat: read:" << strerror(errno) << '\n';
    }

    return 0;
}
