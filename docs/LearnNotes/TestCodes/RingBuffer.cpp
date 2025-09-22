#include <mutex>
#include <iostream>
#include <vector>

class RingBuffer {
public:
    explicit RingBuffer(size_t size) 
        : buffer_(size), size_(size) {}

    bool Enqueue(const std::vector<char>& data) {
        std::unique_lock<std::mutex> lock(mtx_);
        if (data.size() > AvailableSpace()) return false;

        for (char c : data) {
            buffer_[tail_] = c;
            tail_ = (tail_ + 1) % size_;
        }
        count_ += data.size();
        return true;
    }

    bool Dequeue(std::vector<char>& out, size_t length) {
        std::unique_lock<std::mutex> lock(mtx_);
        if (length > count_) return false;

        out.clear();
        out.reserve(length);
        for (size_t i = 0; i < length; i++) {
            out.push_back(buffer_[head_]);
            head_ = (head_ + 1) % size_;
        }
        count_ -= length;
        return true;
    }

    void Clear() {
        std::unique_lock<std::mutex> lock(mtx_);
        head_ = tail_ = count_ = 0;
    }

private:
    size_t AvailableSpace() const { return size_ - count_; }

    std::vector<char> buffer_;
    size_t size_;
    size_t head_ {0};
    size_t tail_ {0};
    size_t count_ {0};
    mutable std::mutex mtx_;
};
