#pragma once

#include <Udp/AMessage.hpp>



namespace udp::message {



class Error
    : public ::udp::AMessage
{

public:

    enum class Type : uint8_t {
        unknown = 0
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
        "Failure"
    };
};




} // namespace udp::message
