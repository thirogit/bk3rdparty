//
// detail/impl/service_registry.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_IMPL_SERVICE_REGISTRY_IPP
#define ASIO_DETAIL_IMPL_SERVICE_REGISTRY_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "RCF/external/asio/asio/detail/config.hpp"
#include <boost/throw_exception.hpp>
#include "RCF/external/asio/asio/detail/service_registry.hpp"

#include "RCF/external/asio/asio/detail/push_options.hpp"

namespace asio {
namespace detail {

service_registry::service_registry(asio::io_service& o)
  : owner_(o),
    first_service_(0)
{
}

service_registry::~service_registry()
{
  // Shutdown all services. This must be done in a separate loop before the
  // services are destroyed since the destructors of user-defined handler
  // objects may try to access other service objects.
  asio::io_service::service* service = first_service_;
  while (service)
  {
    service->shutdown_service();
    service = service->next_;
  }

  // Destroy all services.
  while (first_service_)
  {
    asio::io_service::service* next_service = first_service_->next_;
    destroy(first_service_);
    first_service_ = next_service;
  }
}

void service_registry::init_key(asio::io_service::service::key& key,
    const asio::io_service::id& id)
{
  key.type_info_ = 0;
  key.id_ = &id;
}

bool service_registry::keys_match(
    const asio::io_service::service::key& key1,
    const asio::io_service::service::key& key2)
{
  if (key1.id_ && key2.id_)
    if (key1.id_ == key2.id_)
      return true;
  if (key1.type_info_ && key2.type_info_)
    if (*key1.type_info_ == *key2.type_info_)
      return true;
  return false;
}

void service_registry::destroy(asio::io_service::service* service)
{
  delete service;
}

asio::io_service::service* service_registry::do_use_service(
    const asio::io_service::service::key& key,
    factory_type factory)
{
  asio::detail::mutex::scoped_lock lock(mutex_);

  // First see if there is an existing service object with the given key.
  asio::io_service::service* service = first_service_;
  while (service)
  {
    if (keys_match(service->key_, key))
      return service;
    service = service->next_;
  }

  // Create a new service object. The service registry's mutex is not locked
  // at this time to allow for nested calls into this function from the new
  // service's constructor.
  lock.unlock();
  auto_service_ptr new_service = { factory(owner_) };
  new_service.ptr_->key_ = key;
  lock.lock();

  // Check that nobody else created another service object of the same type
  // while the lock was released.
  service = first_service_;
  while (service)
  {
    if (keys_match(service->key_, key))
      return service;
    service = service->next_;
  }

  // Service was successfully initialised, pass ownership to registry.
  new_service.ptr_->next_ = first_service_;
  first_service_ = new_service.ptr_;
  new_service.ptr_ = 0;
  return first_service_;
}

void service_registry::do_add_service(
    const asio::io_service::service::key& key,
    asio::io_service::service* new_service)
{
  if (&owner_ != &new_service->io_service())
    boost::throw_exception(invalid_service_owner());

  asio::detail::mutex::scoped_lock lock(mutex_);

  // Check if there is an existing service object with the given key.
  asio::io_service::service* service = first_service_;
  while (service)
  {
    if (keys_match(service->key_, key))
      boost::throw_exception(service_already_exists());
    service = service->next_;
  }

  // Take ownership of the service object.
  new_service->key_ = key;
  new_service->next_ = first_service_;
  first_service_ = new_service;
}

bool service_registry::do_has_service(
    const asio::io_service::service::key& key) const
{
  asio::detail::mutex::scoped_lock lock(mutex_);

  asio::io_service::service* service = first_service_;
  while (service)
  {
    if (keys_match(service->key_, key))
      return true;
    service = service->next_;
  }

  return false;
}

} // namespace detail
} // namespace asio

#include "RCF/external/asio/asio/detail/pop_options.hpp"

#endif // ASIO_DETAIL_IMPL_SERVICE_REGISTRY_IPP