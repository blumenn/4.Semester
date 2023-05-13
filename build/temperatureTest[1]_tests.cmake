add_test([=[Test_production.Test_something]=]  [==[C:/Users/ander/Kodemaskinen/4. Semester/SEP4/4.Semester/build/Debug/temperatureTest.exe]==] [==[--gtest_filter=Test_production.Test_something]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[Test_production.Test_something]=]  PROPERTIES WORKING_DIRECTORY [==[C:/Users/ander/Kodemaskinen/4. Semester/SEP4/4.Semester/build]==] SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  temperatureTest_TESTS Test_production.Test_something)
