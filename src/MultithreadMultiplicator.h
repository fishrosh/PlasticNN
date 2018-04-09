/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MultithreadMultiplicator.h
 * Author: kamil
 *
 * Created on March 21, 2018, 8:21 PM
 */

#ifndef MULTITHREADMULTIPLICATOR_H
#define MULTITHREADMULTIPLICATOR_H

#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

#include "SmartMatrix.h"

struct Job {
    using ulong = unsigned long;
    
    ulong i, j;
};

template <class Real = double>
class Multiplicator {
    
protected:
    
    static std::unique_ptr <Multiplicator<Real>> instance;
    
    Multiplicator ();
    Multiplicator (const Multiplicator&) = delete;
    Multiplicator& operator=(const Multiplicator&) = delete;
    
    friend std::unique_ptr <Multiplicator<Real>> std::make_unique <Multiplicator<Real>> ();
    
public:
    
    using Matrix = SmartMatrix<Real>;
    using ulong = unsigned long;
    
    static Multiplicator& Instance () {
        if (!instance) {
            instance = std::make_unique <Multiplicator<Real>> ();
        }
        
        return *instance;
    }
    
    void Multiply (Matrix const * left, Matrix const * right, Matrix * result);
    
private:
    
    bool AcquireJob ();
    bool JobsAvailable ();
    bool Finished ();
    void PerformJob ();
    
    void ThreadLoop ();
    
    std::condition_variable c_var;
    std::mutex _mutex;
    
    Matrix const * m_left;
    Matrix const * m_right;
    Matrix * m_result {nullptr};
    
    static thread_local Job current_job;
    static thread_local ulong local_job_number;
    static thread_local ulong local_done_count;
    static thread_local Matrix * local_result;
    static thread_local ulong local_joppo;
    static thread_local ulong local_M;
    static thread_local ulong local_N;
    
    std::mutex count_mutex;
    ulong job_count, done_count, dispatched_count;
    
    std::mutex job_mutex;
    Job job;
    
    int multi_count = 0;
};

template <class Real>
thread_local Job Multiplicator<Real>::current_job {};

template <class Real>
thread_local unsigned long Multiplicator<Real>::local_job_number {};

template <class Real>
thread_local unsigned long Multiplicator<Real>::local_done_count {};

template <class Real>
thread_local SmartMatrix <Real> * Multiplicator<Real>::local_result {};

template <class Real>
thread_local unsigned long Multiplicator<Real>::local_joppo {};

template <class Real>
thread_local unsigned long Multiplicator<Real>::local_M {};

template <class Real>
thread_local unsigned long Multiplicator<Real>::local_N {};

template <class Real>
std::unique_ptr <Multiplicator<Real>> Multiplicator<Real>::instance = std::make_unique <Multiplicator<Real>> ();

template <class Real>
Multiplicator<Real>::Multiplicator() {
    ulong concurrency = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 4;
    if (concurrency > 1) --concurrency;
    
    for (ulong i = 0; i < concurrency; ++i) {
        std::thread t(&Multiplicator<Real>::ThreadLoop, this);
        t.detach();
    }
}

template <class Real>
bool Multiplicator<Real>::JobsAvailable() {
    return !Finished () && job.i < m_result->size_m() && job.j < m_result->size_n();
}

template <class Real>
bool Multiplicator<Real>::Finished() {
    return job_count == done_count;
}

template <class Real>
bool Multiplicator<Real>::AcquireJob() {
    if (m_result == nullptr) return false;
    
    std::lock_guard <std::mutex> lock(job_mutex);
    if (JobsAvailable ()) {
        current_job = job;
        if (++job.i >= m_result->size_m()) {
            job.i = 0; ++job.j;
        }
        
        local_job_number = dispatched_count;
        local_done_count = done_count;
        local_result = m_result;
        local_joppo = multi_count;
        local_M = m_result->size_m();
        local_N = m_result->size_n();
        
        ++dispatched_count;
        return true;
    }
    
    return false;
}

template <class Real>
void Multiplicator<Real>::PerformJob() {
    Real& out = m_result->at(current_job.i, current_job.j) = 0;
    for (ulong m = 0; m < m_left->size_n(); ++m) {
        out += m_left->at(current_job.i, m) * m_right->at(m, current_job.j);
    }

    std::lock_guard <std::mutex> glock(count_mutex);
    ++done_count;
}

template <class Real>
void Multiplicator<Real>::Multiply(Matrix const * left, Matrix const * right, Matrix * result) {
    ++multi_count;
    {
        std::lock_guard <std::mutex> lock (job_mutex);
        
        job = Job { 0, 0 };

        m_left = left;
        m_right = right;
        m_result = result;

        job_count = m_result->size_m() * m_result->size_n();
        done_count = 0;
        dispatched_count = 0;
    }
    
    if (JobsAvailable ()) {
        c_var.notify_all();
    }
    
    while (AcquireJob ()) {
        PerformJob ();
    }
    
    while (!Finished()) {}
}

template <class Real>
void Multiplicator<Real>::ThreadLoop() {
    while (true) {
        std::unique_lock <std::mutex> lock (_mutex);
        c_var.wait(lock, std::bind(&Multiplicator<Real>::AcquireJob, this));
        lock.unlock ();
        
        do {
            PerformJob ();
        } while (AcquireJob());
    }
}

#endif /* MULTITHREADMULTIPLICATOR_H */

