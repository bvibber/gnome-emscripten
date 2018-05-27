#include <inttypes.h>
#include <stdio.h>
#include <ffi.h>

float call_a_float(void)
{
    printf("called call_a_float\n");
    return 1.0;
}

float call_a_float_with_a_float(float param) {
    return param + 1.0;
}

double call_double_double(double param) {
    return param + 1.0;
}

int64_t call_i64_with_i64(int64_t param) {
  return param + 1LL;
}

int main()
{
  printf("THIS IS MAIN\n");

  ffi_cif cif;
  ffi_type *args[1];
  void *values[1];
  char *s;
  ffi_arg rc;

  /* Initialize the argument info vectors */
  args[0] = &ffi_type_pointer;
  values[0] = &s;

  /* Initialize the cif */
  printf("calling...\n");
  if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 1,
		       &ffi_type_sint, args) == FFI_OK)
    {
        printf("puts pointer is %p\n", puts);

      s = "Hello World!";
      ffi_call(&cif, puts, &rc, values);
      /* rc now holds the result of the call to puts */

      /* values holds a pointer to the function's arg, so to
         call puts() again all we need to do is change the
         value of s */
      s = "This is cool!";
      ffi_call(&cif, puts, &rc, values);
  } else {
      printf("failed to make puts\n");
  }

  {
    ffi_cif cif_float;
    ffi_type *float_args[1];
    void *float_vals[1];

    if (ffi_prep_cif(&cif_float, FFI_DEFAULT_ABI, 0, &ffi_type_float, float_args) == FFI_OK) {
        float ret_float;
        float expected = 1.0;
        ffi_call(&cif_float, call_a_float, &ret_float, float_vals);
        printf("call_a_float pointer is %p\n", call_a_float);
        printf("Got %f back from call_a_float, expected %f\n", ret_float, expected);
        if (ret_float != expected) {
            return 1;
        }
    } else {
        printf("failed to make call_a_float\n");
    }
  }

  {
    ffi_cif cif_float;
    ffi_type *float_args[1];
    void *float_vals[1];

    float float_in = 99.5;
    float_args[0] = &ffi_type_float;
    float_vals[0] = &float_in;

    if (ffi_prep_cif(&cif_float, FFI_DEFAULT_ABI, 1, &ffi_type_float, float_args) == FFI_OK) {
        float ret_float;
        float expected = 100.5;
        ffi_call(&cif_float, call_a_float_with_a_float, &ret_float, float_vals);
        printf("call_a_float_with_a_float pointer is %p\n", call_a_float_with_a_float);
        printf("Got %lf back from call_a_float_with_a_float, expected %lf\n", ret_float, expected);
        if (ret_float != expected) {
            return 1;
        }
    } else {
        printf("failed to make call_a_float_with_a_float\n");
    }
  }

  {
    ffi_cif cif_dbl;
    ffi_type *dbl_args[1];
    void *dbl_vals[1];

    double dbl_in = 99.5;
    dbl_args[0] = &ffi_type_double;
    dbl_vals[0] = &dbl_in;

    if (ffi_prep_cif(&cif_dbl, FFI_DEFAULT_ABI, 1, &ffi_type_double, dbl_args) == FFI_OK) {
        double ret_dbl;
        double expected = 100.5;
        ffi_call(&cif_dbl, call_double_double, &ret_dbl, dbl_vals);
        printf("call_double_double pointer is %p\n", call_double_double);
        printf("Got %lf back from call_double_double, expected %lf\n", ret_dbl, expected);
        if (ret_dbl != expected) {
            return 1;
        }
    } else {
        printf("failed to make call_double_double\n");
    }
  }


    ffi_cif cif_with_i64;
    ffi_type *i64_args[1];
    void *i64_vals[1];

    int64_t i64_in = 1000000000000LL;
    i64_args[0] = &ffi_type_sint64;
    i64_vals[0] = &i64_in;

    if (ffi_prep_cif(&cif_with_i64, FFI_DEFAULT_ABI, 1, &ffi_type_sint64, i64_args) == FFI_OK) {
        int64_t ret_i64;
        int64_t expected = 1000000000001LL;
        ffi_call(&cif_with_i64, call_i64_with_i64, &ret_i64, i64_vals);
        printf("call_i64_with_i64 pointer is %p\n", call_i64_with_i64);
        printf("Got %lld back from call_i64_with_i64, expected %lld\n", ret_i64, expected);
        if (ret_i64 != expected) {
            return 1;
        }
    } else {
        printf("failed to make call_i64_with_i64\n");
    }

  return 0;
}
