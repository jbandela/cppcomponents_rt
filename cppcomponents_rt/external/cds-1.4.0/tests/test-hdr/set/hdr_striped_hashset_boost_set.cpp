/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.4.0
*/


#include "set/hdr_striped_set.h"

#include <boost/version.hpp>
#if BOOST_VERSION >= 104800

#include <cds/container/striped_set/boost_set.h>
#include <cds/container/striped_set.h>
#include <cds/lock/spinlock.h>

namespace set {

    namespace {
        typedef boost::container::set< StripedSetHdrTest::item, StripedSetHdrTest::less<StripedSetHdrTest::item> > set_t ;

        struct my_copy_policy {
            typedef set_t::iterator iterator ;

            void operator()( set_t& set, iterator itWhat )
            {
                set.insert( *itWhat );
            }
        };
    }

    void StripedSetHdrTest::Striped_boost_set()
    {
        CPPUNIT_MESSAGE( "cmp")   ;
        typedef cc::StripedSet< set_t
            , co::hash< hash_int >
            , co::compare< cmp<item> >
            ,co::mutex_policy< cc::striped_set::striping<> >
        >   set_cmp ;
        test_striped< set_cmp >()    ;

        CPPUNIT_MESSAGE( "less")   ;
        typedef cc::StripedSet< set_t
            , co::hash< hash_int >
            , co::less< less<item> >
            ,co::mutex_policy< cc::striped_set::striping<> >
        >   set_less ;
        test_striped< set_less >()    ;

        CPPUNIT_MESSAGE( "cmpmix")   ;
        typedef cc::StripedSet< set_t
            , co::hash< hash_int >
            , co::compare< cmp<item> >
            , co::less< less<item> >
            ,co::mutex_policy< cc::striped_set::striping<> >
        >   set_cmpmix ;
        test_striped< set_cmpmix >()    ;

        // Spinlock as lock policy
        CPPUNIT_MESSAGE( "spinlock")   ;
        typedef cc::StripedSet< set_t
            , co::hash< hash_int >
            , co::less< less<item> >
            ,co::mutex_policy< cc::striped_set::striping< cds::lock::Spin > >
        >   set_spin ;
        test_striped< set_spin >()    ;

        // Resizing policy
        CPPUNIT_MESSAGE( "load_factor_resizing<0>(1024)")   ;
        {
            typedef cc::StripedSet< set_t
                , co::hash< hash_int >
                , co::less< less<item> >
                , co::resizing_policy< cc::striped_set::load_factor_resizing<0> >
            >   set_less_resizing_lf ;
            set_less_resizing_lf s(30, cc::striped_set::load_factor_resizing<0>( 1024 ));
            test_striped_with( s )  ;
        }

        CPPUNIT_MESSAGE( "load_factor_resizing<256>")   ;
        typedef cc::StripedSet< set_t
            , co::hash< hash_int >
            , co::less< less<item> >
            , co::resizing_policy< cc::striped_set::load_factor_resizing<256> >
        >   set_less_resizing_lf16 ;
        test_striped< set_less_resizing_lf16 >()    ;

        CPPUNIT_MESSAGE( "single_bucket_size_threshold<0>(1024")   ;
        {
            typedef cc::StripedSet< set_t
                , co::hash< hash_int >
                , co::less< less<item> >
                , co::resizing_policy< cc::striped_set::single_bucket_size_threshold<0> >
            >   set_less_resizing_sbt ;
            set_less_resizing_sbt s( 30, cc::striped_set::single_bucket_size_threshold<0>(1024)) ;
            test_striped_with(s)    ;
        }

        CPPUNIT_MESSAGE( "single_bucket_size_threshold<256>")   ;
        typedef cc::StripedSet< set_t
            , co::hash< hash_int >
            , co::less< less<item> >
            , co::resizing_policy< cc::striped_set::single_bucket_size_threshold<256> >
        >   set_less_resizing_sbt16 ;
        test_striped< set_less_resizing_sbt16 >()    ;

        // Copy policy
        CPPUNIT_MESSAGE( "load_factor_resizing<256>, copy_item")   ;
        typedef cc::StripedSet< set_t
            , co::hash< hash_int >
            , co::compare< cmp<item> >
            , co::resizing_policy< cc::striped_set::load_factor_resizing<256> >
            , co::copy_policy< cc::striped_set::copy_item >
        >   set_copy_item ;
        test_striped< set_copy_item >()    ;

        CPPUNIT_MESSAGE( "load_factor_resizing<256>, swap_item")   ;
        typedef cc::StripedSet< set_t
            , co::hash< hash_int >
            , co::compare< cmp<item> >
            , co::resizing_policy< cc::striped_set::load_factor_resizing<256> >
            , co::copy_policy< cc::striped_set::swap_item >
        >   set_swap_item ;
        test_striped< set_swap_item >()    ;

        CPPUNIT_MESSAGE( "load_factor_resizing<256>, move_item")   ;
        typedef cc::StripedSet< set_t
            , co::hash< hash_int >
            , co::compare< cmp<item> >
            , co::resizing_policy< cc::striped_set::load_factor_resizing<256> >
            , co::copy_policy< cc::striped_set::move_item >
        >   set_move_item ;
        test_striped< set_move_item >()    ;

        CPPUNIT_MESSAGE( "load_factor_resizing<256>, special copy_item")   ;
        typedef cc::StripedSet< set_t
            , co::hash< hash_int >
            , co::compare< cmp<item> >
            , co::resizing_policy< cc::striped_set::load_factor_resizing<256> >
            , co::copy_policy< my_copy_policy >
        >   set_special_copy_item ;
        test_striped< set_special_copy_item >()    ;
    }

}   // namespace set

#else // BOOST_VERSION < 104800

namespace set {
    void StripedSetHdrTest::Striped_boost_set()
    {
        CPPUNIT_MESSAGE( "Skipped; for boost::container::set you should use boost version 1.48 or above" ) ;
    }
}
#endif  // BOOST_VERSION
