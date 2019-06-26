#pragma once

namespace Messages
{
    class MessagePipe {
    public:
        static void start();

    private:
        MessagePipe() {}
    };
}