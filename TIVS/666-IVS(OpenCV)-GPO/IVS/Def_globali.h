// -----------------------------------------------------------------------------
//
//            Q U I    C I     S O N O     S O L O    L E    D E F I N E
//
// -----------------------------------------------------------------------------


#pragma once

#define WM_INIT     (WM_USER+ 1)
#define WM_MODIFIED (WM_USER+10)
#define MAX_NUM_CONTAINER   20
#define MAX_NUM_PARAMS     500
//#define MAX_NUM_MENU_BTN   100
//#define MAX_NUM_MENU_BTN   120//Pour marked on 20200925 added for TIVS_S60
#define MAX_NUM_MENU_BTN   140//Pour marked on 20200925 added for TIVS_S60

// Valide per finestre con look 3d
#define OFFS_COORDX 4  // Sfasamento coordinate dovuto alla cornice e al tipo di finestra creato
#define OFFS_COORDY 4

#define MBOX_ERROR          10 // Visualizzazione di un messaggio di errore 
#define MBOX_WARNING        20 // Visualizzazione di un messaggio di avviso
#define MBOX_YESNO          30 // Visualizzazione di un messaggio di richiesta sì-no
#define MBOX_INFO           40 // Visualizzazione di un messaggio di informazione/ok 



#define PARTICLE_BOTTOM    100 // Station defined for particle with bottom light
#define PARTICLE_BACK      200 // Station defined for particle with backlight
#define PARTICLE_BOTH      220 // Station defined for particle with selectable backlight/bottomlight
#define COSMETIC_BACK      300 // Station defined for cosmetic with backlight


#define CAKESIDE_ARBACK    400 // Station defined for cake shrink and layer, backlight area scan
#define CAKETOP_ARDIRECT   410 // Station defined for cake top skinning,  direct light area scan


/*
#define FILLEV
//#define SERIAL_N 2013999  // first filling level system to bangladesh
//#define SERIAL_N 2013059  // "YF2013-059C" second filling level system 
#define SERIAL_N 2013058  // "YF2013-058C" third filling level system 
*/

/*
FILLING LEVEL CONTROL 
 1ml - 30.000/h
20ml - 14.000/h
*/



//#define STOPPER_CTRL // 

/*
#define TIVS_S40 // MODEL S40 / PREFILLED SYRINGES INSPECTION MC
#define SERIAL_N 201900101   // New development S40 machine started October 2nd 2019 for Chongqing exhibition
*/


/*
#define TIVS_S60 // MODEL S60 / PREFILLED SYRINGES INSPECTION MC
#define SERIAL_N 202000099   // New development S60 machine started September 25th 2020 for Chongqing exhibition
*/


/*
#define TIVS_A20L // MODEL TIVS-A20L 
//#define SERIAL_N 201600008   // Macter
#define SERIAL_N 201900109   // Aurobindo
*/


//Pour added for new machine TIVS-A20 20180523
/*
#define TIVS_A20 // MODEL TIVS-A20 
#define SERIAL_N 201700023   //2018-003Vc JV Remedy
*/
//Pour added for new machine TIVS-A20 20180523


//Pour added for new machine TIVS-F20 20200812
/*
#define TIVS_F20 // MODEL TIVS-F20 20200812
#define SERIAL_N 202000018   //2020-010Vc Aurobindo 20200812
*/
//Pour added for new machine TIVS-F20 20200812



//Pour added for new combined machine TIVS-A40L 20180124
/*
#define TIVS_A40L // MODEL TIVS-A40L 
//#define SERIAL_N 201700009   // ANKANG Beiyida 2017-019V
//#define SERIAL_N 201700016   // ZISKA 2017-006Vc
#define SERIAL_N 201800031   // Thailand GPO 2018-011Vc
*/
//Pour added for new combined machine TIVS-A40L 20180124


/*
#define TIVS_C20 // MODEL TIVS-C20 
//#define SERIAL_N 201400042   // Mexico Ultra Tech De CV
//#define SERIAL_N 201800030   // 2018-011Vc 20190318
//#define SERIAL_N 201900019   //2018-012Vc 20190917
//#define SERIAL_N 201900026     //2018-012V  澳斯康 20191122
//#define SERIAL_N 201900028   //原子高科 20190803  原201900015，后改为201900028
//#define SERIAL_N 202000004   // 2019-007Vc 20200321
#define SERIAL_N 202000012   // 2020-011Vc 20200807
//#define SERIAL_N 202000103   // 2020-003Vc 20201114
//#define SERIAL_N 202000104   // 2020-004Vc 20201114
*/


/*
#define TIVS_C40 // MODEL TIVS-C40 
//#define SERIAL_N 201700010   // New C40L machine 安徽安科生物 ANKEBIO - (CM0)
//#define SERIAL_N 201800010   // New C40 machine 成都康华生物 Chengdu Kanghua Biological 2018-010V (CM0)
//#define SERIAL_N 201800015   // New C40 machine 天津生物化学制药有限公司 Tianjin Biochemical (CM0)
//#define SERIAL_N 201800019   // New C40L machine  - (CM0)
//#define SERIAL_N 201800011   // New C40L machine with HGA and 3rd deviator - BIOCAD CJSC - (CM0)
//#define SERIAL_N 201900198   // New C40L machine with HGA and 3rd deviator - BIOCAD CJSC - (CM0)
#define SERIAL_N 202000014   // New C40 machine Xiamen Tebao 20200819
*/


/*
#define TIVS_C60 // MODEL TIVS-C60 
#define SERIAL_N 201800001   // New development C60 machine 2018-00-001 玉溪沃森生物技术有限公司 Yuxi Watson Biotechnology Co., Ltd. (CM0)
//#define SERIAL_N 201800002   // New C60 machine 沃森生物 WALVAX BIOTECHNOLOGY CO LTD (CM0)
*/




#define TIVS_A60 // MODEL TIVS-A60
//#define SERIAL_N 201400037   // Reyoung first A60 ampoules
//#define SERIAL_N 201400038   // Reyoung second A60 ampoules
//#define SERIAL_N 201400039   // Reyoung third A60 ampoules
//#define SERIAL_N 201400041   // Reyoung fourth A60 ampoules
//#define SERIAL_N 201400045   // Reyoung fifth A60 ampoules
//#define SERIAL_N 201400044   // New A60 ampoules India Ciron (2014-008vc)
//#define SERIAL_N 201500001   // New A60 ampoules - Kelun - new tip control v30 testing vs truking - yueyang - hunan
//#define SERIAL_N 201500002   // New A60 machine - ex Sanjing - Jinzhou Aohong - recompiled 20160804 - shipped 20160916 - 锦州奥鸿
//#define SERIAL_N 201500014   // New A60 - ampoules Yunnan Longhai 
//#define SERIAL_N 201500015   // New A60 - ampoules Yunnan Longhai 
//#define SERIAL_N 201500016   // New A60L - ampoules fatta notte partenza 20160523 Apolo - HV Leak test
//#define SERIAL_N 201500017   // New A60 - new ampoules 20160607 吉林敖东 - HV Leak test - Jilin Aodong - shipped 20160916
//#define SERIAL_N 201500018   // New A60 - ampoules 河南辅仁 Furen Medicine Group Co., Ltd. - prepared 20160511 - fast cameras and double cap - ex Sanjing convertita in 201500018
//#define SERIAL_N 201600007   // New A60L - seconda Apolo 20160914 - HV Leak test - Agg new partic 20161215//西安迪赛 20181008
//#define SERIAL_N //201600009   // New A60 - nuova iniziata 20160918 per testare nuovo partic - Xinxiang Changle  //project cancelled,need to be changed to CIRION 20180206
//#define SERIAL_N 201600010   // New A60 - nuova iniziata 20160922 
//#define SERIAL_N 201600011   // New A60 - nuova iniziata 20170119 // Ninth Guorui Pharmaceutical Co., Ltd
//#define SERIAL_N 201600012   // New A60 - Pour 20170726 // CaoHuanDan (didn't prepare software)
//#define SERIAL_N 201700001   // New A60 - Pour 20171030 // 600apm TRACKING TESTING 2017-010V
//#define SERIAL_N 201700002   // New A60 - Pour 20171030 // 600apm TRACKING TESTING 2017-008Vc 国药（上海）国际医药卫生有限公司
//#define SERIAL_N 201700006   // New A60 ampoules India Ciron -changed to Myfarma finally(2017-007Vc)
//#define SERIAL_N 201700015     //Xinxiang Changle  //Myfarma //need to be changed to CIRION 20180206
//#define SERIAL_N 201800012     //江苏豪森，Liquid vials inspection machine TIVS-L60
//#define SERIAL_N 201800016   // 乌兹别克斯坦LLC(2018-006Vc)20181010
//#define SERIAL_N 201800022   // 乌兹别克斯坦LLC(2018-008Vc)20181120
//#define SERIAL_N 201800025 // New ampoule liquid machine - single feedback - 虎林珍宝 TIVS-L60 Pour added 20181210  2018-024V
//#define SERIAL_N 201800003 // TIVS-L60 Pour added 20190108  2018-003V 沃森生物
//#define SERIAL_N 201800026 // New ampoule liquid machine - single feedback - 成都百裕 TIVS-A60 Pour added 20180128  2018-027V

//software made with OpenCV 

//#define SERIAL_N 201900017     //CIRION 20190826 //need to be changed to 201900017
//#define SERIAL_N 201900098 // New ampoule liquid machine - single feedback - 北京悦康 TIVS-A60 Pour added 20190703
//#define SERIAL_N 202000007 // New ampoule liquid machine - single feedback - 北京悦康 TIVS-A60 Pour added 20190719 2020-007V 新乡常乐
//#define SERIAL_N 202000006 // New ampoule liquid machine - single feedback - 北京悦康 TIVS-A60 Pour added 20190719   2020-006V 新乡常乐

//#define SERIAL_N 201900095 // New ampoule liquid machine - single feedback - 北京悦康 TIVS-A60 Pour added 20200405
//#define SERIAL_N 201900096 // New ampoule liquid machine - single feedback - 北京悦康 TIVS-A60 Pour added 20200408
//#define SERIAL_N 202000001 // New ampoule liquid machine - single feedback - 宁波三生 TIVS-A60 Pour added 20200611
//#define SERIAL_N 202000005 // New ampoule liquid machine - single feedback - 仁合益康 TIVS-A60 Pour added 20200408

//#define SERIAL_N 201900005   // New L60L 2018-029V - HV Leak test//太极集团 20190517     already updated to OpenCV, added also opacity control
//#define SERIAL_N 201900006   // New L60L 2018-030V - HV Leak test//太极集团 20190517     already updated to OpenCV, added also opacity control
//#define SERIAL_N 201900007   // New L60L 2018-031V - HV Leak test//太极集团 20190722     already updated to OpenCV, added also opacity control
//#define SERIAL_N 201900008   // New L60L 2018-032V - HV Leak test//太极集团 20190722     already updated to OpenCV, added also opacity control 
//#define SERIAL_N 201900009   // New L60L 2018-033V - HV Leak test//太极集团 20190819     already updated to OpenCV, added also opacity control
//#define SERIAL_N 201900010   // New L60L 2018-034V - HV Leak test//太极集团 20190909     already updated to OpenCV, added also opacity control
//#define SERIAL_N 201900011   // New L60L 2018-035V - HV Leak test//太极集团 20201217     already updated to OpenCV, added also opacity control 
//#define SERIAL_N 201900012   // New L60L 2018-036V - HV Leak test//太极集团 20200422  already updated to OpenCV, added also opacity control
//#define SERIAL_N 201900013   // New L60L 2018-037V - HV Leak test//太极集团 20201224  already updated to OpenCV, added also opacity control 
//#define SERIAL_N 201900014   // New L60L 2018-038V - HV Leak test//太极集团 20201224  already updated to OpenCV, added also opacity control
//#define SERIAL_N 201900015   // New L60L 2018-039V - HV Leak test//太极集团 20191224     already updated to OpenCV, added also opacity control
//#define SERIAL_N 201900016   // New L60L 2018-040V - HV Leak test//太极集团 20191224  need to be removed


//#define SERIAL_N 202000011   // 201900012 New L60L 2018-036V - HV Leak test//太极集团 20191125 //202000011 Transferred to TSK 2020-011V on 20200215 to be updated to new particle inspection

//#define SERIAL_N 202000008   // 201900011 New L60L 2018-035V - HV Leak test 太极集团 20190918 //202000008 Transferred to SANJING 2020-008V and removed HV part on 20200220
//#define SERIAL_N 202000009   // 201900013 New L60L 2018-037V - HV Leak test 太极集团 20191210 //202000009 Transferred to SANJING 2020-009V and removed HV part on 20200220
//#define SERIAL_N 202000010   // 201900014 New L60L 2018-038V - HV Leak test 太极集团 20191210 //202000010 Transferred to SANJING 2020-010V and removed HV part on 20200220

//#define SERIAL_N 201900027   // New L60 - 长春金派格 20191205

//#define SERIAL_N 201900020   // New A60L - HV Leak test - 锦州奥鸿 20200404
//#define SERIAL_N 202000021   // New A60L - HV Leak test - 兰州生物所 20200828
//#define SERIAL_N 202000022   // New L60 -2020-022V 北京科兴 20200913
//#define SERIAL_N 202000028   // New L60 - 2020-028V 北京科兴 20201207
#define SERIAL_N 202000090   // New L60 -2020-022V 大连科兴 20200913

//#define SERIAL_N 202000109   // New A60 - UZBEKISTAN DENTAFILL PLYUS 20201207
//software made with OpenCV
*/




//#define TIVS_A40 // MODEL TIVS-A40
//#define SERIAL_N 2013001   // 2013-001VB - TFL-2012-017YF - 2013-005V changed to 2015-025V - ampoule machine for exhibition -  aggiorn e rivenduta a 上海计胜
//#define SERIAL_N 2013006   // CHANGED to 201400005 "2013-006VB" - Ampoule machine - updated IVS+PLC 120.107 2o feedback, controllo luci e altro 20140425 
//#define SERIAL_N 2013020   // 2013-020V - new ampoule machine - updated 20140526
//#define SERIAL_N 201300001 // OLD 201300501 2013-005V - first ampoule prototype - LIONCO HAINAN HAIKOU
//#define SERIAL_N 201300002 // OLD 2013029 "2013-0029V" - ampoule machine Shangdong Reyoung (OLD, sent back??)
//#define SERIAL_N 201400001 // OLD 2014002 2014-002V 2013-002VB Ideal Hightech Shijiazhou Hebei WARNING: REVERSED ENCODER!
//#define SERIAL_N 201400002 // 2013003 "2013-003VB" - Ampoule machine - replacement sent to Reyoung - back to ws recompiled and updated 20151204
//#define SERIAL_N 201400003 // 2013004 2013-004VB - Ampoule machine SHANXI DATONG PUDE
//#define SERIAL_N 201400004 // OLD 201300502 // 2013-005VB/2014-009V - HENG RUI Updated to second feedback on 20140617 - ChangChunChangSheng - Changchun
//#define SERIAL_N 201400005 // old 2013006 lianyungang ampoule machine factory - returned updated to be sent to pakistan 20150714 - INDUS recompiled 20151102
//#define SERIAL_N 201400006 // 201300702 2013-007VB - Wuhan - Humanwell Healthcare - 3 bottom particle - update new partic 20170613
//#define SERIAL_N 201400007 // 2014013 2014-013V 2013008 2013-008VB - YOKO PHARMA
//#define SERIAL_N 201400012 // - TOT Biopharma 20170619 - OLD 201300701 2013-007V - Ampoule machine - controllo luci nastro ingresso+uscita 20140428 - LUYE VIALS - test NITIN DR - ora test Kunming, rotaz tlc e immagine ingrandita 
//#define SERIAL_N 201400023 // new machine 20141011 (2014-009Vc) CIRON L40 for liquid vials with Cap inspection station + three particle inspection station
//#define SERIAL_N 201400025 // OLD 2014001 "2014-001VC" - First  NITIN A40
//#define SERIAL_N 201400026 // OLD 2014002 "2014-002VC" - Second NITIN A40
//#define SERIAL_N 201400027 // OLD 2014003 "2014-003VC" - Third  NITIN L40 - changed to standard A40 on 20141222 - Added D&R cameras 20151209
//#define SERIAL_N 201400028 // OLD 2014004 "2014-004VC" - Fourth NITIN L40 - changed to standard A40 on 20141222
//#define SERIAL_N 201400029 // OLD 2013023 "2013-023V" - sent as replacement to Lincan Haikou - for testing triple tip and LAM - Hebei Zhitong
//#define SERIAL_N 201400030 // OLD "2014-021V" - HARBIN - SONGHE - aumentato filtro 20151224
//#define SERIAL_N 201400031 // OLD "2013-025V" - INTAS India Liquid ampoules + liquid vials 20141206
//#define SERIAL_N 201400032 // New standard liquid machine "Turkish Mypharma"
//#define SERIAL_N 201400033 // New liquid standard machine ORAL LIQUID - Testing for LUNAN - 20161206 updated new particle
//#define SERIAL_N 201400034 // new machine 20141011 A40 machine for Chongqing Exhibition with 3 particle inspection station
//#define SERIAL_N 201400050 // New ampoule liquid machine - single feedback - PAKISTAN BOSCH
//#define SERIAL_N 201400051 // New ampoule liquid machine - single feedback - PAKISTAN BOSCH
//#define SERIAL_N 201400057 // New ampoule liquid machine - single feedback - 20151223 - test Kelun & multithread - Guandong Sunho 20170601
//#define SERIAL_N 201400058 // New ampoule liquid machine - single feedback - 20151223 - Changed to L40 on 20160525 - "北京三元 Beijing Sanyuan"
//#define SERIAL_N 201400059 // New ampoule liquid machine - single feedback - prima 12/2015 - 甘肃成纪 Gansu Chengji - richiesta agg HGA 20170602 //changed to ampoules inspection with color liquid adding 20210320
//#define SERIAL_N 201400060 // New ampoule liquid machine - single feedback - to be used for testing dot&ring - recompiled for FAT 20170628 中科生物 / ZhongKe Biopharm Co., Ltd.
//#define SERIAL_N 201600001 // New ampoule liquid machine - single feedback - for Russian exhibition 20160823
//#define SERIAL_N 201600002 // New ampoule liquid machine - single feedback - new partic
//#define SERIAL_N 201600003 // New ampoule liquid machine - single feedback - for Indian exhibition 20160919 - Guandong Sunho 20170601  update software in May 2020
//#define SERIAL_N 201600004 // New ampoule liquid machine - single feedback - 20170609 changed to 玉溪泽润 Yuxi Zerun - old 吉林紫鑫 Jilin Zixin Pharmaceutical Industrial Co., Ltd.
//#define SERIAL_N 201600006 // New ampoule liquid machine - single feedback - 成都苑东 EASTON BIOHPARMACEUTICALS
//#define SERIAL_N //201700005 // New ampoule liquid machine - single feedback - 华昊中天 Pour added 20171104 //Project cancelled, recovered to FD inspection
//#define SERIAL_N 201700011 // New ampoule liquid machine - single feedback - 瑞安亿拓 Pour added 20171115
//#define SERIAL_N 201700017 // New ampoule liquid machine - single feedback - 巴基斯坦ZAFA Pour added 20180323
//#define SERIAL_N 201700004 // New ampoule liquid machine - single feedback - 巴基斯坦NIP Pour added 20180323
//#define SERIAL_N 201700018 // New ampoule liquid machine - single feedback - 沈阳新马 Pour added 20180406 //Prepared by victor,need to be combined.
//#define SERIAL_N 201700005 // New ampoule liquid machine - single feedback - 复宏翰林 TIVS-L40 Pour added 20180427 //not the newest version, changed by victor
//#define SERIAL_N 201800023 // New ampoule liquid machine - singcle feedback - 重庆福安 Pour added 20181015
//#define SERIAL_N 201800018 // New ampoule liquid machine - single feedback - 烟台百克 TIVS-L40 Pour added 20181020  2018-015V
//#define SERIAL_N 201800017 // New ampoule liquid machine - single feedback - 巴基斯坦SAMI 2018-007Vc Pour added 20181023
//#define SERIAL_N 201800020 // New ampoule liquid machine - single feedback - 佳木斯多多 2018-017V Pour added 20181023
//#define SERIAL_N 201800028 // New ampoule liquid machine - single feedback - 珠海丽珠2018-026V Pour added 20190219
//#define SERIAL_N 201900106 // New ampoule liquid machine - single feedback - 上海丽珠2019-006V备 Pour added 20190219
//#define SERIAL_N 201900099 //ZhengZhou ChuangTai 20190525
//#define SERIAL_N 201900030 //Shanghai HaoHai 2019-027V 20200512
//#define SERIAL_N 201900097 //Angde Shengwu 20190525
//#define SERIAL_N 202000002 // 宁波三生-2020-003V Pour added 20200602
//#define SERIAL_N 202000003 // 宁波三生-2020-003V Pour added 20200602
//#define SERIAL_N 202000004 // 宁波三生-2020-004V Pour added 20200602
//#define SERIAL_N 202000101 // R-Pharma-2020-001Vc Pour added 20201012
//#define SERIAL_N 202000102 // R-Pharma-2020-002Vc Pour added 20201012




/*
#define TIVS_F40  // MODEL TIVS-F40
//#define SERIAL_N 2014020   // "2014-020V" - old 2013009VB ZDB Pharmaceutical - Harbin - Heilongjiang - returned
//#define SERIAL_N 201400008   // "2017-010V" - old 2013009VB ZDB Pharmaceutical - Harbin - Heilongjiang - changed to NuoWeiNuo 20170724 
//#define SERIAL_N 201301001 // "2013-010YF" - first freeze dried prototype delivered to exhibition  - updated IVS+PLC 120.105 20140326
//#define SERIAL_N 201300003 // old 2013001 "2013-001V" - First freeze dried machine AUSIA
//#define SERIAL_N 201300004 // old 2013002 "2013-002V" - Second freeze dried machine AUSIA
//#define SERIAL_N 201300005 // old 2013003 "2013-003V" - Third freeze dried machine AUSIA
//#define SERIAL_N 201300006 // old 2013004 "2013-004V" - Fourth freeze dried machine AUSIA (separate room)
//#define SERIAL_N 201400009 // OLD 2014005 "2014-005V" - Workshop - old SERIAL_N 201301002 "2013-010VB" - LUYE FD
//#define SERIAL_N 201400010 // OLD 2014006 "2014-006V" - New FD machine for testing double cap - LUYE FD
//#define SERIAL_N 201400011 // OLD 2013006 "2013-006V" - New FD machine updated July 2015 - Tianhuang Haikou Hainan
//#define SERIAL_N 201400013 // old 2014010 "2014-010V" - First  Guorui - last update for ALL 6 machines 20141112
//#define SERIAL_N 201400014 // old 2014011 "2014-011V" - Second Guorui - last update for ALL 6 machines 20141112
//#define SERIAL_N 201400015 // old 2014012 "2014-012V" - Third  Guorui - last update for ALL 6 machines 20141112
//#define SERIAL_N 201400017 // New FD machine - GZYPH - (not sure) - Test for new TIANHUANG cap
//#define SERIAL_N 201400018 // New FD machine 20140902 - Fourth Guorui - last update for ALL 6 machines 20141112
//#define SERIAL_N 201400019 // New FD machine 20140902 - Fifth  Guorui - last update for ALL 6 machines 20141112
//#define SERIAL_N 201400021 // New FD machine 20140902 - Sixth  Guorui - last update for ALL 6 machines 20141112
//#define SERIAL_N 201400016 // OLD 2014008 2014-008V - New FD machine - Bosen Xi
//#define SERIAL_N 201400020 // OLD 2014004  "2014-004V" -20140526 - LUYE FD with bypass belt and rotary table  //Pour recompiled 20171214
//#define SERIAL_N 201400022 // New FD machine 20140812 - 20141021 - Single PC Yaopharma (and testing 2nd PC 20141202 for heel)
//#define SERIAL_N 201400024 // New FD machine 20140812 - 20141021 - Konruns(Pour added HGA station 20170724)//machine returned on March 2019
//#define SERIAL_N 201400048 // New FD machine 20150323 - Single feedback - Seventh Guorui
//#define SERIAL_N 201400049 // New FD machine 20150323 - Single feedback - Eighth Guorui 
//#define SERIAL_N 201400053 // New FD machine 20150702 - Single feedback
//#define SERIAL_N 201400054 // New FD machine 20150702 - Single feedback - test new AUSIA controls - test new crimping cap BOSEN - Guandong Sunho 20170601 - test HGA
//#define SERIAL_N 201400055 // New FD machine 20150901 - Single feedback - Rongan - Ningbo vel aumentata a 600, fast cameras cap + aggiunto HGA 20160822 //Pour changed to 201700019 FOR QILU 20180323
//#define SERIAL_N 201400056 // New FD machine 20150901 - Single feedback - Workshop - Test Huapont powder on rubber
//#define SERIAL_N 201500009 // New FD machine 20151113 - Single feedback - VEM Turkey - Alarm log + safety screw switch
//#define SERIAL_N 201500010 // New FD machine 20151113 - Single feedback
//#define SERIAL_N 201500011 // New FD machine 20160321 - Single feedback - Luye
//#define SERIAL_N 201500012 // New FD machine 20160316 - Single feedback - Luye
//#define SERIAL_N 201500013 // New FD machine 20170119 - Single feedback - 490 - INCREASED SPEED TO 600 FOR XINCHANG
//#define SERIAL_N 201700008 // New FD machine 20180702-ANKANG BEIYIDA 2017-018V
//#define SERIAL_N 201800009 // New FD machine 20171214-AnKang Beiyida //changed to GANSU Changee,changed by Victor,need to be combined
//#define SERIAL_N 201700012 // New FD machine 2018116-SHANGHAI KAIMAO  //Not the newest version, changed by Victor
//#define SERIAL_N 201700013 // New FD machine 2018118-Aurobindo 2017012Vc
//#define SERIAL_N 201700019 //  600, fast cameras cap + aggiunto HGA 20160822 //Pour changed from 201400055 to 201700019 FOR QILU 20180323,and removed HGA
//#define SERIAL_N 201800001 // New FD machine 20180514-ShenYang Xinma 20180128
//#define SERIAL_N 201800004 // New FD machine 20180514-SHANXI BOSEN //Not the final version,changed by victor
//#define SERIAL_N 201800024 // New FD machine 20181201 HaiNan Beite
//#define SERIAL_N 201800021 // New FD machine 20181218 Chongqing Yaoyou
//#define SERIAL_N 201800027 // New FD machine 20190213 Changchun Zhuoyi
//#define SERIAL_N 201800029 // Qingdao GuoDa,Pour added 20190318 by 201400024
//#define SERIAL_N 201900001 // QianShan Weikang,2019-001V, Pour added 20190410
//#define SERIAL_N 202000015 // Wuhan Renfu,2020-015V, Pour added 20200801
//#define SERIAL_N 202000016 // Wuhan Renfu,2020-016V, Pour added 20200801
//#define SERIAL_N 201900025 //上海生物所,2019-025V, Pour added 20200520
#define SERIAL_N 201900023 //山东亦度,2019-025V, Pour added 20200907
*/


#define SAVE_IMAGES    // if defined saving of all images is enabled into inspect_3d_particle
#define SAVE_FULL_PROC // if defined saving of full scale processed color images can be made (ONLY IN DEBUG CONFIGURATION)

// 21605985 21502502 21605972
// 21605953 21428840 21428861


  #pragma warning(disable : 4995)

  #define CHILD_ALIGN_BELOW // Child windows aligned below main container

  #define BASLER_SDK

  #ifdef LOCALDEBUG
    #define MAIN_DIRECTORY    L"C:\\IVS"               // Directory di lavoro del programma
    #define PALETTE_FILE      L"C:\\IVS\\btlght.lut"
    #define BATCHDATA_FILE    L"C:\\IVS\\batchdata.dat"
    #define DEFAULT_FILE      L"C:\\RECIPES\\default.ips"
    #define RECIPE_DIRECTORY  L"C:\\RECIPES"           // Directory to save batch reports
    #define BATCH_DIRECTORY   L"C:\\BATCHREPORTS"      // Directory to save batch reports
    #define IMAGES_DIRECTORY  L"C:\\IMAGES"            // Directory to save images into
	//#define LOG_DIRECTORY     L"C:\\LOG REPORT"               //Pour added on 20200506
	//#define ALARM_LOG_DIRECTORY     L"C:\\LOG REPORT"               //Pour added on 20200506
  #else
    #define MAIN_DIRECTORY    L"D:\\IVS"               // Program working directory
    #define PALETTE_FILE      L"D:\\IVS\\btlght.lut"
    #define BATCHDATA_FILE    L"D:\\IVS\\batchdata.dat"
    #define DEFAULT_FILE      L"D:\\RECIPES\\default.ips"
    #define RECIPE_DIRECTORY  L"D:\\RECIPES"           // Directory to save batch reports
    #define BATCH_DIRECTORY   L"D:\\BATCHREPORTS"      // Directory to save batch reports
    #define IMAGES_DIRECTORY  L"D:\\IMAGES"            // Directory to save images into
	//#define LOG_DIRECTORY     L"D:\\LOG REPORT"               //Pour added on 20200506
	//#define ALARM_LOG_DIRECTORY     L"D:\\ALARM LOG REPORT"               //Pour added on 20200506

  #endif  

  #define MAX_LENGHT_PATH         256  // max lenght of folders path

// *********************************************************************************


// def pwd levels OPERATOR_LEVEL=1 / MAINTENANCE_LEVEL=2 / SUPERVISOR_LEVEL=3 / SUPERUSER_LEVEL=4            
/*static int pwd_levels[] = { 2 ,  // 0 - automation button
                            2 ,  // 1 - inspection button
                            1 ,  // 2 - recipes 
                            1 ,  // 3 - start/stop inspection
                            1 ,  // 4 - utilities
                            1 ,  // 5 - exit
                            1 ,  // 6 - batches
                            4 }; // 7 - calibration */
/*
 int pwd_levels[] = { 2 ,   //  0 - automation button
                      2 ,   //  1 - inspection button
                      1 ,   //  2 - recipes 
                      1 ,   //  3 - start/stop inspection
                      1 ,   //  4 - utilities
                      1 ,   //  5 - exit
                      1 ,   //  6 - batches
                      4 ,   //  7 - calibration
                      1 ,   //  8 - load recipe
                      2 ,   //  9 - save recipe
                      2 };  // 10 - delete recipe
                      */


#ifdef STOPPER_CTRL

  // BACKHOFF PANEL PC CP2216-00000 - Celeron 1.6

  #define USEDB
//  #ifdef USEDB

  #include "afxdb.h"
//  CDatabase m_dbCust;
//    return 0;
//  #endif

  #define MODEL_NAME        L"STOPCTRL"
  #define VERSION_PC        L"120.107"

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1          3  // number of cameras connected to PC1
  #define NUM_TLC_PC2          3  // number of cameras connected to PC2
  #define NUM_TLC              3  // (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER           3  // (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
  #define APP_RESX          1366  // Horizontal resolution of application window
  #define APP_RESY           768  // Vertical resolution of application window

  #define LAYOUT_FILENAME L"layout_lq.png"

  #define MACHINE_ID  "2014-001VQ"
  static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
  static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - unused
  static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // unused  - unused
  static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
  static char*    seal_serials  [3] = { "21428870", "21428831", "21405173"             };

#endif




#ifdef FILLEV 

  #define MODEL_NAME        L"TIVS-FILLEV"
  #define VERSION_PC        L"120.107"
  #define MACHINE_ID        "2013-059C"

  #define NUM_TLC_PC1         1  // number of cameras connected to PC1
  #define NUM_TLC_PC2         1  // number of cameras connected to PC2
  #define NUM_TLC             1  // max number of cameras to connect and use
  #define NUMTRIGGER          1  // number of cameras/triggers to link to PLC

  #define IMAGE_DISPLAY
  #define ROT90CCW_SCALE21

  #define APP_RESX         1024   // Horizontal resolution of application window
  #define APP_RESY          768   // Vertical resolution of application window
  #define NUM_TLC             1   // Numero di telecamere attese 

  #define DIMX_ACQ          780   // Larghezza di acquisizione da telecamera (scala ca 0,1mm/pix)
  #define DIMY_ACQ          580   // Altezza di acquisizione da telecamera   (scala ca 0,1mm/pix)
  #define F_ACQ_DSP         0.2   // Fattore di scalatura per la visualizzazione

  static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
  static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic


  static wchar_t* tip_names    [1] = { L"TC_A",    };
  static char*    tip_serials  [1] = { "21321484", };
  
  static wchar_t* part1_names  [1] = { L"P1_A",    };
  static char*    part1_serials[1] = { "21293367"  };
  
  static wchar_t* part2_names  [1] = { L"P2_A",    };
  static char*    part2_serials[1] = { "21322511"  };

  static wchar_t* cosm_names   [1] = { L"CM_A",    };
  static char*    cosm_serials [1] = { "21328440"  };

  static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };


#endif

// *********************************************************************************



//#define TIVS_S40 // MODEL S40 / PREFILLED SYRINGES INSPECTION MC

#ifdef TIVS_S40 // MODEL TIVS-S40

  #define MODEL_NAME        L"TIVS-S40"
  #define VERSION_PC        L"125.100"
//  #define SERIAL_N 201600042   // Macter 

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1            15  // number of cameras connected to PC1
  #define NUM_TLC_PC2             0  // number of cameras connected to PC2
  #define NUM_TLC       NUM_TLC_PC1  //  (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER    NUM_TLC_PC1  // (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
//  #define APP_RESX             1280  // Horizontal resolution of application window
//  #define APP_RESY             1024  // Vertical resolution of application window
//  #define APP_RESX         1366   // Horizontal resolution of application window
//  #define APP_RESY          768   // Vertical resolution of application window
  #define APP_RESX         1920   // Horizontal resolution of application window
  #define APP_RESY         1080   // Vertical resolution of application window

  #define LAYOUT_FILENAME L"layout_S40.png"

  #if SERIAL_N==201900101 // New development S40 machine started October 2nd 2019 for Chongqing exhibition

    #define MACHINE_ID      "2019-00-001"
    #define CUSTOMER_NAME  L"南京健友生化制药股份有限公司"  
    #define CUSTOMER_NAMEE L"Nanjing Kingfriend Biochemical Pharmaceutical Co.,Ltd."  

    #define BATCHLOG_PARAMETERS // When defined changes to parameters will be added to batch report
    #define BATCHLOG_EVENTS     // When defined events/alarms will be added to batch report

  	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define NEW_PASSW_MNGR

    #define SIDEWALL_PFS
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //Pour added on 20190514
//    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[5][2] = { {0,2}, {3,5}, {6,8}, {9,11}, {12,14} }; // 
    static UINT tlcgroupPC2[1][1] = { 0 }; // 
    //static UINT tlcgroupPC2[2][2] = { {0,3}, {4,5}        }; // topcake - sidecake / particle2

    // CAMERAS
    //   stopper           0/1/2         top&flange   stopper sides A B 
    static wchar_t* stopper_names  [3] = { L"T_A",    L"C3_A",    L"C3_B"     };
    static char*    stopper_serials[3] = { "23051988", "23052018", "23052015" };
    // particle1 backlight 3/4/5
    static wchar_t* part1_names    [3] = { L"P1_A",    L"P1_B",   L"P1_C"     };
    static char*    part1_serials  [3] = { "23052012", "23051296", "23051293" };
    // particle2 backlight 6/7/8
    static wchar_t* part2_names    [3] = { L"P2_A",    L"P2_B",   L"P2_C"     };
    static char*    part2_serials  [3] = { "23051282", "23051995", "23052013" };
    // cosmetic sidewall   9/10/11
    static wchar_t* cosm1_names    [3] = { L"C1_A",    L"C1_B",   L"C1_C"     };
    static char*    cosm1_serials  [3] = { "23122406", "23122378", "23122396" };
    // needle/cap         12/13/14
    static wchar_t* cosm2_names    [3] = { L"C2_A",    L"C2_B",   L"C2_C"     };
    static char*    cosm2_serials  [3] = { "23144961", "23136625", "23144959" };
//    static char*    cosm2_serials  [3] = { "23118147", "23122384", "23122383" };
       
  #endif

#endif // TIVS_S40




	//#define TIVS_S60 // MODEL S60 / PREFILLED SYRINGES INSPECTION MC

#ifdef TIVS_S60 // MODEL TIVS-S60

  #define MODEL_NAME        L"TIVS-S60"
  #define VERSION_PC        L"125.100"
//  #define SERIAL_N 201600042   // Macter 

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  //#define NUM_TLC_PC1            19  // number of cameras connected to PC1
  //#define NUM_TLC_PC2             0  // number of cameras connected to PC2
  #define NUM_TLC_PC1            11  // number of cameras connected to PC1
  #define NUM_TLC_PC2             8  // number of cameras connected to PC2
  //#define NUM_TLC       NUM_TLC_PC1  //  (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  //#define NUMTRIGGER    NUM_TLC_PC1  // (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
  #define NUM_TLC         (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER      (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

//  #define APP_RESX             1280  // Horizontal resolution of application window
//  #define APP_RESY             1024  // Vertical resolution of application window
//  #define APP_RESX         1366   // Horizontal resolution of application window
//  #define APP_RESY          768   // Vertical resolution of application window
  #define APP_RESX         1920   // Horizontal resolution of application window
  #define APP_RESY         1080   // Vertical resolution of application window

  #define LAYOUT_FILENAME L"layout_S60.png"

  #if SERIAL_N==202000099 // New development S40 machine started October 2nd 2019 for Chongqing exhibition

    #define MACHINE_ID      "R1-10-172"//R1-10-172   //2020-00-001
    #define CUSTOMER_NAME  L"北京科兴中维生物技术有限公司"  
    #define CUSTOMER_NAMEE L"Beijing Kexing Pharmaceutical Co.,Ltd."  

    #define BATCHLOG_PARAMETERS // When defined changes to parameters will be added to batch report
    #define BATCHLOG_EVENTS     // When defined events/alarms will be added to batch report

  	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define NEW_PASSW_MNGR
	#define EMPTY_VIAL
	#define OPACITY_CTRL    // If defined enables opacity control for Yuxi Zerun

    #define SIDEWALL_PFS
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //Pour added on 20190514

	#define NEWCAP_V21       //Pour added on 20201120
	#define UPPER_COSMETIC   //Pour added on 20201120

	//#define KEYPOINTS_ANALYZE //Pour added on 20201017 for testing

//    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images

	#define STN2_FILLING_LEVEL

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
   //#define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	//For one PC
    //static UINT tlcgroupPC1[5][2] = { {0,2}, {3,6}, {7,10}, {11,14}, {15,18} }; // 
    //static UINT tlcgroupPC2[1][1] = { 0 }; // 

	//For two computers on 20210116
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,10} }; //
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // topcake - sidecake / particle2

    // CAMERAS
    //   stopper           0/1/2         top&flange   stopper sides A B 
    static wchar_t* stopper_names  [3] = { L"T_A",    L"C3_A",    L"C3_B"     };
    static char*    stopper_serials[3] = { "23370892", "23394328", "23370897" };
    // particle1 backlight 3/4/5/6
    static wchar_t* part1_names    [4] = { L"P1_A",    L"P1_B",   L"P1_C",    L"P1_D"     };
    static char*    part1_serials  [4] = { "23370945", "23362844", "23352401", "23362846" };
    // particle2 backlight 7/8/9/10
    static wchar_t* part2_names    [4] = { L"P2_A",    L"P2_B",   L"P2_C" ,    L"P2_D"    };
    static char*    part2_serials  [4] = { "23370902", "23394335", "23376260", "23370891" };
    // cosmetic sidewall   11/12/13/14
    static wchar_t* cosm1_names    [4] = { L"C1_A",    L"C1_B",   L"C1_C",    L"C1_D"     };
    static char*    cosm1_serials  [4] = { "23269430", "23407247", "23269415", "23269429" };
    // needle/cap         15/16/17/18
    static wchar_t* cosm2_names    [4] = { L"C2_A",    L"C2_B",   L"C2_C" ,    L"C2_D"    };
    static char*    cosm2_serials  [4] = { "23394337", "23418150", "23394334", "23383782" };
//    static char*    cosm2_serials  [3] = { "23118147", "23122384", "23122383" };
       
  #endif

#endif // TIVS_S60






#ifdef TIVS_A20L // MODEL TIVS-A20L

  #define MODEL_NAME        L"TIVS-A20L"
  #define VERSION_PC        L"121.227"
//  #define SERIAL_N 201600042   // Macter 

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1             9  // number of cameras connected to PC1
  #define NUM_TLC_PC2             0  // number of cameras connected to PC2
  #define NUM_TLC       NUM_TLC_PC1 //  (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER    NUM_TLC_PC1 // (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
  #define APP_RESX             1280  // Horizontal resolution of application window
  #define APP_RESY             1024  // Vertical resolution of application window

  #define LAYOUT_FILENAME L"layout_A20L.png"

  #if SERIAL_N==201600008 // Macter
    #define MACHINE_ID      "2016-00-008"
    #define CUSTOMER_NAME  L"Macter"  
    #define CRCAM  0x0D4E 
    #define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS
	//Pour added for HV data record 20180913
	#define HV_LEAKTEST
	#define HV_DATAREC

    #define NEW_TRIPLE_TIP

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images


    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[4][2] = { {0,2}, {3,4}, {5,6}, {7,8} }; // tip - part1 bk + part2 bott + part3 bott
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,5}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* tip_names     [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    tip_serials   [3] = { "21683535", "21683547", "21627148" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "21627161", "21629461" };

    static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "21629406", "21680785" };

    static wchar_t* part3_names   [2] = { L"P3_A",    L"P3_B"    };
    static char*    part3_serials [2] = { "21627149", "21627136" };

       
  #endif

	  #if SERIAL_N==201900109 // Aurobindo A20L 2019-009Vc
    #define MACHINE_ID      "2019-00-109"
    #define CUSTOMER_NAME  L"Aurobindo"  
    #define CRCAM  0x0FEE3 //65251
    #define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS
	//Pour added for HV data record 20180913
	#define HV_LEAKTEST
	#define HV_DATAREC

    #define NEW_TRIPLE_TIP

	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images


    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[4][2] = { {0,2}, {3,4}, {5,6}, {7,8} }; // tip - part1 bk + part2 bott + part3 bott
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,5}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* tip_names     [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    tip_serials   [3] = { "23179762", "23352211", "23352156" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "23370896", "23352188" };

    static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "23352207", "23352202" };

    static wchar_t* part3_names   [2] = { L"P3_A",    L"P3_B"    };
    static char*    part3_serials [2] = { "23352197", "23352190" };

       
  #endif


	#endif // TIVS_A20L



	//Pour added for new machine model TIVS-A20 20180523
	#ifdef TIVS_A20 // MODEL TIVS-A20

  #define MODEL_NAME        L"TIVS-A20"
  #define VERSION_PC        L"121.227"
//  #define SERIAL_N 201600042   // Macter 

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1             9  // number of cameras connected to PC1
  #define NUM_TLC_PC2             0  // number of cameras connected to PC2
  #define NUM_TLC       NUM_TLC_PC1 //  (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER    NUM_TLC_PC1 // (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
  #define APP_RESX             1280  // Horizontal resolution of application window
  #define APP_RESY             1024  // Vertical resolution of application window

  #define LAYOUT_FILENAME L"layout_A20L.png"

	#if SERIAL_N==201700023 // JV Remedy 2018-003Vc
    #define MACHINE_ID      "2018-003Vc"
    #define CUSTOMER_NAME  L"JV Remedy"
	#define CUSTOMER_NAMEE  L"JV Remedy"
    #define CRCAM  0x4A97 //19095
    //#define CRCODE 0xB32B // 45867
    //#define GRIPPER_STATISTICS     //Pour removed 20180818

    #define NEW_TRIPLE_TIP
	//Pour added for color detection after dye ingress testing 20180525
	#define COLOR_DETECTION_P1
	//#define SAVE_FULL_PROC
	//#define LOCALDEBUG
	//Pour added for color detection after dye ingress testing 20180525

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images


    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light
	#define STN3 COSMETIC_BACK    //Third station defined for cosmetic with backlight
   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[4][2] = { {0,2}, {3,4}, {5,6}, {7,8} }; // tip - part1 bk + part2 bott + part3 bott
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,5}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* tip_names     [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    tip_serials   [3] = { "22653174", "22653219", "22653225" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "22366417", "21423986" };

    static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "22376434", "22389177" };

    //static wchar_t* part3_names   [2] = { L"P3_A",    L"P3_B"    };
    //static char*    part3_serials [2] = { "22346073", "22376451" };

	static wchar_t* cosm_names   [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials [2] = { "22668734", "22593469" };

       
  #endif

#endif // TIVS_A20

	//Pour added for new machine model TIVS-A20 20180523


//Pour added 20180124

#ifdef TIVS_A40L // MODEL TIVS-A40L

  #define MODEL_NAME        L"TIVS-A40L"
  #define VERSION_PC        L"121.228"
//  #define SERIAL_N 201600042   // Macter 

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1             7  // number of cameras connected to PC1
  #define NUM_TLC_PC2             8  // number of cameras connected to PC2
  #define NUM_TLC       (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER    (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
  #define APP_RESX             1280  // Horizontal resolution of application window
  #define APP_RESY             1024  // Vertical resolution of application window

  #define STN1_FILLING_LEVEL    // add filling level to STN1 by default

  #define LAYOUT_FILENAME L"layout_A40L.png"

  #if SERIAL_N==201700009 // ANKANG BEIYIDA 2017-019V TIVS-A40L
    #define MACHINE_ID      "2017-00-009"
    #define CUSTOMER_NAME  L"BEIYIDA"  
    #define CRCAM  0xDC7F// 56447  Pour 20180124 
    //#define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS

    #define NEW_TRIPLE_TIP

	#define HV_DATAREC

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images


    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light
    #define STN3        COSMETIC_BACK    // Third  station defined for cosmetic with back light

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[4][2] = { {0,2}, {3,4}, {5,6}, {7,8} }; // tip - part1 bk + part2 bott + part3 bott
    //static UINT tlcgroupPC2[2][2] = { {0,3}, {4,5}        }; // topcake - sidecake / particle2

	static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

	static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22389128", "22364184", "22389138"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21691571", "21428844", "21692647", "22346090" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22389119", "22346072", "22346094", "22364191" };
//    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
//    static char*    cosm_serials [4] = { "22346540", "22346547", "22346541", "22346530" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "22346700", "22346708", "22346812", "22306229" };

  #endif
	//Pour added 20180131
	#if SERIAL_N==201700016 //ZISKA  2017-006Vc TIVS-A40L
    #define MACHINE_ID      "2017-00-016"
    #define CUSTOMER_NAME  L"ZISKA"  
    #define CRCAM  0x1AF0// 17098 // 1AF0 //Pour 20180201 6896
    //#define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS

    #define NEW_TRIPLE_TIP
	
	#define HV_DATAREC

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images


    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light
    //#define STN3        COSMETIC_BACK    // Third  station defined for cosmetic with back light

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[4][2] = { {0,2}, {3,4}, {5,6}, {7,8} }; // tip - part1 bk + part2 bott + part3 bott
    //static UINT tlcgroupPC2[2][2] = { {0,3}, {4,5}        }; // topcake - sidecake / particle2

	static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

	static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22468103", "22468128", "22468133"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22468130", "22468140", "21605969", "22468122" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22468093", "22468132", "22468131", "22441849" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21953327", "21931949", "21952423", "21953336" };
	//Pour changed from cosmetic to particle 3 20180525
    //static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    //static char*    cosm_serials [4] = { "21592620", "21594603", "21592613", "21589525" };

  #endif

		//Pour added 20190328
	#if SERIAL_N==201800031 //GPO  2018-010Vc TIVS-A40L
    #define MACHINE_ID      "2018-00-031"
    #define CUSTOMER_NAME  L"GPO"  
    #define CRCAM  0xFF84// 65412 // Pour 20190328
    //#define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS

    #define NEW_TRIPLE_TIP
	
	#define HV_DATAREC

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images


    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light
    //#define STN3        COSMETIC_BACK    // Third  station defined for cosmetic with back light

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[4][2] = { {0,2}, {3,4}, {5,6}, {7,8} }; // tip - part1 bk + part2 bott + part3 bott
    //static UINT tlcgroupPC2[2][2] = { {0,3}, {4,5}        }; // topcake - sidecake / particle2

	static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

	static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22584241", "22671106", "22584229"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22671125", "22697696", "22534414", "22565434" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22697725", "22697702", "22658615", "22584254" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "22671112", "22581646", "22581612", "22658614" };
	//Pour changed from cosmetic to particle 3 20180525
    //static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    //static char*    cosm_serials [4] = { "21592620", "21594603", "21592613", "21589525" };

  #endif

#endif // TIVS_A40L

//Pour added 20180124



//#define SERIAL_N 201400042   // Mexico Ultra Tech De CV

#ifdef TIVS_C20

  #define MODEL_NAME        L"TIVS-C20"
  #define VERSION_PC        L"120.114"

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1       8  // number of cameras connected to PC1
  #define NUM_TLC_PC2       8  // number of cameras connected to PC2
//number of first C20 machine
//#define NUM_TLC_PC1       10  // number of cameras connected to PC1
//#define NUM_TLC_PC2        6  // number of cameras connected to PC2
  #define NUM_TLC      (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER        16 // (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
  #define APP_RESX        1280  // Horizontal resolution of application window
  #define APP_RESY        1024  // Vertical resolution of application window

  #define LAYOUT_FILENAME L"layout_c20.png"

  #if SERIAL_N==201400042 // Mexico Ultra Tech De CV
    #define MACHINE_ID      "2014-00-042"
    #define CUSTOMER_NAME  L"Ulsa Tech"  
    #define CRCAM  0x6FFC 
    #define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

//    #define NEW_TRIPLE_TIP
    #define PARTFLT_H6V3
//    #define PARTFLT_H8V4
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1_FILLING_LEVEL
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3,4} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,1}, {2,3} }; // partic2 - cosmetic

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,9} }; // cap - 2 bottoms+focolor - particle+cosmetic
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,5}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "21502471", "21499092", "21499054" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "21502474", "21502499", "21425009" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "21520535", "21502505" };

    static wchar_t* cosm_names    [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials  [2] = { "21405196", "21404716" };

    // PC2 CAMERAS
    static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "21502504", "21502489" };

    static wchar_t* sideck_names  [2] = { L"SC_A",    L"SC_B"    };
    static char*    topck_serials [2] = { "21405187", "21400223" };

    static wchar_t* topck_names   [2] = { L"TC_A",    L"TC_B"    };
    static char*    sideck_serials[2] = { "21422569", "21420103" };
       
  #endif

	//Pour added 20190318
	  #if SERIAL_N==201800030 // 2018-011Vc
    #define MACHINE_ID      "2018-00-030"
    #define CUSTOMER_NAME  L"Pharmasyntez-Nord"  
    #define CRCAM  0x187E //6270
    #define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	#define TEST_QILU
	#define NECK_INSPECTION
//    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
//    #define PARTFLT_H8V4
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
	#define NEW_PARTICLE //Pour marked 20190318
    #define STN1 PARTICLE_BACK  // First  station defined for particle with Backlight  //Pour
    #define STN1_FILLING_LEVEL
	#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images //Pour
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
	//#define CV_TEST

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3,4} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,1}, {2,3} }; // partic2 - cosmetic

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,7} }; // cap - 2 bottoms+focolor - particle+cosmetic
	static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "22534485", "22534499", "22534486" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "22649363", "22653221", "22390057" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "22647544", "22653239" };

	// PC2 CAMERAS
    static wchar_t* cosm_names    [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials  [2] = { "21538477", "22667669" };

    static wchar_t* sideck_names  [2] = { L"SC_A",    L"SC_B"    };
    static char*    sideck_serials [2]= { "22668722", "22667686" };

    static wchar_t* topck_names   [2] = { L"TC_A",    L"TC_B"    };
    static char*    topck_serials [2] = { "21923560", "21923565" };

	static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "21951257", "22584223" };
       
  #endif
	//Pour added 20190318


	//Pour added 20190917
	#if SERIAL_N==201900019 // 2018-012Vc
    #define MACHINE_ID      "2019-00-019"
    #define CUSTOMER_NAME  L"Pharmasyntez-Nord" 
	#define CUSTOMER_NAMEE  L"Pharmasyntez-Nord" 
    #define CRCAM  0x4F2A //20266
    #define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	#define TEST_QILU
//    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
//    #define PARTFLT_H8V4
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
	#define NEW_PARTICLE //Pour marked 20190318
    #define STN1 PARTICLE_BACK  // First  station defined for particle with Backlight  //Pour
    #define STN1_FILLING_LEVEL
	#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images //Pour
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light

	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define NEWCOSMETIC_COMBO     //Pour 20191123
	#define NEW_PASSW_MNGR  // New users/passwords manager
   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3,4} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,1}, {2,3} }; // partic2 - cosmetic

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,7} }; // cap - 2 bottoms+focolor - particle+cosmetic
	static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "23051291", "23051316", "23051294" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "23051295", "23052005", "22788598" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "23052020", "23052019" };

	// PC2 CAMERAS
    static wchar_t* cosm_names    [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials  [2] = { "22953980", "22868344" };

    static wchar_t* sideck_names  [2] = { L"SC_A",    L"SC_B"    };
    static char*    sideck_serials [2]= { "22874688", "22865025" };

    static wchar_t* topck_names   [2] = { L"TC_A",    L"TC_B"    };
    static char*    topck_serials [2] = { "22874704", "21920773" };

	static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "23051312", "23051359" };
       
  #endif
	//Pour added 20190917

		//Pour added 20190803
	  #if SERIAL_N==201900028 // 2019-005V TIVS-C20L
    #define MACHINE_ID      "2019-00-015"
    #define CUSTOMER_NAME  L"原子高科" 
	#define CUSTOMER_NAMEE  L"原子高科" 
    #define CRCAM  0x8500 //63635-F893//34048-8500
    #define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	#define TEST_QILU

	#define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

	#define NEWCOSMETIC_COMBO     //Pour 20191123
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE

//    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
//    #define PARTFLT_H8V4
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
	#define NEW_PARTICLE //Pour marked 20190318
    #define STN1 PARTICLE_BACK  // First  station defined for particle with Backlight  //Pour
    #define STN1_FILLING_LEVEL
	#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images //Pour
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3,4} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,1}, {2,3} }; // partic2 - cosmetic

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,7} }; // cap - 2 bottoms+focolor - particle+cosmetic
	static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "23032331", "23120677", "23032336" };//23019190 changed to 23120677 on 20190922

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "23032338", "22965502", "22337089" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "23052024", "23001485" };

	// PC2 CAMERAS
    static wchar_t* cosm_names    [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials  [2] = { "22957860", "22967114" };

    static wchar_t* sideck_names  [2] = { L"SC_A",    L"SC_B"    };
    static char*    sideck_serials [2]= { "22971135", "22954019" };

    static wchar_t* topck_names   [2] = { L"TC_A",    L"TC_B"    };
    static char*    topck_serials [2] = { "22945618", "22967112" };

	static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "23032324", "22965491" };
       
  #endif
	//Pour added 20190803


	//Pour added 20191122
	#if SERIAL_N==201900026 // 2019-005V TIVS-C20L
    #define MACHINE_ID      "2019-00-026"
    #define CUSTOMER_NAME  L"澳斯康" 
	#define CUSTOMER_NAMEE  L"澳斯康" 
    #define CRCAM  0x5CA2 //23714
    #define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	#define TEST_QILU
	#define NECK_CRACK //Pour added for testing on 20200320

	#define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

//    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
//    #define PARTFLT_H8V4
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
	#define NEW_PARTICLE //Pour marked 20190318
	#define OPENCV_PARTICLE
    #define STN1 PARTICLE_BACK  // First  station defined for particle with Backlight  //Pour
    #define STN1_FILLING_LEVEL
	#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images //Pour
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light

	#define NEWCOSMETIC_COMBO     //Pour 20191123
	//Pour added 20201106
	//#define UPPER_COSMETIC

	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3,4} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,1}, {2,3} }; // partic2 - cosmetic

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,7} }; // cap - 2 bottoms+focolor - particle+cosmetic
	static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "22997928", "22971251", "23136628" };//23019190 changed to 23120677 on 20190922

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "23001486", "23051302", "23164063" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "23136257", "23153117" };

	// PC2 CAMERAS
    static wchar_t* cosm_names    [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials  [2] = { "23084261", "23122409" };

    static wchar_t* sideck_names  [2] = { L"SC_A",    L"SC_B"    };
    static char*    sideck_serials [2]= { "23174139", "23084267" };

    static wchar_t* topck_names   [2] = { L"TC_A",    L"TC_B"    };
    static char*    topck_serials [2] = { "23174154", "23084265" };

	static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "23136620", "23051344" };
       
  #endif
	//Pour added 20191122



			//Pour added 20200321
	#if SERIAL_N==202000004 // 2019-007Vc TIVS-C20
    #define MACHINE_ID      "2020-00-004"
    #define CUSTOMER_NAME  L"Pharmasyntez" 
	#define CUSTOMER_NAMEE  L"Pharmasyntez" 
    #define CRCAM  0x8986 //35206
	//#define CRCAM  0xA973 //43379 P1-P2 exchanged
    #define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	#define TEST_QILU

//	#define HGA_LEAKTEST        // HGA components installed
//  #define HGA_DATAREC         // enable option to record HGA data

//    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
//    #define PARTFLT_H8V4
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
	#define NEW_PARTICLE //Pour marked 20190318
    #define STN1 PARTICLE_BACK  // First  station defined for particle with Backlight  //Pour
    #define STN1_FILLING_LEVEL
	#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images //Pour
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light

	#define NEWCOSMETIC_COMBO     //Pour 20191123
	//Pour added 20201106
	//#define UPPER_COSMETIC

	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3,4} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,1}, {2,3} }; // partic2 - cosmetic

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,7} }; // cap - 2 bottoms+focolor - particle+cosmetic
	static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "22965532", "23019201", "22965515" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "22965520", "22965489", "23164054" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "22965519", "22957330" };

	//static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    //static char*    part1_serials [2] = { "22965511", "22965517" };

	// PC2 CAMERAS
    static wchar_t* cosm_names    [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials  [2] = { "23240191", "23240186" };

    static wchar_t* sideck_names  [2] = { L"SC_A",    L"SC_B"    };
    static char*    sideck_serials [2]= { "23200092", "23230414" };

    static wchar_t* topck_names   [2] = { L"TC_A",    L"TC_B"    };
    static char*    topck_serials [2] = { "23230406", "23108150" };

	static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "22965511", "22965517" };

	//static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    //static char*    part2_serials [2] = { "22965519", "22957330" };
       
  #endif
	//Pour added 20200321


	//Pour added 20200807
	#if SERIAL_N==202000012 // 2010-011Vc TIVS-C20
    #define MACHINE_ID      "2020-00-011"
    #define CUSTOMER_NAME  L"Aurobindo" 
	#define CUSTOMER_NAMEE  L"Aurobindo" 
    #define CRCAM  0xFAA5 //64165
	//#define CRCAM  0xA973 //43379 P1-P2 exchanged
    #define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	#define TEST_QILU

	#define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

//    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
//    #define PARTFLT_H8V4
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
	#define NEW_PARTICLE //Pour marked 20190318
	#define OPENCV_PARTICLE
    #define STN1 PARTICLE_BACK  // First  station defined for particle with Backlight  //Pour
    #define STN1_FILLING_LEVEL
	#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images //Pour
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light

	#define NEWCOSMETIC_COMBO     //Pour 20191123
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3,4} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,1}, {2,3} }; // partic2 - cosmetic

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,7} }; // cap - 2 bottoms+focolor - particle+cosmetic
	static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "23352128", "23352149", "23352150" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "23352176", "23352182", "23164052" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "23144946", "23352172" };

	//static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    //static char*    part1_serials [2] = { "22965511", "22965517" };

	// PC2 CAMERAS
    static wchar_t* cosm_names    [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials  [2] = { "23269419", "23269446" };

    static wchar_t* sideck_names  [2] = { L"SC_A",    L"SC_B"    };
    static char*    sideck_serials [2]= { "23269457", "23269455" };

    static wchar_t* topck_names   [2] = { L"TC_A",    L"TC_B"    };
    static char*    topck_serials [2] = { "23269425", "23269443" };

	static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "23352187", "23136272" };

	//static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    //static char*    part2_serials [2] = { "22965519", "22957330" };
       
  #endif
	//Pour added 20200807


		//Pour added 20201114
	#if SERIAL_N==202000103 // 2020-003Vc TIVS-C20
    #define MACHINE_ID      "2020-003Vc"
    #define CUSTOMER_NAME  L"JSC R-PHARM" 
	#define CUSTOMER_NAMEE  L"JSC R-PHARM" 
    #define CRCAM  0x923B //37435
	//#define CRCAM  0xA973 //43379 P1-P2 exchanged
    #define CRCODE 0xB32B // 45867
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	#define TEST_QILU

	//#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

//    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
//    #define PARTFLT_H8V4
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
	#define NEW_PARTICLE //Pour marked 20190318
	#define OPENCV_PARTICLE
    #define STN1 PARTICLE_BACK  // First  station defined for particle with Backlight  //Pour
    #define STN1_FILLING_LEVEL
	#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images //Pour
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light

	#define NEWCOSMETIC_COMBO     //Pour 20191123
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE
	#define DOUBLE_REJET
   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3,4} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,1}, {2,3} }; // partic2 - cosmetic

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,7} }; // cap - 2 bottoms+focolor - particle+cosmetic
	static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "23464370", "23464401", "23464349" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "23464350", "23464383", "23389768" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "23464397", "23464389" };

	// PC2 CAMERAS
    static wchar_t* cosm_names    [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials  [2] = { "23505885", "23505894" };

    static wchar_t* sideck_names  [2] = { L"SC_A",    L"SC_B"    };
    static char*    sideck_serials [2]= { "23505880", "23505876" };

    static wchar_t* topck_names   [2] = { L"TC_A",    L"TC_B"    };
    static char*    topck_serials [2] = { "23505895", "23513631" };

	static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "23464384", "23464368" };

  #endif
	//Pour added 20200807


		//Pour added 20201114
	#if SERIAL_N==202000104 // 2020-004Vc TIVS-C20
    #define MACHINE_ID      "2020-004Vc"
    #define CUSTOMER_NAME  L"JSC R-PHARM" 
	#define CUSTOMER_NAMEE  L"JSC R-PHARM" 
    #define CRCAM  0x91ED //37537
	//#define CRCAM  0xA973 //43379 P1-P2 exchanged
    //#define CRCODE 0x91ED // 37357
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	#define TEST_QILU

	//#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

//    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
//    #define PARTFLT_H8V4
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
	#define NEW_PARTICLE //Pour marked 20190318
	#define OPENCV_PARTICLE
    #define STN1 PARTICLE_BACK  // First  station defined for particle with Backlight  //Pour
    #define STN1_FILLING_LEVEL
	#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images //Pour
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light

	#define NEWCOSMETIC_COMBO     //Pour 20191123
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE
	#define DOUBLE_REJET
   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3,4} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,1}, {2,3} }; // partic2 - cosmetic

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,7} }; // cap - 2 bottoms+focolor - particle+cosmetic
	static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // topcake - sidecake / particle2

    // PC1 CAMERAS
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "23464382", "23464365", "23464372" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "23464381", "23464348", "23389766" };

    static wchar_t* part1_names   [2] = { L"P1_A",    L"P1_B"    };
    static char*    part1_serials [2] = { "23464361", "23464346" };

	// PC2 CAMERAS
    static wchar_t* cosm_names    [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials  [2] = { "23505892", "23505891" };

    static wchar_t* sideck_names  [2] = { L"SC_A",    L"SC_B"    };
    static char*    sideck_serials [2]= { "23505862", "23505877" };

    static wchar_t* topck_names   [2] = { L"TC_A",    L"TC_B"    };
    static char*    topck_serials [2] = { "23505857", "23513610" };

	static wchar_t* part2_names   [2] = { L"P2_A",    L"P2_B"    };
    static char*    part2_serials [2] = { "23464354", "23464355" };

  #endif
	//Pour added 20200807



#endif // TIVS_C20


 #ifdef TIVS_C40 // MODEL TIVS-C40

  #define MODEL_NAME        L"TIVS-C40"
  #define VERSION_PC        L"151.001"
//  #define SERIAL_N 201600042   // Macter 

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1            12  // number of cameras connected to PC1
  #define NUM_TLC_PC2             9  // number of cameras connected to PC2
  #define NUM_TLC       (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER    (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
  #define APP_RESX             1280  // Horizontal resolution of application window
  #define APP_RESY             1024  // Vertical resolution of application window

  #define LAYOUT_FILENAME L"layout_C40L.png"


  // TIVS-C40_001 (nohga).tsm
  // TIVS-C40_001.pro
  #if SERIAL_N==201800010   // New C40 machine 成都康华生物 Chengdu Kanghua Biological 2018-010V
    #define MACHINE_ID      "2018-00-010"
    #define CUSTOMER_NAME   L"成都康华生物" //  Chengdu Kanghua Biological
    #define CUSTOMER_NAMEE  L"Chengdu Kanghua Biological"  
    #define CRCAM  0x9DDE
    #define CRCODE 0xA312 // ‭41746‬
    #define GRIPPER_STATISTICS

    //#define OUTPUT_BELT    // if defined output belt management and parameters enabled

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define NEWCAP_V40          // V40 new_cap inspection for Chengdu Kanghua Biological 2018-00-010
    #define FO_COLOR            // If defined camera is searched ad added for flip off color control
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define NEWCOSMETIC_C40   // Special version for C40 (NEWCOSMETIC_V20 for cosmetic stn in liquid mode only)
    #define NEWCOSMETIC_COMBO   // New cosmetic for combo machines 

//    #define HGA_LEAKTEST    // HGA components installed - Aggiunto 20170303 per prove in officina su macchina modificata
//    #define HGA_DATAREC         // enable option to record HGA data

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define HEEL_INSPECTION       // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[4][2] = { {0,2}, {3,5}, {6,8}, {9,11} }; // cap - 2 bottoms+focolor - part1 - sidecake
    static UINT tlcgroupPC2[3][2] = { {0,2}, {3,5}, {6,8}         }; // part2 - heel - cosmetic


    // PC1 CAMERAS

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "22581645", "22581654", "22697726" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "22697736", "22697722", "22401169" };

    static wchar_t* part1_names   [3] = { L"P1_A",    L"P1_B",    L"P1_C"    }; // S1
    static char*    part1_serials [3] = { "22697712", "22697704", "22697718" };

    static wchar_t* sideck_names  [3] = { L"SC_A",    L"SC_B",    L"SC_C"    }; // S2
    static char*    sideck_serials[3] = { "22550546", "22677539", "22550553" };

    // PC2 CAMERAS

    static wchar_t* cosm_names    [3] = { L"CM_A",    L"CM_B",    L"CM_C"    }; // S5 6-7-8
    static char*    cosm_serials  [3] = { "22550542", "22547120", "22547106" };

    static wchar_t* topck_names   [3] = { L"HL_A",    L"HL_B",    L"HL_C",   }; // S4 3-4-5
    static char*    topck_serials [3] = { "22547146", "22547128", "22550545" };

    static wchar_t* part2_names   [3] = { L"P2_A",    L"P2_B",    L"P2_C"    }; // S3 0-1-2
    static char*    part2_serials [3] = { "22697701", "22584246", "22697717" };

  #endif


  // TIVS-C40_002.tsm
  // TIVS-C40_001.pro
  #if SERIAL_N==201700010   // New development C40 machine
    #define MACHINE_ID      "2017-00-010"
    #define CUSTOMER_NAME   L"安科生物" //  ANKEBIO  
    #define CUSTOMER_NAMEE  L"ANKEBIO"  
    #define CRCAM  0x0D47 // 0D35 
    #define CRCODE 0xB320 // 45865
    #define GRIPPER_STATISTICS

    #define OUTPUT_BELT    // if defined output belt management and parameters enabled

//    #define NEW_TRIPLE_TIP
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define FO_COLOR            // If defined camera is searched ad added for flip off color control
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define HGA_LEAKTEST    // HGA components installed - Aggiunto 20170303 per prove in officina su macchina modificata
    #define HGA_DATAREC         // enable option to record HGA data

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define HEEL_INSPECTION       // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[4][2] = { {0,2}, {3,5}, {6,8}, {9,11} }; // cap - 2 bottoms+focolor - part1 - sidecake
    static UINT tlcgroupPC2[3][2] = { {0,2}, {3,5}, {6,8}         }; // part2 - heel - cosmetic


    // PC1 CAMERAS

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "21567190", "21571215", "21629443" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "22389175", "22389136", "21404691" };

    static wchar_t* part1_names   [3] = { L"P1_A",    L"P1_B",    L"P1_C"    };
    static char*    part1_serials [3] = { "21428728", "21691584", "21603548" };

    static wchar_t* sideck_names  [3] = { L"SC_A",    L"SC_B",    L"SC_C"    };
    static char*    sideck_serials[3] = { "22342336", "22346753", "22346788" };

    // PC2 CAMERAS

    static wchar_t* cosm_names    [3] = { L"CM_A",    L"CM_B",    L"CM_C"    };
    static char*    cosm_serials  [3] = { "21374465", "21374041", "22306290" };
    // Heel
    static wchar_t* topck_names   [3] = { L"HL_A",    L"HL_B",    L"HL_C",   };
    static char*    topck_serials [3] = { "21374477", "21374052", "21374048" };

    static wchar_t* part2_names   [3] = { L"P2_A",    L"P2_B",    L"P2_C"    };
    static char*    part2_serials [3] = { "22346085", "21499097", "21499090" };
      
  #endif // SERIAL_N==201700010


//#define SERIAL_N 201800011   // New C40L machine with HGA and 3rd deviator - BIOCAD CJSC - (CM0)
  // TIVS-C40_002 (3devs).tsm
  // TIVS-C40_002.pro
  #if SERIAL_N==201800011   // New C40L machine with HGA and 3rd deviator - BIOCAD CJSC - (CM0)
    #define MACHINE_ID      "2018-00-011"
    #undef  MODEL_NAME
    #define MODEL_NAME      L"TIVS-C40L"

    #undef  LAYOUT_FILENAME
    #define LAYOUT_FILENAME L"layout_C40L_BIOCAD.png"

    #define CUSTOMER_NAME   L"BIOCAD CJSC" //  BIOCAD CJSC  
    #define CUSTOMER_NAMEE  L"BIOCAD CJSC"  
    #define CRCAM  0x2710  
    #define CRCODE 0xC143 // ‭49475‬
    #define GRIPPER_STATISTICS

    #define ALARMLOG_SEPARATE // log alarms separately and add button to show them
    #define SHOW_BATCHNUMBER  // show current batch number in machine_txt field
    #define OUTPUT_BELT      // if defined output belt management and parameters enabled
    #define THIRD_DEVIATOR   // if defined 3rd deviator management and parameters enabled
    #define INPUTBELT_SENSOR        // if defined enables input belt sensor delays
    #define INPSCREW_ALARM        // if defined enables input screw alarm in PLC
    #define OUTTURNTABLE_SENSOR   // if defined enables stopping when output turntable is full

//    #define NEW_TRIPLE_TIP
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define FO_COLOR            // If defined camera is searched ad added for flip off color control
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
//    #define NEWCOSMETIC_C40     // Special version for C40 (NEWCOSMETIC_V20 for cosmetic stn in liquid mode only)
    #define NEWCOSMETIC_COMBO   // New cosmetic for combo machines 


    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define HGA_LEAKTEST    // HGA components installed - Aggiunto 20170303 per prove in officina su macchina modificata
    #define HGA_DATAREC         // enable option to record HGA data

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define HEEL_INSPECTION       // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[4][2] = { {0,2}, {3,5}, {6,8}, {9,11} }; // cap - 2 bottoms+focolor - part1 - sidecake
    static UINT tlcgroupPC2[3][2] = { {0,2}, {3,5}, {6,8}         }; // part2 - heel - cosmetic


    // PC1 CAMERAS
    // CAP
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "22676247", "22676244", "22647543" };
    // BOTTOM AND FLIP OFF COLOR
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "22676233", "22676262", "22390046" };
    // S1 - PARTICLE1
    static wchar_t* part1_names   [3] = { L"P1_A",    L"P1_B",    L"P1_C"    };
    static char*    part1_serials [3] = { "22671118", "22676255", "22653212" };
    // S2 - SIDECAKE
    static wchar_t* sideck_names  [3] = { L"SC_A",    L"SC_B",    L"SC_C"    };
    static char*    sideck_serials[3] = { "22802791", "22805681", "22802792" };

    // PC2 CAMERAS

    // S3 - PARTICLE2
    static wchar_t* part2_names   [3] = { L"P2_A",    L"P2_B",    L"P2_C"    };
    static char*    part2_serials [3] = { "22671132", "22671138", "22676237" }; //"22671150" };
//    static char*    part2_serials [3] = { "22671132", "22796291", "22671150" };
    // S4 - HEEL
    static wchar_t* topck_names   [3] = { L"HL_A",    L"HL_B",    L"HL_C",   };
    static char*    topck_serials [3] = { "22802795", "22798831", "22805679" };
    // S5 - COSMETIC
    static wchar_t* cosm_names    [3] = { L"CM_A",    L"CM_B",    L"CM_C"    };
    static char*    cosm_serials  [3] = { "22802794", "22802787", "22805696" };
      
  #endif // SERIAL_N==201800011

	//#define SERIAL_N 201800011   // New C40L machine with HGA and 3rd deviator - BIOCAD CJSC - (CM0)
  // TIVS-C40_002 (3devs).tsm
  // TIVS-C40_002.pro
  #if SERIAL_N==201900198   // New C40L machine with HGA and 3rd deviator - BIOCAD CJSC - (CM0) 2019-008Vc
    #define MACHINE_ID      "2019-00-008"
    #undef  MODEL_NAME
    #define MODEL_NAME      L"TIVS-C40L"

    #undef  LAYOUT_FILENAME
    #define LAYOUT_FILENAME L"layout_C40L_BIOCAD.png"

    #define CUSTOMER_NAME   L"BIOCAD CJSC" //  BIOCAD CJSC  
    #define CUSTOMER_NAMEE  L""  
    #define CRCAM  0x6052  
    #define CRCODE 0xC143 // ‭49475‬
    #define GRIPPER_STATISTICS
	//#define NEW_LOGGER // log alarms separately and add button to show them
    #define ALARMLOG_SEPARATE // log alarms separately and add button to show them
    #define SHOW_BATCHNUMBER  // show current batch number in machine_txt field
    #define OUTPUT_BELT      // if defined output belt management and parameters enabled
    #define THIRD_DEVIATOR   // if defined 3rd deviator management and parameters enabled
    #define INPUTBELT_SENSOR        // if defined enables input belt sensor delays
    #define INPSCREW_ALARM        // if defined enables input screw alarm in PLC
    //#define OUTTURNTABLE_SENSOR   // if defined enables stopping when output turntable is full

//    #define NEW_TRIPLE_TIP
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define FO_COLOR            // If defined camera is searched ad added for flip off color control
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
//    #define NEWCOSMETIC_C40     // Special version for C40 (NEWCOSMETIC_V20 for cosmetic stn in liquid mode only)
    #define NEWCOSMETIC_COMBO   // New cosmetic for combo machines 


    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define HGA_LEAKTEST    // HGA components installed - Aggiunto 20170303 per prove in officina su macchina modificata
    #define HGA_DATAREC         // enable option to record HGA data

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define HEEL_INSPECTION       // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[4][2] = { {0,2}, {3,5}, {6,8}, {9,11} }; // cap - 2 bottoms+focolor - part1 - sidecake
    static UINT tlcgroupPC2[3][2] = { {0,2}, {3,5}, {6,8}         }; // part2 - heel - cosmetic


    // PC1 CAMERAS
    // CAP
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "23352145", "23352125", "23352119" };
    // BOTTOM AND FLIP OFF COLOR
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "23352154", "23352151", "22788607" };
    // S1 - PARTICLE1
    static wchar_t* part1_names   [3] = { L"P1_A",    L"P1_B",    L"P1_C"    };
    static char*    part1_serials [3] = { "23352130", "23352127", "23352124" };
    // S2 - SIDECAKE
    static wchar_t* sideck_names  [3] = { L"SC_A",    L"SC_B",    L"SC_C"    };
    static char*    sideck_serials[3] = { "23122384", "23118147", "23269691" };

    // PC2 CAMERAS

    // S3 - PARTICLE2
    static wchar_t* part2_names   [3] = { L"P2_A",    L"P2_B",    L"P2_C"    };
    static char*    part2_serials [3] = { "23352137", "23352141", "23352135" }; //"22671150" };
//    static char*    part2_serials [3] = { "22671132", "22796291", "22671150" };
    // S4 - HEEL
    static wchar_t* topck_names   [3] = { L"HL_A",    L"HL_B",    L"HL_C",   };
    static char*    topck_serials [3] = { "23283309", "23122383", "23269420" };
    // S5 - COSMETIC
    static wchar_t* cosm_names    [3] = { L"CM_A",    L"CM_B",    L"CM_C"    };
    static char*    cosm_serials  [3] = { "23269438", "23269689", "23275140" };
      
  #endif // SERIAL_N==201900198



		//#define  2020-014V   // New C40 machine
  // TIVS-C40_002 (3devs).tsm
  // TIVS-C40_002.pro
  #if SERIAL_N==202000014   // New C40 machine 2020-014V
    #define MACHINE_ID      "2020-00-014"
	#define MACHINE_ID2     L"TIVS-C40"
    #undef  MODEL_NAME
    #define MODEL_NAME      L"TIVS-C40"

    #undef  LAYOUT_FILENAME
    #define LAYOUT_FILENAME L"layout_C40.png"

    #define CUSTOMER_NAME   L"厦门特宝生物工程股份有限公司灯检记录" //    
    #define CUSTOMER_NAMEE  L"厦门特宝"  
    #define CRCAM  0x8D83//36227  
    #define CRCODE 0xC143 // ‭49475‬
    #define GRIPPER_STATISTICS
	#define NEW_LOGGER      // uses new log encyrption and digital signature
    #define ALARMLOG_SEPARATE // log alarms separately and add button to show them
    #define SHOW_BATCHNUMBER  // show current batch number in machine_txt field
    #define OUTPUT_BELT      // if defined output belt management and parameters enabled
    //#define THIRD_DEVIATOR   // if defined 3rd deviator management and parameters enabled
    #define INPUTBELT_SENSOR        // if defined enables input belt sensor delays
    //#define INPSCREW_ALARM        // if defined enables input screw alarm in PLC
    //#define OUTTURNTABLE_SENSOR   // if defined enables stopping when output turntable is full

//    #define NEW_TRIPLE_TIP
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define FO_COLOR            // If defined camera is searched ad added for flip off color control
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
//    #define NEWCOSMETIC_C40     // Special version for C40 (NEWCOSMETIC_V20 for cosmetic stn in liquid mode only)
    #define NEWCOSMETIC_COMBO   // New cosmetic for combo machines 

	//#define OPACITY_CRTL
	#define OPACITY_CTRL

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define NEW_PASSW_MNGR  // New users/passwords manager

    //#define HGA_LEAKTEST    // HGA components installed - Aggiunto 20170303 per prove in officina su macchina modificata
    //#define HGA_DATAREC         // enable option to record HGA data

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    //#define HEEL_INSPECTION       // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[4][2] = { {0,2}, {3,5}, {6,8}, {9,11} }; // cap - 2 bottoms+focolor - part1 - sidecake
    static UINT tlcgroupPC2[3][2] = { {0,2}, {3,5}, {6,8}         }; // part2 - heel - cosmetic


    // PC1 CAMERAS
    // CAP
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "23418158", "23394333", "23352388" };
    // BOTTOM AND FLIP OFF COLOR
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "23394336", "23362850", "22967177" };
    // S1 - PARTICLE1
    static wchar_t* part1_names   [3] = { L"P1_A",    L"P1_B",    L"P1_C"    };
    static char*    part1_serials [3] = { "23370893", "23376257", "23370943" };
    // S2 - SIDECAKE
    static wchar_t* sideck_names  [3] = { L"SC_A",    L"SC_B",    L"SC_C"    };
    static char*    sideck_serials[3] = { "23407245", "23407240", "23419747" };

    // PC2 CAMERAS

    // S3 - PARTICLE2
    static wchar_t* part2_names   [3] = { L"P2_A",    L"P2_B",    L"P2_C"    };
    static char*    part2_serials [3] = { "23362879", "23370895", "23370903" }; //"22671150" };
//    static char*    part2_serials [3] = { "22671132", "22796291", "22671150" };
    // S4 - HEEL
    static wchar_t* topck_names   [3] = { L"HL_A",    L"HL_B",    L"HL_C",   };
    static char*    topck_serials [3] = { "23419743", "23419736", "23419731" };
    // S5 - COSMETIC
    static wchar_t* cosm_names    [3] = { L"CM_A",    L"CM_B",    L"CM_C"    };
    static char*    cosm_serials  [3] = { "23407250", "23407246", "23419749" };
      
  #endif // 2020-014V 20200819





// cambiata 20190223 configurazione (no più HGA) per cambio da parte del cliente
  // TIVS-C40_002 (no output belt no hga).tsm
  // TIVS-C40_002.pro
    
// OLD    //#define SERIAL_N 201800019   // New C40L machine  - (CM0)
  // TIVS-C40_002 (no output belt).tsm
  // TIVS-C40_001.pro
  #if SERIAL_N==201800019   // New C40L machine //2018-028V
    #define MACHINE_ID      "2018-028V"//2018-00-019
    #define CUSTOMER_NAME   L"上海生物制品研究所有限责任公司" // Shanghai Tengrui Pharmaceutical  上海腾瑞制药有限公司
    #define CUSTOMER_NAMEE  L"上海生物制品研究所有限责任公司"  
    #define CRCAM  0x6E79 
    //#define CRCODE 0x1685 // 5765
    #define GRIPPER_STATISTICS

 /*
    #undef  LAYOUT_FILENAME
    #define LAYOUT_FILENAME L"layout_C40L_BIOCAD.png"
    #define GRIPPER_STATISTICS
    #define ALARMLOG_SEPARATE // log alarms separately and add button to show them
    #define SHOW_BATCHNUMBER  // show current batch number in machine_txt field
    #define THIRD_DEVIATOR   // if defined 3rd deviator management and parameters enabled
    #define INPUTBELT_SENSOR        // if defined enables input belt sensor delays
    #define INPSCREW_ALARM        // if defined enables input screw alarm in PLC
    #define OUTTURNTABLE_SENSOR   // if defined enables stopping when output turntable is full
 */

 // NO OUTPUT BELT   #define OUTPUT_BELT    // if defined output belt management and parameters enabled
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define FO_COLOR            // If defined camera is searched ad added for flip off color control
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEWCOSMETIC_COMBO   // New cosmetic for combo machines 

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define NEW_PASSW_MNGR  // New users/passwords manager

//    #define HGA_LEAKTEST    // HGA components installed - Aggiunto 20170303 per prove in officina su macchina modificata
//    #define HGA_DATAREC         // enable option to record HGA data

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define HEEL_INSPECTION       // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[4][2] = { {0,2}, {3,5}, {6,8}, {9,11} }; // cap - 2 bottoms+focolor - part1 - sidecake
    static UINT tlcgroupPC2[3][2] = { {0,2}, {3,5}, {6,8}         }; // part2 - heel - cosmetic

    // PC1 CAMERAS

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "22676245", "22676264", "22671129" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "22676269", "22671143", "22390018" };

    static wchar_t* part1_names   [3] = { L"P1_A",    L"P1_B",    L"P1_C"    };
    static char*    part1_serials [3] = { "22671121", "22671120", "22671130" };

    static wchar_t* sideck_names  [3] = { L"SC_A",    L"SC_B",    L"SC_C"    };
    static char*    sideck_serials[3] = { "22798552", "22801144", "22798550" };

    // PC2 CAMERAS

    static wchar_t* cosm_names    [3] = { L"CM_A",    L"CM_B",    L"CM_C"    };
    static char*    cosm_serials  [3] = { "22798828", "22798829", "22798817" };
    // Heel
    static wchar_t* topck_names   [3] = { L"HL_A",    L"HL_B",    L"HL_C",   };
    static char*    topck_serials [3] = { "22798822", "22798584", "22798545" };

    static wchar_t* part2_names   [3] = { L"P2_A",    L"P2_B",    L"P2_C"    };
    static char*    part2_serials [3] = { "22676266", "22697706", "22671141" };
      
  #endif // SERIAL_N==201800019





  // TIVS-C40_002.tsm
  // TIVS-C40_001.pro
  #if SERIAL_N==201800015   // New C40 machine
    #define MACHINE_ID      "2018-00-015"
    #define CUSTOMER_NAME   L"天津生物化学制药有限公司" //  
    #define CUSTOMER_NAMEE  L"Tianjin Biochemical"  
    #define CRCAM  0x4DC0 
    #define CRCODE 0x2A21 // 10785
    #define GRIPPER_STATISTICS

    #define OUTPUT_BELT    // if defined output belt management and parameters enabled

//    #define NEW_TRIPLE_TIP
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define FO_COLOR            // If defined camera is searched ad added for flip off color control
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR      // New users/passwords manager

    #define INPUTBELT_SENSOR    // Id defined set input sensor delays


    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC        // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define HGA_LEAKTEST        // HGA components installed - Aggiunto 20170303 per prove in officina su macchina modificata
    #define HGA_DATAREC         // enable option to record HGA data

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define HEEL_INSPECTION       // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[4][2] = { {0,2}, {3,5}, {6,8}, {9,11} }; // cap - 2 bottoms+focolor - part1 - sidecake
    static UINT tlcgroupPC2[3][2] = { {0,2}, {3,5}, {6,8}         }; // part2 - heel - cosmetic


    // PC1 CAMERAS

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    };
    static char*    seal_serials  [3] = { "22647547", "22581623", "22634407" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "22647540", "22647532", "22627219" };

    static wchar_t* part1_names   [3] = { L"P1_A",    L"P1_B",    L"P1_C"    };
    static char*    part1_serials [3] = { "22649361", "22653217", "22581630" };

    static wchar_t* sideck_names  [3] = { L"SC_A",    L"SC_B",    L"SC_C"    };
    static char*    sideck_serials[3] = { "22558534", "22667675", "22547121" };

    // PC2 CAMERAS

    static wchar_t* cosm_names    [3] = { L"CM_A",    L"CM_B",    L"CM_C"    };
    static char*    cosm_serials  [3] = { "22616525", "22547119", "22677534" };
    // Heel
    static wchar_t* topck_names   [3] = { L"HL_A",    L"HL_B",    L"HL_C",   };
    static char*    topck_serials [3] = { "22667681", "22550548", "22668742" };

    static wchar_t* part2_names   [3] = { L"P2_A",    L"P2_B",    L"P2_C"    };
    static char*    part2_serials [3] = { "22649369", "22649377", "22649371" };
      
  #endif // SERIAL_N==201800015


#endif // TIVS_C40




    /*
#define TIVS_C60 // MODEL TIVS-C60 
#define SERIAL_N 201800001   // New development C60 machine
*/
    
// TIVS-C60_001.pro
// TIVS-C60_001.tsm

#ifdef TIVS_C60 // MODEL TIVS-C60

  #define MODEL_NAME        L"TIVS-C60"
  #define VERSION_PC        L"151.001"

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1            10  // number of cameras connected to PC1
  #define NUM_TLC_PC2            16  // number of cameras connected to PC2
  #define NUM_TLC       (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER    (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
  #define APP_RESX             1280  // Horizontal resolution of application window
  #define APP_RESY             1024  // Vertical resolution of application window

  #define LAYOUT_FILENAME L"layout_C60.png"

// TIVS-C60_001.pro - TIVS-C60_001.tsm
  #if SERIAL_N==201800001   // New development C60 machine
    #define MACHINE_ID      "2018-00-001"
    #define CUSTOMER_NAME   L"玉溪沃森生物技术有限公司" //  
    #define CUSTOMER_NAMEE  L"Yuxi Walvax Biotechnology Co. Ltd."  
//    #define CRCAM  0x0D47 // 0D35 
    #define CRCODE 0x1232 // 4658
    #define GRIPPER_STATISTICS

    #define USE_ACA640_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define INPUTBELT_SENSOR // Id defined set input sensor delays
    #define OUTPUT_BELT    // if defined output belt management and parameters enabled

    //#define INPBELT_MANRUN // if defined input  belt can be manually run
    //#define OUTBELT_MANRUN // if defined output belt can be manually run


//    #define NEW_TRIPLE_TIP
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define FO_COLOR            // If defined camera is searched ad added for flip off color control
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

//    #define HGA_LEAKTEST    // HGA components installed - Aggiunto 20170303 per prove in officina su macchina modificata
//    #define HGA_DATAREC         // enable option to record HGA data

    #define NEWCOSMETIC_COMBO


    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define HEEL_INSPECTION       // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,9}           }; // cap - 2 bottoms+focolor - cosmetic
    static UINT tlcgroupPC2[4][2] = { {0,3}, {4,7}, {8,11}, {12,15} }; // sidecake - paricle1 - heel - particle2


    // PC1 CAMERAS

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    }; 
    static char*    seal_serials  [3] = { "21993886", "21993883", "22699682" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "22653236", "22653240", "21786256" };

    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    }; // S1 - COSMETIC LQ/FD
    static char*    cosm_serials  [4] = { "22592272", "22521287", "22592268", "22593488" };


    // PC2 CAMERAS

    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    }; // S2 - SIDECAKE FD
    static char*    sideck_serials[4] = { "22547107", "22628515", "22628512", "22628521" };

    static wchar_t* part1_names   [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    }; // S3 - PARTICLE1 BKLIGHT LQ
    static char*    part1_serials [4] = { "22534457", "22534435", "22376416", "22534456" };

    static wchar_t* topck_names   [4] = { L"HL_A",    L"HL_B",    L"HL_C",    L"HL_D"    }; // S4 - HEEL FD
    static char*    topck_serials [4] = { "22624587", "22624583", "22624581", "22624599" };

    static wchar_t* part2_names   [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    }; // S5 - PARTICLE2 BOTTOM LIGHT LQ
    static char*    part2_serials [4] = { "22376421", "22460689", "22376435", "22534436" };
       
  #endif


//#define SERIAL_N 201800002   // New C60 machine 沃森生物 WALVAX BIOTECHNOLOGY CO LTD (CM0)
// TIVS-C60_001.pro - TIVS-C60_001.tsm
  #if SERIAL_N==201800002   // New C60 machine 沃森生物 WALVAX BIOTECHNOLOGY CO LTD 20190108  (CM0)
    #define MACHINE_ID      "2018-00-002"
    #define CUSTOMER_NAME   L"沃森生物" //  
    #define CUSTOMER_NAMEE  L"WALVAX BIOTECHNOLOGY"  
    #define CRCAM  0x8419 // 
    #define CRCODE 0x15A2 // 5538
    #define GRIPPER_STATISTICS

    #define USE_ACA640_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define OUTPUT_BELT    // if defined output belt management and parameters enabled

//    #define NEW_TRIPLE_TIP
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define FO_COLOR            // If defined camera is searched ad added for flip off color control
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEWCOSMETIC_COMBO   // New cosmetic for combo machines 


    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

//    #define HGA_LEAKTEST    // HGA components installed - Aggiunto 20170303 per prove in officina su macchina modificata
//    #define HGA_DATAREC         // enable option to record HGA data

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define HEEL_INSPECTION       // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

   // #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,9}           }; // cap - 2 bottoms+focolor - cosmetic
    static UINT tlcgroupPC2[4][2] = { {0,3}, {4,7}, {8,11}, {12,15} }; // sidecake - paricle1 - heel - particle2

    // PC1 CAMERAS

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"    }; 
    static char*    seal_serials  [3] = { "22699696", "22699695", "22638827" };

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"    };
    static char*    bottom_serials[3] = { "22653218", "22653175", "22390040" };

    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    }; // S1 - COSMETIC LQ/FD
    static char*    cosm_serials  [4] = { "22667680", "22667656", "22616521", "22616522" };


    // PC2 CAMERAS

    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    }; // S2 - SIDECAKE FD
    static char*    sideck_serials[4] = { "22628505", "22628496", "22612268", "22667667" };

    static wchar_t* part1_names   [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    }; // S3 - PARTICLE1 BKLIGHT LQ
    static char*    part1_serials [4] = { "22534440", "22534415", "22376420", "22376425" };

    static wchar_t* topck_names   [4] = { L"HL_A",    L"HL_B",    L"HL_C",    L"HL_D"    }; // S4 - HEEL FD
    static char*    topck_serials [4] = { "22628499", "22624576", "22628520", "22667671" };

    static wchar_t* part2_names   [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    }; // S5 - PARTICLE2 BOTTOM LIGHT LQ
    static char*    part2_serials [4] = { "22532194", "22389165", "22389146", "22389161" };
       
  #endif





#endif // TIVS_C60




#ifdef TIVS_A60

  #define MODEL_NAME        L"TIVS-A60"
  #define VERSION_PC        L"120.114"

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1       10   // number of cameras connected to PC1 (10 for L60)  Pour marked as a key piont
  #define NUM_TLC_PC2       12  // number of cameras connected to PC2
  #define NUM_TLC      (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER   (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
  #define APP_RESX        1280  // Horizontal resolution of application window
  #define APP_RESY        1024  // Vertical resolution of application window

  #define LAYOUT_FILENAME L"layout_a60.png"

  #define SAVE_IMAGES  // set by default


  // preparato aggiornamento new partic 20170216
  #if SERIAL_N==201400037 // First Reyoung A60 ampoule machine
    #define MACHINE_ID      "2014-00-037"
    #define MACHINE_ID2     "322-052"  // if defined is used/printed in batch report
    #define CUSTOMER_NAME  L"瑞阳制药"  //L"山东瑞阳"  
    #define CRCODE 0xB00A // 45066
    #define CRCAM  0x9AF0 
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define AUTORECONNECT_CAMERA

    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21502511", "21502513", "21502497"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21502509", "21520542", "21517352", "21520553", "21517363", "21520550" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21517369", "21520534", "21517364", "21517354", "21520545", "21520543" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21517365", "21517361", "21520547", "21520544", "21691578", "21520537" }; // cambiata p3e 20160317 su richiesta morris
//    static char*    cosm_serials [6] = { "21517365", "21517361", "21520547", "21520544", "21520533", "21520537" }; 
    
  #endif

  // preparato aggiornamento new partic 20170216
  #if SERIAL_N==201400038 // Second Reyoung A60 ampoule machine
    #define MACHINE_ID      "2014-00-038"
    #define MACHINE_ID2     "322-053"  // if defined is used/printed in batch report
    #define CUSTOMER_NAME  L"瑞阳制药"  //L"山东瑞阳"  
    #define CRCODE 0xB00B // 45067
    #define CRCAM  0x52F3 
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21502508", "21502469", "21502516"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21502473", "21502515", "21502485", "21502495", "21502512", "21502476" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21502514", "21499058", "21502472", "21499059", "21502498", "21502482" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21502484", "21502496", "21502475", "21502486", "21499082", "21499080" };
  #endif

  #if SERIAL_N==201400039 // Third Reyoung A60 ampoule machine
    #define MACHINE_ID      "2014-00-039"
    #define MACHINE_ID2     "322-054"  // if defined is used/printed in batch report
    #define CUSTOMER_NAME  L"瑞阳制药"  //L"山东瑞阳"  
    #define CRCAM  0x381E 
    #define CRCODE 0xB00C // 45068
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21499061", "21502478", "21502503"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21499088", "21502479", "21502483", "21502519", "21502493", "21502470" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21502477", "21520538", "21517349", "21517372", "21520530", "21502487" }; // P2_B changed on 20170227
//    static char*    part2_serials[6] = { "21502477", "21520521", "21517349", "21517372", "21520530", "21502487" };
//    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
//    static char*    part2_serials[6] = { "21502477", "21405147", "21517349", "21517372", "21520530", "21502487" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21502506", "21520529", "21517351", "21520546", "21517366", "21517360" };
  #endif


  #if SERIAL_N==201400041 // Fourth Reyoung A60 ampoule machine
    #define MACHINE_ID      "2014-00-041"
    #define MACHINE_ID2     "321-187"  // if defined is used/printed in batch report
    #define CUSTOMER_NAME  L"瑞阳制药"  //L"山东瑞阳"  
//    #define CUSTOMER_NAME  L"山东瑞阳"  
    #define CRCODE 0xB31A // 45850
    #define CRCAM  0x920E 
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define PARTFLT_H8V4
    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21571178", "21573743", "21521116"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21517358", "21573741", "21571221", "21571217", "21526595", "21571175" }; 
//    static char*    part1_serials[6] = { "21517358", "21573741", "21571221", "21571217", "21526595", "21561175" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21691598", "21627142", "21571216", "21571218", "21499066", "21567200" }; // P2_B sostituita 2016108
//    static char*    part2_serials[6] = { "21691598", "21573216", "21571216", "21571218", "21499066", "21567200" }; // P2_A sostituita 20160808
//    static char*    part2_serials[6] = { "21571185", "21573216", "21571216", "21571218", "21499066", "21567200" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21502468", "21521102", "21520539", "21502488", "21517356", "21521109" };
    
  #endif

/*
#ifdef CIRON_BATCH   // to use ciron custom batch report
#else
#endif
*/


  #if SERIAL_N==201400044 // New A60 machine - India Ciron, program upgaded 2nd tray June 15th - 2014-009vc
    #define MACHINE_ID      "2014-00-044"
    #define CUSTOMER_NAME  L"CIRON" 
	#define CUSTOMER_NAMEE  L"CIRON"//ciron 20190402
    #define CRCAM  0x8A0D 
    #define CRCODE 0xB30A // 45834
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
    #define CIRON_BATCH   // to use ciron custom batch report
//    #define PARTFLT_H6V3
    //#define PARTFLT_H8V4  //Pour removed learning on 20190328
	#define NEW_PARTICLE    //Pour added on 20190328

    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BACK    // Third  station defined for particle with backlight
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21521098", "21521100", "21370116"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21520523", "21520492", "21521118", "21521121", "21520481", "21520504" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21520527", "21517362", "21521099", "21405152", "21517359", "21520548" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21520483", "21520528", "21521111", "21516387", "21520526", "21520536" };
    
  #endif


  #if SERIAL_N==201400045 // Fifth Reyoung A60 ampoule machine
    #define MACHINE_ID      "2014-00-045"
    #define MACHINE_ID2     "321-188"  // if defined is used/printed in batch report
    #define CUSTOMER_NAME  L"瑞阳制药"  //L"山东瑞阳"  
    #define CRCAM  0xF779 
    #define CRCODE 0x1512 // 5394
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW


    #define PARTFLT_H8V4
    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21571219", "21573736", "21573733"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21517371", "21520519", "21520531", "21571212", "21526583", "21517355" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21571222", "21573740", "21573744", "21571220", "21499098", "21567198" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21520484", "21573735", "21571192", "21571213", "21573747", "21573737" };
    
  #endif



  // machine_005_A60.pro - TIVS_A60_v004 (synch)
  #if SERIAL_N==201500001 // New A60 machine - double reject - Kelun - Yueyang - Hunan
    #define MACHINE_ID      "2015-00-001"
    #define CUSTOMER_NAME  L"湖南科伦"
    #define CRCAM  0xA8FA 
    #define CRCODE 0x64A0 // 25760
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
    #define NEW_TIP_V30   // new tip control developed for Kelun

    //#define NEW_PASSW_MNGR  // New users/passwords manager
    #ifdef NEW_PASSW_MNGR  // New users/passwords manager
    #else
    #endif

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - partic3
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21570780", "21567203", "21570777"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21570748", "21571203", "21570788", "21570776", "21570786", "21571207" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21570785", "21571205", "21571214", "21570781", "21571202", "21573738" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21570784", "21570783", "21570750", "21571189", "21570752", "21573742" };
  #endif

  #if SERIAL_N==201500002 // New A60 machine - ex Sanjing - now Jinzhou Aohong - recompiled and sent 20170606 - added multi user and new cosmetic on 20170615
    // <TIVS_A60_v002.tsm>
    // <machine_004_A60.pro>
//    #undef  MODEL_NAME     //   L"TIVS-A60"
//    #define MODEL_NAME        L"TIVS-L60"
    #define MACHINE_ID      "2015-00-002"
    #define CUSTOMER_NAME  L"锦州奥鸿"  // Jinzhou Aohong
    #define CRCAM  0xA50A
    #define CRCODE 0x635F // 25439
    #define GRIPPER_STATISTICS

    #define NEW_PASSW_MNGR  // New users/passwords manager
    #define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE
    #define SAVE_IMAGES
    #define NEW_PARTICLE

    #define NEW_TRIPLE_TIP
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)

//    #define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

//    #define NEWCAP_DOUBLE
//    #define NEWCAP_ORALCAP
//    #define NEWCAP_V21            // add new functions for V2.1 new_cap inspection
//    #define FO_COLOR        // If defined camera is searched ad added for flip off color control
//    #define GRIPPER_STATISTICS
//    #undef  NUM_TLC_PC1    
//    #define NUM_TLC_PC1        10  // number of cameras connected to PC1
    //#define NUM_TLC_PC2        8  // number of cameras connected to PC2
//    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
//    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
    //static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
//    static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,8}, {9,9} }; // tip/cap - partic1 - FO color
    //static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic


    #define NEWCOSMETIC_V20
    //#define NEWCOSMETIC_V30


//    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with bottom light
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
//    #define STN2 PARTICLE_BACK    // Second station defined for particle with bottom light

//    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21763773", "21629450", "21762632"             };
//    static char*    tip_serials  [3] = { "21567191", "21570779", "21570778"             };
//    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
//    static char*    foc_serials  [1] = { "21423947"                                     };

    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21683521", "21681439", "21681496", "21680764", "21680762", "21683543" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21681475", "21683534", "21683542", "21683525", "21683526", "21683539" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "21818391", "21818400", "21922603", "21819759", "21819757", "21923586" };
//    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
//    static char*    cosm_serials [6] = { "21573745", "21570749", "21573739", "21571211", "21573734", "21570751" };
  #endif



  // aggiornato a distanza verso 20161118 a new partic con stesso sw e configurazione di Kelun
  #if SERIAL_N==201500014 // New A60 machine - double reject -  Yunnan Longhai 
    #define MACHINE_ID      "2015-00-014"
    #define CUSTOMER_NAME  L"云南龙海"  
    #define CRCAM  0x629E 
    #define CRCODE 0x33FB // 13307
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
    //#define NEW_TIP_V30   // new tip control developed for Kelun

    //    #define PARTFLT_H6V3
    //#define PARTFLT_H8V4

    //#define WORKER_THREAD
    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL    // Enable filling level on stn1
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21603543", "21605946", "21605966"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21603537", "21605951", "21605986", "21603545", "21603524", "21605962" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21627143", "21605947", "21605983", "21629415", "21603529", "21629470" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21605949", "21680797", "21629418", "21627156", "21629397", "21605965" };
  #endif


  // aggiornato a distanza verso 20161118 a new partic con stesso sw e configurazione di Kelun
  #if SERIAL_N==201500015 // New A60 machine - double reject - Yunnan Longhai 
    #define MACHINE_ID      "2015-00-015"
    #define CUSTOMER_NAME  L"云南龙海"  
    #define CRCAM  0xC2A3 
    #define CRCODE 0x3461 // 13409
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
    //#define NEW_TIP_V30   // new tip control developed for Kelun
//    #define PARTFLT_H6V3

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21567197", "21603541", "21603558"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21605952", "21605980", "21605950", "21603547", "21605981", "21601297" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21603528", "21603555", "21605968", "21601278", "21629442", "21605955" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21605961", "21601276", "21605960", "21627163", "21603549", "21605971" };
  #endif


  // 20161216 agg a new partic
  // TIVS_A60_v004(synch).tsm - machine_005_a60.pro ha problemi zero
  // TIVS_A60_v004(synch).tsm - machine_004_a60 (synch).pro sembra a posto
  // originale machine_004_A60.pro - TIVS_A60_v003.tsm
  #if SERIAL_N==201500016 // New A60 machine - double reject - Apolo - HV Leak test
    #define MACHINE_ID      "2015-00-016"
    #define CUSTOMER_NAME   L"Apolo"  
    #define CUSTOMER_NAMEE  L"Apolo"  
    #define CRCAM  0x7A20 //31264
    #define CRCODE 0x316A // 12650
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-A60L"
    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21520533", "21516390", "21517357"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21680788", "21683532", "21683536", "21681497", "21681477", "21681456" };
	//Pour changed software for cameras replacement 20190131
    //static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    //static char*    part2_serials[6] = { "21683524", "21683537", "21683549", "21680750", "21692678", "21680752" };
	static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22912677", "22912667", "21683549", "21680750", "23418161", "21680752" };//20200820, changed camera P2E from 21692678 to 23418161

    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "21372279", "21370203", "21372303", "21372296", "21370509", "21372285" };
    //static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif


  // 20161216 ricompilato con new particle e da associare all'ultime versione PLC come 
  // TIVS_A60_v004(synch).tsm - machine_005_a60.pro ha problemi zero
  // TIVS_A60_v004(synch).tsm - machine_004_a60 (synch).pro sembra a posto
  #if SERIAL_N==201500017 // New A60 machine - double reject - 吉林敖东 - Jilin Aodong
    #define MACHINE_ID      "2015-00-017"
    #define CUSTOMER_NAME  L"吉林敖东"  
    #define CRCAM  0xBAA4 
    #define CRCODE 0x623A // 25146

    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-A60L"
    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)

    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21603532", "21603542", "21629398"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21681498", "21683523", "21680758", "21680767", "21681471", "21680777" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21683540", "21680749", "21680770", "21680784", "21681450", "21681474" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21680772", "21680765", "21680769", "21680760", "21680794", "21680771" };
  #endif


  // TIVS_A60_v003.tsm / machine_004_A60.pro (originale)
  // Proviamo ad aggiornare 20161213 usando config recente PLC e cambiando solo IVS
  // TIVS_A60_v004(synch).tsm - machine_005_a60.pro ha problemi zero
  // TIVS_A60_v004(synch).tsm - machine_004_a60 (synch).pro sembra a posto
  #if SERIAL_N==201500018 // New A60 machine - double reject Furen Medicine Group Co., Ltd. - prepared 20160511 (era vecchia 2015-00-002 Sanjing)
    #define MACHINE_ID      "2015-00-018"
    #define CUSTOMER_NAME  L"河南辅仁"  
    #define CRCAM  0xD120 
    #define CRCODE 0x4A21 // 18977
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
    #define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21701589", "21701590", "21707448"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21571199", "21571177", "21571194", "21571204", "21571183", "21571208" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21428871", "21428716", "21571200", "21691570", "21571181", "21570782" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21683528", "21680791", "21681453", "21680773", "21680755", "21680798" };
  #endif


  // originale: machine_004_A60.pro - TIVS_A60_v003.tsm
  // 20161215 agg a new partic per prova e poi aggiorn anche sulla prima macchina
  // TIVS_A60_v004(synch).tsm - machine_005_a60.pro ha problemi zero
  // TIVS_A60_v004(synch).tsm - machine_004_a60 (synch).pro sembra a posto
  #if SERIAL_N==201600007   // New A60L - seconda Apolo 20160914 - HV Leak test
    #define MACHINE_ID      "2016-00-007"
    #define CUSTOMER_NAME  L"西安迪赛"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0xF207 //61959  //for 3 cameras of tip
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    #define NEW_TRIPLE_TIP
	#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	//#define NEWCAP_DOUBLE
    //#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    //#define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-A60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

	//Pour added for TAIJI testing 20180908
	//#define NEWCOSMETIC_V30

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	//static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21605988", "21691623", "21692673"             };
	//Pour added for testing of TaiJi 20180913
	//static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    //static char*    foc_serials  [1] = { "22408124"                                     };
	//Pour added for testing of TaiJi 20180913
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };//P1C from 21601269 to 22649378
    static char*    part1_serials[6] = { "21692645", "21601273", "22649378", "21692679", "21692675", "21603531" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21603525", "21605975", "21603516", "21603523", "21605976", "21603521" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21923588", "21923562", "21922142", "21923566", "21923565", "21923560" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "22653237", "22653238", "22653189", "22653176", "22653199", "22653173" };
  #endif
  


  // originale: machine_004_A60.pro - TIVS_A60_v003.tsm
  // nuovo con synchro flashes: machine_005_A60.pro - TIVS_A60_v004 (synch).tsm
  #if SERIAL_N==201600009   // New A60L - nuova 20160918 - test new partic
    #define MACHINE_ID      "2016-00-009"
    #define CUSTOMER_NAME  L"新乡常乐"  // Xinxiang Changle
    #define CRCAM  0x98E0 
    #define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW


    //#define SAVE_IMAGES  // set by default

 /*   #define NEW_PASSW_MNGR  // New users/passwords manager
    #ifdef NEW_PASSW_MNGR  // New users/passwords manager
    #else
    #endif  */

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21603540", "21603520", "21692657"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21603519", "21603546", "21605984", "21603527", "21603538", "21603539" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21603557", "21692667", "21692674", "21691614", "21603553", "21692656" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21683520", "21603517", "21601270", "21601266", "21603563", "21603552" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif

	//Pour changed to Ciron machine 20180206
	  // originale: machine_004_A60.pro - TIVS_A60_v003.tsm
  // nuovo con synchro flashes: machine_005_A60.pro - TIVS_A60_v004 (synch).tsm
  #if SERIAL_N==201700015   // 2017-009Vc
    #define MACHINE_ID      "2017-00-015"
    #define CUSTOMER_NAME  L"CIRON"// Xinxiang Changle//Myfarma//ciron 20180206
	#define CUSTOMER_NAMEE  L"CIRON"// Xinxiang Changle//Myfarma//ciron 20180206
    #define CRCAM  0x98E0 //39136
    #define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	//Pour added 20180207
	#define CIRON_BATCH   // to use ciron custom batch report
	//Pour added 20180207

    //#define SAVE_IMAGES  // set by default

    #define NEW_PASSW_MNGR  // New users/passwords manager
 /*
    #ifdef NEW_PASSW_MNGR  // New users/passwords manager
    #else
    #endif  */

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21603540", "21603520", "21692657"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21603519", "21603546", "21605984", "21603527", "21603538", "21603539" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21603557", "21692667", "21692674", "21691614", "21603553", "21692656" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21683520", "21603517", "21601270", "21601266", "21603563", "21603552" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif
	//Pour changed to Ciron machine 20180206


  // originale: machine_004_A60.pro - TIVS_A60_v003.tsm
  #if SERIAL_N==201600010   // New A60 - nuova iniziata 20160922 
    #define MACHINE_ID      "2016-00-010"
    //#define CUSTOMER_NAME  L"新乡常乐"  // Xinxiang Changle
    #define CRCAM  0x6A23
    #define CRCODE 0x1D7B // 7547
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

    //#define ALLOW_TRACKPRCCHNG   // If defined allows 
    //#define NEW_PARTICLE    // New particle
    //#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21692664", "21692669", "21692665"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21681447", "21683527", "21680779", "21681490", "21680774", "21680778" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21683546", "21629449", "21681467", "21681451", "21680753", "21683548" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21605990", "21692671", "21763781", "21692651", "21696392", "21763784" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif


  //  TIVS_A60_v004 (synch).tsm - machine_004_A60(synch).pro
  #if SERIAL_N==201600011   // New A60 - nuova iniziata 20170119 
    #define MACHINE_ID      "2016-00-011"
    #define CUSTOMER_NAME  L"安徽国瑞"  // China National Medicines Guorui Pharmaceutical Co., Ltd_devices_ffff
    #define CRCAM  0x33D9 
    #define CRCODE 0x4D72 // 19826
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define SAVE_IMAGES  // set by default
    #define NEW_PASSW_MNGR  // New users/passwords manager
 /*   #ifdef NEW_PASSW_MNGR  // New users/passwords manager
    #else
    #endif  */

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21627164", "21629479", "21629464"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21681443", "21627139", "21627141", "21627140", "21683541", "21627154" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21629462", "21683529", "21931937", "21629403", "21629404", "21627144" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21629476", "21763783", "21629469", "21627155", "21603535", "21629471" };
  #endif

	//Pour added 20170726
	  //  TIVS_A60_v004 (synch).tsm - machine_004_A60(synch).pro
  #if SERIAL_N==201600012   // New A60 -  20170726
	//    #undef  MODEL_NAME     //   L"TIVS-A60"
	//    #define MODEL_NAME        L"TIVS-L60"
    #define MACHINE_ID      "2016-00-009"  //should be changed to 2016-00-012
    #define CUSTOMER_NAME  L"草还丹"  // Cao HuanDan_devices_ffff
    #define CRCAM  0xC45F //50271 Pour added 20170726
    #define CRCODE 0x4D72 // 19826
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define SAVE_IMAGES  // set by default
    #define NEW_PASSW_MNGR  // New users/passwords manager
 /*   #ifdef NEW_PASSW_MNGR  // New users/passwords manager
    #else
    #endif  */

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21751955", "21742517", "21751958"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21567196", "21571201", "21571191", "21571176", "21571196", "21567189" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21571197", "21571188", "21571187", "21571184", "21571193", "21571195" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "22161121", "22160367", "22161155", "22161123", "22161131", "22160363" };
  #endif
	//Pour added_the end, Cao HuanDan 20170726


  // CHANGE VERSION FOR RECOMPILED WITH NEW PARTICLE
  #ifdef NEW_PARTICLE
    #undef VERSION_PC   // L"120.114" original
    #define VERSION_PC     L"120.214"
  #endif


#endif // TIVS_A60
	
//Pour added 20171031
// originale: machine_004_A60.pro - TIVS_A60_v003.tsm
// nuovo con synchro flashes: machine_005_A60.pro - TIVS_A60_v004 (synch).tsm
  #if SERIAL_N==201700001   // New A60L - nuova 20160918 - test new partic
    #define MACHINE_ID      "2017-00-001"   //2016-010V
    #define CUSTOMER_NAME  L"TIVS-A60备货"  // Xinxiang Changle
    #define CRCAM  0x1108 //4360 Pour added 20171031 //Pour checked p3f 20180313
//    #define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW


    //#define SAVE_IMAGES  // set by default

 /*   #define NEW_PASSW_MNGR  // New users/passwords manager
    #ifdef NEW_PASSW_MNGR  // New users/passwords manager
    #else
    #endif  */

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21567201", "21629477", "21629465"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22177468", "22177472", "22177500", "22177506", "22180189", "22180206" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22180191", "22177469", "22180196", "22177488", "22177489", "22177470" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "22177490", "22177467", "22177505", "22177495", "22177466", "22177504" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif
	//Pour added 20171031

	//Pour added 20171031
// originale: machine_004_A60.pro - TIVS_A60_v003.tsm
// nuovo con synchro flashes: machine_005_A60.pro - TIVS_A60_v004 (synch).tsm
  #if SERIAL_N==201700002   // 2017-008VC 国药（上海）国际医药
    #define MACHINE_ID      "2017-00-002"
    #define CUSTOMER_NAME  L"TIVS-A60"  // Xinxiang Changle
    #define CRCAM  0xF96A //63875 Pour added 20171031 //changed p3e 20180313
//    #define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW


    //#define SAVE_IMAGES  // set by default

 /*   #define NEW_PASSW_MNGR  // New users/passwords manager
    #ifdef NEW_PASSW_MNGR  // New users/passwords manager
    #else
    #endif  */

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21629455", "21629481", "21629459"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22177482", "22177459", "22177475", "22177456", "22180197", "22177492" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21691618", "21629451", "21627135", "21696367", "21627158", "21405133" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "22177464", "22177494", "22177474", "22177458", "22401392", "22177485" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif
	//Pour added 20171031

		//Pour added 20180102
  #if SERIAL_N==201700006   // 2017-007Vc Myfarma
    #define MACHINE_ID      "2017-00-006"
    #define CUSTOMER_NAME  L"Myfarma" 
    #define CRCAM  0xA8FA //43258 Pour added 20180102
//    #define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW


    //#define SAVE_IMAGES  // set by default

 /*   #define NEW_PASSW_MNGR  // New users/passwords manager
    #ifdef NEW_PASSW_MNGR  // New users/passwords manager
    #else
    #endif  */

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21570780", "21567203", "21570777"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21570748", "21571203", "21570788", "21570776", "21570786", "21571207" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21570785", "21571205", "21571214", "21570781", "21571202", "21573738" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "21570784", "21570783", "21570750", "21571189", "21570752", "21573742" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif
	//Pour added 20180102

			//Pour added 20180920
  #if SERIAL_N==201800012   // 2017-007Vc Myfarma
    #define MACHINE_ID      "2018-00-012"
    #define CUSTOMER_NAME  L"江苏豪森" 
    #define CRCAM  0x2D9B //11675 Pour added 20180920
//    #define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS

	//Pour removed 20180920
    //#define NEW_TRIPLE_TIP

	//Pour added 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW


    //#define SAVE_IMAGES  // set by default

    #define NEW_PASSW_MNGR  // New users/passwords manager


    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8}, {9,9} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22649360", "22647546", "22653235"             };

	//Pour added for testing of TaiJi 20180913
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22627220"                                     };
	//Pour added for testing of TaiJi 20180913

    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22468137", "22376406", "22376438", "22346081", "22346073", "22676270" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22653220", "22653198", "22653178", "22647545", "22653191", "22653172" };
    //static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    //static char*    cosm_serials [6] = { "21570784", "21570783", "21570750", "21571189", "21570752", "21573742" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22668732", "22667679", "22677532", "22667651", "22551358", "22558530" };
  #endif
	//Pour added 20180920

	//Pour added 20190108
  #if SERIAL_N==201800003   // 2018-003V
    #define MACHINE_ID      "2018-00-003"
    #define CUSTOMER_NAME  L"沃森生物"
    #define CRCAM  0xD84A //55370 Pour added 20190108
//    #define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS

	#undef  MODEL_NAME     //   L"TIVS-A60"
    #define MODEL_NAME        L"TIVS-L60"

	//Pour removed 20180920
    //#define NEW_TRIPLE_TIP

	//Pour added 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

    //#define SAVE_IMAGES  // set by default

    #define NEW_PASSW_MNGR  // New users/passwords manager


    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
	#define NEWCOSMETIC_COMBO     //Pour 20200815
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8}, {9,9} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21995333", "21995334", "21992998"             };

	//Pour added for testing of TaiJi 20180913
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22390056"                                     };
	//Pour added for testing of TaiJi 20180913

    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22653203", "22653224", "22653234", "22653204", "22653195", "22653192" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22653213", "22653188", "22653186", "22653193", "22653209", "22653211" };
    //static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    //static char*    cosm_serials [6] = { "21570784", "21570783", "21570750", "21571189", "21570752", "21573742" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "21922142", "21923588", "21923562", "21589525", "21592613", "21594603" };
  #endif
	//Pour added 20190108


//Pour added 20181010
  #if SERIAL_N==201800016   // 2018-006Vc
    #define MACHINE_ID      "2018-00-016/2018-006Vc"
    #define CUSTOMER_NAME  L"Jurabek Laboratories"  //乌兹别克斯坦 
	#define CUSTOMER_NAMEE  L"Jurabek Laboratories" //
    #define CRCAM  0x99AB //39339
    //#define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define SAVE_IMAGES  // set by default

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22671140", "22676265", "22671144"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22653190", "22653228", "22676231", "22653197", "22653206", "22653194" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22581604", "22581628", "22584234", "22534464", "22676222", "22581609" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "22591094", "22581611", "22584224", "22676230", "22584225", "22534425" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif
	//Pour added 20181010

	//Pour added 20181010
  #if SERIAL_N==201800022   // 2018-008Vc
    #define MACHINE_ID      "2018-00-022/2018-008Vc"
    #define CUSTOMER_NAME  L"DENTAFILL PLYUS"  //乌兹别克斯坦 
	#define CUSTOMER_NAMEE  L"DENTAFILL PLYUS" //
    #define CRCAM  0xFB1 //4017
    //#define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define SAVE_IMAGES  // set by default

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21629453", "21404674", "22671150"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22676241", "22676250", "22676227", "22676242", "22676263", "22676267" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22676240", "22676249", "22676251", "22676239", "22676248", "22676246" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "22676235", "22676256", "22676232", "22676224", "22676259", "22676253" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif
	//Pour added 20181010


//Pour added 20181210
  #if SERIAL_N==201800025   // 2017-024V 虎林珍宝
    #define MACHINE_ID      "2018-00-025"
    #define CUSTOMER_NAME  L"虎林珍宝" 
    #define CRCAM  0xEB8D //60301 Pour added 20181210
//    #define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS

	//Pour removed 20180920
    //#define NEW_TRIPLE_TIP

	//Pour added 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

    //#define SAVE_IMAGES  // set by default

    #define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEWCOSMETIC_V30

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8}, {9,9} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22870747", "22870746", "22870750"             };
	//Pour added for testing of TaiJi 20180913
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22401154"                                     };
	//Pour added for testing of TaiJi 20180913
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22671123", "22671110", "22676271", "22676252", "22671124", "22671127" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22671103", "22658598", "22676254", "22671117", "22676260", "22565436" };
    //static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    //static char*    cosm_serials [6] = { "21570784", "21570783", "21570750", "21571189", "21570752", "21573742" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22816588", "22820235", "22828700", "22788223", "22828702", "22820234" };
  #endif
	//Pour added 20181210

		//Pour added 20190128
  #if SERIAL_N==201800026   // 2018-027V
    #define MACHINE_ID      "2018-027V"
    #define CUSTOMER_NAME  L"成都百裕"  //成都百裕
	#define CUSTOMER_NAMEE  L"成都百裕" //
    #define CRCAM  0xC6A7 //50855
    //#define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define SAVE_IMAGES  // set by default

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22649359", "22653222", "22647535"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "21404648", "21428841", "21404685", "21570751", "21573745", "21692677" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "21571211", "21573739", "21404653", "21573734", "21691611", "21404684" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "22676234", "22649364", "22671133", "22581610", "22389169", "22671109" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif
	//Pour added 20190128

	//Pour added 20190703
#if SERIAL_N==201900098   // 2018-027V
#define MACHINE_ID      "2020-006V"
#define CUSTOMER_NAME  L"新乡常乐"  //北京悦康（2019-007V） 
#define CUSTOMER_NAMEE  L"新乡常乐" //
#define CRCAM  0xA856 //43094
//#define CRCODE 0x3D70 // 15728
#define GRIPPER_STATISTICS
#define NEW_TRIPLE_TIP

	// #define ALLOW_TRACKPRCCHNG   // If defined allows 
#define NEW_PARTICLE    // New particle
#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

#define OPENCV_PARTICLE //Pour added on 20190514

	//#define SAVE_IMAGES  // set by default
#define NEW_LOGGER      // uses new log encyrption and digital signature

#define NEW_PASSW_MNGR  // New users/passwords manager

#define STN1_FILLING_LEVEL
#define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

	//    #define PARTFLT_H6V3
#define PARTFLT_H8V4
#define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	static UINT tlcgroupPC1[2][2] = { { 0,2 },{ 3, 8 } }; // tip/cap - partic1
	static UINT tlcgroupPC2[2][2] = { { 0,5 },{ 6,11 } }; // partic2 - cosmetic
	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23032329", "23003372", "23001489" };
	static wchar_t* part1_names[6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F" };
	static char*    part1_serials[6] = { "22534469", "22534478", "22534477", "22697727", "22634408", "22581613" };
	static wchar_t* part2_names[6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F" };
	static char*    part2_serials[6] = { "22534494", "22676268", "22671105", "22671126", "22671115", "22671135" };
	static wchar_t* cosm_names[6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F" };
	static char*    cosm_serials[6] = { "22671131", "22671134", "22581643", "22671116", "22676243", "22676261" };
	//static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
	//static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
#endif
	//Pour added 20190703

	//Pour added 20190719
#if SERIAL_N==202000007   
#define MACHINE_ID      "2020-007V"
#define CUSTOMER_NAME  L"新乡常乐"  //北京悦康（2019-008V） changed to 新乡常乐（2020-007V） in March of 2020
#define CUSTOMER_NAMEE  L"新乡常乐" //
#define CRCAM  0xD4D9 //54489
	//#define CRCODE 0x3D70 // 15728
#define GRIPPER_STATISTICS
#define NEW_TRIPLE_TIP

	// #define ALLOW_TRACKPRCCHNG   // If defined allows 
#define NEW_PARTICLE    // New particle
#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

#define OPENCV_PARTICLE //Pour added on 20190514

	//#define SAVE_IMAGES  // set by default
#define NEW_LOGGER      // uses new log encyrption and digital signature

#define NEW_PASSW_MNGR  // New users/passwords manager

#define STN1_FILLING_LEVEL
#define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

	//    #define PARTFLT_H6V3
#define PARTFLT_H8V4
#define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	static UINT tlcgroupPC1[2][2] = { { 0,2 },{ 3, 8 } }; // tip/cap - partic1
	static UINT tlcgroupPC2[2][2] = { { 0,5 },{ 6,11 } }; // partic2 - cosmetic
	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "22584257", "22584248", "22581603" };
	static wchar_t* part1_names[6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F" };
	static char*    part1_serials[6] = { "22534423", "22971250", "22965534", "22581631", "22376423", "22534416" };
	static wchar_t* part2_names[6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F" };
	static char*    part2_serials[6] = { "22697710", "22584255", "22671108", "22697703", "22591090", "22581606" };
	static wchar_t* cosm_names[6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F" };
	static char*    cosm_serials[6] = { "22584249", "22581627", "22591096", "22581626", "22584260", "22581625" };
	//static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
	//static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
#endif
	//Pour added 20190719

	//Pour added 20190719
#if SERIAL_N==202000006   // 2018-027V  changed to 新乡常乐（2020-006V） in March of 2020
#define MACHINE_ID      "2020-006V"
#define CUSTOMER_NAME  L"新乡常乐"  //北京悦康
#define CUSTOMER_NAMEE  L"新乡常乐" //
#define CRCAM  0x2DC6 //11718
	//#define CRCODE 0x3D70 // 15728
#define GRIPPER_STATISTICS
#define NEW_TRIPLE_TIP

	// #define ALLOW_TRACKPRCCHNG   // If defined allows 
#define NEW_PARTICLE    // New particle
#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

#define OPENCV_PARTICLE //Pour added on 20190514

	//#define SAVE_IMAGES  // set by default
#define NEW_LOGGER      // uses new log encyrption and digital signature

#define NEW_PASSW_MNGR  // New users/passwords manager

#define STN1_FILLING_LEVEL
#define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

	//    #define PARTFLT_H6V3
#define PARTFLT_H8V4
#define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	static UINT tlcgroupPC1[2][2] = { { 0,2 },{ 3, 8 } }; // tip/cap - partic1
	static UINT tlcgroupPC2[2][2] = { { 0,5 },{ 6,11 } }; // partic2 - cosmetic
	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23032346", "23019193", "23019195" };
	static wchar_t* part1_names[6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F" };
	static char*    part1_serials[6] = { "22957342", "22949125", "22949123", "22971258", "22965508", "23019192" };
	static wchar_t* part2_names[6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F" };
	static char*    part2_serials[6] = { "22965501", "23032327", "22949126", "22957325", "22957327", "22949122" };
	static wchar_t* cosm_names[6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F" };
	static char*    cosm_serials[6] = { "23032348", "23032326", "22957332", "23032333", "22965504", "23032332" };
	//static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
	//static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
#endif
	//Pour added 20190719

		//Pour added 20200405
#if SERIAL_N==201900095   // 2019-007V 北京悦康
#define MACHINE_ID      "2019-007V"
#define CUSTOMER_NAME  L"北京悦康"  //北京悦康
#define CUSTOMER_NAMEE  L"北京悦康" //
#define CRCAM  0xC21E //49694
	//#define CRCODE 0x3D70 // 15728
#define GRIPPER_STATISTICS
#define NEW_TRIPLE_TIP

	// #define ALLOW_TRACKPRCCHNG   // If defined allows 
#define NEW_PARTICLE    // New particle
#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

#define OPENCV_PARTICLE //Pour added on 20190514

	//#define SAVE_IMAGES  // set by default
#define NEW_LOGGER      // uses new log encyrption and digital signature

#define NEW_PASSW_MNGR  // New users/passwords manager

#define STN1_FILLING_LEVEL
#define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

	//    #define PARTFLT_H6V3
#define PARTFLT_H8V4
#define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	static UINT tlcgroupPC1[2][2] = { { 0,2 },{ 3, 8 } }; // tip/cap - partic1
	static UINT tlcgroupPC2[2][2] = { { 0,5 },{ 6,11 } }; // partic2 - cosmetic
	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23342112", "23342144", "23144960" };
	static wchar_t* part1_names[6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F" };
	static char*    part1_serials[6] = { "23153105", "23144964", "23136269", "23144945", "23136259", "23144950" };
	static wchar_t* part2_names[6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F" };
	static char*    part2_serials[6] = { "23136632", "23144952", "23051367", "23136270", "23179759", "23144947" };
	static wchar_t* cosm_names[6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F" };
	static char*    cosm_serials[6] = { "23144957", "23144948", "23179761", "23136275", "23269507", "23186625" };
	//static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
	//static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
#endif
	//Pour added 20200405

		//Pour added 20200408
  #if SERIAL_N==201900096   //2019-008V final
    #define MACHINE_ID      "2019-008V"
    #define CUSTOMER_NAME  L"北京悦康" // 2019-008V
	#define CUSTOMER_NAMEE  L"北京悦康"// 2019-008V
    #define CRCAM  0x2112 //8466
    #define CRCODE 0x3D70 // 15728
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    // #define ALLOW_TRACKPRCCHNG   // If defined allows 
    #define NEW_PARTICLE    // New particle
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define OPENCV_PARTICLE //Pour added on 20190514
    //#define SAVE_IMAGES  // set by default

    #define NEW_PASSW_MNGR  // New users/passwords manager
 /*
    #ifdef NEW_PASSW_MNGR  // New users/passwords manager
    #else
    #endif  */

    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22965531", "23051383", "23052025"             };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22965522", "23051361", "22965493", "23051384", "23051994", "23051989" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23034660", "23051380", "22958428", "22965533", "23051990", "23052026" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "23052035", "23052034", "23052027", "23051335", "22957338", "22965500" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
  #endif
	//Pour added 20200408


	//Pour added 20200611
#if SERIAL_N==202000001   
#define MACHINE_ID      "2020-001V" //2020-001V 宁波三生
#define CUSTOMER_NAME  L"宁波三生"  //
#define CUSTOMER_NAMEE  L"宁波三生" //
#define CRCAM  0x8402 //33794
//#define CRCODE 0x3D70 // 15728
#define GRIPPER_STATISTICS
#define NEW_TRIPLE_TIP

	// #define ALLOW_TRACKPRCCHNG   // If defined allows 
#define NEW_PARTICLE    // New particle
#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

#define OPENCV_PARTICLE //Pour added on 20190514

	//#define SAVE_IMAGES  // set by default
#define NEW_LOGGER      // uses new log encyrption and digital signature

#define NEW_PASSW_MNGR  // New users/passwords manager

#define STN1_FILLING_LEVEL
#define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

	//    #define PARTFLT_H6V3
#define PARTFLT_H8V4
#define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	static UINT tlcgroupPC1[2][2] = { { 0,2 },{ 3, 8 } }; // tip/cap - partic1
	static UINT tlcgroupPC2[2][2] = { { 0,5 },{ 6,11 } }; // partic2 - cosmetic
	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23352198", "23352196", "23352191" };
	static wchar_t* part1_names[6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F" };
	static char*    part1_serials[6] = { "23352162", "23144951", "23136261", "22965505", "23329309", "23278869" };
	static wchar_t* part2_names[6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F" };
	static char*    part2_serials[6] = { "23352158", "23352167", "23329297", "23352163", "23352203", "23352138" };
	static wchar_t* cosm_names[6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F" };
	static char*    cosm_serials[6] = { "22965503", "23352165", "23352159", "23352184", "23352134", "23352393" };
	//static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
	//static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
#endif
	//Pour added 20200611


	//Pour added 20200408
#if SERIAL_N==202000005   
#define MACHINE_ID      "2020-005V" //2020-005V 仁合益康
#define CUSTOMER_NAME  L"仁合益康"  //
#define CUSTOMER_NAMEE  L"仁合益康" //
#define CRCAM  0xD0C2 //53442
//#define CRCODE 0x3D70 // 15728
#define GRIPPER_STATISTICS
#define NEW_TRIPLE_TIP
#define INPUTBELT_SENSOR
	// #define ALLOW_TRACKPRCCHNG   // If defined allows 
#define NEW_PARTICLE    // New particle
#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

#define OPENCV_PARTICLE //Pour added on 20190514

	//#define SAVE_IMAGES  // set by default
#define NEW_LOGGER      // uses new log encyrption and digital signature

#define NEW_PASSW_MNGR  // New users/passwords manager

#define STN1_FILLING_LEVEL
#define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

	//    #define PARTFLT_H6V3
#define PARTFLT_H8V4
#define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	static UINT tlcgroupPC1[2][2] = { { 0,2 },{ 3, 8 } }; // tip/cap - partic1
	static UINT tlcgroupPC2[2][2] = { { 0,5 },{ 6,11 } }; // partic2 - cosmetic
	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23136616", "23342145", "23144955" };
	static wchar_t* part1_names[6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F" };
	static char*    part1_serials[6] = { "23342147", "23342156", "23342127", "23342122", "23342110", "23342154" };
	static wchar_t* part2_names[6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F" };
	static char*    part2_serials[6] = { "23342134", "23342148", "23342133", "23342149", "23342157", "23342155" };
	static wchar_t* cosm_names[6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F" };
	static char*    cosm_serials[6] = { "23342115", "23342109", "23342111", "23342150", "23342135", "23342131" };
	//static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
	//static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
#endif
	//Pour added 20200408


		//Pour 20190517
	  #if SERIAL_N==201900005   // New A60L-2018-029V - 20190517 - HV Leak test
    #define MACHINE_ID      "2019-00-005"
    #define CUSTOMER_NAME  L"太极制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0x5FB1 //24497  //for 3 cameras of tip
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

	#define OPACITY_CTRL

	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22914597", "22914657", "22914655"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22967173"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };//P1C from 21601269 to 22649378
    static char*    part1_serials[6] = { "22971256", "22971252", "22949119", "22971243", "22965496", "22965506" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22957333", "22957340", "22965527", "22958435", "22958441", "22965536" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22874685", "22954006", "22946675", "22945591", "22945609", "22945607" };
  #endif


	//Pour 20201030
	#if SERIAL_N==201900006   // New A60L-2018-030V - 20190521 - HV Leak test
    #define MACHINE_ID      "2019-00-006"
    #define CUSTOMER_NAME  L"太极制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0x506A //20586  //for 3 cameras of tip
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

	#define OPACITY_CTRL

	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22915964", "22915943", "22915988"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22967172"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };//P1C from 21601269 to 22649378
    static char*    part1_serials[6] = { "22965537", "22965494", "22965492", "22965490", "22958444", "22965521" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22971239", "22971240", "22965523", "22965509", "22965524", "22965513" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22945601", "22945622", "22944842", "22945577", "22945598", "22944849" };
  #endif
  //Pour 20201030



			//Pour 20190722
	#if SERIAL_N==201900007   // New A60L-2018-031V - 20190521 - HV Leak test
    #define MACHINE_ID      "2019-00-007"
    #define CUSTOMER_NAME  L"太极制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0xF180 //61824  //for 3 cameras of tip
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

	#define OPACITY_CTRL
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22915962", "22915941", "22915965"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22967164"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22971193", "22971226", "22971192", "22971199", "22971198", "22971255" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22971231", "22971230", "22971203", "22971200", "22971207", "22971188" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22953976", "22956785", "22957876", "22957856", "22957882", "22957867" };
  #endif
  //Pour 20190722

	//Pour 20190722
	#if SERIAL_N==201900008   // New A60L-2018-032V - 20190521 - HV Leak test
    #define MACHINE_ID      "2019-00-008"
    #define CUSTOMER_NAME  L"太极制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0xF0E2 //61666  //for 3 cameras of tip
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

	#define OPACITY_CTRL
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22915986", "22915942", "22915961"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22967165"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22971186", "22971201", "22971227", "22971028", "22971213", "22971242" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22971241", "22971246", "22971187", "22971249", "22971248", "22971216" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22945588", "22945585", "22945576", "22949395", "22953988", "22944837" };
  #endif
  //Pour 20190722

	//Pour 20190819 Created, recompiled to include OPENCV and density control on 20201117
	#if SERIAL_N==201900009   // New A60L-2018-033V - 20190819 - HV Leak test
    #define MACHINE_ID      "2019-00-009"
    #define CUSTOMER_NAME  L"太极制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0x13DB //5083  //for 3 cameras of tip
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821
	//#define OPACITY_CTRL
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define OPACITY_CTRL
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22914636", "22914571", "22914569"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22967162"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22971236", "22971184", "23019182", "23019188", "22971247", "23019191" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22971206", "23019194", "23019185", "22971179", "22971222", "23032330" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22957871", "22945592", "22949415", "22945611", "22957898", "22945614" };
  #endif
  //Pour 20190819

	//Pour 20201217
	#if SERIAL_N==201900010   // New A60L-2018-034V - 20190909 - HV Leak test
    #define MACHINE_ID      "2019-00-010"
    #define CUSTOMER_NAME  L"太极制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0x8D4F //36175  //for 4 cameras of cap with color camera
	//#define CRCAM  0x2A6B //10859  //for 4 cameras of tip with BW camera for testing

    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
	//Pour added just for testing
	//#define NEWCAP_ORALCAP //To be removed 
	//Pour added just for testing
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821
	#define OPACITY_CTRL
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22914653", "22914592", "22914570"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22945536"                                     };//23051308 BW camera for testing   22945536 color camera
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22971195", "22971223", "22971209", "22971218", "22971204", "22971234" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22971217", "22971233", "22971235", "22971221", "22971225", "22971027" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22945578", "22949394", "22949408", "22953996", "22953995", "22949403" };
  #endif
  //Pour 20201217

	//Pour 20201217
	#if SERIAL_N==201900015   // New A60L-2018-039V - HV Leak test
    #define MACHINE_ID      "2019-00-015"
    #define CUSTOMER_NAME  L"太极制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0xAFB0 //44976  //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821
	#define OPACITY_CTRL
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23078703", "23078683", "23078705"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22408139"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23032356", "23010514", "23010530", "23010527", "23010517", "23003379" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23034672", "23003371", "23051984", "23010526", "23051987", "23003376" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22957889", "22957875", "22957853", "22945599", "22956776", "22957865" };
  #endif
  //Pour 20201217


	//Pour 20200422
	#if SERIAL_N==201900012   // New L60L-2018-036V - 20200422 - HV Leak test
    #define MACHINE_ID      "2019-00-012"
    #define CUSTOMER_NAME  L"太极制药"  
	#define CRCAM  0x6BEE //27630
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define OPACITY_CTRL
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23222745", "23222746", "23222725"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22945538"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23342153", "23342146", "23153121", "22965530", "22584247", "23342152" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23352189", "23352397", "23352216", "23352115", "23352113", "23352169" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "23283307", "23271567", "23271568", "23269433", "23269449", "23271569" };
  #endif
  //Pour 20200422


	//Pour 20200420
	#if SERIAL_N==201900013   // New A60L-2018-032V - 20190521 - HV Leak test
    #define MACHINE_ID      "2019-00-013"
    #define CUSTOMER_NAME  L"太极制药"  
	#define CRCAM  0x7466 //29798
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 
	#define OPACITY_CTRL
	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23233194", "23222727", "23233202"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22401165"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23342129", "22965535", "23342130", "23342151", "23342136", "23051998" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23352178", "23352179", "23352180", "23352175", "23352209", "23352183" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "23269435", "23291413", "23275137", "23269690", "23269418", "23269458" };
  #endif
  //Pour 20200420


		//Pour 20200506
	#if SERIAL_N==201900014   // New A60L-2018-038V - HV Leak test
    #define MACHINE_ID      "2019-00-014"
    #define CUSTOMER_NAME  L"太极制药"  
	#define CRCAM  0x7BD //1981
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 
	#define OPACITY_CTRL
	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23222747", "23222762", "23233234"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "23164053"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23352392", "23352206", "23352112", "23352114", "23352116", "23352122" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23352117", "23352210", "23352181", "23352185", "23352161", "23352173" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "23271566", "23269432", "23269437", "23269428", "23269431", "23283310" };
  #endif
  //Pour 20200506


	//Pour 20191224
	#if SERIAL_N==201900011   // New A60L-2018-035V - HV Leak test
    #define MACHINE_ID      "2019-00-011"
    #define CUSTOMER_NAME  L"太极制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0xBECD //48845  //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821
	#define OPACITY_CTRL
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23078686", "23078704", "23078682"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "23110669"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23051985", "23051983", "23032335", "23032358", "23052007", "23010523" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23034666", "23052031", "23052009", "23051980", "23051981", "23032353" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22956799", "22960670", "22960672", "22956798", "22957858", "22957881" };
  #endif
  //Pour 20191224


	//Pour 20200522
	#if SERIAL_N==201900016   // New A60L-2018-040V - HV Leak test, 2020-011V TSK to TAIJI
    #define MACHINE_ID      "2019-00-016"
    #define CUSTOMER_NAME  L"太极制药"  
	#define CRCAM  0xD35A //54106
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23213960", "23213959", "23213961"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22401181"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23352139", "23352152", "23352213", "23352142", "23352131", "23352205" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23352126", "23352155", "23352109", "23352118", "23352146", "23352108" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "23269688", "23269414", "23269442", "23291404", "23271570", "23269441" };
  #endif
  //Pour 20200522


	//Pour 20200220
	#if SERIAL_N==202000008   // 201900011 New L60L-2018-035V - 20190909 - HV Leak test //202000008 Transferred to SANJING 2020-008V
    #define MACHINE_ID      "2020-00-008"
    #define CUSTOMER_NAME  L"哈药集团三精制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0x1748 //5960  //for 3 cameras of tip
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60"

	#define TEST_QILU //Pour added for testing on 20200312
	#define NECK_CRACK //Pour added for testing on 20200320
	#define OPACITY_CTRL
    //#define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
    //#define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	//#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	//#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //upgraded to OPENCV on 20200220
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22914594", "22914654", "22914595"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22788608"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22971196", "23032361", "22971224", "22971202", "22971220", "22971229" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22997933", "23001492", "22971194", "23019184", "23019197", "23019200" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22957864", "22945593", "22945590", "22945584", "22957873", "22960679" };
  #endif
  //Pour 20200220

	//Pour 20200220  to be updated to TSK
	#if SERIAL_N==202000011   //201900012 New A60L-2018-036V - 20190909 - HV Leak test //202000011 To be re-compiled to TSK 2020-011V
    #define MACHINE_ID      "2020-00-011"
    #define CUSTOMER_NAME  L"江西天施康"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0x1BE7 //7143  //for 3 cameras of tip  //5960
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22914656", "22914637", "22914572"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "23366112"                                     };//22967170 BW camera
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "22971244", "23032325", "22971254", "22019189", "22971208", "23019199" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "22971257", "22971183", "22971190", "22971245", "22971214", "22971212" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22946680", "22945619", "22953998", "22945606", "22946676", "22944838" };
  #endif
  //Pour 20200220


	//Pour 20200220
	#if SERIAL_N==202000009   // 201900013 New A60L-2018-037V - 20190909 - HV Leak test  //202000009 Transferred to SANJING 2020-009V
    #define MACHINE_ID      "2020-00-009"
    #define CUSTOMER_NAME  L"哈药集团三精制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0x583D //22589  //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60"

    //#define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
    //#define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	//#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	//#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //upgraded to OPENCV on 20200220
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22914596", "22914589", "22914590"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22967160"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23001487", "23010516", "23032334", "22997932", "23010522", "23001490" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23001488", "23010519", "23003378", "23052016", "23010518", "23032357" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22960666", "22960674", "22960682", "22957900", "22957888", "22957883" };
  #endif
  //Pour 20200220

	//Pour 20200220
	#if SERIAL_N==202000010   //201900014 New A60L-2018-038V - 20190909 - HV Leak test //202000010 Transferred to SANJING 2020-010V
    #define MACHINE_ID      "2020-00-010"
    #define CUSTOMER_NAME  L"哈药集团三精制药"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0xA407 //41991  //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913
	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60"

    //#define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
    //#define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	//#define HV_DATAREC
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	//#define THIRD_REJET //Pour added for TAIJI HV rejection
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //upgraded to OPENCV on 20200220
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23062986", "23062971", "23062989"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22408138"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23052011", "23032345", "23032360", "23051982", "23052010", "23032362" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23003377", "23032354", "23003380", "23009102", "23051986", "23052008" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "22954012", "22960681", "22957884", "22957872", "22956777", "22957869" };
  #endif
  //Pour 20200220


			//Pour 20191205
	#if SERIAL_N==201900027   // New L60-2019-xxxV
    #define MACHINE_ID      "2019-00-027"
    #define CUSTOMER_NAME  L"长春金派格"  
	#define CRCAM  0xF7B3 //63411
	#define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913

	#define TEST_QILU //Pour added for testing on 20200312
	#define NECK_CRACK //Pour added for testing on 20200320


	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

	//#define TEST_QILU //Pour added for testing on 20200312

    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60"

//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	//#define OUTPUT_BELT      // if defined output belt management and parameters enabled
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	//Pour added for TAIJI testing 20180908
	#define NEWCOSMETIC_V30
	//#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23086118", "23126743", "23126715"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "23164050"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23051336", "23051340", "23136626", "23136258", "23120673", "23051343" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23051342", "23051300", "23120659", "23136630", "23120685", "23051334" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "23040099", "22970207", "23122370", "23118144", "22967113", "21420098" };
  #endif
  //Pour 20191205

		//Pour 20200404
	  #if SERIAL_N==201900020   // New A60L -2019-016V  HV Leak test
    #define MACHINE_ID      "2019-00-020"
    #define CUSTOMER_NAME  L"锦州奥鸿"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0x5070 //20592  //for 3 cameras of tip
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    #define NEW_TRIPLE_TIP
	#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//#define THIRD_REJET //Pour added for HV rejection
	//Pour added for Taiji testing 20180913
	//#define NEWCAP_DOUBLE
    //#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    //#define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-A60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //Pour added on 20190514
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define NEW_PASSW_MNGR  // New users/passwords manager //Pour added for FAT modification on 20191021
	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE // log alarms separately and add button to show them

	//Pour added for TAIJI testing 20180908
	//#define NEWCOSMETIC_V30

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	//static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23051321", "23051317", "23051366"             };
	//Pour added for testing of TaiJi 20180913
	//static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    //static char*    foc_serials  [1] = { "22408124"                                     };
	//Pour added for testing of TaiJi 20180913
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };//P1C from 21601269 to 22649378
    static char*    part1_serials[6] = { "23051290", "23120661", "23120683", "23120657", "23120667", "23120676" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23120662", "23120686", "23052003", "23120684", "23120679", "23051382" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "21923588", "21923562", "21922142", "21923566", "21923565", "21923560" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "23051373", "23052023", "23052000", "23051286", "23051374", "23120660" };
  #endif
	//Pour 20200404


	//Pour 20200828
	#if SERIAL_N==202000021   // New A60L -2019-021V  HV Leak test
    #define MACHINE_ID      "2020-00-021"
    #define CUSTOMER_NAME  L"兰州生物制品研究所"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0x1CE4 //7396  //for 3 cameras of tip
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    #define NEW_TRIPLE_TIP
	#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//#define THIRD_REJET //Pour added for HV rejection
	//Pour added for Taiji testing 20180913
	//#define NEWCAP_DOUBLE
    //#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    //#define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-A60L"

    #define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	#define HV_DATAREC
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //Pour added on 20190514
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define NEW_PASSW_MNGR  // New users/passwords manager //Pour added for FAT modification on 20191021
	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE // log alarms separately and add button to show them

	//Pour added for TAIJI testing 20180908
	//#define NEWCOSMETIC_V30

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	//static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23416222", "23416225", "23416224"             };
	//Pour added for testing of TaiJi 20180913
	//static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    //static char*    foc_serials  [1] = { "22408124"                                     };
	//Pour added for testing of TaiJi 20180913
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };//P1C from 21601269 to 22649378
    static char*    part1_serials[6] = { "23352403", "23370942", "23370890", "23352402", "23370900", "23416212" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23394322", "23394327", "23394318", "23394320", "23416220", "23416227" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "23423570", "23402034", "23423573", "23402047", "23403837", "23403834" };
    //static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    //static char*    cosm_serials [6] = { "23051373", "23052023", "23052000", "23051286", "23051374", "23120660" };
  #endif
	//Pour 20200828


		//Pour 20200913
	#if SERIAL_N==202000022   // New L60-2020-022V
    #define MACHINE_ID      "2020-00-022"
    #define CUSTOMER_NAME  L"北京科兴中维"  
	#define CRCAM  0x9363 //37731
	#define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913

	#define TEST_QILU //Pour added for testing on 20200312
	#define NECK_CRACK //Pour added for testing on 20200320

	#define OPACITY_CTRL    // If defined enables opacity control for Yuxi Zerun

	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

	//#define TEST_QILU //Pour added for testing on 20200312

    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60"

//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	//#define OUTPUT_BELT      // if defined output belt management and parameters enabled
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	#define THIRD_REJET
	//Pour added for TAIJI testing 20180908
	//#define NEWCOSMETIC_V30
	#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
	#define STN2_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23436501", "23437225", "23437226"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "23366103"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23416210", "23370898", "23370904", "23370946", "23416223", "23352400" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23376249", "23418159", "23383777", "23352405", "23394331", "23370888" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "23368599", "23393380", "23355039", "23393383", "23393382", "23388062" };
  #endif
  //Pour 20200913



	//Pour 20201208
	#if SERIAL_N==202000028   // New L60-2020-028V
    #define MACHINE_ID      "2020-00-028"
    #define CUSTOMER_NAME  L"北京科兴中维"  
	#define CRCAM  0xDCE2 //56546
	#define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    //#define NEW_TRIPLE_TIP
	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//Pour added for Taiji testing 20180913

	#define TEST_QILU //Pour added for testing on 20200312
	#define NECK_CRACK //Pour added for testing on 20200320

	#define OPACITY_CTRL    // If defined enables opacity control for Yuxi Zerun

	#define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

	//#define TEST_QILU //Pour added for testing on 20200312

    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-L60"

//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	//#define OUTPUT_BELT      // if defined output belt management and parameters enabled
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define DOUBLE_REJET
	#define THIRD_REJET
	//Pour added for TAIJI testing 20180908
	//#define NEWCOSMETIC_V30
	#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    // #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23580593", "23588263", "23602275"             };
	static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "23366107"                                     };
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };
    static char*    part1_serials[6] = { "23464351", "23464347", "23607187", "23607193", "23362880", "23464369" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23607192", "23464385", "23416215", "23464388", "23464343", "23464371" };
    static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    static char*    cosm_serials [6] = { "23573240", "23613714", "23573244", "23613706", "23613709", "23613711" };
  #endif
  //Pour 20201208

#if SERIAL_N==202000090   // New L60-2020-028V
#define MACHINE_ID      "2020-00-028"
#define CUSTOMER_NAME  L"大连科兴"  
//#define CRCAM  0xDCE2 //56546
#define CRCODE 0x316D // 12653
#define GRIPPER_STATISTICS
//Pour removed 20180913
//#define NEW_TRIPLE_TIP
//#define INPUTBELT_SENSOR // Id defined set input sensor delays
//Pour added for Taiji testing 20180913

#define TEST_QILU //Pour added for testing on 20200312
#define NECK_CRACK //Pour added for testing on 20200320

#define OPACITY_CTRL    // If defined enables opacity control for Yuxi Zerun

#define NEWCAP_DOUBLE
#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
#define FO_COLOR         // If defined camera is searched ad added for flip off color control
#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

//#define TEST_QILU //Pour added for testing on 20200312

#undef  MODEL_NAME    
#define MODEL_NAME      L"TIVS-L60"

//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

#define NEW_PASSW_MNGR  // New users/passwords manager
#define NEW_LOGGER
#define ALARMLOG_SEPARATE
//#define OUTPUT_BELT      // if defined output belt management and parameters enabled
#define SAVE_IMAGES
#define NEW_PARTICLE
#define OPENCV_PARTICLE
#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
#define DOUBLE_REJET
#define THIRD_REJET
//Pour added for TAIJI testing 20180908
//#define NEWCOSMETIC_V30
#define NEWCOSMETIC_COMBO//Pour added on 20190826
//    #define PARTFLT_H6V3
#define PARTFLT_H8V4
// #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
#define STN1_FILLING_LEVEL
#define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
//#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

	//static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
	//static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
	static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char* tip_serials[3] = { "23645205", "23645223", "23645206" };
	static wchar_t* foc_names[1] = { L"TC_D" };
	static char* foc_serials[1] = { "23587659" };
	static wchar_t* part1_names[6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F" };
	static char* part1_serials[6] = { "23656926", "23656909", "23592995", "23656914", "23656922", "23696927" };
	static wchar_t* part2_names[6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F" };
	static char* part2_serials[6] = { "23656905", "23639004", "23656933", "23639019", "23592959", "23656908" };
	static wchar_t* cosm_names[6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F" };
	static char* cosm_serials[6] = { "23621365", "23572137", "23621377", "23580086", "23572114", "23572144" };
#endif

	//Pour 20201207
	#if SERIAL_N==202000109   // New A60 -2020-009Vc
    #define MACHINE_ID      "2020-00-109"
    #define CUSTOMER_NAME  L"UZBEKISTAN-Dentafill LLC"  
    //#define CRCAM  0xF3D0 //for 3 cameras of tip
	#define CRCAM  0x5E8B //24203  //for 3 cameras of tip
	//#define CRCAM  0xDF85 //for 4 cameras of tip
    #define CRCODE 0x316D // 12653
    #define GRIPPER_STATISTICS
	//Pour removed 20180913
    #define NEW_TRIPLE_TIP
	#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//#define THIRD_REJET //Pour added for HV rejection
	//Pour added for Taiji testing 20180913
	//#define NEWCAP_DOUBLE
    //#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    //#define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #undef  MODEL_NAME    
    #define MODEL_NAME      L"TIVS-A60L"

    //#define HV_LEAKTEST          // If defined enables parameters and functions for HV leak test unit
//    #define ALLOW_TRACKPRCCHNG   // If defined allows 

	//Pour added for HV detection 20180821
	//#define HV_DATAREC
	//Pour added for HV detection 20180821

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //Pour added on 20190514
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	#define NEW_PASSW_MNGR  // New users/passwords manager //Pour added for FAT modification on 20191021
	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE // log alarms separately and add button to show them

	//Pour added for TAIJI testing 20180908
	//#define NEWCOSMETIC_V30

//    #define PARTFLT_H6V3
    #define PARTFLT_H8V4
    #define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
    #define STN1_FILLING_LEVEL
    #define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bar light//added for TAIJI testing
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3, 8} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

	//Pour added for TAIJI testing 20180913
	//static UINT tlcgroupPC1[3][2] = { {0,2}, {3, 8},{9, 9} }; // tip/cap - partic1-FO color
    //static UINT tlcgroupPC2[2][2] = { {0,5}, {6,11} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23585187", "23607191", "23583808"             };
	//Pour added for testing of TaiJi 20180913
	//static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    //static char*    foc_serials  [1] = { "22408124"                                     };
	//Pour added for testing of TaiJi 20180913
    static wchar_t* part1_names  [6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F"    };//P1C from 21601269 to 22649378
    static char*    part1_serials[6] = { "23593000", "23593007", "23418261", "23593005", "23418260", "23418252" };
    static wchar_t* part2_names  [6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F"    };
    static char*    part2_serials[6] = { "23585188", "23607183", "23592999", "23592997", "23593006", "23583809" };
    //static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
    //static char*    cosm_serials [6] = { "23423570", "23402034", "23423573", "23402047", "23403837", "23403834" };
    static wchar_t* cosm_names   [6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F"    };
    static char*    cosm_serials [6] = { "23583807", "23464376", "23607185", "23593004", "23596987", "23583818" };
  #endif
	//Pour 20201207



	//Pour changed to Ciron machine 20190827
#if SERIAL_N==201900017   // 2019-002Vc//changed to 201900017
#define MACHINE_ID      "2019-00-016"
#define CUSTOMER_NAME  L"CIRON"// Xinxiang Changle//Myfarma//ciron 20180206
#define CUSTOMER_NAMEE  L"CIRON"// Xinxiang Changle//Myfarma//ciron 20180206
#define CRCAM  0x9836 //38966
#define CRCODE 0x3D70 // 15728
#define GRIPPER_STATISTICS
#define NEW_TRIPLE_TIP

	// #define ALLOW_TRACKPRCCHNG   // If defined allows 

#define OPENCV_PARTICLE //Pour added on 20190514

#define NEW_PARTICLE    // New particle
#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
	//Pour added 20180207
#define CIRON_BATCH   // to use ciron custom batch report
	//Pour added 20180207

	//#define SAVE_IMAGES  // set by default

#define NEW_PASSW_MNGR  // New users/passwords manager
	/*
	#ifdef NEW_PASSW_MNGR  // New users/passwords manager
	#else
	#endif  */

#define STN1_FILLING_LEVEL
#define FILLING_BOTTOMCOMP    // Enable compensation of bottom of containers for filling level calculation BACKLIGHT ONLY!

	//    #define PARTFLT_H6V3
#define PARTFLT_H8V4
#define DOUBLE_REJET // When defined double reject on output is enabled (also in PLC SW)
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for particle with bottom light
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	static UINT tlcgroupPC1[2][2] = { { 0,2 },{ 3, 8 } }; // tip/cap - partic1
	static UINT tlcgroupPC2[2][2] = { { 0,5 },{ 6,11 } }; // partic2 - cosmetic
	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23032349", "22971232", "22957326" };
	static wchar_t* part1_names[6] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D",    L"P1_E",    L"P1_F" };
	static char*    part1_serials[6] = { "22957344", "22957329", "22958429", "23019198", "23032328", "22958437" };
	static wchar_t* part2_names[6] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D",    L"P2_E",    L"P2_F" };
	static char*    part2_serials[6] = { "22949117", "22965525", "22965538", "23019196", "22965526", "23032351" };
	static wchar_t* cosm_names[6] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D",    L"P3_E",    L"P3_F" };
	static char*    cosm_serials[6] = { "23052029", "22949130", "22957328", "22957343", "22965495", "22957336" };
	//static wchar_t* cosm_names   [6] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D",    L"CM_E",    L"CM_F"    };
	//static char*    cosm_serials [6] = { "21680790", "21680759", "21680775", "21680768", "21683538", "21681442" };
#endif
	//Pour changed to Ciron machine 20190827



#ifdef TIVS_A40

  #define MODEL_NAME        L"TIVS-A40"
  #define VERSION_PC        L"120.114"

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1        7  // number of cameras connected to PC1
  #define NUM_TLC_PC2        8  // number of cameras connected to PC2
  #define NUM_TLC      (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
  #define NUMTRIGGER   (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
  #define APP_RESX        1280  // Horizontal resolution of application window
  #define APP_RESY        1024  // Vertical resolution of application window

  #define STN1_FILLING_LEVEL    // add filling level to STN1 by default

  #define LAYOUT_FILENAME L"layout_lq.png"


  #if SERIAL_N==2013001 
    #define MACHINE_ID      "2013-001VB"
//    #define CUSTOMER_NAME  L"山东瑞阳"  
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21371036", "21371044", "21371055"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21371086", "21371085", "21371062", "21371038" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21371075", "21371037", "21369681", "21369691" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21372305", "21368176", "21370501", "21370507" };
  #endif



//  #if SERIAL_N==2013003 // Ampoule machine - Replacement sent to Reyoung
  #if SERIAL_N==201400002 //2013003 "2013-003VB" - Ampoule machine - replacement sent to Reyoung - back to ws recompiled and updated 20151204
    // <machine_004 (2nd feedback and lights fault).pro>
    // <TIVS_LQ_004 (2nd feedback and lights fault and LAM faults).tsm>

    #define MACHINE_ID      "2014-00-002" // old "2013-003VB"
//    #define CUSTOMER_NAME  L"山东瑞阳"  
    #define CRCAM  0x405A // F0ED 
    #define CRCODE 0x3C30 // 15408
    //#define PARTFLT_H6V3
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP // compensation of bottom height in backlight images
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21371034", "21364281", "21371035"             };
// old    static char*    tip_serials  [3] = { "21371034", "21364281", "21371035"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21371030", "21521113", "21371026", "21370104" };
// old    static char*    part1_serials[4] = { "21370112", "21371020", "21369682", "21371070" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21570767", "21371020", "21369682", "21371070" };
// old    static char*    part2_serials[4] = { "21371030", "21370105", "21371026", "21370104" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21404668", "21404644", "21404662", "21405176" };
// old    static char*    cosm_serials [4] = { "21404668", "21404644", "21404662", "21405176" };
  #endif


  #if SERIAL_N==201400003 // OLD 2013004 Shanxxi Pude - Special filter for vertical cracks in tip inspe
    #define MACHINE_ID     "2014-00-003" // OLD "2013-004VB"
    #define CUSTOMER_NAME L"山西普德"
    #define CRCAM  0x0CFD 
    #define CRCODE 0x318B // 12683
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
    #define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21370094", "21371088", "21371056"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21371023", "21370111", "21371029", "21371019" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21370109", "21369692", "21371081", "21370106" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21428737", "21428827", "21499078", "21428888" };
  #endif


  #if SERIAL_N==201300001 // OLD 201300501 2013-005V - first ampoule prototype LIOCO HAIKOU (old machine, replaced october 2014)
    #define MACHINE_ID     "2013-00-001" // old 2013-005V
//    #define CUSTOMER_NAME L"山西普德"
    #define CRCAM  0xD14F // old 5F6F 
    #define CRCODE 0x4270 // 17008
    #define NEW_TRIPLE_TIP

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    //#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    #define GRIPPER_STATISTICS

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21321495", "21322510", "21321484"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21321482", "21321486", "21321490", "21322511" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21629446", "21629405", "21680786", "21629421" };
//    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21321488", "21321485", "21321487", "21293367" };
  #endif

  #if SERIAL_N==201400004 // old 201300502 HENG RUI LIANYUNGANG- SW updated 20140415 includes FO color! - sent to ChangChun ChangSheng
    // <machine_004 (2nd feedback and lights fault).pro>
    // <TIVS_LQ_004 (2nd feedback and lights fault and LAM faults).tsm>
    // macchina speciale - P1 bottom - P2 backlight - P3 cosmetic 
    // liquid vials + FO camera - filling on p2 + bottom alignment
    // venuto con pour 20141218 per inserire e testare allineamento livello fondo
    // tornato con morris 20160302 aggiornato tutto per verificare e risolvere il problema
    // di mancato allineamento scarti
    #define MACHINE_ID     "2014-00-004" // old "2013-005VB"
    #define CUSTOMER_NAME L"长春长生"

    #undef  MODEL_NAME     //   L"TIVS-A40"
    #define MODEL_NAME        L"TIVS-L40"

    #define CRCAM  0xA4F4 
    #define CRCODE 0xFA41 // 64065
    #define GRIPPER_STATISTICS
    #define FO_COLOR              // Flip off color control
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21            // add new functions for V2.1 new_cap inspection
    //#define FILLING_BOTTOMCOMP    // filling level with bottom height compensation
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
//    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN2 PARTICLE_BACK    // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK    // Third  station defined for cosmetic with back light
//    #define STN3 PARTICLE_BACK    // Third  station defined for particle with back light
//    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define PARTFLT_H6V3          // More dilation because of higher magnification
    #undef  STN1_FILLING_LEVEL    // remove filling level to STN1 defined by default
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC      (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER   (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21370099", "21369684", "21371024"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "21407696"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21502501", "21499070", "21371021", "21370101" };
//    static char*    part1_serials[4] = { "21369695", "21499070", "21371021", "21370101" };
//    static char*    part1_serials[4] = { "21369695", "21371082", "21371021", "21370101" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21370110", "21363243", "21364269", "21369685" };
//    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
//    static char*    cosm_serials [4] = { "21491218", "21499079", "21499095", "21499067" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21368172", "21370575", "21372307", "21372289" };
//    static char*    cosm_serials [4] = { "21405187", "21400223", "21404716", "21405196" };
  #endif

  #if SERIAL_N==201400005 // old 2013006 lianyungang ampoule machine factory - returned updated to be sent to pakistan 20150714
    #define MACHINE_ID     "2014-00-005" // old "2013-006VB"
    #define CUSTOMER_NAME L"INDUS"

    #define CRCAM  0xED13 
    #define CRCODE 0x379D // 14237

    #define STN1 PARTICLE_BOTH    // First  station defined for selectable particle
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    #define FILLING_BOTTOMCOMP // compensation of bottom height in backlight images

    #define NEW_TRIPLE_TIP
    #define GRIPPER_STATISTICS

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21369686", "21371080", "21371027"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21371065", "21363244", "21370089", "21371051" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21371048", "21371040", "21371039", "21370120" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21397063", "21404641", "21404669", "21404658" };
//    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
//    static char*    cosm_serials [4] = { "21394075", "21405190", "21405181", "21404321" };
  #endif

  

// ex Luye aggiornata 20170316 a nuovo sw e configurazione standard
// da confermare

// TIVSL40_004 (2nd feedback and newpartic and outputbelt).tsm
// machine_004 (2nd feedback and newpartic).pro
#if SERIAL_N==201400012 // TOT Biopharma 20170619 OLD 201300701 Ampoule machine factory - Second feedback cap + FO color + 3 bottom particles + input/output belts

    // usata x test Kunming, rotaz tlc e immagine ingrandita

    // <machine_004 (2nd feedback and lights fault).pro>
    // <TIVS_L40_005 (2nd feedback and lights fault and output belt and LAM faults).tsm>
    #define MACHINE_ID     "2014-00-012" // OLD "2013-007V" LUYE LIQUID VIALS
    #define CUSTOMER_NAME  L"东曜药业"          // 东曜药业有限公司 TOT Biopharma Co., Ltd  - old L"山东绿叶" // Luye Pharma
    #define CUSTOMER_NAMEE L"TOT Biopharma"    // 东曜药业有限公司 TOT Biopharma Co., Ltd  - old L"山东绿叶" // Luye Pharma

    #undef  MODEL_NAME     //   L"TIVS-A40"
    #define MODEL_NAME        L"TIVS-L40"
  
    #define NEW_LOGGER      // uses new log encyrption and digital signature
    #define NEW_PASSW_MNGR  // New users/passwords manager


    #define INPUTBELT_SENSOR // Id defined set input sensor delays
    #define OUTPUT_BELT      // if defined output belt management and parameters enabled

    #define CRCAM  0x0343 // oldc820B 
    #define CRCODE 0x5105 // 20741
    #define GRIPPER_STATISTICS
  // old  #define INPUTBELT_SENSOR // Id defined set input sensor delays
  // old  #define FALLEN_CONTAINER // If defined, use code to check fallen containers/set delay/enable alarm
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
    
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21            // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
    #define READ_CYCLES     // if defined enables reading of cycles from PLC
	#define OPENCV_PARTICLE //Pour added on 20190613
//    #define STN1        PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1        PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP           // filling level with bottom height compensation
  // old  #define STN1_OR     HORIZONTAL       // if defined cameras are setup horizonthally

    #define STN2        PARTICLE_BOTTOM  // Second station defined for particle with bottom light
  // old  #define STN2_OR     HORIZONTAL       // if defined cameras are setup horizonthally

   // #define STN3        COSMETIC_BACK    // Third  station defined for cosmetic with back light
    #define STN3        PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static  BYTE        PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    /*
    #define DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    #undef  FO_COLOR         // If defined camera is searched ad added for flip off color control
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        9  // number of cameras connected to PC1
    static wchar_t* dtrng_names  [2] = { L"DR_A",    L"DR_B"                            };
    static char*    dtrng_serials[2] = { "21418146", "21422339"                         };
    */

    #undef  NUM_TLC
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use


    //static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,8} }; // tip/cap - partic1 - DOT/RING colorf
    static UINT         tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21405160", "21405144", "21404647"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "21418146"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21691581", "21691575", "21691613", "21691585" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21763792", "21767264", "21762650", "21763818" };
//    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
//    static char*    cosm_serials [4] = { "21593609", "21426569", "21370197", "21420094" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21404654", "21404686", "21405157", "21404677" };


/*  original  static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21405160", "21404647", "21405144"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "21418146"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21404654", "21405157", "21404677", "21404686" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21404655", "21405138", "21405159", "21404645" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21426569", "21420094", "21593609", "21370197" };  */

//    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
//    static char*    cosm_serials [4] = { "21404684", "21404674", "21404653", "21404685" };
  #endif




  // TIVSA40_LQ_004 (2nd feedback and newpartic).tsm
  // machine_004 (2nd feedback and newpartic).pro
  #if SERIAL_N==201400006 // OLD 201300702 "2013-007VB" - Wuhan - Humanwell Healthcare - 3 bottom particle - update new partic 20170613
    #define MACHINE_ID     "2014-00-006" // OLD "2013-007VB"
    #define CUSTOMER_NAME L"人福药业"
    #define CRCAM  0x050C 
    #define CRCODE 0x7A22 // 31266

    #define NEW_TRIPLE_TIP
    #define GRIPPER_STATISTICS

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21371084", "21369693", "21371087"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21369697", "21369687", "21371069", "21371076" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21371042", "21371063", "21370092", "21371059" };
    //static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21405141", "21404681", "21405139", "21405145" };
  #endif

/* OLD CHANGED TO NITIN 2nd
  #if SERIAL_N==2013020 // Ampoule machine factory - SW installed 20131222
    #define MACHINE_ID  "2013-020V"
    #define CRCODE 0x3F31 // 16177
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21428854", "21428837", "21428847"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21428839", "21428745", "21428711", "21428855" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21428742", "21428842", "21428836", "21428832" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21428740", "21428833", "21428834", "21371050" };
  #endif
  */

  // double feedback 
  // aggiornata 20161202 a nuovo particellare, da provare e confermare

  // TIVSA40_LQ_004 (2nd feedback and newpartic).tsm
  // machine_004 (2nd feedback and newpartic).pro
  #if SERIAL_N==201400029 // OLD "2013-023V" 2013023 New Ampoule machine - for testing triple tip and LAM
    #define MACHINE_ID     "2014-00-029" // OLD 2013-023V
    #define CUSTOMER_NAME L"河北智同"    // Hebei Zhitong
    #define CRCAM  0x3F51 
    #define CRCODE 0x5123 // 20771
    #define GRIPPER_STATISTICS
    #define NEW_TRIPLE_TIP
    #define TIP_THICKNESS

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BOTTOM  // First station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21428890", "21428856", "21428748"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21428710", "21428731", "21428729", "21428846" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21405153", "21405132", "21405171", "21405151" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21404642", "21371047", "21404661", "21404643" };
  #endif


  // Updated 20170502 to new particle

  #if SERIAL_N==201300002 // OLD 2013029 Shangdong Reyong 2013-00-002 updated 20150629, to be sent to Haisco
    #define MACHINE_ID     "2013-00-002" // "2013-029V"
    #define CUSTOMER_NAME L"海思科" // Haisco
    #define CRCAM  0xD68E 
    #define CRCODE 0x38DB // 14555
    #define NEW_TRIPLE_TIP

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21371083", "21371054", "21364268"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21363242", "21371049", "21369696", "21371077" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21371025", "21364279", "21370118", "21369699" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21397069", "21405170", "21405165", "21397046" };
//    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
//    static char*    cosm_serials [4] = { "21368172", "21370575", "21372307", "21372289" };
  #endif


  // preparato aggiornamento a nuovo partic 20170216
  // TIVSA40_LQ_004 (2nd feedback and newpartic) 2014-00-001.tsm
  // machine_004 (2nd feedback and newpartic).pro

  #if SERIAL_N==201400001 // 2014002 Ampoule machine - Ideal Hightech Shijiazhuang Hebei - WARNING: REVERSED ENCODER!
    #define MACHINE_ID     "2014-00-001" // OLD "2014-002V"
    #define CUSTOMER_NAME L"河北爱尔海泰"
    #define CRCAM  0x14B7 
    #define CRCODE 0x16FA // 5882  (tolto per evitare problema persistent 20140702)

    #define GRIPPER_STATISTICS
    #define SAVE_INPUTDATA

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BOTTOM  // First station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21371036", "21371044", "21371055"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21371086", "21371085", "21371062", "21371038" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21371075", "21371037", "21369681", "21369691" };
	static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21405163", "22534422", "21404650", "21405167" };
	//Pour changed camera for P3_B from 21405982 to 22534422
    //static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    //static char*    cosm_serials [4] = { "21405163", "21405982", "21404650", "21405167" };

/*    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21371034", "21364281", "21371035"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21370112", "21371020", "21369682", "21371070" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21371030", "21370105", "21371026", "21370104" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21405182", "21396297", "21405197", "21394088" }; */
  #endif


  // Updated to new particle 20161219
  // machine_004 (2nd feedback and newpartic).pro - TIVSA40_LQ_004 (2nd feedback and newpartic).tsm
  #if SERIAL_N==201400007 // OLD 2014013 2013008  - 3 bottom lights - updated 20140512
    #define MACHINE_ID     "2014-00-007" // "2014-013V" "2013-008VB" Yoko Pharma
    #define CUSTOMER_NAME L"南京优科"
    #define CRCAM  0xE386 
    #define CRCODE 0x4DA8 // 19880

    #define GRIPPER_STATISTICS
    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    //#define NEW_BURNS_INSPECTION  // test new filter
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21370103", "21369683", "21370090"             }; //21371058 OLD FOR TC-C
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21371033", "22949129", "21428887", "21364274" };// Replaced P2-B on 20190408
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };//P2A 21369705 TO 22676236 P2B 21371089 TO 22671142
    static char*    part2_serials[4] = { "22676236", "21371089", "21371058", "21371046" };//Changed cameras as above 20181017 //changed camera for P2_D from 21370090 to 21371046 on 20190403
    //static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    //static char*    cosm_serials [4] = { "21404664", "21405169", "21405161", "21405175" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21427724", "21416226", "21378297", "21396297" };
  #endif



  #if SERIAL_N==201400023   // new machine 20141011 L40 for liquid vials with Cap inspection station + three particle inspection station - CIRON
    #define MACHINE_ID         "2014-00-023" // 
    #define CUSTOMER_NAME     L"CIRON"

    #undef  MODEL_NAME     //   L"TIVS-A40"
    #define MODEL_NAME        L"TIVS-L40"
    
    #define CRCODE 0xFF1 // 4081
    #define CRCAM  0x5EEC
    #define GRIPPER_STATISTICS
    
    #define CIRON_BATCH   // to use ciron custom batch report
	#define NEW_PARTICLE //Pour added for upgrading of software on 20190404

    //#define COMBINED_TIPCAP       // If defined both tip and cap are possible REMOVE AFTER TEST
    //#define REVERSE_BELT    // If defined button is added to allow reverse of belt in manual/jog mode

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21            // add new functions for V2.1 new_cap inspection
    #define FO_COLOR    // If defined camera is searched ad added for flip off color control

 //   #define DOTRING_COLOR  // If defined uses new dot/ring color analysis (NITIN)

    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
//    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    //#define STN1 PARTICLE_BOTH    // First  station defined for selectable particle  //Pour removed 20190405
	#define STN1 PARTICLE_BACK    // First  station defined for particle with bottom light  //Pour added 20190405
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    #undef  STN1_FILLING_LEVEL    // remove filling level to STN1 defined by default

    static BYTE         PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT         tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21428843", "21499086", "21428853"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "21422357"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21499081", "21491221", "21499064", "21499055" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21405175", "21405161", "21404664", "21405169" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21499083", "21499085", "21521104", "21521106" };
  #endif



  #if SERIAL_N==201400034   // new machine 20141011 A40 machine for Chongqing Exhibition with 3 particle inspection station
    #define MACHINE_ID "2014-00-034" // 
    #define CRCAM  0x86C3 
    #define CRCODE 0x1055 // 4181
    #define NEW_TRIPLE_TIP
    //#define NEW_BURNS_INSPECTION  // Filter to neglect vertical lines/scratches
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21428861", "21428840", "21428841"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21405130", "21405143", "21428741", "21428712" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21428740", "21428833", "21428834", "21371050" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21405133", "21502491", "21499053", "21499065" };
  #endif

// 21605985 21502502 21605972
// 21605953 21428840 21428861


  #if SERIAL_N==201400025 // OLD 2014001 1ST NITIN - 2 BOTTOM + COSMETIC ( AMPOULES )
    // ORIGINAL
    // machine_004 (2nd feedback and lights fault).pro
    // TIVSA40_LQ_004 (2nd feedback and lights fault and LAM faults).tsm

    // NEW FOR TESTING NEW PARTIC WITH CAMSYNCH
    // machine_004 (2nd feedback and newpartic).pro
    // TIVSA40_LQ_004 (2nd feedback and newpartic NITIN).tsm
    #define MACHINE_ID     "2014-00-025" // OLD "2014-001VC"
    #define CUSTOMER_NAME L"NITIN"
    #define CRCAM   0x5597 // 9385 
    #define CRCODE  0x43B4 // 17332

    #define NEW_SVPWD // if defined set new supervisor password to TFLSUPWD

    #define DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    #undef  FO_COLOR         // If defined camera is searched ad added for flip off color control
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        9  // number of cameras connected to PC1
    static wchar_t* dtrng_names  [2] = { L"DR_A",    L"DR_B"                            };
    static char*    dtrng_serials[2] = { "21422348", "21422351"                         };

    #define STN1 PARTICLE_BACK    // First  station defined for particle with bottom light
//    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define NEW_TRIPLE_TIP
    #define GRIPPER_STATISTICS
    #define SAVE_INPUTDATA        // save input data and filling level data
    //#undef  STN1_FILLING_LEVEL    // remove filling level to STN1 defined by default
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,8} }; // tip/cap - partic1 - DOT/RING colorf
    static UINT         tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic

//    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
//    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21404652", "21405131", "21404671"             };
//    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
//    static char*    foc_serials  [1] = { "21413191"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21404675", "21405136", "21397047", "21405154" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21405149", "21404657", "21404656", "21404679" };
    //static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    //static char*    cosm_serials [4] = { "21397050", "21370130", "21405147", "21405152" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21426575", "21427715", "21422449", "21427703" };
  #endif

  #if SERIAL_N==201400026 // OLD 2013020 2ND NITIN - 2 BOTTOM + COSMETIC ( AMPOULES )
    // machine_004 (2nd feedback and lights fault).pro
    // TIVSA40_LQ_004 (2nd feedback and lights fault and LAM faults).tsm
    #define MACHINE_ID     "2014-00-026" // OLD "2013-020V"
    #define CUSTOMER_NAME L"NITIN"
    #define CRCAM  0x49F6 // 18934 //17303 on 20190325
    #define CRCODE 0x3F31 // 16177

    #define NEW_SVPWD // if defined set new supervisor password to TFLSUPWD

    #define DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    #undef  FO_COLOR         // If defined camera is searched ad added for flip off color control
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        9  // number of cameras connected to PC1
    static wchar_t* dtrng_names  [2] = { L"DR_A",    L"DR_B"                            };
    static char*    dtrng_serials[2] = { "21425011", "21423982"                         };// DR_A replaced from 21418148 on 20190321  DR_B replaced from 21422363 on 20190325 //DR_B 21422351 changed to 21423982

    #define STN1 PARTICLE_BACK    // First  station defined for particle with bottom light
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define NEW_TRIPLE_TIP
    #define GRIPPER_STATISTICS
    #define SAVE_INPUTDATA        // save input data and filling level data
    //#undef  STN1_FILLING_LEVEL    // remove filling level to STN1 defined by default
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,8} }; // tip/cap - partic1 - DOT/RING colorf
//    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21428854", "21428837", "21428847"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22421225", "21499060", "21428711", "21428855" }; // P1_B replaced from 21428745 on 20141015
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21428742", "21428842", "21428836", "21428832" };
    //static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    //static char*    cosm_serials [4] = { "21428740", "21428833", "21428834", "21371050" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21426565", "21427701", "21427709", "21427695" };
  #endif

  #if SERIAL_N==201400027 // OLD 2014003 3RD NITIN - 2 BOTTOM + COSMETIC + FO COLOR (VIALS)
    // machine_004 (2nd feedback and lights fault).pro
    // TIVSA40_LQ_004 (2nd feedback and lights fault and LAM faults).tsm
    #define MACHINE_ID     "2014-00-027" // OLD "2014-003VC"
    #define CUSTOMER_NAME L"NITIN"
    #define CRCAM  0x362D // 974E // CED0 //362D
    #define CRCODE 0xA0F1 // 41201

    #define NEW_SVPWD // if defined set new supervisor password to TFLSUPWD

/*    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
    #define NEWCAP_DOUBLE
    #define FO_COLOR    // If defined camera is searched ad added for flip off color control*/
    #define DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    #undef  FO_COLOR         // If defined camera is searched ad added for flip off color control
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        9  // number of cameras connected to PC1
    static wchar_t* dtrng_names  [2] = { L"DR_A",    L"DR_B"                            };
    static char*    dtrng_serials[2] = { "21422366", "21423981"                         };

//    #define STN1        PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1        PARTICLE_BACK    // First  station defined for particle with bottom light
    #define STN2        PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3        PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define NEW_TRIPLE_TIP
    #define GRIPPER_STATISTICS
    #define SAVE_INPUTDATA        // save input data and filling level data
    //#undef  STN1_FILLING_LEVEL    // remove filling level to STN1 defined by default
    static BYTE     PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
//    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
//    static UINT         tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1

    static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,8} }; // tip/cap - partic1 - DOT/RING colorf
    static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21428850", "21428852", "21428872"             };
//    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
//    static char*    foc_serials  [1] = { "21413191"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21499087", "22597079", "21405162", "21405134" };
//	static char*    part1_serials[4] = { "21499087", "21405135", "21405162", "21405134" }; // old, P1_B problem replaced as above on 20190404
//  static char*    part1_serials[4] = { "21405174", "21405135", "21405162", "21405134" }; // old, P1_A problem replaced as above on 20170517
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21405150", "21370115", "21364277", "21405166" };
    //static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    //static char*    cosm_serials [4] = { "21404667", "21404660", "21404687", "21404666" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21413752", "21416063", "21413754", "21413756" };
  #endif

  #if SERIAL_N==201400028 // OLD 2014004 4TH NITIN - 2 BOTTOM + COSMETIC + FO COLOR (VIALS)
    // machine_004 (2nd feedback and lights fault).pro
    // TIVSA40_LQ_004 (2nd feedback and lights fault and LAM faults).tsm
    #define MACHINE_ID     "2014-00-028"  // OLD "2014-004VC"
    #define CUSTOMER_NAME L"NITIN"
    #define CRCAM  0x17D6 //44F2 
    #define CRCODE 0xB770 // 46960

    #define NEW_SVPWD // if defined set new supervisor password to TFLSUPWD

/*    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
    #define NEWCAP_DOUBLE
    #define FO_COLOR    // If defined camera is searched ad added for flip off color control */

    #define DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    #undef  FO_COLOR         // If defined camera is searched ad added for flip off color control
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        9  // number of cameras connected to PC1
    static wchar_t* dtrng_names  [2] = { L"DR_A",    L"DR_B"                            };
    static char*    dtrng_serials[2] = { "21423987", "21425016"                         };

//    #define STN1        PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1        PARTICLE_BACK    // First  station defined for particle with bottom light
    #define STN2        PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3        PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define NEW_TRIPLE_TIP
    #define GRIPPER_STATISTICS
    #define SAVE_INPUTDATA        // save input data and filling level data
    //#undef  STN1_FILLING_LEVEL    // remove filling level to STN1 defined by default
    static BYTE         PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
//    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
//    static UINT         tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,8} }; // tip/cap - partic1 - DOT/RING colorf
    static UINT         tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21428874", "21428739", "21428715"             };
//    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
//    static char*    foc_serials  [1] = { "21422364"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21405168", "21404670", "21371052", "21404665" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21404680", "21397068", "21405172", "21404672" };
    //static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    //static char*    cosm_serials [4] = { "21405143", "21405130", "21428712", "21428741" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21416234", "21413201", "21413757", "21416046" };
  #endif


// Aggiornamento 20170228

  #if SERIAL_N==201400030 // NEW AMPOULE MACHINE - 3 BOTTOM AMPOULES - HARBIN - SONG HE
    #define MACHINE_ID  "2014-00-030" // OLD "2014-021V"
    #define CUSTOMER_NAME L"哈尔滨松鹤"
    #define CRCAM  0xE3C7 
    #define CRCODE 0x7B90 // 31632
    #define GRIPPER_STATISTICS

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //Pour added on 20191223
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define PARTFLT_H8V4

    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21405155", "21428877", "21428860"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21428886", "21428717", "21428724", "21428869" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21428849", "21428734", "21428725", "21428881" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21428893", "21428889", "21428878", "21428738" };
  #endif


  #if SERIAL_N==201400031 // INTAS India Liquid ampoules + liquid vials 20141206
    #define MACHINE_ID  "2014-00-031" // OLD "2013-025V"
    #define CRCAM  0xA6FE 
    #define CRCODE 0x7C21 // 31777
    #define GRIPPER_STATISTICS
    #define COMBINED_TIPCAP       // If defined both tip and cap are possible
    #define FO_COLOR              // Flip off color control
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21            // add new functions for V2.1 new_cap inspection
    //#define NEW_TRIPLE_TIP
    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #undef  STN1_FILLING_LEVEL    // remove filling level to STN1 defined by default
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    //#define STN3 PARTICLE_BACK  // Third  station defined for particle with bottom light
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
//    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
//    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - cosmetic
    static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21499074", "21428868", "21428863"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "21517341"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21428755", "21428749", "21428880", "21428876" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21428754", "21428726", "21428750", "21428746" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "22376449", "21428864", "21428713", "21428757" };//from 21428895 to 22376449 on 20190408
  #endif


  #if SERIAL_N==201400032 // New standard liquid machine "Turkish Mypharma"
    #define MACHINE_ID  "2014-00-032" 
    #define CUSTOMER_NAME L"Turkish Mypharma"
    #define CRCAM  0x4010 //16400 Pour changed from 0x4010 to xxx for new P2_D camera 20180116
    #define CRCODE 0x639B // 25499
    #define NEW_TRIPLE_TIP

    // updated to new partic and recompiled on 20170314
    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21428882", "21428896", "21428851"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21428735", "21428714", "21428897", "21428720" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };  //Pour changed P2_D camera from 21428751 to 22376418
    static char*    part2_serials[4] = { "21428752", "21428828", "21428848", "22376418" };  //Pour 20180116
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21426577", "21426571", "21425313", "21571409" }; // 20170322 sost CM_D
//    static char*    cosm_serials [4] = { "21426577", "21426571", "21425313", "21414303" };
//    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
//    static char*    cosm_serials [4] = { "21428727", "21428744", "21405158", "21428826" };
  #endif


  // Macchina officina 
  // aggiornata 20161206 al nuovo particellare
  // TIVS_L40_005 (2nd feedback and newpartic).tsm
  // machine_004 (2nd feedback and newpartic).pro
  #if SERIAL_N==201400033 // New standard liquid machine Lunan Houpu O40
    #define MACHINE_ID     "2014-00-033" 
    #define CUSTOMER_NAME L"Lunan Houpu"
    #define CRCODE  0xD07A // 53370
    #define CRCAM   0xA9BE
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    //#define NEWCAP_ORALCAP
    #define NEWCAP_V21            // add new functions for V2.1 new_cap inspection
    #define FO_COLOR        // If defined camera is searched ad added for flip off color control
    #define REVERSE_BELT    // If defined button is added to allow reverse of belt in manual/jog mode

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
    //#define READ_CYCLES     // if defined enables reading of cycles from PLC

    // #define CUSTOMER_NAME L""
    //#define NEW_TRIPLE_TIP
    #define GRIPPER_STATISTICS
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with bottom light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images

    //#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN2 PARTICLE_BACK    // Second station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    
    //#define NEWCOSMETIC_V20
    #define NEWCOSMETIC_V30


    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
//    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
//    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21428866", "21499075", "21428732"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "21418145"                                     };
//    static char*    foc_serials  [1] = { "21423947"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21404667", "21404660", "21404687", "21404666" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21428756", "21428835", "21428830", "21369689" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21571408", "21427718", "21427737", "21538483" };
//    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
//    static char*    cosm_serials [4] = { "21428753", "21428728", "21428743", "21428857" };
  #endif

  #if SERIAL_N==201400050 // New ampoules liquid machine - single feedback - PAKISTAN BOSCH
    #define MACHINE_ID  "2014-00-050" 
    #define CUSTOMER_NAME L"BOSCH"
    #define CRCAM   0x093D
    #define CRCODE  0x3A74 // 14964
    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP
//    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
//    #define STN1 PARTICLE_BACK    // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BOTH    // First  station defined for particle with selectable back/bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK    // Third  station defined for cosmetic with backlight
//    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21520480", "21517353", "21520524"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21428727", "21428744", "21405158", "21428826" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21521101", "21521115", "21516392", "21521097" };
//    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
//    static char*    cosm_serials [4] = { "21521122", "21521103", "21521095", "21521105" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21427756", "21427757", "21427698", "21427705" };
  #endif

  #if SERIAL_N==201400051 // New ampoules liquid machine - single feedback - PAKISTAN BOSCH
    #define MACHINE_ID  "2014-00-051" 
    #define CUSTOMER_NAME L"BOSCH"
    #define CRCAM   0x83C2
    #define CRCODE  0x39AC // 14764
    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP
//    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
//    #define STN1 PARTICLE_BACK    // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BOTH    // First  station defined for particle with selectable back/bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK    // Third  station defined for cosmetic with backlight
//    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21516386", "21517370", "21521112"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21491218", "21499079", "21499095", "21499067" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21520522", "21521120", "21520525", "21520541" };
//    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
//    static char*    cosm_serials [4] = { "21521094", "21517368", "21520532", "21517348" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21427730", "21427750", "21427751", "21427736" };
  #endif


  #if SERIAL_N==201400057 // New ampoules liquid machine - single feedback - 20151223 - Guandong Sunho 20170601
    // Originale:
    // TIVSA40_LQ_004 (single feedback and lights fault and LAM faults - ratio490).tsm
    // machine_004 (single feedback and lights fault).pro

    // Per prova new particle:
    // TIVSA40_LQ_005 (single feedback and newpartic- ratio490).tsm
    // machine_005 (single feedback and newpartic).pro
    #define MACHINE_ID  "2014-00-057" 
    #define CUSTOMER_NAME L"广东星昊" // Guangdong Sunho Pharmaceutical Co., Ltd
    #define CRCAM   0xD852
    #define CRCODE  0x16D2 // 5842
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP
 //   #define NEW_TIP_V30    // new tip control developed for Kelun
 //   #define WORKER_THREAD  // if defined uses worker threads to speed-up processing

	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE
    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define SAVE_IMAGES
    #define NEW_PARTICLE

	//Pour added 20190717
	//#define OPENCV_PARTICLE
	//Pour added 20190717

    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW
    #define READ_CYCLES     // if defined enables reading of cycles from PLC


//    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    /*
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21692684", "21692676", "21691583"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21397050", "21691569", "21691574", "21428733" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21691596", "21692683", "21692660", "21763814" };
//    static char*    part2_serials[4] = { "21691596", "21692683", "21692660", "21691591" }; // P2_D replaced
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21691581", "21691575", "21691613", "21691585" };
    original at Luye */

    // New cameras changed on 20170316
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21692684", "21692676", "21691583"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21397050", "21691569", "21691574", "21428733" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21691596", "21692683", "21692660", "21763814" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21920775", "21920767", "21920783", "21920780" };

  #endif



  #if SERIAL_N==201400058 // New ampoules liquid machine - single feedback - 20151223 - changed to L40 on 20160525
    // TIVSA40_LQ_004 (single feedback and lights fault and LAM faults - ratio490).tsm
    // machine_004 (single feedback and lights fault).pro
    
    #undef  MODEL_NAME     //   L"TIVS-A40"
    #define MODEL_NAME     L"TIVS-L40"

    #define MACHINE_ID  "2014-00-058" 
    #define CUSTOMER_NAME L"北京三元"
    #define CRCAM   0x8028
    #define CRCODE  0x1665 // 5733
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    //#define NEW_TRIPLE_TIP

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21            // add new functions for V2.1 new_cap inspection
    #define FO_COLOR    // If defined camera is searched ad added for flip off color control

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

//    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images

    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS

    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    // static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    // static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic


    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21692650", "21691610", "21691607"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "21424996"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21521103", "21521095", "21629400", "21521105" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21691605", "21692659", "21691589", "21691572" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21521094", "21517368", "21520532", "21517348" };
  #endif


//  richiesta agg HGA 20170602
// TIVSA40_LQ_005 (single feedback and newpartic and HGA - ratio420).tsm
// machine_004 (single feedback and lights fault).pro

  #if SERIAL_N==201400059 // New ampoules liquid machine - single feedback - 20151223 - Changed 20160518 in L40 - Gansu Chengji 
    // TIVSA40_LQ_004 (single feedback and lights fault and LAM faults - ratio420).tsm
    // machine_004 (single feedback and lights fault).pro
    #undef  MODEL_NAME     //   L"TIVS-A40"
    #define MODEL_NAME     L"TIVS-L40" // Gansu Chengji 
    #define MACHINE_ID      "2014-00-059" 
    #define CUSTOMER_NAME  L"甘肃成纪"
    //#define CRCAM   0xF78F //1026
	#define CRCAM   0x7FC9 //32713
    #define CRCODE  0x1615 // 5653
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data
	#define COLOR_DETECTION_P1 //added for liquid color inspection on 20210320
    //#define NEWCAP_DOUBLE
    //#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    //#define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #define SAVE_IMAGES
    #define NEW_PARTICLE

    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
    //#define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #define NEWCOSMETIC_V20       // add new functions for V2.0 new cosmetic (Gansu Chengji)

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    #define GRIPPER_STATISTICS

    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        7  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
     static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
     static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    //static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    //static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21680796", "21605964", "21680783"             };
    //static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    //static char*    foc_serials  [1] = { "21423947"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
	static char*    part1_serials[4] = { "23597001", "23597000", "23596994", "23696648" };
    //static char*    part1_serials[4] = { "21683545", "21680780", "21680757", "21680789" };
    //static char*    part1_serials[4] = { "21601282", "21629410", "21603551", "21629441" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21603551", "21629441", "21629410", "21601282" };
    //static char*    part2_serials[4] = { "21603542", "21603532", "21629450", "21629398" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21700010", "21537466", "21692824", "21538489" };
//    static char*    cosm_serials [4] = { "21700010", "21537466", "21427755", "21538489" };
    //static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    //static char*    cosm_serials [4] = { "21680789", "21680757", "21680780", "21683545" };
  #endif



// ricompilato per FAT 20170628 con nome customer
// aggiornata 20170329 con new partic e color dot/ring per test prima di Nitin
// machine_005 (single feedback and newpartic).pro
// TIVSA40_LQ_005 (single feedback and newpartic- ratio420).tsm
  #if SERIAL_N==201400060 // New ampoules liquid machine - single feedback
    #define MACHINE_ID  "2014-00-060" 
    #define CUSTOMER_NAME  L"中科生物"              // ZhongKe Biopharm Co., Ltd.
    #define CUSTOMER_NAMEE L"ZhongKe Biopharm Co." // ZhongKe Biopharm Co., Ltd.
    #define CRCAM   0x503B
    #define CRCODE  0x16D5 // 5845
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

/* test dot ring
    #define CRCAM   0xE7C3 // orig 503B
    #define DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    //#undef  FO_COLOR         // If defined camera is searched ad added for flip off color control
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        9  // number of cameras connected to PC1
    static wchar_t* dtrng_names  [2] = { L"DR_A",    L"DR_B"                            };
    static char*    dtrng_serials[2] = { "22106488", "22103553"                         };
    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,8} }; // tip/cap - partic1 - DOT/RING colorf
    static UINT         tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic
    */

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21680756", "21680793", "21605977"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21680781", "21629474", "21629437", "21605958" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21629467", "21629411", "21627159", "21629408" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21680795", "21603556", "21603518", "21680776" };
  #endif


  #if SERIAL_N==201600001 // New ampoule liquid machine - single feedback - for Russian exhibition 20160823
    // TIVSA40_LQ_004 (single feedback and lights fault and LAM faults - ratio420).tsm
    // machine_004 (single feedback and lights fault).pro
    #define MACHINE_ID  "2016-00-001" 
    #define CUSTOMER_NAMEE L"RUE BELMEDPREPARATY"
    #define CRCAM   0x9296
    #define CRCODE  0x15DA // 5594
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21691612", "21762634", "21691621"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21750622", "21762602", "21762648", "21763774" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21680775", "21680759", "21680790", "21763790" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21763812", "21762616", "21762639", "21762609" };
  #endif


  // old machine_005 (single feedback and newpartic).pro
  // old TIVSA40_LQ_005 (single feedback and newpartic- ratio420).tsm

  // agg 20170330 per aggiungere dotring x test presso Nitin
  // machine_005 (single feedback and newpartic).pro
  // TIVSA40_LQ_005 (single feedback and newpartic- ratio420).tsm
  #if SERIAL_N==201600002 // New ampoule liquid machine - single feedback - prepared 20161221
    #define MACHINE_ID  "2016-00-002" 
    //#define CUSTOMER_NAME L"备货"
    #define CRCAM   0x64C5 // CD3D
    #define CRCODE  0x52DA // 21210
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_SVPWD // if defined set new supervisor password to TFLSUPWD

    #define DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    //#undef  FO_COLOR         // If defined camera is searched ad added for flip off color control
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        9  // number of cameras connected to PC1
    static wchar_t* dtrng_names  [2] = { L"DR_A",    L"DR_B"                            };
    static char*    dtrng_serials[2] = { "22106488", "22103553"                         };

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define STN3 PARTICLE_BACK    // Third  station defined for particle with back light

    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    //static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

     // x dotring
    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,8} }; // tip/cap - partic1 - DOT/RING colorf
    static UINT         tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21756764", "21762645", "21763779"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21762646", "21692653", "21763815", "21763778" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21603522", "21603530", "21762611", "21763793" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21603560", "21681442", "21683538", "21680768" };
  #endif


// changed on 20170327 from A40 to L40

  #if SERIAL_N==201600003 // New ampoule liquid machine - single feedback - for Indian exhibition 20160919 - Guandong Sunho 20170601
    // machine_005 (single feedback and newpartic).pro
    // TIVSA40_LQ_005 (single feedback and newpartic- ratio420).tsm
    
    // old TIVSA40_LQ_004 (single feedback and lights fault and LAM faults - ratio420).tsm
    // old machine_004 (single feedback and lights fault).pro
    #undef  MODEL_NAME     //   L"TIVS-A40"
    #define MODEL_NAME     L"TIVS-L40"

    #define MACHINE_ID  "2016-00-003" 
    #define CUSTOMER_NAME L"广东星昊" // Guangdong Sunho Pharaceutical Co., Ltd.
    #define CRCAM   0x3FD7 // F9D2
    #define CRCODE  0x50DA // 20698
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    //#define NEW_TRIPLE_TIP

	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE
    #define NEW_PASSW_MNGR  // New users/passwords manager

    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC
    
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21            // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #define GRIPPER_STATISTICS
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	//Pour added 20190717
	#define OPENCV_PARTICLE
	//Pour added 20190717
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21763769", "21763804", "21762621"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22103554"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21762618", "21762617", "21763791", "21763795" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21762638", "21763772", "21762635", "21750625" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "21916046", "21920766", "21372311", "21374063" };

  #endif


  #if SERIAL_N==201600004 // New ampoule liquid machine - single feedback - 20170609 changed to 玉溪泽润 Yuxi Zerun - old 吉林紫鑫 Jilin Zixin Pharmaceutical Industrial Co., Ltd.

    // TIVSA40_LQ_005 (single feedback and newpartic- ratio420).tsm
    // machine_005 (single feedback and newpartic).pro
    #define MACHINE_ID  "2016-00-004" 
    #define CUSTOMER_NAME L"玉溪泽润" // Yuxi Zerun (old Jilin Zixin Pharmaceutical Industrial Co., Ltd.)
    #define CRCAM   0x1E4C
    #define CRCODE  0x2C01 // 11265
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21950319", "21953333", "21953342"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21951219", "21951224", "21952422", "21951216" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21952436", "21951252", "21951243", "21951253" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21953340", "21953329", "21950327", "21952424" };
  #endif


  #if SERIAL_N==201600006 // New ampoule liquid machine - single feedback - 成都苑东 EASTON BIOHPARMACEUTICALS
    // TIVSA40_LQ_004 (single feedback and lights fault and LAM faults - ratio420).tsm
    // machine_004 (single feedback and lights fault).pro
    #define MACHINE_ID  "2016-00-006" 
    #define CUSTOMER_NAME L"成都苑东" // EASTON BIOHPARMACEUTICALS
    #define CRCAM   0x2452
    #define CRCODE  0x2C20 // 11296
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21952437", "21952435", "21953346"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21950318", "21953339", "21952430", "21950314" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21953338", "21953343", "21953341", "21953334" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21952423", "21953327", "21931949", "21953336" };
  #endif

	//Pour added 20171104
	//  richiesta agg HGA 20170602
// TIVSA40_LQ_005 (single feedback and newpartic and HGA - ratio420).tsm
// machine_004 (single feedback and lights fault).pro

  #if SERIAL_N==20170000500 // New ampoules liquid machine - single feedback - 20151223 - Changed 20160518 in L40 - Gansu Chengji 
    // TIVSA40_LQ_004 (single feedback and lights fault and LAM faults - ratio420).tsm
    // machine_004 (single feedback and lights fault).pro
    #undef  MODEL_NAME     //   L"TIVS-A40"
    #define MODEL_NAME     L"TIVS-L40" // Gansu Chengji 
    #define MACHINE_ID      "2017-00-005" 
    #define CUSTOMER_NAME  L"华昊中天"
    #define CRCAM   0x161A //5658
    //#define CRCODE  0x1615 // 5653  //Pour removed 20171104
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    //#define NEW_TRIPLE_TIP

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

	#define NEW_LOGGER      // uses new log encyrption and digital signature
    #define NEW_PASSW_MNGR  // New users/passwords manager

	#define INPUTBELT_SENSOR // Id defined set input sensor delays
	#define OUTPUT_BELT      // if defined output belt management and parameters enabled

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
	#define READ_CYCLES     // if defined enables reading of cycles from PLC

    //#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
    //#define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #define NEWCOSMETIC_V20       // add new functions for V2.0 new cosmetic (Gansu Chengji)

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    #define GRIPPER_STATISTICS

    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    // static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    // static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22346068", "22389125", "22389130"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "21425002"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22389159", "22389117", "22389164", "22389145" };
    //static char*    part1_serials[4] = { "21601282", "21629410", "21603551", "21629441" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22389154", "22389156", "22389176", "22364194" };
    //static char*    part2_serials[4] = { "21603542", "21603532", "21629450", "21629398" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "22346688", "22306240", "22346773", "22346704" };
//    static char*    cosm_serials [4] = { "21700010", "21537466", "21427755", "21538489" };
    //static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    //static char*    cosm_serials [4] = { "21680789", "21680757", "21680780", "21683545" };
  #endif
	//Pour added 20171104

	//Pour added 20171115
	// ricompilato per FAT 20170628 con nome customer
// aggiornata 20170329 con new partic e color dot/ring per test prima di Nitin
// machine_005 (single feedback and newpartic).pro
// TIVSA40_LQ_005 (single feedback and newpartic- ratio420).tsm
  #if SERIAL_N==201700011 // 2017-024V
    #define MACHINE_ID  "2017-00-011" 
    #define CUSTOMER_NAME  L"瑞安亿拓"              // ZhongKe Biopharm Co., Ltd.
    #define CUSTOMER_NAMEE L"Fanavar Mobtabker Caspian" // FMC.
    #define CRCAM   0x9E8D //40589 //Pour added 20171115
//    #define CRCODE  0x16D5 // 5845
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

/* test dot ring
    #define CRCAM   0xE7C3 // orig 503B
    #define DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    //#undef  FO_COLOR         // If defined camera is searched ad added for flip off color control
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        9  // number of cameras connected to PC1
    static wchar_t* dtrng_names  [2] = { L"DR_A",    L"DR_B"                            };
    static char*    dtrng_serials[2] = { "22106488", "22103553"                         };
    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,8} }; // tip/cap - partic1 - DOT/RING colorf
    static UINT         tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic
    */

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22389163", "21931944", "21627137"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22346502", "22346522", "22346507", "22346523" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21952429", "22346518", "21629473", "21629416" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "22346540", "22346547", "22346541", "22346530" };
  #endif
	//Pour added 20171115

		//Pour added 20180323
	// ricompilato per FAT 20170628 con nome customer
// aggiornata 20170329 con new partic e color dot/ring per test prima di Nitin
// machine_005 (single feedback and newpartic).pro
// TIVSA40_LQ_005 (single feedback and newpartic- ratio420).tsm
  #if SERIAL_N==201700017 // New ampoules liquid machine - single feedback
    #define MACHINE_ID  "2017-00-017" 
    #define CUSTOMER_NAME  L"ZAFA"       // ZAFA
    #define CUSTOMER_NAMEE L"ZAFA"              // ZAFA
    #define CRCAM   0x52E //1326 //Pour added 20180323
//    #define CRCODE  0x16D5 // 5845
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

/* test dot ring
    #define CRCAM   0xE7C3 // orig 503B
    #define DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    //#undef  FO_COLOR         // If defined camera is searched ad added for flip off color control
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        9  // number of cameras connected to PC1
    static wchar_t* dtrng_names  [2] = { L"DR_A",    L"DR_B"                            };
    static char*    dtrng_serials[2] = { "22106488", "22103553"                         };
    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,8} }; // tip/cap - partic1 - DOT/RING colorf
    static UINT         tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic
    */

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "21321495", "21691597", "21322510"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21321488", "21321485", "21321487", "21293367" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "21321482", "21321486", "21321490", "21322511" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21629446", "21629405", "21680786", "21629421" };
  #endif
	//Pour added 20180323

			//Pour added 20180323
	// ricompilato per FAT 20170628 con nome customer
// aggiornata 20170329 con new partic e color dot/ring per test prima di Nitin
// machine_005 (single feedback and newpartic).pro
// TIVSA40_LQ_005 (single feedback and newpartic- ratio420).tsm
  #if SERIAL_N==201700004 // 2017-011Vc
    #define MACHINE_ID  "2017-00-004" 
    #define CUSTOMER_NAME  L"巴基斯坦NIP"       // Nabi Qasim--Pakistan
    #define CUSTOMER_NAMEE L"Nabi Qasim--Pakistan"              // Nabi Qasim--Pakistan
    #define CRCAM   0xDC59 //56409 //Pour added 20180323
//    #define CRCODE  0x16D5 // 5845
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

/* test dot ring
    #define CRCAM   0xE7C3 // orig 503B
    #define DOTRING_COLOR    // If defined, NITIN dor-ring presence and color enabled
    //#undef  FO_COLOR         // If defined camera is searched ad added for flip off color control
    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        9  // number of cameras connected to PC1
    static wchar_t* dtrng_names  [2] = { L"DR_A",    L"DR_B"                            };
    static char*    dtrng_serials[2] = { "22106488", "22103553"                         };
    static UINT         tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,8} }; // tip/cap - partic1 - DOT/RING colorf
    static UINT         tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic
    */

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22389143", "22389133", "22389123"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };//P1A changed from 22346071 to 23352133
    static char*    part1_serials[4] = { "23352133", "22389129", "22389126", "22346097" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22364195", "22389122", "22376431", "22389132" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "22460693", "22468088", "22468136", "22468141" };
  #endif
	//Pour added 20180323
/*
	//Pour added 20180406
// TIVSA40_LQ_005 (single feedback and newpartic and HGA - ratio420).tsm
// machine_004 (single feedback and lights fault).pro

  #if SERIAL_N==201700018 // New ampoules liquid machine - single feedback - 20151223 - Changed 20160518 in L40 - Gansu Chengji 
    // TIVSA40_LQ_004 (single feedback and lights fault and LAM faults - ratio420).tsm
    // machine_004 (single feedback and lights fault).pro
    #undef  MODEL_NAME     //   L"TIVS-A40"
    #define MODEL_NAME     L"TIVS-L40" // Oral liquid testing
    #define MACHINE_ID      "2017-00-020" 
    #define CUSTOMER_NAME  L"通辽华邦"
    #define CRCAM   0x345C //55422 //13404
    //#define CRCODE  0x1615 // 5653  //Pour removed 20171104
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    //#define NEW_TRIPLE_TIP

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

	#define NEW_LOGGER      // uses new log encyrption and digital signature
    #define NEW_PASSW_MNGR  // New users/passwords manager

	#define INPUTBELT_SENSOR // Id defined set input sensor delays
	#define OUTPUT_BELT      // if defined output belt management and parameters enabled

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
	#define READ_CYCLES     // if defined enables reading of cycles from PLC

    //#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
    //#define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #define NEWCOSMETIC_V20       // add new functions for V2.0 new cosmetic (Gansu Chengji)

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

    #define GRIPPER_STATISTICS

    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    // static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    // static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22376464", "22376450", "22376437"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "21423946"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22346070", "21692658", "22346079", "22389118" };
    //static char*    part1_serials[4] = { "21601282", "21629410", "21603551", "21629441" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22389159", "22389117", "22389164", "22389145" };
    //static char*    part2_serials[4] = { "21603542", "21603532", "21629450", "21629398" };
//    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
//    static char*    cosm_serials [4] = { "22346688", "22306240", "22346773", "22346704" };
//    static char*    cosm_serials [4] = { "21700010", "21537466", "21427755", "21538489" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "22389154", "22389156", "22389176", "22364194" };
  #endif

	//Pour added 20180406
*/
	// yang added 20180514
#if SERIAL_N==201700018 // New ampoules liquid machine - single feedback
    #define MACHINE_ID  "2017-028V" 
    #define CUSTOMER_NAME  L"通辽华邦"              //
    //#define CUSTOMER_NAMEE L"Fanavar Mobtabker Caspian" // FMC.
    #define CRCAM   0x4D0F //40589 //Pour added 20171115
//    #define CRCODE  0x16D5 // 5845
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //    #define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22376464", "22376450", "22376437"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22346070", "21692658", "22346079", "22389118" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22389159", "22389117", "22389164", "22389145" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "22389154", "22389156", "22389176", "22364194" };
  #endif
	//yang  added 20180514


		//Pour added 20180427
	//  richiesta agg HGA 20170602
// TIVSA40_LQ_005 (single feedback and newpartic and HGA - ratio420).tsm
// machine_004 (single feedback and lights fault).pro

  #if SERIAL_N==201700005 // 2018-006V 
    // TIVSA40_LQ_004 (single feedback and lights fault and LAM faults - ratio420).tsm
    // machine_004 (single feedback and lights fault).pro
    #undef  MODEL_NAME     //   L"TIVS-A40"
    #define MODEL_NAME     L"TIVS-L40"  
    #define MACHINE_ID      "2017-00-005" 
    #define CUSTOMER_NAME  L"复宏汉霖"
    #define CRCAM   0xF8C7 //63687
    //#define CRCODE  0x1615 // 5653  //Pour removed 20171104
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    //#define NEW_TRIPLE_TIP

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data
	#define ALARMLOG_SEPARATE
	#define NEW_LOGGER      // uses new log encyrption and digital signature
    #define NEW_PASSW_MNGR  // New users/passwords manager

	#define INPUTBELT_SENSOR // Id defined set input sensor delays
	#define OUTPUT_BELT      // if defined output belt management and parameters enabled

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control
	#define TEST_QILU //Pour defined for additional area on 20200310

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //Pour added on 20191129

    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
	#define READ_CYCLES     // if defined enables reading of cycles from PLC

    //#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
    //#define FO_COLOR         // If defined camera is searched ad added for flip off color control

    //#define NEWCOSMETIC_V20       // add new functions for V2.0 new cosmetic (Gansu Chengji)  //Pour removed on 20200304

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
    //#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

	#define MANU_CHECK

    #define GRIPPER_STATISTICS

    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    // static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    // static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22346531", "21939016", "22346545"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "21423968"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "21629463", "22346529", "22346542", "21950323" };
    //static char*    part1_serials[4] = { "21601282", "21629410", "21603551", "21629441" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22346539", "22346546", "22346517", "22346544" };
    //static char*    part2_serials[4] = { "21603542", "21603532", "21629450", "21629398" };
    static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials [4] = { "22346714", "22306241", "22306247", "22306239" };
//    static char*    cosm_serials [4] = { "21700010", "21537466", "21427755", "21538489" };
    //static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    //static char*    cosm_serials [4] = { "21680789", "21680757", "21680780", "21683545" };
  #endif
	//Pour added 20180427

	//Pour added 20181111
  #if SERIAL_N==201800023 // 2018-019V
    #define MACHINE_ID  "2018-00-023" 
    #define CUSTOMER_NAME  L"重庆福安"       //重庆福安
    #define CUSTOMER_NAMEE L"重庆福安"       // 重庆福安
    #define CRCAM   0x909C //37020 //Pour added 20181015
//    #define CRCODE  0x16D5 // 5845
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

	#define INPUTBELT_SENSOR

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22653208", "22581607", "22581632"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22581640", "22534413", "22676223", "22671136" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22653207", "22671147", "22584253", "22649366" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "22671148", "22581608", "22649370", "22581644" };
  #endif
	//Pour added 20181111

		//Pour added 20181015
  #if SERIAL_N==201800020 // 2018-017V
    #define MACHINE_ID  "2018-00-023" 
    #define CUSTOMER_NAME  L"多多药业"       //佳木斯多多
    #define CUSTOMER_NAMEE L"多多药业"       //佳木斯多多
    #define CRCAM   0x3327 //13095 //Pour added 20181015
//    #define CRCODE  0x16D5 // 5845
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22376430", "22346076", "22468104"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22468135", "22376409", "22468106", "22376412" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22376413", "22401399", "22468101", "22389189" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21605987", "21605970", "22180198", "22376429" };
  #endif
	//Pour added 20181015

		
	//Pour added 20181020
// TIVSA40_LQ_005 (single feedback and newpartic and HGA - ratio420).tsm
// machine_004 (single feedback and lights fault).pro


  #if SERIAL_N==201800018 // 2018-015V  201800018
    // machine_004 (single feedback and lights fault).pro
    #undef  MODEL_NAME     //   L"TIVS-A40"
    #define MODEL_NAME     L"TIVS-L40"  
    #define MACHINE_ID      "2018-00-016" 
    #define CUSTOMER_NAME  L"烟台百克"
    #define CRCAM   0xF1D0 //61904
    //#define CRCODE  0x1615 // 5653  //Pour removed 20171104
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    //#define NEW_TRIPLE_TIP

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

	#define NEW_LOGGER      // uses new log encyrption and digital signature
    #define NEW_PASSW_MNGR  // New users/passwords manager

	#define INPUTBELT_SENSOR // Id defined set input sensor delays
	#define OUTPUT_BELT      // if defined output belt management and parameters enabled

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	
	#define OPENCV_PARTICLE //Pour added on 20190514

    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
	#define READ_CYCLES     // if defined enables reading of cycles from PLC

    //#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
    //#define FO_COLOR         // If defined camera is searched ad added for flip off color control

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    //#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

	#define MANU_CHECK

    #define GRIPPER_STATISTICS

    #undef  NUM_TLC_PC1    
    #define NUM_TLC_PC1        8  // number of cameras connected to PC1
    #define NUM_TLC_PC2        8  // number of cameras connected to PC2
    #define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
    #define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    // static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    // static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
    static UINT     tlcgroupPC1[3][2] = { {0,2}, {3,6}, {7,7} }; // tip/cap - partic1 - FO color
    static UINT     tlcgroupPC2[2][2] = { {0,3}, {4,7}        }; // partic2 - cosmetic

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22653205", "22649372", "22647528"             };
    static wchar_t* foc_names    [1] = { L"TC_D"                                        };
    static char*    foc_serials  [1] = { "22627216"                                     };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22647541", "22649375", "22653179", "22649376" };
    //static char*    part1_serials[4] = { "21601282", "21629410", "21603551", "21629441" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22649374", "22653183", "22649365", "22649362" };
    //static char*    part2_serials[4] = { "21603542", "21603532", "21629450", "21629398" };
    //static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    //static char*    cosm_serials [4] = { "22346714", "22306241", "22306247", "22306239" };
//    static char*    cosm_serials [4] = { "21700010", "21537466", "21427755", "21538489" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "22676258", "22671107", "22671142", "21627146" };
  #endif
	//Pour added 20181020

	//Pour added 20190525
	// TIVSA40_LQ_005 (single feedback and newpartic and HGA - ratio420).tsm
	// machine_004 (single feedback and lights fault).pro


#if SERIAL_N==201900099 // 2018-XXX
	// machine_004 (single feedback and lights fault).pro
#undef  MODEL_NAME     //   L"TIVS-A40"
#define MODEL_NAME     L"TIVS-L40"  
#define MACHINE_ID      "2019-00-000" 
#define CUSTOMER_NAME  L"郑州创泰"
#define CRCAM   0x16E7 //5863 //Pour 20190525
	//#define CRCODE  0x1615 // 5653  //Pour removed 20171104
	//#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
	//#define NEW_TRIPLE_TIP

	//#define HGA_LEAKTEST        // HGA components installed
	//#define HGA_DATAREC         // enable option to record HGA data

#define NEW_LOGGER      // uses new log encyrption and digital signature
#define NEW_PASSW_MNGR  // New users/passwords manager

//#define INPUTBELT_SENSOR // Id defined set input sensor delays
//#define OUTPUT_BELT      // if defined output belt management and parameters enabled

#define NEWCAP_DOUBLE
#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define SAVE_IMAGES
#define NEW_PARTICLE

//#define OPENCV_PARTICLE //Pour added on 20190514

#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
#define READ_CYCLES     // if defined enables reading of cycles from PLC

	//#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
	//#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define STN1 PARTICLE_BACK    // First  station defined for particle with back light
#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

#define MANU_CHECK

#define GRIPPER_STATISTICS

#undef  NUM_TLC_PC1    
#define NUM_TLC_PC1        8  // number of cameras connected to PC1
#define NUM_TLC_PC2        8  // number of cameras connected to PC2
#define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
#define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	// static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
	// static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
	static UINT     tlcgroupPC1[3][2] = { { 0,2 },{ 3,6 },{ 7,7 } }; // tip/cap - partic1 - FO color
	static UINT     tlcgroupPC2[2][2] = { { 0,3 },{ 4,7 } }; // partic2 - cosmetic

	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "22581617", "22653241", "22653229" };
	static wchar_t* foc_names[1] = { L"TC_D" };
	static char*    foc_serials[1] = { "22408124" };
	static wchar_t* part1_names[4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D" };
	static char*    part1_serials[4] = { "22581624", "22591085", "22611460", "22581633" };
	//static char*    part1_serials[4] = { "21601282", "21629410", "21603551", "21629441" };
	static wchar_t* part2_names[4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D" };
	static char*    part2_serials[4] = { "22584237", "22653177", "22591097", "22584232" };
	//static char*    part2_serials[4] = { "21603542", "21603532", "21629450", "21629398" };
	//static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
	//static char*    cosm_serials [4] = { "22346714", "22306241", "22306247", "22306239" };
	//    static char*    cosm_serials [4] = { "21700010", "21537466", "21427755", "21538489" };
	static wchar_t* cosm_names[4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D" };
	static char*    cosm_serials[4] = { "22581602", "22581635", "22584258", "22653233" };
#endif
	//Pour added 20190525

	//Pour added 20190709
#if SERIAL_N==201900097 // 2018-XXX
	// machine_004 (single feedback and lights fault).pro
#undef  MODEL_NAME     //   L"TIVS-A40"
#define MODEL_NAME     L"TIVS-L40"  
#define MACHINE_ID      "2019-00-000" 
#define CUSTOMER_NAME  L"昂德生物"
#define CRCAM   0xF3E5 //62437 //Pour 20190709
	//#define CRCODE  0x1615 // 5653  //Pour removed 20171104
	//#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
	//#define NEW_TRIPLE_TIP


#define NEW_LOGGER      // uses new log encyrption and digital signature
#define NEW_PASSW_MNGR  // New users/passwords manager

	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//#define OUTPUT_BELT      // if defined output belt management and parameters enabled

#define NEWCAP_DOUBLE
#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define SAVE_IMAGES
#define NEW_PARTICLE

#define OPENCV_PARTICLE //Pour added on 20190514

#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
#define READ_CYCLES     // if defined enables reading of cycles from PLC

	//#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
	//#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define STN1 PARTICLE_BACK    // First  station defined for particle with back light
#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

#define MANU_CHECK

#define GRIPPER_STATISTICS

#undef  NUM_TLC_PC1    
#define NUM_TLC_PC1        8  // number of cameras connected to PC1
#define NUM_TLC_PC2        8  // number of cameras connected to PC2
#define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
#define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	// static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
	// static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
	static UINT     tlcgroupPC1[3][2] = { { 0,2 },{ 3,6 },{ 7,7 } }; // tip/cap - partic1 - FO color
	static UINT     tlcgroupPC2[2][2] = { { 0,3 },{ 4,7 } }; // partic2 - cosmetic

	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "22376451", "22671137", "22949124" };
	static wchar_t* foc_names[1] = { L"TC_D" };
	static char*    foc_serials[1] = { "22945539" };
	static wchar_t* part1_names[4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D" };
	static char*    part1_serials[4] = { "23003361", "23003369", "22965497", "23032323" };//P1-D 22945539
	//static char*    part1_serials[4] = { "21601282", "21629410", "21603551", "21629441" };
	static wchar_t* part2_names[4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D" };
	static char*    part2_serials[4] = { "22965518", "22965512", "22965529", "22957335" };
	//static char*    part2_serials[4] = { "21603542", "21603532", "21629450", "21629398" };
	//static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
	//static char*    cosm_serials [4] = { "22346714", "22306241", "22306247", "22306239" };
	//    static char*    cosm_serials [4] = { "21700010", "21537466", "21427755", "21538489" };
	static wchar_t* cosm_names[4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D" };
	static char*    cosm_serials[4] = { "22965510", "23001491", "23032337", "23001494" };
#endif
	//Pour added 20190709


		//Pour added 20200512
#if SERIAL_N==201900030 // 2019-027V
	// machine_004 (single feedback and lights fault).pro
#undef  MODEL_NAME     //   L"TIVS-A40"
#define MODEL_NAME     L"TIVS-L40"  
#define MACHINE_ID      "2019-00-030" 
#define CUSTOMER_NAME  L"上海昊海"
#define CRCAM   0x5D8C //23948
	//#define CRCODE  0x1615 // 5653  //Pour removed 20171104
	//#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
	//#define NEW_TRIPLE_TIP


#define NEW_LOGGER      // uses new log encyrption and digital signature
#define NEW_PASSW_MNGR  // New users/passwords manager

	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//#define OUTPUT_BELT      // if defined output belt management and parameters enabled

#define NEWCAP_DOUBLE
#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define TEST_QILU //Pour added for testing on 20200515
#define NECK_CRACK //Pour added for testing on 20200515

#define SAVE_IMAGES
#define NEW_PARTICLE

#define OPENCV_PARTICLE //Pour added on 20190514

#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
#define READ_CYCLES     // if defined enables reading of cycles from PLC

	//#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
	//#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define STN1 PARTICLE_BACK    // First  station defined for particle with back light
#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
#define NEWCOSMETIC_COMBO
//#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

//#define MANU_CHECK

#define GRIPPER_STATISTICS

#undef  NUM_TLC_PC1    
#define NUM_TLC_PC1        8  // number of cameras connected to PC1
#define NUM_TLC_PC2        8  // number of cameras connected to PC2
#define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
#define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	// static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
	// static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
	static UINT     tlcgroupPC1[3][2] = { { 0,2 },{ 3,6 },{ 7,7 } }; // tip/cap - partic1 - FO color
	static UINT     tlcgroupPC2[2][2] = { { 0,3 },{ 4,7 } }; // partic2 - cosmetic

	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "22965507", "22965498", "23179764" };
	static wchar_t* foc_names[1] = { L"TC_D" };
	static char*    foc_serials[1] = { "22443250" };
	static wchar_t* part1_names[4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D" };
	static char*    part1_serials[4] = { "22965516", "23269520", "22957339", "23144956" };//P1-D 22945539
	//static char*    part1_serials[4] = { "21601282", "21629410", "21603551", "21629441" };
	static wchar_t* part2_names[4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D" };
	static char*    part2_serials[4] = { "23019187", "22957341", "22965528", "23136276" };
	//static char*    part2_serials[4] = { "21603542", "21603532", "21629450", "21629398" };
	static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
	static char*    cosm_serials [4] = { "23269434", "23269412", "23269454", "23269447" };
	//    static char*    cosm_serials [4] = { "21700010", "21537466", "21427755", "21538489" };
	//static wchar_t* cosm_names[4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D" };
	//static char*    cosm_serials[4] = { "22965510", "23001491", "23032337", "23001494" };
#endif
	//Pour added 20200512



	//Pour added 20181023
  #if SERIAL_N==201800017 // 2018-007Vc
    #define MACHINE_ID  "2018-00-017" 
    #define CUSTOMER_NAME  L"巴基斯坦SAMI"       // SAMI--Pakistan
    #define CUSTOMER_NAMEE L"SAMI--Pakistan"     // SAMI--Pakistan
    #define CRCAM   0x9F25 //40741 //Pour added 20181023
//    #define CRCODE  0x16D5 // 5845
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define SAVE_IMAGES
    #define NEW_PARTICLE
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    //#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22671145", "22649367", "22671146"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22671101", "22649368", "22584228", "22649373" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22671149", "23352144", "22584230", "22584243" };//22584231
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "22584244", "22584256", "22581637", "22676229" };
  #endif
	//Pour added 20181023


	//Pour added 20190219
  #if SERIAL_N==201800028 // 2018-026V
    #define MACHINE_ID  "2018-00-028" 
    #define CUSTOMER_NAME  L"丽珠集团丽珠制药厂"       //珠海丽珠
    #define CUSTOMER_NAMEE L"丽珠集团丽珠制药厂"       //珠海丽珠
    #define CRCAM   0x3B //59 //Pour added 20190219
//    #define CRCODE  0x16D5 // 5845
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

	//#define INPUTBELT_SENSOR

    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //Pour added on 20200531
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight

    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "22346064", "22389121", "21369704"             };
    static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    static char*    part1_serials[4] = { "22534501", "22584261", "22584259", "22534442" };
    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "22534498", "22534452", "22581615", "22534437" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "21428834", "21428740", "21428833", "21371050" };
	//static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    //static char*    cosm_serials [4] = { "22874697", "22860043", "22874702", "22860047" };

  #endif

	//Pour added 20190219

	//Pour added 20201121
  #if SERIAL_N==201900106 // 2019-006V备
    #define MACHINE_ID  "2019-00-106" 
    #define CUSTOMER_NAME  L"备货"       //上海丽珠样品测试
    #define CUSTOMER_NAMEE L"备货"       //上海丽珠样品测试
    //#define CRCAM   0xDC37 //59 //56375

	#define CRCAM   0xB385 //45957 //replaced with color cameras for testing

//    #define CRCODE  0x16D5 // 5845
//    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEW_TRIPLE_TIP

    #define NEW_PASSW_MNGR  // New users/passwords manager

	//#define INPUTBELT_SENSOR
	#define COLOR_DETECTION_P1 //added for liquid color inspection on 20210320
    #define SAVE_IMAGES
    #define NEW_PARTICLE
	#define OPENCV_PARTICLE //Pour added on 20200531
    #define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW

    #define STN1 PARTICLE_BACK    // First  station defined for particle with back light
    #define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
    
    #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
    #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
	//#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight

    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };

    static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
    static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3

    static wchar_t* tip_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    tip_serials  [3] = { "23051277", "23051338", "23051349"             };
    //static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };
    //static char*    part1_serials[4] = { "23051328", "23051350", "23136273", "23052017" };

	static wchar_t* part1_names  [4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D"    };//replaced with color cameras for testing 
    static char*    part1_serials[4] = { "23389775", "22401181", "21425013", "23164063" };//replaced with color cameras for testing

    static wchar_t* part2_names  [4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D"    };
    static char*    part2_serials[4] = { "23051339", "23051332", "23051329", "23051341" };
    static wchar_t* cosm_names   [4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D"    };
    static char*    cosm_serials [4] = { "23051351", "23051386", "23144953", "23051346" };
	//static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    //static char*    cosm_serials [4] = { "22874697", "22860043", "22874702", "22860047" };

  #endif

	//Pour added 20201121



		//Pour added 20200602
#if SERIAL_N==202000002 // 2020-002V
	// machine_004 (single feedback and lights fault).pro
#undef  MODEL_NAME     //   L"TIVS-A40"
#define MODEL_NAME     L"TIVS-L40"  
#define MACHINE_ID      "2020-00-002" 
#define CUSTOMER_NAME  L"宁波三生"
#define CUSTOMER_NAMEE L"宁波三生"       //宁波三生
#define CRCAM   0x804C //32844
	//#define CRCODE  0x1615 // 5653  //Pour removed 20171104
	//#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
	//#define NEW_TRIPLE_TIP
#define NEW_LOGGER      // uses new log encyrption and digital signature
#define NEW_PASSW_MNGR  // New users/passwords manager

	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//#define OUTPUT_BELT      // if defined output belt management and parameters enabled

#define NEWCAP_DOUBLE
#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define TEST_QILU //Pour added for testing on 20200515
#define NECK_CRACK //Pour added for testing on 20200515

#define SAVE_IMAGES
#define NEW_PARTICLE

#define OPENCV_PARTICLE //Pour added on 20190514

#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
#define READ_CYCLES     // if defined enables reading of cycles from PLC

	//#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
	//#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define STN1 PARTICLE_BACK    // First  station defined for particle with back light
#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
//#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
//#define NEWCOSMETIC_COMBO
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

//#define MANU_CHECK

#define GRIPPER_STATISTICS

#undef  NUM_TLC_PC1    
#define NUM_TLC_PC1        8  // number of cameras connected to PC1
#define NUM_TLC_PC2        8  // number of cameras connected to PC2
#define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
#define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	// static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
	// static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
	static UINT     tlcgroupPC1[3][2] = { { 0,2 },{ 3,6 },{ 7,7 } }; // tip/cap - partic1 - FO color
	static UINT     tlcgroupPC2[2][2] = { { 0,3 },{ 4,7 } }; // partic2 - cosmetic

	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23352199", "23352166", "23352201" };
	static wchar_t* foc_names[1] = { L"TC_D" };
	static char*    foc_serials[1] = { "22401176" };
	static wchar_t* part1_names[4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D" };
	static char*    part1_serials[4] = { "23352147", "23352208", "23352111", "23352120" };
	static wchar_t* part2_names[4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D" };
	static char*    part2_serials[4] = { "23352214", "23352192", "23352153", "23352200" };
	//static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
	//static char*    cosm_serials [4] = { "23269434", "23269412", "23269454", "23269447" };
	static wchar_t* cosm_names[4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D" };
	static char*    cosm_serials[4] = { "23352396", "23352407", "23329310", "23352168" };
#endif
	//Pour added 20200602


	//Pour added 20200602
#if SERIAL_N==202000003 // 2020-003V
	// machine_004 (single feedback and lights fault).pro
#undef  MODEL_NAME     //   L"TIVS-A40"
#define MODEL_NAME     L"TIVS-L40"  
#define MACHINE_ID      "2020-00-003" 
#define CUSTOMER_NAME  L"宁波三生"
#define CUSTOMER_NAMEE L"宁波三生"       //宁波三生
#define CRCAM   0x2449 //9289
	//#define CRCODE  0x1615 // 5653  //Pour removed 20171104
	//#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
	//#define NEW_TRIPLE_TIP
#define NEW_LOGGER      // uses new log encyrption and digital signature
#define NEW_PASSW_MNGR  // New users/passwords manager

	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//#define OUTPUT_BELT      // if defined output belt management and parameters enabled

#define NEWCAP_DOUBLE
#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define TEST_QILU //Pour added for testing on 20200515
#define NECK_CRACK //Pour added for testing on 20200515

#define SAVE_IMAGES
#define NEW_PARTICLE

#define OPENCV_PARTICLE //Pour added on 20190514

#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
#define READ_CYCLES     // if defined enables reading of cycles from PLC

	//#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
	//#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define STN1 PARTICLE_BACK    // First  station defined for particle with back light
#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
//#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
//#define NEWCOSMETIC_COMBO
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

//#define MANU_CHECK

#define GRIPPER_STATISTICS

#undef  NUM_TLC_PC1    
#define NUM_TLC_PC1        8  // number of cameras connected to PC1
#define NUM_TLC_PC2        8  // number of cameras connected to PC2
#define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
#define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	// static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
	// static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
	static UINT     tlcgroupPC1[3][2] = { { 0,2 },{ 3,6 },{ 7,7 } }; // tip/cap - partic1 - FO color
	static UINT     tlcgroupPC2[2][2] = { { 0,3 },{ 4,7 } }; // partic2 - cosmetic

	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23352194", "23352160", "23352391" };
	static wchar_t* foc_names[1] = { L"TC_D" };
	static char*    foc_serials[1] = { "22967176" };
	static wchar_t* part1_names[4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D" };
	static char*    part1_serials[4] = { "23352195", "23352186", "23352170", "23352123" };
	static wchar_t* part2_names[4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D" };
	static char*    part2_serials[4] = { "23352193", "23352171", "23352399", "23352395" };
	//static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
	//static char*    cosm_serials [4] = { "23269434", "23269412", "23269454", "23269447" };
	static wchar_t* cosm_names[4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D" };
	static char*    cosm_serials[4] = { "23352406", "23352394", "23352217", "23352215" };
#endif
	//Pour added 20200602



	//Pour added 20200602
#if SERIAL_N==202000004 // 2020-003V
	// machine_004 (single feedback and lights fault).pro
#undef  MODEL_NAME     //   L"TIVS-A40"
#define MODEL_NAME     L"TIVS-L40"  
#define MACHINE_ID      "2020-00-004" 
#define CUSTOMER_NAME  L"宁波三生"
#define CUSTOMER_NAMEE L"宁波三生"       //宁波三生
#define CRCAM   0x29C2 //10690
	//#define CRCODE  0x1615 // 5653  //Pour removed 20171104
	//#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
	//#define NEW_TRIPLE_TIP
#define NEW_LOGGER      // uses new log encyrption and digital signature
#define NEW_PASSW_MNGR  // New users/passwords manager

	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//#define OUTPUT_BELT      // if defined output belt management and parameters enabled

#define NEWCAP_DOUBLE
#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define TEST_QILU //Pour added for testing on 20200515
#define NECK_CRACK //Pour added for testing on 20200515

#define SAVE_IMAGES
#define NEW_PARTICLE

#define OPENCV_PARTICLE //Pour added on 20190514

#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
#define READ_CYCLES     // if defined enables reading of cycles from PLC

	//#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
	//#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define STN1 PARTICLE_BACK    // First  station defined for particle with back light
#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
//#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
//#define NEWCOSMETIC_COMBO
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

//#define MANU_CHECK

#define GRIPPER_STATISTICS

#undef  NUM_TLC_PC1    
#define NUM_TLC_PC1        8  // number of cameras connected to PC1
#define NUM_TLC_PC2        8  // number of cameras connected to PC2
#define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
#define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	// static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
	// static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
	static UINT     tlcgroupPC1[3][2] = { { 0,2 },{ 3,6 },{ 7,7 } }; // tip/cap - partic1 - FO color
	static UINT     tlcgroupPC2[2][2] = { { 0,3 },{ 4,7 } }; // partic2 - cosmetic

	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23352177", "23352174", "23352157" };
	static wchar_t* foc_names[1] = { L"TC_D" };
	static char*    foc_serials[1] = { "22994165" };
	static wchar_t* part1_names[4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D" };
	static char*    part1_serials[4] = { "22649374", "22653183", "23352390", "23352136" };
	static wchar_t* part2_names[4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D" };
	static char*    part2_serials[4] = { "22647541", "22649375", "22653179", "22649376" };
	//static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
	//static char*    cosm_serials [4] = { "23269434", "23269412", "23269454", "23269447" };
	static wchar_t* cosm_names[4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D" };
	static char*    cosm_serials[4] = { "22676258", "22671107", "22671142", "21627146" };
#endif
	//Pour added 20200602


		//Pour added 20201012
#if SERIAL_N==202000101 // 2020-001VC
	// machine_004 (single feedback and lights fault).pro
#undef  MODEL_NAME     //   L"TIVS-A40"
#define MODEL_NAME     L"TIVS-L40"  
#define MACHINE_ID      "2020-00-101" 
#define CUSTOMER_NAME  L"R-Pharma"
#define CUSTOMER_NAMEE L"R-Pharma"
#define CRCAM   0xCC89 //52361
	//#define CRCODE  0x1615 // 5653  //Pour removed 20171104
	//#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
	//#define NEW_TRIPLE_TIP
#define NEW_LOGGER      // uses new log encyrption and digital signature
#define ALARMLOG_SEPARATE
#define NEW_PASSW_MNGR  // New users/passwords manager

	//#define INPUTBELT_SENSOR // Id defined set input sensor delays
	//#define OUTPUT_BELT      // if defined output belt management and parameters enabled

#define NEWCAP_DOUBLE
#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define TEST_QILU //Pour added for testing on 20200515
#define NECK_CRACK //Pour added for testing on 20200515

#define SAVE_IMAGES
#define NEW_PARTICLE

#define OPENCV_PARTICLE //Pour added on 20190514

#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
#define READ_CYCLES     // if defined enables reading of cycles from PLC

	//#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
	//#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define STN1 PARTICLE_BACK    // First  station defined for particle with back light
#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
//#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
//#define NEWCOSMETIC_COMBO
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

//#define MANU_CHECK

#define GRIPPER_STATISTICS

#undef  NUM_TLC_PC1    
#define NUM_TLC_PC1        8  // number of cameras connected to PC1
#define NUM_TLC_PC2        8  // number of cameras connected to PC2
#define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
#define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	// static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
	// static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
	static UINT     tlcgroupPC1[3][2] = { { 0,2 },{ 3,6 },{ 7,7 } }; // tip/cap - partic1 - FO color
	static UINT     tlcgroupPC2[2][2] = { { 0,3 },{ 4,7 } }; // partic2 - cosmetic

	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23418238", "23418250", "23418234" };
	static wchar_t* foc_names[1] = { L"TC_D" };
	static char*    foc_serials[1] = { "22967170" };
	static wchar_t* part1_names[4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D" };
	static char*    part1_serials[4] = { "23394319", "23418239", "23416213", "23418242" };
	static wchar_t* part2_names[4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D" };
	static char*    part2_serials[4] = { "23418249", "23418268", "23370905", "23418228" };
	//static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
	//static char*    cosm_serials [4] = { "23269434", "23269412", "23269454", "23269447" };
	static wchar_t* cosm_names[4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D" };
	static char*    cosm_serials[4] = { "23418255", "23418221", "23416209", "23416226" };
#endif
	//Pour added 20201012



			//Pour added 20201012
#if SERIAL_N==202000102 // 2020-002VC
	// machine_004 (single feedback and lights fault).pro
#undef  MODEL_NAME     //   L"TIVS-A40"
#define MODEL_NAME     L"TIVS-L40"  
#define MACHINE_ID      "2020-00-102" 
#define CUSTOMER_NAME  L"R-Pharma"
#define CUSTOMER_NAMEE L"R-Pharma"
#define CRCAM   0xD39 //3385
	//#define CRCODE  0x1615 // 5653  //Pour removed 20171104
	//#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
	//#define NEW_TRIPLE_TIP
#define NEW_LOGGER      // uses new log encyrption and digital signature
#define ALARMLOG_SEPARATE
#define NEW_PASSW_MNGR  // New users/passwords manager

#define INPUTBELT_SENSOR // Id defined set input sensor delays
//#define OUTPUT_BELT      // if defined output belt management and parameters enabled

#define NEWCAP_DOUBLE
#define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define TEST_QILU //Pour added for testing on 20200515
#define NECK_CRACK //Pour added for testing on 20200515

#define SAVE_IMAGES
#define NEW_PARTICLE

#define OPENCV_PARTICLE //Pour added on 20190514

#define SYNCH_PARTIC    // If enabled synch of cameras A/D B/E C/F enabled in HW  //Pour removed 20171104
#define READ_CYCLES     // if defined enables reading of cycles from PLC

	//#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
	//#define FO_COLOR         // If defined camera is searched ad added for flip off color control

#define STN1 PARTICLE_BACK    // First  station defined for particle with back light
#define FILLING_BOTTOMCOMP    // compensation of bottom height in backlight images
	//#define STN1 PARTICLE_BOTTOM  // First  station defined for particle with bottom light
#define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
//#define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with backlight
//#define NEWCOSMETIC_COMBO
#define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light

//#define MANU_CHECK

#define GRIPPER_STATISTICS

#undef  NUM_TLC_PC1    
#define NUM_TLC_PC1        8  // number of cameras connected to PC1
#define NUM_TLC_PC2        8  // number of cameras connected to PC2
#define NUM_TLC     (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2) // max number of cameras to connect and use
#define NUMTRIGGER  (NUM_TLC_PC1+NUM_TLC_PC2)                         // number of cameras/triggers to link to PLC

	static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
	// static UINT tlcgroupPC1[2][2] = { {0,2}, {3,6} }; // tip/cap - partic1
	// static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} }; // partic2 - partic3
	static UINT     tlcgroupPC1[3][2] = { { 0,2 },{ 3,6 },{ 7,7 } }; // tip/cap - partic1 - FO color
	static UINT     tlcgroupPC2[2][2] = { { 0,3 },{ 4,7 } }; // partic2 - cosmetic

	static wchar_t* tip_names[3] = { L"TC_A",    L"TC_B",    L"TC_C" };
	static char*    tip_serials[3] = { "23464392", "23418237", "23418223" };
	static wchar_t* foc_names[1] = { L"TC_D" };
	static char*    foc_serials[1] = { "23389783" };
	static wchar_t* part1_names[4] = { L"P1_A",    L"P1_B",    L"P1_C",    L"P1_D" };
	static char*    part1_serials[4] = { "23464390", "23418256", "23418226", "23418246" };
	static wchar_t* part2_names[4] = { L"P2_A",    L"P2_B",    L"P2_C",    L"P2_D" };
	static char*    part2_serials[4] = { "23416217", "23418222", "23418267", "23418262" };
	//static wchar_t* cosm_names   [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
	//static char*    cosm_serials [4] = { "23269434", "23269412", "23269454", "23269447" };
	static wchar_t* cosm_names[4] = { L"P3_A",    L"P3_B",    L"P3_C",    L"P3_D" };
	static char*    cosm_serials[4] = { "23418243", "23418230", "23464360", "23362862" };
#endif
	//Pour added 20201012



#ifndef STN1  // First  station defined for particle with bottom light
  #define STN1 PARTICLE_BACK    // First  station defined for particle with backlight
#endif

#ifndef STN2 // Default is bottomlight particle for second station
  #define STN2 PARTICLE_BOTTOM  // Second station defined for particle with bottom light
//  #define STN2 PARTICLE_BACK    // Second station defined for particle with backlight
#endif

#ifndef STN3
  #define STN3 COSMETIC_BACK  // Third  station defined for cosmetic with bbacklight
//  #define STN3 PARTICLE_BOTTOM  // Third  station defined for particle with bottom light
//  #define STN3 PARTICLE_BACK    // Third  station defined for particle with backlight
#endif


  // CHANGE VERSION FOR RECOMPILED WITH NEW PARTICLE
  #ifdef NEW_PARTICLE
    #undef VERSION_PC   // L"120.114" original
    #define VERSION_PC     L"120.214"
  #endif

  // CHANGE VERSION FOR RECOMPILED WITH NEW PARTICLE
  #ifdef OPENCV_PARTICLE
    #undef VERSION_PC   // L"120.114" original
    #define VERSION_PC     L"120.314"
  #endif

/*
  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define ROT90CCW_SCALE21
  #define BASLER_SDK

  #define NUMTRIGGER  15  // number of cameras/triggers to link to PLC

  #define NUM_TLC             8   // max number of cameras to connect and use
  
  #define APP_RESX         1280 // Horizontal resolution of application window
  #define APP_RESY         1024 // Vertical resolution of application window
  */
#endif // #ifdef TIVS_A40


#ifdef TIVS_F40

  #define MODEL_NAME        L"TIVS-F40"
  #define VERSION_PC        L"120.114"

  #define INPUTBELT_SENSOR // Id defined set input sensor delays

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1       10  // number of cameras connected to PC1
  #define NUM_TLC_PC2        8  // number of cameras connected to PC2
  #define NUM_TLC      (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2)  // max number of cameras to connect and use
  #define NUMTRIGGER   (NUM_TLC_PC1+NUM_TLC_PC2) //    15  // number of cameras/triggers to link to PLC
  #define APP_RESX        1280  // Horizontal resolution of application window
  #define APP_RESY        1024  // Vertical resolution of application window

  static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6,9} }; // cap - 2 bottoms+focolor - cosmetic
  static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} };        // sidecake - topcake

  #define LAYOUT_FILENAME L"layout_fd.png"


  #if SERIAL_N==201300003 // old 2013001 2013-001V - FIRST MACHINE AUSIA
    #define MACHINE_ID     "2013-00-003" // old "2013-001V"
    #define CUSTOMER_NAME L"杭州澳亚"
    #define CRCODE  0x1891 // 6289
    #define CRCAM   0x0DC2
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21371045", "21371066", "21371067"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21371041", "21371061", "21405227"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21374042", "21374473", "21372300", "21374043" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21372306", "21370165", "21370167", "21372323" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21370177", "21370184", "21370499", "21372297" };
  #endif

  #if SERIAL_N==201300004 // old 2013002 2013-002V - SECOND MACHINE AUSIA
    #define MACHINE_ID     "2013-00-004" // old "2013-002V"
    #define CUSTOMER_NAME L"杭州澳亚"
    #define CRCODE  0x1892 // 6290
    #define CRCAM   0xC1E7
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21370121", "21369698", "21369700"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21371078", "21371068", "21405226"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21368168", "21370505", "21370183", "21370175" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21370201", "21372299", "21370198", "21374050" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21370173", "21374047", "21374040", "21374054" };
  #endif

  #if SERIAL_N==201300005 // old 2013003 2013-003V - THIRD MACHINE AUSIA
    #define MACHINE_ID     "2013-00-005" // old "2013-003V"
    #define CUSTOMER_NAME L"杭州澳亚"
    #define CRCODE  0x1893 // 6291
    #define CRCAM   0xAE8E
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                }; // ABC cap
    static char*    seal_serials  [3] = { "21369690", "21364280", "21364267"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21371079", "21364278", "21404692"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21372291", "21374053", "21374061", "21372318" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21372298", "21372304", "21370511", "21372315" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21370190", "21372293", "21372286", "21370182" };
  #endif






// machine_004 (2nd feedback and lights fault).pro
// TIVSF40_FD_004 (2nd feedback and lighs fault and LAM faults).tsm
  #if SERIAL_N==201300006 // old 2013004 2013-004V - FOURTH MACHINE AUSIA (separate room)
    #define MACHINE_ID     "2013-00-006" // old "2013-004V"
    #define CUSTOMER_NAME L"杭州澳亚"
    #define CRCAM   0x0421 // 8598
    #define CRCODE  0x1894 // 6292
    #define GRIPPER_STATISTICS
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    // new controls definitions
    #define STN1 CAKESIDE_ARBACK    // 400 - Station defined for cake shrink and layer, backlight area scan
    #define STN3 CAKETOP_ARDIRECT   // 410 - Station defined for cake top skinning,  direct light area scan
    #define NOTHIRDSTN_FDPC2
    #undef  NUM_TLC_PC2    //  8     number of cameras connected to PC2
    #define NUM_TLC_PC2       12  // number of cameras connected to PC2
    #define NUM_TLC      (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2)  // max number of cameras to connect and use
    #define NUMTRIGGER   (NUM_TLC_PC1+NUM_TLC_PC2) //    22  // number of cameras/triggers to link to PLC


    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21369694", "21369704", "21370123"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21371028", "21371018", "21404691"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21374052", "21374041", "21374477", "21374048" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21374058", "21374480", "21372294", "21374465" };
    
    // OLD/ORIGINAL
    //static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    //static char*    topck_serials [4] = { "21370509", "21372303", "21370203", "21372296" }; // "21370197" /*21372296"*/ };


    static wchar_t* cakesd_names  [4] = { L"T3_A",    L"T3_B",    L"T3_C",    L"T3_D"    };
    static char*    cakesd_serials[4] = { "21605969", "21521110", "21691586", "21692658" };

    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21521114", "21692647", "21691597", "21691571" };


  #endif

  #if SERIAL_N==2014020 // "2014-020V" - old 2013009VB ZDB Pharmaceutical - ZBD Harbin - Heilongjiang 
    #define MACHINE_ID     "2014-020V"
    #define CUSTOMER_NAME L"哈尔滨珍宝岛药业"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    #define CRCAM  0x2806 
    #define CRCODE 0x5B7A // 23418
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21363246", "21371060", "21369702"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21371074", "21371073", "21424996"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21372279", "21372285", "21372311", "21374063" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21374466", "21374470", "21370176", "21374479" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21368161", "21370194", "21370174", "21372322" };
  #endif

	//Pour added 20170724
	  #if SERIAL_N==201400008 // "2017-010V" - old 2013009VB ZDB Pharmaceutical - ZBD Harbin - Heilongjiang-Switch to NuoWeiNuo
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40L"
    #define MACHINE_ID     "2014-020V"
    #define CUSTOMER_NAME L"辽宁诺维诺制药有限公司"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0x82B  //2091 Pour added 20170725
    #define CRCODE 0x5B7A //23418
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21369702", "21371060", "21363246"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21371074", "21371073", "21425006"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21420094", "21370197", "21426569", "21593609" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21370174", "21368161", "21372322", "21370194" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22161136", "22161129", "22159187", "22161125" };
  #endif
	//Pour added 20170724 the end

  #if SERIAL_N==201400020 // OLD 2014004 "2014-004V" LUYE PHARMA Yantai - bypass+pre-input belts and rotary table
    #define MACHINE_ID     "2014-00-020" // OLD "2014-004V" 
    #define CUSTOMER_NAME L"山东绿叶" // Luye Pharma
    #define CRCAM  0x8DEE 
    #define CRCODE 0x2199 // 8601 
    #define FALLEN_CONTAINER // If defined, use code to check fallen containers/set delay/enable alarm
    //#define POWDER_STOPPER   // If defined, test powder on rubber stopper (added 20160324 for improvement) //Pour removed 20171214
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21428865", "21428873", "21428891"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21428867", "21428858", "21418147"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21405741", "21402667", "21406135", "21404323" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21404325", "21402669", "21404322", "21405752" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21406110", "21404703", "21404700", "21406123" };
  #endif


  #if SERIAL_N==201400009 // OLD 2014005 201301002 2013-010VB - Factory FD updated 20140512 Luye Pharma
    #define MACHINE_ID     "2014-00-009" // OLD "2014-005V"
    #define CUSTOMER_NAME  L"山东绿叶"     // Luye Pharma
    #define CUSTOMER_NAMEE L"Luye Pharma"
    #define CRCAM  0xC91F 
    #define CRCODE 0x83AE                // 33710
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21370093", "21371043", "21370096"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21371022", "21370108", "21398383"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21372280", "21372295", "21372278", "21370178" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21400221", "21405186", "21405178", "21405184" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21374472", "21372284", "21372316", "21372288" };
  #endif

  #if SERIAL_N==201400010 // OLD 2014006  // New FD - second feedback - updated 20140522 Luye Pharma
    #define MACHINE_ID      "2014-00-010" // OLD "2014-006V"
    #define CUSTOMER_NAME  L"山东绿叶" // Luye Pharma
    #define CUSTOMER_NAMEE L"Luye Pharma"
    #define CRCAM  0x3C1C 
    #define CRCODE 0xA145  // 41285
    #define FD_DATAREC
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define GRIPPER_STATISTICS
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21428894", "21428747", "21428892"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21428862", "21428829", "21425015"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21416045", "21413197", "21413755", "21413751" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21416214", "21413762", "21416215", "21416055" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21404324", "21406124", "21416219", "21416216" }; 
  #endif

  // SECOND FEEDBACK - NEW NAME P60 - HEEL INSPECTION AND FASTER CAMERAS FOR CAP INSPECTION
  #if SERIAL_N==201400011 // OLD 2013006 "2013-006V" - New FD machine for Medisam of Herbin 20141030 then changed for Tianhuang Hainan Haikou
    // TIVSF40_FD_004 (2nd feedback and lighs fault and LAM faults)
    // machine_004 (2nd feedback and lights fault)
    #undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-P60"

    #define MACHINE_ID     "2014-00-011"   // "2013-006V"
    #define CUSTOMER_NAME L"海南双成"      // L"海南天煌制药有限公司"
    #define CRCAM  0x4C6C    // 78F0      // BF5C  // ABFC
//    #define CRCAM  0x0B5D  // 
    #define CRCODE 0x1A45    // 6725

    #define NEW_PASSW_MNGR  // New users/passwords manager

    //#define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
    //#define NEWCAP_ORALCAP
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    //#define DOUBLE_BOTTOM_ANALYSIS  // If defined, double alignment/analysis for KONRUS (experimental)

    //#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };

    static char*    seal_serials  [3] = { "21683531", "21680782", "21683544"             }; // new 780 cameras installed for new v30 cap
//    static char*    seal_serials  [3] = { "21751954", "21751953", "21751956"             }; // new 640@120 cameras
    //static char*    seal_serials  [3] = { "21736119", "21736120", "21736118"             }; // new 640@120 cameras
    //static char*    seal_serials  [3] = { "21404649", "21404659", "21404648"             }; // original
    //static char*    seal_serials  [3] = { "21404649", "21697619", "21404648"             }; // with Aca640-120gm

    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
//    static char*    bottom_serials[3] = { "21428879", "21428885", "21428730"             };
//    static char*    bottom_serials[3] = { "21428879", "21428885", "21521093"             };
    static char*    bottom_serials[3] = { "21428879", "21428885", "21422341"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21406113", "21406126", "21406114", "21404720" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21406112", "21404717", "21406127", "21405200" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21406130", "21406121", "21406118", "21406116" };  
  #endif


  #if SERIAL_N==201400016 // OLD 2014008 2014-008V (ex 2013-010YF) Bosen Xi'an
    #define MACHINE_ID     "2014-00-016" // OLD "2014-008V" "2013-010YF"
    #define CUSTOMER_NAME L"西安博森"
    #define CRCAM  0x4984 
    #define CRCODE 0x263A  // 9786
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21              // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING      // If defined, double ring analysis (experimental)
 //   #define DOUBLE_BOTTOM_ANALYSIS  // If defined, double alignment/analysis for KONRUS (experimental)

    #define TEST_BOSEN // enables new experimental crimping analysis

    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21405146", "21370102", "21404651"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21370125", "21370107", "21405225"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21372282", "21372287", "21372313", "21328429" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21374476", "21374469", "21374478", "21374468" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21372325", "21368164", "21372301", "21372308" };
  #endif

  // testing for new cap for tianhuang
  // machine_004 (2nd feedback and lights fault).pro
  // TIVSF40_FD_004 (2nd feedback and lighs fault and LAM faults).tsm
  #if SERIAL_N==201400017 // New FD machine 20140902 - Guandong (Guanzhou) GZYPH
    #define MACHINE_ID     "2014-00-017" // 
    #define CUSTOMER_NAME L"广州一品红"
    #define CRCODE  0x2414  // 9236
    #define CRCAM   0xE99B
//    #define NEWCAP_DOUBLE
//    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)
    #define GRIPPER_STATISTICS
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21499091", "21499097", "21499090"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21499056", "21491219", "21423954"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21405745", "21404320", "21405195", "21405759" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21403696", "21405756", "21405749", "21405737" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21406115", "21404707", "21406131", "21404705" };
  #endif

    /*
  #if SERIAL_N==201400018 // New FD machine 20140902
    #define MACHINE_ID  "2014-00-018" // 
    #define CRCODE 0x2482  // 9346
    //#define NEWCAP_DOUBLE
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21499099", "21499062", "21499072"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21499073", "21502507", "21423962"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21405754", "21405736", "21405758", "21405757" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21405747", "21403702", "21403694", "21405739" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21394088", "21370507", "21370501", "21404711" };
  #endif
  */


  #if SERIAL_N==201400022 // New FD machine first programming 20140812 - Yaopharma - recompiled 20141202 with PC2 to test heel
    // machine_004 (2nd feedback and lights fault).pro
    // TIVSF40_FD_004 (2nd feedback and lighs fault and LAM faults).tsm
    #define MACHINE_ID     "2014-00-022" 
    #define CUSTOMER_NAME L"重庆药友"
    #define CRCAM  0x44A6 
    #define CRCODE 0x6979 // 27001

	//Pour added 20181105
	//#define NEW_PASSW_MNGR
	//Pour added 20181105
    #define FD_DATAREC
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection
//    #define NOTHIRDSTN_FDPC2 // ONLY on YAOPHARMA no TOPCAKE on PC2
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21428875", "21428884", "21428838"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21499063", "21428723", "21422345"             };
//    static char*    bottom_serials[3] = { "21277561", "21428723", "21422345"             }; bottomA camera replaced for CQ exhibition on 20141023
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21416223", "21416064", "21413760", "21410898" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21403700", "21404713", "21405177", "21405189" };
//    static char*    sideck_serials[4] = { "21426572", "21416050", "21413760", "21426561" };
//    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static wchar_t* topck_names   [4] = { L"HL_A",    L"HL_B",    L"HL_C",    L"HL_D"    }; // renamed as HEEL CAMERAS
//    static char*    topck_serials [4] = { "21405197", "21404701", "21404702", "21405182" };
    static char*    topck_serials [4] = { "21403704", "21404706", "21403698", "21404715" };
  #endif

/*  #if SERIAL_N==201400023 // New FD machine first programming 20140812 - Yaopharma - single computer
    #define MACHINE_ID  "2014-00-023" 
    #define CRCODE 0x6982 // 27010
    //#define NEWCAP_DOUBLE
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21428843", "21428844", "21428853"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21502501", "21502502", "21422357"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21426577", "21426571", "21425313", "21414303" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21426572", "21416050", "21425313", "21426561" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21405197", "21404701", "21404702", "21405182" };
  #endif */


//Pour-modified-20170712
/*
  #if SERIAL_N==201400024 // New FD machine first programming 20140812 Konruns
    #define MACHINE_ID     "2014-00-024" 
    #define CUSTOMER_NAME L"北京康辰" // to be confirmed
    #define CRCAM  0x7784 
    #define CRCODE 0x42AC // 17068
    
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define DOUBLE_BOTTOM_ANALYSIS  // If defined, double alignment/analysis for KONRUS (experimental)
*/
//end-removed-20170712

	//HGA function adding.Pour-20170712
	//Pour changed to Qingdao Guoda 20190318
	#if SERIAL_N==201800029 // New FD machine first programming 20140812 Konruns
	#define MACHINE_ID     "2018-00-029" 
    #define CUSTOMER_NAME L"青岛国大"  // Konruns

    #undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40"

	#define CRCAM  0x3683 //13955
    #define CRCODE 0x42AC // 17068

    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	#define DOUBLE_BOTTOM_ANALYSIS  // If defined, double alignment/analysis for KONRUS (experimental)

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

	//End-20170712
    
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21428883", "21405137", "21404673"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21499096", "22534419", "21425013"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21372305", "21405190", "21404321", "21394075" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21426572", "21416050", "21413748", "21426561" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21405197", "21404701", "21404702", "21405182" };
  #endif


  #if SERIAL_N==201400013 // old 2014010 2014-010V - First Guorui - updated 20141112
    #define MACHINE_ID     "2014-00-013" // old "2014-010V" 
    #define CUSTOMER_NAME L"国药集团国瑞"
    #define CRCAM  0x41E9 
    #define CRCODE 0xA343 // 41795

    #define NEW_PASSW_MNGR  // New users/passwords manager - added and recompiled 20170605

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21          // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21371032", "21428722", "21428719"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21428721", "21428736", "21413187"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21416229", "21414305", "21416057", "21416218" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21416043", "21416065", "21416060", "21413209" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21404712", "21406128", "21406125", "21406119" };
  #endif

  #if SERIAL_N==201400014 // old 2014011 2013011 2013-011VB - Second Guorui - updated 20141112
    #define MACHINE_ID     "2014-00-014" // old "2014-011V"
    #define CUSTOMER_NAME L"国药集团国瑞"
    #define CRCAM  0xB2DB 
    #define CRCODE 0xA344 // 41796

    #define NEW_PASSW_MNGR  // New users/passwords manager - added and recompiled 20170605

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21364270", "21370091", "21371031"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21371072", "21370119", "21407700"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21396288", "21405194", "21403692", "21403697" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21404319", "21403701", "21405199", "21403703" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21374062", "21374467", "21368174", "21374471" };
  #endif

  #if SERIAL_N==201400015 // old 2014012 - Third Guorui - updated 20141112
    #define MACHINE_ID     "2014-00-015" // old "2014-012V" 
    #define CUSTOMER_NAME L"国药集团国瑞"
    #define CRCAM  0xC303 
    #define CRCODE 0xA345 // 41797

    #define NEW_PASSW_MNGR  // New users/passwords manager - added and recompiled 20170605

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21405129", "21371064", "21405164"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21371057", "21401192", "21422344"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21405742", "21405743", "21405192", "21405753" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21405750", "21405746", "21405740", "21405748" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21427738", "21406120", "21404708", "21404698" };
  #endif



  #if SERIAL_N==201400018 // Fourth Guorui - updated 20141112
    #define MACHINE_ID     "2014-00-018" 
    #define CUSTOMER_NAME L"国药集团国瑞"
    #define CRCAM  0x1D43 
    #define CRCODE 0xA346 // 41798

    #define NEW_PASSW_MNGR  // New users/passwords manager - added and recompiled 20170605

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21499076", "21499057", "21499072"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21499073", "21502507", "21423962"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21405754", "21405736", "21405758", "21405757" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21405747", "21403702", "21403694", "21405739" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21394088", "21370507", "21370501", "21404711" };
  #endif

  #if SERIAL_N==201400019 // Fifth Guorui - updated 20141112
    #define MACHINE_ID     "2014-00-019" 
    #define CUSTOMER_NAME L"国药集团国瑞"
    #define CRCAM  0xCC64 // E8E1 
    #define CRCODE 0xA347 // 41799

    #define NEW_PASSW_MNGR  // New users/passwords manager - added and recompiled 20170605

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21499084", "21499094", "21499099"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21499069", "21499068", "21425008"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21593627", "21405188", "21396286", "21406134" };
//    static char*    cosm_serials  [4] = { "21404316", "21405188", "21396286", "21406134" }; old cameras, CMA replaced 20160106
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21405751", "21405744", "21406136", "21402668" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21406111", "21405755", "21404699", "21405180" };
  #endif

  #if SERIAL_N==201400021 // Sixth Guorui - updated 20141112
    #define MACHINE_ID     "2014-00-021" 
    #define CUSTOMER_NAME L"国药集团国瑞"
    #define CRCAM  0xF953 
    #define CRCODE 0xA348 // 41800

    #define NEW_PASSW_MNGR  // New users/passwords manager - added and recompiled 20170605

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21491220", "21499089", "21499062"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21502500", "21499071", "21423953"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21406117", "21406133", "21404714", "21404719" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21404710", "21406137", "21404718", "21405179" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21405198", "21406139", "21406122", "21404709" };
  #endif


  #if SERIAL_N==201400048 // New FD machine first programming 20150323 IVS / plc last thurs - Single feedback prototype - Seventh Guorui
    #define MACHINE_ID     "2014-00-048" 
    //#define CUSTOMER_NAME L"" // to be confirmed
    #define CRCAM  0x0FDA
    #define CRCODE 0x2C0E // 11278

    #define NEW_PASSW_MNGR  // New users/passwords manager - added and recompiled 20170605

    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21520520", "21521117", "21521119"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21605974", "21605954", "21423956"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21427742", "21427711", "21572217", "21571405" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21572226", "21571404", "21572211", "21566954" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21571401", "21571415", "21427712", "21593623" };
  #endif



  #if SERIAL_N==201400049 // New FD machine first programming 20150323 IVS / plc last thurs - Single feedback - Eighth Guorui
    #define MACHINE_ID     "2014-00-049" 
    //#define CUSTOMER_NAME L"" // to be confirmed
    #define CRCAM  0x57EC
    #define CRCODE 0x0EDD // 3805

    #define NEW_PASSW_MNGR  // New users/passwords manager - added and recompiled 20170605

    #define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21521108", "21521107", "21520549"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21516388", "21520482", "21423957"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21404704", "21400222", "21403699", "21394089" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21405181", "21368176", "21593607", "21416059" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21404721", "21405191", "21378292", "21406132" };
  #endif


  #if SERIAL_N==201400053 // New FD machine 20150702 - Single feedback
    // machine_004 (single feedback and lights fault).pro
    // TIVSF40_FD_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
    #define MACHINE_ID     "2014-00-053" 
    #define CUSTOMER_NAME L"成都百裕金阁莱药业" // to be confirmed
    #define CRCAM  0x6A43
    #define CRCODE 0x39BD // 14781
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21601279", "21605989", "21605973"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21605956", "21605963", "21423951"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21427727", "21426567", "21426564", "21425317" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21426574", "21426558", "21425320", "21427716" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21426563", "21413210", "21427714", "21571422" };
  #endif




  // aggiornata il 20170327 per nuovo cliente insieme alla 2016-00-003
                          // test new AUSIA controls - test new crimping cap BOSEN
  #if SERIAL_N==201400054 // New FD machine 20150702 - Single feedback - Guandong Sunho 20170601
    // machine_004 (single feedback and lights fault).pro
    // old TIVSF40_FDHGA_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
    // TIVSF40_FDHGA_004 (single feedback - HGA datarec - ratio490).tsm dopo aggiornamento con schede agg x data logging
    #define MACHINE_ID     "2014-00-054" 
    #define CUSTOMER_NAME L"广东星昊"  // Guangdong Sunho Pharmaceutical Co.Ltd

    #undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40L"

    #define CRCAM  0x65F1 // 0xEF7E // 0x2119  // 0xEF7E
    #define CRCODE 0x39C6 // 14790
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output

	#define NEW_LOGGER      // uses new log encyrption and digital signature
	#define ALARMLOG_SEPARATE
    #define NEW_PASSW_MNGR  // New users/passwords manager

    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define TEST_BOSEN

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    //#define STN1 CAKESIDE_ARBACK    // 400 - Station defined for cake shrink and layer, backlight area scan
    //#define STN3 CAKETOP_ARDIRECT   // 410 - Station defined for cake top skinning,  direct light area scan
    //#define NOTHIRDSTN_FDPC2

    //static UINT tlcgroupPC1[3][2] = { {0,2}, {3,5}, {6, 9} }; // cap - 2 bottoms+focolor - cosmetic
    // DIFFERENT: AUSIA
    //static UINT tlcgroupPC2[3][2] = { {0,3}, {4,7}, {8,11} }; // sidecake lin - sidecake area - topcake area

    //#define NUM_TLC_PC1       10  // number of cameras connected to PC1
    //#undef  NUM_TLC_PC2    //  8     number of cameras connected to PC2
    //#define NUM_TLC_PC2       12  // number of cameras connected to PC2
    //#define NUM_TLC      (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2)  // max number of cameras to connect and use
    //#define NUMTRIGGER   (NUM_TLC_PC1+NUM_TLC_PC2) //    22  // number of cameras/triggers to link to PLC

    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21605944", "21605991", "21605945"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21605979", "21570749", "21423958"             };
//    static char*    bottom_serials[3] = { "21605979", "21605982", "21423958"             }; replaced 20170327
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21416231", "21566957", "21572231", "21573435" };
//    static char*    cosm_serials  [4] = { "21416231", "21572231", "21566957", "21573435" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21427726", "21593626", "21426560", "21427728" };

    //static wchar_t* cakesd_names  [4] = { L"T3_A",    L"T3_B",    L"T3_C",    L"T3_D"    };
    //static char*    cakesd_serials[4] = { "21428753", "21428728", "21428743", "21428857" };

    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    //static char*    topck_serials [4] = { "21405173", "21428870", "21521093", "21428831" }; // area scan used for testing
//    static char*    topck_serials [4] = { "21427739", "21593619", "21530219", "21416232" };
    static char*    topck_serials [4] = { "22001981", "22010662", "21998311", "22010640" };
  #endif


 // New FD machine 20150901 - Single feedback - Rongan - Ningbo vel aumentata a 600, fast cameras cap + aggiunto HGA 20160822

  #if SERIAL_N==201400055 // New FD machine 20150901 - Single feedback - 490 - INCREASED SPEED TO 600 - added HGA interface 20160822
    // machine_004 (single feedback and lights fault).pro
    // TIVSF40_FDHGA_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
    #define MACHINE_ID     "2014-00-055" //2014-00-055
    #define CUSTOMER_NAME L"宁波荣安" // Rongan Ningbo
    #undef  MODEL_NAME
    #define MODEL_NAME L"F60"
    #define CRCAM  0xEB8A // 0xA8
    #define CRCODE 0xC936 // 51510
    
    #define HGA_LEAKTEST            // HGA components installed

    #define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21736119", "21736120", "21736118"             };
//    static char*    seal_serials  [3] = { "21629475", "21627153", "21629453"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21629466", "21680751", "21422350"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21426576", "21426566", "21425319", "21427744" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21425318", "21426559", "21427746", "21422561" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21540931", "21538490", "21700013", "21540258" };
  #endif

	//Pour added 20180323 for QILU
	  #if SERIAL_N==201700019 // New FD machine 20150901 - Single feedback - 490 - INCREASED SPEED TO 600 - REMOVED HGA interface 20180323
    // machine_004 (single feedback and lights fault).pro
    // TIVSF40_FDHGA_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
    #define MACHINE_ID     "2020-00-020" //
    #define CUSTOMER_NAME L"合肥平光" // QILU
    #undef  MODEL_NAME
    #define MODEL_NAME L"F60"
    #define CRCAM  0xEB8A // 0xA8 //Pour added 20180323
    #define CRCODE 0xC936 // 51510
	//Pour added 20180323
	#define NEW_PASSW_MNGR  // New users/passwords manager
	#define HEEL_INSPECTION

	//Pour added 20180707
	//#define TEST_BOSEN
	#define TEST_QILU //Pour defined for additional area for QILU
	//#define NEWCAP_V30  //Pour added for testing for QILU
	//Pour added 20180707 
    
    //#define HGA_LEAKTEST            // HGA components installed //Pour removed 20180323

    #define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection //Pour removed 20170707 for testing
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21736119", "21736120", "21736118"             };
//    static char*    seal_serials  [3] = { "21629475", "21627153", "21629453"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21629466", "21680751", "21422350"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21426576", "21426566", "21425319", "21427744" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21425318", "21426559", "21427746", "21422561" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21540931", "21538490", "21700013", "21540258" };
  #endif
	//Pour added 20180323 for QILU

  #if SERIAL_N==201400056 // New FD machine 20150901 - Single feedback - 490 - Workshop - Test Huapong powder on rubber
    // machine_004 (single feedback and lights fault).pro
    // TIVSF40_FD_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
    #define MACHINE_ID     "2014-00-056" 
    #define CUSTOMER_NAME L"重庆华邦"
    #define CRCAM  0x1DA0
    #define CRCODE 0xC837 // 51255
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define POWDER_STOPPER // id defined, test powder on rubber stopper
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21680792", "21683522", "21680754"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21680761", "21603533", "21418142"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21422560", "21427717", "21700006", "21427696" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21427734", "21427741", "21422565", "21426570" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21537482", "21540917", "21540921", "21427747" };
  #endif

  #if SERIAL_N==201500009 // New FD machine 20151113 - Single feedback - 490 - VEM Turkey - Alarm log + safety screw switch
    // machine_004 (single feedback and lights fault).pro
    // TIVSF40_FD_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
    #define MACHINE_ID     "2015-00-009" 
    #define CUSTOMER_NAME L"VEM ILAC"
    #define CRCAM  0xC4DD
    #define CRCODE 0xC7E7 // 51175
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define ALARMLOG_SEPARATE // log alarms separately and add button to show them

    // 20160823 compiled and sent with new cap v30 from tianhuang
//    #define NEWCAP_DOUBLE
//    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define NEWCAP_V30       // add new functions for V3.0 new_cap inspection ( Tianhuang)


    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21520521", "21521122", "21405147"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21567191", "21370112", "21425010"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21594603", "21592620", "21592613", "21589525" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21538486", "21538482", "21538498", "21427708" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21416232", "21530219", "21593619", "21427739" };
  #endif

  #if SERIAL_N==201500010 // New FD machine 20151113 - Single feedback - 490
    // machine_004 (single feedback and lights fault).pro
    // TIVSF40_FD_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
    #define MACHINE_ID     "2015-00-010" 
//    #define CUSTOMER_NAME L"重庆华邦"
    #define CRCAM  0x2250
    #define CRCODE 0xC7D3 // 51155
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21692680", "21691582", "21691592"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21691576", "21691580", "21418143"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21538484", "21689304", "21540255", "21540267" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21689346", "21592604", "21593612", "21594606" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21590771", "21592612", "21593620", "21593614" };
  #endif

  #if SERIAL_N==201500011 // New FD machine 20160321 - Single feedback - 490 - Shandong Luye
    // machine_004 (single feedback and lights fault).pro
    // TIVSF40_FD_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
    #define MACHINE_ID     "2015-00-011" 
    #define CUSTOMER_NAME L"山东绿叶"
    #define CRCAM  0x267E // 0875
    #define CRCODE 0xBD78 // 48504
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21              // Add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING      // If defined, double ring analysis (experimental)
    #define DOUBLE_BOTTOM_ANALYSIS  // If defined, double alignment/analysis for KONRUS (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21629426", "21627145", "21570778"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21681464", "21692681", "21791249"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21374479", "21416230", "21593629", "21416224" };
//    static char*    cosm_serials  [4] = { "21374479", "21426230", "21593629", "21416224" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21592617", "21374466", "21540248", "21537479" };
//    static char*    sideck_serials[4] = { "21592617", "21427755", "21540248", "21537479" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21538475", "21574489", "21573439", "21538488" };
  #endif



  #if SERIAL_N==201500012 // New FD machine 20160316 - Single feedback - 490 - Shandong Luye
    // machine_004 (single feedback and lights fault).pro
    // TIVSF40_FD_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
    #define MACHINE_ID     "2015-00-012" 
    #define CUSTOMER_NAME L"山东绿叶"
    #define CRCAM  0xD6CF // C663
    #define CRCODE 0xB7D8 // 47064
    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21629444", "21629424", "21691599"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21691573", "21629483", "21422339"             };
//    static char*    bottom_serials[3] = { "21691573", "21629483", "21418143"             }; TCD sostit 20160628
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21372320", "21372290", "21372309", "21372283" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21328449", "21328440", "21328437", "21328435" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21592621", "21592614", "21370176", "21540254" };
  #endif

// NEW WITH HGA
// TIVSF40_FDHGA_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
// machine_004 (single feedback and lights fault).pro
  #if SERIAL_N==201500013 // New FD machine 20170119 - Single feedback - 490 - INCREASED SPEED TO 600 
    // machine_004 (single feedback and lights fault).pro 
    // TIVSF40_FD_004 (single feedback and lighs fault and LAM faults - ratio490).tsm
    #define MACHINE_ID     "2015-00-013" 
    #define CUSTOMER_NAME L"浙江新昌" // Zhejiang Xinchang
    #undef  MODEL_NAME
    #define MODEL_NAME L"F60"
    #define CRCAM  0x34A7
    #define CRCODE 0xCA3A // 51770
    
    #define NEW_PASSW_MNGR  // New users/passwords manager

//    #define NEW_LOGGER // uses new log encyrption and digital signature


    #define HGA_LEAKTEST    // HGA components installed - Aggiunto 20170303 per prove in officina su macchina modificata

    #define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)

    //#define VACUUM_PUMP // if defined add button connected to output 6th board/4th output
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
    #define GRIPPER_STATISTICS
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21751956", "21751954", "21751953"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21629452", "21629413", "21422364"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "21404316", "21406129", "21912243", "21923576" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21923572", "21923577", "21922590", "21923594" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21923581", "21923563", "21912265", "21923579" };
  #endif


		//Pour added 20171214
	  #if SERIAL_N==201800009 // "2017-009V" - GANSU Changee
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40"
    #define MACHINE_ID     "2017-009V"
    #define CUSTOMER_NAME L"甘肃成纪"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0x40F4 //61235 //Pour added 20171214  //16628-40F4
    //#define CRCODE 0xEF33 //61235 //Pour added 20171214
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN

	//Pour added for rejection separation 20180925
	#define REJECTION_SEPARATION
	//Pour added for rejection separation 20180925

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "22389124", "22389116", "22389127"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21521114", "21691586", "22390043"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22346786", "22346690", "22346709", "22346762" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "22346702", "22346711", "22346801", "22346757" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22317767", "22161143", "22159192", "22306238" };
  #endif
	//Pour added 20171214 the end

			//Pour added 20180116
	  #if SERIAL_N==201700012 // "2017-025V" 
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40"
    #define MACHINE_ID     "2017-025V"
    #define CUSTOMER_NAME L"上海凯茂"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0x6CB7 //27831 //Pour added 20180116
    //#define CRCODE 0xEF33 //61235 //Pour added 20171214
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN

	#define AUTORECONNECT_CAMERA  //Pour added for testing 20180320

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21369694", "21321484", "21370123"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21371018", "21371028", "22390037"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22317765", "22309031", "22306274", "22306284" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21372294", "21374480", "21374058", "22309026" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22308968", "22319520", "22306248", "22317771" };
  #endif
	//Pour added 20180116 the end

	//Pour added 20180702
	#if SERIAL_N==201700008 // "2017-018V" - AnKang Beiyida
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40"
    #define MACHINE_ID     "2018-018V"
    #define CUSTOMER_NAME L"安康北医大"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0xB3F5 //46069 //Pour added 20180702
    //#define CRCODE 0xEF33 //61235 //Pour added 20171214
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN

	//#define AUTORECONNECT_CAMERA  //Pour added for testing 20180320

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "22653181", "22647539", "22653182"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "22653180", "22653201", "22390055"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22624602", "22540938", "22547124", "22624590" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "22624574", "22628507", "22628513", "22624596" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22628508", "22628493", "22628506", "22547137" };
  #endif
	//Pour added 20180702 the end


		//Pour added 20180514
	#if SERIAL_N==201800004 // "2017-010V" - AnKang Beiyida
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40"
    #define MACHINE_ID     "2018-004V"
    #define CUSTOMER_NAME L"陕西博森"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0x3B63 //15203 //Pour added 20180514
    //#define CRCODE 0xEF33 //61235 //Pour added 20171214
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN
	#define TEST_QILU //Pour defined for additional area

	#define AUTORECONNECT_CAMERA  //Pour added for testing 20180320

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "22534487", "22534458", "22534412"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "22534420", "22534432", "22443262"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22554387", "22593470", "22593480", "21592606" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "22306217", "22306261", "22306250", "22306265" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22306214", "22306216", "22306207", "22513886" };
  #endif
	//Pour added 20180514 the end

		//Pour added 20190128
	#if SERIAL_N == 201800001 // "2018-000V" - beihuo
	#undef  MODEL_NAME    //    L"TIVS-F60"
    #define MODEL_NAME    L"TIVS-F60"
    #define MACHINE_ID     "2017-027V"
    #define CUSTOMER_NAME L"沈阳新马"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	//#define CRCAM  0xEF33 //61235 //Pour added 20171214 removed for color testing 20180112
	#define CRCAM  0x2367 //40F4 //9951 //Pour added 20180112 //16628
    //#define CRCODE 0xEF33 //61235 //Pour added 20171214
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR  // New users/passwords manager
	#define USE_ACA640_TIP
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	//#define MANU_CHECK
    //#define REJECTION_SEPARATION
	#define TEST_QILU //Pour defined for additional area


	//Pour added for testing 20180111
	//#define COLOR_DETECT
	//#define FO_COLOR  //Pour added 20180206
	//Pour added for testing 20180111

    //#define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21751957", "21973941", "21995336"             };
    //static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    //static char*    bottom_serials[3] = { "21499091", "21627146", "22390043"             };
	//Pour changed for color difference detection testing 20180112
	static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "22534418", "22401401", "22390044"             };  //"22443250", "22401181",
	//Pour changed for color difference detection testing 20180112
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22593486", "22592267", "22306221", "22593477" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "22554381", "22551315", "22551321", "22551317" };
    static wchar_t* topck_names   [4] = { L"HC_A",    L"HC_B",    L"HC_C",    L"HC_D"    };
    static char*    topck_serials [4] = { "22551338", "22554386", "22551330", "22551332" };
  #endif
	//Pour added 20190128 the end


	//Pour added 20181201
	#if SERIAL_N==201800024 // "2018-023V" - HaiNan Beite
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F60"
    #define MACHINE_ID     "2018-023V"
    #define CUSTOMER_NAME L"海南倍特"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0xFDFC //65020 //Pour added 20180514
    //#define CRCODE 0xEF33 //61235 //Pour added 20171214

	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN
	//#define TEST_QILU //Pour defined for additional area

	//#define AUTORECONNECT_CAMERA  //Pour added for testing 20180320

    //#define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "22796291", "22796256", "22796292"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "22581649", "22581642", "21423946"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22667688", "22558533", "22667672", "22612274" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "22668736", "22616523", "22612281", "22612254" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22816594", "22816581", "22822361", "22822333" };
  #endif
	//Pour added 20181201 the end


	//Pour added 20181218
	#if SERIAL_N==201800021 // "2018-018V" - Chongqing Yaoyou
	#undef  MODEL_NAME    //    L"TIVS-F60"
    #define MODEL_NAME    L"TIVS-F60"
    #define MACHINE_ID     "2018-018V"
    #define CUSTOMER_NAME L"重庆药友"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0xDBA //3514 //Pour added 20181218
    //#define CRCODE 0xEF33 //61235 //Pour added 20171214
	#define FD_DATAREC //Pour added for closed loop data record 20190213
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN
	//#define TEST_QILU //Pour defined for additional area

	//#define AUTORECONNECT_CAMERA  //Pour added for testing 20180320

    //#define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "22879803", "22879823", "22844218"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "22584240", "22597071", "22627223"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22822371", "22828701", "22828728", "22822363" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "22667687", "21592620", "22616524", "22612260" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22820233", "22815458", "22816597", "22822332" };
  #endif
	//Pour added 20181218 the end



		//Pour added 20180118
	  #if SERIAL_N==201700013 // "2017-012Vc" - old HUAHAO ZHONGTIAN
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40L"
    #define MACHINE_ID     "2017-012Vc"
    #define CUSTOMER_NAME L"Aurobindo Pharma"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0xD10  //3344 Pour added 20180118
    #define CRCODE 0x5B7A //23418
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "22346068", "22389125", "22389130"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "21370112", "21567191", "21425002"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22346688", "22306240", "22346773", "22346704" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "21538486", "21538482", "21538498", "21427708" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "21427739", "21530219", "21416232", "21593619" };
  #endif
	//Pour added 20180118 the end


	//Pour added 20190213
	#if SERIAL_N==201800027 // "2017-012Vc" - Changchun Zhuoyi
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40L"
    #define MACHINE_ID     "2018-041V"
    #define CUSTOMER_NAME L"卓谊生物"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0x462F  //17967
    #define CRCODE 0x5B7A //23418
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN
	//Pour added 20190328
	#define NEW_LOGGER
	//#define ALARMLOG_SEPERATE
	#define ALARMLOG_SEPARATE
	//Pour added 20190328

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "21371082", "21370105", "21369695"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "22835001", "22671119", "22788606"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22868338", "22868343", "22860044", "22860041" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "22868340", "22868345", "22860040", "22860039" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22860035", "22865028", "22865027", "22860042" };
  #endif
	//Pour added 20190213 the end

		//Pour added 20190410
	#if SERIAL_N==201900001 // "2019-001V" - QianShan Weikang
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40L"
    #define MACHINE_ID     "2019-001V"
    #define CUSTOMER_NAME L"潜山卫康"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0xD9C2  //55746
    #define CRCODE 0x5B7A //23418
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN
	//Pour added 20190328
	#define NEW_LOGGER
	//#define ALARMLOG_SEPERATE
	#define ALARMLOG_SEPARATE
	//Pour added 20190328

    #define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "22915984", "22915981", "22915985"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "22534490", "22581639", "22967163"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22945623", "22945583", "22949411", "22946852" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "22954017", "22949397", "22944845", "22954008" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22946679", "22945595", "22954000", "22945594" };
  #endif
	//Pour added 20190410 the end


	//Pour added 20200801
	#if SERIAL_N==202000015 // "2019-015V" - WUHAN RENFU
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40L"
    #define MACHINE_ID     "2020-015V"
    #define CUSTOMER_NAME L"武汉人福"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0x63D4 //25556
    #define CRCODE 0x5B7A //23418
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	//#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN
	//Pour added 20190328
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	//Pour added 20190328

    //#define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "23376247", "23376252", "23370889"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "22584227", "22581621", "22967161"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22945587", "22945582", "22945620", "22937135" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "22953999", "22937124", "22945604", "22953983" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22953984", "22945616", "22874691", "22874687" };
  #endif
	//Pour added 20200801 the end


	//Pour added 20200801
	#if SERIAL_N==202000016 // "2020-016V" -WUHAN RENFU
	#undef  MODEL_NAME    //    L"TIVS-F40"
    #define MODEL_NAME    L"TIVS-F40L"
    #define MACHINE_ID     "2020-016V"
    #define CUSTOMER_NAME L"武汉人福"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0x8D6E  //36206
    #define CRCODE 0x5B7A //23418
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)
	//#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN
	//Pour added 20190328
	#define NEW_LOGGER
	#define ALARMLOG_SEPARATE
	//Pour added 20190328

    //#define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "23370894", "23362868", "23370906"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "22534490", "22581639", "22967163"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "22945623", "22945583", "22949411", "22946852" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "22954017", "22949397", "22944845", "22954008" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "22946679", "22945595", "22954000", "22945594" };
  #endif
	//Pour added 20200801 the end



			//Pour added 20200520
	#if SERIAL_N==201900025 // "2019-025V" -ShangHai shengwusuo
	#undef  MODEL_NAME    //    L"TIVS-F40L"
    #define MODEL_NAME    L"TIVS-F40L"
    #define MACHINE_ID     "2019-025V"
    #define CUSTOMER_NAME L"上海生物所"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0x6CB8  //27832
    #define CRCODE 0x5B7A //23418
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

	#define TEST_QILU //Pour added for testing on 20200515
	#define NECK_CRACK //Pour added for testing on 20200515

	//#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN
	//Pour added 20190328
	#define NEW_LOGGER
	//#define ALARMLOG_SEPERATE
	#define ALARMLOG_SEPARATE
	//Pour added 20190328
	#define UPPER_COSMETIC
    //#define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "22653205", "23352398", "23352164"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "23051331", "22649372", "22627216"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "23122385", "22822377", "23240185", "23227498" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "23039282", "22957857", "22945579", "22798577" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "23200086", "22945610", "23118145", "23240183" };
  #endif
	//Pour added 20190520 the end

				//Pour added 20200907
	#if SERIAL_N==201900023 // "2019-023V" -ShanDong Yidu
	#undef  MODEL_NAME    //    L"TIVS-F40L"
    #define MODEL_NAME    L"TIVS-F40L"
    #define MACHINE_ID     "2019-023V"
    #define CUSTOMER_NAME L"山东亦度"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0xF060  //61536
    #define CRCODE 0x5B7A //23418
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

	#define TEST_QILU //Pour added for testing on 20200515
	#define NECK_CRACK //Pour added for testing on 20200515
	#define MANU_CHECK
	//#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN
	//Pour added 20190328
	#define NEW_LOGGER
	//#define ALARMLOG_SEPERATE
	#define ALARMLOG_SEPARATE
	//Pour added 20190328
	#define UPPER_COSMETIC
    //#define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    #define HGA_LEAKTEST        // HGA components installed
    #define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "23370941", "23418224", "23416214"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "23376263", "23370934", "23366105"             };
    static wchar_t* cosm_names    [4] = { L"CM_A",    L"CM_B",    L"CM_C",    L"CM_D"    };
    static char*    cosm_serials  [4] = { "23393376", "23393375", "23368588", "23393387" };
    static wchar_t* sideck_names  [4] = { L"SC_A",    L"SC_B",    L"SC_C",    L"SC_D"    };
    static char*    sideck_serials[4] = { "23369664", "23393374", "23388074", "23393381" };
    static wchar_t* topck_names   [4] = { L"TC_A",    L"TC_B",    L"TC_C",    L"TC_D"    };
    static char*    topck_serials [4] = { "23138657", "23393385", "23393378", "23420067" };
  #endif
	//Pour added 20200907




#endif // #ifdef TIVS_F40

	//Pour 20200812
	#ifdef TIVS_F20

  #define MODEL_NAME        L"TIVS-F20"
  #define VERSION_PC        L"120.114"

  #define INPUTBELT_SENSOR // Id defined set input sensor delays

  #define BECKHOFF              // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
  #define IMAGE_DISPLAY
  #define BASLER_SDK
  #define NUM_TLC_PC1       12  // number of cameras connected to PC1
  #define NUM_TLC_PC2        0  // number of cameras connected to PC2
  #define NUM_TLC      (NUM_TLC_PC1>NUM_TLC_PC2?NUM_TLC_PC1:NUM_TLC_PC2)  // max number of cameras to connect and use
  #define NUMTRIGGER   (NUM_TLC_PC1+NUM_TLC_PC2 + 4) //    12  // number of cameras/triggers to link to PLC //Pour added +4 on 20201011
  #define APP_RESX        1280  // Horizontal resolution of application window
  #define APP_RESY        1024  // Vertical resolution of application window

  static UINT tlcgroupPC1[5][2] = { {0,2}, {3,5}, {6,7}, {8,9}, {10,11} }; // cap - 2 bottoms+focolor -cosmetic-sidecake- topcake
  static UINT tlcgroupPC2[2][2] = { {0,3}, {4,7} };        // sidecake - topcake

  #define LAYOUT_FILENAME L"layout_fd.png"


	//Pour added 20200812
	#if SERIAL_N==202000018 // "2020-010Vc" -Auroubindo
	#undef  MODEL_NAME    //    L"TIVS-F20"
    #define MODEL_NAME    L"TIVS-F20"
    #define MACHINE_ID     "2020-010Vc"
    #define CUSTOMER_NAME L"Aurobindo"
    static BYTE PLC_ADDRESS[6] = { 5 , 19 , 40 , 80 , 1 , 1 };
    //#define CAP_KEYPOINTS6
    //#define CRCAM  0x2806 
	#define CRCAM  0x507B //20603
    #define CRCODE 0x5B7A //23418
    #define NEWCAP_DOUBLE
    #define NEWCAP_V21     // add new functions for V2.1 new_cap inspection
    #define DOUBLE_BOTTOM_RING  // If defined, double ring analysis (experimental)

	#define TEST_QILU //Pour added for testing on 20200515
	//#define NECK_CRACK //Pour added for testing on 20200515

	//#define USE_ACA640_TIP  // use Aca640-120gm for double cap (120fps)
    #define NEW_PASSW_MNGR  // New users/passwords manager
    //#define TEST_BOSEN
	//Pour added 20190328
	#define NEW_LOGGER
	//#define ALARMLOG_SEPERATE
	#define ALARMLOG_SEPARATE
	//Pour added 20190328
	//#define UPPER_COSMETIC
    //#define HEEL_INSPECTION     // If defined HEEL texts are used in place of TOP-CAKE inspection

    //#define HGA_LEAKTEST        // HGA components installed
    //#define HGA_DATAREC         // enable option to record HGA data

    static wchar_t* seal_names    [3] = { L"TC_A",    L"TC_B",    L"TC_C"                };
    static char*    seal_serials  [3] = { "23352148", "23352212", "23352140"             };
    static wchar_t* bottom_names  [3] = { L"BC_A",    L"BC_B",    L"TC_D"                };
    static char*    bottom_serials[3] = { "23352121", "23352129", "23389785"             };
    static wchar_t* cosm_names    [2] = { L"CM_A",    L"CM_B"    };
    static char*    cosm_serials  [2] = { "23269452", "23269436" };
    static wchar_t* sideck_names  [2] = { L"SC_A",    L"SC_B"    };
    static char*    sideck_serials[2] = { "23269453", "23269448" };
    static wchar_t* topck_names   [2] = { L"TC_A",    L"TC_B"    };
    static char*    topck_serials [2] = { "23269444", "23269411" };
  #endif
	//Pour added 20200812



	#endif // #ifdef TIVS_F20
//Pour 20200812


#define PARTICDIMX         768          // horizontal particle resolution (mod16)
#define PARTICDIMY         400          // vertical particle resolution (mod16)


// COLORREF value has the following hexadecimal form: 0x00bbggrr

#define BGR(r,g,b) (0xFF<24)|((r)<<16)|((g)<<8)|(b)

// GENERAL APPEARANCE COLORS
//#define BKGRND_COL         RGB(142,138,130) // returns in COLORREF format
//#define COLORE_SFONDO      0x00333444   // RGB(

// TEXT SHOWING COLORS TXT_xxxxxxx (BBGGRR)
/*#define GRN_COL            0x0096F398   // RGB(152,243,150)
#define YEL_COL            0x003EEFFC   // RGB(252,239, 62)
#define RED_COL            0x003C46FF   // RGB(255, 70, 60)
#define DARK_RED           0x000A0EB8   // RGB(184, 14, 10)
#define DARK_GREEN         0x00007900   // RGB(  0,121,  0)
#define DARK_BLUE          0x00C00000   // RGB(  0,  0, 12) */

#define TXT_BLACK          0x00000000   // RGB(  0,  0,  0)
#define TXT_GREEN          0x0096F398   // RGB(152,243,150)
#define TXT_YELLOW         0x003EEFFC   // RGB(252,239, 62)
#define TXT_RED            0x003C46FF   // RGB(255, 70, 60)
#define TXT_DARKRED        0x000A0EB8   // RGB(184, 14, 10)
#define TXT_DARKORANGE     0x000D7AF2   // RGB(242,122, 13)
#define TXT_DARKGREEN      0x00007900   // RGB(  0,121,  0)
#define TXT_DARKBLUE       0x00C00000   // RGB(  0,  0, 12)




// DISPLAY COLORS COL_xxxxxxx (AARRGGBB)
#define COL_WHITE          0xFFFFFFFF   // BGR(255,255,255)
#define COL_BLACK          0xFF000000   // BGR(  0,  0,  0)

#define COL_RED            0xFFFF0000   // BGR(255,  0,  0)
#define COL_GREEN          0xFF00FF00   // BGR(  0,255,  0)
#define COL_BLUE           0xFF0000FF   // BGR(  0,  0,255)
#define COL_DARKGREEN      0xFF007900   // RGB(  0,121,  0)

#define COL_CYAN           0xFF00FFFF   // BGR(  0,255,255)
#define COL_MAGENTA        0xFFFF00FF   // BGR(255,  0,255)
#define COL_YELLOW         0xFFFFFF00   // BGR(255,255,  0)
#define COL_ORANGE         0xFFFF7700   // BGR(255,119,  0)
#define COL_LIGHTORANGE    0xFFFD9F1A   // BGR(253,159, 26)
#define COL_PURPLE         0xFFA900D4   // BGR(169,  0,212)

#define IS_PC1 !wcsncmp( PC_name, L"PC1", 3 )
#define IS_PC2 !wcsncmp( PC_name, L"PC2", 3 )
 



/*
  #ifndef BASLER_SDK
    // PC1
    #if PC_num==1
      #define DIMX_ACQ   1360 // Larghezza di acquisizione da telecamera (scala ca 0,1mm/pix)
      #define DIMY_ACQ   1024 // Altezza di acquisizione da telecamera   (scala ca 0,1mm/pix)
      #define F_ACQ_DSP  0.4 // Fattore di scalatura per la visualizzazione
    #endif

    #if PC_num==2
      // Dimensioni di acquisizione (bicchiere max diam=84/sviluppo=264 - H=ca 100)
      #define DIMX_ACQ  1280 // Larghezza di acquisizione da telecamera (scala ca 0,1mm/pix)
      #define DIMY_ACQ   960 // Altezza di acquisizione da telecamera   (scala ca 0,1mm/pix)
      #define F_ACQ_DSP  0.4 // Fattore di scalatura per la visualizzazione
    #endif

  #endif
  */

  //#define BECKHOFF           // Se definito viene attivata la comunicazione con Beckhoff tramite ADS
 

  #define TS_TLC_OK        1 // Alto quando la TLC è stata inizializzata correttamente
  #define TS_FRAME      1024 // Acquisisci e visualizza singolo frame
  #define TS_MOVIE      2048 // 1 quando il thread del filmato è in funzione
  #define TS_INSPECT    4096 // 1 quando il thread di ispezione è in funzione
  #define TS_RUNNING    8192 // viene abbassato dopo l'uscita dei thread di ispezione/filmato
  #define TS_EXIT      16384 // Comando di uscita valido per tutti i thread


  //#define BACK_LIGHT
  #define LOAD_IMAGE      //caricamento dell'immagine da disco, non acquisizione da TLC

//#endif // BANCO BPROVA



/*
#ifdef FILLEV
  #undef  DIMX_ACQ
  #undef  DIMY_ACQ
  #undef  F_ACQ_DSP
  #define DIMX_ACQ    780   // Larghezza di acquisizione da telecamera (scala ca 0,1mm/pix)
  #define DIMY_ACQ    580   // Altezza di acquisizione da telecamera   (scala ca 0,1mm/pix)
  #define F_ACQ_DSP   0.2   // Fattore di scalatura per la visualizzazione
#endif
  */


// OLD
/*
#define MAIN_BTN_FONT     120, L"DejaVu Sans Condensed"   // font used for text into buttons on main screen
#define TEXT_ROWS_FONT     90, L"DejaVu Sans Condensed"   // font used for text into text rows below cameras
#define ALM_WRN_FONT      160, L"DejaVu Sans Condensed"   // font used for text for alarms/warnings
#define TEXT_LAYOUT_FONT  190, L"DejaVu Sans Condensed Bold Oblique"   // font used for text into layout
*/

//
//#define MAIN_BTN_FONT      12, L"Arial"   // font used for text into buttons on main screen
#define TEXT_ROWS_FONT_ENG  15, L"Arial"   // font used for text into text rows below cameras
#define TEXT_ROWS_FONT_CHN  13, L"Arial"   // font used for text into text rows below cameras
#define ALM_WRN_FONT        27, L"Arial"   // font used for text for alarms/warnings
#define TEXT_LAYOUT_FONT    22, L"Arial"   // font used for text into layout




//#define PDF_REPORT_FONT   "DejaVu Sans Condensed"         // font used for text for alarms/warnings
//#define PDF_REPORT_FONT   "Helvetica"         // font used for text for alarms/warnings

#ifndef CUSTOMER_NAME
  #define CUSTOMER_NAME L"N/A"
#endif

#ifndef CUSTOMER_NAMEE
  #define CUSTOMER_NAMEE L"N/A"
#endif

#if !defined PARTFLT_H6V3 && !defined PARTFLT_H8V4
  #define PARTFLT_H4V1
#endif


#ifndef NUM_TLC 
  #define NUM_TLC 0 // Numero di telecamere attese 
#endif

/*
#ifndef NUM_IMG
  #define NUM_IMG 0
  #define BECKHOFF 
  #define COLORE_CONTAIN  0x00C8D0D4
  #define COLORE_SFONDO   0x00888888
  #define APP_RESX  1280 
  #define APP_RESY  1024 
#endif
  */

//#define KYCD 0x0001
#define ESC_PUSHED ( GetAsyncKeyState( VK_ESCAPE ) & 1 ) // Vero se il tasto è stato premuto almeno una volta dalla chiamata precedente
#define F1_PUSHED  ( GetAsyncKeyState( VK_F1     ) & 1 ) 
#define F2_PUSHED  ( GetAsyncKeyState( VK_F2     ) & 1 ) 
#define F3_PUSHED  ( GetAsyncKeyState( VK_F3     ) & 1 ) 
#define F4_PUSHED  ( GetAsyncKeyState( VK_F4     ) & 1 ) 
#define F5_PUSHED  ( GetAsyncKeyState( VK_F5     ) & 1 ) 
#define F6_PUSHED  ( GetAsyncKeyState( VK_F6     ) & 1 ) 
#define F7_PUSHED  ( GetAsyncKeyState( VK_F7     ) & 1 ) 
#define F8_PUSHED  ( GetAsyncKeyState( VK_F8     ) & 1 ) 
#define F9_PUSHED  ( GetAsyncKeyState( VK_F9     ) & 1 ) 
#define F10_PUSHED ( GetAsyncKeyState( VK_F10    ) & 1 ) 
#define F11_PUSHED ( GetAsyncKeyState( VK_F11    ) & 1 ) 
#define F12_PUSHED ( GetAsyncKeyState( VK_F12    ) & 1 ) 
#define Fxx_PUSHED(i) ( GetAsyncKeyState(ind+VK_F1) & 1 ) // ATT: i MUST BE >=0 and <= 11 !!
//#undef KYCD 



#define B00        1
#define B01        2
#define B02        4
#define B03        8
#define B04       16
#define B05       32
#define B06       64
#define B07      128
#define B08      256
#define B09      512
#define B10     1024
#define B11     2048
#define B12     4096
#define B13     8192
#define B14    16384
#define B15    32728
#define B16    65536
#define B17   131072
#define B18   262144
#define B19   524288
#define B20  1048576




// conversione con arrotondamento corretto anche in caso di valori negativi
inline int DOUBLETOINT(double x)
{
  return int(x>0.0?x+0.5:x-0.5);
}

// conversione con arrotondamento corretto anche in caso di valori negativi
inline int DOUBLETOSHORT(double x)
{
  return short(x>0.0?x+0.5:x-0.5);
}



// function to return round(minimum) distance between to int angles in 1/100 of degrees (0..36000)
inline int ROUNDIST( int a1, int a2 )
{
  int d1 = abs(a1-a2);
  if ( d1<36000 ) return( d1 );
  return( 360000-d1 );
}



#define PIGRECO 3.1415926536
#define RAD2DEG 180.0/PIGRECO // coeff to convert radians into degrees
#define DEG2RAD PIGRECO/180.0 // coeff to convert degrees into radians

#define CHNGCTRLNAME( ID, STRING )  if ( GetDlgItem(ID) && GetDlgItem(ID)->m_hWnd ) GetDlgItem(ID)->SetWindowText( STRING )

#define INT2VDSP(i) 50.0*i/32767.0
#define INT2ADSP(i) 20.0*i/32767.0


//#define MAX_BOTTLEHEIGHT_MISSES  10

#define MAXNUMMIN  20
#define MAXNUMMAX  20


#define MAX_TEXT_LEN 100

/*
struct minmax_type
{
	double    val;      // = 0.0 value of minimum/maximum in mm
	int       ind;      // = 0   index into results data array of min/max
};
*/



  struct serie_type_double
  {
    double *data; // contains all data
    UINT    dim;  // number of data 
  };

  struct serie_type_int
  {
    int    *data; // contains all data
    UINT    dim;  // number of data 
  };


  /*

// MACROs TO SET, CLEAR AND TEST A SINGLE BIT INTO A INTEGER VAR
#define SET_BIT(var,nbit) var|= (1<<(nbit))
#define CLR_BIT(var,nbit) var&=~(1<<(nbit))
#define CHK_BIT(var,nbit) ((var>>(nbit))&1)

*/


//#ifdef IMAGE_DISPLAY
//#define SCALE_IMG(nimg, img)    (image_dsp[nimg]->*(image_dsp[nimg]->scale_img))( img )
//#define DRAW_PT(nimg,x,y,col)   (image_dsp[nimg]->*(image_dsp[nimg]->draw_pt))(x,y,col)




int Message_box( wchar_t* msg, int mode=MBOX_INFO );
int Message_box( CString& msg, int mode=MBOX_INFO );



  #ifndef _DEBUG
    //#undef SAVE_IMAGES
    #undef SAVE_FULL_PROC
  #endif


