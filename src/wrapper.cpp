/* wrapper.cpp */
#include "../include/wrapper.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/interprocess/detail/atomic.hpp>

Hive::Hive()
: m_work_ptr(new boost::asio::io_service::work(m_io_service)), m_shutdown(0)
{
}

Hive::~Hive() {
}


boost::asio::io_service &Hive::get_service() {
    return m_io_service;
}

bool Hive::has_stopped() {
    return (boost::interprocess::ipcdetail::atomic_cas32(&m_shutdown,1,1) == 1);
}

void Hive::poll() {
  m_io_service.poll();
}

void Hive::run() {
  m_io_service.run();
}

void Hive::stop() {
   if(boost::interprocess::ipcdetail::atomic_cas32(&m_shutdown,1,1) == 0){
       m_work_ptr.reset();
       m_io_service.run();
       m_io_service.stop();
   }
}

void Hive::reset() {
    if(boost::interprocess::ipcdetail::atomic_cas32(&m_shutdown,1,1) == 0){
        m_io_service.reset();
        m_work_ptr.reset(new boost::asio::io_service::work(m_io_service));
    }
}








