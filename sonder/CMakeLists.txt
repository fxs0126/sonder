cmake_minimum_required(VERSION 2.8)

include_directories(include)
message(STATUS"add module common")
subdirs(common)
message(STATUS"add module messages")
subdirs(messages)
message(STATUS"add module options")
subdirs(options)
