# CMake generated Testfile for 
# Source directory: E:/2024_project/plagiarism_check/cppjieba-master
# Build directory: E:/2024_project/plagiarism_check/cppjieba-master
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(./demo "./demo")
  set_tests_properties(./demo PROPERTIES  _BACKTRACE_TRIPLES "E:/2024_project/plagiarism_check/cppjieba-master/CMakeLists.txt;22;ADD_TEST;E:/2024_project/plagiarism_check/cppjieba-master/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(./demo "./demo")
  set_tests_properties(./demo PROPERTIES  _BACKTRACE_TRIPLES "E:/2024_project/plagiarism_check/cppjieba-master/CMakeLists.txt;22;ADD_TEST;E:/2024_project/plagiarism_check/cppjieba-master/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(./demo "./demo")
  set_tests_properties(./demo PROPERTIES  _BACKTRACE_TRIPLES "E:/2024_project/plagiarism_check/cppjieba-master/CMakeLists.txt;22;ADD_TEST;E:/2024_project/plagiarism_check/cppjieba-master/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(./demo "./demo")
  set_tests_properties(./demo PROPERTIES  _BACKTRACE_TRIPLES "E:/2024_project/plagiarism_check/cppjieba-master/CMakeLists.txt;22;ADD_TEST;E:/2024_project/plagiarism_check/cppjieba-master/CMakeLists.txt;0;")
else()
  add_test(./demo NOT_AVAILABLE)
endif()
subdirs("test")
