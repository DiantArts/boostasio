#include <pch.hpp>
#include <Server/Udp/Server.hpp>



int main(int argc, char **argv)
{
    try {
        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            return EXIT_FAILURE;
        }
        ::boost::asio::io_context ioContext;
        ::udp::Server server{ ioContext, 8080 };
        ::std::cout << "> SERVER START <" << ::std::endl;
        ioContext.run();
        ::std::cout << "> EXIT SERVER <" << ::std::endl;
        return EXIT_SUCCESS;

    } catch (const ::std::exception& e) {
       ::std::cerr << "ERROR: " << e.what() <<::std::endl;
        return EXIT_FAILURE;

    } catch (...) {
       ::std::cerr << "ERROR: unknown" <<::std::endl;
        return EXIT_FAILURE;

    }
}