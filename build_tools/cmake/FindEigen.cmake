IF(WIN32)
find_path(EIGEN_INCLUDE_DIR signature_of_eigen3_matrix_library
	C:/usr/*
	C:/usr/*/*
	)
ELSE(WIN32)
find_path(EIGEN_INCLUDE_DIR signature_of_eigen3_matrix_library
	/usr/local/*
	/usr/local/*/*
)
ENDIF(WIN32)


if(EIGEN_INCLUDE_DIR)
	set(EIGEN_FOUND "YES")
endif(EIGEN_INCLUDE_DIR)
