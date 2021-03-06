#include <pch.hpp>
#include <Client/Client.hpp>



int main(int argc, char **argv)
{
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return EXIT_FAILURE;
        }
        ::std::cout << "Choose a name: " << ::std::flush;
        ::std::string name;
        ::std::getline(::std::cin, name);
        ::Client client(argv[1], "8080", name);
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
