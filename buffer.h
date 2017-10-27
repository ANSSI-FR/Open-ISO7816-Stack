#indef _STACK_H_
#define _STACK_H_

struct buffer_elem{
	buffer_elem *prev_elem;
	buffer_elem *next_elem;
	uint8_t elem_data;
};
typedef struct buffer_element buffer_element;


struct buffer{
	buffer_elem *first_elem;
	buffer_elem *last_elem;
};
typedef struct buffer buffer;




#endif
