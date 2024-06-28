#ifndef UNIQUE_PTR
#define UNIQUE_PTR

#include <utility>

template <typename T>
class UniquePtr
{
private:
    T* _p{nullptr};
public:
    UniquePtr() noexcept: _p{nullptr} {}  // {}：一致性初始化
    UniquePtr(std::nullptr_t) noexcept: UniquePtr() {}
    explicit UniquePtr(T* pointer) noexcept : _p{pointer} {}
    ~UniquePtr() noexcept {
        reset();
    }

    UniquePtr(const UniquePtr& other) noexcept = delete;
    UniquePtr& operator= (const UniquePtr & other) noexcept = delete;

    UniquePtr(UniquePtr && other) noexcept: _p{other.release()} {}
    UniquePtr& operator= (UniquePtr &&other) noexcept{
        if(this != &other) reset(other.release());
        return *this;
    }
    UniquePtr& operator= (std::nullptr_t) noexcept{
        reset();
        return *this;
    }

    explicit operator bool() const noexcept {
        return _p != nullptr;
    }
    T* get() const noexcept {
        return _p;
    }
    T& operator*() const noexcept {
        return *_p;
    }
    T* operator->() const noexcept {
        return _p;
    }
    void reset() noexcept {
        if(_p) {
            delete _p;
            _p = nullptr;
        }
    }
    void reset(T* new_pointer) noexcept {
        if (new_pointer != _p) {
            if (_p) delete _p;
            _p = new_pointer;
        }
    }
    T* release() noexcept {
        if(_p) {
            T* tmp = _p;
            _p = nullptr;
            return tmp;
        }
        return nullptr;
    }
};

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
} // make_unique<std::vector<int>>();

#endif //UNIQUE_PTR