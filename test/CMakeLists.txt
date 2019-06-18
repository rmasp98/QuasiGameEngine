#add_test

add_executable(quasiGameEngineTest 
  main_test.cc
  utils/data_types/qge_queue_test.cc
  utils/logging/log_worker_test.cc
  utils/logging/log_data_test.cc
  utils/logging/log_source_test.cc
  # utils/memory/heap_allocator_test.cc
  # utils/memory/heap_manager_test.cc
  utils/memory/heap_block_list_test.cc
  utils/memory/heap_free_list_test.cc
  utils/memory/heap_page_list_test.cc
)

target_link_libraries(quasiGameEngineTest
  gtest
  qgeUtils
  pthread
)
