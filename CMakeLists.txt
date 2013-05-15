#--------------------------------------------------------
# The CMakeLists.txt for:                       pControls
# Author(s):                              Janille Maragh
#--------------------------------------------------------

# FILE(GLOB SRC *.cpp)

SET(SRC
  Serial.cpp
  Controls.cpp
  Controls_Info.cpp
  main.cpp
)



ADD_EXECUTABLE(pControls ${SRC})

TARGET_LINK_LIBRARIES(pControls
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

