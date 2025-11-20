#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "c_fms.h"

void test_module_is_initialized();

int main()
{

  test_module_is_initialized();
  cFMS_end();
  return EXIT_SUCCESS;

}

void test_module_is_initialized()
{

  cFMS_init(NULL, NULL, NULL, NULL, NULL);
  
  bool module_is_initialized = c_fms_is_initialized();
  
  if(!module_is_initialized){
    cFMS_error(FATAL, "module is not initialized");
    exit(EXIT_FAILURE);
  }
  
};
