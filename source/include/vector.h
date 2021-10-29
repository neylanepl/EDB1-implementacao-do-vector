#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <algorithm>         // std::copy, std::equal, std::fill
#include <cassert>           // assert()
#include <cstddef>           // std::size_t
#include <exception>         // std::out_of_range
#include <initializer_list>  // std::initializer_list
#include <iostream>          // std::cout, std::endl
#include <iterator>          // std::advance, std::begin(), std::end(), std::ostream_iterator
#include <limits>            // std::numeric_limits<T>
#include <memory>            // std::unique_ptr

/// Sequence container namespace.
namespace sc {

/// Implements tha infrastrcture to support a bidirectional iterator.
template <class T>
class MyForwardIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    typedef MyForwardIterator self_type;     //!< Alias to iterator.
    typedef std::ptrdiff_t difference_type;  //!< Difference type used to calculated distance between iterators.
    typedef T value_type;                    //!< Value type the iterator points to.
    typedef T* pointer;                      //!< Pointer to the value type.
    typedef T& reference;                    //!< Reference to the value type.
    typedef const T& const_reference;        //!< Reference to the value type.
    typedef std::bidirectional_iterator_tag iterator_category;  //!< Iterator category.

   private:
    pointer m_ptr;  //!< The raw pointer.

   public:
    //=== [I] CONSTRUCTORS AND DETRUCTOR
    /**
     * @brief Construct a new My Forward Iterator object.
     */
    MyForwardIterator(pointer pt_ = nullptr) : m_ptr{pt_} {}
    /**
     * @brief Construct a new My Forward Iterator object.
     */
    MyForwardIterator(const MyForwardIterator&) = default;
    /**
     * @brief Destroy the My Forward Iterator object.
     */
    ~MyForwardIterator(void) = default;

    //=== [II] OPERATORS
    /**
     * @brief The operator equal - assign the left MyForwardIterator variable the right MyForwardIterator value.
     *
     * @return MyForwardIterator& The result of the expression.
     */
    MyForwardIterator& operator=(const MyForwardIterator&) = default;
    /**
     * @brief The unary indirection operator dereferences a pointer
     *
     * @return a value of the type from which the pointer's type is derived
     */
    reference operator*() const { return *m_ptr; }
    /**
     * @brief The operator postfix increment.
     *
     * @return MyForwardIterator& The result of the expression.
     */
    MyForwardIterator& operator++() {
        m_ptr++;
        return *this;
    }
    /**
     * @brief The operator prefix increment.
     *
     * @return MyForwardIterator The result of the expression.
     */
    MyForwardIterator operator++(int) {
        MyForwardIterator iterator = *this;
        ++(*this);
        return iterator;
    }
    /**
     * @brief The operator postfix decrement.
     *
     * @return MyForwardIterator& The result of the expression.
     */
    MyForwardIterator& operator--() {
        m_ptr--;
        return *this;
    }
    /**
     * @brief The operator prefix decrement.
     *
     * @return MyForwardIterator The result of the expression.
     */
    MyForwardIterator operator--(int) {
        MyForwardIterator iterator = *this;
        --(*this);
        return iterator;
    }
    /**
     * @brief The operator minus.
     *
     * @param obj Variable on the right side of the operation.
     * @return MyForwardIterator The result of the expression minus.
     */
    difference_type operator-(MyForwardIterator& obj) {
        if (m_ptr > obj.m_ptr) return std::distance(obj.m_ptr, m_ptr);
        return std::distance(m_ptr, obj.m_ptr);
    }
    /**
     * @brief The equality operator.
     *
     * @param other Variable on the right side of the operation.
     * @return true true Case the equality is true.
     * @return false false Case otherwise.
     */
    bool operator==(const MyForwardIterator& other) const { return m_ptr == other.m_ptr; }
    /**
     * @brief The inequality operator.
     *
     * @param other Variable on the right side of the operation.
     * @return true Case the inequality is true.
     * @return false Case otherwise.
     */
    bool operator!=(const MyForwardIterator& other) const { return !(*this == other); }
    /**
     * @brief The operator less than.
     *
     * @param other Variable on the right side of the operation.
     * @return true Case the MyForwardIterator on the left is less than the integer on the right.
     * @return false Case otherwise.
     */
    bool operator<(MyForwardIterator const& other) { return m_ptr < other.m_ptr; }

    //=== [III] Friend functions.
    /**
     * @brief The operation of sum between values ​​of type difference_type and difference_type.
     *
     * @param lhs Variable on the left side of the operation.
     * @param rhs Variable on the right side of the operation.
     * @return MyForwardIterator The result of the expression sum
     */
    friend MyForwardIterator operator+(difference_type lhs, MyForwardIterator rhs) {
        return MyForwardIterator(lhs + rhs.m_ptr);
    }
    /**
     * @brief The operation of sun between values ​​of type MyForwardIterator and difference_type.
     *
     * @param lhs Variable on the left side of the operation.
     * @param rhs Variable on the right side of the operation.
     * @return MyForwardIterator The result of the expression minus
     */
    friend MyForwardIterator operator+(MyForwardIterator lhs, difference_type rhs) {
        return MyForwardIterator(lhs.m_ptr + rhs);
    }
    /**
     * @brief The operation of minus between values ​​of type MyForwardIterator and difference_type.
     *
     * @param lhs Variable on the left side of the operation.
     * @param rhs Variable on the right side of the operation.
     * @return MyForwardIterator The result of the expression minus
     */
    friend MyForwardIterator operator-(MyForwardIterator lhs, difference_type rhs) {
        return MyForwardIterator(lhs.m_ptr - rhs);
    }
};

/// This class implements the ADT list with dynamic array.
/*!
 * sc::vector is a sequence container that encapsulates dynamic size arrays.
 *
 * The elements are stored contiguously, which means that elements can
 * be accessed not only through iterators, but also using offsets to
 * regular pointers to elements.
 * This means that a pointer to an element of a vector may be passed to
 * any function that expects a pointer to an element of an array.
 *
 * \tparam T The type of the elements.
 */
template <typename T>
class vector {
   public:
    using size_type = unsigned long;                    //!< The size type.
    using value_type = T;                               //!< The value type.
    using pointer = T*;                                 //!< Pointer to a value stored in the container.
    using reference = T&;                               //!< Reference to a value stored in the container.
    using const_reference = const T&;                   //!< Const reference to a value stored in the container.
    using iterator = MyForwardIterator<T>;              //!< The iterator.
    using const_iterator = MyForwardIterator<const T>;  //!< The const_iterator.

   private:
    size_type m_end;       //!< The list's current size (or index past-last valid element).
    size_type m_capacity;  //!< The list's storage capacity.
    pointer m_storage;     //!< The list's data storage area.

   public:
    //=== [I] SPECIAL MEMBERS (7 OF THEM)
    /**
     * @brief Construct a new vector object.
     *
     * @param new_cap Capacity of my new vector.
     */
    explicit vector(size_type new_cap = 0)
        : m_capacity{new_cap}, m_end{new_cap}, m_storage{new value_type[m_capacity]} {
        std::fill(m_storage, m_storage + m_capacity, value_type{});
    }
    /**
     * @brief Destroy the vector object.
     */
    virtual ~vector(void) {
        for (auto i = 0; i < static_cast<int>(m_end); i++) m_storage[i].~value_type();
        delete[] m_storage;
    }
    /**
     * @brief Construct a new vector object.
     *
     * @param other Another vector to construct a new vector identical to this one.
     */
    vector(const vector& other)
        : m_capacity{other.m_capacity}, m_end{other.m_end}, m_storage{new value_type[m_capacity]} {
        std::copy(other.cbegin(), other.cend(), this->begin());
    }
    /**
     * @brief Construct a new vector object.
     *
     * @param il Iinitializer List to construct a new vector.
     */
    vector(std::initializer_list<value_type> il)
        : m_capacity{il.size()}, m_end{il.size()}, m_storage{new value_type[m_capacity]} {
        std::copy(il.begin(), il.end(), m_storage);
    }
    /**
     * @brief Constructs the container with the contents of the range [first, last).
     *
     * @param first Pointer/iterator to the beginning of range.
     * @param last Pointer/iterator to the location just past the last valid value of the range.
     */
    template <typename InputItr>
    vector(InputItr first, InputItr last) {
        m_capacity = 0;
        for (auto i = first; i < last; i++) {
            m_capacity++;
        }

        m_end = m_capacity;
        m_storage = new value_type[m_capacity];

        auto i = 0;
        while (first != last) {
            m_storage[i] = *first;
            first++;
            i++;
        }
    }
    /**
     * @brief Implements the operator = that replaces the container's contents.
     * @param other Another container to use as data source.
     *
     * @return The vector with its updated content.
     */
    vector& operator=(const vector& other) {
        if (this != &other) {
            if (m_capacity != other.m_end) {
                this->~vector();
                m_storage = new value_type[other.m_end];
            }
            std::copy(other.m_storage, other.m_storage + other.m_end, m_storage);
        }
        m_capacity = m_end = other.m_end;
        return (*this);
    }
    /**
     * @brief Implements the operator = that replaces the container's contents.
     *
     * @param ilist List Initializer to use as data source.
     * @return The vector with its updated content.
     */
    vector& operator=(std::initializer_list<value_type> ilist) {
        this->~vector();

        m_capacity = m_end = ilist.size();
        m_storage = new value_type[m_capacity];

        std::copy(ilist.begin(), ilist.end(), m_storage);

        return *this;
    }

    //=== [II] ITERATORS (4)
    /**
     * @brief Return iterator to the first element.
     *
     * @return iterator Iterator to the first element.
     */
    iterator begin(void) { return iterator(&m_storage[0]); }
    /**
     * @brief Return iterator to the element following the last element.
     *
     * @return iterator Iterator to the element following the last element.
     */
    iterator end(void) { return iterator(&m_storage[m_end]); }
    /**
     * @brief Return constant iterator to the first element.
     *
     * @return const_iterator Constant iterator to the first element.
     */
    const_iterator cbegin(void) const { return const_iterator(&m_storage[0]); }
    /**
     * @brief Return constant iterator to the element following the last element.
     *
     * @return const_iterator Constant iterator to the element following the last element.
     */
    const_iterator cend(void) const { return const_iterator(&m_storage[m_end]); }

    //=== [III] Capacity (3)
    /**
     * @brief Return size of the vector.
     *
     * @return size_type Size of the vector.
     */
    size_type size(void) const { return m_end; }
    /**
     * @brief Return capacity of the vector.
     *
     * @return size_type Capacity of the vector.
     */
    size_type capacity(void) const { return m_capacity; }
    /**
     * @brief Checks if the vector is empty.
     *
     * @return true Case the vector is empty.
     * @return false Case otherwise.
     */
    bool empty(void) const { return m_end == 0; }
    /**
     * @brief Verify whether the container vector is full.
     *
     * @return true Case the container vector is full.
     * @return false Case otherwise.
     */
    bool full(void) const { return m_end == m_capacity; }

    //=== [IV] Modifiers
    /**
     * @brief Erases all elements from the list.
     */
    void clear(void) {
        for (auto i{0}; i < static_cast<int>(m_end); i++) m_storage[i].~value_type();
        m_end = 0;
    }
    /**
     * @brief Appends the given element value to the end of the container.
     * @param value the value of the element to append.
     */
    void push_back(const_reference value) {
        if (m_end == m_capacity) {
            reserve(m_capacity + (m_capacity / 2) + 1);
        }
        m_storage[m_end++] = value;
    }
    /**
     * @brief Removes the last element of the container, if it exists.
     */
    void pop_back(void) {
        if (empty())
            throw std::length_error("[vector::pop_back()]: não é possível remover um elemento de um vetor vazio.");

        m_storage[--m_end].~value_type();
    }
    /**
     * @brief Inserts element before pos.
     *
     * @param pos_ Iterator before which the content will be inserted.
     * @param value_ Element to be insert.
     * @return iterator Iterator pointing to the first element inserted.
     */
    iterator insert(iterator pos_, const_reference value_) {
        long int diff = std::distance(m_storage, &*pos_);
        long int i = m_end++ - 1;

        if (m_end > m_capacity) reserve(m_capacity + (m_capacity / 2) + 1);

        for (; i >= diff; i--) m_storage[i + 1] = m_storage[i];
        m_storage[diff] = value_;

        return &m_storage[diff];
    }
    /**
     * @brief Inserts element before pos.
     *
     * @param pos_ Constant iterator before which the content will be inserted.
     * @param value_ Element to be insert.
     * @return iterator Iterator pointing to the first element inserted.
     */
    iterator insert(const_iterator pos_, const_reference value_) {
        long int diff = std::distance(m_storage, &*pos_);
        long int i = m_end++ - 1;

        if (m_end > m_capacity) reserve(m_capacity + (m_capacity / 2) + 1);

        for (; i >= diff; i--) m_storage[i + 1] = m_storage[i];
        m_storage[diff] = value_;

        return &m_storage[diff];
    }
    /**
     * @brief Inserts elements of range [first, last) before pos.
     *
     * @tparam InputItr Type of pointer.
     * @param pos_ Iterator before which the content will be inserted.
     * @param first_ Pointer to first element to be insert.
     * @param last_ Pointer to one position after the last element to be insert.
     * @return iterator Iterator pointing to the first element inserted.
     */
    template <typename InputItr>
    iterator insert(iterator pos_, InputItr first_, InputItr last_) {
        long int len = std::distance(first_, last_);
        long int diff = std::distance(m_storage, &*pos_);
        long int i = m_end - 1;
        m_end += len;

        if (m_end > m_capacity) reserve(m_end + (m_capacity / 2) + 1);

        for (; i >= diff; i--) m_storage[i + len] = m_storage[i];

        i = diff;
        for (; i < (diff + len); i++) m_storage[i] = *(first_ + i - diff);

        return &m_storage[diff];
    }
    /**
     * @brief Inserts elements of range [first, last) before pos.
     *
     * @tparam InputItr Type of pointer.
     * @param pos_ Constant iterator before which the content will be inserted.
     * @param first_ Pointer to first element to be insert.
     * @param last_ Pointer to one position after the last element to be insert.
     * @return iterator Iterator pointing to the first element inserted.
     */
    template <typename InputItr>
    iterator insert(const_iterator pos_, InputItr first_, InputItr last_) {
        long int len = std::distance(first_, last_);
        long int diff = std::distance(m_storage, &*pos_);
        long int i = m_end - 1;
        m_end += len;

        if (m_end > m_capacity) reserve(m_end + (m_capacity / 2) + 1);

        for (; i >= diff; i--) m_storage[i + len] = m_storage[i];

        i = diff;
        for (; i < (diff + len); i++) m_storage[i] = *(first_ + i - diff);

        return &m_storage[diff];
    }
    /**
     * @brief Inserts elements from initializer list ilist before pos.
     *
     * @param pos_ Iterator before which the content will be inserted.
     * @param ilist_ Itializer list to insert the values from.
     * @return Iterator pointing to the first element inserted.
     */
    iterator insert(iterator pos_, const std::initializer_list<value_type>& ilist_) {
        return insert(pos_, ilist_.begin(), ilist_.end());
    }
    /**
     * @brief Inserts elements from initializer list ilist before pos.
     *
     * @param pos_ Constant iterator before which the content will be inserted.
     * @param ilist_ Itializer list to insert the values from.
     * @return Iterator pointing to the first element inserted.
     */
    iterator insert(const_iterator pos_, const std::initializer_list<value_type>& ilist_) {
        return insert(pos_, ilist_.begin(), ilist_.end());
    }
    /**
     * @brief Increase the capacity of the vector to a value that's greater or equal
     *        to new_cap. If new_cap is greater than the current capacity(), new storage
     *        is allocated, otherwise the method does nothing.
     * @param new_cap new capacity of the vector.
     */
    void reserve(size_type new_cap) {
        if (new_cap > m_capacity) {
            pointer temp = new value_type[new_cap];

            for (auto i{0}; i < static_cast<int>(m_end); i++) temp[i] = m_storage[i];

            this->~vector();
            m_storage = temp;
            m_capacity = new_cap;
        }
    }
    /**
     * @brief Requests the removal of unused capacity.
     */
    void shrink_to_fit(void) {
        if (m_capacity != m_end) {
            pointer temp = new value_type[m_end];

            for (auto i{0}; i < static_cast<int>(m_end); i++) temp[i] = m_storage[i];

            this->~vector();
            m_storage = temp;
            m_capacity = m_end;
        }
    }
    /**
     * @brief Replaces the contents with count copies of value value.
     * @param count_ the new size of the container.
     * @param value_ the value to initialize elements of the container with.
     */
    void assign(size_type count_, const_reference value_) {
        if (count_ >= m_capacity) {
            reserve(count_);
            m_end = count_;
            for (auto i{0}; i < static_cast<int>(m_end); i++) m_storage[i] = value_;
        } else {
            for (auto i{0}; i < static_cast<int>(count_); i++) m_storage[i] = value_;
            for (auto i{count_}; i < m_end; i++) m_storage[i].~value_type();
            m_end = count_;
        }
    }
    /**
     * @brief Replaces the contents with copies of those in the range [first, last).
     * @param first Pointer/iterator to the beginning of range where replaces the contents.
     * @param last Pointer/iterator to the location just past the last valid value of the range where replaces the
     *             contents.
     */
    template <typename InputItr>
    void assign(InputItr first, InputItr last) {
        auto diff = std::distance(first, last);

        if (diff >= m_capacity) {
            reserve(diff);
            m_end = diff;
            for (auto i = 0; i < m_end; i++) m_storage[i] = first[i];
        } else {
            for (auto i = 0; i < diff; i++) m_storage[i] = first[i];
            for (auto i = diff; i < m_end; i++) m_storage[i].~value_type();
            m_end = diff;
        }
    }
    /**
     * @brief Replaces the contents with the elements from the initializer list ilist.
     * @param ilist initializer list to copy the values from.
     */
    void assign(const std::initializer_list<value_type>& il) { assign(il.begin(), il.end()); }
    /**
     * @brief Erase an element in vector.
     *
     * @param pos Constant pointer to element to be erased.
     * @return iterator Pointer to 'new' element in position of element erased.
     */
    iterator erase(const_iterator pos) {
        long int diff = std::distance(m_storage, &*pos);
        long int i = diff;
        m_end--;

        for (; i < static_cast<long int>(m_end); i++) m_storage[i] = m_storage[i + 1];

        m_storage[m_end].~value_type();

        return &m_storage[diff];
    }
    /**
     * @brief Erase an element in vector.
     *
     * @param pos Pointer to element to be erased.
     * @return iterator Pointer to 'new' element in position of element erased.
     */
    iterator erase(iterator pos) {
        long int diff = std::distance(m_storage, &*pos);
        long int i = diff;
        m_end--;

        for (; i < static_cast<long int>(m_end); i++) m_storage[i] = m_storage[i + 1];

        m_storage[m_end].~value_type();

        return &m_storage[diff];
    }
    /**
     * @brief Erase elements in range [first, last) inside vector.
     *
     * @param first Pointer to first element to be erased.
     * @param last Pointer to one position after the last element to be erased.
     * @return iterator Pointer to the 'new' element in position of first element erased.
     */
    iterator erase(iterator first, iterator last) {
        long int len = std::distance(first, last);
        long int diff = std::distance(m_storage, &*first);
        long int i = diff;
        m_end -= len;

        for (; i < static_cast<long int>(m_end); i++) m_storage[i] = m_storage[i + len];

        i = m_end;
        for (; i < static_cast<long int>(m_end) + len; i++) m_storage[i].~value_type();

        return &m_storage[diff];
    }
    /**
     * @brief Erase elements in range [first, last) inside vector.
     *
     * @param first Constant pointer to first element to be erased.
     * @param last Constant pointer one position after the last element to be erased.
     * @return iterator Pointer to the 'new' element in position of first element erased.
     */
    iterator erase(const_iterator first, const_iterator last) {
        long int len = std::distance(first, last);
        long int diff = std::distance(m_storage, &*first);
        long int i = diff;
        m_end -= len;

        for (; i < static_cast<long int>(m_end); i++) m_storage[i] = m_storage[i + len];

        i = m_end;
        for (; i < static_cast<long int>(m_end) + len; i++) m_storage[i].~value_type();

        return &m_storage[diff];
    }

    //=== [V] Element access (10)
    /**
     * @brief Return the constant last-1 element of the list.
     *
     * @return const_reference The constant last-1 element of the list.
     */
    const_reference back(void) const {
        if (empty()) {
            throw std::length_error("[vector::back()]: vetor vazio.");
        }

        return m_storage[m_end - 1];
    }
    /**
     * @brief Return the constant first element of the list.
     *
     * @return const_reference The constant first element of the list.
     */
    const_reference front(void) const {
        if (empty()) {
            throw std::length_error("[vector::front()]: vetor vazio.");
        }

        return m_storage[0];
    }
    /**
     * @brief Returns the element at the end of the list.
     *
     * @return reference The element at the end of the list.
     */
    reference back(void) { return m_storage[m_end - 1]; }
    /**
     * @brief Returns the element at the beginning of the list.
     *
     * @return reference The first element of the list.
     */
    reference front(void) { return m_storage[0]; }
    /**
     * @brief Returns a pointer to the memory array used internally by the container to store its owned elements.
     *
     * @return pointer A pointer to the memory array.
     */
    pointer data(void) { return m_storage; }
    /**
     * @brief Returns a constant pointer to the memory array used internally by the container to store its owned
     * elements.
     *
     * @return const_reference A constant pointer to the memory array.
     */
    const_reference data(void) const { return m_storage; }
    /**
     * @brief Implements the operator [].
     * @param position index of the element to be accessed.
     * @return constant reference to the requested element.
     */
    const_reference operator[](size_type position) const { return m_storage[position]; }
    /**
     * @brief Implements the operator [].
     * @param position index of the element to be accessed.
     * @return reference to the requested element.
     */
    reference operator[](size_type position) { return m_storage[position]; }
    /**
     * @brief Returns element at index `position`.
     * @param position index of the element that should be returned.
     * @return the element of the index `position`.
     */
    const_reference at(size_type position) const {
        if (position >= m_end) throw std::out_of_range("[vector::at()]: tentativa de leitura fora do vetor.");

        return m_storage[position];
    }
    /**
     * @brief Returns the element's address at index `position`, for eventual content alteration.
     * @param position index of the element in container.
     * @return the address of the value at index `position`.
     */
    reference at(size_type position) {
        if (position >= m_end) throw std::out_of_range("[vector::at()]: tentativa de leitura fora do vetor.");

        return m_storage[position];
    }

    //=== [VI] Friend functions.
    /**
     * @brief Prints vector data (stored elements).
     *
     * @param os_ Stream to print stored elements.
     * @param v_ Vector to print.
     * @return std::ostream& Stream with vector printed elements.
     */
    friend std::ostream& operator<<(std::ostream& os_, const vector<value_type>& v_) {
        os_ << "{ ";
        for (auto i{0u}; i < v_.m_capacity; ++i) {
            if (i == v_.m_end) os_ << "| ";
            os_ << v_.m_storage[i] << " ";
        }
        os_ << "}, m_end=" << v_.m_end << ", m_capacity=" << v_.m_capacity;

        return os_;
    }
    /**
     * @brief Swap two elements in vector.
     *
     * @param first_ First element to swap.
     * @param second_ Second element to swap.
     */
    friend void swap(vector<value_type>& first_, vector<value_type>& second_) {
        // Swap each member of the class.
        std::swap(first_.m_end, second_.m_end);
        std::swap(first_.m_capacity, second_.m_capacity);
        std::swap(first_.m_storage, second_.m_storage);
    }
};  // class vector.

//=== [VII] Operators (2)
/**
 * @brief Checks if the contents of lhs and rhs are equal, that is, they have the same number of
 *        elements and each element in lhs compares equal with the element in rhs at the same position.
 * @param lhs vector whose content is compared with `rhs`.
 * @param rhs vector whose content is compared with `lhs`.
 * @return true if the contents of the vectors are equal, false otherwise.
 */
template <typename T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (auto i = 0; i < lhs.size(); i++) {
        if (lhs.at(i) != rhs.at(i)) {
            return false;
        }
    }

    return true;
}
/**
 * @brief Checks if the contents of lhs and rhs are not equal, that is, they have the same number of
 *        elements and each element in lhs compares not equal with the element in rhs at the same position.
 * @param lhs vector whose content is compared with `rhs`.
 * @param rhs vector whose content is compared with `lhs`.
 * @return true if the contents of the vectors are not equal, false otherwise.
 */
template <typename T>
bool operator!=(const vector<T>& lhs, const vector<T>& rhs) {
    if (lhs.size() != rhs.size()) {
        return true;
    }

    for (auto i = 0; i < lhs.size(); i++) {
        if (lhs.at(i) != rhs.at(i)) {
            return true;
        }
    }

    return false;
}

}  // namespace sc.
#endif
