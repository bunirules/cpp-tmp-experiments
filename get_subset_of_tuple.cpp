#include <concepts>
#include <iostream>
#include <type_traits>
#include <utility>

// adapted from possible implementation section of https://en.cppreference.com/w/cpp/utility/integer_sequence
namespace detail 
{
template <class T, T I, T N, T... integers>
struct make_integer_sequence_helper {
    using type = typename make_integer_sequence_helper<T, I + 1, N, integers...,
                                                       I>::type;
};

template <class T, T N, T... integers>
struct make_integer_sequence_helper<T, N, N, integers...> {
    using type = std::integer_sequence<T, integers...>;
};
}  // namespace detail

template <size_t Begin, size_t End>
using make_index_sequence =
    typename detail::make_integer_sequence_helper<size_t, Begin, End>::type;

template <class T, T... ints, typename... Ts>
auto get(std::integer_sequence<T, ints...> seq, Ts... args) 
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return std::tuple{
        std::get<ints>(std::forward_as_tuple(std::forward<Ts>(args)...))...};
}

template <size_t start, size_t end, typename... Ts>
auto get(Ts... args) 
{
    return get(make_index_sequence<start, end>{}, args...);
}

template <size_t start, size_t end, typename... Ts>
void print_some_args(Ts... args) 
{
    std::apply(
        []<typename... Tss>(Tss... args)
        {
            ((std::cout << args << '\n'), ...);
            std::cout << '\n';
        },
        get<start, end>(args...));
}

int main() 
{
    std::string a = "this is a";
    std::string b = "this is b";
    int c = 26;
    std::string d = "this is d";
    std::string e = "this is e";
    print_some_args<1,4>(a, b, c, d, e);
}