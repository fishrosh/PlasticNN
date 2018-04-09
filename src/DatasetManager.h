/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DatasetManager.h
 * Author: kamil
 *
 * Created on March 4, 2018, 1:07 PM
 */

#ifndef DATASETMANAGER_H
#define DATASETMANAGER_H

#include <vector>
#include <utility>
#include "DatasetLoader.h"
#include "ImageProcessor.h"
#include "DataProcessor.h"
#include "SmartMatrix.h"

template <class output_t = double, class input_t = unsigned char>
class DatasetManager {
public:
        
    using DataProc = DataProcessor <output_t, input_t>;  
    using Loader = DatasetLoader<input_t>; 
    using ulong = unsigned long;
    
private:
        
    std::shared_ptr<DataProc> imgProc;
    std::shared_ptr<DataProc> labelProc;
    
    std::unique_ptr<input_t> images_ptr;
    std::unique_ptr<input_t> labels_ptr;
    
    SmartMatrix<output_t> in {10};
    SmartMatrix<output_t> out {10};
    
    std::vector<input_t> labels;
    
    long unit_size;
    long set_size;
 
public:
    
    void LoadDataset(Loader& images, Loader& labels);
    
    void LinkImgProc(std::weak_ptr<DataProc> _imgProc);
    void LinkLabelProc(std::weak_ptr<DataProc> _labelProc);
    
    ulong batch_size = 10;
    
    struct Sample {
        SmartMatrix<output_t>& in;
        SmartMatrix<output_t>& out;
        
        std::vector<input_t>& labels;
    };
    
    struct DatasetIterator {
        Sample operator* ();
        
        DatasetIterator& operator++();
        DatasetIterator& operator--();
        
        bool operator==(const DatasetIterator& it) const;
        bool operator!=(const DatasetIterator& it) const;
        
        DatasetIterator(long, DatasetManager& _dataset);
        
        DatasetIterator(const DatasetIterator&) = delete;
        DatasetIterator& operator=(const DatasetIterator&) = delete;
        
        DatasetIterator(DatasetIterator&&);
        DatasetIterator& operator=(DatasetIterator&&);
        
    private:
        
        DatasetManager& dataset;
        
        long index = 0;
        long unavailable_items = 0;
        
        std::shared_ptr<Randomize<output_t, int>> r;
        std::vector<bool> used_items;
    };
    
    DatasetIterator begin ();
    DatasetIterator end ();
};

template <class output_t, class input_t>
DatasetManager<output_t, input_t>::DatasetIterator::DatasetIterator(long _unavailable_items, DatasetManager& _dataset)
    : 
    r {std::make_shared<Randomize<output_t, int>>()},
    unavailable_items {_unavailable_items},
    dataset {_dataset},
    used_items (_dataset.set_size, false)
{
}
    
template <class output_t, class input_t>
DatasetManager<output_t, input_t>::DatasetIterator::DatasetIterator(DatasetIterator&& other) 
    : dataset {other.dataset}, 
    index {other.index}, 
    unavailable_items{other.unavailable_items}, 
    r{other.r},
    used_items {std::move(other.used_items)}
{
    
}

template <class output_t, class input_t>
void DatasetManager<output_t, input_t>::LoadDataset(Loader& images, Loader& labels) {
    
    if (images.GetSetSize() != labels.GetSetSize()) {
        return;
    }
    
    unit_size = images.GetUnitSize();
    set_size = images.GetSetSize();
    
    images_ptr = images.FetchAll();
    labels_ptr = labels.FetchAll();
}

template <class output_t, class input_t>
void DatasetManager<output_t, input_t>::LinkImgProc(std::weak_ptr<DataProc> _imgProc) {
    imgProc = _imgProc.lock();
    
    if (imgProc) {
        in = SmartMatrix<output_t> (imgProc->GetOutputLength());
    }
}

template <class output_t, class input_t>
void DatasetManager<output_t, input_t>::LinkLabelProc(std::weak_ptr<DataProc> _labelProc) {
    labelProc = _labelProc.lock();
    
    if (labelProc) {
        out = SmartMatrix<output_t> (labelProc->GetOutputLength());
    }
}

template <class output_t, class input_t>
typename DatasetManager<output_t, input_t>::DatasetIterator DatasetManager<output_t, input_t>::begin() {
    return std::move(++DatasetIterator {0, *this});
}

template <class output_t, class input_t>
typename DatasetManager<output_t, input_t>::DatasetIterator DatasetManager<output_t, input_t>::end() {
    return DatasetIterator {set_size + 1, *this};
}

template <class output_t, class input_t>
typename DatasetManager<output_t, input_t>::Sample DatasetManager<output_t, input_t>::DatasetIterator::operator *() {
    
    Sample output {dataset.in, dataset.out, dataset.labels};
    
    return output;
}

template <class output_t, class input_t>
typename DatasetManager<output_t, input_t>::DatasetIterator& DatasetManager<output_t, input_t>::DatasetIterator::operator ++() {
    dataset.in.reset();
    dataset.out.reset();
    dataset.labels.clear();
    
    for (ulong i = 0; i < dataset.batch_size; ++i) {
        if (++unavailable_items > dataset.set_size) {
            return *this;
        }
        
        std::vector<output_t> imageVec;
        std::vector<output_t> labelVec;

        r->SetRangeInt(0, dataset.set_size - unavailable_items);

        long offset = r->GetInt();
        long fOffset = 0;

        index = 0;

        while (fOffset != offset)
        {
            if (!used_items[index]) {
                ++fOffset;
            }

            ++index;
        }
        
        dataset.imgProc->operator ()(imageVec, dataset.images_ptr, index);
        dataset.labelProc->operator ()(labelVec, dataset.labels_ptr, index);
        
        dataset.in.concatenate(imageVec);
        dataset.out.concatenate(labelVec);
        
        dataset.labels.push_back(*(dataset.labels_ptr.get() + index));

        used_items[index] = true;
    }
    
    return *this;
}

template <class output_t, class input_t>
bool DatasetManager<output_t, input_t>::DatasetIterator::operator ==(const DatasetIterator& other) const {
    return this->unavailable_items == other.unavailable_items;
}

template <class output_t, class input_t>
bool DatasetManager<output_t, input_t>::DatasetIterator::operator !=(const DatasetIterator& other) const {
    return this->unavailable_items != other.unavailable_items;
}

#endif /* DATASETMANAGER_H */

