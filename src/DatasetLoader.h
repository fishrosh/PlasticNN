/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DatasetLoader.h
 * Author: kamil
 *
 * Created on March 4, 2018, 1:24 PM
 */

#ifndef DATASETLOADER_H
#define DATASETLOADER_H

template <class input_t>
class DatasetLoader {
    
public:
    
    virtual std::unique_ptr<input_t> FetchItem() = 0;
    virtual std::unique_ptr<input_t> FetchAll() = 0;

    virtual long GetUnitSize() const = 0;
    virtual long GetSetSize() const = 0;
};

#endif /* DATASETLOADER_H */

