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
#include <algorithm>
#include <iterator>
#include "SmartMatrix.h"

class ErrorMeasure {
    
    using Matrix = SmartMatrix;
    using ulong = unsigned long;
    
    ulong sum = 0;
    ulong trial_limit = 1000;
    
    std::queue <char> values;
    
    void CheckTrialLimit ();
    
public:
    
    long double GetHitRate () const;
    
    void AddSuccess ();
    void AddFailure ();
    
    void Evaluate (const Matrix& network_output, const Matrix& labels);
    
    void SetTrialLimit (ulong _limit);
    
    bool operator< (const long double& _other) const;
    bool operator> (const long double& _other) const;
    
    bool operator<= (const long double& _other) const;
    bool operator>= (const long double& _other) const;
};

long double ErrorMeasure::GetHitRate() const {
    return values.size() != 0 ? sum / static_cast <long double> (values.size()) : 0;
}

void ErrorMeasure::Evaluate(const Matrix& network_output, const Matrix& labels) {
    for (ulong i = 0; i < network_output.RowCount (); ++i) {
        auto& output_row = network_output.extractRow(i);
        auto& labels_row = labels.extractRow(i);
        
        auto out_max = std::max_element (output_row.begin(), output_row.end());
        auto lab_max = std::max_element (labels_row.begin(), labels_row.end());
        
        if (std::distance (output_row.begin(), out_max) == std::distance (labels_row.begin(), lab_max)) {
            AddSuccess ();
        }
        
        else {
            AddFailure ();
        }
    }
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

