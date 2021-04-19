#pragma once
#include "Def_globali.h"

// STRINGS DEDICATED TO PDF BATCH REPORT GENERATION
// sembra funzionare
// attenzione a salvare il file in VS con sava as e specificare la codifica 
// CHINESE SIMPLIFIED (GB2312) - CODEPAGE 936
// se appare dialog che chiede se salvare in Unicode rispondere SI (solo questo documento)

// font = HPDF_GetFont( pdf, "SimSun", "GB-EUC-H" ); // ca uguale a CP936 - simplified chinese (no traditional)
// HPDF_Page_SetFontAndSize( page, font, 18 );

/*

static char* string_8000[NLANGS] = {
  "BATCH REPORT",
  "���α���",
  "@" };

static char* string_8006[NLANGS] = {
  "Page %d/%d",
  "ҳ %d/%d",
  "@ %d/%d" };

static char* string_8010[NLANGS] = {
  "Machine ID ____________ %s",
  "������� _______________ %s",
  "@" };

static char* string_8012[NLANGS] = {
  "Machine ID",
  "�������",
  "@" };


static char* string_8020[NLANGS] = {
  "Current recipe ________ %s",
  "��ǰ�䷽ _______________  %s",
  "@" };

static char* string_8021[NLANGS] = {
  "Current recipe",
  "��ǰ�䷽",
  "@" };

static char* string_8022[NLANGS] = {
  "Batch name ____________ %s",
  "�������� _______________ %s",
  "@" };

static char* string_8023[NLANGS] = {
  "Batch name",
  "��������",
  "@" };

static char* string_8024[NLANGS] = {
  "Batch operator ________ %s",
  "���β���Ա      ________ %s",
  "@" };

static char* string_8025[NLANGS] = {
  "Batch operator",
  "���β���Ա",
  "@" };

static char* string_8026[NLANGS] = {
  "Batch description _____ %s",
  "�������� ______________ %s",
  "@" };

static char* string_8027[NLANGS] = {
  "Batch description",
  "��������",
  "@" };

static char* string_8028[NLANGS] = {
  "Batch start ___________ %s",
  "��ʼ���� ______________ %s",
  "@" };

static char* string_8029[NLANGS] = {
  "Batch start",
  "��ʼ����",
  "@" };

static char* string_8030[NLANGS] = {
  "Batch end _____________ %s",
  "�������� ______________ %s",
  "@" };

static char* string_8031[NLANGS] = {
  "Batch end",
  "��������",
  "@" };

static char* string_8032[NLANGS] = {
  "Batch status __________ %s",
  "����״̬ _______________ %s",
  "@" };

static char* string_8033[NLANGS] = {
  "Batch status",
  "����״̬",
  "@" };

static char* string_8034[NLANGS] = {
  "RUNNING",
  "����",
  "@" };

static char* string_8036[NLANGS] = {
  "NOT ACTIVE",
  "δ����",
  "@" };

static char* string_8040[NLANGS] = {
  "Batch responsible: ____________________ Signature: ____________________",
  "���θ�����: __________________________  ǩ��: _____________________________",
  "@" };

static char* string_8046[NLANGS] = {
  "Unable to read data from PLC",
  "���ܴ�PLC��ȡ����",
  "@" };

static char* string_8050[NLANGS] = {
  "OVERALL PRODUCTION STATISTICS",
  "��Ʒͳ��",
  "@" };

static char* string_8052[NLANGS] = {
  "NO PROCESSED PRODUCT",
  "�޼���Ʒ",
  "@" };
  
static char* string_8055[NLANGS] = {
  "Processed products",
  "����Ʒ",
  "@" };

static char* string_8057[NLANGS] = {
  "Accepted products",
  "�ϸ�Ʒ",
  "@" };

static char* string_8059[NLANGS] = {
  "Rejected products",
  "���ϸ�Ʒ",
  "@" };

static char* string_8061[NLANGS] = {
  "Products rejected on tray 1",
  "����1�޳���Ʒ",
  "@" };

static char* string_8063[NLANGS] = {
  "Products rejected on tray 2",
  "����2�޳���Ʒ",
  "@" };

static char* string_8065[NLANGS] = {
  "Particle rejects",
  "�����޳�",
  "@" };

static char* string_8067[NLANGS] = {
  "Particle 1 rejects",
  "����1�޳�",
  "@" };

static char* string_8069[NLANGS] = {
  "Particle 2 rejects",
  "����2�޳�",
  "@" };

static char* string_8071[NLANGS] = {
  "Particle 3 rejects",
  "����3�޳�",
  "@" };

static char* string_8073[NLANGS] = {
  "Filling level rejects",
  "Һλ�޳�",
  "@" };

static char* string_8075[NLANGS] = {
  "Filling level 1 rejects",
  "Һλ1�޳�",
  "@" };

static char* string_8077[NLANGS] = {
  "Filling level 2 rejects",
  "Һλ2�޳�",
  "@" };

static char* string_8079[NLANGS] = {
  "Cosmetic rejects",
  "����޳�",
  "@" };

static char* string_8081[NLANGS] = {
  "Tip rejects",
  "ͷ���޳�",
  "@" };

static char* string_8083[NLANGS] = {
  "Cap rejects",
  "ͷ���޳�",
  "@" };

static char* string_8087[NLANGS] = {
  "Flip-off color rejects",
  "�׿�����ɫ�޳�",
  "@" };

static char* string_8089[NLANGS] = {
  "Spin rejects",
  "��ת�����޳�",
  "@" };

static char* string_8090[NLANGS] = {
  "COMMUNICATION WITH PLC NOT ACTIVE - STATISTICS ARE NOT AVAILABLE",
  "��PLCͨѶδ���� - ����������",
  "@" };

static char* string_8103[NLANGS] = {
  "Sidewall rejects",
  "��ۼ���޳�",
  "@" };

static char* string_8105[NLANGS] = {
  "Side Cake rejects",
  "�۱��������޳�",
  "@" };

static char* string_8107[NLANGS] = {
  "Top Cake rejects",
  "�۱�ͷ������޳�",
  "@" };

static char* string_8109[NLANGS] = {
  "Bottom A rejects",
  "�ײ�A����޳�",
  "@" };

static char* string_8111[NLANGS] = {
  "Bottom B rejects",
  "�ײ�B����޳�",
  "@" };


static char* string_8150[NLANGS] = {
  "GRIPPERS STATISTICS",
  "^",
  "@" };


  */


//////////////////////////////////////////////////////////////////////////////////////////////
  



// LIQUID MACHINES

#if SERIAL_N==22013001   // "2013-001VB" - ampoule machine for exhibition
  #define CUSTOMER ""
#endif

#if SERIAL_N==2013003   // "2013-003VB" - Ampoule machine - replacement sent to Reyoung
  #define CUSTOMER ""
#endif

#if SERIAL_N==2013004   // "2013-004VB" - Ampoule machine SHANXXI PUDE
//  #define CUSTOMER "ɽ���յ�"
#endif

#if SERIAL_N==201400001 // OLD 2014002 Ampoule machine - Ideal Hightech Shijiazhou Hebei
//  #define CUSTOMER "�ӱ�������̩"
#endif

#if SERIAL_N==2013023 // New Ampoule machine - first sw installed 20140616
  #define CUSTOMER ""
#endif

#if SERIAL_N==2014013 // 
//  #define CUSTOMER "�Ͼ��ſ�"
#endif

//#if SERIAL_N==201300001 // first ampoule prototype LIOCO HAIKOU
#if SERIAL_N==201400029 // New machine for LIOCO HAIKOU
//  #define CUSTOMER "�����鿵"
#endif

#if SERIAL_N==201400003 // SHANXI PUDE
//  #define CUSTOMER "ɽ���յ�"
#endif

#if SERIAL_N==201400004 // HENG RUI LIANYUNGANG
//  #define CUSTOMER "���պ���"
#endif

#if SERIAL_N==201400006 // WUHAN HUMANWELL
//  #define CUSTOMER "�˸�ҩҵ"
#endif

#if SERIAL_N==201400025 || SERIAL_N==201400026 || SERIAL_N==201400027 ||SERIAL_N==201400028 // NITIN - 4 MACHINES
//  #define CUSTOMER "NITIN"
#endif

#if SERIAL_N==201400037 || SERIAL_N==201400038 || SERIAL_N==201400039  // REYOUNG - YIYUAN 
//  #define CUSTOMER "ɽ������"
#endif





// FREEZE DRIED MACHINES
#if SERIAL_N==2014020 // "2014-020V" - old 2013009VB ZDB Pharmaceutical - Harbin - Heilongjiang 
//  #define CUSTOMER "�������䱦��ҩҵ"
#endif

 // AUSIA - HANGZHOU - 4 F40 MACHINES
#if SERIAL>=201300003 && SERIAL<=201300006 // old numeration SERIAL_N>=2013001 && SERIAL_N<=2013004
//  #define CUSTOMER "���ݰ���"
#endif

#if SERIAL_N==201400011 //  OLD 2013006 "2013-006V" - FD machine for Medisam of Herbin 20141030
//  #define CUSTOMER "����������"
#endif

#if SERIAL_N==201400016 // Xi'an Bosen
//  #define CUSTOMER "������ɭ"
#endif

#if SERIAL_N==201400017 // Guandong (Guanzhou) GDYPH
//  #define CUSTOMER "�㶫һƷ��"
#endif

// Guorui - 6 F40 machines
#if SERIAL_N==20140013 || SERIAL_N==20140014 || SERIAL_N==20140015 || SERIAL_N==20140018 || SERIAL_N==20140019 || SERIAL_N==20140021 
//  #define CUSTOMER "��ҩ���Ź���"
#endif

#if SERIAL_N==201400009 || SERIAL_N==201400010 || SERIAL_N==201400012 || SERIAL_N==201400020 // LUYE PHARMA (3xF40 e 1xA40) 
//  #define CUSTOMER "ɽ����Ҷ"
#endif


  



//#define SERIAL_N 2014020   // "2014-020V" - old 2013009 "2013-009VB" - Workshop - updated IVS+PLC 120.105 20140326 - test cap with one more keypoint
//#define SERIAL_N 2013001   // "2013-001VB" - ampoule machine for exhibition
//#define SERIAL_N 2013003   // "2013-003VB" - Ampoule machine - replacement sent to Reyoung
//#define SERIAL_N 2013004   // "2013-004VB" - Ampoule machine SHANXXI PUDE
//#define SERIAL_N 201300501 // "2013-005V" - first ampoule prototype - LIONCO HAINAN HAIKOU
//#define SERIAL_N 201300502 // 2013-005VB/2014-009V - HENG RUI Updated to second feedback on 20140617
//#define SERIAL_N 2013006   // "2013-006VB" - Ampoule machine - updated IVS+PLC 120.107 2o feedback, controllo luci e altro 20140425
//#define SERIAL_N 201300701 // "2013-007V" - Ampoule machine - IVS+PLC 120.107 2o feedback, controllo luci nastro ingresso+uscita 20140428
//#define SERIAL_N 201300702 // "2013-007VB" - Ampoule machine - double bottom light + cosmetic
//#define SERIAL_N 2013020   // "2013-020V" - new ampoule machine - updated 20140526
//#define SERIAL_N 2013023   // "2013-023V" - new ampoule machine - first sw installed 20140616
//#define SERIAL_N 2013029   // "2013-0029V" - ampoule machine Shangdong Reyoung (OLD, sent back??)
//#define SERIAL_N 2014001   // "2014-001VC" - ampoule 3 bottomlight
//#define SERIAL_N 2014002   // "2014-002V" - old "2013-002VB" Ampoule machine - Ideal Hightech Shijiazhou Hebei
//#define SERIAL_N 2014003   // "2014-003VC" - new machine - NITIN
//#define SERIAL_N 2014004   // "2014-004VC" - new machine - NITIN
//#define SERIAL_N 2014013   // "2014-013V" - previous 2013008 "2013-008VB" - Ampoule machine - updated IVS+PLC 120.105 20140326


// FREEZE DRIED MACHINES
//#define TIVS_F40  // MODEL TIVS-F40
//#define SERIAL_N 2013001   // "2013-001V" - First freeze dried machine AUSIA
//#define SERIAL_N 2013002   // "2013-002V" - Second freeze dried machine AUSIA
//#define SERIAL_N 2013003   // "2013-003V" - Third freeze dried machine AUSIA
//#define SERIAL_N 2013004   // "2013-004V" - Fourth freeze dried machine AUSIA
//#define SERIAL_N 2013006   // "2013-006V" - New FD machine
//#define SERIAL_N 201301001 // "2013-010YF" - first freeze dried prototype delivered to exhibition  - updated IVS+PLC 120.105 20140326
//#define SERIAL_N 2014004   // "2014-004V" - Workshop - new machine 20140526
//#define SERIAL_N 2014005   // "2014-005V" - Workshop - old SERIAL_N 201301002 "2013-010VB"
//#define SERIAL_N 2014006   // "2014-006V" - New FD machine
//#define SERIAL_N 2014008   // "2014-008V" - New FD machine
//#define SERIAL_N 2014010   // "2014-010V" - New FD machine - First Guorui
//#define SERIAL_N 2014011   // "2014-011V" - New FD machine - Second Guorui
//#define SERIAL_N 2014012   // "2014-012V" - New FD machine - Thirs Guorui
//#define SERIAL_N 2014020   // "2014-020V" - old 2013009 "2013-009VB" - Workshop - updated IVS+PLC 120.105 20140326 - test cap with one more keypoint






#ifndef MODEL
  #ifdef TIVS_A60
    #define MODEL "TIVS-A60"
  #endif
  #ifdef TIVS_A40
    #define MODEL "TIVS-A40"
  #endif
  #ifdef TIVS_F40
    #define MODEL "TIVS-F40"
  #endif
  #ifdef TIVS_F20
    #define MODEL "TIVS-F20"
  #endif

#endif

#ifndef CUSTOMER
  #define CUSTOMER "N/A"
//  #define MODEL    "undefined"
#endif



