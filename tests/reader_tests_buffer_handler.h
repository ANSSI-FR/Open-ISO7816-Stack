#ifndef __READER_TESTS_BUFFER_HANDLER_H__
#define __READER_TESTS_BUFFER_HANDLER_H__


#include "reader_t1_buffer_handler.h"





void test_READER_BUFFER_all(void);


void test_READER_T1_BUFFER_Init_shouldSetInitValues(void);
void test_READER_T1_BUFFER_Clear_shouldReinitValues(void);
void test_READER_T1_BUFFER_SetLength_shouldWork(void);
void test_T1_BUFFER_SetLength_shoulCheckMaxSize(void);
void test_READER_T1_BUFFER_IncLength_shouldWork(void);
void test_READER_T1_BUFFER_IncLength_shouldCheckMaxSize(void);
void test_READER_T1_BUFFER_DecLength_shouldWork(void);
void test_READER_T1_BUFFER_DecLength_shouldCheckMinSize(void);
void test_READER_T1_BUFFER_PlacesLeft_shouldWork(void);
void test_READER_T1_BUFFER_IsFull_shouldWork(void);
void test_READER_T1_BUFFER_PlacesLeft_shouldWorkWhenFull(void);
void test_READER_T1_BUFFER_IsEmpty_shouldWork(void);
void test_READER_T1_BUFFER_Stack_stackAndDequeueShouldWorkWithSBlock_case1(void);
void test_READER_T1_BUFFER_Stack_stackAndDequeueShouldWorkWithSBlock_case2(void);
void test_READER_T1_BUFFER_Enqueue_enqueueAndDequeueShouldWorkWithSBlock_case1(void);
void test_READER_T1_BUFFER_Enqueue_enqueueAndDequeueShouldWorkWithSBlock_case2(void);
void test_READER_T1_BUFFER_Dequeue_shouldCheckIfEmpty(void);
void test_READER_T1_BUFFER_Stack_shouldCheckIfFull(void);
void test_READER_T1_BUFFER_Enqueue_shouldCheckIfFull(void);






#endif
