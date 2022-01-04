//
// Created by jstol on 04.01.2022.
//

#ifndef ASIO_NETWORK_PROGRAMMING_WRAPPER_H
#define ASIO_NETWORK_PROGRAMMING_WRAPPER_H

// Include the required header files
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <list>
#include <boost/cstdint.hpp>

// Class declaration
class Hive;
class Acceptor;
class Connection;

class Hive : public boost::enable_shared_from_this<Hive> {
private:
    boost::asio::io_service m_io_service;
    boost::shared_ptr<boost::asio::io_service::work> m_work_ptr;
    volatile boost::uint32_t m_shutdown;

private:
    Hive(const Hive &rhs);

    Hive &operator=(const Hive &rhs);

public:
    Hive();

    virtual ~Hive();

    // Returns the io_service of this object.
    boost::asio::io_service &get_service();

    // Returns true if the Stop function has been called.
    bool has_stopped();

    // Polls the networking subsystem once from the current thread and
    // returns.
    void poll();

    // Runs the networking system on the current thread. This function blocks
    // until the networking system is stopped, so do not call on a single
    // threaded application with no other means of being able to call Stop
    // unless you code in such logic.
    void run();

    // Stops the networking system. All work is finished and no more
    // networking interactions will be possible afterwards until Reset is called.
    void stop();

    // Restarts the networking system after Stop as been called. A new work
    // object is created ad the shutdown flag is cleared.
    void reset();
};

class Acceptor: public boost::enable_shared_from_this<Acceptor>{
    friend class Hive;

private:
    boost::shared_ptr<Hive> m_hive;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::io_service::strand m_io_strand;
    boost::asio::deadline_timer m_timer;
    boost::int32_t m_timer_interval;
    volatile boost::uint32_t m_error_state;

private:
    Acceptor(const Acceptor& rhs);
    Acceptor& operator=(const Acceptor& rhs);
    void start_timer();
    void start_error(const boost::system::error_code& error);
    void dispatch_accept(boost::shared_ptr<Connection> connection);
    void handle_timer(const boost::system::error_code& error);
    void handle_accept(const boost::system::error_code& error, boost::shared_ptr<Connection> connection);

protected:
    Acceptor(boost::shared_ptr<Hive> hive);
    virtual ~Acceptor();

private:
    // Called when a connection has connected to the server. This function
    // should return true to invoke the connection's OnAccept function if the
    // connection will be kept. If the connection will not be kept, the
    // connection's Disconnect function should be called and the function
    // should return false.
    virtual bool on_accept(boost::shared_ptr<Connection> connection, const std::string& host, boost::uint16_t port) = 0;

    // Called on each timer event.
    virtual void on_timer(const boost::posix_time::time_duration& delta) = 0;

    // Called when an error is encountered. Most typically, this is when the
    // acceptor is being closed via the Stop function or if the Listen is
    // called on an address that is not available.
    virtual void on_error(const boost::system::error_code& error) = 0;

public:
    

};

#endif //ASIO_NETWORK_PROGRAMMING_WRAPPER_H
