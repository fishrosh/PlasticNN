/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ErrorMeasure.h
 * Author: kamil
 *
 * Created on March 8, 2018, 6:57 PM
 */

#ifndef ERRORMEASURE_H
#define ERRORMEASURE_H

#include <queue>

class ErrorMeasure {
    
    using ulong = unsigned long;
    
    ulong sum = 0;
    ulong trial_limit = 1000;
    
    std::queue <char> values;
    
    void CheckTrialLimit ();
    
public:
    
    long double GetHitRate () const;
    
    void AddSuccess ();
    void AddFailure ();
    
    void SetTrialLimit (ulong _limit);
    
    bool operator< (const long double& _other) const;
    bool operator> (const long double& _other) const;
    
    bool operator<= (const long double& _other) const;
    bool operator>= (const long double& _other) const;
};

long double ErrorMeasure::GetHitRate() const {
    return values.size() != 0 ? sum / static_cast <long double> (values.size()) : 0;
}

void ErrorMeasure::AddSuccess() {
    values.push(1);
    ++sum;
    
    CheckTrialLimit ();
}

void ErrorMeasure::AddFailure() {
    values.push(0);
    
    CheckTrialLimit ();
}

void ErrorMeasure::SetTrialLimit (ulong _limit) {
    trial_limit = _limit;
    CheckTrialLimit ();
}

bool ErrorMeasure::operator <(const long double& _other) const {
    return GetHitRate () < _other;
}

bool ErrorMeasure::operator >(const long double& _other) const {
    return GetHitRate () > _other;
}

bool ErrorMeasure::operator <=(const long double& _other) const {
    return !(*this > _other);
}

bool ErrorMeasure::operator >=(const long double& _other) const {
    return !(*this < _other);
}

void ErrorMeasure::CheckTrialLimit() {
    while (values.size() > trial_limit) {
        sum -= values.front();
        values.pop ();
    }
}

#endif /* ERRORMEASURE_H */

