/*******************************************************************************


   Copyright (C) 2011-2014 SequoiaDB Ltd.

   This program is free software: you can redistribute it and/or modify
   it under the term of the GNU Affero General Public License, version 3,
   as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warrenty of
   MARCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program. If not, see <http://www.gnu.org/license/>.

   Source File Name = sdbInterface.hpp

   Descriptive Name = Process MoDel Engine Dispatchable Unit Event Header

   When/how to use: this program may be used on binary and text-formatted
   versions of PMD component. This file contains structure for events that
   used as inter-EDU communications.

   Dependencies: N/A

   Restrictions: N/A

   Change Activity:
   defect Date        Who Description
   ====== =========== === ==============================================
          09/14/2012  TW  Initial Draft

   Last Changed =

*******************************************************************************/
#ifndef SDB_INTERFACE_HPP__
#define SDB_INTERFACE_HPP__

#include "core.hpp"
#include "oss.hpp"
#include "msg.h"
#include "msgDef.h"
#include <string>

namespace engine
{

   /*
      ENUM define
   */
   enum SDB_CB_TYPE
   {
      SDB_CB_DPS              = 0,
      SDB_CB_DMS,
      SDB_CB_BPS,
      SDB_CB_RTN,
      SDB_CB_TRANS,           // must after SDB_CB_DPS
      SDB_CB_CATALOGUE,
      SDB_CB_CLS,             // must after SDB_CB_CATALOGUE
      SDB_CB_COORD,
      SDB_CB_SQL,
      SDB_CB_AUTH,
      SDB_CB_AGGR,
      SDB_CB_FMP,
      SDB_CB_OMSVC,
      SDB_CB_OMAGT,

      SDB_CB_PMDCTRL,
      SDB_CB_OMPROXY,
      // THE MAX CB TYPE
      SDB_CB_MAX
   } ;

   /*
      SDB_INTERFACE_TYPE DEFINE
   */
   enum SDB_INTERFACE_TYPE
   {
      SDB_IF_CB      = 1,
      SDB_IF_SESSION,
      SDB_IF_EVT_HANDLER,
      SDB_IF_EVT_HOLDER,

      // THD MAX IF TYPE
      SDB_IF_MAX
   } ;

   /*
      SDB_SESSION_TYPE define
   */
   enum SDB_SESSION_TYPE
   {
      SDB_SESSION_LOCAL       = 0,
      SDB_SESSION_REST,
      SDB_SESSION_REPL_DST,
      SDB_SESSION_REPL_SRC,
      SDB_SESSION_SHARD,
      SDB_SESSION_FS_SRC,
      SDB_SESSION_FS_DST,
      SDB_SESSION_SPLIT_SRC,
      SDB_SESSION_SPLIT_DST,
      SDB_SESSION_OMAGENT,
      SDB_SESSION_PROTOCOL,

      // Reserved
      SDB_SESSION_MAX
   } ;

   /*
      SDB_CLIENT_TYPE define
   */
   enum SDB_CLIENT_TYPE
   {
      SDB_CLIENT_EXTERN    = 1,  // external client,ex: local service
      SDB_CLIENT_INNER,          // inner client, ex: shard service

      // Reserved
      SDB_CLIENT_MAX
   } ;

   /*
      SDB_DB_STATUS define
   */
   enum SDB_DB_STATUS
   {
      SDB_DB_NORMAL           = 0,
      SDB_DB_SHUTDOWN,
      SDB_DB_REBUILDING,
      SDB_DB_FULLSYNC,
      SDB_DB_OFFLINE_BK,

      SDB_DB_STATUS_MAX
   } ;

   /*
      Define SDB_DB_MODE value
   */
   #define SDB_DB_MODE_READONLY        0x00000001
   #define SDB_DB_MODE_DEACTIVATED     0x00000002

   /*
      _ISDBRoot define
   */
   class _ISDBRoot
   {
      public:
         _ISDBRoot () {}
         virtual ~_ISDBRoot () {}

         virtual void* queryInterface( SDB_INTERFACE_TYPE type )
         {
            return NULL ;
         }
   } ;
   typedef _ISDBRoot ISDBRoot ;

   /*
      EVENT MASK DEFINE
   */
   #define EVENT_MASK_ON_REGISTERED          0x00000001
   #define EVENT_MASK_ON_PRIMARYCHG          0x00000002

   enum SDB_EVENT_OCCUR_TYPE
   {
      SDB_EVT_OCCUR_BEFORE   = 1,
      SDB_EVT_OCCUR_AFTER
   } ;

   /*
      _IEventHander define
   */
   class _IEventHander
   {
      public :
         _IEventHander () {}
         virtual ~_IEventHander () {}

         virtual UINT32 getMask() const = 0 ;

         virtual void   onRegistered( const MsgRouteID &nodeID )
         {}
         virtual void   onPrimaryChange( BOOLEAN primary,
                                         SDB_EVENT_OCCUR_TYPE occurType )
         {}

   } ;
   typedef _IEventHander IEventHander ;

   /*
      _IEventHolder define
   */
   class _IEventHolder
   {
      public:
         _IEventHolder () {}
         virtual ~_IEventHolder () {}

         virtual INT32  regEventHandler( IEventHander *pHandler ) = 0 ;
         virtual void   unregEventHandler( IEventHander *pHandler ) = 0 ;
   } ;
   typedef _IEventHolder IEventHolder ;

   /*
      _IConfigHandle define
   */
   class _IConfigHandle
   {
      public:
         _IConfigHandle () {}
         virtual ~_IConfigHandle () {}

         virtual void   onConfigChange ( UINT32 changeID ) = 0 ;
         virtual INT32  onConfigInit () = 0 ;
         virtual void   onConfigSave () = 0 ;
   } ;
   typedef _IConfigHandle IConfigHandle ;

   /*
      _IParam define
   */
   class _IParam
   {
      public:
         _IParam() {}
         virtual ~_IParam() {}

      public:
         virtual  BOOLEAN hasField( const CHAR *pFieldName ) = 0 ;
         virtual  INT32   getFieldInt( const CHAR *pFieldName,
                                       INT32 &value,
                                       INT32 *pDefault = NULL ) = 0 ;
         virtual  INT32   getFieldStr( const CHAR *pFieldName,
                                       CHAR *pValue, UINT32 len,
                                       const CHAR *pDefault = NULL ) = 0 ;
         virtual  INT32   getFieldStr( const CHAR *pFieldName,
                                       std::string &strValue,
                                       const CHAR *pDefault = NULL ) = 0 ;

   } ;
   typedef _IParam IParam ;

   /*
      _IClient define
   */
   class _IClient : public SDBObject
   {
      public:
         _IClient() {}
         virtual ~_IClient() {}

      public:
         virtual SDB_CLIENT_TYPE clientType() const = 0 ;
         virtual const CHAR*     clientName() const = 0 ;

         virtual INT32        authenticate( MsgHeader *pMsg ) = 0 ;
         virtual INT32        authenticate( const CHAR *username,
                                            const CHAR *password ) = 0 ;
         virtual void         logout() = 0 ;
         virtual INT32        disconnect() = 0 ;

         virtual BOOLEAN      isAuthed() const = 0 ;
         virtual BOOLEAN      isConnected() const = 0 ;
         virtual BOOLEAN      isClosed() const = 0 ;

         virtual UINT16       getLocalPort() const = 0 ;
         virtual const CHAR*  getLocalIPAddr() const = 0 ;
         virtual UINT16       getPeerPort() const = 0 ;
         virtual const CHAR*  getPeerIPAddr() const = 0 ;
         virtual const CHAR*  getUsername() const = 0 ;
         virtual const CHAR*  getPassword() const = 0 ;

         virtual const CHAR*  getFromIPAddr() const = 0 ;
         virtual UINT16       getFromPort() const = 0 ;

   } ;
   typedef _IClient IClient ;

   /*
      _ISession define
   */
   class _ISession : public SDBObject
   {
      public:
         _ISession() {}
         virtual ~_ISession() {}

      public:
         virtual UINT64             identifyID() = 0 ;
         virtual UINT32             identifyTID() = 0 ;
         virtual UINT64             identifyEDUID() = 0 ;

         virtual const CHAR*        sessionName() const = 0 ;
         virtual SDB_SESSION_TYPE   sessionType() const = 0 ;
         virtual INT32              getServiceType() const = 0 ;
         virtual IClient*           getClient() = 0 ;

      protected:
         virtual void               _onAttach () {}
         virtual void               _onDetach () {}

   } ;
   typedef _ISession ISession ;

   /*
      _IControlBlock define
   */
   class _IControlBlock : public SDBObject, public _ISDBRoot
   {
      public:
         _IControlBlock () {}
         virtual ~_IControlBlock () {}

         virtual SDB_CB_TYPE cbType() const = 0 ;
         virtual const CHAR* cbName() const = 0 ;

         virtual INT32  init () = 0 ;
         virtual INT32  active () = 0 ;
         virtual INT32  deactive () = 0 ;
         virtual INT32  fini () = 0 ;
         virtual void   onConfigChange() {}
         virtual void   onConfigSave() {}

   } ;
   typedef _IControlBlock IControlBlock ;

   /*
      _IResource define
   */
   class _IResource
   {
      public:
         _IResource() {}
         virtual ~_IResource() {}

      public:
         virtual IParam*            getParam() = 0 ;
         virtual IControlBlock*     getCBByType( SDB_CB_TYPE type ) = 0 ;
         virtual BOOLEAN            isCBValue( SDB_CB_TYPE type ) const = 0 ;
         virtual void*              getOrgPointByType( SDB_CB_TYPE type ) = 0 ;

         virtual SDB_DB_STATUS      getDBStatus() const = 0 ;
         virtual const CHAR*        getDBStatusDesp() const = 0 ;
         virtual BOOLEAN            isShutdown() const = 0 ;
         virtual BOOLEAN            isNormal() const = 0 ;
         virtual BOOLEAN            isAvailable( INT32 *pCode = NULL ) const = 0 ;
         virtual INT32              getShutdownCode() const = 0 ;

         virtual UINT32             getDBMode() const = 0 ;
         virtual std::string        getDBModeDesp() const = 0 ;
         virtual BOOLEAN            isDBReadonly() const = 0 ;
         virtual BOOLEAN            isDBDeactivated() const = 0 ;

         virtual BOOLEAN            isInFlowControl() const = 0 ;

         virtual SDB_ROLE           getDBRole() const = 0 ;
         virtual const CHAR*        getDBRoleDesp() const = 0 ;

         virtual BOOLEAN            isPrimary() const = 0 ;
         virtual UINT16             getLocalPort() const = 0 ;
         virtual const CHAR*        getSvcname() const = 0 ;
         virtual const CHAR*        getDBPath() const = 0 ;
         virtual const CHAR*        getHostName() const = 0 ;
         virtual const CHAR*        getGroupName () const = 0 ;
         virtual UINT32             getNodeID() const = 0 ;
         virtual UINT32             getGroupID() const = 0 ;

         virtual UINT64             getStartTime() const = 0 ;
         virtual UINT64             getDBTick() const = 0 ;

         virtual void               getVersion( INT32 &ver,
                                                INT32 &subVer,
                                                INT32 &fixVer,
                                                INT32 &release,
                                                const CHAR **build ) const = 0 ;

         virtual BOOLEAN            isRestore() const = 0 ;

   } ;
   typedef _IResource IResource ;

}

#endif // SDB_INTERFACE_HPP__

