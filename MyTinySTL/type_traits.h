
#ifndef MYTINYSTL_TYPE_TRAITS_H_
#define MYTINYSTL_TYPE_TRAITS_H_
//条件编译的使用，通常需要在尾部添加endif
#include <type_traits>
namespace mystl
{
template <class T, T v>
struct m_integral_constant
{
  static constexpr T value = v;
//静态变量要在类外进行初始化，但是static constexpr可以在内部直接初始化
};

template <bool b>
using m_bool_constant = m_integral_constant<bool, b>;
//这个using是用来给类型取别名的
typedef m_bool_constant<true>  m_true_type;
typedef m_bool_constant<false> m_false_type;

/*****************************************************************************************/
// type traits

// is_pair

// --- forward declaration begin
template <class T1, class T2>
struct pair;
// --- forward declaration end

template <class T>
struct is_pair : mystl::m_false_type {};

template <class T1, class T2>
struct is_pair<mystl::pair<T1, T2>> : mystl::m_true_type {};

} // namespace mystl

#endif // !MYTINYSTL_TYPE_TRAITS_H_

