add_library(sqlpp11-connector-sqlite3 STATIC
		connection.cpp
		bind_result.cpp
		prepared_statement.cpp
		detail/connection_handle.cpp)
	      
if ("${CMAKE_SYSTEM_NAME}" MATCHES "^Android$")
else ()
  target_compile_options(sqlpp11-connector-sqlite3 PRIVATE "-fPIC")
endif ()
install(TARGETS sqlpp11-connector-sqlite3 DESTINATION lib)
