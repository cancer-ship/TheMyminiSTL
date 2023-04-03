#ifndef MYTINYSTL_CONSTRUCT_H_
#define MYTINYSTL_CONSTRUCT_H_

#include <new>

#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)  // unused parameter
#endif // _MSC_VER

namespace mystl
{

// construct 构造对象

template <class Ty>
void construct(Ty* ptr)
{
  ::new ((void*)ptr) Ty();
  /*::new: 这是C++中的一个运算符，用于动态地分配内存并返回指向分配的对象的指针。
  与new运算符不同，::new运算符只分配内存，但不会调用对象的构造函数。
  因此，使用::new来分配原始内存空间，然后使用placement new在该空间上构造对象。
((void*)ptr): 这是将指向ptr的指针转换为指向void类型的指针，即表示不确定的类型。
Ty(): 这是一个类型Ty的默认构造函数，用于在上面分配的内存位置上构造一个类型为Ty的对象。*/
}

template <class Ty1, class Ty2>
void construct(Ty1* ptr, const Ty2& value)
{
  ::new ((void*)ptr) Ty1(value);
}

template <class Ty, class... Args>
void construct(Ty* ptr, Args&&... args)
{
  ::new ((void*)ptr) Ty(mystl::forward<Args>(args)...);
}

// destroy 将对象析构

template <class Ty>
void destroy_one(Ty*, std::true_type) {}

template <class Ty>
void destroy_one(Ty* pointer, std::false_type)
{
  if (pointer != nullptr)
  {
    pointer->~Ty();
  }
}

template <class ForwardIter>
void destroy_cat(ForwardIter , ForwardIter , std::true_type) {}

template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type)
{
  for (; first != last; ++first)
    destroy(&*first);
}

template <class Ty>
void destroy(Ty* pointer)
{
  destroy_one(pointer, std::is_trivially_destructible<Ty>{});
}
/*
这段代码是C++中的模板函数，其目的是销毁一个对象并释放其占用的内存。

函数的参数是一个指向对象的指针Ty*。

函数内部调用了一个辅助函数destroy_one，
其第二个参数是一个bool类型的值，用于判断对象的析构函数是否是trivial的，
即是否是一个无操作的析构函数。

如果对象的析构函数是trivial的，即无需执行任何操作，那么就直接调用delete释放内存；
否则就调用对象的析构函数，再调用delete释放内存。
这样可以保证在销毁对象时，其占用的内存都能被正确地释放，避免出现内存泄漏的问题。*/


template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last)
{
  destroy_cat(first, last, std::is_trivially_destructible<
              typename iterator_traits<ForwardIter>::value_type>{});
}

} // namespace mystl

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // !MYTINYSTL_CONSTRUCT_H_