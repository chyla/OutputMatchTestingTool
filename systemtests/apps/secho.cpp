#include <iostream>

int main(int argc, char **argv) {
    --argc;
    ++argv;

    if (argc > 0) {
        for (int i = 0; i < argc - 1; ++i) {
            std::cout << argv[i] << " ";
        }
        std::cout << argv[argc - 1] << '\n';
    }
    return 0;
}
