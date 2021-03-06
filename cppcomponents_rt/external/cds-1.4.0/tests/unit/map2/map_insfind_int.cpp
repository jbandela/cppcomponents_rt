/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.4.0
*/


#include "map2/map_types.h"
#include "cppunit/thread.h"

#include <cds/os/topology.h>

namespace map2 {

#    define TEST_MAP(X)         void X() { test<MapTypes<key_type, value_type>::X >()    ; }
#    define TEST_MAP_NOLF(X)    void X() { test_nolf<MapTypes<key_type, value_type>::X >()    ; }

    namespace {
        static size_t  c_nMapSize = 1000000    ;  // map size
        static size_t  c_nThreadCount = 4      ;  // count of insertion thread
        static size_t  c_nMaxLoadFactor = 8    ;  // maximum load factor
        static bool    c_bPrintGCState = true  ;
    }

    class Map_InsFind_int: public CppUnitMini::TestCase
    {
        typedef size_t  key_type    ;
        typedef size_t  value_type  ;

        template <class Map>
        class Inserter: public CppUnitMini::TestThread
        {
            Map&     m_Map      ;

            virtual Inserter *    clone()
            {
                return new Inserter( *this )    ;
            }
        public:
            size_t  m_nInsertSuccess    ;
            size_t  m_nInsertFailed     ;
            size_t  m_nFindSuccess      ;
            size_t  m_nFindFail         ;

        public:
            Inserter( CppUnitMini::ThreadPool& pool, Map& rMap )
                : CppUnitMini::TestThread( pool )
                , m_Map( rMap )
            {}
            Inserter( Inserter& src )
                : CppUnitMini::TestThread( src )
                , m_Map( src.m_Map )
            {}

            Map_InsFind_int&  getTest()
            {
                return reinterpret_cast<Map_InsFind_int&>( m_Pool.m_Test )   ;
            }

            virtual void init() { cds::threading::Manager::attachThread()   ; }
            virtual void fini() { cds::threading::Manager::detachThread()   ; }

            virtual void test()
            {
                Map& rMap = m_Map   ;

                m_nInsertSuccess =
                    m_nInsertFailed =
                    m_nFindSuccess =
                    m_nFindFail = 0 ;

                size_t nInc = c_nThreadCount    ;
                for ( size_t nItem = m_nThreadNo; rMap.size() < c_nMapSize; nItem += nInc ) {
                    if ( rMap.insert( nItem, nItem * 8 ) )
                        ++m_nInsertSuccess  ;
                    else
                        ++m_nInsertFailed   ;

                    for ( size_t nFind = m_nThreadNo; nFind <= nItem; nFind += nInc ) {
                        if ( rMap.find( nFind) )
                            ++m_nFindSuccess    ;
                        else
                            ++m_nFindFail       ;
                    }
                }
            }
        };

    protected:

        template <class Map>
        void do_test( Map& testMap )
        {
            typedef Inserter<Map>       InserterThread  ;
            cds::OS::Timer    timer    ;

            CppUnitMini::ThreadPool pool( *this )   ;
            pool.add( new InserterThread( pool, testMap ), c_nThreadCount ) ;
            pool.run()  ;
            CPPUNIT_MSG( "   Duration=" << pool.avgDuration() ) ;

            size_t nInsertSuccess = 0   ;
            size_t nInsertFailed = 0    ;
            size_t nFindSuccess = 0     ;
            size_t nFindFailed = 0      ;
            for ( CppUnitMini::ThreadPool::iterator it = pool.begin(); it != pool.end(); ++it ) {
                InserterThread * pThread = static_cast<InserterThread *>( *it )   ;

                nInsertSuccess += pThread->m_nInsertSuccess ;
                nInsertFailed += pThread->m_nInsertFailed   ;
                nFindSuccess += pThread->m_nFindSuccess     ;
                nFindFailed += pThread->m_nFindFail         ;
            }

            CPPUNIT_MSG( "    Totals: Ins succ=" << nInsertSuccess << " fail=" << nInsertFailed << "\n"
                      << "           Find succ=" << nFindSuccess << " fail=" << nFindFailed
            ) ;

            CPPUNIT_ASSERT( nInsertFailed == 0 )    ;
            CPPUNIT_ASSERT( nFindFailed == 0 )      ;

            print_stat( testMap ) ;
        }

        template <class Map>
        void test()
        {
            static_assert( (!std::is_same< typename Map::item_counter, cds::atomicity::empty_item_counter >::value),
                "Empty item counter is not suitable for this test") ;

            CPPUNIT_MSG( "Thread count: " << c_nThreadCount
                << " map size=" << c_nMapSize
                );

            for ( size_t nLoadFactor = 1; nLoadFactor <= c_nMaxLoadFactor; nLoadFactor *= 2 ) {
                CPPUNIT_MSG( "Load factor=" << nLoadFactor )   ;
                Map  testMap( c_nMapSize, nLoadFactor ) ;
                do_test( testMap )     ;
                if ( c_bPrintGCState )
                    print_gc_state()            ;
            }
        }

        template <class Map>
        void test_nolf()
        {
            static_assert( (!std::is_same< typename Map::item_counter, cds::atomicity::empty_item_counter >::value),
                "Empty item counter is not suitable for this test") ;

            CPPUNIT_MSG( "Thread count: " << c_nThreadCount
                << " map size=" << c_nMapSize
                );

            Map testMap ;
            do_test( testMap )     ;
            if ( c_bPrintGCState )
                print_gc_state()            ;
        }

        void setUpParams( const CppUnitMini::TestCfg& cfg ) {
            c_nThreadCount = cfg.getULong("ThreadCount", 0 )            ;
            c_nMapSize = cfg.getULong("MapSize", 10000 )                ;
            c_nMaxLoadFactor = cfg.getULong("MaxLoadFactor", 8 )        ;
            c_bPrintGCState = cfg.getBool("PrintGCStateFlag", true )    ;
            if ( c_nThreadCount == 0 )
                c_nThreadCount = cds::OS::topology::processor_count()   ;
        }

#   include "map2/map_defs.h"
        CDSUNIT_DECLARE_MichaelMap
        CDSUNIT_DECLARE_MichaelMap_nogc
        CDSUNIT_DECLARE_SplitList
        CDSUNIT_DECLARE_SplitList_nogc
        CDSUNIT_DECLARE_SkipListMap
        CDSUNIT_DECLARE_SkipListMap_nogc
        CDSUNIT_DECLARE_StripedMap
        CDSUNIT_DECLARE_RefinableMap
        CDSUNIT_DECLARE_CuckooMap
        CDSUNIT_DECLARE_StdMap

        CPPUNIT_TEST_SUITE( Map_InsFind_int )
            CDSUNIT_TEST_MichaelMap
            CDSUNIT_TEST_MichaelMap_nogc
            CDSUNIT_TEST_SplitList
            CDSUNIT_TEST_SplitList_nogc
            CDSUNIT_TEST_SkipListMap
            CDSUNIT_TEST_SkipListMap_nogc
            CDSUNIT_TEST_StripedMap
            CDSUNIT_TEST_RefinableMap
            CDSUNIT_TEST_CuckooMap
            CDSUNIT_TEST_StdMap
        CPPUNIT_TEST_SUITE_END()

    } ;

    CPPUNIT_TEST_SUITE_REGISTRATION( Map_InsFind_int );
} // namespace map2
