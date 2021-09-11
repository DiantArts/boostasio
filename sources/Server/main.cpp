#include <pch.hpp>
#include <Server/Server.hpp>



int main(int argc, char **argv)
{
    try {

        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            return EXIT_FAILURE;
        }
        ::Server server{ ::std::atoi(argv[1]) };
        server.run();
        return EXIT_SUCCESS;

    } catch (const ::std::exception& e) {
       ::std::cerr << "ERROR: " << e.what() <<::std::endl;
        return EXIT_FAILURE;

    } catch (...) {
       ::std::cerr << "ERROR: unknown" <<::std::endl;
        return EXIT_FAILURE;

    }
}
