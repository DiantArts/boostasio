#pragma once

#include <Packet/APacket.hpp>



namespace packet {



class Error
    : public ::APacket
{

public:

    enum class Type : uint8_t {
        unknown = 0,
        alreadyConnected
    };



public:

    // ------------------------------------------------------------------ *structors

    Error(
        Error::Type type
    );

    ~Error();



    // ------------------------------------------------------------------ informations

    [[ nodiscard ]] auto getId() const
        -> Error::Type;

    [[ nodiscard ]] auto toString() const
        -> ::std::string;



private:

    Error::Type m_errorType;

    static inline constexpr char errorMessages[][256] = {
        "Unknown error type",
        "Client already connected",
        "Failure"
    };
};




} // namespace packet
