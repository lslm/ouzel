// Copyright (C) 2018 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <functional>
#include <string>
#if defined(_MSC_VER)
#include <Windows.h>
#else
#include <pthread.h>
#endif

namespace ouzel
{
    class Thread
    {
    public:
        Thread() {}
        explicit Thread(const std::function<void()>& function, const std::string& name = "");

        ~Thread();

        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        Thread(Thread&& other);
        Thread& operator=(Thread&& other);

        bool run();
        bool join();

        struct Parameters
        {
            std::function<void()> function;
            std::string name;
        };

    protected:
        Parameters parameters;

#if defined(_MSC_VER)
        HANDLE handle = nullptr;
#else
        pthread_t thread = 0;
#endif
    };
}
