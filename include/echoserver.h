#ifndef ASIO_NETWORK_PROGRAMMING_ECHOSERVER_H
#define ASIO_NETWORK_PROGRAMMING_ECHOSERVER_H

#include "wrapper.h"
#include <conio.h>
#include <boost/thread/mutex.hpp>

boost::mutex global_stream_lock;

int workspace(){
    return 0;
}

#endif //ASIO_NETWORK_PROGRAMMING_ECHOSERVER_H
