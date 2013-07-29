//          Copyright John R. Bandela 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDE_GUARD_CPPCOMPONENTS_RT_HPP_8aeaa1fc_2b3a_49b7_b6d4_8905d088b366

#define INCLUDE_GUARD_CPPCOMPONENTS_RT_HPP_8aeaa1fc_2b3a_49b7_b6d4_8905d088b366
#include <cppcomponents/cppcomponents.hpp>
#include "hstring.hpp"

namespace cppcomponents{

	namespace rt{

		// Implementation of object count 
		inline error_code dll_can_unload_now(){
			auto count = cross_compiler_interface::object_counter::get().get_count();
			return count == 0 ? 0 : 1;
		}

		struct initializer{
			initializer(std::int32_t rinit = 1){
				implementation::Implementation::Initialize(rinit);

			}
			~initializer(){
				implementation::Implementation::Uninitialize();
			}


		};

		typedef std::int32_t TrustLevel;
		namespace detail{

			// A custom_cross_function for GetIids. 
			template<class Iface, int Id>
			struct get_iids_cross_function
				: public cross_compiler_interface::custom_cross_function<Iface, Id, std::vector<cross_compiler_interface::uuid_base>(), cross_compiler_interface::error_code(cross_compiler_interface::portable_base*, std::uint32_t*, cross_compiler_interface::uuid_base**),
				get_iids_cross_function<Iface, Id> >
			{
				std::vector<cross_compiler_interface::uuid_base*> call_vtable_function()const{
					portable_base* r = 0;
					std::uint32_t count = 0;
					cross_compiler_interface::uuid_base* piids = 0;
					auto ret = this->get_vtable_fn()(this->get_portable_base(), &count, &piids);
					std::unique_ptr<decltype(piids)> uptr(piids, ::CoTaskMemFree);
					if (ret < 0){
						this->exception_from_error_code(ret);
					}
					if (!piids) {
						throw cross_compiler_interface::error_pointer();
					}
					return std::vector<cross_compiler_interface::uuid_base*>(piids, piids + count);
				}
				template<class F>
				static cross_compiler_interface::error_code vtable_function(F f, cross_compiler_interface::portable_base* p, std::uint32_t* pcount, cross_compiler_interface::uuid_base** iids){
					try{
						if (!iids || *pcount){
							return cross_compiler_interface::error_pointer::ec;
						}
						std::vector<cross_compiler_interface::uuid_base> v = f();
						if (v.empty()){
							*pcount = 0;
							*iids = nullptr;
						}
						auto piids = static_cast<cross_compiler_interface::uuid_base*>(::CoTaskMemAlloc(v.size()*sizeof(v[0])));
						if (!piids){
							*pcount = 0;
							return cross_compiler_interface::error_out_of_memory::ec;
						}
#ifdef _MSC_VER
						std::copy(v.begin(), v.end(), stdext::checked_array_iterator<decltype(piids)>(piids, v.size()));
#else
						std::copy(v.begin(), v.end(), piids);
#endif
						auto p = piids;
						for (const auto& i : v){
							*p++ = i;
						}
						*iids = piids;
						*pcount = v.size();


					}
					catch (std::exception& e){
						return cross_compiler_interface::general_error_mapper::error_code_from_exception(e);
					}
					return 0;
				}

				template<class F>
				void operator=(F f){
					this->set_function(f);

				}

				template<class T>
				get_iids_cross_function(T t) : get_iids_cross_function::base_t(t){}



			};
		}

		// IInspectable
		struct InterfaceInspectable : public cppcomponents::define_interface<cppcomponents::uuid<0xAF86E2E0, 0xB12D, 0x4c6a, 0x9C5A, 0xD7AA65101E90>>{
			template<class T>
			struct Interface : public cross_compiler_interface::define_unknown_interface<T, cppcomponents::uuid<0xAF86E2E0, 0xB12D, 0x4c6a, 0x9C5A, 0xD7AA65101E90>, cppcomponents::InterfaceUnknown::Interface>{

				detail::get_iids_cross_function<Interface, 0> GetIids;
				cross_compiler_interface::cross_function<Interface, 1, implementation::hstring_type()> GetRuntimeClassName;
				cross_compiler_interface::cross_function<Interface, 2, TrustLevel()> GetTrustLevel;


				CROSS_COMPILER_INTERFACE_HELPER_DEFINE_INTERFACE_CONSTRUCTOR_INTROSPECTION_NO_METHODS(InterfaceInspectable);
				Interface() : GetIids(this), GetRuntimeClassName(this), GetTrustLevel(this){}

			};
		};

		template<class TUUID, class Base = InterfaceInspectable>
		struct define_rt_interface : public define_interface<TUUID, Base>{};


		// Implement interfaces derived from IInspectable and provide implementations of IInspectable Methods
		template<class Derived, class... Interfaces>
		struct implement_inspectable_interfaces{

		private:



			template<class First, class... Rest>
			struct helper{


				template<class T>
				static void set_mem_functions(T* t){
					auto p = t->template get_implementation<First>();
					p->GetIids.template set_mem_fn<implement_inspectable_interfaces,
						&implement_inspectable_interfaces::GetIids>(t);
					p->GetRuntimeClassName.template set_mem_fn<implement_inspectable_interfaces,
						&implement_inspectable_interfaces::GetRuntimeClassName>(t);
					p->GetTrustLevel.template set_mem_fn<implement_inspectable_interfaces,
						&implement_inspectable_interfaces::GetTrustLevel>(t);


					helper<Rest...>::set_mem_functions(t);
				}

				static void add_iids(std::vector<cross_compiler_interface::uuid_base>& v){
					auto& u = typename First::uuid_type::get();
					v.push_back(u);

					helper<Rest...>::add_iids(v);
				}

			};
			template<class First>
			struct helper<First>{
				template<class T>
				static void set_mem_functions(T* t){
					auto p = t->template get_implementation<First>();
					p->GetIids.template set_mem_fn<implement_inspectable_interfaces,
						&implement_inspectable_interfaces::GetIids>(t);
					p->GetRuntimeClassName.template set_mem_fn<implement_inspectable_interfaces,
						&implement_inspectable_interfaces::GetRuntimeClassName>(t);
					p->GetTrustLevel.template set_mem_fn<implement_inspectable_interfaces,
						&implement_inspectable_interfaces::GetTrustLevel>(t);

				}


				static void add_iids(std::vector<cross_compiler_interface::uuid_base>& v){
					auto& u = typename First::uuid_type::get();
					v.push_back(u);
				}

			};



		public:
			std::vector<cross_compiler_interface::uuid_base> GetIids(){
				std::vector<cross_compiler_interface::uuid_base> v;
				helper<Interfaces...>::add_iids(v);
				return v;
			}

			implementation::hstring_type GetRuntimeClassName(){
				return typename Derived::runtime_class_t::get_runtime_class_name().release();
			}

			TrustLevel GetTrustLevel(){
				return Derived::GetTrustLevel();
			}

			implement_inspectable_interfaces(){
				helper<Interfaces...>::set_mem_functions(static_cast<Derived*>(this));
			}
		};


		// IActivationFactory
		struct InterfaceActivationFactory : public define_rt_interface < cppcomponents::uuid<0x00000035, 0x0000, 0x0000, 0xC000, 0x000000000046>>{

			cppcomponents::use<cppcomponents::InterfaceUnknown> ActivateInstance();

			CPPCOMPONENTS_CONSTRUCT(InterfaceActivationFactory, ActivateInstance);
		};





		namespace detail{

			template<class FactoryInterface, class StaticInterface>
			struct check_activation_factory_present{
			};



			template<class... StaticInterfaces>
			struct check_activation_factory_present<cppcomponents::factory_interface<InterfaceActivationFactory>, cppcomponents::static_interfaces<StaticInterfaces...> >{
				typedef cppcomponents::static_interfaces<StaticInterfaces...> type;
			};

			template<class FactoryInterface, class... StaticInterfaces>
			struct check_activation_factory_present < cppcomponents::factory_interface<FactoryInterface>, cppcomponents::static_interfaces<StaticInterfaces...> >{
				typedef cppcomponents::static_interfaces<StaticInterfaces..., InterfaceActivationFactory> type;
			};


			struct default_interfaces{
				typedef cppcomponents::object_interfaces<InterfaceInspectable> oi;
				typedef cppcomponents::factory_interface<InterfaceActivationFactory> fi;
				typedef cppcomponents::static_interfaces<> si;
			};

		}


		// Defines a winrt_runtime class
		template<hstring(*pfun_runtime_class_name)(), class... I>
		struct rt_runtime_class{

			typedef cppcomponents::detail::to_oi_fi_si<detail::default_interfaces, I...> oifisi;
			typedef typename detail::check_activation_factory_present<typename oifisi::fi, typename oifisi::si>::type si_t;

			typedef cppcomponents::detail::runtime_class_helper<hstring, pfun_runtime_class_name, typename oifisi::oi, typename oifisi::fi, si_t> helper;


			// Make sure that activation factory is present, if not add to list as static interface
			typedef typename helper::type type;
		};

		namespace detail{

			template<class Derived, class RCB>
			struct inherit_from_implement_inspectable{};

			template < class Derived, hstring(*pfun_runtime_class_name)(), class DefaultInterface, class FactoryInterface, class StaticInterface, class... OtherInterfaces>
			struct inherit_from_implement_inspectable<Derived, cppcomponents::runtime_class_base<hstring, pfun_runtime_class_name, DefaultInterface, FactoryInterface, StaticInterface, OtherInterfaces...>>{
				typedef implement_inspectable_interfaces<Derived, DefaultInterface, OtherInterfaces...> object_type;

				template<class F, class S>
				struct helper{
					typedef implement_inspectable_interfaces<Derived, F, S> type;
				};
				template<class F, class... S>
				struct helper < F, cppcomponents::static_interfaces<S...> >{
					typedef implement_inspectable_interfaces<Derived, F, S...> type;
				};

				typedef typename helper<FactoryInterface, StaticInterface>::type static_type;
			};


		}
		// Enables implementing runtime class
		template<class Derived, class WRC>
		struct implement_rt_runtime_class
			: public cppcomponents::implement_runtime_class_base<Derived, typename WRC::type>
			, public detail::inherit_from_implement_inspectable<Derived, typename WRC::type>::object_type{

				typedef cppcomponents::implement_runtime_class_base<Derived, typename WRC::type> imp_rcb_t;


				struct implement_factory_static_interfaces
					: public imp_rcb_t::implement_factory_static_interfaces
					, public detail::inherit_from_implement_inspectable<implement_factory_static_interfaces, typename WRC::type>::static_type{


						static TrustLevel GetTrustLevel(){ return Derived::GetTrustLevel(); }




				};

				typedef implement_factory_static_interfaces ImplementFactoryStaticInterfaces;
				static TrustLevel GetTrustLevel(){ return BaseTrust; }


				static cppcomponents::use<cppcomponents::InterfaceUnknown> InterfaceActivationFactory_ActivateInstance(){
					return Derived::create();
				}
		};

		template<class...Imps>
		implementation::hstring_type get_activation_factory(implementation::hstring_type activatibleClassId, portable_base** factory){

			return cppcomponents::get_activation_factory(cppcomponents::type_list<Imps...>(), activatibleClassId, reinterpret_cast<cppcomponents::portable_base**>(factory));


		}

		// Usage






		namespace detail{
			struct activation_factory_holder{


				activation_factory_holder(hstring hs) : af_(get_activation_factory(hs)){};
				cppcomponents::use<InterfaceUnknown> get(){ return af_.QueryInterface<cppcomponents::InterfaceUnknown>(); };

				static cppcomponents::use<cppcomponents::InterfaceUnknown> create(hstring hs){
					return get_activation_factory(hs).QueryInterface<cppcomponents::InterfaceUnknown>();
				}
			private:
				cppcomponents::use<InterfaceActivationFactory> af_;

				static cppcomponents::use<InterfaceActivationFactory> get_activation_factory(const hstring& id){
					auto paf = implementation::Implementation::GetActivationFactory(id.value(), &cppcomponents::use<InterfaceActivationFactory>::uuid_type::get());
					cppcomponents::use<InterfaceActivationFactory> ret(cross_compiler_interface::reinterpret_portable_base<InterfaceActivationFactory::Interface>(paf), false);
					return ret;
				}
			};
		}


		template<class RC>
		struct use_rt_runtime_class : public cppcomponents::use_runtime_class_base<typename RC::type, detail::activation_factory_holder>{
			typedef cppcomponents::use_runtime_class_base<typename RC::type, detail::activation_factory_holder> base_t;



			use_rt_runtime_class(const base_t& b) : base_t{ b }{}

			template<class P0>
			explicit use_rt_runtime_class(P0 && p0) : base_t{ std::forward<P0>(p0) }{}

			template<class P0, class... P>
			explicit use_rt_runtime_class(P0 && p0, P && ... p) : base_t{ std::forward<P0>(p0), std::forward<P>(p)... }{}

			use_rt_runtime_class() : base_t{ base_t::from_interface(base_t::factory_interface().QueryInterface<InterfaceActivationFactory>().ActivateInstance()) }{}



		};



	}

}

#endif

