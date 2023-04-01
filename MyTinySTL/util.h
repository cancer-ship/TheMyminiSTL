#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_
// 这个文件包含一些通用工具，包括 move, forward, swap 等函数，以及 pair 等
#include <cstddef>

#include "type_traits.h"

namespace mystl
{
template <class T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept
{                                              //T&& 右值引用类型，用于表示传入的参数
          //typename有时需要指定嵌套的类型，用于指定这是一个类型
          //对move函数的实现用来实现移动语义
          //noexcept用来保证不会出错
          /*
这段代码是C++中的move函数的实现，用于实现移动语义。
下面是各个关键字的含义解释： 
1. typename：在模板编程中，有时需要指定嵌套类型，
但是由于模板的参数化会引起名称求值的问题，
因此需要加上typename关键字，以指示该标识符是一个类型，
并非变量或函数名。 
2. std::remove_reference：这是一个模板元函数，
用于移除类型T的引用修饰符，并定义一个名为type的内部类型，
表示移除引用修饰符后的类型。 
3. T&&：这是一个右值引用类型，用于表示传入的参数arg是可以被移动的数据，
此时arg只能绑定到右值，即不具有持久的身份，可以被临时创建的对象所引用。 
4. noexcept：这是一个异常说明符，用于指示函数是否抛出异常。如果标记为noexcept，
表示该函数不能抛出任何异常，如果抛出了异常，将会调用std::terminate()终止程序的执行。 
5. static_cast：这是一个类型转换运算符，用于将一个对象或表达式转换为指定类型。
在这里，用static_cast将右值引用类型的arg转换为移除引用修饰符后的类型，
这样就可以将右值参数传递到下一个函数中。
 综上，move函数的作用是将传入的参数使用右值引用的方式转移，
 以实现移动语义，同时提高代码的性能和效率
*/

  return static_cast<typename std::remove_reference<T>::type&&>(arg);
        //类型转换运算符，用于将一个对象或者表达式转化为指定的类型
}

template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept
{
    /*
    这是C++中的forward模板函数，用于实现完美转发，即将传入的参数以原本的引用类型转发到另一个函数中。各个关键字的含义如下：
     1. template：表示这是一个模板函数声明。 
     2. class T：表示函数的模板参数类型，这里使用了class和typename都可以。 
     3. typename std::remove_reference<T>::type：这是一个模板元函数，
     用于将类型T中的引用修饰符（如&和&&）移除，并定义一个名为type的内部类型，表示移除修饰符后的类型。 
     4. &arg：表示arg是一个左值引用类型的变量，这里使用了&符号。 
     5. T&&：表示返回值类型是一个右值引用类型的变量，这里使用了&&符号。 
     6. noexcept：表示函数是否抛出异常，如果标记为noexcept，则表示该函数会在不抛出异常的情况下终止执行，否则会调用std::terminate()函数终止程序执行。 
     7. static_cast<T&&>(arg)：将左值引用类型的arg转换为右值引用类型的变量，实现完美转发。 
     综上，forward函数的作用是将一个左值引用类型的参数（如函数的形参）
     以原本的引用类型转发到另一个函数中，实现完美转发，同时提高代码的性能和效率。
    */
  return static_cast<T&&>(arg);
}
template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
{
  static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
  return static_cast<T&&>(arg);
}
template <class Tp>
void swap(Tp& lhs, Tp& rhs)
{
  auto tmp(mystl::move(lhs));
  lhs = mystl::move(rhs);
  rhs = mystl::move(tmp);
}

template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
{
  for (; first1 != last1; ++first1, (void) ++first2)
    mystl::swap(*first1, *first2);
  return first2;
}

template <class Tp, size_t N>
void swap(Tp(&a)[N], Tp(&b)[N])
{
  mystl::swap_range(a, a + N, b);
}



//结构体模板
template <class Ty1, class Ty2>
struct pair
{
  typedef Ty1    first_type;
  typedef Ty2    second_type;

  first_type first;    // 保存第一个数据
  second_type second;  // 保存第二个数据

  // default constructiable
  template <class Other1 = Ty1, class Other2 = Ty2,
    typename = typename std::enable_if<
    std::is_default_constructible<Other1>::value &&
    std::is_default_constructible<Other2>::value, void>::type>
    constexpr pair()
    : first(), second()
  {
  }

  // implicit constructiable for this type
  template <class U1 = Ty1, class U2 = Ty2,
    typename std::enable_if<
    std::is_copy_constructible<U1>::value &&
    std::is_copy_constructible<U2>::value &&
    std::is_convertible<const U1&, Ty1>::value &&
    std::is_convertible<const U2&, Ty2>::value, int>::type = 0>
    constexpr pair(const Ty1& a, const Ty2& b)
    : first(a), second(b)
  {
  }

  // explicit constructible for this type
  template <class U1 = Ty1, class U2 = Ty2,
    typename std::enable_if<
    std::is_copy_constructible<U1>::value &&
    std::is_copy_constructible<U2>::value &&
    (!std::is_convertible<const U1&, Ty1>::value ||
     !std::is_convertible<const U2&, Ty2>::value), int>::type = 0>
    explicit constexpr pair(const Ty1& a, const Ty2& b)
    : first(a), second(b)
  {
  }

  pair(const pair& rhs) = default;
  pair(pair&& rhs) = default;

  // implicit constructiable for other type
  template <class Other1, class Other2,
    typename std::enable_if<
    std::is_constructible<Ty1, Other1>::value &&
    std::is_constructible<Ty2, Other2>::value &&
    std::is_convertible<Other1&&, Ty1>::value &&
    std::is_convertible<Other2&&, Ty2>::value, int>::type = 0>
    constexpr pair(Other1&& a, Other2&& b)
    : first(mystl::forward<Other1>(a)),
    second(mystl::forward<Other2>(b))
  {
  }

  // explicit constructiable for other type
  template <class Other1, class Other2,
    typename std::enable_if<
    std::is_constructible<Ty1, Other1>::value &&
    std::is_constructible<Ty2, Other2>::value &&
    (!std::is_convertible<Other1, Ty1>::value ||
     !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
    explicit constexpr pair(Other1&& a, Other2&& b)
    : first(mystl::forward<Other1>(a)),
    second(mystl::forward<Other2>(b))
  {
  }

  // implicit constructiable for other pair
  template <class Other1, class Other2,
    typename std::enable_if<
    std::is_constructible<Ty1, const Other1&>::value &&
    std::is_constructible<Ty2, const Other2&>::value &&
    std::is_convertible<const Other1&, Ty1>::value &&
    std::is_convertible<const Other2&, Ty2>::value, int>::type = 0>
    constexpr pair(const pair<Other1, Other2>& other)
    : first(other.first),
    second(other.second)
  {
  }

  // explicit constructiable for other pair
  template <class Other1, class Other2,
    typename std::enable_if<
    std::is_constructible<Ty1, const Other1&>::value &&
    std::is_constructible<Ty2, const Other2&>::value &&
    (!std::is_convertible<const Other1&, Ty1>::value ||
     !std::is_convertible<const Other2&, Ty2>::value), int>::type = 0>
    explicit constexpr pair(const pair<Other1, Other2>& other)
    : first(other.first),
    second(other.second)
  {
  }

  // implicit constructiable for other pair
  template <class Other1, class Other2,
    typename std::enable_if<
    std::is_constructible<Ty1, Other1>::value &&
    std::is_constructible<Ty2, Other2>::value &&
    std::is_convertible<Other1, Ty1>::value &&
    std::is_convertible<Other2, Ty2>::value, int>::type = 0>
    constexpr pair(pair<Other1, Other2>&& other)
    : first(mystl::forward<Other1>(other.first)),
    second(mystl::forward<Other2>(other.second))
  {
  }

  // explicit constructiable for other pair
  template <class Other1, class Other2,
    typename std::enable_if<
    std::is_constructible<Ty1, Other1>::value &&
    std::is_constructible<Ty2, Other2>::value &&
    (!std::is_convertible<Other1, Ty1>::value ||
     !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
    explicit constexpr pair(pair<Other1, Other2>&& other)
    : first(mystl::forward<Other1>(other.first)),
    second(mystl::forward<Other2>(other.second))
  {
  }
//都属于构造函数
  // copy assign for this pair
  pair& operator=(const pair& rhs)
  {
    if (this != &rhs)
    {
      first = rhs.first;
      second = rhs.second;
    }
    return *this;
  }

  // move assign for this pair
  pair& operator=(pair&& rhs)
  {
    if (this != &rhs)
    {
      first = mystl::move(rhs.first);
      second = mystl::move(rhs.second);
    }
    return *this;
  }

  // copy assign for other pair
  template <class Other1, class Other2>
  pair& operator=(const pair<Other1, Other2>& other)
  {
    first = other.first;
    second = other.second;
    return *this;
  }

  // move assign for other pair
  template <class Other1, class Other2>
  pair& operator=(pair<Other1, Other2>&& other)
  {
    first = mystl::forward<Other1>(other.first);
    second = mystl::forward<Other2>(other.second);
    return *this;
  }

  ~pair() = default;

  void swap(pair& other)
  {
    if (this != &other)
    {
      mystl::swap(first, other.first);
      mystl::swap(second, other.second);
    }
  }

};
template <class Ty1,class Ty2>
bool operator==(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs)
 {
    return lhs.first==rhs.first&&lhs.second==rhs.second;
 }
 template<class Ty1,class Ty2>
 bool operator<(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs)
  {
    return lhs.first==rhs.first&&lhs.second==rhs.second;
  }
//大于操作符是小于操作符的逆
template <class Ty1, class Ty2>
bool operator>(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
{
  return rhs < lhs;
}
template <class Ty1, class Ty2>
bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
{
  return !(lhs == rhs);
}
template <class Ty1, class Ty2>
bool operator<=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
{
  return !(rhs < lhs);
}

template <class Ty1, class Ty2>
bool operator>=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
{
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class Ty1, class Ty2>
void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs)
{
  lhs.swap(rhs);
}

// 全局函数，让两个数据成为一个 pair
template <class Ty1, class Ty2>
pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second)
{
  return pair<Ty1, Ty2>(mystl::forward<Ty1>(first), mystl::forward<Ty2>(second));
}

}


#endif