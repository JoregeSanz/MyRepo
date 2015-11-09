//
//  TypeDef.h
//  LingLing
//
//  Created by jyn on 3/8/14.
//
//

#ifndef LingLing_TypeDef_h
#define LingLing_TypeDef_h

#include <vector.h>

typedef unsigned char byte;
typedef enum {
	DEVICE_IPHONE4, DEVICE_IPHONE5, DEVICE_IPAD, DEVICE_IPAD_R
} DEVICE_TYPE;

typedef std::vector<std::string> CSVRow;
typedef CSVRow::const_iterator CSVRowCI;
typedef std::vector<CSVRow> CSVDatabase;
typedef CSVDatabase::const_iterator CSVDatabaseCI;

#endif
