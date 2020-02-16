#include <iostream>
#include <unistd.h>

int
main()
{
    close(0);
    std::cout << "some output\n";
    return 0;
}
