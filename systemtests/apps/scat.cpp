#include <cstdio>
#include <iostream>

int
main()
{
    int c = getchar();
    while (c != EOF) {
        std::cout << char(c);
        c = getchar();
    }
    return 0;
}
