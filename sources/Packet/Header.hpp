// Every message is devided into multiple 250 chars messages at max.

#pragma once



namespace packet {



struct Header {

    enum class Type : ::std::uint8_t {
        error = 0,
        ping,
        pingAck,
        connect,
        connectionNeeded,
        connectionRequest,
        disconnect,
        text,
        packetLoss
    };
    Header::Type type;

    static inline ::std::uint8_t packetIdGenerator{ 0 };
    mutable ::std::uint8_t packetId; // 0 = not important, else, ID equal to last ID + 1

    const ::std::uint8_t packetPosition; // position of the message in the multi messages range

    const ::std::uint8_t bodySize;

};



} // namespace packet
