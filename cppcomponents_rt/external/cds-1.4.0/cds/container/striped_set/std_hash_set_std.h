/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.4.0
*/


#ifndef __CDS_CONTAINER_STRIPED_SET_STD_HASH_SET_STD_ADAPTER_H
#define __CDS_CONTAINER_STRIPED_SET_STD_HASH_SET_STD_ADAPTER_H

#ifndef __CDS_CONTAINER_STRIPED_SET_STD_HASH_SET_ADAPTER_H
#   error <cds/container/striped_set/std_hash_set.h> must be included instead of <cds/container/striped_set/std_hash_set_std.h> header
#endif

#include <cds/container/striped_set/adapter.h>
#include <unordered_set>

//@cond
namespace cds { namespace container {
    namespace striped_set {

        // Copy policy for std::unordered_set
        template <typename T, typename Hash, typename Pred, typename Alloc>
        struct copy_item_policy< std::unordered_set< T, Hash, Pred, Alloc > >
        {
            typedef std::unordered_set< T, Hash, Pred, Alloc > set_type ;
            typedef typename set_type::iterator iterator ;

            void operator()( set_type& set, iterator itWhat )
            {
                set.insert( *itWhat )   ;
            }
        };

        template <typename T, typename Hash, typename Pred, typename Alloc>
        struct swap_item_policy< std::unordered_set< T, Hash, Pred, Alloc > >: public copy_item_policy< std::unordered_set< T, Hash, Pred, Alloc > >
        {};

#ifdef CDS_MOVE_SEMANTICS_SUPPORT
        // Move policy for std::unordered_set
        template <typename T, typename Hash, typename Pred, typename Alloc>
        struct move_item_policy< std::unordered_set< T, Hash, Pred, Alloc > >
        {
            typedef std::unordered_set< T, Hash, Pred, Alloc > set_type ;
            typedef typename set_type::iterator iterator ;

            void operator()( set_type& set, iterator itWhat )
            {
                set.insert( std::move( *itWhat ) )    ;
            }
        };
#endif

    }   // namespace striped_set
}} // namespace cds::container

namespace cds { namespace intrusive { namespace striped_set {
    /// std::unordered_set  adapter for hash set bucket
    template <typename T, class Hash, class Pred, class Alloc, CDS_SPEC_OPTIONS>
    class adapt< std::unordered_set<T, Hash, Pred, Alloc>, CDS_OPTIONS >
    {
    public:
        typedef std::unordered_set<T, Hash, Pred, Alloc>  container_type  ;   ///< underlying container type

    private:
        /// Adapted container type
        class adapted_container: public cds::container::striped_set::adapted_container
        {
        public:
            typedef typename container_type::value_type value_type  ;   ///< value type stored in the container
            typedef typename container_type::iterator      iterator ;   ///< container iterator
            typedef typename container_type::const_iterator const_iterator ;    ///< container const iterator

            static bool const has_find_with = false     ;
            static bool const has_erase_with = false    ;

        private:
            //@cond
            typedef typename cds::opt::select<
                typename cds::opt::value<
                    typename cds::opt::find_option<
                        cds::opt::copy_policy< cds::container::striped_set::move_item >
                        , CDS_OPTIONS
                    >::type
                >::copy_policy
                , cds::container::striped_set::copy_item, cds::container::striped_set::copy_item_policy<container_type>
                , cds::container::striped_set::swap_item, cds::container::striped_set::swap_item_policy<container_type> // not defined
#ifdef CDS_MOVE_SEMANTICS_SUPPORT
                , cds::container::striped_set::move_item, cds::container::striped_set::move_item_policy<container_type>
#endif
            >::type copy_item   ;
            //@endcond

        private:
            //@cond
            container_type  m_Set  ;
            //@endcond

        public:
            template <typename Q, typename Func>
            bool insert( const Q& val, Func f )
            {
                std::pair<iterator, bool> res = m_Set.insert( value_type(val) ) ;
                if ( res.second )
                    ::cds::unref(f)( const_cast<value_type&>(*res.first) )  ;
                return res.second   ;
            }

#       ifdef CDS_EMPLACE_SUPPORT
            template <typename... Args>
            bool emplace( Args&&... args )
            {
#           if defined(__GLIBCXX__) && __GLIBCXX__ < 20130322
                // GCC < 4.8:  std::set has no "emplace" member function. Emulate it
                std::pair<iterator, bool> res = m_Set.insert( value_type( std::forward<Args>(args)...)) ;
#           else
                std::pair<iterator, bool> res = m_Set.emplace( std::forward<Args>(args)... ) ;
#           endif
                return res.second   ;
            }
#       endif

            template <typename Q, typename Func>
            std::pair<bool, bool> ensure( const Q& val, Func func )
            {
                std::pair<iterator, bool> res = m_Set.insert( value_type(val) ) ;
                cds::unref(func)( res.second, const_cast<value_type&>(*res.first), val )     ;
                return std::make_pair( true, res.second )   ;
            }

            template <typename Q, typename Func>
            bool erase( const Q& key, Func f )
            {
                const_iterator it = m_Set.find( value_type(key) ) ;
                if ( it == m_Set.end() )
                    return false ;
                cds::unref(f)( const_cast<value_type&>(*it) )  ;
                m_Set.erase( it )       ;
                return true ;
            }

            template <typename Q, typename Func>
            bool find( Q& val, Func f )
            {
                iterator it = m_Set.find( value_type(val) ) ;
                if ( it == m_Set.end() )
                    return false ;
                cds::unref(f)( const_cast<value_type&>(*it), val ) ;
                return true ;
            }

            /// Clears the container
            void clear()
            {
                m_Set.clear()    ;
            }

            iterator begin()                { return m_Set.begin(); }
            const_iterator begin() const    { return m_Set.begin(); }
            iterator end()                  { return m_Set.end(); }
            const_iterator end() const      { return m_Set.end(); }

            void move_item( adapted_container& /*from*/, iterator itWhat )
            {
                assert( m_Set.find( *itWhat ) == m_Set.end() ) ;
                copy_item()( m_Set, itWhat ) ;
            }

            size_t size() const
            {
                return m_Set.size() ;
            }
        };

    public:
        typedef adapted_container type ; ///< Result of \p adapt metafunction
    };
}}} // namespace cds::intrusive::striped_set


//@endcond

#endif  // #ifndef __CDS_CONTAINER_STRIPED_SET_STD_HASH_SET_STD_ADAPTER_H
