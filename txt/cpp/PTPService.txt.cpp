/* PTPService.txt.cpp */
//----------------------------------------------------------------------------------------
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

                             /*                         */
                             /* PTP Service Format 1.00 */
                             /*                         */

/* 1) Data types */

   using ServiceIdType = uint32 ;

   using FunctionIdType = uint32 ;

   using LenType = uint32 ;

   using BoolType = uint32 ;

   using FlagType = uint32 ;

   using ErrorIdType = uint32 ;

/* 2) Constants */

   const ErrorIdType NoError    = 0 ;
   const ErrorIdType BadInput   = 1 ;
   const ErrorIdType NoFunction = 2 ;
   const ErrorIdType Exhausted  = 3 ;
   const ErrorIdType Unknown    = 4 ;

/* 3) Service identification */

   struct ServiceFunction
    {
     ServiceIdType service_id;
     FunctionIdType function_id;
    };

   struct Result // if( error_id==NoError ) result follows
    {
     ServiceFunction serv_func;
     ErrorIdType error_id;
    };


