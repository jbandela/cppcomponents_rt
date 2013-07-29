//          Copyright John R. Bandela 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDE_GUARD_CPPCOMPONENTS_RT_HSTRING_HPP_a74fb375_59ea_45f3_80e8_2a0b2972062a
#define INCLUDE_GUARD_CPPCOMPONENTS_RT_HSTRING_HPP_a74fb375_59ea_45f3_80e8_2a0b2972062a


#include <string>
#include "implementation/implementation_interfaces.hpp"
#include "external/utf8_v2_3_4/source/utf8.h"
#include <algorithm>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace cppcomponents{
	namespace rt{

		namespace detail{
			template<class charT>
			struct native_to_string;

			template<int wcharsz>
			struct wstring_conversion;

			template<>
			struct wstring_conversion<2>{
				static std::string to_string(const std::wstring& w){
					std::string ret;
					utf8::utf16to8(w.begin(), w.end(), std::back_inserter(ret));
					return ret;
				}

				static std::wstring from_string(const std::string& s){
					std::wstring ret;
					utf8::utf8to16(s.begin(), s.end(), std::back_inserter(ret));
					return ret;
				}
			};	
			template<>
			struct wstring_conversion<4>{
				static std::string to_string(const std::wstring& w){
					std::string ret;
					utf8::utf32to8(w.begin(), w.end(), std::back_inserter(ret));
					return ret;
				}

				static std::wstring from_string(const std::string& s){
					std::wstring ret;
					utf8::utf8to32(s.begin(), s.end(), std::back_inserter(ret));
					return ret;
				}
			};

			template<>
			struct native_to_string<char>{
				static const std::string& to_string(const std::string& s){ return s; }
				static std::wstring to_wstring(const std::string& s){
					return wstring_conversion < sizeof(wchar_t) >::from_string(s);
				}
				static const std::string& to_native_string(const std::string& s){ return s; }
				static std::string to_native_string(const std::wstring& s){
					return wstring_conversion < sizeof(wchar_t) >::to_string(s);
				}

				static const char* empty_native_string(){ return ""; }


			};
			template<>
			struct native_to_string<wchar_t>{
				static std::string to_string(const std::wstring& s){ 
					return wstring_conversion < sizeof(wchar_t) >::to_string(s);
				}
				static const std::wstring& to_wstring(const std::wstring& s){
					return s;
				}
				static std::wstring to_native_string(const std::string& s){
					return wstring_conversion < sizeof(wchar_t) >::from_string(s);
				}

				static const std::wstring& to_native_string(const std::wstring& s){
					return s;
				}

				static const wchar_t* empty_native_string(){ return L""; }
			};

		}

    // Adapted from CxxReflect utility.hpp
    //                            Copyright James P. McNellis 2011 - 2013.                            //
    //                   Distributed under the Boost Software License, Version 1.0.                   //
    //     (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    //
    // Modified by John R. Bandela






       class hstring
    {
    public:

        typedef implementation::native_char_t           value_type;
        typedef std::size_t         size_type;
        typedef std::ptrdiff_t   difference_type;

        typedef value_type const& reference;
        typedef value_type const& const_reference;
        typedef value_type const* pointer;
        typedef value_type const* const_pointer;

        typedef pointer           iterator;
        typedef const_pointer     const_iterator;

        typedef std::reverse_iterator<iterator>       reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        hstring()
            : _value()
        {
        }

        hstring(const_pointer const s)
        {
			basic_cr_string < value_type, std::char_traits<value_type>> cr(s);
            _value = implementation::Implementation::CreateString(cr);
        }

		template <class T>
		explicit hstring(cppcomponents::basic_cr_string<implementation::native_char_t,T> crs)
			: _value(implementation::Implementation::CreateString(crs)){


		}
		template <class C>
			explicit hstring(const std::basic_string<C>& s)
			: _value(implementation::Implementation::CreateString(detail::native_to_string<implementation::native_char_t>::to_native_string(s))){


		}

		hstring(hstring const& other) : _value(implementation::Implementation::DuplicateString(other._value))
        {
        }

        hstring(hstring&& other)
            : _value(other._value)
        {
            other._value = nullptr;
        }

		hstring(const implementation::hstring_type& h) : _value(implementation::Implementation::DuplicateString(h))
		{
        }

        hstring(implementation::hstring_type&& h){
            _value = h;
            h = nullptr;
        }

        auto operator=(hstring other) -> hstring&
        {
            swap(other);
            return *this;
        }

        auto operator=(hstring&& other) -> hstring&
        {
            implementation::Implementation::DeleteString(_value);
            _value = other._value;
            other._value = nullptr;
            return *this;
        }

        auto operator=(implementation::hstring_type&& h) -> hstring&
        {
            _value = h;
            h = nullptr;
            return *this;
        }

        auto operator=(const std::wstring& w) -> hstring&
        {
			_value = implementation::Implementation::CreateString(detail::native_to_string<implementation::native_char_t>::to_native_string(w)); 
            return *this;
        }

        ~hstring()
        {
            implementation::Implementation::DeleteString(_value);
        }

        auto swap(hstring& other) -> void
        {
            std::swap(_value, other._value);
        }

        auto begin()  const -> const_iterator { return get_buffer_begin(); }
        auto end()    const -> const_iterator { return get_buffer_end();   }
        auto cbegin() const -> const_iterator { return begin();            }
        auto cend()   const -> const_iterator { return end();              }

        auto rbegin()  const -> const_reverse_iterator { return reverse_iterator(get_buffer_end());   }
        auto rend()    const -> const_reverse_iterator { return reverse_iterator(get_buffer_begin()); }
        auto crbegin() const -> const_reverse_iterator { return rbegin();                             }
        auto crend()   const -> const_reverse_iterator { return rend();                               }

        auto size()     const -> size_type { return static_cast<size_type>(end() - begin()); }
        auto length()   const -> size_type { return size();                                  }
        auto max_size() const -> size_type { return std::numeric_limits<size_type>::max();   }
        auto capacity() const -> size_type { return size();                                  }
        auto empty()    const -> bool      { return size() == 0;                             }

        auto operator[](size_type const n) const -> const_reference
        {
            return get_buffer_begin()[n];
        }

        auto at(size_type const n) const -> const_reference
        {
            if (n >= size())
                throw cross_compiler_interface::error_out_of_range();

            return get_buffer_begin()[n];
        }

        auto front() const -> const_reference { return *get_buffer_begin();     }
        auto back()  const -> const_reference { return *(get_buffer_end() - 1); }

        auto c_str() const -> const_pointer { return get_buffer_begin(); }
        auto data()  const -> const_pointer { return get_buffer_begin(); }

		auto to_native_string() const -> std::basic_string<implementation::native_char_t> {
			std::basic_string<implementation::native_char_t> ret(get_buffer_begin(), get_buffer_end());
			return ret;
		}
		auto to_wstring() const -> std::wstring { return detail::native_to_string<implementation::native_char_t>::to_wstring(to_native_string()); }
		auto to_string() const -> std::string { return detail::native_to_string<implementation::native_char_t>::to_string(to_native_string()); }



        // A reference proxy, returned by proxy(), that can be passed into a function expecting an
        // implementation::hstring_type*.  When the reference proxy is destroyed, it sets the value of the SmartHString
        // from which it was created.
        class reference_proxy
        {
        public:

            reference_proxy(hstring* const value)
                : _value(value), _proxy(value->_value)
            {
            }

            ~reference_proxy()
            {
                if (_value->_value == _proxy)
                    return;

                hstring new_string;
                new_string._value = _proxy;

                _value->swap(new_string);
            }

            operator implementation::hstring_type*() { return &_proxy; }

        private:

            // Note that this type is copyable though it is not intended to be copied, aside from
            // when it is returned from SmartHString::proxy().
            auto operator=(reference_proxy const&) -> reference_proxy&;

            implementation::hstring_type        _proxy;
            hstring* _value;
        };

        auto proxy()       -> reference_proxy { return reference_proxy(this);                          }
        auto value() const -> implementation::hstring_type         { return _value;                                         }
        auto release()     -> implementation::hstring_type         { implementation::hstring_type value(_value); _value = nullptr; return value; }

		friend auto operator==(hstring const& lhs, hstring const& rhs) -> bool
		{
			return compare(lhs, rhs) == 0;
		}
		friend auto operator+(hstring const& lhs, hstring const& rhs) -> hstring
		{
			return implementation::Implementation::ConcatenateStrings(lhs.value(), rhs.value());
		}

        friend auto operator<(hstring const& lhs, hstring const& rhs) -> bool
        {
            return compare(lhs, rhs) == -1;
        }

        friend auto operator!=(hstring const& lhs, hstring const& rhs) -> bool { return !(lhs == rhs); }
        friend auto operator> (hstring const& lhs, hstring const& rhs) -> bool { return   rhs <  lhs ; }
        friend auto operator>=(hstring const& lhs, hstring const& rhs) -> bool { return !(lhs <  rhs); }
        friend auto operator<=(hstring const& lhs, hstring const& rhs) -> bool { return !(rhs <  lhs); }

    private:

        auto get_buffer_begin() const -> const_pointer
        {
			const_pointer const result(implementation::Implementation::GetStringRawBuffer(_value).first);
            return result == nullptr ? get_empty_string() : result;
        }
        auto get_buffer_end() const -> const_pointer
        {
			auto rb = implementation::Implementation::GetStringRawBuffer(_value);
            std::uint32_t length = rb.second;
			const_pointer const first(rb.first);
            return first == nullptr ? get_empty_string() : first + length;
        }

        static auto get_empty_string() -> const_pointer
        {
            static const_pointer const value(detail::native_to_string<implementation::native_char_t>::empty_native_string());
            return value;
        }

        static auto compare(hstring const& lhs, hstring const& rhs) -> int
        {
            std::int32_t result = implementation::Implementation::CompareStrings(lhs._value, rhs._value);
            return result;
        }

        implementation::hstring_type _value;
    };
	}
	}



// allow hstring to be used in interface definitions
namespace cross_compiler_interface{
	template<>
	struct cross_conversion<cppcomponents::rt::hstring>{
		typedef cppcomponents::rt::hstring original_type;
		typedef cppcomponents::rt::implementation::hstring_type converted_type;
		static converted_type to_converted_type(const original_type& h){
			return h.value();
		}
		static  original_type to_original_type(cppcomponents::rt::implementation::hstring_type c){
			return cppcomponents::rt::hstring(c);
		}

	};

}

CROSS_COMPILER_INTERFACE_DEFINE_TYPE_INFORMATION(cppcomponents::rt::hstring);

#endif