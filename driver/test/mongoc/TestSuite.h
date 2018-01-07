/*
 * Copyright 2014 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef TEST_SUITE_H
#define TEST_SUITE_H


#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


#ifdef ASSERT
# undef ASSERT
#endif
#define ASSERT assert


#ifdef ASSERT_CMPINT
# undef ASSERT_CMPINT
#endif

#define ASSERT_CMPPTR(a, eq, b) \
   do { \
      if (!((a) eq (b))) { \
         fprintf(stderr, "FAIL\n\nAssert Failure: %p %s %p\n" \
                         "%s:%d  %s()\n", \
                         a, #eq, b, \
                         __FILE__, __LINE__, __FUNCTION__); \
         output_report(&suite);\
         abort(); \
      } \
   } while (0)

#define ASSERT_CMPINT(a, eq, b) \
   do { \
      if (!((a) eq (b))) { \
         fprintf(stderr, "FAIL\n\nAssert Failure: %d %s %d\n" \
                         "%s:%d  %s()\n", \
                         a, #eq, b, \
                         __FILE__, __LINE__, __FUNCTION__); \
         output_report(&suite);\
         abort(); \
      } \
   } while (0)


#define ASSERT_CMPSTR(a, b) \
   do { \
      if (((a) != (b)) && !!strcmp((a), (b))) { \
         fprintf(stderr, "FAIL\n\nAssert Failure: \"%s\" != \"%s\"\n", \
                         a, b); \
         output_report(&suite);\
         abort(); \
      } \
   } while (0)


typedef void (*TestFunc) (void);
typedef struct _Test Test;
typedef struct _TestSuite TestSuite;


struct _Test
{
   Test *next;
   char *name;
   TestFunc func;
   int exit_code;
   unsigned seed;
   int (*check) (void);
};


struct _TestSuite
{
   char *prgname;
   char *name;
   char *testname;
   Test *tests;
   FILE *outfile;
   int flags;
   bool generatexmlreport;
   int testcasesnum;
   int successfulnum;
   int failurenum;
   int errornum;
   void * reportnodes;
};

typedef struct _XmlNode XmlNode;
struct _XmlNode
{
   const char* name;
   const char *content;
   XmlNode *next;
};

void output_report(TestSuite *suite);
void Add_XmlNode(TestSuite *suite, const char*name, const char* content);
void Destrory_XmlNodes(TestSuite *suite);
void TestSuite_Init    (TestSuite *suite,
                        const char *name,
                        int argc,
                        char **argv);
void TestSuite_Add     (TestSuite *suite,
                        const char *name,
                        TestFunc func);
void TestSuite_AddFull (TestSuite *suite,
                        const char *name,
                        TestFunc func,
                        int (*check) (void));
int  TestSuite_Run     (TestSuite *suite);
void TestSuite_Destroy (TestSuite *suite);

#ifdef __cplusplus
}
#endif


#endif /* TEST_SUITE_H */
