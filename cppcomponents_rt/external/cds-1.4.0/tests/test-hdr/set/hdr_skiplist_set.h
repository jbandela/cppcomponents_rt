/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.4.0
*/


#include "set/hdr_set.h"

namespace set {

    class SkipListSetHdrTest: public HashSetHdrTest
    {
        typedef HashSetHdrTest base_class ;

        template <class Set, typename PrintStat >
        void test()
        {
            Set s ;
            base_class::test_int_with( s ) ;

            static int const nLimit = 10000 ;
            typedef typename Set::iterator          set_iterator ;
            typedef typename Set::const_iterator    const_set_iterator ;

            int nCount = 0  ;
            int nPrevKey = 0;

            // Test iterator - ascending order
            s.clear() ;
            CPPUNIT_ASSERT( s.empty() ) ;

            for ( int i = 0; i < nLimit; ++i ) {
                CPPUNIT_ASSERT( s.insert(i) ) ;
            }
            CPPUNIT_MSG( PrintStat()(s, "Iterator test, ascending insert order") ) ;

            nCount = 0  ;
            nPrevKey = 0;
            for ( set_iterator it = s.begin(), itEnd = s.end(); it != itEnd; ++it ) {
                CPPUNIT_ASSERT( (*it).nKey == it->nVal )    ;
                CPPUNIT_ASSERT( s.find( it->nKey )) ;
                it->nVal = (*it).nKey * 2  ;
                ++nCount    ;
                if ( it != s.begin() ) {
                    CPPUNIT_ASSERT( nPrevKey + 1 == it->nKey ) ;
                }
                nPrevKey = it->nKey ;
            }
            CPPUNIT_ASSERT( nCount == nLimit ) ;

            nCount = 0  ;
            for ( const_set_iterator it = s.cbegin(), itEnd = s.cend(); it != itEnd; ++it ) {
                CPPUNIT_ASSERT( (*it).nKey * 2 == it->nVal )    ;
                ++nCount    ;
                if ( it != s.cbegin() ) {
                    CPPUNIT_ASSERT( nPrevKey + 1 == it->nKey ) ;
                }
                nPrevKey = it->nKey ;
            }
            CPPUNIT_ASSERT( nCount == nLimit ) ;

            // Test iterator - descending order
            s.clear() ;
            CPPUNIT_ASSERT( s.empty() ) ;

            for ( int i = nLimit; i > 0; --i ) {
                CPPUNIT_ASSERT( s.insert( std::make_pair(i - 1, (i-1) * 2) )) ;
            }
            CPPUNIT_MSG( PrintStat()(s, "Iterator test, descending insert order") ) ;

            nCount = 0  ;
            nPrevKey = 0;
            for ( set_iterator it = s.begin(), itEnd = s.end(); it != itEnd; ++it ) {
                CPPUNIT_ASSERT( (*it).nKey * 2 == it->nVal )    ;
                CPPUNIT_ASSERT( s.find( it->nKey )) ;
                it->nVal = (*it).nKey  ;
                ++nCount    ;
                if ( it != s.begin() ) {
                    CPPUNIT_ASSERT( nPrevKey + 1 == it->nKey ) ;
                }
                nPrevKey = it->nKey ;
            }
            CPPUNIT_ASSERT( nCount == nLimit ) ;

            nCount = 0  ;
            for ( const_set_iterator it = s.cbegin(), itEnd = s.cend(); it != itEnd; ++it ) {
                CPPUNIT_ASSERT( (*it).nKey == it->nVal )    ;
                ++nCount    ;
                if ( it != s.cbegin() ) {
                    CPPUNIT_ASSERT( nPrevKey + 1 == it->nKey ) ;
                }
                nPrevKey = it->nKey ;
            }
            CPPUNIT_ASSERT( nCount == nLimit ) ;

            // Test iterator - random order
            s.clear() ;
            CPPUNIT_ASSERT( s.empty() ) ;
            {
                int nRand[nLimit] ;
                for ( int i = 0; i < nLimit; ++i ) {
                    nRand[i] = i ;
                }
                std::random_shuffle( nRand, nRand + nLimit ) ;

                for ( int i = 0; i < nLimit; ++i ) {
                    CPPUNIT_ASSERT( s.insert(i) ) ;
                }
                CPPUNIT_MSG( PrintStat()(s, "Iterator test, random insert order") ) ;
            }

            nCount = 0  ;
            nPrevKey = 0;
            for ( set_iterator it = s.begin(), itEnd = s.end(); it != itEnd; ++it ) {
                CPPUNIT_ASSERT( (*it).nKey == it->nVal )    ;
                CPPUNIT_ASSERT( s.find( it->nKey )) ;
                it->nVal = (*it).nKey * 2  ;
                ++nCount    ;
                if ( it != s.begin() ) {
                    CPPUNIT_ASSERT( nPrevKey + 1 == it->nKey ) ;
                }
                nPrevKey = it->nKey ;
            }
            CPPUNIT_ASSERT( nCount == nLimit ) ;

            nCount = 0  ;
            for ( const_set_iterator it = s.cbegin(), itEnd = s.cend(); it != itEnd; ++it ) {
                CPPUNIT_ASSERT( (*it).nKey * 2 == it->nVal )    ;
                ++nCount    ;
                if ( it != s.cbegin() ) {
                    CPPUNIT_ASSERT( nPrevKey + 1 == it->nKey ) ;
                }
                nPrevKey = it->nKey ;
            }
            CPPUNIT_ASSERT( nCount == nLimit ) ;
        }

        template <class Set, typename PrintStat >
        void test_nogc()
        {
            typedef typename Set::value_type        value_type  ;
            typedef typename Set::iterator          iterator    ;
            typedef typename Set::const_iterator    const_iterator  ;

            Set s ;
            iterator it     ;

            CPPUNIT_ASSERT( s.empty() )     ;
            CPPUNIT_ASSERT( check_size( s, 0 )) ;

            // insert
            it = s.insert( 10 ) ;
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 10 )   ;
            CPPUNIT_ASSERT( it->val() == 10 )   ;
            CPPUNIT_ASSERT( !s.empty() )        ;
            CPPUNIT_ASSERT( check_size( s, 1 )) ;
            CPPUNIT_ASSERT( s.insert( 10 ) == s.end() ) ;

            it = s.insert( std::make_pair( 50, 25 )) ;
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 50 )   ;
            CPPUNIT_ASSERT( it->val() == 25 )   ;
            CPPUNIT_ASSERT( !s.empty() )        ;
            CPPUNIT_ASSERT( check_size( s, 2 )) ;
            CPPUNIT_ASSERT( s.insert( 50 ) == s.end() ) ;

            // ensure
            std::pair< iterator, bool>  ensureResult    ;
            ensureResult = s.ensure( 20 )   ;
            CPPUNIT_ASSERT( ensureResult.first != s.end() ) ;
            CPPUNIT_ASSERT( ensureResult.second  ) ;
            CPPUNIT_ASSERT( ensureResult.first->key() == 20 )   ;
            CPPUNIT_ASSERT( ensureResult.first->val() == 20 )   ;
            CPPUNIT_ASSERT( !s.empty() )        ;
            CPPUNIT_ASSERT( check_size( s, 3 )) ;

            ensureResult = s.ensure( std::make_pair( 20, 200 ))   ;
            CPPUNIT_ASSERT( ensureResult.first != s.end() ) ;
            CPPUNIT_ASSERT( !ensureResult.second  ) ;
            CPPUNIT_ASSERT( ensureResult.first->key() == 20 )   ;
            CPPUNIT_ASSERT( ensureResult.first->val() == 20 )   ;
            CPPUNIT_ASSERT( !s.empty() )        ;
            CPPUNIT_ASSERT( check_size( s, 3 )) ;
            ensureResult.first->nVal = 22      ;

            ensureResult = s.ensure( std::make_pair( 30, 33 ))   ;
            CPPUNIT_ASSERT( ensureResult.first != s.end() ) ;
            CPPUNIT_ASSERT( ensureResult.second  ) ;
            CPPUNIT_ASSERT( ensureResult.first->key() == 30 )   ;
            CPPUNIT_ASSERT( ensureResult.first->val() == 33 )   ;
            CPPUNIT_ASSERT( !s.empty() )        ;
            CPPUNIT_ASSERT( check_size( s, 4 )) ;

            // find
            it = s.find( 10 )   ;
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 10 )   ;
            CPPUNIT_ASSERT( it->val() == 10 )   ;

            it = s.find( 20 )   ;
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 20 )   ;
            CPPUNIT_ASSERT( it->val() == 22 )   ;

            it = s.find_with( 30, base_class::less<value_type>() )   ;
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 30 )   ;
            CPPUNIT_ASSERT( it->val() == 33 )   ;

            it = s.find( 40 )   ;
            CPPUNIT_ASSERT( it == s.end() ) ;

            it = s.find( 50 )   ;
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 50 )   ;
            CPPUNIT_ASSERT( it->val() == 25 )   ;

#       ifdef CDS_EMPLACE_SUPPORT
            // emplace test
            it = s.emplace( 151 ) ;  // key = 151,  val = 151
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 151 ) ;
            CPPUNIT_ASSERT( it->val() == 151 ) ;

            it = s.emplace( 174, 471 ) ; // key == 174, val = 471
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 174 ) ;
            CPPUNIT_ASSERT( it->val() == 471 ) ;

            it = s.emplace( std::make_pair( 190, 91 )) ; // key == 190, val = 91
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 190 ) ;
            CPPUNIT_ASSERT( it->val() == 91 ) ;

            it = s.find( 174 )  ;
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 174 ) ;
            CPPUNIT_ASSERT( it->val() == 471 ) ;

            it = s.find( 190 )  ;
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 190 ) ;
            CPPUNIT_ASSERT( it->val() == 91 ) ;

            it = s.find( 151 )  ;
            CPPUNIT_ASSERT( it != s.end() ) ;
            CPPUNIT_ASSERT( it->key() == 151 ) ;
            CPPUNIT_ASSERT( it->val() == 151 ) ;
#       endif

            CPPUNIT_ASSERT( !s.empty() )     ;
            s.clear()   ;
            CPPUNIT_ASSERT( s.empty() )     ;
            CPPUNIT_ASSERT( check_size( s, 0 )) ;

            // iterator test
            for ( int i = 0; i < 500; ++i ) {
                CPPUNIT_ASSERT( s.insert( std::make_pair( i, i * 2) ) != s.end() )   ;
            }
            CPPUNIT_ASSERT( !s.empty() )     ;
            CPPUNIT_ASSERT( check_size( s, 500 )) ;

            for ( iterator it = s.begin(), itEnd = s.end(); it != itEnd; ++it ) {
                CPPUNIT_ASSERT( (*it).nKey * 2 == it->nVal )    ;
                it->nVal = (*it).nKey   ;
            }

            Set const& refSet = s   ;
            for ( const_iterator it = refSet.begin(), itEnd = refSet.end(); it != itEnd; ++it ) {
                CPPUNIT_ASSERT( (*it).nKey == it->nVal )    ;
            }
        }

    public:
        void SkipList_HP_less()  ;
        void SkipList_HP_cmp()  ;
        void SkipList_HP_cmpless()  ;
        void SkipList_HP_less_stat()  ;
        void SkipList_HP_cmp_stat()  ;
        void SkipList_HP_cmpless_stat()  ;
        void SkipList_HP_xorshift_less()  ;
        void SkipList_HP_xorshift_cmp()  ;
        void SkipList_HP_xorshift_cmpless()  ;
        void SkipList_HP_xorshift_less_stat()  ;
        void SkipList_HP_xorshift_cmp_stat()  ;
        void SkipList_HP_xorshift_cmpless_stat()  ;
        void SkipList_HP_turbopas_less()  ;
        void SkipList_HP_turbopas_cmp()  ;
        void SkipList_HP_turbopas_cmpless()  ;
        void SkipList_HP_turbopas_less_stat()  ;
        void SkipList_HP_turbopas_cmp_stat()  ;
        void SkipList_HP_turbopas_cmpless_stat()  ;
        void SkipList_HP_michaelalloc_less()  ;
        void SkipList_HP_michaelalloc_cmp()  ;
        void SkipList_HP_michaelalloc_cmpless()  ;
        void SkipList_HP_michaelalloc_less_stat()  ;
        void SkipList_HP_michaelalloc_cmp_stat()  ;
        void SkipList_HP_michaelalloc_cmpless_stat()  ;

        void SkipList_HRC_less()  ;
        void SkipList_HRC_cmp()  ;
        void SkipList_HRC_cmpless()  ;
        void SkipList_HRC_less_stat()  ;
        void SkipList_HRC_cmp_stat()  ;
        void SkipList_HRC_cmpless_stat()  ;
        void SkipList_HRC_xorshift_less()  ;
        void SkipList_HRC_xorshift_cmp()  ;
        void SkipList_HRC_xorshift_cmpless()  ;
        void SkipList_HRC_xorshift_less_stat()  ;
        void SkipList_HRC_xorshift_cmp_stat()  ;
        void SkipList_HRC_xorshift_cmpless_stat()  ;
        void SkipList_HRC_turbopas_less()  ;
        void SkipList_HRC_turbopas_cmp()  ;
        void SkipList_HRC_turbopas_cmpless()  ;
        void SkipList_HRC_turbopas_less_stat()  ;
        void SkipList_HRC_turbopas_cmp_stat()  ;
        void SkipList_HRC_turbopas_cmpless_stat()  ;
        void SkipList_HRC_michaelalloc_less()  ;
        void SkipList_HRC_michaelalloc_cmp()  ;
        void SkipList_HRC_michaelalloc_cmpless()  ;
        void SkipList_HRC_michaelalloc_less_stat()  ;
        void SkipList_HRC_michaelalloc_cmp_stat()  ;
        void SkipList_HRC_michaelalloc_cmpless_stat()  ;

        void SkipList_PTB_less()  ;
        void SkipList_PTB_cmp()  ;
        void SkipList_PTB_cmpless()  ;
        void SkipList_PTB_less_stat()  ;
        void SkipList_PTB_cmp_stat()  ;
        void SkipList_PTB_cmpless_stat()  ;
        void SkipList_PTB_xorshift_less()  ;
        void SkipList_PTB_xorshift_cmp()  ;
        void SkipList_PTB_xorshift_cmpless()  ;
        void SkipList_PTB_xorshift_less_stat()  ;
        void SkipList_PTB_xorshift_cmp_stat()  ;
        void SkipList_PTB_xorshift_cmpless_stat()  ;
        void SkipList_PTB_turbopas_less()  ;
        void SkipList_PTB_turbopas_cmp()  ;
        void SkipList_PTB_turbopas_cmpless()  ;
        void SkipList_PTB_turbopas_less_stat()  ;
        void SkipList_PTB_turbopas_cmp_stat()  ;
        void SkipList_PTB_turbopas_cmpless_stat()  ;
        void SkipList_PTB_michaelalloc_less()  ;
        void SkipList_PTB_michaelalloc_cmp()  ;
        void SkipList_PTB_michaelalloc_cmpless()  ;
        void SkipList_PTB_michaelalloc_less_stat()  ;
        void SkipList_PTB_michaelalloc_cmp_stat()  ;
        void SkipList_PTB_michaelalloc_cmpless_stat()  ;

        void SkipList_NOGC_less()  ;
        void SkipList_NOGC_cmp()  ;
        void SkipList_NOGC_cmpless()  ;
        void SkipList_NOGC_less_stat()  ;
        void SkipList_NOGC_cmp_stat()  ;
        void SkipList_NOGC_cmpless_stat()  ;
        void SkipList_NOGC_xorshift_less()  ;
        void SkipList_NOGC_xorshift_cmp()  ;
        void SkipList_NOGC_xorshift_cmpless()  ;
        void SkipList_NOGC_xorshift_less_stat()  ;
        void SkipList_NOGC_xorshift_cmp_stat()  ;
        void SkipList_NOGC_xorshift_cmpless_stat()  ;
        void SkipList_NOGC_turbopas_less()  ;
        void SkipList_NOGC_turbopas_cmp()  ;
        void SkipList_NOGC_turbopas_cmpless()  ;
        void SkipList_NOGC_turbopas_less_stat()  ;
        void SkipList_NOGC_turbopas_cmp_stat()  ;
        void SkipList_NOGC_turbopas_cmpless_stat()  ;
        void SkipList_NOGC_michaelalloc_less()  ;
        void SkipList_NOGC_michaelalloc_cmp()  ;
        void SkipList_NOGC_michaelalloc_cmpless()  ;
        void SkipList_NOGC_michaelalloc_less_stat()  ;
        void SkipList_NOGC_michaelalloc_cmp_stat()  ;
        void SkipList_NOGC_michaelalloc_cmpless_stat()  ;

        CPPUNIT_TEST_SUITE(SkipListSetHdrTest)
            CPPUNIT_TEST(SkipList_HP_less)
            CPPUNIT_TEST(SkipList_HP_cmp)
            CPPUNIT_TEST(SkipList_HP_cmpless)
            CPPUNIT_TEST(SkipList_HP_less_stat)
            CPPUNIT_TEST(SkipList_HP_cmp_stat)
            CPPUNIT_TEST(SkipList_HP_cmpless_stat)
            CPPUNIT_TEST(SkipList_HP_xorshift_less)
            CPPUNIT_TEST(SkipList_HP_xorshift_cmp)
            CPPUNIT_TEST(SkipList_HP_xorshift_cmpless)
            CPPUNIT_TEST(SkipList_HP_xorshift_less_stat)
            CPPUNIT_TEST(SkipList_HP_xorshift_cmp_stat)
            CPPUNIT_TEST(SkipList_HP_xorshift_cmpless_stat)
            CPPUNIT_TEST(SkipList_HP_turbopas_less)
            CPPUNIT_TEST(SkipList_HP_turbopas_cmp)
            CPPUNIT_TEST(SkipList_HP_turbopas_cmpless)
            CPPUNIT_TEST(SkipList_HP_turbopas_less_stat)
            CPPUNIT_TEST(SkipList_HP_turbopas_cmp_stat)
            CPPUNIT_TEST(SkipList_HP_turbopas_cmpless_stat)
            CPPUNIT_TEST(SkipList_HP_michaelalloc_less)
            CPPUNIT_TEST(SkipList_HP_michaelalloc_cmp)
            CPPUNIT_TEST(SkipList_HP_michaelalloc_cmpless)
            CPPUNIT_TEST(SkipList_HP_michaelalloc_less_stat)
            CPPUNIT_TEST(SkipList_HP_michaelalloc_cmp_stat)
            CPPUNIT_TEST(SkipList_HP_michaelalloc_cmpless_stat)

            CPPUNIT_TEST(SkipList_HRC_less)
            CPPUNIT_TEST(SkipList_HRC_cmp)
            CPPUNIT_TEST(SkipList_HRC_cmpless)
            CPPUNIT_TEST(SkipList_HRC_less_stat)
            CPPUNIT_TEST(SkipList_HRC_cmp_stat)
            CPPUNIT_TEST(SkipList_HRC_cmpless_stat)
            CPPUNIT_TEST(SkipList_HRC_xorshift_less)
            CPPUNIT_TEST(SkipList_HRC_xorshift_cmp)
            CPPUNIT_TEST(SkipList_HRC_xorshift_cmpless)
            CPPUNIT_TEST(SkipList_HRC_xorshift_less_stat)
            CPPUNIT_TEST(SkipList_HRC_xorshift_cmp_stat)
            CPPUNIT_TEST(SkipList_HRC_xorshift_cmpless_stat)
            CPPUNIT_TEST(SkipList_HRC_turbopas_less)
            CPPUNIT_TEST(SkipList_HRC_turbopas_cmp)
            CPPUNIT_TEST(SkipList_HRC_turbopas_cmpless)
            CPPUNIT_TEST(SkipList_HRC_turbopas_less_stat)
            CPPUNIT_TEST(SkipList_HRC_turbopas_cmp_stat)
            CPPUNIT_TEST(SkipList_HRC_turbopas_cmpless_stat)
            CPPUNIT_TEST(SkipList_HRC_michaelalloc_less)
            CPPUNIT_TEST(SkipList_HRC_michaelalloc_cmp)
            CPPUNIT_TEST(SkipList_HRC_michaelalloc_cmpless)
            CPPUNIT_TEST(SkipList_HRC_michaelalloc_less_stat)
            CPPUNIT_TEST(SkipList_HRC_michaelalloc_cmp_stat)
            CPPUNIT_TEST(SkipList_HRC_michaelalloc_cmpless_stat)

            CPPUNIT_TEST(SkipList_PTB_less)
            CPPUNIT_TEST(SkipList_PTB_cmp)
            CPPUNIT_TEST(SkipList_PTB_cmpless)
            CPPUNIT_TEST(SkipList_PTB_less_stat)
            CPPUNIT_TEST(SkipList_PTB_cmp_stat)
            CPPUNIT_TEST(SkipList_PTB_cmpless_stat)
            CPPUNIT_TEST(SkipList_PTB_xorshift_less)
            CPPUNIT_TEST(SkipList_PTB_xorshift_cmp)
            CPPUNIT_TEST(SkipList_PTB_xorshift_cmpless)
            CPPUNIT_TEST(SkipList_PTB_xorshift_less_stat)
            CPPUNIT_TEST(SkipList_PTB_xorshift_cmp_stat)
            CPPUNIT_TEST(SkipList_PTB_xorshift_cmpless_stat)
            CPPUNIT_TEST(SkipList_PTB_turbopas_less)
            CPPUNIT_TEST(SkipList_PTB_turbopas_cmp)
            CPPUNIT_TEST(SkipList_PTB_turbopas_cmpless)
            CPPUNIT_TEST(SkipList_PTB_turbopas_less_stat)
            CPPUNIT_TEST(SkipList_PTB_turbopas_cmp_stat)
            CPPUNIT_TEST(SkipList_PTB_turbopas_cmpless_stat)
            CPPUNIT_TEST(SkipList_PTB_michaelalloc_less)
            CPPUNIT_TEST(SkipList_PTB_michaelalloc_cmp)
            CPPUNIT_TEST(SkipList_PTB_michaelalloc_cmpless)
            CPPUNIT_TEST(SkipList_PTB_michaelalloc_less_stat)
            CPPUNIT_TEST(SkipList_PTB_michaelalloc_cmp_stat)
            CPPUNIT_TEST(SkipList_PTB_michaelalloc_cmpless_stat)

            CPPUNIT_TEST(SkipList_NOGC_less)
            CPPUNIT_TEST(SkipList_NOGC_cmp)
            CPPUNIT_TEST(SkipList_NOGC_cmpless)
            CPPUNIT_TEST(SkipList_NOGC_less_stat)
            CPPUNIT_TEST(SkipList_NOGC_cmp_stat)
            CPPUNIT_TEST(SkipList_NOGC_cmpless_stat)
            CPPUNIT_TEST(SkipList_NOGC_xorshift_less)
            CPPUNIT_TEST(SkipList_NOGC_xorshift_cmp)
            CPPUNIT_TEST(SkipList_NOGC_xorshift_cmpless)
            CPPUNIT_TEST(SkipList_NOGC_xorshift_less_stat)
            CPPUNIT_TEST(SkipList_NOGC_xorshift_cmp_stat)
            CPPUNIT_TEST(SkipList_NOGC_xorshift_cmpless_stat)
            CPPUNIT_TEST(SkipList_NOGC_turbopas_less)
            CPPUNIT_TEST(SkipList_NOGC_turbopas_cmp)
            CPPUNIT_TEST(SkipList_NOGC_turbopas_cmpless)
            CPPUNIT_TEST(SkipList_NOGC_turbopas_less_stat)
            CPPUNIT_TEST(SkipList_NOGC_turbopas_cmp_stat)
            CPPUNIT_TEST(SkipList_NOGC_turbopas_cmpless_stat)
            CPPUNIT_TEST(SkipList_NOGC_michaelalloc_less)
            CPPUNIT_TEST(SkipList_NOGC_michaelalloc_cmp)
            CPPUNIT_TEST(SkipList_NOGC_michaelalloc_cmpless)
            CPPUNIT_TEST(SkipList_NOGC_michaelalloc_less_stat)
            CPPUNIT_TEST(SkipList_NOGC_michaelalloc_cmp_stat)
            CPPUNIT_TEST(SkipList_NOGC_michaelalloc_cmpless_stat)

        CPPUNIT_TEST_SUITE_END()

    };
}
