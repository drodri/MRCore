IF(WIN32)
find_path(BOOST_INCLUDE_DIR boost/config.hpp
	C:/usr/*
	C:/usr/*/*
	)
ELSE(WIN32)
find_path(BOOST_INCLUDE_DIR boost/config.hpp
	/usr/local/*
	/usr/local/*/*
)
ENDIF(WIN32)


if(BOOST_INCLUDE_DIR)
	set(BOOST_FOUND "YES")
endif(BOOST_INCLUDE_DIR)
