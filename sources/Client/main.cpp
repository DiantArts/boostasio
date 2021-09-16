#include <pch.hpp>
#include <Client/Client.hpp>



int main(int argc, char **argv)
{
    return 0;
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return EXIT_FAILURE;
        }
        ::Client client(argv[1], "8080");
        client.run();
        ::std::cout << "> EXIT CLIENT <" << ::std::endl;
        return EXIT_SUCCESS;

    } catch (const ::std::exception& e) {
       ::std::cerr << "ERROR: " << e.what() <<::std::endl;
        return EXIT_FAILURE;

    } catch (...) {
       ::std::cerr << "ERROR: unknown" <<::std::endl;
        return EXIT_FAILURE;

    }
}
