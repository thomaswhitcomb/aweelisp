/* file: minunit.h */
 #define mu_assert(message, test) do { if (!(test)) {tests_error++; puts(message); } } while (0)
 #define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
 extern int tests_run;
 extern int tests_error;
 char *all_tests();


