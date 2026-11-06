// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from iris_msgs:msg/LaneState.idl
// generated code does not contain a copyright notice
#include "iris_msgs/msg/detail/lane_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"

bool
iris_msgs__msg__LaneState__init(iris_msgs__msg__LaneState * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    iris_msgs__msg__LaneState__fini(msg);
    return false;
  }
  // found
  // error
  // heading
  // n_blobs
  return true;
}

void
iris_msgs__msg__LaneState__fini(iris_msgs__msg__LaneState * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // found
  // error
  // heading
  // n_blobs
}

bool
iris_msgs__msg__LaneState__are_equal(const iris_msgs__msg__LaneState * lhs, const iris_msgs__msg__LaneState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // found
  if (lhs->found != rhs->found) {
    return false;
  }
  // error
  if (lhs->error != rhs->error) {
    return false;
  }
  // heading
  if (lhs->heading != rhs->heading) {
    return false;
  }
  // n_blobs
  if (lhs->n_blobs != rhs->n_blobs) {
    return false;
  }
  return true;
}

bool
iris_msgs__msg__LaneState__copy(
  const iris_msgs__msg__LaneState * input,
  iris_msgs__msg__LaneState * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // found
  output->found = input->found;
  // error
  output->error = input->error;
  // heading
  output->heading = input->heading;
  // n_blobs
  output->n_blobs = input->n_blobs;
  return true;
}

iris_msgs__msg__LaneState *
iris_msgs__msg__LaneState__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  iris_msgs__msg__LaneState * msg = (iris_msgs__msg__LaneState *)allocator.allocate(sizeof(iris_msgs__msg__LaneState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(iris_msgs__msg__LaneState));
  bool success = iris_msgs__msg__LaneState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
iris_msgs__msg__LaneState__destroy(iris_msgs__msg__LaneState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    iris_msgs__msg__LaneState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
iris_msgs__msg__LaneState__Sequence__init(iris_msgs__msg__LaneState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  iris_msgs__msg__LaneState * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(iris_msgs__msg__LaneState)) {
      return false;
    }
    data = (iris_msgs__msg__LaneState *)allocator.zero_allocate(size, sizeof(iris_msgs__msg__LaneState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = iris_msgs__msg__LaneState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        iris_msgs__msg__LaneState__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
iris_msgs__msg__LaneState__Sequence__fini(iris_msgs__msg__LaneState__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      iris_msgs__msg__LaneState__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

iris_msgs__msg__LaneState__Sequence *
iris_msgs__msg__LaneState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  iris_msgs__msg__LaneState__Sequence * array = (iris_msgs__msg__LaneState__Sequence *)allocator.allocate(sizeof(iris_msgs__msg__LaneState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = iris_msgs__msg__LaneState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
iris_msgs__msg__LaneState__Sequence__destroy(iris_msgs__msg__LaneState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    iris_msgs__msg__LaneState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
iris_msgs__msg__LaneState__Sequence__are_equal(const iris_msgs__msg__LaneState__Sequence * lhs, const iris_msgs__msg__LaneState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!iris_msgs__msg__LaneState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
iris_msgs__msg__LaneState__Sequence__copy(
  const iris_msgs__msg__LaneState__Sequence * input,
  iris_msgs__msg__LaneState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(iris_msgs__msg__LaneState)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(iris_msgs__msg__LaneState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    iris_msgs__msg__LaneState * data =
      (iris_msgs__msg__LaneState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!iris_msgs__msg__LaneState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          iris_msgs__msg__LaneState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!iris_msgs__msg__LaneState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
