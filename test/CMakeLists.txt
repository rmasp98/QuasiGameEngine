#add_test

add_executable(quasiGameEngineTest 
  main_test.cc
  # utils/memory/heap_allocator_test.cc
  # utils/memory/heap_manager_test.cc
  utils/memory/heap_block_list_test.cc
  utils/memory/heap_page_list_test.cc
)

target_link_libraries(quasiGameEngineTest
  gtest
  qgeUtils
)
