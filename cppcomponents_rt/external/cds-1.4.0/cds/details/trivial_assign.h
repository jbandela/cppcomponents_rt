/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky (libcds.sf.com) 2006-2013
    Distributed under the BSD license (see accompanying file license.txt)

    Version 1.4.0
*/


#ifndef __CDS_DETAILS_TRIVIAL_ASSIGN_H
#define __CDS_DETAILS_TRIVIAL_ASSIGN_H

#include <cds/details/defs.h>

//@cond
namespace cds { namespace details {

    template <typename Dest, typename Source>
    struct trivial_assign
    {
        Dest& operator()( Dest& dest, const Source& src )
        {
            return dest = src ;
        }
    };
}}  // namespace cds::details
//@endcond

#endif // #ifndef __CDS_DETAILS_TRIVIAL_ASSIGN_H
