#ifndef PROTON_CPP_ENDPOINT_H
#define PROTON_CPP_ENDPOINT_H

/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */
#include "proton/export.hpp"
#include "proton/connection.h"

namespace proton {

class handler;
class connection;
class transport;
class session;
class link;

/** endpoint is a base class for session, connection and link */
class endpoint
{
  public:
    /** state is a bit mask of state_bit values.
     *
     * A state mask is matched against an endpoint as follows: If the state mask
     * contains both local and remote flags, then an exact match against those
     * flags is performed. If state contains only local or only remote flags,
     * then a match occurs if any of the local or remote flags are set
     * respectively.
     *
     * @see connection::links, connection::sessions
     */
    typedef int state;

    /// endpoint state bit values @{
    PN_CPP_EXTERN static const int LOCAL_UNINIT;    ///< Local endpoint is un-initialized
    PN_CPP_EXTERN static const int REMOTE_UNINIT;   ///< Remote endpoint is un-initialized
    PN_CPP_EXTERN static const int LOCAL_ACTIVE;    ///< Local endpoint is active
    PN_CPP_EXTERN static const int REMOTE_ACTIVE;   ///< Remote endpoint is active
    PN_CPP_EXTERN static const int LOCAL_CLOSED;    ///< Local endpoint has been closed
    PN_CPP_EXTERN static const int REMOTE_CLOSED;   ///< Remote endpoint has been closed
    PN_CPP_EXTERN static const int LOCAL_MASK;      ///< Mask including all LOCAL_ bits (UNINIT, ACTIVE, CLOSED)
    PN_CPP_EXTERN static const int REMOTE_MASK;     ///< Mask including all REMOTE_ bits (UNINIT, ACTIVE, CLOSED)
    ///@}

    // TODO: condition, remote_condition, update_condition, get/handler

};

///@cond INTERNAL
template <class T, class D> class iter_base {
  public:
    typedef T value_type;

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    operator bool() const { return ptr_; }
    bool operator !() const { return !ptr_; }
    iter_base<T, D>& operator++() { static_cast<D*>(this)->advance(); return *this; }
    iter_base<T, D>& operator++(int) { iter_base<T, D> x(*this); ++(*this); return x; }
    bool operator==(const iter_base<T, D>& x) const { return ptr_ == x.ptr_; }
    bool operator!=(const iter_base<T, D>& x) const { return ptr_ != x.ptr_; }

  protected:
    explicit iter_base(T* p = 0, endpoint::state s = 0) : ptr_(p), state_(s) {}
    T* ptr_;
    endpoint::state state_;
};

template<class I> class range {
  public:
    typedef I iterator;

    explicit range(I begin = I(), I end = I()) : begin_(begin), end_(end) {}
    I begin() const { return begin_; }
    I end() const { return end_; }
  private:
    I begin_, end_;
};
///@endcond INTERNAL

///@ An iterator for a range of sessions.
class session_iterator : public iter_base<session, session_iterator> {
 public:
    explicit session_iterator(session* p = 0, endpoint::state s = 0) : iter_base(p, s) {}
  private:
    PN_CPP_EXTERN void advance();
  friend class iter_base<session, session_iterator>;
};

///@ A range of sessions.
typedef range<session_iterator> session_range;

///@ An iterator for a range of links.
class link_iterator : public iter_base<link, link_iterator> {
  public:
    explicit link_iterator(link* p = 0, endpoint::state s = 0) :
        iter_base(p, s), session_(0) {}
    explicit link_iterator(const link_iterator& i, session *ssn) :
        iter_base(i.ptr_, i.state_), session_(ssn) {}
  private:
    PN_CPP_EXTERN void advance();
    session* session_;
  friend class iter_base<link, link_iterator>;
};

///@ A range of links.
typedef range<link_iterator> link_range;

}

#endif  /*!PROTON_CPP_H*/
