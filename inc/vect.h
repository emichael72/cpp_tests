#ifndef VEC_H
#define VEC_H

#include <string.h>
#include <typeinfo>

template <typename T>
class Vector
{

public:
    Vector() {};
    ~Vector()
    {
        if (m_data)
        {
            delete m_data;
            m_data = nullptr;
        }
    }

    // copy constructor
    Vector(const Vector &other)
        : m_data(nullptr), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        if (m_capacity > 0)
        {
            m_data = new T[m_capacity];
            for (int indx = 0; indx < m_size; indx++)
                m_data[indx] = other.m_data[indx];
        }
    }

    size_t size() const { return m_size; };
    size_t capacity() const { return m_capacity; };

    void reserve(int new_cap)
    {
        if (m_capacity >= new_cap)
            return;

        resize_capacity(new_cap);
    }

    bool empty() const { return m_size == 0; }
    void push_back(const T &val)
    {
        // no need for allocation
        if (m_capacity > m_size)
        {
            m_data[m_size] = val;
            m_size++;
            return;
        }

        // allocate tmp vector and populate with cur vector
        resize_capacity(m_size + 1);

        m_data[m_size] = val;
        m_size++;
    }

    T pop_back()
    {
        // note if empty -> UB
        T var = m_data[m_size - 1];

        m_size--;
        return var;
    }

    // iterators
    T* begin() { return m_data; }
    T* end()   { return &m_data[m_size]; }

    // move
    Vector(Vector &&other)
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        // pointers flipping (note: do we have to modify other?)
        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;
    }
    Vector &operator=(Vector &&other)
    {
        // full move  (fre the old, point to the new, return the new.
        if (this != &other)
        {
            delete[] m_data;
            m_data = other.m_data;
            m_capacity = other.m_capacity;
            m_size = other.m_size;

            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
        }

        return *this;
    }

    // [] overloads
    T &operator[](size_t index) { return m_data[index]; }
    const T &operator[](size_t index) const { return m_data[index]; }

    bool operator==(const Vector &other) const { return are_equal(other); }
    bool operator!=(const Vector &other) const { return !(*this == other); }

private:
    void resize_capacity(int new_cap)
    {
        T *tmp_ptr = new T[new_cap]; // todo: potential new without delete on exception
        m_capacity = new_cap;

        if (m_data)
        {
            for (int indx = 0; indx < m_size; indx++)
                tmp_ptr[indx] = m_data[indx];
            delete[] m_data;
        }

        m_data = tmp_ptr;
    }

    bool are_equal(const Vector &other) const
    {
        if (m_size != other.m_size)
            return false;
        if (m_size == 0)
            return true; // local and other are empty

        for (int indx = 0; indx < m_size; indx++)
        {
            if (m_data[indx] != other[indx])
                return false;
        }

        return true;
    }

    T *m_data = nullptr;
    int m_size = 0;
    int m_capacity = 0; // initial capacity
};

#endif // VEC_H
