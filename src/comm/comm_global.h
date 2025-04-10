#pragma once

#ifndef OOSU_COMM_STATIC

#    ifdef OOSU_COMM_EXPORTS
#        define COMM_EXPORT __declspec(dllexport)
#    else
#        define COMM_EXPORT __declspec(dllimport)
#    endif

#else

#    define COMM_EXPORT

#endif
