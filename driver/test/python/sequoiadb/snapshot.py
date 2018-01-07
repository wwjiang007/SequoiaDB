#modify: Ting YU 2015-12-11
import pysequoiadb
from pysequoiadb import client
from pysequoiadb import collectionspace
from pysequoiadb import collection
from pysequoiadb import cursor
from pysequoiadb import const
from pysequoiadb.error import (SDBTypeError, SDBBaseError, SDBEndOfCursor)
import sys,getopt
import traceback

hostname=None
service=None

# parase the args
def parse_option():

   if len( sys.argv ) < 2:
      usage()
      sys.exit(1)
   
   try:  
      opts, args = getopt.getopt( sys.argv[1:], 'hH:p:', ['help'] )
   except getopt.GetoptError, err:
      print str( err )
      usage()
      sys.exit(1)
   
   global hostname, service
   for op, value in opts:
      if op == '-H':
         hostname = value
      elif op == '-p':
         service = int( value )
      elif op in ('-h', '--help'):
         usage()
         sys.exit()
      else:
         print 'arguments error'
         usage()
         sys.exit(1)
         
def usage():
   print 'Command options:'
   print '-h,--help  help'
   print '-H   arg   hostname'
   print '-p   arg   coord_port'

def createCL( cs_name, cl_name ):
   print '---begin to drop cs in ready'
   try:
      db.drop_collection_space( cs_name )
   except SDBBaseError, e:
      if ( -34 != e.code ):         
         raise e
                   
   print '---begin to create cs cl'
   cs = db.create_collection_space( cs_name )     
   cl = cs.create_collection( cl_name, {"ReplSize":0} )
   
   return cl
   
def snapshot_5( cl ):   
   print '---begin to snapshot(5)'
   cond = {'Name': cs_name }
   cursor = db.get_snapshot( 5, condition=cond )
    
   i = 0        
   while True:
      try:
         record = cursor.next()
         i = i + 1
      except SDBEndOfCursor :
         break
      except SDBBaseError :
         raise e
   if( i != 1 ):
      print 'exeute: db.get_snapshot( 5, condition=%s )' % ( cond )
      print 'return record number, expect: 1, actual: %d' % ( i )
      raise  Exception( 'COUNT_ERROR' ) 
      
def clean( cs_name ):
   print '---begin to drop cs in finally'
   try:
      db.drop_collection_space( cs_name )
   except SDBBaseError, e:
      if ( -34 != e.code ):
         pysequoiadb._print(e.detail)              
         raise e
      
if __name__ == "__main__":   
   try:    
      parse_option()
      
      cs_name = "pydriver_snapshot_cs"
      cl_name = "pydriver_snapshot_cl"
            
      db = client( hostname, service )
      cl = createCL( cs_name, cl_name )
      
      # main
      snapshot_5( cl )
      
   except SDBBaseError, e:
      pysequoiadb._print( e.detail )
      raise e  
            
   finally:  
      if( locals().has_key('db') ):                    
         clean( cs_name )     
         db.disconnect()
         del db 