/**
 * Template.cpp
 * This file is part of the YATE Project http://YATE.null.ro
 *
 * Yet Another Telephony Engine - a fully featured software PBX and IVR
 * Copyright (C) 2025 Null Team
 *
 * This software is distributed under multiple licenses;
 * see the COPYING file in the main directory for licensing
 * information for this specific distribution.
 *
 * This use of this software may be subject to additional restrictions.
 * See the LEGAL file in the main directory for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "yateclass.h"

using namespace TelEngine;

//
// Numeric vectors (inherit BaseNumVectorTemplate)
//

const unsigned int UintVector::s_minVal = 0;
const unsigned int UintVector::s_maxVal = UINT_MAX;
const unsigned long UlongVector::s_minVal = 0;
const unsigned long UlongVector::s_maxVal = ULONG_MAX;
const uint8_t Uint8Vector::s_minVal = 0;
const uint8_t Uint8Vector::s_maxVal = 0xff;
const uint16_t Uint16Vector::s_minVal = 0;
const uint16_t Uint16Vector::s_maxVal = 0xffff;
const uint32_t Uint32Vector::s_minVal = 0;
const uint32_t Uint32Vector::s_maxVal = 0xffffffff;
const uint64_t Uint64Vector::s_minVal = 0;
const uint64_t Uint64Vector::s_maxVal = 0xffffffffffffffff;
const int IntVector::s_minVal = INT_MIN;
const int IntVector::s_maxVal = INT_MAX;
const long LongVector::s_minVal = LONG_MIN;
const long LongVector::s_maxVal = LONG_MAX;
const int8_t Int8Vector::s_minVal = -128;
const int8_t Int8Vector::s_maxVal = 127;
const int16_t Int16Vector::s_minVal = -32768;
const int16_t Int16Vector::s_maxVal = 32767;
const int32_t Int32Vector::s_minVal = -2147483648;
const int32_t Int32Vector::s_maxVal = 2147483647;
const int64_t Int64Vector::s_minVal = LLONG_MIN;
const int64_t Int64Vector::s_maxVal = LLONG_MAX;

#define DECLARE_TEMPL_NUM_SIGNED(ClsName,isSigned) static const bool s_Signed##ClsName = isSigned
DECLARE_TEMPL_NUM_SIGNED(UintVector,false);
DECLARE_TEMPL_NUM_SIGNED(UlongVector,false);
DECLARE_TEMPL_NUM_SIGNED(Uint8Vector,false);
DECLARE_TEMPL_NUM_SIGNED(Uint16Vector,false);
DECLARE_TEMPL_NUM_SIGNED(Uint32Vector,false);
DECLARE_TEMPL_NUM_SIGNED(Uint64Vector,false);
DECLARE_TEMPL_NUM_SIGNED(IntVector,true);
DECLARE_TEMPL_NUM_SIGNED(LongVector,true);
DECLARE_TEMPL_NUM_SIGNED(Int8Vector,true);
DECLARE_TEMPL_NUM_SIGNED(Int16Vector,true);
DECLARE_TEMPL_NUM_SIGNED(Int32Vector,true);
DECLARE_TEMPL_NUM_SIGNED(Int64Vector,true);
#undef DECLARE_TEMPL_NUM_SIGNED

#define DECLARE_TEMPL_NUM_EMPTY_OBJ(Type) static const Type s_Empty##Type
DECLARE_TEMPL_NUM_EMPTY_OBJ(UintVector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(UlongVector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(Uint8Vector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(Uint16Vector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(Uint32Vector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(Uint64Vector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(IntVector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(LongVector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(Int8Vector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(Int16Vector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(Int32Vector);
DECLARE_TEMPL_NUM_EMPTY_OBJ(Int64Vector);
#undef DECLARE_TEMPL_NUM_EMPTY_OBJ

template <class Type> inline Type str2num(bool typeSigned, const String& str, Type defVal,
    Type minVal, Type maxVal, bool clamp = true)
{
    return typeSigned ? (Type)str.toInt64(defVal,0,minVal,maxVal,clamp) :
	(Type)str.toUInt64(defVal,0,minVal,maxVal,clamp);
}


#define IMPL_TEMPL_NUM_FUNC_parse(ClsName,Type) \
int ClsName::parse(const String& str, Type defVal, Type minVal, Type maxVal, \
    unsigned int flags, int loc) \
{ \
    if (!str) \
	return 0; \
    bool unique = 0 != (flags & Unique); \
    ObjList values; \
    str.split(values,',',false,true,unique); \
    if (!values.skipNull()) \
	return 0; \
    bool zero = 0 == (flags & NoZero); \
    bool range = 0 != (flags & Range); \
    bool clamp = 0 != (flags & Clamp); \
    unsigned int n = 0; \
    if (loc < 0) \
	loc = length(); \
    for (ObjList* o = values.skipNull(); o; o = o->skipNext()) { \
	String& s = *static_cast<String*>(o->get()); \
	if (range) { \
	    int pos = s.find("-"); \
	    if (pos >= 0) { \
		Type v = str2num(s_Signed##ClsName,s.substr(0,pos),defVal,minVal,maxVal,clamp); \
		Type end = str2num(s_Signed##ClsName,s.substr(pos + 1),defVal,minVal,maxVal,clamp); \
		for (; v <= end; v++) { \
		    if (!((v || zero) && !(unique && includes(v)))) \
			continue; \
		    if (!insert(v,loc++)) \
			return -(int)(n + 1); \
		    n++; \
		} \
		continue; \
	    } \
	} \
	Type v = str2num(s_Signed##ClsName,s,defVal,minVal,maxVal,clamp); \
	if (!((v || zero) && !(unique && includes(v)))) \
	    continue; \
	if (!insert(v,loc++)) \
	    return -(int)(n + 1); \
	n++; \
    } \
    return n; \
}
IMPL_TEMPL_NUM_FUNC_parse(UintVector,unsigned int)
IMPL_TEMPL_NUM_FUNC_parse(UlongVector,unsigned long)
IMPL_TEMPL_NUM_FUNC_parse(Uint8Vector,uint8_t)
IMPL_TEMPL_NUM_FUNC_parse(Uint16Vector,uint16_t)
IMPL_TEMPL_NUM_FUNC_parse(Uint32Vector,uint32_t)
IMPL_TEMPL_NUM_FUNC_parse(Uint64Vector,uint64_t)
IMPL_TEMPL_NUM_FUNC_parse(IntVector,int)
IMPL_TEMPL_NUM_FUNC_parse(LongVector,long)
IMPL_TEMPL_NUM_FUNC_parse(Int8Vector,int8_t)
IMPL_TEMPL_NUM_FUNC_parse(Int16Vector,int16_t)
IMPL_TEMPL_NUM_FUNC_parse(Int32Vector,int32_t)
IMPL_TEMPL_NUM_FUNC_parse(Int64Vector,int64_t)
#undef IMPL_TEMPL_NUM_FUNC_parse


// NOTE: compact is not implemented for signed type
#define IMPL_TEMPL_NUM_FUNC_dump(ClsName) \
String& ClsName::dump(String& str, const char* sep, bool compact) const \
{ \
    return bufDump(str,data(),length(),sep,compact && !s_Signed##ClsName); \
}
IMPL_TEMPL_NUM_FUNC_dump(UintVector)
IMPL_TEMPL_NUM_FUNC_dump(UlongVector)
IMPL_TEMPL_NUM_FUNC_dump(Uint8Vector)
IMPL_TEMPL_NUM_FUNC_dump(Uint16Vector)
IMPL_TEMPL_NUM_FUNC_dump(Uint32Vector)
IMPL_TEMPL_NUM_FUNC_dump(Uint64Vector)
IMPL_TEMPL_NUM_FUNC_dump(IntVector)
IMPL_TEMPL_NUM_FUNC_dump(LongVector)
IMPL_TEMPL_NUM_FUNC_dump(Int8Vector)
IMPL_TEMPL_NUM_FUNC_dump(Int16Vector)
IMPL_TEMPL_NUM_FUNC_dump(Int32Vector)
IMPL_TEMPL_NUM_FUNC_dump(Int64Vector)
#undef IMPL_TEMPL_NUM_FUNC_dump


#define IMPL_TEMPL_NUM_FUNC_empty(ClsName) const ClsName& ClsName::empty() { return s_Empty##ClsName; }
IMPL_TEMPL_NUM_FUNC_empty(UintVector)
IMPL_TEMPL_NUM_FUNC_empty(UlongVector)
IMPL_TEMPL_NUM_FUNC_empty(Uint8Vector)
IMPL_TEMPL_NUM_FUNC_empty(Uint16Vector)
IMPL_TEMPL_NUM_FUNC_empty(Uint32Vector)
IMPL_TEMPL_NUM_FUNC_empty(Uint64Vector)
IMPL_TEMPL_NUM_FUNC_empty(IntVector)
IMPL_TEMPL_NUM_FUNC_empty(LongVector)
IMPL_TEMPL_NUM_FUNC_empty(Int8Vector)
IMPL_TEMPL_NUM_FUNC_empty(Int16Vector)
IMPL_TEMPL_NUM_FUNC_empty(Int32Vector)
IMPL_TEMPL_NUM_FUNC_empty(Int64Vector)
#undef IMPL_TEMPL_NUM_FUNC_empty

/* vi: set ts=8 sw=4 sts=4 noet: */
