#indef _BUFFER_H_
#define _BUFFER_H_


#define BUFFER_EMPTY 0
#define BUFFER_NOT_EMPTY 1
#define BUFFER_ERROR 1
#define BUFFER_OK 0



struct buffer_elem{
	buffer_elem *prev_elem;
	uint8_t elem_data;
};
typedef struct buffer_element buffer_element;


struct buffer{
	buffer_elem *top_elem;
	buffer_elem *bottom_elem;
};
typedef struct buffer buffer;



buffer *buffer_create(void);
void buffer_delete(buffer *buffer_ptr);
uint8_t buffer_check_empty(buffer *buffer_ptr);
uint8_t buffer_enqueue(buffer *buffer_ptr, uint8_t data);
uint8_t buffer_dequeue(buffer *buffer_ptr);


#endif
