#pragma once
#include "dataType.h"
#include "middlewareCommon.h"

MIDDLEWARE_IMPROCESS_API extern bool readInterParaFromXml(ParaInter& _para, string _xml_path);
MIDDLEWARE_IMPROCESS_API extern bool writeInterParaToXml(ParaInter& _para, string _xml_path);
MIDDLEWARE_IMPROCESS_API extern bool MiddleWriteInterParaToXml(ParaInter& _para_inter, string _path);
MIDDLEWARE_IMPROCESS_API extern bool MiddleReadInterParaFromXml(ParaInter& _para_inter, string _path);

MIDDLEWARE_IMPROCESS_API extern bool readParaFromXml(MiddlewareImprocessSeq::ParaIn& _para, string _xml_path);
MIDDLEWARE_IMPROCESS_API extern bool writeParaToXml(MiddlewareImprocessSeq::ParaIn& _para, string _xml_path);