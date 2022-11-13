function(ProjectLog message)
    message(STATUS "[${CMAKE_PROJECT_NAME}] ${message}")
endfunction()

function(ProjectWarning message)
    message(WARNING "[${CMAKE_PROJECT_NAME}] ${message}")
endfunction()
