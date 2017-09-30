#include "stdafx.h"
#include "asio_post_base.h"

IoServicePool::IoServicePool() :io_service_()
{
}
IoServicePool::~IoServicePool()
{
}
void IoServicePool::run(size_t thread_size)
{
	thread_size_ = thread_size > 0 ? thread_size : 1;
	work_ = std::make_shared<boost::asio::io_service::work>(io_service_);
	//// 只调用一次
	for (size_t i(0); i < thread_size_; i++)
	{
		thread_ptr thread(new boost::thread(
			boost::bind(&boost::asio::io_service::run, &io_service_)));
		threads_.push_back(thread);
	}
}
void IoServicePool::stop()
{
	// Explicitly stop all io_services.
	work_.reset();
	io_service_.stop(); //强制结束
	for (size_t i = 0; i < thread_size_; ++i)
	{
		threads_[i].reset();
	}
}
boost::asio::io_service& IoServicePool::get_io_service()
{
	// 返回ioservice
	return io_service_;
}