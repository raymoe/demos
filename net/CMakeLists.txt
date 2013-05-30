set(cppsrcs
epoll_demo1
)
foreach(src ${cppsrcs})
    add_executable(${src} ${src}.cpp)
    #target_link_libraries()
endforeach()
