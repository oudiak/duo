/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：Condition.h
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

#ifndef DUO_BASE_CONDITION_H
#define DUO_BASE_CONDITION_H

#include "Mutex.h"

#include <boost/noncopyable.hpp>
#include <pthread.h>
#include <errno.h>

namespace duo {
    class Condition : boost::noncopyable {
    public:
        explicit Condition(MutexLock& mutex) : mutex_(mutex) {
            pthread_cond_init(&pcond_, NULL);
        }

        ~Condition() {
            pthread_cond_destroy(&pcond_);
        }

        void wait() {
            pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
        }

        bool waitForSeconds(int seconds) {
            struct timespec abstime;

            clock_gettime(CLOCK_REALTIME, &abstime);
            abstime.tv_sec += seconds;
            return ETIMEDOUT ==
                pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
        }

        void notify() {
            pthread_cond_signal(&pcond_);
        }

        void notifyAll() {
            pthread_cond_broadcast(&pcond_);
        }

    private:
        MutexLock& mutex_;
        pthread_cond_t pcond_;
    };
}

#endif  // DUO_BASE_CONDITION_H
