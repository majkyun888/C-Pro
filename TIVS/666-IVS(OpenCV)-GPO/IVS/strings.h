#pragma once

#define NLANGS 5

#define ENGLISH   0
#define CHINESE   1   // ^
#define SPANISH   2   // #
#define TURKISH   3   // @
#define RUSSIAN   4   // &

#include "Def_globali.h"

/////////////////////////////////////////
//
//    U N I T A'  D I   M I S U R A
//
/////////////////////////////////////////


static wchar_t* string_mm[NLANGS] = {
  L"[mm]",
  L"[mm]",
  L"[mm]",
  L"@",
  L"[мм]"};


static wchar_t* string_ec[NLANGS] = { // Encoder Counts
  L"[ec]",
  L"[编码值]",
  L"[ec]",
  L"@",
  L"[ec]"};

static wchar_t* string_pix[NLANGS] = {  // pixels
  L"[pix]",
  L"[像素]",
  L"[pix]",
  L"@",
  L"[pix]" };

static wchar_t* string_pz[NLANGS] = {
  L"[pcs]",
  L"[pcs]",
  L"[pie]",
  L"@",
  L"[шт.]" };

static wchar_t* string_pzh[NLANGS] = {
  L"[pcs/h]",
  L"[pcs/h]",
  L"[pie/h]",
  L"@",
  L"[шт./ч"};

static wchar_t* string_pcm[NLANGS] = {
  L"[pcs/']",
  L"[pcs/']",
  L"[pcs]",
  L"@",
  L"[pcs/]" };

static wchar_t* string_prh[NLANGS] = {
  L"[prod/h]",
  L"[prod/h]",
  L"[prod/h]",
  L"@",
  L"[prod/h]"};

static wchar_t* string_unit[NLANGS] = {
  L"[units]",
  L"[units]",
  L"[unidades]",
  L"@",
  L"[units]" };

static wchar_t* string_rpm[NLANGS] = {
  L"[rpm]",
  L"[rpm]",
  L"[rpm]",
  L"@",
  L"[rpm]" };

static wchar_t* string_rpms[NLANGS] = {
  L"[rpm/s]",
  L"[rpm/s]",
  L"[rpm/s]",
  L"@",
  L"[rpm/s]" };

static wchar_t* string_rpm4[NLANGS] = {
  L"[rpm/4]",
  L"[rpm/4]",
  L"[rpm/4]",
  L"@",
  L"[rpm/4]" };

static wchar_t* string_hz[NLANGS] = {
  L"[Hertz]",
  L"[Hertz]",
  L"[Hertz]",
  L"@",
  L"[Hertz]" };

static wchar_t* string_sec[NLANGS] = {
  L"[sec]",
  L"[sec]",
  L"[sec]",
  L"@",
  L"[sec]" };

static wchar_t* string_msec[NLANGS] = {
  L"[msec]",
  L"[msec]",
  L"[msec]",
  L"@",
  L"[msec]"};

static wchar_t* string_usec[NLANGS] = {
  L"[usec]",
  L"[usec]",
  L"[usec]",
  L"@",
  L"[usec]"};

static wchar_t* string_log[NLANGS] = {
  L"[log]",
  L"[灰度]",
  L"[inicio]",
  L"@",
  L"[log]"};

static wchar_t* string_fps[NLANGS] = {
  L"[fps]",
  L"[帧率]",
  L"[pie/s]",
  L"@",
  L"[pie/s]"};

static wchar_t* string_step[NLANGS] = {
  L"[steps]",
  L"[步]",
  L"[pasos]",
  L"@",
  L"[шаг]"};

static wchar_t* string_cams[NLANGS] = {
  L"[cams]",
  L"[^]",
  L"[#]",
  L"[@]",
  L"[&]"};

static wchar_t* string_degrees[NLANGS] = {
  L"[°]",
  L"[°]",
  L"[°]",
  L"@",
  L"[°]"};

static wchar_t* string_centdegrees[NLANGS] = {
  L"[°x 100]",
  L"[°x 100]",
  L"[°x 100]",
  L"@",
  L"[°x 100]"};

static wchar_t* string_mmpix[NLANGS] = {
  L"[mm/pix]",
  L"[mm/pix]",
  L"[mm/pix]",
  L"@",
  L"[mm/pix]"};

static wchar_t* string_percent[NLANGS] = {
  L"[%]",
  L"[%]",
  L"[%]",
  L"@",
  L"[%]"};

static wchar_t* string_kvolt[NLANGS] = {
  L"[kV]",
  L"[kV]",
  L"[kV]",
  L"[kV]",
  L"[kV]"};

static wchar_t* string_ma[NLANGS] = {
  L"[mA]",
  L"[mA]",
  L"[mA]",
  L"[mA]",
  L"[mA]"};

    static wchar_t* string_day[NLANGS] = {
  L"[day]",
  L"[天]",
  L"[day]",
  L"[day]",
  L"[day]"};

static wchar_t* string_minute[NLANGS] = {
  L"[min]",
  L"[分钟]",
  L"[min]",
  L"[min]",
  L"[min]"};


static wchar_t* string_uppercase[NLANGS] = {
  L"UPPERCASE",
  L"大写字母",
  L"MAYUSCULAS",
  L"@",
  L"[ПРОПИСНЫЕ БУКВЫ]" };

static wchar_t* string_lowercase[NLANGS] = {
  L"LOWERCASE",
  L"小写字母",
  L"MINUSCULAS",
  L"@",
  L"[строчные буквы]" };

static wchar_t* string_ok[NLANGS] = {
  L"Ok",
  L"确认",
  L"Ok",
  L"@",
  L"[Да]"  };

static wchar_t* string_cancel[NLANGS] = {
  L"Cancel",
  L"取消",
  L"Cancelar",
  L"@",
  L"Отмена"  };

static wchar_t* string_backspace[NLANGS] = {
  L"Backspace",
  L"退格键",
  L"Retroceso",
  L"@",
  L"Назад" };

static wchar_t* string_clear[NLANGS] = {
  L"Clear",
  L"清除",
  L"Limpiar",
  L"@",
  L"Очистить"  };

static wchar_t* string_coords[NLANGS] = {
  L"Coords:\r\n(%d,%d)\r\n(%d,%d)\r\nDims:\r\n%dx%d",
  L"坐标:\r\n(%d,%d)\r\n(%d,%d)\r\n尺寸:\r\n%dx%d",
  L"Coords:\r\n(%d,%d)\r\n(%d,%d)\r\nDims:\r\n%dx%d",
  L"@",
  L"Координаты:\r\n(%d,%d)\r\n(%d,%d)\r\nРазмеры:\r\n%dx%d", };



////////////////////////////////////////////



static wchar_t* string_undef[NLANGS] = {
  L"undefined",
  L"undefined",
  L"undefined",
  L"@",
  L"undefined"};

static wchar_t* string_error[NLANGS] = {
  L"ERROR:",
  L"错误:",
  L"ERROR:",
  L"@",
  L"Ошибка:"  };

static wchar_t* string_warning[NLANGS] = {
  L"WARNING:",
  L"警告:",
  L"ATENCIÓN:",
  L"@",
  L"ВНИМАНИЕ:"  };

static wchar_t* string_question[NLANGS] = {
  L"?",
  L"?",
  L"?",
  L"@",
  L"?"  };


static wchar_t* string_enabled[NLANGS] = {
  L"ENABLED",
  L"启用",
  L"HABILITADO",
  L"@",
  L"ВКЛ."  };

static wchar_t* string_disabled[NLANGS] = {
  L"DISABLED",
  L"关闭",
  L"DESHABILITADO",
  L"@",
  L"ВЫКЛ."  };

static wchar_t* string_on[NLANGS] = {
  L" (ON)",
  L" (打开)",
  L" (ENCENDER)",
  L"@",
  L"ВКЛ."  };

static wchar_t* string_off[NLANGS] = {
  L" (OFF)",
  L" (关闭)",
  L" (APAGAR)",
  L"@",
  L"ВЫКЛ."  };


static wchar_t* string_good[NLANGS] = {
  L"GOOD",
  L"合格",
  L"BIEN",
  L"@",
  L"Пригодный"  };

static wchar_t* string_reject[NLANGS] = {
  L"REJECT",
  L"剔除",
  L"RECHAZO",
  L"@",
  L"ОТБР."  };




  
static wchar_t* string_002[NLANGS] = {
  L"Confirm program exit?",
  L"确认退出程序?",
  L"¿Salir del programa?",
  L"@",
  L"Выйти из программы?"  };

static wchar_t* string_004[NLANGS] = {
  L"Shutdown Windows and computer too?",
  L"退出程序并关机?",
  L"¿Apagar Windows y la computadora?",
  L"@",
  L"Выйти из Windows и ВЫКЛ.ючить ПК?"  };

static wchar_t* string_008[NLANGS] = {
  L"Machine/program status",
  L"机器/程序状态",
  L"Máquina/estado del programa",
  L"@",
  L"Машина/Состояние программы"  };

static wchar_t* string_010[NLANGS] = {
  L"ALARMS",
  L"警报",
  L"ALARMAS",
  L"@",
  L"Сигналы тревоги"  };

static wchar_t* string_012[NLANGS] = {
  L"WARNINGS",
  L"警告",
  L"ALERTAS",
  L"@",
  L"Предупреждения"  };

static wchar_t* string_014[NLANGS] = {
  L"Starting up communication with PLC... (%d\")\r\nPress F1 to abort waiting",
  L"启动和PLC通讯... (%d\")\r\n按F1退出等待",
  L"Comenzar comunicación con PLC... (%d\")\r\n Presiona F1 para abortar",
  L"@",
  L"Начало связи с PLC... (%d\")\r\nНажмите F1, чтобы выйти"   };

static wchar_t* string_016[NLANGS] = {
  L"Starting up program. Please wait... (%d\")",
  L"启动程序。请等待... (%d\")",
  L"Arrancar programa. Porfavor espere... (%d\")",
  L"@",
  L"Запуск программы .Подождите...(%d\")"  };

static wchar_t* string_020[NLANGS] = {
  L"No active alarm",
  L"无警报",
  L"Alarma inactiva",
  L"@",
  L"Откл. сигнала тревоги"  };

static wchar_t* string_022[NLANGS] = {
  L"No active warning",
  L"无警告",
  L"Precaución inactiva",
  L"@",
  L"Откл. предупреждения"  };

static wchar_t* string_026[NLANGS] = {
  L"Unable to load previous parameters from file <default.ips>\r\n<%s>\r\nDefault parameters will be set and used",
  L"不能从文档 <default.ips>\r\n<%s>\r\n下载之前的参数，使用默认参数",
  L"Incapaz de cargar paráms previos desde archivo <default.ips>\r\n<%s>\r\nParam. predeterminados establecidos",
  L"@",
     L"Не удалось загрузить предыдущие параметры из файла <default.ips>\r\n<%s>\r\n Будут загружены и установлены параметры по умолчанию" };

static wchar_t* string_028[NLANGS] = {
  L"Unable to save current parameters to file <default.ips>\r\n<%s>",
  L"不能把当前参数保存到文档 <default.ips>\r\n<%s>",
  L"Incapaz de guardar los parámetros actuales al archivo<default.ips>\r\n<%s>",
  L"@",
  L"Не удалось сохранить текущие параметры в файл<default.ips>\r\n<%s>"  };

static wchar_t* string_030[NLANGS] = {
  L"Unable to load required file <%s>\r\n<%s>\r\n",
  L"不能载入所需文档 <%s>\r\n<%s>\r\n",
  L"Incapaz de cargar el archivo solicitado<%s>\r\n<%s>\r\n"  ,
  L"@",
  L"Не удалось загрузить нужный файл <%s>\r\n<%s>\r\n"  };

static wchar_t* string_032[NLANGS] = {
  L"Unable to save required file <%s>\r\n<%s>\r\n",
  L"不能保存所需文档 <%s>\r\n<%s>\r\n",
  L"Incapaz de guardar el archivo solicitado <%s>\r\n<%s>\r\n"  ,
  L"@",
  L"Невозможно сохранить файл<%s>\r\n<%s>\r\n"  };

static wchar_t* string_034[NLANGS] = {
  L"Recipe <%s> loaded successfully",
  L"配方 <%s> 下载成功",
  L"Receta <%s> cargada exitosamente"  ,
  L"@",
  L"Рецепт <%s> успешно загружен"  };

static wchar_t* string_036[NLANGS] = {
  L"Recipe <%s> saved successfully (overwritten)",
  L"配方 <%s> 保存成功（覆盖）",
  L"Receta <%s> cargada exitosamente (reemplazada)"  ,
  L"@",
  L"Рецепт<%s> успешно сохранён (перезаписан)"  };

static wchar_t* string_037[NLANGS] = {
  L"Recipe <%s> created and saved successfully",
  L"配方 <%s> 创建并保存成功",
  L"Receta <%s> creada y guardada exitosamente "  ,
  L"@",
  L"Рецепт<%s> создан и успешно сохранен"  };

static wchar_t* string_038[NLANGS] = {
  L"Are you sure you want to save current parameters to existing recipe <%s> (OVERWRITE) ?",
  L"确定将现有参数保存到当前配方吗 <%s> （覆盖）？",
  L"¿Esta seguro que desea guardar los parametros actuales a la receta existente <%s> (Reemplazar)?"  ,
  L"@",
  L"Сохранить текущие параметры в существующий рецепт<%s> (ПЕРЕЗАПИСЬ)?"  };

static wchar_t* string_040[NLANGS] = {
  L"Please insert the name of new recipe to be saved",
  L"请输入新配方的名字",
  L"Inserte el nombre de la nueva receta para ser guardada"  ,
  L"@",
  L"Пожалуйста, введите название нового рецепта"  };

static wchar_t* string_042[NLANGS] = {
  L"Recipe name <%s> already exists on disk\r\nPlease select a non existing name!\r\n\r\nWARNING: CURRENT PARAMETERS NOT SAVED!",
  L"配方名字 <%s>  在磁盘上已存在\r\n 请选择一个新的名字！\r\n\r\n 警告:  现有参数没有保存",
  L"El nombre de la receta<%s> ya existe\r\nPor favor seleccione otro nombre\r\n\r\nPRECAUCION: LOS PARAMETROS ACTUALES NO SE GUARDARON "  ,
  L"@",
  L"Рецепт <%s> уже существует \r\nВведите другое название!\r\n\r\nВНИМАНИЕ: текущие параметры не сохранены!"  };

static wchar_t* string_044[NLANGS] = {
  L"Are you sure you want to delete selected recipe <%s> ?",
  L"确定删除当前配方吗 <%s> ？",
  L"¿Esta seguro que desea borrar la receta seleccionada <%s>?"  ,
  L"@",
  L"Удалить рецепт <%s>?"  };

static wchar_t* string_046[NLANGS] = {
  L"Selected recipe <%s> successfully deleted",
  L"当前配方 <%s> 删除成功",
  L"La receta seleccionada <%s> fue borrada exitosamente"  ,
  L"@",
  L"Выбранный рецепт <%s> успешно удален!"  };

static wchar_t* string_048[NLANGS] = {
  L"Unable to delete required file <%s>\r\nError n.%d",
  L"不能删除所需文档 <%s>\r\nError n.%d",
  L"Incapaz de borrar el archivo <%s>\r\nError n.%d",
  L"@ %d",
  L"Невозможно удалить файл <%s>\r\nError n.%d "  };


static wchar_t* string_060[NLANGS] = {
  L"Program initialized - current recipe <%s>",
  L"软件初始化-当前配方菜单 <%s>",
  L"Programa inicializado - receta actual <%s>",
  L"@ <%s>",
  L"Инициализация программного обеспечения -текущий рецепт <%s>"  };

static wchar_t* string_061[NLANGS] = {
  L"Program initialized - recipe <%s> - batch <%s>",
  L"软件初始化-当前配方菜单 <%s> - 批次 <%s>",
  L"Programa inicializado - receta actual <%s> - # <%s>",
  L"@ <%s> @ <%s>",
  L"Инициализация программного обеспечения -текущий рецепт <%s> - & <%s>"  };

  static wchar_t* string_061a[NLANGS] = {
  L"Program initialized - recipe <%s> - batch <%s> - speed <%s>",
  L"软件初始化-当前配方菜单 <%s> - 批次 <%s> - 速度<%s>",
  L"Programa inicializado - receta actual <%s> - # <%s>",
  L"@ <%s> @ <%s>",
  L"Инициализация программного обеспечения -текущий рецепт <%s> - & <%s>"  };


static wchar_t* string_062[NLANGS] = {
  L"Program waiting for command - current recipe <%s>",
  L"软件等待指令-当前配方菜单 <%s>",
  L"Programa en espera de comando - receta actual <%s>",
  L"@ <%s>",
  L"Программа ожидает команды – текущий рецепт <%s>"  };

static wchar_t* string_063[NLANGS] = {
  L"Program waiting for command - recipe <%s> - batch <%s>",
  L"软件等待指令-当前配方菜单 <%s> - 批次 <%s>",
  L"Programa en espera de comando - receta <%s> - # <%s>",
  L"@ <%s> - @ <%s>",
  L"Программа ожидает команды – текущий рецепт <%s> - & <%s>"  };

static wchar_t* string_064[NLANGS] = {
  L"Inspection active - current recipe <%s>",
  L"检测激活-当前配方菜单 <%s>",
  L"Inspección activa - receta actual <%s>",
  L"@ <%s>",
  L"Запустить инспекцию – текущий рецепт <%s>"  };

static wchar_t* string_065[NLANGS] = {
  L"Inspection active - recipe <%s> - batch <%s>",
  L"检测激活-当前配方菜单 <%s> - 批次 <%s>",
  L"Inspección activa - receta actual <%s> - # <%s>",
  L"@ <%s> @ <%s>",
  L"Запустить инспекцию – текущий рецепт <%s> - & <%s>"  };

static wchar_t* string_066[NLANGS] = {
  L"Learning active - current recipe <%s>",
  L"学习激活-当前配方菜单 <%s>",
  L"Aprendizaje activo - receta actual <%s>",
  L"@ <%s>",
  L"Запустить обучение – текущий рецепт <%s>"  };

static wchar_t* string_067[NLANGS] = {
  L"Learning active - recipe <%s> - batch <%s>",
  L"学习激活-当前配方菜单 <%s> - 批次 <%s>",
  L"Aprendizaje activo - receta actual <%s> - # <%s>",
  L"@ <%s> @ <%s>",
  L"Запустить обучение – текущий рецепт <%s> - & <%s>"  };

static wchar_t* string_068[NLANGS] = {
  L"Live image active - current recipe <%s>",
  L"实时图像激活-当前配方菜单 <%s>",
  L"Película activa - receta actual <%s>",
  L"@ <%s>",
  L"Активация live изображения  – текущий рецепт <%s>"  };

static wchar_t* string_069[NLANGS] = {
  L"Live image active - recipe <%s> - batch <%s)",
  L"实时图像激活-当前配方菜单 <%s> - 批次 <%s>",
  L"Película activa - receta actual <%s> - # <%s>",
  L"@ <%s> @ <%s>",
  L"Активация live изображения  – текущий рецепт <%s> - & <%s>"  };

static wchar_t* string_070[NLANGS] = {
  L"Exiting program....",
  L"退出程序 ....",
  L"Salida del programa....",
  L"@",
  L"Выход из программы..."  };


static wchar_t* string_090[NLANGS] = {
  L"Unable to access/create main program directory\r\n<%s>",
  L"不能访问/创建主程序目录\r\n<%s>",
  L"Incapaz de accesar/crear el directorio del programa\r\n<%s>",
  L"@",
  L"Нет доступа/невозможно создать основной каталог программы\r\n<%s>"  };

static wchar_t* string_092[NLANGS] = {
  L"Unable to access/create input recipes directory\r\n<%s>",
  L"不能访问/创建输入配方目录\r\n<%s>",
  L"Incapaz de accesar/crear el directorio de recetas de entrada\r\n<%s>",
  L"@",
  L"Нет доступа/невозможно создать входящий рецепт в каталоге\r\n<%s>"  };

static wchar_t* string_094[NLANGS] = {
  L"Unable to access/create output recipes directory\r\n<%s>",
  L"不能访问/创建输出配方目录\r\n<%s>",
  L"Incapaz de accesar/crear el directorio de recetas a la salida\r\n<%s>",
  L"@",
  L"Нет доступа/невозможно создать выходящий рецепт\r\n<%s>"   };


static wchar_t* string_104[NLANGS] = {
  L"Show alarm log",
  L"显示报警日志",
  L"Mostrar Log Alarmas",
  L"@",
  L"Показать журнал сигналов тревоги"  };



static wchar_t* string_110[NLANGS] = {
  L"RUN BACK BELT",
  L"运行后网带",
  L"ARRANCAR BANDA TRASERA ",
  L"@",
  L"Запустить конвейер выгрузки"  };

static wchar_t* string_112[NLANGS] = {
  L"RUN ROTARY TABLE",
  L"运行转盘",
  L"ARRANCAR MESA GIRATORIA",
  L"@",
  L"Запустить роторный стол"  };

static wchar_t* string_114[NLANGS] = {
  L"RUN PRE-INPUT BELT",
  L"运行前进料网带",
  L"ARRANCAR BANDA DE PRE-ENTRADA",
  L"@",
  L"Запустить конвейер загрузки"  };

static wchar_t* string_116[NLANGS] = {
  L"MANUAL INPUT BELT",
  L"手动启停进料网带",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_118[NLANGS] = {
  L"MANUAL OUTPUT BELT",
  L"手动启停出料网带",
  L"#",
  L"@",
  L"&"  };






static wchar_t* string_120[NLANGS] = {
  L"RECIPES",
  L"配方参数",
  L"RECETAS",
  L"@",
  L"Рецепты"  };

  //Pour added for Sunho on 20200701
  static wchar_t* string_122[NLANGS] = {
  L"Cannot change parameters in run model",
  L"设备运行时不允许修改参数",
  L"RECETAS",
  L"@",
  L"Рецепты"  };

static wchar_t* string_124[NLANGS] = {
  L"Machine lights",
  L"机器灯光",
  L"Luces de la máquina",
  L"@",
  L"Световая сигнализация"  };

    static wchar_t* string_126[NLANGS] = {
  L"RUN INFEED BELTS",
  L"开启进料输送带",
  L"ARRANCAR BANDAS",
  L"@",
  L"Запустить конвейеры"  };

static wchar_t* string_128[NLANGS] = {
  L"RUN BELTS",
  L"开启输送带",
  L"ARRANCAR BANDAS",
  L"@",
  L"Запустить конвейеры"  };



/*  
static wchar_t* string_100[NLANGS] = {
  L"MACHINE PARAMETERS",
  L"机器参数",
  L"PARÁMETROS DE MÁQUINA",
  L"@",
  L"Параметры машины"  };

static wchar_t* string_110[NLANGS] = {
  L"INSPECTION PARAMETERS",
  L"检测参数",
  L"PARÁMETROS DE INSPECCIÓN",
  L"@",
  L"Параметры инспекции"  };

static wchar_t* string_120[NLANGS] = {
  L"RECIPES",
  L"配方参数",
  L"RECETAS",
  L"@",
  L"Рецепты"  };
  */
static wchar_t* string_130[NLANGS] = {
  L"UTILITIES",
  L"公用工程",
  L"SERVICIOS",
  L"@",
  L"Энергоносители"  };

static wchar_t* string_140[NLANGS] = {
  L"EXIT",
  L"退出",
  L"SALIDA",
  L"@",
  L"ВЫХОД"  };

static wchar_t* string_142[NLANGS] = {
  L"REVERSE INPUT BELT",
  L"进料网带反转",
  L"REVERSA DE BANDA DE ENTRADA",
  L"@",
  L"Реверс конвейера загрузки"  };


  
static wchar_t* string_150[NLANGS] = {
  L"START INSPECTION",
  L"开始检测",
  L"INICIO DE INSPECCIÓN ",
  L"@",
  L"Запустить инспекцию"  };


static wchar_t* string_152[NLANGS] = {
  L"STOP INSPECTION",
  L"停止检测",
  L"PARO DE INSPECCIÓN",
  L"@",
  L"Остановить инспекцию",};

static wchar_t* string_154[NLANGS] = {
  L"STOPPING...",
  L"停止中...",
  L"PARAR",
  L"@",
  L"Остановка..."  };

static wchar_t* string_160[NLANGS] = {
  L"START LIVE VIEW",
  L"启动实时显示",
  L"INICIO DE VISTA EN VIVO",
  L"@",
  L"ВКЛ. отображение "  };

static wchar_t* string_162[NLANGS] = {
  L"STOP LIVE VIEW",
  L"停止实时显示",
  L"DETENER VISTA EN VIVO",
  L"@",
  L"Выкл. LIVE отображение "  };

static wchar_t* string_170[NLANGS] = {
  L"Save images",
  L"保存图片",
  L"Guardar imágenes",
  L"@",
  L"Сохранить изображения"  };

static wchar_t* string_172[NLANGS] = {
  L"Load images",
  L"下载图片",
  L"Cargar imágenes",
  L"@",
  L"Загрузить изображения"  };

static wchar_t* string_178[NLANGS] = {
  L"Waiting to establish communication with PLC - F1 to abort.. %.0lf\"",
  L"等待和PLC建立通讯-按F1取消 %.0lf\" ",
  L"En espera para establecer comunicación con el PLC - F1 para abortar %.0lf\" ",
  L"@",
  L"Ожидание связи с ПЛК -Для отмены нажмите F1 %.0lf\" "  };


static wchar_t* string_180[NLANGS] = {
  L"Communication with PLC currently disabled: PLC params are going to be saved with zeros!\r\nWould you like to save default parameters anyway?",
  L"现在和PLC不能通讯: PLC参数带小数点位保存 !\r\n是否想保存默认参数?",
  L"¡Comunicación con el PLC actualmente deshabilitada: los parámetros del PLC seran restablecidos a ceros!\r\n¿Le gustaría guardar los parámetros aun así?",
  L"@",
  L"Потеряна связь с ПЛК: возможно только сохранение пустых параметров! \r\nСохранить параметры по умолчанию?"  };

static wchar_t* string_182[NLANGS] = {
  L"Unable to write value of <%s> into PLC\r\nError n.%d <%s>\r\n\r\nWould you like to disable communication with PLC?",
  L"不能把 <%s>值写入PLC\r\n错误 n.%d <%s>\r\n\r\n是否想断开和PLC的通讯?",
  L"Incapaz de establecer valores  <%s> al PLC\r\nError n.%d <%s>\r\n\r\n¿Le gustaría deshabilitar la comunicación con el PLC?",
  L"@",
  L"Невозможно сохранить значение <%s> в ПЛК\r\nОшибка n.%d <%s>\r\n\r\nОтключить связь с ПЛК?"  };

static wchar_t* string_184[NLANGS] = {
  L"Unable to read value of <%s> from PLC\r\nError n.%d <%s>\r\n\r\nWould you like to disable communication with PLC?",
  L"不能从PLC读出<%s>的值 \r\n错误 n.%d <%s>\r\n\r\n是否想断开和PLC的通讯?",
  L"Incapaz de leer el valor <%s> del PLC\r\nError n.%d <%s>\r\n\r\n¿Le gustaría deshabilitar la comunicación con el PLC?",
  L"@",
  L"Невозможно чтение значения <%s> в ПЛК\r\nОшибка n.%d <%s>\r\n\r\nОтключить связь с ПЛК?"   };

static wchar_t* string_186[NLANGS] = {
  L"Unable to find variable <%s> on the PLC\r\nError n.%d <%s>\r\n\r\nIt will not be possible to show/modify such parameter",
  L"不能从PLC上找到变量<%s> \r\n错误 n.%d <%s>\r\n\r\n不能显示/更改这些参数",
  L"Incapaz de encontrar la variable <%s> en el PLC\r\nError n.%d <%s>\r\n\r\nNo será posible mostrar/modificar tales parámetros",
  L"@",
  L"Не удалось найти переменную <%s> \r\nОшибка n.%d <%s>\r\n\r\n Отображение / изменение параметра невозможно"  };


static wchar_t* string_190[NLANGS] = {
  L"OPERATOR",
  L"操作人",
  L"OPERADOR",
  L"@",
  L"Оператор"  };

static wchar_t* string_200[NLANGS] = {
  L"Processed products",
  L"进料产品",
  L"Productos procesados",
  L"@",
  L"Продукт, прошедший инспекцию"  };

static wchar_t* string_210[NLANGS] = {
  L"Rejected products",
  L"不合格品",
  L"Productos rechazados",
  L"@",
  L"ОТБР. "  };

static wchar_t* string_220[NLANGS] = {
  L"Good products",
  L"合格品",
  L"Productos buenos",
  L"@",
  L"Пригодный продукт"  };

static wchar_t* string_230[NLANGS] = {
  L"Bottom rejects",
  L"底部检测不合格品",
  L"Rechazos de fondo",
  L"@",
  L"ОТБР. - дно"  };

static wchar_t* string_240[NLANGS] = {
  L"Wall rejects",
  L"瓶壁检测不合格品",
  L"Rechazos de superficies",
  L"@",
  L"ОТБР. - стенки"  };

static wchar_t* string_250[NLANGS] = {
  L"Production speed",
  L"生产速度",
  L"Velocidad de producción",
  L"@",
  L"Производительность"  };

static wchar_t* string_260[NLANGS] = {
  L"Batch",
  L"批次",
  L"Lote",
  L"@",
  L"Партия"  };

static wchar_t* string_270[NLANGS] = {
  L"Description",
  L"描述",
  L"Descripción",
  L"@",
  L"Описание"  };

static wchar_t* string_280[NLANGS] = {
  L"Operator",
  L"操作人",
  L"Operador",
  L"@",
  L"Оператор"  };

static wchar_t* string_300[NLANGS] = {
  L"Get backup",
  L"获取备份",
  L"Obtención de antecedentes",
  L"@",
  L"Сделать резервную копию"  };

static wchar_t* string_302[NLANGS] = {
  L"Would you like to save acquired backup to disk?",
  L"是否把获得的备份记录存储到硬盘?",
  L"¿Guardar antecedentes al disco?",
  L"@",
  L"Сохранить резервную копию на диск?"  };

static wchar_t* string_304[NLANGS] = {
  L"Datarec OFF",
  L"数据记录关闭",
  L"Apagar Grab.Datos ",
  L"@",
  L"Запись данных ВЫКЛ. "  };

static wchar_t* string_306[NLANGS] = {
  L"Datarec ENABLED",
  L"数据记录启用",
  L"Habilitar Grab.Datos",
  L"@",
  L"Запись данных ВКЛ."  };

static wchar_t* string_310[NLANGS] = {
  L"Loop inspection OFF",
  L"循环检测关闭",
  L"Apagar inspección de bucle ",
  L"@",
  L"Круговая инспекция ВЫКЛ."  };

static wchar_t* string_312[NLANGS] = {
  L"Loop inspection ENABLED",
  L"循环检测开启",
  L"Habilitar inspección de bucle",
  L"@",
  L"Круговая проверка ВКЛ."  };

static wchar_t* string_320[NLANGS] = {
  L"Save Inspections OFF",
  L"保存检测关闭",
  L"Apagar inspecciones salvaguardas",
  L"@",
  L"Сохранение инспекции ВЫКЛ."  };

static wchar_t* string_320a[NLANGS] = {
	L"Save Inspections mode(*)",
	L"保存检测模式选择(*)",
	L"Apagar inspecciones salvaguardas",
	L"@",
	L"Сохранение инспекции ВЫКЛ." };

static wchar_t* string_322[NLANGS] = {
  L"Save Inspections ENABLED",
  L"保存检测使能",
  L"Habilitar inspecciones salvaguardas",
  L"@",
  L"Сохранение инспекции ВКЛ."  };

static wchar_t* string_324[NLANGS] = {
  L"Save Sequences OFF",
  L"保存序列图片关闭",
  L"Apagar secuencias salvaguardas",
  L"@",
  L"Сохранение изображений ВЫКЛ."  };

static wchar_t* string_326[NLANGS] = {
  L"Save Sequences ENABLED",
  L"保存序列图片使能",
  L"Habilitar secuencias salvaguardas",
  L"@",
  L"Сохранение  изображений ВКЛ."  };

  /*
static wchar_t* string_328[NLANGS] = {
  L"Grippers statistics",
  L"夹爪统计",
  L"Estadísticas de pinzas",
  L"@",
  L"Статистика захватов"  };
  */

static wchar_t* string_330[NLANGS] = {
  L"Open Doors Alarm",
  L"开门警报",
  L"Alarma puertas abiertas",
  L"@",
  L"Сигнал-открытие дверей "  };

  static wchar_t* string_330a[NLANGS] = {
  L"Open HV Doors Alarm",
  L"HV开门警报",
  L"HV Alarma puertas abiertas",
  L"@",
  L"HV Сигнал-открытие дверей "  };

static wchar_t* string_332[NLANGS] = {
  L"Light alarm",
  L"光源警报",
  L"Alarma de iluminadores ",
  L"@",
  L"Световой сигнал "  };

  static wchar_t* string_334[NLANGS] = {
  L"Gripper spin alarm",
  L"夹爪旋转警报",
  L"Alarma de iluminadores ",
  L"@",
  L"Световой сигнал "  };

    static wchar_t* string_336[NLANGS] = {
  L"Gripper close alarm",
  L"夹爪开闭警报",
  L"Alarma de iluminadores ",
  L"@",
  L"Световой сигнал "  };

static wchar_t* string_340[NLANGS] = {
  L"Vacuum Pump",
  L"真空泵",
  L"Bomba de vacío",
  L"@",
  L"Вакуумный насос"  };

static wchar_t* string_342[NLANGS] = {
  L"Cameras Reset",
  L"相机复位",
  L"Restablecer cámaras",
  L"@",
  L"Сброс камер "  };

static wchar_t* string_344[NLANGS] = {
  L"Resetting cameras (%d/20\")",
  L"相机复位中 (%d/20\")",
  L"Restablecimiento de cámaras (%d/20\")",
  L"@",
  L"Сброс камер(%d/20\")"  };

static wchar_t* string_346[NLANGS] = {
  L"After camera reset it is required to restart both PC1 and PC2 software",
  L"相机复位后，需要重启PC1和PC2的视觉软件",
  L"Después de restablecer camara, reiniciar ambos software PC1 y PC2 (%d/20\")",
  L"@",
  L"После сброса требуется перезапуск  ПК-1 и ПК-2 "  };

static wchar_t* string_348[NLANGS] = {
  L"Are you sure you want to reset all cameras?",
  L"确定要复位所有相机吗?",
  L"¿Seguro qe desea restablecer todas las cámaras?",
  L"@",
  L"Сбросить все камеры?"  };


static wchar_t* string_354[NLANGS] = {
  L"Input Data rec Off",
  L"进料传感器数据收集关闭",
  L"Apagar Reg.Datos de entrada",
  L"@",
  L"Запись входных данных ВЫКЛ. "  };

static wchar_t* string_356[NLANGS] = {
  L"Input Data rec ON",
  L"使能进料传感器数据记录",
  L"Encender Reg.Datos de entrada",
  L"@",
  L"Запись входных данных ВКЛ."  };


static wchar_t* string_360[NLANGS] = {
  L"Filling lev Datarec Off",
  L"液位检测数据记录关闭",
  L"Apagdo detección de nivel de Reg.Datos",
  L"@",
  L"Запись данных об уровне ВЫКЛ."  };

static wchar_t* string_362[NLANGS] = {
  L"Filling lev Datarec ON",
  L"液位检测数据记录开启",
  L"Encendido detección de nivel de Reg.Datos",
  L"@",
  L"Запись данных об уровне ВКЛ."  };




static wchar_t* string_400[NLANGS] = {
  L"REJECT: Error %d/%d (%d mS)",
  L"剔除: 错误 %d/%d (%d mS)",
  L"RECHAZO: Error %d/%d (%d mS)",
  L"@",
  L"ОТБР.: Ошибка %d/%d (%d mS)"  };

static wchar_t* string_410[NLANGS] = {
  L"GOOD: Error %d/%d (%d mS)",
  L"合格: 错误 %d/%d (%d mS)",
  L"BIEN: Error %d/%d (%d mS)",
  L"@",
  L"ПРИГОДЕН: Ошибка%d/%d (%d mS)"  };

static wchar_t* string_420[NLANGS] = {
  L"Load",
  L"下载",
  L"Carga",
  L"@",
  L"Загрузить"  };

static wchar_t* string_422[NLANGS] = {
  L"Save",
  L"保存",
  L"Guardar",
  L"@",
  L"Сохранить"  };

static wchar_t* string_424[NLANGS] = {
  L"Page up",
  L"向上翻页",
  L"Página arriba",
  L"@",
  L"Вверх"  };

static wchar_t* string_426[NLANGS] = {
  L"Page down",
  L"向下翻页",
  L"Página abajo",
  L"@",
  L"Вниз"  };

static wchar_t* string_428[NLANGS] = {
  L"Rename",
  L"新建",
  L"Renombrar",
  L"@",
  L"Переименовать"  };

static wchar_t* string_430[NLANGS] = {
  L"Delete",
  L"删除",
  L"Borrar",
  L"@",
  L"Удалить"  };

static wchar_t* string_432[NLANGS] = {
  L"Close",
  L"关闭",
  L"Cerrar",
  L"@",
  L"Закрыть"  };


static wchar_t* string_500 [NLANGS] = {
  L"Input value <%d> is outside allowed range <%d,%d>",
  L"输入值 <%d> 超出允许值<%d,%d>",
  L"Valor de entrada <%d> fuera del rango <%d,%d>",
  L"@",
  L"Введенное значение <%d> выходит за рамки <%d,%d>"  };

static wchar_t* string_501 [NLANGS] = {
  L"Input value <%.1lf> is outside allowed range <%.1lf,%.1lf>",
  L"输入值 <%.1lf> 超出允许值 <%.1lf,%.1lf>",
  L"Valor de entrada <%.1lf> fuera del rango <%.1lf,%.1lf>",
  L"@",
  L"Введенное значение <%.1lf> выходит за рамки <%.1lf,%.1lf>"  };

static wchar_t* string_502[NLANGS] = {
  L"Unable to read input value",
  L"不能读取输入值",
  L"Incapaz de leer valor de entrada",
  L"@",
  L"Чтение значения невозможно"  };


static wchar_t* string_510[NLANGS] = {
  L"Inspection[%d] active (%d done)",
  L"检测[%d] 激活 (%d 完成)",
  L"Inspección[%d] activa (%d fini)",
  L"@",
  L"Инспекция[%d] активна(%d выполнено)"  };

static wchar_t* string_512[NLANGS] = {
  L"Initializing camera[%d]...",
  L"初始化相机 [%d]",
  L"Inicializando cámara [%d]",
  L"@",
  L"Инициализация камеры [%d]…"  };

static wchar_t* string_514[NLANGS] = {
  L"Initializing camera[%d] %s SN%s",
  L"初始化相机 [%d] %s SN%s",
  L"Inicializando cámara [%d] %s SN%s",
  L"@",
  L"Инициализация камеры[%d] %s SN%s"  };

static wchar_t* string_520[NLANGS] = {
  L"Unable to initialize camera <%s>\r\n%s",
  L"不能初始化相机 <%s>\r\n%s",
  L"Incapaz de inicializar cámara <%s>\r\n%s",
  L"@",
  L"Инициализация камеры <%s> невозможна\r\n%s "  };

static wchar_t* string_522[NLANGS] = {
  L"Unable to initialize camera <%s> SN%s\r\n%s",
  L"不能初始化相机 <%s> SN%s\r\n%s",
  L"Incapaz de inicializar cámara <%s> SN%s\r\n%s",
  L"@",
  L"Инициализация камеры <%s> SN%s невозможна \r\n%s "  };



static wchar_t* string_538[NLANGS] = {
  L"Show machine layout",
  L"返回主界面",
  L"Mostrar plano de máquina",
  L"@",
  L"Показать чертеж машины"  };

static wchar_t* string_539[NLANGS] = {
  L"Show PARTICLE cameras",
  L"显示粒子检测视图",
  L"Mostrar cámaras PARTICULA",
  L"@",
  L"Показать включения"  };

static wchar_t* string_540[NLANGS] = {
  L"Show TIP cameras",
  L"显示头部检测视图",
  L"Mostrar cámaras TIP",
  L"@",
  L"Показать кончик ампулы"  };

static wchar_t* string_541[NLANGS] = {
  L"Show CAP cameras",
  L"显示头部检测视图",
  L"Mostrar cámaras CAP",
  L"@",
  L"Показать колпачок "  };

static wchar_t* string_542[NLANGS] = {
  L"Show PARTICLE1 cameras",
  L"显示粒子1检测视图",
  L"Mostrar cámaras PARTICULA1",
  L"@",
  L"Показать включения 1"  };

    static wchar_t* string_543[NLANGS] = {
  L"Show NECK cameras",
  L"显示瓶脖检测视图",
  L"Mostrar cámaras NECK",
  L"@",
  L"Показать NECK"  };

static wchar_t* string_544[NLANGS] = {
  L"Show PARTICLE2 cameras",
  L"显示粒子2检测视图",
  L"Mostrar cámaras PARTICULA2",
  L"@",
  L"Показать включения 2"  };

  static wchar_t* string_544a[NLANGS] = {
  L"Show TOP-CAKE/P2 cameras",
  L"显示粉饼上/粒子2检测视图",
  L"Mostrar cámaras PARTICULA2",
  L"@",
  L"Показать включения 2"  };

static wchar_t* string_545[NLANGS] = {
  L"Show PARTICLE3 cameras",
  L"显示粒子3检测视图",
  L"Mostrar cámaras PARTICULA3",
  L"@",
  L"Показать включения 3"  };

static wchar_t* string_546[NLANGS] = {
  L"Show COSMETIC cameras",
  L"显示外观检测视图",
  L"Mostrar cámaras COSMETICA",
  L"@",
  L"Показать косметические дефекты"  };

static wchar_t* string_548[NLANGS] = {
  L"Show TOP-CAKE cameras",
  L"显示粉饼顶部检测视图",
  L"Mostrar cámaras SUPERIOR-PASTEL",
  L"@",
  L"Показать ЛИО-продукт сверху"  };

static wchar_t* string_549[NLANGS] = {
  L"Show SIDE-WALL cameras",
  L"显示外观检测相机",
  L"Mostrar cámaras LATERAL-SUPERFICIE",
  L"@",
  L"Показать стенки контейнера"  };

static wchar_t* string_550[NLANGS] = {
  L"Show SIDE-CAKE cameras",
  L"显示粉饼侧面检测视图",
  L"Mostrar cámaras LATERAL-PASTEL",
  L"@",
  L"Показать ЛИО сбоку"  };

static wchar_t* string_551[NLANGS] = {
  L"Show SIDE/TOP-CAKE cameras",
  L"显示粉饼侧面/顶部检测视图",
  L"MOstrar cámaras LAT/SUP PASTEL",
  L"@",
  L"Боковой/верхний вид ЛИО"  };

  static wchar_t* string_551a[NLANGS] = {
  L"Show COS/S-CAKE cameras",
  L"显示外观/粉饼侧面检测视图",
  L"MOstrar cámaras LAT/SUP PASTEL",
  L"@",
  L"Боковой/верхний вид ЛИО"  };

static wchar_t* string_552[NLANGS] = {
  L"Show BOTTOM/FO cameras",
  L"显示底部检测视图",
  L"Mostrar cámaras FONDO/FO",
  L"@",
  L"Показать ЛИО снизу"  };

static wchar_t* string_553[NLANGS] = {
  L"Show WAIST CAKE cameras",
  L"显示粉饼萎缩检测视图",
  L"Compresión Pastel",
  L"@",
  L"Показать сжатие ЛИО-продукта"  };

static wchar_t* string_554[NLANGS] = {
  L"Show Flip Off camera",
  L"显示易开盖检测视图",
  L"Mostrar cámara VOLTEADA",
  L"@",
  L"Показать Flip-off"  };

static wchar_t* string_556[NLANGS] = {
  L"Show DotRing cameras",
  L"显示头部检测视图",
  L"Mostrar cámaras de Punto-anillo",
  L"@",
  L"Показать точки"  };
  /*
    static wchar_t* string_555[NLANGS] = {
  L"Show Neck camera",
  L"显示瓶脖检测视图",
  L"Mostrar cámara ",
  L"@",
  L"Показать "  };
  */
static wchar_t* string_558[NLANGS] = {
  L"Show HEEL cameras",
  L"显示瓶根检测视图",
  L"Mostrar Cam.´base",
  L"@",
  L"Показать донную сторону ЛИО"  };


static wchar_t* string_559[NLANGS] = {
  L"Show liquid color cam",
  L"显示色水相机",
  L"#",
  L"@",
  L"&"  };


static wchar_t* string_545a[NLANGS] = {
  L"Show TCA C3A&&B cameras",
  L"显示法兰检测视图",
  L"Mostrar cámaras #",
  L"@",
  L"&"  };

static wchar_t* string_545b[NLANGS] = {
  L"Show NEEDLE cameras",
  L"显示针帽检测视图",
  L"Mostrar cámaras #",
  L"@",
  L"&"  };

  //Pour added 20190428
  static wchar_t* string_555[NLANGS] = {
  L"Show Neck camera",
  L"显示瓶脖检测视图",
  L"Mostrar cámara ",
  L"@",
  L"Показать "  };
  //Pour added 20190428



static wchar_t* string_560[NLANGS] = {
  L"AB Max lenght",
  L"AB最大长度",
  L"AB longitud máx.",
  L"@",
  L"MAX длина AB"  };

static wchar_t* string_561[NLANGS] = {
  L"AB Min lenght",
  L"AB最小长度",
  L"AB longitud mín.",
  L"@",
  L"MIN длина AB "  };

static wchar_t* string_562[NLANGS] = {
  L"AB Max percentage",
  L"AB最大比例值",
  L"AB porcentaje máx",
  L"@",
  L"MAX процент AB"  };

static wchar_t* string_563[NLANGS] = {
  L"AB Max angle",
  L"AB最大角度",
  L"AB ángulo máx.",
  L"@",
  L"MAX угол AB"  };

static wchar_t* string_564[NLANGS] = {
  L"AB Min angle",
  L"AB最小角度",
  L"AB ángulo mín.",
  L"@",
  L"MIN угол AB"  };





static wchar_t* string_600[NLANGS] = {
  L"Filling level",
  L"液位高度参数",
  L"Nivel de Llenado",
  L"@",
  L"Уровень наполнения"  };


static wchar_t* string_604[NLANGS] = {
  L"Perc for bottom detection (*)",
  L"底部检测百分比变化 (*)",
  L"Detección de fondo",
  L"@",
  L"Процент инспекции дна(*)"  };


// TIP / CAP INSPECTION

static wchar_t* string_606[NLANGS] = {
  L"Loop inspection",
  L"单工位检测",
  L"Inspección de bucle",
  L"@",
  L"Круговая инспекция"  };

static wchar_t* string_608[NLANGS] = {
  L"Tip MODE",
  L"安瓿头部检测模式",
  L"MODO Punta",
  L"@",
  L"Кончик ампулы"  };

static wchar_t* string_610[NLANGS] = {
  L"Cap MODE",
  L"西林瓶头部检测模式",
  L"MODO Tapa",
  L"@",
  L"Колпачок флакона"  };

static wchar_t* string_612[NLANGS] = {
  L"Neck search height",
  L"头部搜索高度",
  L"Altura de busqueda de cuello",
  L"@",
  L"Высота горлышка"  };

static wchar_t* string_614[NLANGS] = {
  L"Neck search threshold",
  L"头部搜索阈值",
  L"Umbral de busqueda de cuello",
  L"@",
  L"Порог поиска кончика"  };

static wchar_t* string_616[NLANGS] = {
  L"Ampoule diameter",
  L"安瓿头部直径",
  L"Diámetro",
  L"@",
  L" Диаметр ампулы"  };

static wchar_t* string_618[NLANGS] = {
  L"Minimum brightness",
  L"最小亮度",
  L"Brillo mínimo",
  L"@",
  L"MIN яркость"  };

static wchar_t* string_619[NLANGS] = {
  L"Maximum brightness",
  L"最大亮度",
  L"Brillo Máximo",
  L"@",
  L"MAX яркость"  };

static wchar_t* string_620[NLANGS] = {
  L"Min number of pts",
  L"最少点数",
  L"Min número de pts",
  L"@",
  L"MIN количество"  };

static wchar_t* string_622[NLANGS] = {
  L"Max allowed error (*)",
  L"最大允许错误值 (*)",
  L"Error Máximo Permitido (*)",
  L"@ (*)",
  L" MAX допустимое кол-во ошибок(*)"  };

static wchar_t* string_624[NLANGS] = {
  L"Brightness threshold",
  L"亮度阈值",
  L"Brillo de umbral",
  L"@",
  L"Порог яркости"  };

static wchar_t* string_630[NLANGS] = {
  L"Max allowed height (*)",
  L"最大允许高度(*)",
  L"Max. altura permitida",
  L"@",
  L"MAX допустимая высота(*)"  };

static wchar_t* string_632[NLANGS] = {
  L"Min allowed height (*)",
  L"最小允许高度(*)",
  L"Min. altura permitida",
  L"@",
  L"MIN допустимая высота"  };

static wchar_t* string_634[NLANGS] = {
  L"Filling level control disabled",
  L"液位检测未开启",
  L"Deshabilitar control de nivel de llenado",
  L"@",
  L"Контроль уровня наполнения ВЫКЛ."  };


  
static wchar_t* string_636[NLANGS] = {
  L"Filling level too low REJECT (%d/%d/%d)",
  L"液位太低 (%d/%d/%d)",
  L"Nivel de llenado bajo RECHAZO (%d/%d/%d)",
  L"@",
  L"низкий уровень (%d/%d/%d)"  };

static wchar_t* string_638[NLANGS] = {
  L"Filling level too high REJECT (%d/%d/%d)",
  L"液位太高 (%d/%d/%d)",
  L"Nivel de llenado alto RECHAZO (%d/%d/%d)",
  L"@",
  L"высокий уровень (%d/%d/%d)"  };

static wchar_t* string_640[NLANGS] = {
  L"Filling level GOOD (%d/%d/%d)",
  L"液位合格 (%d/%d/%d)",
  L"Nivel de llenado bien (%d/%d/%d)",
  L"@",
  L"ПРИГОДЕН: уровень (%d/%d/%d)"  };

static wchar_t* string_642[NLANGS] = {
  L"Filling level NOT FOUND",
  L"未找到液位",
  L"Nivel de llenado no encontrado",
  L"@",
  L"НЕ НАЙДЕН: уровень"  };



static wchar_t* string_650[NLANGS] = {
  L"Filling level too low REJECT (%d/%d/%d) %d/%d",
  L"液位太低 (%d/%d/%d) %d/%d",
  L"Nivel de llenado bajo RECHAZO (%d/%d/%d) %d/%d",
  L"@",
  L"низкий уровень (%d/%d/%d) %d/%d"  };

static wchar_t* string_652[NLANGS] = {
  L"Filling level too high REJECT (%d/%d/%d) %d/%d",
  L"液位太高 (%d/%d/%d) %d/%d",
  L"Nivel de llenado alto RECHAZO (%d/%d/%d) %d/%d",
  L"@",
  L"высокий уровень (%d/%d/%d) %d/%d "  };

static wchar_t* string_654[NLANGS] = {
  L"Filling level GOOD (%d/%d/%d) %d/%d",
  L"液位合格 (%d/%d/%d) %d/%d",
  L"Nivel de llenado bien (%d/%d/%d) %d/%d",
  L"@",
  L"ПРИГОДЕН: уровень (%d/%d/%d) %d/%d "  };

static wchar_t* string_656[NLANGS] = {
  L"Filling level NOT FOUND %d/%d",
  L"未找到液位 %d/%d",
  L"Nivel de llenado no encontrado %d/%d",
  L"@",
  L"НЕ НАЙДЕН: уровень %d/%d "  };




static wchar_t* string_660[NLANGS] = {
  L"Cap control",
  L"头部控制",
  L"Control de tapa",
  L"@",
  L"Проверка колпачка"  };

static wchar_t* string_662[NLANGS] = {
  L"Tip/Cap control",
  L"安瓿/西林瓶头部检测模式",
  L"Control punta/tapa",
  L"@",
  L"Кончик/колпачок"  };

static wchar_t* string_664[NLANGS] = {
  L"Max CAP error",
  L"最大头部",
  L"Max. error de tapa",
  L"@",
  L"Колпачок: МАX кол-во ошибок "  };

static wchar_t* string_666[NLANGS] = {
  L"Fins skipping height",
  L"飞边忽略高度",
  L"Salto altura aleta",
  L"@",
  L"Игнорируемая высота"  };
  
static wchar_t* string_670[NLANGS] = {
  L"CD Max lenght",
  L"CD最大长度",
  L"CD longitud máx.",
  L"@",
  L"MAX длина CD"  };

static wchar_t* string_671[NLANGS] = {
  L"CD Min lenght",
  L"CD最小长度",
  L"CD longitud mín.",
  L"@",
  L"MIN длина CD"  };

static wchar_t* string_672[NLANGS] = {
  L"CD Max percentage",
  L"CD最大比例值",
  L"CD porcentaje máx.",
  L"@",
  L"MAX процент CD"  };


static wchar_t* string_674[NLANGS] = {
  L"DE Max lenght",
  L"DE最大长度",
  L"DE longitud máx.",
  L"@",
  L"MAX длина DE"  };

static wchar_t* string_675[NLANGS] = {
  L"DE Min lenght",
  L"DE最小长度",
  L"DE longitud mín.",
  L"@",
  L" MIN длина DE"  };

static wchar_t* string_676[NLANGS] = {
  L"DE Max percentage",
  L"DE最大比例值",
  L"DE porcentaje máx.",
  L"@",
  L"MAX процент DE"  };

static wchar_t* string_678[NLANGS] = {
  L"DE Max angle",
  L"DE最大角度",
  L"DE ángulo máx.",
  L"@",
  L"MAX угол DE"  };

static wchar_t* string_679[NLANGS] = {
  L"DE Min angle",
  L"DE最小角度",
  L"DE ángulo mín.",
  L"@",
  L"MIN угол DE"  };


  // New ORAL


static wchar_t* string_680[NLANGS] = {
  L"AD Max lenght",
  L"AD最大长度",
  L"AD longitud máx.",
  L"@",
  L"MAX длина AD "};

static wchar_t* string_681[NLANGS] = {
  L"AD Min lenght",
  L"AD最小长度",
  L"AD longitud mín.",
  L"@",
  L"MIN длина AD "  };

static wchar_t* string_682[NLANGS] = {
  L"AD Max percentage",
  L"AD最大比例值",
  L"AD porcentaje máx.",
  L"@",
  L"MAX процент AD "  };

static wchar_t* string_684[NLANGS] = {
  L"DRL Max lenght",
  L"DRL最大长度",
  L"DRL longitud máx.",
  L"@",
  L"MAX длина DRL"  };

static wchar_t* string_685[NLANGS] = {
  L"DRL Min lenght",
  L"DRL最小长度",
  L"DRL longitud mín.",
  L"@",
  L"MIN длина DRL"  };

static wchar_t* string_687[NLANGS] = {
  L"BC Max lenght",
  L"BC最大长度",
  L"BC longitud máx.",
  L"@",
  L"MAX длина BC"  };

static wchar_t* string_688[NLANGS] = {
  L"BC Min lenght",
  L"BC最小长度",
  L"BC longitud mín.",
  L"@",
  L"MIN длина BC"  };

static wchar_t* string_689[NLANGS] = {
  L"BC Max percentage",
  L"BC最大比例值",
  L"BC porcentaje máx",
  L"@",
  L"MAX процент BC"  };

static wchar_t* string_690[NLANGS] = {
  L"BC Max angle",
  L"BC最大角度",
  L"BC ángulo máx.",
  L"@",
  L"MAX угол BC"  };

static wchar_t* string_691[NLANGS] = {
  L"BC Min angle",
  L"BC最小角度",
  L"BC ángulo mín.",
  L"@",
  L"MIN угол CB"  };






static wchar_t* string_694[NLANGS] = {
  L"CAP GOOD",
  L"头部轮廓合格",
  L"BUENA TAPA",
  L"@",
  L"ПРИГОДЕН: колпачок"  };



static wchar_t* string_696[NLANGS] = {
  L"REJECT: Wrong profile",
  L"剔除: 错误的轮廓",
  L"RECHAZO: Archivo erróneo",
  L"@",
  L"ОТБР.: Неправильный контур"  };

static wchar_t* string_698[NLANGS] = {
  L"Cannot find valid keypoints",
  L"不能找到关键点",
  L"No encuentra puntos claves validos",
  L"@",
  L" Невозможно найти ключевые точки"  };




static wchar_t* string_700[NLANGS] = {
  L"Tip control",
  L"头部检测",
  L"Control de punta",
  L"@",
  L"Инспекция кончика"  };

static wchar_t* string_702[NLANGS] = {
  L"Contour threshold (*)",
  L"轮廓搜索值 (*)",
  L"Umbral de contorno (*)",
  L"@",
  L"Определение контура (*)"  };

static wchar_t* string_704[NLANGS] = {
  L"Tip diameter",
  L"头部直径",
  L"Diámetro de punta",
  L"@",
  L"Диаметр кончика"  };
  
static wchar_t* string_706[NLANGS] = {
  L"Max curvature error (*)",
  L"最大曲线值(*)",
  L"Error de curvatura máx.",
  L"@",
  L"MAX ошибок кривой(*)"  };

static wchar_t* string_708[NLANGS] = {
  L"Max symmetry error (*)",
  L"最大对称性允许值(*)",
  L"Error de simetría máx.",
  L"@",
  L"MAX ошибок симметрии (*)"  };

static wchar_t* string_710[NLANGS] = {
  L"Max bulge error (*)",
  L"最大泡头允许值(*)",
  L"Error de abultamiento máx.",
  L"@",
  L"MAX ошибок (*)"  };

static wchar_t* string_712[NLANGS] = {
  L"Burns border",
  L"焦头边缘",
  L"Quemaduras de frontera",
  L"@",
  L"Граница пригара"  };

static wchar_t* string_714[NLANGS] = {
  L"Max burns variance",
  L"最大焦头方差",
  L"Máx. varianza quemaduras",
  L"@",
  L"MAX вариативность пригара"  };

static wchar_t* string_715[NLANGS] = {
  L"Top burns max",
  L"顶部焦头高度",
  L"Máx. quemaduras superiores",
  L"@",
  L"MAX пригар"  };

static wchar_t* string_716[NLANGS] = {
  L"Bottom burns height",
  L"底部焦头高度",
  L"Altura de quemaduras de fondo",
  L"@",
  L"Нижняя граница пригара"  };

static wchar_t* string_718[NLANGS] = {
  L"Max burns error (*)",
  L"焦头最大允许值 (*)",
  L"Máx. error de quemaduras",
  L"@",
  L"MAX ошибок пригара"  };

static wchar_t* string_720[NLANGS] = {
  L"Height LOW %d/%d/%d",
  L"头部高度太低 %d/%d/%d",
  L"Altura BAJA %d/%d/%d",
  L"@",
  L"Значение высоты низкое %d/%d/%d "  };

static wchar_t* string_722[NLANGS] = {
  L"Height HIGH %d/%d/%d",
  L"头部高度太高 %d/%d/%d",
  L"Altura ALTA %d/%d/%d",
  L"@",
  L"Значение высоты высокое %d/%d/%d "  };

static wchar_t* string_724[NLANGS] = {
  L"Height GOOD %d/%d/%d",  // max 26 long
  L"高度合格 %d/%d/%d",
  L"Altura correcta (%d/%d/%d)",
  L"@",
  L" ПРИГОДЕН: Высота (%d/%d/%d)"  };

static wchar_t* string_728[NLANGS] = {
  L" - Bulge DISABLED",
  L" - 泡头检测关闭",
  L" - Abultamiento DESHABILITADO",
  L"@",
  L"Инспекция на пузырьки ОТКЛ"  };

static wchar_t* string_730[NLANGS] = {
  L" - Bulge REJECT %d/%d",
  L" - 泡头检测剔除 %d/%d",
  L" - Abultamiento RECHAZADO %d/%d",
  L"@",
  L"ОТБР.: пузырьки %d/%d "  };

static wchar_t* string_732[NLANGS] = {
  L" - Bulge GOOD %d/%d",
  L" - 泡头检测合格 %d/%d",
  L" - Abultamiento CORRECTO %d/%d",
  L"@",
  L"Пригоден: пузырьки %d/%d "  };


static wchar_t* string_734[NLANGS] = {
  L"Burns REJECT %d/%d",
  L"焦头剔除 %d/%d",
  L"Quemaduras RECHAZADAS %d/%d",
  L"@",
  L"ОТБР.: пригар %d/%d"  };

static wchar_t* string_736[NLANGS] = {
  L"Burns GOOD %d/%d",
  L"焦头检测合格 %d/%d",
  L"Quemaduras CORRECTAS %d/%d",
  L"@",
  L"ПРИГОДЕН: пригар %d/%d"  };

static wchar_t* string_738[NLANGS] = {
  L"Burns DISABLED",
  L"焦头检测合格 DISABLED",
  L"Quemaduras DESHABILITAR",
  L"@",
  L"Инспекция пригара ОТКЛ"  };

static wchar_t* string_740[NLANGS] = {
  L" - Curv REJECT %d/%d",
  L" - 曲线不合格 %d/%d",
  L" - RECHAZO de curva %d/%d",
  L"@",
  L"ОТБР.: кривая %d/%d"  };

static wchar_t* string_742[NLANGS] = {
  L" - Curv GOOD %d/%d",
  L" - 曲线检测合格 %d/%d",
  L" - Curva CORRECTA %d/%d",
  L"@",
  L"ПРИГОДЕН: кривая %d/%d"  };

static wchar_t* string_744[NLANGS] = {
  L" - Curve DISABLED",
  L" - 曲度检测关闭",
  L" - Curva DESHABILITADA",
  L"@",
  L"Инспекция кривой ОТКЛ"  };

static wchar_t* string_746[NLANGS] = {
  L"Container profile NOT FOUND %d/%d",
  L"瓶轮廓未找到 %d/%d",
  L"Perfil de contenedores no encontrado %d/%d",
  L"@",
  L"Не найден контур контейнера %d/%d "  };

static wchar_t* string_748[NLANGS] = {
  L" - Too close to image borders",
  L" - 距离图像边缘太近",
  L" - Muy cerca para imagenes de frontera",
  L"@",
  L"Слишком близко к границам изображения"  };

static wchar_t* string_750[NLANGS] = {
  L"Learn model",
  L"学习模式",
  L"Mod. aprender",
  L"@",
  L"Учебная модель"  };

static wchar_t* string_756[NLANGS] = {
  L"REJECT: Container NOT found %d/%d",
  L"剔除: 没有找到瓶子 %d/%d",
  L"RECHAZO:Contenedor no encontrado  %d/%d",
  L"@",
  L"ОТБР.: не найден контейнер %d/%d "  };


static wchar_t* string_758[NLANGS] = {
  L"Profile GOOD %d AB:%s/%s BC:%s/%s",
  L"轮廓合格 %d AB:%s/%s BC:%s/%s",
  L"Perfil CORRECTO %d AB:%s/%s BC:%s/%s",
  L"@ %d AB:%s/%s BC:%s/%s",
  L"ПРИГОДЕН: контур %d AB:%s/%s BC:%s/%s"  };

static wchar_t* string_759[NLANGS] = {
  L"Profile GOOD %d",
  L"轮廓合格 %d",
  L"Perfil CORRECTO %d",
  L"@ %d",
  L"ПРИГОДЕН: контур %d"  };

static wchar_t* string_760[NLANGS] = {
  L"Too few points %d/%d",
  L"太少的点 %d/%d",
  L"Muy pocos puntos %d/%d",
  L"@",
  L"Не хватает контрольных точек %d/%d "  };

  //20200321
  static wchar_t* string_761[NLANGS] = {
  L"Crack points %d/%d",
  L"裂纹面积 %d/%d",
  L"Muy pocos puntos %d/%d",
  L"@",
  L"Не хватает контрольных точек %d/%d "  };

static wchar_t* string_762[NLANGS] = {
  L"CAP REJECT: Too few points %d/%d",
  L"头部剔除: 太少的点 %d/%d",
  L"RECHAZO DE TAPA: pocos puntos %d/%d",
  L"@",
  L"ОТБР. колпачок: Не хватает точек %d/%d "  };

static wchar_t* string_764[NLANGS] = {
  L"Profile NOT FOUND",
  L"易开盖轮廓未找到",
  L"Perfil no encontrado",
  L"@",
  L"Не найден контур Flip-off "  };

static wchar_t* string_766[NLANGS] = {
  L"Profile REJECT: TOO FEW POINTS %d (%d/%d)",
  L"轮廓剔除: 点数太少 %d (%d/%d)",
  L"Perfil RECHAZO: POCOS PUNTOS %d (%d/%d)",
  L"@ %d (%d/%d)",
  L"ОТБР. контур: Не хватает  точек %d (%d/%d)"  };

static wchar_t* string_768[NLANGS] = {
  L"Profile REJECT: TOO MANY POINTS %d (%d/%d)",
  L"轮廓剔除: 点数太多 %d (%d/%d)",
  L"Perfil RECHAZO: MUCHOS PUNTOS %d (%d/%d)",
  L"@ %d (%d/%d)",
  L"ОТБР. контур:  много точек %d (%d/%d)"  };


static wchar_t* string_770[NLANGS] = {
  L"REJECT: left %s too low %d (%d,%d)",
  L"剔除: 左边 %s 太低 %d (%d,%d)",
  L"RECHAZO:izquierda %s muy baja %d (%d,%d)",
  L"@",
  L"ОТБР.: слева %s  низкий %d (%d,%d)"  };

static wchar_t* string_772[NLANGS] = {
  L"REJECT: left %s too high %d (%d,%d)",
  L"剔除: 左边 %s 太高 %d (%d,%d)",
  L"RECHAZO:izquierda %s muy alta %d (%d,%d)",
  L"@",
  L"ОТБР.: слева %s  высокий %d (%d,%d)"  };

static wchar_t* string_774[NLANGS] = {
  L"REJECT: right %s too low %d (%d,%d)",
  L"剔除: 右边 %s 太低 %d (%d,%d)",
  L"RECHAZO:derecha %s muy baja %d (%d,%d)",
  L"@",
  L"ОТБР.: справа %s  низкий %d (%d,%d)"  };

static wchar_t* string_776[NLANGS] = {
  L"REJECT: right %s too high %d (%d,%d)",
  L"剔除: 右边 %s 太高 %d (%d,%d)",
  L"RECHAZO:derecha %s muy alta %d (%d,%d)",
  L"@",
  L"ОТБР.: справа %s  высокий %d (%d,%d)"  };
 

static wchar_t* string_780[NLANGS] = {
  L"REJECT: left %s too short %d (%d,%d)",
  L"剔除: 左边 %s 太短 %d (%d,%d)",
  L"RECHAZO:izquierda %s muy corto %d (%d,%d)",
  L"@",
  L"ОТБР.: слева %s  короткий %d (%d,%d)"  };

static wchar_t* string_782[NLANGS] = {
  L"REJECT: left %s too long %d (%d,%d)",
  L"剔除: 左边 %s 太长 %d (%d,%d)",
  L"RECHAZO:izquierda %s muy larga %d (%d,%d)",
  L"@",
  L"ОТБР.: слева %s  длинный %d (%d,%d)"  };

static wchar_t* string_784[NLANGS] = {
  L"REJECT: left %s perc too high %d (%d)",
  L"剔除: 左边 %s 比值太高 %d (%d)",
  L"RECHAZO:izquierda %s porcen. alto %d (%d)",
  L"@",
  L"ОТБР.: слева %s соотношение высокое %d (%d,%d)"  };



static wchar_t* string_790[NLANGS] = {
  L"REJECT: right %s too short %d (%d,%d)",
  L"剔除: 右边 %s 太短 %d (%d,%d)",
  L"RECHAZO:derecha %s muy corto %d (%d,%d)",
  L"@",
  L"ОТБР.: справа %s  короткий %d (%d,%d)"  };

static wchar_t* string_792[NLANGS] = {
  L"REJECT: right %s too long %d (%d,%d)",
  L"剔除: 右边 %s 太长 %d (%d,%d)",
  L"RECHAZO:derecha %s muy larga  %d (%d,%d)",
  L"@",
  L"ОТБР.: справа %s  длинный %d (%d,%d) "  };

static wchar_t* string_794[NLANGS] = {
  L"REJECT: right %s perc too high %d (%d)",
  L"剔除: 右边 %s 比值太高 %d (%d)",
  L"RECHAZO: derecha %s porcen. alto %d (%d)",
  L"@",
  L"ОТБР.: справа %s соотношение высокое %d (%d,%d) "   };



static wchar_t* string_796[NLANGS] = {
  L"REJECT: %s left angle too low %d (%d,%d)",
  L"剔除:  %s 左边角度太小 %d (%d,%d)",
  L"RECHAZO: %s ángulo izquierdo bajo %d (%d,%d)",
  L"@",
  L"ОТБР.: %s левый угол  мал %d (%d,%d) "  };

static wchar_t* string_797[NLANGS] = {
  L"REJECT: %s left angle too high %d (%d,%d)",
  L"剔除:  %s 左边角度太大 %d (%d,%d)",
  L"RECHAZO: %s ángulo izquierdo alto %d (%d,%d)",
  L"@",
  L"ОТБР.: %s левый угол большой %d (%d,%d)"  };


static wchar_t* string_798[NLANGS] = {
  L"REJECT: %s right angle too low %d (%d,%d)",
  L"剔除:  %s 右边角度太小 %d (%d,%d)",
  L"RECHAZO: %s ángulo derecho bajo %d (%d,%d)",
  L"@",
  L"ОТБР.: %s правый угол мал %d (%d,%d)"  };

static wchar_t* string_799[NLANGS] = {
  L"REJECT: %s right angle too high %d (%d,%d)",
  L"剔除:  %s 右边角度太大 %d (%d,%d)",
  L"RECHAZO: %s ángulo derecho alto %d (%d,%d)",
  L"@",
  L"ОТБР.: %s правый угол большой %d (%d,%d)"  };


/*
static wchar_t* string_784[NLANGS] = {
  L"Max color error",
  L"最大颜色",
  L"Error de color máx.",
  L"@",
  L"MAX ошибок цвета"  };
  */



static wchar_t* string_800[NLANGS] = {
  L"Camera[%d] %s",
  L"相机[%d] %s",
  L"Cámara[%d] %s",
  L"@",
  L"Камера [%d] %s "  };

  static wchar_t* string_801[NLANGS] = {
  L"Display selection",
  L"显示选择",
  L"@",
  L"@",
  L"@"  };


static wchar_t* string_802[NLANGS] = {
  L"Camera [%d] %s NOT AVAILABLE",
  L"相机[%d] %s 无法使用",
  L"Cámara[%d] %s NO DISPONIBLE",
  L"@",
  L"Камера [%d] %s недоступна"  }; 

static wchar_t* string_804[NLANGS] = {
  L"waiting trigger... (%d\")",
  L"等待触发 (%d\")",
  L"gatillo en espera  (%d\")",
  L"@",
  L"Ожидание запуска ... (%d\")"  };

static wchar_t* string_808[NLANGS] = {
  L"Camera[%d] %s - Insp active",
  L"相机[%d] %s - 检测激活",
  L"Cámara[%d] %s - Insp. activa",
  L"@",
  L"Камера [%d] %s – Инспекция активна "  };

static wchar_t* string_810[NLANGS] = {
  L"Camera[%d] %s - bottomlight",
  L"相机[%d] %s - 底光检测",
  L"Cámara[%d] %s - luz de fondo",
  L"@",
  L"Камера [%d] %s -нижний свет"  };

static wchar_t* string_811[NLANGS] = {
  L"Camera[%d] %s - backlight",
  L"相机[%d] %s - 背光检测",
  L"Cámara[%d] %s - luz trasera",
  L"@",
  L"Камера [%d] %s – задний свет"  };

static wchar_t* string_812[NLANGS] = {
  L"Camera[%d] %s - image sequence",
  L"相机[%d] %s - 图像序列",
  L"Cámara[%d] %s - secuencia de imágenes ",
  L"@",
  L"Камера [%d] %s - посл-ть изображений"  };

static wchar_t* string_814[NLANGS] = {
  L"Camera[%d] %s - Movie active",
  L"相机[%d] %s - 实时模式启动",
  L"Cámara[%d] %s - Película activa",
  L"@",
  L"Камера [%d] %s – Запуск режима просмотра"  };

static wchar_t* string_816[NLANGS] = {
  L"Camera[%d] %s - Closed loop %d",
  L"相机[%d] %s - 关闭闭环 %d",
  L"Cámara[%d] %s - Bucle cerrado %d",
  L"@",
  L"Камера[%d] %s - замкнутый контур %d "  };

static wchar_t* string_818[NLANGS] = {
  L"Free Running",
  L"自动拍摄",
  L"Arranque libre",
  L"@",
  L"Авто съёмка"  };

static wchar_t* string_820[NLANGS] = {
  L"Frames acquisition rate %.3lf",
  L"采集帧率 %.3lf",
  L"Taza de adquisición de marcos  %.3lf",
  L"@",
  L"Скорость сбора данных %.3lf "  };

static wchar_t* string_822[NLANGS] = {
  L"%d acquired frames (%d lost)",
  L"%d 获得帧数(%d 丢失)",
  L"%d marcos adquiridos (%d perdido)",
  L"@",
  L"%d получено кадров (%d потерь)"  };

static wchar_t* string_824[NLANGS] = {
  L"time %d us - executed %d insp - lost %d frames",
  L"%d 微秒 - %d 开始检测 - %d 丢帧 ",
  L"%d tiempo (%d ejecutado) %d lost",
  L"@",
  L"Время %d мксек – вып. %d инспекции – потеряно %d кадров"  };

static wchar_t* string_825[NLANGS] = {
  L"time %d/%d - executed %d - lost %d",
  L"%d%d - %d 执行 - %d 丢失",
  L"%d%d - tiempo %d ejecutado - %d perdido",
  L"@",
  L"время %d%d - %d выполнено - %d потеряно"  };

static wchar_t* string_826[NLANGS] = {
  L"%d inspections (%d frames lost)",
  L"%d 检测 (%d 丢帧数目)",
  L"%d inspecciones (%d marcos perdidos)",
  L"@",
  L"%d инспекция (%d кадров потеряно )"  };

static wchar_t* string_828[NLANGS] = {
  L"Inspections=%d Frames lost=%d PT=%d[ms] AF=%.1lf[fps]",   // NOTE: PT=Processing time - AF=Acquisition rate
  L"检测=%d 丢帧数目=%d PT=%d[ms] AF=%.1lf[fps]",
  L"Inspecciones=%d Marcos perdidos=%d PT=%d[ms] AF=%.1lf[fps]",
  L"@",
  L"Инспекция =%d кадров потеряно =%d PT=%d[ms] AF=%.1lf[fps]"  };

static wchar_t* string_830[NLANGS] = {
  L"inspection time %d ms",
  L"检测时间 %d",
  L"Tiempo de inspección %d ms",
  L"@",
  L"Время инспекции %d ms"  };

static wchar_t* string_832[NLANGS] = {
  L"%d insp - %d/%d frames - %d ms",
  L"%d 检测循环 - %d/%d 帧 - %d ms",
  L"%d inspec. - %d/%d marcos - %d ms",
  L"@",
  L"%d цикл инспекции- %d/%d кадров - %d ms "  };

static wchar_t* string_834[NLANGS] = {
  L"%d insp %d/%d/%d frm %d/%d ms %.1lf fps",
  L"%d 检测 %d/%d/%d帧 %d/%dms %.1lffps",
  L"%d Inspec. %d/%d/%d marcos %d/%d ms %.1lf fps",
  L"@ %d   %d/%d/%d  %d/%d  %.1lf",
  L"%d insp %d/%d/%d frm %d/%d ms %.1lf fps"  };

static wchar_t* string_836[NLANGS] = {
  L"%d/%d rej/insp %d/%d/%d img/proc/tot",
  L"%d/%d 剔除/检测 %d/%d/%d 采图/处理/总共",
  L"%d/%d rech/inspec %d/%d/%d img/proc/tot",
  L"@",
  L"%d/%d rej/insp %d/%d/%d img/proc/tot" };

static wchar_t* string_838[NLANGS] = {
  L"Cosmetic",
  L"外观",
  L"Cosmetico",
  L"@",
  L"Внешние дефекты"  };

static wchar_t* string_840[NLANGS] = {
  L"PARTICLE GOOD (%d/%d)",
  L"粒子检测合格 (%d/%d)",
  L"PARTICULA CORRECTA (%d/%d)",
  L"@",
  L"ПРИГОДЕН: включения(%d/%d)"  };

static wchar_t* string_842[NLANGS] = {
  L"PARTICLE REJECT (%d/%d)",
  L"粒子检测不合格 (%d/%d)",
  L"PARTICULA RECHAZADA (%d/%d)",
  L"@",
  L"ОТБР.: включения(%d/%d)"  };

static wchar_t* string_850[NLANGS] = {
  L"COSMETIC GOOD (%d/%d)",
  L"外观合格 (%d/%d)",
  L"COSMETICO CORRECTO (%d/%d)",
  L"@",
  L"ПРИГОДЕН: Внешние дефекты(%d/%d)"  };
  
static wchar_t* string_852[NLANGS] = {
  L"COSMETIC REJECT (%d/%d)",
  L"外观不合格 (%d/%d)",
  L"COSMETICO RECHAZADO (%d/%d)",
  L"@",
  L"ОТБР.: Внешние дефекты(%d/%d)"  };




static wchar_t* string_rejectv30[NLANGS] = {
  L"REJECT: %s %s %s (%d %d/%d)",   // REJECT left/right AB/BC too short/too long/perc too high/angle too low/angle too high
  L"剔除: %s %s %s (%d %d/%d)",
  L"RECHAZO: %s %s %s (%d %d/%d)",
  L"@: %s %s %s (%d %d/%d)",
  L"ОТБР.: %s %s %s (%d %d/%d)"  };

  
static wchar_t* string_left[NLANGS] = {
  L"Left",
  L"左边",
  L"izquierda",
  L"@",
  L"Левый"  };

static wchar_t* string_right[NLANGS] = {
  L"Right",
  L"右边",
  L"derecha",
  L"@",
  L"Правый"  };

static wchar_t* string_toolong[NLANGS] = {
  L"too long",
  L"太长",
  L"Desmasiado Largo",
  L"@",
  L"Слишком длинный"  };

static wchar_t* string_tooshort[NLANGS] = {
  L"too short",
  L"太短",
  L"Demasiado Corto",
  L"@",
  L"Короткий"  };

static wchar_t* string_perctoohigh[NLANGS] = {
  L"perc too high",
  L"比值太高",
  L"Porcentaje Muy Alto",
  L"@",
  L"Высокое соотношение"  };

static wchar_t* string_angletoohigh[NLANGS] = {
  L"angle too high",
  L"角度太大",
  L"Ángulo Muy Alto",
  L"@",
  L"Угол  большой"  };

static wchar_t* string_angletoolow[NLANGS] = {
  L"angle too low",
  L"角度太小",
  L"Ángulo Muy Bajo",
  L"@",
  L"Угол мал"  };



/*static wchar_t* string_ab[NLANGS] = {
  L"AB",
  L"AB",
  L"AB",
  L"AB",
  L"AB"  }; */






static wchar_t* string_900[NLANGS] = {
  L"MACHINE PARAMS",
  L"机器参数",
  L"PARÁMETROS MÁQUINA",
  L"@",
  L"Параметры машины"  };

static wchar_t* string_902[NLANGS] = {
  L"INSPECTION PARAMS",
  L"检测参数",
  L"PARAMETROS DE INSPEC.",
  L"@",
  L"Параметры инспекции"  };

static wchar_t* string_904[NLANGS] = {
  L"INSP (A)",
  L"检测(A)",
  L"INSPEC.(A)",
  L"@",
  L"Инспекция(A)"  };

static wchar_t* string_906[NLANGS] = {
  L"INSP (B)",
  L"检测(B)",
  L"INSPEC. (B)",
  L"@",
  L"Инспекция（B)"  };

static wchar_t* string_908[NLANGS] = {
  L"Analysis menu",
  L"分析菜单",
  L"Menú de análisis",
  L"@",
  L"Меню Анализа"  };

  /*
static wchar_t* string_820[NLANGS] = {
  L"Vision Station",
  L"视觉站位",
  L"Estación de visión",
  L"@",
  L"Станции инспекции"  };
  */

/*
static wchar_t* string_902[NLANGS] = {
  L"START CALIBRATION",
  L"开始校准",
  L"COMENZAR CALIBRACIÓN",
  L"@",
  L"Старт калибровки"  };
  */

static wchar_t* string_910[NLANGS] = {
  L"Process test",
  L"工艺试验",
  L"Prueba de proceso",
  L"@",
  L"Тест процесса"  };

static wchar_t* string_912[NLANGS] = {
  L"Acquire sequence",
  L"获取序列图像",
  L"Secuencia adquirida",
  L"@",
  L"Получить посл-ность изображений"  };


static wchar_t* string_920[NLANGS] = {
  L"LIQUID",
  L"水针",
  L"&",
  L"@",
  L"Жидкость"  };

static wchar_t* string_922[NLANGS] = {
  L"FREEZE/D",
  L"冻干",
  L"&",
  L"@",
  L"Лиофилизация"  };




static wchar_t* string_930[NLANGS] = {
  L"Filling lum threshold",
  L"装量亮度阈值",
  L"Umbral lum llenado",
  L"@",
  L"Порог яркости наполнения"  };

static wchar_t* string_932[NLANGS] = {
  L"Maximum filling level (*)",
  L"最大装量 (*)",
  L"Máx. nivel de llenado",
  L"@",
  L"MAX уровень розлива (*)"  };

  //Pour added 20181027
  static wchar_t* string_932a[NLANGS] = {
  L"Maximum filling level 1(*)",
  L"最大装量1 (*)",
  L"Máx. nivel de llenado",
  L"@",
  L"MAX уровень розлива (*)"  };

  static wchar_t* string_932b[NLANGS] = {
  L"Maximum filling level 2(*)",
  L"最大装量2 (*)",
  L"Máx. nivel de llenado",
  L"@",
  L"MAX уровень розлива (*)"  };

static wchar_t* string_934[NLANGS] = {
  L"Minimum filling level (*)",
  L"最小装量 (*)",
  L"Mín. nivel de llenado",
  L"@",
  L"MIN уровень розлива (*)"  };

  //Pour added 20181027
  static wchar_t* string_934a[NLANGS] = {
  L"Minimum filling level 1(*)",
  L"最小装量1 (*)",
  L"Mín. nivel de llenado",
  L"@",
  L"MIN уровень розлива (*)"  };

    static wchar_t* string_934b[NLANGS] = {
  L"Minimum filling level 2(*)",
  L"最小装量2 (*)",
  L"Mín. nivel de llenado",
  L"@",
  L"MIN уровень розлива (*)"  };




static wchar_t* string_950[NLANGS] = {
  L"Calibration slope",
  L"标定斜率",
  L"Pendiente de calibración",
  L"@",
  L"Калибровка наклона "  };

static wchar_t* string_951[NLANGS] = {
  L"Calibration offset",
  L"校准偏差",
  L"Desplazamiento de calibración",
  L"@",
  L"Ошибка калибровки"  };

static wchar_t* string_972[NLANGS] = {
  L"Error during reading of recipe <%s>",
  L"读取配方<%s>错误",
  L"Error durante la lectura de receta <%s>",
  L"@",
  L"Ошибка чтения рецепта <%s>"  };

static wchar_t* string_974[NLANGS] = {
  L"Recipe <%s> <%s> read and decoded ( %d controls - %d heights )",
  L"配方<%s> <%s> 阅读和解码 ( %d 控制 - %d高度 )",
  L"Receta <%s> <%s> leida y decodificada( %d controles - %d alturas )",
  L"@",
  L"Рецепт <%s> <%s>Чтение и расшифровка( %d контроль - %d высота )"  };

static wchar_t* string_1030[NLANGS] = {
  L"Set input/output paths",
  L"设置输入/输出路径",
  L"Establecer rutas de entrada/salida",
  L"@",
  L"Установить пути вывода / ввода"  };


static wchar_t* string_1032[NLANGS] = {
  L"Please select input path to read recipes from",
  L"请选择输入路径来从......阅读配方",
  L"seleccione la ruta de entrada para leer recetas de",
  L"@",
  L" Выберите путь для открытия рецептов"  };

static wchar_t* string_1034[NLANGS] = {
  L"Please select output path to write results to",
  L"请选择输出路径来给......写结果",
  L"Selecione la ruta de salida para anexar los resultados a",
  L"@",
  L" Выберите путь для записи результатов"  };

    static wchar_t* string_1035[NLANGS] = {
  L"Annotate on",
  L"添加注释",
  L"@",
  L"@",
  L"@"  };
 

static wchar_t* string_1036[NLANGS] = {
  L"Login control",
  L"登录控制",
  L"@",
  L"@",
  L"элемент управления входом"  };

static wchar_t* string_1037[NLANGS] = {
  L"Change password period",
  L"更改密码周期",
  L"@",
  L"@",
  L"изменить срок действия пароля"  };

static wchar_t* string_1038[NLANGS] = {
  L"Auto logout period",
  L"自动登出周期",
  L"@",
  L"@",
  L"период автоматического аннулирования"  };


static wchar_t* string_1039[NLANGS] = {
  L"Logout",
  L"登出",
  L"Cerrar sesión",
  L"@",
  L"Выйти"  };

static wchar_t* string_1040[NLANGS] = {
  L"Set userids/passwords",
  L"设置用户ID/密码",
  L"Establecer ID de usuarios/contraseñas ",
  L"@",
  L"Установить ID пользователя / пароль"  };

static wchar_t* string_1041[NLANGS] = {
  L"Please insert user name",
  L"请输入用户名",
  L"Inserte el nombre de usuario ",
  L"@",
  L" Введите имя пользователя"  };

static wchar_t* string_1042[NLANGS] = {
  L"Please insert password",
  L"请输入密码",
  L"Inserte la contraseña",
  L"@",
  L" Введите пароль"  };


  static wchar_t* string_1043[NLANGS] = {
  L"Please note the reason of operation",
  L"请注释操作原因",
  L"@",
  L"@",
  L"@"  };


static wchar_t* string_1046[NLANGS] = {
  L"Please insert new user-id for <%s> user",
  L"请为<%s> 输入新用户名",
  L"Inserte un nuevo ID de usuario %s",
  L"@",
  L"Введите новый ID  для <% s> пользователя"  };

static wchar_t* string_1047[NLANGS] = {
  L"Please confirm new user-id for <%s> user",
  L"请为<%s> 确定新用户名 ",
  L"Confirme el nuevo ID de usuario %s",
  L"@",
  L"Подтвердите новый ID для <% s> пользователя"  };

static wchar_t* string_1048[NLANGS] = {
  L"Please insert new password for <%s> user",
  L"请为<%s> 输入新的密码",
  L"Inserte la nueva contraseña %s",
  L"@",
  L"Введите новый пароль для  <% s>,"  };

static wchar_t* string_1049[NLANGS] = {
  L"Please confirm new password for <%s> user",
  L"请为<%s> 确认新密码",
  L"Confirme la nueva contraseña de usuario %s",
  L"@",
  L"Подтвердите новый пароль для <% s> "  };



// AUTOMATION/MACHINE PARAMETERS
static wchar_t* string_1050[NLANGS] = {
  L"Manual Movements",
  L"手动运转",
  L"Movimientos manuales",
  L"@",
  L"Проведение операций вручную"  };

static wchar_t* string_1052[NLANGS] = {
  L"Manual Movements are not allowed in this machine's status",
  L"机器当前状态不允许手动运转",
  L"Movimientos manuales no son perimitidos en este estatus",
  L"@",
  L"Проведение операций вручную не допускаются в этом статусе"  };

static wchar_t* string_1054[NLANGS] = {
  L"Current axis position: %d",
  L"当前轴位置: %d",
  L"Posición actual del eje",
  L"@",
  L"Текущее положение оси: %d"  };

static wchar_t* string_1060[NLANGS] = {
  L"JOG+",
  L"点动+",
  L"JOG+",
  L"@",
  L"JOG+"  };

static wchar_t* string_1062[NLANGS] = {
  L"JOG-",
  L"点动-",
  L"JOG-",
  L"@",
  L"JOG-"  };

static wchar_t* string_1064[NLANGS] = {
  L"Start Homing",
  L"开始归零",
  L"Comienzo original",
  L"@",
  L"Начать с исходной позиции"  };

static wchar_t* string_1066[NLANGS] = {
  L"STOP",
  L"停止",
  L"PARO",
  L"@",
  L"СТОП"  };

static wchar_t* string_1078[NLANGS] = {
  L"Execute",
  L"执行",
  L"Ejecutar",
  L"@",
  L"Выполнить"  };

static wchar_t* string_1080[NLANGS] = {
  L"Motor index",
  L"电机指数",
  L"Indice del motor",
  L"@",
  L"Индекс двигателя"  };

static wchar_t* string_1082[NLANGS] = {
  L"Max speed",
  L"最大速度",
  L"Velocidad máx.",
  L"@",
  L"MAX скорость"  };

static wchar_t* string_1084[NLANGS] = {
  L"Required position",
  L"指定位置",
  L"Posición necesaria",
  L"@",
  L"Требуемая позиция"  };

static wchar_t* string_1112[NLANGS] = {
  L"Homing position",
  L"归零位置",
  L"Posición de origen",
  L"@",
  L"Исходная позиция"  };

static wchar_t* string_1118[NLANGS] = {
  L"Photocell position",
  L"光电传感器位置",
  L"Posición de fotocelda",
  L"@",
  L"Позиция датчиков"  };

static wchar_t* string_1120[NLANGS] = {
  L"Unloading distance",
  L"卸载距离",
  L"Distancia de descarga",
  L"@",
  L"Расстояние выгрузки"  };

static wchar_t* string_1134[NLANGS] = {
  L"Photocell offset",
  L"传感器偏移",
  L"Composición fotocelda",
  L"@",
  L"Смещение датчика"  };

static wchar_t* string_1146[NLANGS] = {
  L"Zero height position",
  L"原点高度位置",
  L"Posición de altura en ceros",
  L"@",
  L"Положение нулевой высоты"  };

static wchar_t* string_1208[NLANGS] = {
  L"Jerk/smoothness",
  L"平滑度",
  L"Estiron/suavidad",
  L"@",
  L"Шероховатость"  };

static wchar_t* string_1210[NLANGS] = {
  L"Acceleration",
  L"加速",
  L"Aceleración",
  L"@",
  L"Ускорение"  };

static wchar_t* string_1212[NLANGS] = {
  L"Deceleration",
  L"减速",
  L"Desaceleración",
  L"@",
  L"Замедление"  };

static wchar_t* string_1214[NLANGS] = {
  L"Maximum speed",
  L"最大速度",
  L"Velocidad máx.",
  L"@",
  L"MAX скорость"  };

static wchar_t* string_1216[NLANGS] = {
  L"Number of rotations",
  L"旋转圈数",
  L"Número de revoluciones",
  L"@",
  L"Число оборотов"  };

static wchar_t* string_1218[NLANGS] = {
  L"Total cycle span",
  L"总循环跨度",
  L"Lapso de ciclo total",
  L"@",
  L"Общая продолжительность цикла"  };

static wchar_t* string_1220[NLANGS] = {
  L"Inspection test loop",
  L"灯检测试循环",
  L"Bucle prueba de inspec.",
  L"@",
  L"Цикл инспекции"  };

static wchar_t* string_1222[NLANGS] = {
  L"Continuous rotation",
  L"连续旋转",
  L"Rotación continua",
  L"@",
  L"Непрерывное вращение"  };

static wchar_t* string_1224[NLANGS] = {
  L"Cosmetic1 test loop",
  L"外观1灯检测试循环",
  L"#Bucle prueba de inspec.",
  L"@",
  L"&Цикл инспекции"  };

static wchar_t* string_1226[NLANGS] = {
  L"Cosmetic2 test loop",
  L"外观2灯检测试循环",
  L"# Bucle prueba de inspec.",
  L"@ ",
  L"& Цикл инспекции"  };




// BATCH

static wchar_t* string_1300[NLANGS] = {
  L"BATCHES",
  L"批次",
  L"LOTES",
  L"@",
  L"Партия"  };

static wchar_t* string_1304[NLANGS] = {
  L"Start new batch",
  L"开始新的批次",
  L"Iniciar nuevo lote",
  L"@",
  L"Начать новую партию"  };

static wchar_t* string_1306[NLANGS] = {
  L"End current batch",
  L"结束当前批次",
  L"Terminar lote",
  L"@",
  L"Закончить текущую партию"  };

static wchar_t* string_1308[NLANGS] = {
  L"Print report",
  L"显示批次信息",
  L"Imprimir reporte",
  L"@",
  L"Печать отчета"  };

static wchar_t* string_1310[NLANGS] = {
  L"Reset current statistics",
  L"重置当前统计",
  L"Reiniciar estadísticas actuales",
  L"@",
  L"Сброс текущей статистики"  };

static wchar_t* string_1312[NLANGS] = {
  L"Are you sure to reset current statistics?",
  L"确认重置当前统计?",
  L"¿Esta seguro de reiniciar las estadísticas?",
  L"@",
  L"Сбросить текущую статистику?"  };

static wchar_t* string_1314[NLANGS] = {
  L"Batch < %s / %s > started",
  L"批次 < %s / %s > 开始",
  L"# < %s / %s >",
  L"@ < %s / %s >",
  L"партия < %s / %s >Старт"  };

static wchar_t* string_1316[NLANGS] = {
  L"Batch < %s / %s > ended",
  L"批次 < %s / %s > 结束",
  L"# < %s / %s >",
  L"@ < %s / %s >",
  L"партия< %s / %s >Финиш"  };

static wchar_t* string_1318[NLANGS] = {
  L"Batch statistics resetted",
  L"批次统计重置",
  L"#",
  L"@",
  L"Обнуление партия"  };




static wchar_t* string_1320[NLANGS] = {
  L"empty",
  L"空的",
  L"vacío",
  L"@",
  L"пустой"  };

static wchar_t* string_1322[NLANGS] = {
  L"BATCH DATA",
  L"批次数据",
  L"Información lote",
  L"@",
  L"Данные партии"  };

static wchar_t* string_1324[NLANGS] = {
  L"Operator",
  L"操作人",
  L"Nombre del operador",
  L"@",
  L"Имя оператора"  };

  /*
static wchar_t* string_1326[NLANGS] = {
  L"Product name",
  L"品名/规格",
  L"Nombre del lote",
  L"@",
  L"Наименование продуктов"  };
*/

static wchar_t* string_1326[NLANGS] = {
  L"Product name",
  L"品名/规格",
  L"Nombre del lote",
  L"@",
  L"Наименование продуктов"  };


/*
static wchar_t* string_1328[NLANGS] = {
  L"Batch code",
  L"批次代码",
  L"Código de lote",
  L"@",
  L"Код партии"  };
  */

static wchar_t* string_1328[NLANGS] = {
  L"Batch number",
  L"生产批号",
  L"Código de lote",
  L"@",
  L"Код партии"  };

  static wchar_t* string_1328a[NLANGS] = {
  L"Batch number:<%s>, product name:<%s>",
  L"生产批号:<%s>,产品名称<%s>",
  L"Código de lote",
  L"@",
  L"Код партии"  };

    static wchar_t* string_1328b[NLANGS] = {
  L"No batches started",
  L"批次未开始",
  L"Código de lote",
  L"@",
  L"Код партии"  };


static wchar_t* string_1329[NLANGS] = {
  L"Batch size",
  L"批次大小",
  L"#",
  L"@",
  L"Размер партии"  };

  static wchar_t* string_1329a[NLANGS] = {
  L"Batch code",
  L"批次大小",
  L"#",
  L"@",
  L"Размер партии"  };



/*
static wchar_t* string_1330[NLANGS] = {
  L"Batch date",
  L"生产日期",
  L"Descripción de lote",
  L"@",
  L"Дата изготовления"  };
*/

  
static wchar_t* string_1330a[NLANGS] = {
  L"Batch description",
  L"批次描述",
  L"Descripción de lote",
  L"@",
  L"Дата изготовления"  };


static wchar_t* string_1330[NLANGS] = {
  L"Batch date",
  L"机检日期",
  L"Descripción de lote",
  L"@",
  L"Дата изготовления"  };

static wchar_t* string_1332[NLANGS] = {
  L"Confirm",
  L"确认",
  L"Confirmaci{on",
  L"@",
  L"Подтвердить"  };

static wchar_t* string_1334[NLANGS] = {
  L"Cancel",
  L"取消",
  L"Cancelar",
  L"@",
  L"Отмена"  };

static wchar_t* string_1336[NLANGS] = {
  L"PLEASE INSERT DATA TO START A NEW BATCH",
  L"请输入数据开始新的批次",
  L"INGRESE LA INFO. PARA UN NUEVO LOTE ",
  L"@",
  L"Введите данные, чтобы начать новую партию"  };

static wchar_t* string_1338[NLANGS] = {
  L"ARE YOU SURE TO END CURRENT ACTIVE BATCH ?",
  L"确定终止当前批次？",
  L"¿ESTA SEGURO DE TERMINAR EL LOTE ACTUAL?",
  L"@",
  L"Завершить текущую партию?"  };

  //Pour added for Sunho on 202020503
  static wchar_t* string_1340[NLANGS] = {
  L"CURRENT BATCH HAS ALREADY FINISHED, DO YOU WANT TO PRINT REPORT AGAIN?",
  L"当前批次已完成，再次打印报告？",
  L"¿ESTA SEGURO DE TERMINAR EL LOTE ACTUAL?",
  L"@",
  L"Завершить текущую партию?"  };

    static wchar_t* string_1341[NLANGS] = {
  L"Are you sure to delete current administrator?",
  L"确定删除该用户？",
  L"¿ESTA SEGURO DE TERMINAR EL LOTE ACTUAL?",
  L"@",
  L"Завершить текущую партию?"  };


// NEW TOPLIGHT CAP INSPECTION
static wchar_t* string_1402[NLANGS] = {
  L"Toplight contour disabled",
  L"顶光轮廓检测屏蔽",
  L"Contador de luz superior deshabilitado",
  L"@",
  L"Верхний свет отключен"  };


static wchar_t* string_1404[NLANGS] = {
  L"Toplight parameters",
  L"顶部光源参数",
  L"Parámetros de luz superior",
  L"@",
  L"Параметры верхнего света"  };

static wchar_t* string_1405[NLANGS] = {
  L"Toplight parameters (2)",
  L"顶部光源参数 (2)",
  L"Parámetros de luz superior (2)",
  L"@ (2)",
  L"Параметры верхнего света (2)"  };


static wchar_t* string_1406[NLANGS] = {
  L"Backlight parameters",
  L"背部光源参数",
  L"Parámetros de luz trasera",
  L"@",
  L"Параметры заднего света"  };

static wchar_t* string_1408[NLANGS] = {
  L"Tip/cap parameters",
  L"安瓿/西林瓶头部检测参数",
  L"Parámetros de punta/tapa",
  L"@",
  L"Параметры кончика ампулы/колпачка"  };

  
// ALUMINIUM STRINGS
static wchar_t* string_1410[NLANGS] = {
  L"ALU Vertical offset",
  L"铝盖竖直补偿",
  L"Compensación vertical ALU",
  L"@",
  L" Вертикальное смещение колпачка"  };

static wchar_t* string_1412[NLANGS] = {
  L"ALU Analysis width",
  L"铝盖分析宽度",
  L"Análisis de grosor ALU",
  L"@",
  L"Анализ ширины колпачка"  };

static wchar_t* string_1414[NLANGS] = {
  L"ALU Analysis height",
  L"铝盖分析高度",
  L"Análisis de peso ALU",
  L"@",
  L"Анализ высоты колпачка"  };

static wchar_t* string_1416[NLANGS] = {
  L"ALU change threshold",
  L"铝盖差值门槛",
  L"Cambio de umbral ALU",
  L"@",
  L"Порог изменения колпачка"  };

static wchar_t* string_1420[NLANGS] = {
  L"ALU max brightness",
  L"铝盖最高亮度",
  L"Máx. brillo ALU",
  L"@",
  L"MAX яркость колпачка"  };

static wchar_t* string_1422[NLANGS] = {
  L"ALU min brightness",
  L"铝盖最低亮度",
  L"Mín brillo ALU",
  L"@",
  L"MIN яркость колпачка"  };

static wchar_t* string_1424[NLANGS] = {
  L"ALU analysis lenght",
  L"铝盖分析长度",
  L"Análisis de longitud ALU",
  L"@",
  L"Анализ длины колпачка"  };

static wchar_t* string_1426[NLANGS] = {
  L"ALU maximum error (*)",
  L"铝盖最大值 (*)",
  L"Error máx. ALU",
  L"@",
  L"MAX ошибок колпачка (*)"  };

  //Pour added 20180927

  static wchar_t* string_1426a[NLANGS] = {
  L"ALU maximum error 1 (*)",
  L"铝盖最大值1 (*)",
  L"Error máx. ALU",
  L"@",
  L"MAX ошибок колпачка (*)"  };

  static wchar_t* string_1426b[NLANGS] = {
  L"ALU maximum error 2 (*)",
  L"铝盖最大值2 (*)",
  L"Error máx. ALU",
  L"@",
  L"MAX ошибок колпачка (*)"  };

// FLIP OFF STRINGS

static wchar_t* string_1430[NLANGS] = {
  L"FO Vertical offset",
  L"易开盖竖直补偿",
  L"Compensación vertical FO",
  L"@",
  L"Вертикальное смещение  Flip-off"  };

static wchar_t* string_1432[NLANGS] = {
  L"FO Analysis width",
  L"易开盖分析宽度",
  L"Análisis de grosor FO",
  L"@",
  L"Анализ ширины Flip-off"  };

static wchar_t* string_1434[NLANGS] = {
  L"FO Analysis height",
  L"易开盖分析高度",
  L"Análisis de peso FO",
  L"@",
  L"Анализ высоты Flip-off"  };

static wchar_t* string_1436[NLANGS] = {
  L"FO change threshold",
  L"易开盖差值门槛",
  L"Cambio de umbral FO",
  L"@",
  L"Порог изменения колпачка Flip-off"  };

static wchar_t* string_1440[NLANGS] = {
  L"FO maximum brightness",
  L"易开盖最高亮度",
  L"Brillo Máx. FO",
  L"@",
  L"MAX яркость  Flip-off"  };

static wchar_t* string_1442[NLANGS] = {
  L"FO minimum brightness",
  L"易开盖最低亮度",
  L"Brillo mín. FO",
  L"@",
  L"MIN яркость Flip-off"  };

static wchar_t* string_1444[NLANGS] = {
  L"FO analysis lenght",
  L"易开盖分析长度",
  L"Análisis de longitud FO",
  L"@",
  L"Анализ длины Flip-off"  };

static wchar_t* string_1446[NLANGS] = {
  L"FO maximum error (*)",
  L"易开盖最大值(*)",
  L"Error máx FO",
  L"@",
  L"MAX ошибок Flip-off (*)"  };

//Pour added 20180927

  static wchar_t* string_1446a[NLANGS] = {
  L"FO maximum error 1 (*)",
  L"易开盖最大值1 (*)",
  L"Error máx FO",
  L"@",
  L"MAX ошибок Flip-off (*)"  };

  static wchar_t* string_1446b[NLANGS] = {
  L"FO maximum error 2 (*)",
  L"易开盖最大值2 (*)",
  L"Error máx FO",
  L"@",
  L"MAX ошибок Flip-off (*)"  };


static wchar_t* string_1450[NLANGS] = {
  L"FlipOff REJECT %d,%d",
  L"易开盖剔除 %d,%d",
  L" RECHAZO de presintos %d,%d",
  L"@",
  L"ОТБР.:  Flip-off %d,%d "  };

static wchar_t* string_1452[NLANGS] = {
  L"FlipOff GOOD %d,%d",
  L"易开盖合格 %d,%d",
  L"Presinto CORRECTO %d,%d",
  L"@",
  L"ПРИГОДЕН:  Flip-off %d,%d"  };

static wchar_t* string_1454[NLANGS] = {
  L"FlipOff DISABLED",
  L"易开盖分析屏蔽",
  L"DESHABILITAR presintos",
  L"@",
  L"Инспекция Flip-off ОТКЛ "  };

static wchar_t* string_1460[NLANGS] = {
  L"Aluminium REJECT %d,%d",
  L"铝盖剔除 %d,%d",
  L"RECHAZO de aluminio %d,%d",
  L"@",
  L"ОТБР.: колпачок %d,%d "  };

static wchar_t* string_1462[NLANGS] = {
  L"Aluminium GOOD %d,%d",
  L"铝盖合格 %d,%d",
  L"Alumnio CORRECTO %d,%d",
  L"@",
  L"ПРИГОДЕН: колпачок %d,%d"  };

static wchar_t* string_1464[NLANGS] = {
  L"Aluminium DISABLED",
  L"铝盖分析屏蔽",
  L"DESHABILITAR alumnio",
  L"@",
  L"Инспекция колпачка ОТКЛ"  };


static wchar_t* string_1470[NLANGS] = {
  L"Stopper REJECT %d,%d",
  L"胶塞剔除 %d,%d",
  L"RECHAZO de tapón %d,%d",
  L"@",
  L"ОТБР.: пробка %d,%d"  };

static wchar_t* string_1472[NLANGS] = {
  L"Stopper GOOD %d,%d",
  L"胶塞检测合格 %d,%d",
  L"Tapón CORRECTO %d,%d",
  L"@",
  L"ПРИГОДЕН: пробка %d,%d"  };

static wchar_t* string_1474[NLANGS] = {
  L"Stopper DISABLED",
  L"胶塞检测关闭",
  L"DESHABILITAR tapón",
  L"@",
  L"Инспекция пробки ОТКЛ"  };

// Lower stopper results
static wchar_t* string_1470a[NLANGS] = {
  L"Lowstop REJECT %d,%d",
  L"胶塞下部剔除 %d,%d",
  L"# RECHAZO de tapón %d,%d",
  L"@",
  L"& ОТБР.: пробка %d,%d"  };

static wchar_t* string_1472a[NLANGS] = {
  L"Lowstop GOOD %d,%d",
  L"胶塞下部合格 %d,%d",
  L"# Tapón CORRECTO %d,%d",
  L"@",
  L"& ПРИГОДЕН: пробка %d,%d"  };

static wchar_t* string_1474a[NLANGS] = {
  L"Lowstop DISABLED",
  L"胶塞下部关闭",
  L"# DESHABILITAR tapón",
  L"@",
  L"& Инспекция пробки ОТКЛ"  };





static wchar_t* string_1476[NLANGS] = {
  L"Stopper powder DISABLED",
  L"胶塞带药检测关闭",
  L"DESHABILITAR tapón #",
  L"@",
  L"Инспекция пробки с продуктом ОТКЛ"  };



static wchar_t* string_1480[NLANGS] = {
  L"BACK %s %d/%d (%d) TOP %s %d/%d (%d)",
  L"背光 %s %d/%d (%d) 顶光 %s %d/%d (%d)",
  L"ATRAS %s %d/%d (%d) SUPERIOR %s %d/%d (%d)",
  L"@",
  L"Задний свет %s %d/%d (%d) верхний свет %s %d/%d (%d)"  };


// SIDE STOPPER PARAMETERS STRINGS

static wchar_t* string_1484[NLANGS] = {
  L"Stopper Vertical offset",
  L"胶塞竖直补偿",
  L"Compensación vertical tapón",
  L"@",
  L"Вертикальное смещение пробки"  };

static wchar_t* string_1486[NLANGS] = {
  L"Stopper Analysis width",
  L"胶塞分析宽度",
  L"Análisis de ancho tapón",
  L"@",
  L"Анализ ширины пробки"  };

static wchar_t* string_1488[NLANGS] = {
  L"Stopper Analysis height",
  L"胶塞分析高度",
  L"Análisis de altura tapón",
  L"@",
  L"Анализ высоты пробки"  };

static wchar_t* string_1490[NLANGS] = {
  L"Stopper change threshold",
  L"胶塞差值门槛",
  L"Cambio de umbral tapón",
  L"@",
  L"Порог изменения пробки"  };

static wchar_t* string_1492[NLANGS] = {
  L"Stopper max brightness",
  L"胶塞最高亮度",
  L"Brillo máx. tapón",
  L"@",
  L"MAX яркость пробки"  };

static wchar_t* string_1494[NLANGS] = {
  L"Stopper min brightness",
  L"胶塞最低亮度",
  L"Brillo mín. tapón",
  L"@",
  L"MIN яркость пробки"  };

static wchar_t* string_1496[NLANGS] = {
  L"Stopper analysis lenght",
  L"胶塞分析长度",
  L"Analísis longitud tapón",
  L"@",
  L"Анализ длины пробки"  };

static wchar_t* string_1498[NLANGS] = {
  L"Stopper maximum error (*)",
  L"胶塞最大 (*)",
  L"Error máx. tapón",
  L"@",
  L"MAX ошибок пробки(*)"  };



// LOWER STOPPER PARAMETERS STRINGS

static wchar_t* string_1484a[NLANGS] = {
  L"Lowstop Vertical offset",
  L"胶塞下部竖直补偿",
  L"# Compensación vertical tapón",
  L"@",
  L"& Вертикальное смещение пробки"  };

static wchar_t* string_1486a[NLANGS] = {
  L"Lowstop Analysis width",
  L"胶塞下部分析宽度",
  L"# Análisis de ancho tapón",
  L"@",
  L"& Анализ ширины пробки"  };

static wchar_t* string_1488a[NLANGS] = {
  L"Lowstop Analysis height",
  L"胶塞下部分析高度",
  L"# Análisis de altura tapón",
  L"@",
  L"& Анализ высоты пробки"  };

static wchar_t* string_1490a[NLANGS] = {
  L"Lowstop change threshold",
  L"胶塞下部差值门槛",
  L"# Cambio de umbral tapón",
  L"@",
  L"& Порог изменения пробки"  };

static wchar_t* string_1492a[NLANGS] = {
  L"Lowstop max brightness",
  L"胶塞下部最高亮度",
  L"# Brillo máx. tapón",
  L"@",
  L"& MAX яркость пробки"  };

static wchar_t* string_1494a[NLANGS] = {
  L"Lowstop min brightness",
  L"胶塞下部最低亮度",
  L"# Brillo mín. tapón",
  L"@",
  L"& MIN яркость пробки"  };

static wchar_t* string_1496a[NLANGS] = {
  L"Lowstop analysis lenght",
  L"胶塞下部分析长度",
  L"# Analísis longitud tapón",
  L"@",
  L"& Анализ длины пробки"  };

  static wchar_t* string_1498a[NLANGS] = {
  L"Stopper maximum error 1(*)",
  L"胶塞最大1 (*)",
  L"Error máx. tapón",
  L"@",
  L"MAX ошибок пробки(*)"  };

  static wchar_t* string_1498b[NLANGS] = {
  L"Stopper maximum error 2(*)",
  L"胶塞最大2 (*)",
  L"Error máx. tapón",
  L"@",
  L"MAX ошибок пробки(*)"  };



// MOTION PARAMETERS

  static wchar_t* string_1504[NLANGS] = {
  L"Carousel height",
  L"主轴高度",
  L"Altura carrusel",
  L"@",
  L"Высота башни"  };


  static wchar_t* string_1510[NLANGS] = {
  L"Motion parameters",
  L"运动参数",
  L"Parámetros movimiento",
  L"@",
  L"Параметры движения"  };

  static wchar_t* string_1520[NLANGS] = {
  L"Production speed",
  L"生产速度",
  L"Vel. Producción",
  L"@",
  L"Производительность"  };

  static wchar_t* string_1530[NLANGS] = {
  L"Jog speed",
  L"点动速度",
  L"Vel. jog",
  L"@",
  L"Пошаговая скорость"  };

  static wchar_t* string_1532[NLANGS] = {
  L"Production speed",
  L"生产高速",
  L"Vel. Producción",
  L"@",
  L"Производительность"  };

  static wchar_t* string_1534[NLANGS] = {
  L"Jog speed MEDIUM",
  L"点动中速",
  L"Media vel. jog",
  L"@",
  L"Средняя пошаговая скорость"  };

  static wchar_t* string_1536[NLANGS] = {
  L"Jog speed",
  L"点动速度",
  L"Vel. jog",
  L"@",  
  L"Скорость при пошаговом"   };

   static wchar_t* string_1540[NLANGS] = {
  L"Carousel zero position",
  L"主轴归零位置",
  L"Posición inicial carrusel",
  L"@",
  L"Исх. поз. башни"  };

  static wchar_t* string_1542[NLANGS] = {
  L"Infeed screw zero position",
  L"进料螺杆归零位置",
  L"Posición inicial husillo ent.",
  L"@",
  L"Исх. поз. подающего шнека"  };

  static wchar_t* string_1544[NLANGS] = {
  L"Tracker zero position",
  L"跟随归零位置",
  L"Posición inicial rastreador",
  L"@",
  L"Нуль трекера"  };

  static wchar_t* string_1545[NLANGS] = {
  L"Outfeed screw 1 zero position",
  L"出料螺杆一归零位置",
  L"Posición inicial husillo ent.",
  L"@",
  L"Исх. поз. подающего шнека"  };

  static wchar_t* string_1546[NLANGS] = {
  L"Cam zero position",
  L"第二反馈原点位置",
  L"Posición inic. cámara",
  L"@",
  L"Исх. поз. камеры"  };

  static wchar_t* string_1547[NLANGS] = {
  L"Outfeed screw 2 zero position",
  L"出料螺杆二归零位置",
  L"Posición inicial husillo ent.",
  L"@",
  L"Исх. поз. подающего шнека"  };

  static wchar_t* string_1548[NLANGS] = {
  L"Infeed starwheel zero position",
  L"进料星轮原点位置",
  L"Pos. incial rueda estrella ent.",
  L"@",
  L"Исх. поз. колеса-звёздочки"  };

  static wchar_t* string_1549[NLANGS] = {
  L"Outfeed starwheel zero position",
  L"出料星轮原点位置",
  L"#",
  L"@",
  L"$"  };


  static wchar_t* string_1550[NLANGS] = {
  L"Infeed belt speed(*)",
  L"进料输送带速度(*)",
  L"Vel. banda entrada(*)",
  L"@",
  L"Скорость транспортёра заг. (*)"  };

  //Pour added 20180821
    static wchar_t* string_1550a[NLANGS] = {
  L"FAN speed(*)",
  L"风机速度(*)",
  L"Vel. banda entrada(*)",//to be corrected
  L"@",
  L"Скорость транспортёра заг. (*)"  };
  //Pour added 20180821

  static wchar_t* string_1552[NLANGS] = {
  L"Outfeed belt speed(*)",
  L"出料输送带速度(*)",
  L"Vel. banda salida(*)",
  L"@",
  L"Скорость выгр. конвейера (*)"  };

  static wchar_t* string_1554[NLANGS] = {
  L"Pre-input belt speed(*)",
  L"进料网带前网带速度(*)",
  L"Vel. banda pre-entrada(*)",
  L"@",
  L"Скорость загр. конвейера(*)"  };

  static wchar_t* string_1556[NLANGS] = {
  L"Back belt speed(*)",
  L"运输网带速度(*)",
  L"Vel. banda trasera(*)",
  L"@",
  L"Скорость заднего транспортёра (*)"  };

  static wchar_t* string_1558[NLANGS] = {
  L"Rotary table speed(*)",
  L"转盘速度(*)",
  L"Vel. mesa rotatoria(*)",
  L"@",
  L"Скорость стола (*)"  };

  static wchar_t* string_1560[NLANGS] = {
  L"Input starwheel holes",
  L"进料星轮齿数 ",
  L"Orificios rueda estrella ent.",
  L"@",
  L"Кол-во пазов колеса-звёздочки"  };

  static wchar_t* string_1562[NLANGS] = {
  L"Tracking percentage",
  L"跟随比",
  L"Porcentaje de Rastreo",
  L"@",
  L"Процент отслеживания"  };

  static wchar_t* string_1564[NLANGS] = {
  L"Reject forcing",
  L"强制剔除",
  L"#",
  L"@",
  L"&"  };

    static wchar_t* string_1566[NLANGS] = {
  L"Dist from Input to screw Syc",
  L"螺杆同步位置距传感器",
  L"#",
  L"@",
  L"&"  };

    static wchar_t* string_1568[NLANGS] = {
  L"Dist from Input to screw Spl",
  L"螺杆分离位置距传感器",
  L"#",
  L"@",
  L"&"  };



  static wchar_t* string_1570[NLANGS] = {
  L"Inpscrew ratio num",
  L"",
  L"",
  L"@",
  L"Молекула коэффициента входного винта"  };

  static wchar_t* string_1572[NLANGS] = {
  L"Inpscrew ratio den",
  L"",
  L"",
  L"@",
  L"Знаменатель коэффициента входного винта"  };






// GRIPPERS PARAMETERS

  static wchar_t* string_1610[NLANGS] = {
  L"Grippers",
  L"夹爪",
  L"Pinzas",
  L"@",
  L"Захваты"  };

  static wchar_t* string_1612[NLANGS] = {
  L"Check door open/close sensor",
  L"检查开启/闭合传感器",
  L"Revisar sensor abie/cerr",
  L"@",
  L"Проверьте датчик откр. / закр."  };

  static wchar_t* string_1614[NLANGS] = {
  L"Timeout for opening",
  L"打开超时",
  L"Apertura tiempo fuera",
  L"@",
  L"Тайм-аут для открытия"  };

  static wchar_t* string_1616[NLANGS] = {
  L"Timeout for closing",
  L"闭合超时",
  L"Clausura tiempo fuera",
  L"@",
  L"Тайм-аут для закрытия"  };

  static wchar_t* string_1620[NLANGS] = {
  L"Opening phase",
  L"开启位置",
  L"Fase apertura",
  L"@",
  L"Фаза открытия"  };

  static wchar_t* string_1622[NLANGS] = {
  L"Opening advance",
  L"提前打开",
  L"Avance apertura",
  L"@",
  L"Открыть заранее"  };

  static wchar_t* string_1624[NLANGS] = {
  L"Opening pressure",
  L"开启压力",
  L"Presión apertura",
  L"@",
  L"Давление открытия"  };


  //Pour added for TIVS-S60
  static wchar_t* string_1626[NLANGS] = {
  L"Load to rolling-over",
  L"进料到翻转一",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_1626a[NLANGS] = {
  L"Load to rolling-over",
  L"前翻转序号",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_1626b[NLANGS] = {
  L"Phase of rolling-over",
  L"前翻转相位",
  L"#",
  L"@",
  L"&"  };


  static wchar_t* string_1626c[NLANGS] = {
  L"First rolling-over suction length",
  L"翻转一吸气长度",
  L"#",
  L"@",
  L"&"  };


  static wchar_t* string_1628[NLANGS] = {
  L"Load to revert",
  L"进料到翻转二",
  L"#",
  L"@",
  L"&"  };

    static wchar_t* string_1628a[NLANGS] = {
  L"Index of revert",
  L"后翻转序列号",
  L"#",
  L"@",
  L"&"  };

    static wchar_t* string_1628b[NLANGS] = {
  L"Phase of revert",
  L"后翻转相位",
  L"#",
  L"@",
  L"&"  };


  static wchar_t* string_1628c[NLANGS] = {
  L"Second revert suction length",
  L"翻转二吸气长度",
  L"#",
  L"@",
  L"&"  };


  //Pour added for TIVS_S60



  static wchar_t* string_1630[NLANGS] = {
  L"Closing position",
  L"闭合位置",
  L"Fase clausura",
  L"@",
  L"Позиция закрытия"  };

  static wchar_t* string_1632[NLANGS] = {
  L"Closing advance",
  L"提前闭合",
  L"Avance clausura",
  L"@",
  L"Закрыть заранее"  };

  static wchar_t* string_1634[NLANGS] = {
  L"Closing pressure",
  L"闭合压力",
  L"Presión clausura",
  L"@",
  L"Давление закрытия"  };

  static wchar_t* string_1636[NLANGS] = {
  L"Closed loop running",
  L"闭环运行",
  L"Arranque bucle cerrado",
  L"@",
  L"Работа в замкнутом цикле "  };


// INPUT / OUTPUT / DEVIATORS PARAMETERS

  static wchar_t* string_1638[NLANGS] = {
  L"Min input width",
  L"最小输入宽度",
  L"Anchura entrada mín.",
  L"@",
  L"MIN входная ширина"  };

  static wchar_t* string_1639[NLANGS] = {
  L"Max input width",
  L"最大输入宽度",
  L"Anchura entrada máx.",
  L"@",
  L"MAX входная ширина"  };

  static wchar_t* string_1640[NLANGS] = {
  L"Input/Output",
  L"进料/出料",
  L"Entrada/Salida",
  L"@",
  L"Ввод/Вывод"  };

  static wchar_t* string_1641[NLANGS] = {
  L"Distance PS01-DEV1",
  L"PS01- DEV1的距离",
  L"Distancia PS01-DEV1",
  L"@",
  L"Расстояние PS01-DEV1"  };

  static wchar_t* string_1641_2[NLANGS] = {
  L"Distance PS01-DEV2",
  L"PS01- DEV2的距离",
  L"Distancia PS01-DEV2",
  L"@",
  L"Расстояние PS01-DEV2"  };

  static wchar_t* string_1641_3[NLANGS] = {
  L"Distance PS01-DEV3",
  L"PS01- DEV3的距离",
  L"Distancia PS01-DEV3",
  L"@",
  L"Расстояние PS01-DEV3"  };

  static wchar_t* string_1641_4[NLANGS] = {
  L"Distance PS01-DEV4",
  L"PS01- DEV4的距离",
  L"Distancia PS01-DEV4",
  L"@",
  L"Расстояние PS01-DEV4"  };

    //Pour added on 20190620
    static wchar_t* string_1641a[NLANGS] = {
  L"Distance PS01-DEV2",
  L"PS01- DEV2的距离",
  L"Distancia PS01-DEV2",
  L"@",
  L"Расстояние PS01-DEV2"  };
  //Pour added on 20190620

  //Pour changed description from "Distance PS01-PS02" to "Distance LOAD-GOOD"
  static wchar_t* string_1642[NLANGS] = {
  L"Distance LOAD-GOOD",
  L"LOAD-GOOD的距离",
  L"Distancia LOAD-GOOD",
  L"@",
  L"Расстояние LOAD-GOOD"  };

  //Pour changed description from "Distance PS01-PS03" to "Distance LOAD-REJ-1"
  static wchar_t* string_1643[NLANGS] = {
  L"Distance LOAD-REJ-1",
  L"LOAD-REJ-1的距离",
  L"Distancia LOAD-REJ-1",
  L"@",
  L"Расстояние LOAD-REJ-1"  };

  //Pour changed description from "Distance PS01-PS04" to "Distance LOAD-REJ-2"
  static wchar_t* string_1644[NLANGS] = {
  L"Distance LOAD-REJ-2",
  L"LOAD-REJ-2的距离",
  L"Distancia LOAD-REJ-2",
  L"@",
  L"Расстояние LOAD-REJ-2"  };

  //Pour changed description from "Distance PS01-HVGOOD" to "Distance LOAD-HVGOOD"
  static wchar_t* string_1644a[NLANGS] = {
  L"Distance LOAD-HVGOOD",
  L"LOAD-HVGOOD的距离",
  L"Distancia LOAD-HVGOOD",
  L"@",
  L"Расстояние LOAD-HVGOOD"  };

  //Pour added 20180301

  //Pour changed description from "Distance PS01-PS05" to "Distance LOAD-REJ-2" 20200902
  static wchar_t* string_1644b[NLANGS] = {
  L"Distance LOAD-REJ-2",
  L"LOAD-REJ-2的距离",
  L"Distancia LOAD-REJ-2",
  L"@",
  L"Расстояние LOAD-REJ-2"  };

  //Pour changed description from "Distance PS01-PS06" to "Distance LOAD-REJ-3"
    static wchar_t* string_1644c[NLANGS] = {
  L"Distance LOAD-REJ-3",
  L"LOAD-REJ-3的距离",
  L"Distancia LOAD-REJ-3",
  L"@",
  L"Расстояние LOAD-REJ-3"  };
  //Pour added 20180301

  static wchar_t* string_1645[NLANGS] = {
  L"Distance DEV1-DEV2",
  L"DEV1-DEV2的距离",
  L"Distancia DEV1-DEV2",
  L"@",
  L"Расстояние DEV1-DEV2"  };

    //Pour added 20190620
    static wchar_t* string_1645a[NLANGS] = {
  L"Distance DEV2-DEV3",
  L"DEV2-DEV3的距离",
  L"Distancia DEV2-DEV3",
  L"@",
  L"Расстояние DEV2-DEV3"  };
  //Pour added 20190620

    static wchar_t* string_1645b[NLANGS] = {
  L"Distance DEV1-DEV3",
  L"DEV1-DEV3的距离",
  L"Distancia DEV1-DEV3",
  L"@",
  L"Расстояние DEV1-DEV3"  };

  static wchar_t* string_1646[NLANGS] = {
  L"Main starwheel suction lenght",
  L"主星轮吸气长度",
  L"Long. succión rueda estrella",
  L"@",
  L"Длит. присасывания гл. колеса-звёздочки"  };

  static wchar_t* string_1647[NLANGS] = {
  L"Distance PS01-DEV HV",
  L"PS01- DEV HV 的距离",
  L"Distancia PS01-DEV HV",
  L"@ PS01-DEV HV",
  L"Расстояние PS01-DEV HV"  };

  static wchar_t* string_1648[NLANGS] = {
  L"Main starwheel blowing lenght",
  L"主星轮吹气长度",
  L"Long soplado rueda estrella",
  L"@",
  L"Длит. продувки гл. колеса-звёздочки"  };

  static wchar_t* string_1650[NLANGS] = {
  L"Good/reject stwhl suction lenght",
  L"小星轮吸气长度 ",
  L"Acpt/rech long. succ. rue-est",
  L"@",
  L"Присасывания колеса-звёздочки отбраковки"  };

  static wchar_t* string_1652[NLANGS] = {
  L"Good/reject stwhl blowing lenght",
  L"小星轮吹气长度",
  L"Acpt/rech long. sopl. rue-est",
  L"@",
  L"Длит. продувки колеса отбраковки"  };

  static wchar_t* string_1653[NLANGS] = {
  L"DISTANCES",
  L"距离",
  L"DISTANCIAS",
  L"@",
  L"Расстояния"  };

  static wchar_t* string_1654[NLANGS] = {
  L"Loading value",
  L"加载值",
  L"Valor de carga",
  L"@",
  L"Значение нагрузки"  };

  static wchar_t* string_1656[NLANGS] = {
  L"Output mixed simulation",
  L"混合出料模拟",
  L"Simulación mezclada salida",
  L"@",
  L"Симуляция смешанной выгр"  };

  static wchar_t* string_1657[NLANGS] = {
  L"Sampling rate",
  L"抽样率",
  L"#",
  L"@",
  L"&"  };


    static wchar_t* string_1658[NLANGS] = {
  L"Sampling number",
  L"抽样数",
  L"#",
  L"@",
  L"&"  };



  static wchar_t* string_1660[NLANGS] = {
  L"Crosscheck goods output",
  L"复检合格品出料",
  L"Revisión cruzada bien salida",
  L"@",
  L"Выгр. повт. инсп. пригодных"  };

  static wchar_t* string_1662[NLANGS] = {
  L"Crosscheck rejects 1 output",
  L"复检剔除托盘1出料",
  L"Rev. cruzada rechaza 1 sal.",
  L"@",
  L"Выгрузка 1 с инспекции ОТБР. "  };

  static wchar_t* string_1664[NLANGS] = {
  L"Crosscheck rejects 2 output",
  L"复检剔除托盘2出料",
  L"Revisión cruzada rechaza 2 sal.",
  L"@",
  L"Выгр. 2 повт. инспекции  ОТБР."  };

  static wchar_t* string_1664a[NLANGS] = {
  L"Crosscheck rejects 3 output",
  L"复检剔除托盘3出料",
  L"Revisión cruzada rechaza 3 sal.",
  L"@",
  L"Выгр. 3 повт. инспекции  ОТБР."  };

  static wchar_t* string_1665[NLANGS] = {
  L"Crosscheck rejects 3 output",
  L"复检剔除托盘3出料",
  L"Revisión cruzada rechaza 3 sal.",
  L"@ 3",
  L"Выгр. 3 повт. инспекции ОТБР."  };

  static wchar_t* string_1666[NLANGS] = {
  L"Enable Spin rejects 1",
  L"旋转不良剔除到托盘1",
  L"Habilitar rechazos giro",
  L"@",
  L"ОТБР. в лоток 1: плохое вращение"  };

  static wchar_t* string_1667[NLANGS] = {
  L"Spin rejects to Tray 2",
  L"旋转不良剔除到托盘2",
  L"Rechazos giro bandeja 2",
  L"@",
  L"ОТБР. в лоток 2: плохое вращение"  };


  static wchar_t* string_1668[NLANGS] = {
  L"REJECTS",
  L"剔除",
  L"RECHAZOS",
  L"@",
  L"ОТБР."  };

  static wchar_t* string_1670[NLANGS] = {
  L"Enable particle 1 rejects to tray 1",
  L"粒子1剔除到托盘1",
  L"Habi. partícula 1 rechazos",
  L"@",
  L"ОТБР. в лоток 1: включения 1"  };

  static wchar_t* string_1672[NLANGS] = {
  L"Enable particle 2 rejects to tray 1",
  L"粒子2剔除到托盘1",
  L"Habi. partícula 2 rechazos",
  L"@",
  L"ОТБР. в лоток 1: включения 2"  };

  static wchar_t* string_1673[NLANGS] = {
  L"Enable particle 3 rejects",
  L"粒子3剔除到托盘1",
  L"Habi. rechazos 3 partícula.",
  L"@",
  L"ОТБР. в лоток 1: включения 3"  };

  static wchar_t* string_1674[NLANGS] = {
  L"Enable filling 1 rejects",
  L"液位1剔除到托盘1",
  L"Habi. rechaz. 1 llenado",
  L"@",
  L"ОТБР.в лоток 1: уровень 1"  };

    static wchar_t* string_1674a[NLANGS] = {
  L"Enable filling 1 rejects",
  L"浓度1剔除到托盘1",
  L"Habi. rechaz. 1 llenado",
  L"@",
  L"ОТБР.в лоток 1: уровень 1"  };

  static wchar_t* string_1675[NLANGS] = {
  L"Enable filling 2 rejects",
  L"液位2剔除到托盘1",
  L"Habi. rechaz. 2 llenado",
  L"@",
  L"ОТБР. в лоток 1: уровень 2"  };

  static wchar_t* string_1676[NLANGS] = {
  L"Enable Sidewall rejects",
  L"使能外观剔废",
  L"Habi. rechaz. superfi.",
  L"@",
  L"ОТБР. косм. дефекты стенок"  };

  static wchar_t* string_1678[NLANGS] = {
  L"Enable Cosmetic rejects to tray 1",
  L"外观剔除到托盘1",
  L"Habi. rechaz. cosmetic.",
  L"@",
  L"ОТБР. в лоток 1: косм. дефекты "  };

  static wchar_t* string_1680[NLANGS] = {
  L"Enable Tip/Cap rejects ",
  L"使能头部剔除",
  L"Habi. rechaz. punta/tapa",
  L"@",
  L"ОТБР. в лоток 1: кончик/колпачок"  };

  static wchar_t* string_1682[NLANGS] = {
  L"Particle 1 rejects to Tray2",
  L"粒子1剔除到托盘2",
  L"Partícula 1 rechaz. bandeja 2",
  L"@",
  L"ОТБР. в лоток 2: включения 1"  };

  static wchar_t* string_1683[NLANGS] = {
  L"Particle 2 rejects to Tray2",
  L"粒子2剔除到托盘2",
  L"Partícula 2 rechaz. bandeja 2",
  L"@",
  L"ОТБР. в лоток 2: включения 2"  };

  static wchar_t* string_1684[NLANGS] = {
  L"Filling 1 rejects to Tray2",
  L"液位1剔除到托盘2",
  L"Llenado 1 rechaz. bandeja 2",
  L"@",
  L"ОТБР. в лоток 2: уровень 1"  };

    static wchar_t* string_1684a[NLANGS] = {
  L"Filling 1 rejects to Tray2",
  L"浓度1剔除到托盘2",
  L"Llenado 1 rechaz. bandeja 2",
  L"@",
  L"ОТБР. в лоток 2: уровень 1"  };

  static wchar_t* string_1685[NLANGS] = {
  L"Filling 2 rejects to Tray2",
  L"液位2剔除到托盘2",
  L"Llenado 2 rechazos bandeja 2",
  L"@",
  L"ОТБР. в лоток 2: уровень 2"  };

  static wchar_t* string_1686[NLANGS] = {
  L"Cosmetic rejects to Tray2",
  L"外观剔除到托盘2",
  L"Cosmetico rechaz. bandeja 2",
  L"@",
  L"ОТБР. в лоток 2: косм. дефекты"  };

  static wchar_t* string_1687[NLANGS] = {
  L"Particle 3 rejects to Tray2",
  L"粒子3剔除到托盘2",
  L"Partícula 3 rechaz. bandeja 2",
  L"@",
  L"ОТБР. в лоток 2: включения 3"  };

  static wchar_t* string_1688[NLANGS] = {
  L"Tip/Cap rejects to Tray2",
  L"头部检测剔除到托盘2",
  L"Punta/tapa rechaz. bandeja 2",
  L"@",
  L"ОТБР. в лоток 2: кончик/колпачок"  };

  static wchar_t* string_1689[NLANGS] = {
  L"Sidewall rejects to Tray2",
  L"外观剔除到托盘2",
  L"Superficie rechaz. bandeja 2",
  L"@",
  L"ОТБР. в лоток 2: дефекты стенок"  };




  static wchar_t* string_1690[NLANGS] = {
  L"TRIGGERS",
  L"触发",
  L"PINZAS",
  L"@",
  L"Триггеры"  };

  static wchar_t* string_1691[NLANGS] = {
  L"Tip/cap phase",
  L"头部相位",
  L"Fase punta/tapa",
  L"@",
  L"Фаза кончика/колпачка"  };

  static wchar_t* string_1692[NLANGS] = {
  L"FO color phase",
  L"易开盖颜色头部相位",
  L"Fase color FO",
  L"@",
  L"Фаза Flip-off"  };

  static wchar_t* string_1692b[NLANGS] = {
  L"Liquid color phase",
  L"色水相位",
  L"#",
  L"@",
  L"Фаза цвета жидкости"  };

  static wchar_t* string_1693[NLANGS] = {
  L"Bottom A (HA) phase",
  L"底部检测相机A相位",
  L"Fase fondo A (HA)",
  L"@",
  L"Фаза A(HA): Дно"  };

  static wchar_t* string_1694[NLANGS] = {
  L"Bottom B (LA) phase",
  L"底部检测相机B相位",
  L"Fase fondo B (LA)",
  L"@",
  L"Фаза В(LA): Дно"  };

  static wchar_t* string_1695[NLANGS] = {
  L"Sidewall phase",
  L"外观检测相位",
  L"Fase superficie",
  L"@",
  L"Фаза : дефекты "  };

  static wchar_t* string_1696a[NLANGS] = {
  L"Side cake phase",
  L"粉饼侧面检测相位",
  L"Fase lateral pastel",
  L"@",
  L"Фаза : боковая  ЛИО"  };

  static wchar_t* string_1696b[NLANGS] = {
  L"Top cake phase",
  L"粉饼上面检测相位",
  L"Fase superior past.",
  L"@",
  L"Фаза : верх ЛИО"  };

  static wchar_t* string_1696c[NLANGS] = {
  L"Dot-ring color phase",
  L"色点色环检测相位",
  L"Fase anillo color",
  L"@",
  L"Фаза : точки и цветной контур"  };

  static wchar_t* string_1696d[NLANGS] = {
  L"Heel phase",
  L"瓶根检测相位",
  L"Fase Base",
  L"@",
  L"Фаза : нижняя сторона ЛИО"  };

      static wchar_t* string_1696e[NLANGS] = {
  L"Neck phase",
  L"瓶脖检测相位",
  L"Fase Base",
  L"@",
  L"Фаза : нижняя сторона ЛИО"  };


  static wchar_t* string_1697[NLANGS] = {
  L"Particle 1 phase",
  L"粒子1相位",
  L"Fase partícula 1",
  L"@",
  L"Фаза : включения 1"  };

  static wchar_t* string_1698[NLANGS] = {
  L"Particle 2 phase",
  L"粒子2相位",
  L"Fase partícula 2",
  L"@",
  L"Фаза : включения 2"  };

  static wchar_t* string_1698b[NLANGS] = {
  L"Particle 3 phase",
  L"粒子3相位",
  L"Fase partícula 3",
  L"@",
  L"Фаза : включения 3"  };

  static wchar_t* string_1699[NLANGS] = {
  L"Cosmetic phase",
  L"外观相位",
  L"Fase cosmetica",
  L"@",
  L"Фаза: косм. дефекты"  };


  static wchar_t* string_1697a[NLANGS] = {
  L"Particle1/Sidecake phase",
  L"粒子1/粉饼侧面相位",
  L"# Fase partícula 1",
  L"@",
  L"Фаза: включения 1"  };

  static wchar_t* string_1697b[NLANGS] = {
  L"Particle2/Heel phase",
  L"粒子2/瓶跟相位",
  L"# Fase partícula 2",
  L"@",
  L"Фаза: включения 2"  };





// SPINNING PARAMETERS

  static wchar_t* string_1700[NLANGS] = {
  L"Particle 1 spin params",
  L"粒子1旋转参数",
  L"Parám. giro partícula 1",
  L"@",
  L"Параметры : включения 1"  };

  static wchar_t* string_1701[NLANGS] = {
  L"Particle 2 spin params",
  L"粒子2旋转参数",
  L"Parám. giro partícula 2",
  L"@",
  L"Параметры: включения 2"  };

  static wchar_t* string_1702[NLANGS] = {
  L"Heel spin params",
  L"瓶根旋转参数",
  L"Param. Rot. Botella",
  L"@",
  L"Параметры: низ ЛИО"  };

  static wchar_t* string_1703[NLANGS] = {
  L"Cosmetic spin params",
  L"外观工位旋转参数",
  L"Parám. giro cosmetico",
  L"@",
  L"Параметры : дефекты"  };

  static wchar_t* string_1704[NLANGS] = {
  L"Side-cake spin params",
  L"粉饼侧面工位旋转参数",
  L"Parám. giro lateral past.",
  L"@",
  L"Параметры: бок ЛИО",  };

  static wchar_t* string_1705[NLANGS] = {
  L"Top-cake spin params",
  L"粉饼顶部工位旋转参数",
  L"Parám. giro superior past.",
  L"@",
  L"Параметры : верхняя ЛИО"  };

  static wchar_t* string_1706[NLANGS] = {
  L"Sidewall spin params",
  L"外观旋转参数",
  L"Parám. giro superficies",
  L"@",
  L"Параметры: стенки"  };

  static wchar_t* string_1707[NLANGS] = {
  L"Particle 3 spin params",
  L"粒子3旋转参数",
  L"Parám. giro partícula 3",
  L"@",
  L"Параметры : включения 3"  };

  static wchar_t* string_1708[NLANGS] = {
  L"Enable auto-alignment",
  L"启用自动对齐功能",
  L"Habi. auto-alineación",
  L"@",
  L"ВКЛ. авто выравнивание"  };

  static wchar_t* string_1710[NLANGS] = {
  L"Send params to motors",
  L"发送参数",
  L"Enviar parám. a motores",
  L"@",
  L"Параметры передачи"  };

  static wchar_t* string_1716[NLANGS] = {
  L"Enable spinning",
  L"启用旋转功能",
  L"Habilitar giro",
  L"@",
  L"ВКЛ.ючить вращение"  };

  static wchar_t* string_1718[NLANGS] = {
  L"Enable auto-alignment",
  L"启用自动对齐功能",
  L"Habi. auto-alineación",
  L"@",
  L"ВКЛ. авто выравнивание"  };

  static wchar_t* string_1716c[NLANGS] = {
  L"Enable Cosmetic1 spinning",
  L"启用外观1旋转功能",
  L"# Habilitar giro",
  L"@ ",
  L"& ВКЛ.ючить вращение"  };

  static wchar_t* string_1716n[NLANGS] = {
  L"Enable Cosmetic2 spinning",
  L"启用外观2旋转功能",
  L"# Habilitar giro",
  L"@ ",
  L"& ВКЛ.ючить вращение"  };

  static wchar_t* string_1719[NLANGS] = {
  L"Cosmetic1 spin params",
  L"外观1旋转参数",
  L"# Parám. giro superficies",
  L"@",
  L"& Параметры: стенки"  };

    static wchar_t* string_1719a[NLANGS] = {
  L"Cosmetic2 spin params",
  L"外观2旋转参数",
  L"# Parám. giro superficies",
  L"@",
  L"& Параметры: стенки"  };



  static wchar_t* string_1720[NLANGS] = {
  L"Maximum speed",
  L"最大速度",
  L"Valocidad máx.",
  L"@",
  L"MAX скорость"  };

  static wchar_t* string_1722[NLANGS] = {
  L"Acceleration",
  L"加速",
  L"Aceleración",
  L"@",
  L"Ускорение"  };

  static wchar_t* string_1724[NLANGS] = {
  L"Deceleration",
  L"减速",
  L"Desacerelación",
  L"@",
  L"Замедление"  };

  static wchar_t* string_1726[NLANGS] = {
  L"Num of turns",
  L"旋转圈数",
  L"Num. de vueltas",
  L"@",
  L"Число оборотов"  };

  static wchar_t* string_1728[NLANGS] = {
  L"Delay",
  L"延迟",
  L"Retraso",
  L"@",
  L"Задержка"  };

  static wchar_t* string_1730[NLANGS] = {
  L"Enable spin alarms",
  L"使能旋转电机警报",
  L"Habi. alarmas apin",
  L"@",
  L"ВКЛ. сигналов тревоги spin "  };

  static wchar_t* string_1732[NLANGS] = {
  L"Particle1/Sidecake spin params",
  L"粒子1/粉饼侧面旋转参数",
  L"# Parám. giro partícula 1/",
  L"@",
  L"Параметры: включения 1"  };

  static wchar_t* string_1734[NLANGS] = {
  L"Particle 2/Heel spin params",
  L"粒子2/瓶跟旋转参数",
  L"# Parám. giro partícula 2",
  L"@",
  L"Параметры: включения 2"  };



  static wchar_t* string_1742[NLANGS] = {
  L"Particle1/Sidewall spin",
  L"粒子1/粉饼侧面旋转参数",
  L"Partícula1/Superficie giro",
  L"@",
  L"Включения 1/Дефекты стенок"  };

  static wchar_t* string_1744[NLANGS] = {
  L"Particle2/Topcake spin",
  L"粒子2/粉饼顶部工位旋转参数",
  L"Partícula2/Superior giro",
  L"@",
  L"включения 2/Параметры : верх"  };



  static wchar_t* string_1746[NLANGS] = {
  L"Enable waist/layer rejects",
  L"使能收腰/分层剔除",
  L"@",
  L"@",
  L"ВКЛ. ОТБР. по сжатию / слою ЛИО"  };

  static wchar_t* string_1748[NLANGS] = {
  L"Waist/layer rejects to Tray2",
  L"收腰/分层剔除到托盘2",
  L"@",
  L"@",
  L"ВКЛ. ОТБР. по сжатию / слою ЛИО в лоток 2"  };




  // DELAYS
  static wchar_t* string_1750[NLANGS] = {
  L"DELAYS",
  L"延时",
  L"RETRASOS",
  L"@",
  L"Задержка"  };

  static wchar_t* string_1752[NLANGS] = {
  L"Goods full warning (*)",
  L"合格品托盘料满警告(*)",
  L"Advertencia correcta(*)",
  L"@",
  L"ВНИМАНИЕ! Лоток приг. прод. переполнен (*)"  };

  static wchar_t* string_1754[NLANGS] = {
  L"Goods full stop (*)",
  L"合格品托盘料满停止(*)",
  L"Paro completo",
  L"@",
  L"СТОП! Лоток пригодного продукта переполнен (*)"  };

  static wchar_t* string_1755[NLANGS] = {
  L"Goods full restart (*)",
  L"合格品托盘料满重启 (*)",
  L"Reinicio completo",
  L"@",
  L"ПЕРЕЗАГРУЗКА! Лоток пригодного продукта переполнен (*)"  };

  static wchar_t* string_1756[NLANGS] = {
  L"Rejects 1 full warning (*)",
  L"托盘1料满警告(*)",
  L"Rechazos 1 advertencia",
  L"@",
  L" ВНИМАНИЕ! Лоток ОТБР. 1 переполнен (*)"  };

  static wchar_t* string_1758[NLANGS] = {
  L"Rejects 1 full stop (*)",
  L"托盘1料满停止(*)",
  L"Rechazos 1 paro",
  L"@",
  L"СТОП! Лоток ОТБР. 1 переполнен (*)"  };

  static wchar_t* string_1759[NLANGS] = {
  L"Rejects 1 full restart (*)",
  L"托盘1料满重启 (*)",
  L"Rechazos 1 reinicio",
  L"@",
  L"ПЕРЕЗАГРУЗКА! Лоток ОТБР. 1 переполнен (*)"  };

  static wchar_t* string_1760[NLANGS] = {
  L"Rejects 2 full stop (*)",
  L"托盘2料满停止(*)",
  L"Rechazos 2 paro",
  L"@",
  L"СТОП! Лоток ОТБР. 2 переполнен (*)"  };

  static wchar_t* string_1761[NLANGS] = {
  L"Rejects 3 full stop (*)",
  L"托盘3料满停止(*)",
  L"Rechazos 3 paro",
  L"@",
  L"СТОП! Лоток ОТБР. 3 переполнен (*)"  };

  static wchar_t* string_1762[NLANGS] = {
  L"Rejects 2 full restart (*)",
  L"托盘2料满重启(*)",
  L"Rechazos 2 reinicio",
  L"@",
  L"ПЕРЕЗАГРУЗКА! Лоток ОТБР. 2 переполнен (*)"  };

  //Pour added for HV rejection tray 3
    static wchar_t* string_1760a[NLANGS] = {
  L"Rejects 3 full stop (*)",
  L"托盘3料满停止(*)",
  L"Rechazos 3 paro",
  L"@",
  L"СТОП! Лоток ОТБР. 3 переполнен (*)"  };

  static wchar_t* string_1762a[NLANGS] = {
  L"Rejects 3 full restart (*)",
  L"托盘3料满重启(*)",
  L"Rechazos 3 reinicio",
  L"@",
  L"ПЕРЕЗАГРУЗКА! Лоток ОТБР. 3 переполнен (*)"  };

    static wchar_t* string_1762b[NLANGS] = {
  L"Max continuous HV rejects (*)",
  L"HV最大连续剔除数量(*)",
  L"@",
  L"@",
  L"ПЕРЕЗАГРУЗКА! Лоток ОТБР. 3 переполнен (*)"  };

  static wchar_t* string_1763[NLANGS] = {
  L"Rejects 3 full restart (*)",
  L"托盘3料满重启(*)",
  L"Rechazos 3 reinicio",
  L"@",
  L"ПЕРЕЗАГРУЗКА! Лоток ОТБР. 3 переполнен (*)"  };

  static wchar_t* string_1764[NLANGS] = {
  L"Input empty stop delay (*)",
  L"进料空瓶停止延迟(*)",
  L"Retraso de paro ent. vacía(*)",
  L"@",
  L"Зона загрузки пуста, задержка остановки (*)"  };

  static wchar_t* string_1766[NLANGS] = {
  L"Input empty restart delay (*)",
  L"进料空瓶后重启延迟(*)",
  L"Retraso reinicio ent. vacía(*)",
  L"@",
  L"Зона загрузки пуста, задержка перезагрузки (*)"  };

  static wchar_t* string_1767[NLANGS] = {
  L"Rotary table stop delay (*)",
  L"转盘停止延迟(*)",
  L"Retraso paro mesa rotaroria (*)",
  L"@",
  L"Задержка остановки поворотного стола (*)"  };

  static wchar_t* string_1768[NLANGS] = {
  L"Rotary table restart delay (*)",
  L"转盘重启延迟(*)",
  L"Retraso reinicio mesa giratoria (*)",
  L"@",
  L"Задержка перезагрузки поворотного стола (*)"  };
  
  static wchar_t* string_1769[NLANGS] = {
  L"Fallen container delay (*)",
  L"倒瓶延迟(*)",
  L"Retraso contenedor caído (*)",
  L"@",
  L"Упавшие контейнеры: задержка"  };
  


  static wchar_t* string_1770[NLANGS] = {
  L"Enable Side cake rejects",
  L"使能粉饼侧面剔除",
  L"Habi. rechaz.lateral past.",
  L"@",
  L"ВКЛ. ОТБР. по боковой стороне ЛИО"  };

  static wchar_t* string_1772[NLANGS] = {
  L"Enable Top cake rejects",
  L"使能粉饼顶部剔除",
  L"Habilitar rechaz. superior past.",
  L"@",
  L"ВКЛ. ОТБР. по верху  ЛИО"  };

  static wchar_t* string_1774[NLANGS] = {
  L"Enable FO Color rejects",
  L"使能易开盖颜色剔除",
  L"Habi. rechaz. color FO",
  L"@",
  L"ВКЛ. ОТБР. по цвету Flip-off"  };

  static wchar_t* string_1775[NLANGS] = {
  L"Enable Liquid Color rejects",
  L"使能色水剔除",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_1776[NLANGS] = {
  L"Enable Bottom A rejects",
  L"使能底部相机A剔除",
  L"Habi. rechaz. fondo A",
  L"@",
  L"ВКЛ. ОТБР.: дно, камера A"  };

  static wchar_t* string_1778[NLANGS] = {
  L"Enable Bottom B rejects",
  L"使能底部相机B剔除",
  L"Habi. rechaz. fondo B",
  L"@",
  L"ВКЛ. ОТБР.: дно, камера B "  };

  static wchar_t* string_1779[NLANGS] = {
  L"Enable dot-ring col rejects",
  L"使能色点色环剔除",
  L"Habilitar rech. anillo color",
  L"@",
  L"ВКЛ. ОТБР. по точкам и цв.  контуру"  };

     static wchar_t* string_1779a[NLANGS] = {
  L"Enable neck-crimping rejects",
  L"使能瓶脖翻边剔除",
  L"Habilitar rech. anillo color",
  L"@",
  L"ВКЛ. ОТБР. по точкам и цв.  контуру"  };


  static wchar_t* string_1780[NLANGS] = {
  L"Side Cake rejects to Tray2",
  L"粉饼侧面剔除到托盘2",
  L"Rechaz. lateral past. bandeja 2",
  L"@",
  L"ОТБР.по боковой поверхности в лоток 2"  };

  static wchar_t* string_1782[NLANGS] = {
  L"Top Cake rejects to Tray2",
  L"粉饼顶部剔除到托盘2",
  L"Rechaz. superior past. bandeja 2",
  L"@",
  L"ОТБР. по верху ЛИО в лоток 2"  };

  static wchar_t* string_1784[NLANGS] = {
  L"FO Color rejects to Tray2",
  L"易开盖颜色剔除",
  L"Rechaz. color FO bandeja 2",
  L"@",
  L"ОТБР. по цвету Flip-off в лоток 2"  };

  static wchar_t* string_1785[NLANGS] = {
  L"Liquid Color rejects to Tray2",
  L"色水剔除到托盘2",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_1786[NLANGS] = {
  L"Bottom A rejects to Tray2",
  L"底部相机A剔除到托盘2",
  L"Rechaz. fondo A bandeja 2",
  L"@",
  L"ОТБР.: дно, камера A, в лоток 2"  };

  static wchar_t* string_1788[NLANGS] = {
  L"Bottom B rejects to Tray2",
  L"底部相机B剔除到托盘2",
  L"Rechaz. fondo B bandeja 2",
  L"@",
  L"ОТБР.: дно, камера B, в лоток 2"  };

  static wchar_t* string_1789[NLANGS] = {
  L"Dot-ring col rejects to Tray2",
  L"色点色环剔除到托盘2",
  L"Rech. anillo color a bandeja 2",
  L"@",
  L"ОТБР. по точкам и цв.  контуру в лоток 2"  };


      static wchar_t* string_1789a[NLANGS] = {
  L"Neck-crimping rejects to Tray2",
  L"瓶脖翻边剔除到托盘2",
  L"Rech. anillo color a bandeja 2",
  L"@",
  L"ОТБР. по точкам и цв.  контуру в лоток 2"};


  static wchar_t* string_1790[NLANGS] = {
  L"Enable Particle 1/Sidewall",
  L"使能粒子1/外观剔除",
  L"Habi. partícula 1/superfice",
  L"@",
  L"ВКЛ. ОТБР. включение 1/Дефекты стенок"  };

    //Pour added for second C20 machine on 20190504
    static wchar_t* string_1790a[NLANGS] = {
  L"Enable Particle 1/Sidecake",
  L"使能粒子1/粉饼侧面剔除",
  L"Habi. partícula 1/superfice",
  L"@",
  L"ВКЛ. ОТБР. включение 1/Дефекты стенок"  };
  //Pour added for second C20 machine on 20190504

        //Pour added for second C20 machine on 20190504
    static wchar_t* string_1790b[NLANGS] = {
  L"Enable Particle 1/Sidecake reject to 2",
  L"使能粒子1/粉饼侧面剔除到通道2",
  L"Habi. partícula 1/superfice",
  L"@",
  L"ВКЛ. ОТБР. включение 1/Дефекты стенок"  };
  //Pour added for second C20 machine on 20190504


  static wchar_t* string_1792[NLANGS] = {
  L"Enable Side cake",
  L"使能粉饼侧面剔除",
  L"Habi. lateral pastel",
  L"@",
  L"ВКЛ. ОТБР. по боку ЛИО"  };


    //Pour added for second C20 machine on 20190504
    static wchar_t* string_1792a[NLANGS] = {
  L"Enable Sidewall",
  L"使能外观剔除",
  L"Habi. lateral pastel",
  L"@",
  L"ВКЛ. ОТБР. по боку ЛИО"  };
  //Pour added for second C20 machine on 20190504

  static wchar_t* string_1794[NLANGS] = {
  L"Enable Particle 2/Topcake",
  L"使能粒子2/粉饼顶部剔除",
  L"Habi. partícula 2/superior",
  L"@",
  L"ВКЛ. ОТБР. включение 2/Верхняя  ЛИО"  };


  static wchar_t* string_1794b[NLANGS] = {
  L"Enable Particle 2/Topcake reject to 2",
  L"使能粒子2/粉饼顶部剔除到通道2",
  L"Habi. partícula 2/superior",
  L"@",
  L"ВКЛ. ОТБР. включение 2/Верхняя  ЛИО"  };


  static wchar_t* string_1796[NLANGS] = {
  L"Enable Heel rejects",
  L"使能瓶根剔除",
  L"Habilitar rech. base",
  L"@",
  L"ВКЛ. ОТБР. по низу ЛИО"  };

  static wchar_t* string_1798[NLANGS] = {
  L"Heel rejects to Tray2",
  L"瓶根剔除到托盘2",
  L"Rech. base a band. 2",
  L"@",
  L"ОТБР. по нижней стороне ЛИО в лоток 2"  };




  // ADVANCED/CALIBRATION
  static wchar_t* string_1800[NLANGS] = {
  L"CALIBRATION PARAMETERS",
  L"校准参数",
  L"PARÁMETROS CALIBRACIÓN",
  L"@",
  L"Параметры калибровки"  };

  static wchar_t* string_1801[NLANGS] = {
  L"Particle1 datarec offset",
  L"粒子1数据收集记录补偿",
  L"Compensa.reg. partícula 1 ",
  L"@",
  L"Комп. смещ. – Включения 1"  };

  static wchar_t* string_1802[NLANGS] = {
  L"Particle2 datarec offset",
  L"粒子2数据收集记录补偿",
  L"Compensa.reg. partícula 2 ",
  L"@",
  L"Комп. смещ. – Включения 2"  };

  static wchar_t* string_1803[NLANGS] = {
  L"Particle3 datarec offset",
  L"粒子3数据收集记录补偿",
  L"Compensa.reg. partícula 3 ",
  L"@",
  L"Комп. смещ. – Включения 3"  };


  static wchar_t* string_1801a[NLANGS] = {
  L"Station1 datarec offset",
  L"工位1数据收集记录补偿",
  L"# Compensa.reg. partícula 1 ",
  L"@",
  L"Комп. смещ. – Включения 1"  };

  static wchar_t* string_1801b[NLANGS] = {
  L"Station2 datarec offset",
  L"工位2数据收集记录补偿",
  L"# Compensa.reg. partícula 1 ",
  L"@",
  L"Комп. смещ. – Включения 1"  };

  static wchar_t* string_1801c[NLANGS] = {
  L"Station3 datarec offset",
  L"工位3数据收集记录补偿",
  L"# Compensa.reg. partícula 1 ",
  L"@",
  L"Комп. смещ. – Включения 1"  };


  static wchar_t* string_1804[NLANGS] = {
  L"Bottom A datarec offset",
  L"底部相机A数据记录补偿",
  L"Compensa.reg. fondo A ",
  L"@",
  L"Компенсация смещения – дно, камера A"  };

  static wchar_t* string_1805[NLANGS] = {
  L"Bottom B datarec offset",
  L"底部相机B数据记录补偿",
  L"Compensa.reg. fondo B ",
  L"@",
  L"Комп. смещ. – дно, камера B"  };

  static wchar_t* string_1806[NLANGS] = {
  L"Sidewall datarec offset",
  L"外观数据记录补偿",
  L"Compensa.reg. superficie ",
  L"@",
  L"Комп. смещ. – дефекты стенок "  };

  static wchar_t* string_1807[NLANGS] = {
  L"Sidecake datarec offset",
  L"粉饼侧面数据记录补偿",
  L"Compensa.reg. lateral",
  L"@",
  L"Комп. смещ. – бок ЛИО"  };

  static wchar_t* string_1808[NLANGS] = {
  L"Topcake datarec offset",
  L"粉饼上面数据记录补偿",
  L"Compensa.reg. superior",
  L"@",
  L"Комп. смещ. – верх ЛИО"  };



  static wchar_t* string_1809[NLANGS] = {
  L"ABC phases",
  L"ABC相位",
  L"Fases ABC",
  L"@",
  L"Фазы ABC"  };


  static wchar_t* string_1810[NLANGS] = {
  L"Input phase A",
  L"进料相位A",
  L"Entrada fase A",
  L"@",
  L"Фаза загрузки A"  };

  static wchar_t* string_1812[NLANGS] = {
  L"Input phase B",
  L"进料相位B",
  L"Entrada fase B",
  L"@",
  L"Фаза загрузки B"  };

  //Pour added 20180317
  static wchar_t* string_1810a[NLANGS] = {
  L"Input hv phase A",
  L"出料HV相位A",
  L"Entrada fase A",
  L"@",
  L"Фаза загрузки A"  };

  static wchar_t* string_1812a[NLANGS] = {
  L"Input hv phase B",
  L"出料HV相位B",
  L"Entrada fase B",
  L"@",
  L"Фаза загрузки B"  };
  //Pour added 20180317


  static wchar_t* string_1820[NLANGS] = {
  L"Cam phase TCA",
  L"相机相位TCA",
  L"Cam fase TCA",
  L"@",
  L"Фаза камеры TCA"  };

  static wchar_t* string_1822[NLANGS] = {
  L"Cam phase TCB",
  L"相机相位TCB",
  L"Cam fase TCB",
  L"@",
  L"Фаза камеры TCB"  };

  static wchar_t* string_1824[NLANGS] = {
  L"Cam phase TCC",
  L"相机相位TCC",
  L"Cam fase TCC",
  L"@",
  L"Фаза камеры TCC"  };

  static wchar_t* string_1825[NLANGS] = {
  L"Cam phase TCD",
  L"相机相位TCD",
  L"Cam fase TCD",
  L"@",
  L"Фаза камеры TCD"  };

  static wchar_t* string_1826[NLANGS] = {
  L"Cam phase TCE",
  L"相机相位TCE",
  L"Cam fase TCE",
  L"@",
  L"Фаза камеры TCE "  };


  static wchar_t* string_1828[NLANGS] = {
  L"Carousel height encoder",
  L"主轴高度编码器",
  L"Altura codificador carrusel",
  L"@",
  L"Датчик высоты башни"  };


  static wchar_t* string_1830[NLANGS] = {
  L"Max allowed height",
  L"最大允许高度",
  L"Altura máx.",
  L"@",  
  L"MAX допустимая высота"  };


  static wchar_t* string_1832[NLANGS] = {
  L"Min allowed height",
  L"最小允许高度",
  L"Altura mín.",
  L"@",
  L"MIN допустимая высота"  };

  static wchar_t* string_1840[NLANGS] = {
  L"Top point height",
  L"高度1",
  L"Altura punto super.",
  L"@",
  L"Высота 1"  };

  static wchar_t* string_1842[NLANGS] = {
  L"Low point height",
  L"高度2",
  L"Altura punto bajo",
  L"@",
  L"Высота 2"  };

  static wchar_t* string_1848[NLANGS] = {
  L"Offsets and distances",
  L"补偿和距离",
  L"Compensaciones y dist.",
  L"@",
  L"Смещения и расстояния"  };

  static wchar_t* string_1850[NLANGS] = {
  L"Spin 1 offset",
  L"旋转1位置",
  L"Compensa. giro 1",
  L"@",
  L"Вращ. 1 смещ"  };

  static wchar_t* string_1852[NLANGS] = {
  L"Spin 2 offset",
  L"旋转2位置",
  L"Compensa. giro 2",
  L"@",
  L"Вращ. 2 смещ."  };

  static wchar_t* string_1854[NLANGS] = {
  L"Spin 3 offset",
  L"旋转3位置",
  L"Compensa. giro 3",
  L"@",
  L"Вращ. 3 смещ."  };

  static wchar_t* string_1855[NLANGS] = {
  L"Spin 4 offset",
  L"旋转4位置",
  L"Compensa. giro 4",
  L"@",
  L"Вращ. 4 смещ."  };




  static wchar_t* string_1856[NLANGS] = {
  L"Align offset",
  L"对齐位置",
  L"Compensa. alinea.",
  L"@",
  L"Выравнивание смещения"  };

  static wchar_t* string_1858[NLANGS] = {
  L"DISTANCES",
  L"距离",
  L"DISTANCIAS",
  L"@",
  L"Расст."  };

  static wchar_t* string_1859[NLANGS] = {
  L"OFFSETS",
  L"补偿",
  L"COMPENSACIONES",
  L"@",
  L"Комп."  };


  static wchar_t* string_1860[NLANGS] = {
  L"Input-Tip/Cap distance",
  L"进料-头部距离",
  L"Distancia entrada punta/tapa",
  L"@",
  L"Загр.- расст. до кончика"  };

  static wchar_t* string_1861[NLANGS] = {
  L"Input-FO color distance",
  L"进料-易开盖颜色距离",
  L"Distancia color entrada FO",
  L"@",
  L"Загр.- расст. цвет. Flip-off"  };

  static wchar_t* string_1861b[NLANGS] = {
  L"Input-DR color distance",
  L"进料-色点色环距离",
  L"Ent- DR Dist. Color",
  L"@",
  L"Загр.- расст. цвет. контур"  };

      static wchar_t* string_1861c[NLANGS] = {
  L"Input-neck distance",
  L"进料-瓶脖距离",
  L"Ent- DR Dist. Color",
  L"@",
  L"Загр.- расст. цвет. контур"  };



  static wchar_t* string_1862[NLANGS] = {
  L"Input-Bottom A (HA) distance",
  L"进料=底部相机检测A距离",
  L"Dist. ent. fondo A (HA)",
  L"@",
  L"Загр.- расст., дно, камера A"  };

  static wchar_t* string_1863[NLANGS] = {
  L"Input-Bottom B (LA) distance",
  L"进料 -底部相机B距离",
  L"Dist. ent. fondo B (LA)",
  L"@",
  L"Загр.- расст., дно, камера B"  };



  static wchar_t* string_1864[NLANGS] = {
  L"Input-Part1 A distance",
  L"进料-粒子1相机A距离",
  L"Dista. entrada parte 1A",
  L"@",
  L"Загр.- расст. 1  A"  };

  static wchar_t* string_1865[NLANGS] = {
  L"Input-Part1 B distance",
  L"进料-粒子1相机B距离",
  L"Dista. entrada parte 1B",
  L"@",
  L"Загр.- расст. 1  B"  };

  static wchar_t* string_1866[NLANGS] = {
  L"Input-Part1 C distance",
  L"进料-粒子1相机C距离",
  L"Dista. entrada parte 1C",
  L"@",
  L"Загр.- расст. 1  C"  };

  static wchar_t* string_1867[NLANGS] = {
  L"Input-Part1 D distance",
  L"进料-粒子1相机D距离",
  L"Dista. entrada parte 1D",
  L"@",
  L"Загр.- расст. 1  D"  };

  static wchar_t* string_1868[NLANGS] = {
  L"Input-Part1 E distance",
  L"进料-粒子1相机E距离",
  L"Dista. entrada parte 1E",
  L"@",
  L"Загр.- расст. 1  E"  };

  static wchar_t* string_1869[NLANGS] = {
  L"Input-Part1 F distance",
  L"进料-粒子1相机F距离",
  L"Dista. entrada parte 1F",
  L"@",
  L"Загр.- расст. 1  F"  };

  
  static wchar_t* string_1870[NLANGS] = {
  L"Carous pos=%.2lf",
  L"主轴位置= %.2lf",
  L"Pos. carru= %.2lf",
  L"@ %.2lf",
  L"Позиция башни = %.2lf"  };

  static wchar_t* string_1871[NLANGS] = {
  L" Inp ph (%d) @3=%.2lf @2=%.2lf",
  L" 进料相位 (%d) @3=%.2lf @2=%.2lf",
  L" Fase ent (%d) @3=%.2lf @2=%.2lf",
  L"@ (%d) @3=%.2lf @2=%.2lf",
  L"Фаза загрузки (%d) @3=%.2lf @2=%.2lf"  };

  static wchar_t* string_1872[NLANGS] = {
  L" Inp ph (%d) @3=%.2lf @4=%.2lf",
  L" 进料相位 (%d) @3=%.2lf @4=%.2lf",
  L" Fase ent (%d) @3=%.2lf @4=%.2lf",
  L" @ (%d) @3=%.2lf @4=%.2lf",
  L" Фаза загр. (%d) @3=%.2lf @4=%.2lf"  };

    static wchar_t* string_1872a[NLANGS] = {
  L" Inp ph (%d) @3=%.2lf @4=%.2lf @18=%.2lf",
  L" 进料相位 (%d) @3=%.2lf @4=%.2lf @18=%.2lf",
  L" Fase ent (%d) @3=%.2lf @4=%.2lf @18=%.2lf",
  L" @ (%d) @3=%.2lf @4=%.2lf @18=%.2lf",
  L" Фаза загр. (%d) @3=%.2lf @4=%.2lf @18=%.2lf"  };

  static wchar_t* string_1873[NLANGS] = {
  L" Inp ph (%d) @3=%.2lf @6=%.2lf",
  L" 进料相位 (%d) @3=%.2lf @6=%.2lf",
  L" Fase ent (%d) @3=%.2lf @6=%.2lf",
  L" @ (%d) @3=%.2lf @6=%.2lf",
  L" Фаза загр. (%d) @3=%.2lf @6=%.2lf"  };

  static wchar_t* string_1874[NLANGS] = {
  L" Inp width=%.2lf",
  L" 进料宽度=%.2lf",
  L" Ancho ent=%.2lf",
  L" @=%.2lf",
  L" Ширина загр. =%.2lf"  };

  static wchar_t* string_1875[NLANGS] = {
  L" Inp ph (%d) @3=%.2lf",
  L" 进料相位 (%d) @3=%.2lf",
  L" Fase ent (%d) @3=%.2lf",
  L" @ (%d) @3=%.2lf",
  L" Фаза загр. (%d) @3=%.2lf"  };

  static wchar_t* string_1876[NLANGS] = {
  L" Dist=%.2lf",
  L" 距离=%.2lf",
  L" Dist=%.2lf",
  L" @",
  L" Расстояние =%.2lf"  };

  static wchar_t* string_1878[NLANGS] = {
  L" Inp-Rej1=%.2lf",
  L" 进料到剔除托盘1=%.2lf",
  L" Entrada-Rechaz1=%.2lf",
  L" @",
  L" загр. - ОТБР. 1=%.2lf"  };
 

  static wchar_t* string_1880[NLANGS] = {
  L" Inp-Rej2=%.2lf",
  L" 进料到剔除托盘2=%.2lf",
  L" Entrada-Rechaz2=%.2lf",
  L" @2=%.2lf",
  L" загр. - ОТБР. 2=%.2lf"  };

  static wchar_t* string_1882[NLANGS] = {
  L" Inp-Good=%.2lf",
  L" 进料-合格品=%.2lf",
  L" Entra-corre=%.2lf",
  L" @=%.2lf",
  L" загр.- Пригодный продукт=%.2lf"  };

  static wchar_t* string_1884[NLANGS] = {
  L" Height=%d",
  L" 高度=%d",
  L" Altura=%d",
  L" @=%d",
  L" Высота=%d"  };


  static wchar_t* string_1890[NLANGS] = {
  L"Input-SideWall A dist",
  L"进料到粉饼侧面A相机的距离",
  L"Dista. A entrada-superf.",
  L"@",
  L"загр.- расстояние стенка A"  };

  static wchar_t* string_1892[NLANGS] = {
  L"Input-SideWall B dist",
  L"进料到粉饼侧面B相机的距离",
  L"Dista. B entrada-superf.",
  L"@",
  L"загр.- расстояние стенка B"  };

  static wchar_t* string_1894[NLANGS] = {
  L"Input-SideWall C dist",
  L"进料到粉饼侧面相机C的距离",
  L"Dista. C entrada-superf.",
  L"@",
  L"загр.- расстояние стенка C"  };

  static wchar_t* string_1896[NLANGS] = {
  L"Input-SideWall D dist",
  L"进料到粉饼侧面相机D的距离",
  L"Dista. D entrada-superf.",
  L"@",
  L"загр.- расстояние стенка D"  };


  static wchar_t* string_1897a[NLANGS] = {
  L"Input-STN1_A dist",
  L"进料-工位1_A距离",
  L"# Dista. A entrada-superf.",
  L"@",
  L"загр.- расстояние стенка A"  };

  static wchar_t* string_1897b[NLANGS] = {
  L"Input-STN1_B dist",
  L"进料-工位1_B距离",
  L"# Dista. B entrada-superf.",
  L"@ B",
  L"загр.- расстояние стенка B"  };

  static wchar_t* string_1897c[NLANGS] = {
  L"Input-STN1_C dist",
  L"进料-工位1_C距离",
  L"# Dista. C entrada-superf.",
  L"@ C",
  L"загр.- расстояние стенка C"  };

  static wchar_t* string_1897d[NLANGS] = {
  L"Input-STN1_D dist",
  L"进料-工位1_D距离",
  L"# Dista. D entrada-superf.",
  L"@ D",
  L"загр.- расстояние стенка D"  };



// VIAL SHOULDER CAP INSPECTION
  static wchar_t* string_1910[NLANGS] = {
  L"Shoulder chk height (*)",
  L"瓶肩检测高度",
  L"Altura revisión hombro"   ,
  L"@",
  L"Высота инспекции горлышка (*)"  };

  static wchar_t* string_1914[NLANGS] = {
  L"Shoulder min slope",
  L"瓶肩最小坡度",
  L"Pendiente mín. hombro"   ,
  L"@",
  L"MIN уклон горлышка"  };

  static wchar_t* string_1916[NLANGS] = {
  L"Shoulder max slope",
  L"瓶肩最大坡度",
  L"Pendiente máx. hombro"   ,
  L"@",
  L"MAX уклон горлышка"  };

  static wchar_t* string_1918[NLANGS] = {
  L"Shoulder lenght",
  L"瓶肩长度",
  L"Longitud hombro"   ,
  L"@",
  L"Длина горлышка"  };

  static wchar_t* string_1920[NLANGS] = {
  L"Shoulder max error",
  L"瓶肩最大",
  L"Error máx. hombro"   ,
  L"@",
  L"MAX ошибок горлышка"  };


  static wchar_t* string_1930[NLANGS] = {
  L"Shoulder GOOD (%d/%d)",
  L"瓶肩检测合格 (%d/%d)",
  L"Hombro CORRECTO (%d/%d)"   ,
  L"@ (%d/%d)",
  L"ПРИГОДЕН: горлышко"  };

  static wchar_t* string_1932[NLANGS] = {
  L"Shoulder REJECT (%d/%d)",
  L"瓶肩检测剔除 (%d/%d)",
  L"Hombro RECHAZO (%d/%d)"   ,
  L"@ (%d/%d)",
  L"ОТБР.: горлышко(%d/%d)"  };


static wchar_t* string_1950[NLANGS] = {
  L"HGA",
  L"HGA",
  L"HGA",
  L"HGA",
  L"HGA"  };

static wchar_t* string_1952[NLANGS] = {
  L"Trigger phase A",
  L"触发相位A",
  L"Fase de disparo A",
  L"@",
  L"Фазы запуска A"  };

static wchar_t* string_1954[NLANGS] = {
  L"Trigger phase B",
  L"触发相位B",
  L"Fase de disparo B",
  L"@",
  L"Фазы запуска B"  };

static wchar_t* string_1956[NLANGS] = {
  L"Dist load-HGA",
  L"距离",
  L"Dist de carga HGA",
  L"@",
  L"Расстояние"  };

  //Pour added on 20200531
  static wchar_t* string_1957[NLANGS] = {
  L"Percentage of vial",
  L"瓶子光程占比",
  L"Dist de carga HGA",
  L"@",
  L"Расстояние"  };

  /*
static wchar_t* string_1958[NLANGS] = {
  L"HGA threshold HIGH (*)",
  L"头部气体分析高阈值 (*)",
  L"Umbral de HGA (*)",
  L"@ (*)",
  L"Порог HGA (*)"  };
  */


  static wchar_t* string_1958[NLANGS] = {
  L"HGA threshold (*)",
  L"头部气体分析阈值 (*)",
  L"Umbral de HGA (*)",
  L"@ (*)",
  L"Порог HGA (*)"  };

static wchar_t* string_1959[NLANGS] = {
  L"Normalization air set",
  L"归一化空气值",
  L"(*)",
  L"@ (*)",
  L"Порог HGA (*)"  };

static wchar_t* string_1960[NLANGS] = {
  L"Normalization min set",
  L"归一化最小值",
  L"(*)",
  L"@ (*)",
  L"(*)"  };

/*
static wchar_t* string_1960[NLANGS] = {
  L"HGA threshold LOW (*)",
  L"头部气体分析低阈值 (*)",
  L"Umbral de HGA (*)",
  L"@ (*)",
  L"Порог HGA (*)"  };
  */

  static wchar_t* string_1961[NLANGS] = {
  L"Normalization max set",
  L"归一化最大值",
  L"(*)",
  L"@ (*)",
  L"(*)"  };


static wchar_t* string_1962[NLANGS] = {
  L" HGA %d",
  L" HGA %d",
  L" HGA %d",
  L" HGA %d",
  L" HGA %d"  };

  static wchar_t* string_1963[NLANGS] = {
  L" HGA%2.1f%%",
  L" HGA%2.1f%%",
  L" HGA%2.1f%%",
  L" HGA%2.1f%%",
  L" HGA%2.1f%%"  };

static wchar_t* string_1964[NLANGS] = {
  L"Reject to tray1",
  L"剔除到托盘1",
  L"Rechazo a charola 1",
  L"@",
  L"ОТБР. в лоток 1"  };

static wchar_t* string_1966[NLANGS] = {
  L"Reject to tray2",
  L"剔除到托盘2",
  L"Rechazo a charola 2",
  L"@",
  L"ОТБР. в лоток 2"  };

static wchar_t* string_1967[NLANGS] = {
  L"Reject to tray3",
  L"剔除到托盘3",
  L"Rechazo a charola 3",
  L"@ 3",
  L"ОТБР. в лоток 3"  };

  //Pour removed 20170714
/*
static wchar_t* string_1970[NLANGS] = {
  L"HGA Data rec Off",
  L" 关闭头部气体分析数据收集",
  L"# Apagar Reg.Datos de entrada",
  L"@",
  L"& Запись входных данных ВЫКЛ. "  };

static wchar_t* string_1972[NLANGS] = {
  L"HGA Data rec ON",
  L" 打开头部气体分析数据记录",
  L" # Encender Reg.Datos de entrada",
  L" @ ",
  L" & Запись входных данных ВКЛ."  };

static wchar_t* string_1976a[NLANGS] = {
  L"Serial communication",
  L"串联通讯",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976b[NLANGS] = {
  L"Scan minimum set",
  L"扫描最小值",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976c[NLANGS] = {
  L"Scan maximum set",
  L"扫描最大值",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976d[NLANGS] = {
  L"Wave type set",
  L"波形选项",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976e[NLANGS] = {
  L"Normalization min set",
  L"归化最小值",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976f[NLANGS] = {
  L"Normalization max set",
  L"归化最大值",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976g[NLANGS] = {
  L"Normalization air set",
  L"归化空气值",
  L"#",
  L"@",
  L"&"  };
*/
  //Pour removed 20170714
  //Pour added 20170714_Start
  static wchar_t* string_1970[NLANGS] = {
  L"HGA Data rec Off",
  L" 关闭头部气体分析数据收集",
  L"# Apagar Reg.Datos de entrada",
  L"@",
  L"& Запись входных данных ВЫКЛ. "  };

static wchar_t* string_1972[NLANGS] = {
  L"HGA Data rec ON",
  L" 打开头部气体分析数据记录",
  L" # Encender Reg.Datos de entrada",
  L" @ ",
  L" & Запись входных данных ВКЛ."  };

static wchar_t* string_1976a[NLANGS] = {
  L"Serial communication",
  L"串联通讯",
  L"#",
  L"@",
  L"&"  };
//Pour added 20170720
  static wchar_t* string_1976a0[NLANGS] = {
  L"EN_RS232COM",
  L"开启RS232通信",
  L"#",
  L"@",
  L"&"  };
//+++++++++++++++++++++++++
  static wchar_t* string_1976a1[NLANGS] = {
  L"HGA sampling",
  L"显示波形",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_1976a2[NLANGS] = {
  L"HGA Display Wave",
  L"启用采样功能",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_1976a3[NLANGS] = {
  L"EN HGA reading",
  L"启用读入",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_1976a4[NLANGS] = {
  L"EN HGA writing",
  L"启用写入",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976b[NLANGS] = {
  L"HGA WRITETYPE",
  L"HGA 写入类型",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976c[NLANGS] = {
  L"HGA WRITEADDRESS",
  L"HGA 写入地址",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976d[NLANGS] = {
  L"HGA WRITEDATA",
  L"HGA 写入数据",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976e[NLANGS] = {
  L"HGA READTYPE",
  L"HGA 读取类型",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976f[NLANGS] = {
  L"HGA READADDRESS",
  L"HGA 读取地址",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_1976g[NLANGS] = {
  L"HGA READDATA",
  L"HGA 读取值",
  L"#",
  L"@",
  L"&"  };
  //Pour added 20170714_End


/*
#define ENGLISH   0
#define CHINESE   1   // ^
#define SPANISH   2   // #
#define TURKISH   3   // @
#define RUSSIAN   4   // &
*/


// PARAMETRI HV LD

  static wchar_t* string_1980[NLANGS] = {
  L"Enable HV rejects to tray 1",
  L"使能高压放电检测到托盘1",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_1982[NLANGS] = {
  L"Enable HV rejects to tray 2",
  L"使能高压放电检测到托盘2",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_1988[NLANGS] = {
  L"Dev2 reject suction lenght",
  L"分离点2剔除吸气长度",
  L"#",
  L"@",
  L"DEV2 Удалить отсос длина"    };

  static wchar_t* string_1990[NLANGS] = {
  L"Dev2 reject blow lenght",
  L"分离点2剔除吹气长度",
  L"#",
  L"@",
  L"DEV2 Удалить взрывание длина"    };

  static wchar_t* string_1992[NLANGS] = {
  L"Dev2 good sw suction lenght",
  L"分离点2合格品吸气长度",
  L"#",
  L"@",
  L"DEV2 соответствие отсос длина"    };

  static wchar_t* string_1994[NLANGS] = {
  L"Dev2 good sw blow lenght",
  L"分离点2合格品吹气长度",
  L"#",
  L"@",
  L"DEV2 соответствие взрывание длина"    };

  //Pour added for HV rejection 20180127
    static wchar_t* string_1988a[NLANGS] = {
  L"Dev3 reject suction lenght",
  L"分离点3剔除吸气长度",
  L"#",
  L"@",
  L"&"    };

  static wchar_t* string_1990a[NLANGS] = {
  L"Dev3 reject blow lenght",
  L"分离点3剔除吹气长度",
  L"#",
  L"@",
  L"&"    };

  static wchar_t* string_1992a[NLANGS] = {
  L"Dev3 good sw suction lenght",
  L"分离点3合格品吸气长度",
  L"#",
  L"@",
  L"&"    };

  static wchar_t* string_1994a[NLANGS] = {
  L"Dev3 good sw blow lenght",
  L"分离点3合格品吹气长度",
  L"#",
  L"@",
  L"&"    };
  //Pour added 20180127



// PARAMETRI ISPEZIONE

static wchar_t* string_2010[NLANGS] = {
  L"Camera %d parameters",
  L"相机%d参数",
  L"Parámetros %d cámara",
  L"@",
  L"Параметры камеры %d"  };

static wchar_t* string_2020[NLANGS] = {
  L"Area adjustment",
  L"区域调整",
  L"Ajuste de zona",
  L"@",
  L"Зона настройки"  };

static wchar_t* string_2022[NLANGS] = {
  L"Area adjustment is not possible while movie or inspection are active.\r\nPlease stop movie/inspection and try again",
  L"当实时图像或者检测开启时，检测区域不能调整.\r\n请停止实时图像/检测并再次尝试",
  L"Ajuste de zona no posible mientras la inspección este activa.\r\nDetenga la inspección e intente nuevamente ",
  L"@",
  L"Зональная настройка невозможна, т.к. активирована инспекция или режим изображения.\r\nВЫКЛ.ючите инспекцию / режим  и повторите "  };

static wchar_t* string_2026[NLANGS] = {
  L"Mode change is not possible while movie or inspection are active.\r\nPlease stop movie/inspection and try again",
  L"当实时或者检测激活不能改变模式\r\n请停止实时图像/检测并再次尝试",
  L"Cambio de modo no posible mientras a inspección este activa.\r\nDetenga la inspección e intente nuevamente ",
  L"@",
  L"Переключение режимов невозможно, т.к. активирована инспекция или режим  изображения .\r\nВЫКЛ.ючите инспекцию / режим  и повторите "  };


static wchar_t* string_2028[NLANGS] = {
  L"Image to show",
  L"显示图片",
  L"Imagen a Mostrar",
  L"@",
  L"Показать изображение"  };


static wchar_t* string_2030[NLANGS] = {
  L"Image scale coeff",
  L"图像比例尺系数",
  L"Coef. escala imagen",
  L"@",
  L"Коэфф. масштаб. изобр."  };

static wchar_t* string_2032[NLANGS] = {
  L"Number of images",
  L"图片数目",
  L"Número de imagenes",
  L"@",
  L"Кол-во изображений"  };



static wchar_t* string_2034[NLANGS] = {
  L"Reflections menu",
  L"反射学习菜单",
  L"Menú de reflexiones",
  L"@",
  L"Меню изображения - обучение"  };

static wchar_t* string_2036[NLANGS] = {
  L"Export current learning",
  L"导出当前学习图像",
  L"Exportar aprendizaje actual",
  L"@",
  L"Экспорт текущего изобр. обучения"  };

static wchar_t* string_2038[NLANGS] = {
  L"Import learning from images",
  L"从图片导入学习图像",
  L"Importar aprendizaje de imagenes",
  L"@",
  L"Импорт учебного изобр. из фото"  };




static wchar_t* string_2040[NLANGS] = {
  L"Do you want to keep processed reflections?\r\nYES=Keep - NO=Restore previous data",
  L"保存学习后的图像？\r\n是=保存 - 否=保存之前的数据",
  L"¿Desea mantener las reflexiones procesadas?\r\n SI=Mantener - No= Restaure info previa",
  L"@",
  L"Хотите сохранить изображения?\r\nДа=Сохранить - Нет=Восстановить предыдущие данные"  };

static wchar_t* string_2042[NLANGS] = {
  L"Filter reflections",
  L"图像过滤",
  L"Filtrar reflexiones",
  L"@",
  L"Фильтр изображений"  };

static wchar_t* string_2044[NLANGS] = {
  L"Filtering reflections: wait...",
  L"图像过滤中: 等待...",
  L"Filtrando reflexiones: espere...",
  L"@",
  L"Изображения фильтруются: подождите…"  };

static wchar_t* string_2046[NLANGS] = {
  L"Reflections learning in progress...",
  L"学习模式中...",
  L"Aprendizaje reflexiones en progeso...",
  L"@",
  L"Применение изображения - в процессе…"  };
 
static wchar_t* string_2048[NLANGS] = {
  L"Reflections learning completed",
  L"学习模式完成",
  L"Aprendizaje reflexiones completado",
  L"@",
  L"Применение изображения – завершено!"  };
 
static wchar_t* string_2050[NLANGS] = {
  L"Reflections cleared",
  L"图像清除",
  L"Reflexiones borradas",
  L"@",
  L"Изображения очищены"  };

static wchar_t* string_2052[NLANGS] = {
  L"Learn reflections",
  L"学习图像",
  L"Aprender reflexiones",
  L"@",
  L"Изображения обучения"  };

static wchar_t* string_2054[NLANGS] = {
  L"Show current reflections",
  L"显示当前图像",
  L"Mostrar reflexiones actuales",
  L"@",
  L"Показать текущее изображение"  };

static wchar_t* string_2056[NLANGS] = {
  L"Do you want to keep current reflections?\r\nYES=Keep and add - NO=Clean and restart",
  L"保存当前图像？\r\n是=保存并添加 - 否= 清空并重新开始",
  L"¿Desea mantener las reflexiones actuales?\r\n SI=Mantener y agregar - NO= borrar y reiniciar",
  L"@",
  L"Сохранить текущее изображение？\r\nДа=Сохранить и добавить- Нет=Очистить и начать заново"  };

static wchar_t* string_2058[NLANGS] = {
  L"Current learning",
  L"当前学习",
  L"Aprendizaje actual",
  L"@",
  L"Текущие обучение"  };

  
static wchar_t* string_2060[NLANGS] = {
  L"Exposure duration",
  L"曝光时间",
  L"Tiempo de exposición",
  L"@",
  L"Длительность экспозиции"  };

static wchar_t* string_2062[NLANGS] = {
  L"Camera gain",
  L"相机增益",
  L"Ganancia cámara",
  L"@",
  L"Усиление камеры"  };

static wchar_t* string_2064[NLANGS] = {
  L"Images averaging",
  L"图片均衡",
  L"Imagenes promedio",
  L"@",
  L"Изображение сбалансировано"  };

static wchar_t* string_2066[NLANGS] = {
  L"Frame rate (*)",
  L"帧率 (*)",
  L"Cuadros por seg(*)",
  L"@",
  L"Частота кадров (*)"  };

static wchar_t* string_2068[NLANGS] = {
  L"Trigger waiting",
  L"触发等待",
  L"Gatillo en espera",
  L"@",
  L"Ожидание триггера"  };

static wchar_t* string_2070[NLANGS] = {
  L"Changes sensitivity",
  L"灵敏度",
  L"Cambiar sensibilidad",
  L"@",
  L"Чувствительность"  };

static wchar_t* string_2072[NLANGS] = {
  L"Particle inspection",
  L"粒子检测",
  L"Inspección partícula",
  L"@",
  L"Инспекция на включения"  };

static wchar_t* string_2074[NLANGS] = {
  L"Cosmetic inspection",
  L"外观检测",
  L"Inspección cosmetica",
  L"@",
  L"Инспекция внешних дефектов"  };

static wchar_t* string_2076[NLANGS] = {
  L"BACKLIGHT MODE",
  L"背光模式",
  L"MODO LUZ TRASERA",
  L"@",
  L"Режим заднего света"  };


  static wchar_t* string_2078[NLANGS] = {
  L"Needle shield inspection",
  L"针帽外观检测",
  L"Inspección partícula",
  L"@",
  L"Инспекция на включения"  };


static wchar_t* string_2080[NLANGS] = {
  L"Maximum error",
  L"最大误差",
  L"Error máximo",
  L"@",
  L"MAX ошибок"  };

  static wchar_t* string_2080c[NLANGS] = {
  L"Maximum error 1",
  L"最大误差（上）",
  L"Error máximo",
  L"@",
  L"MAX ошибок"  };


static wchar_t* string_2082[NLANGS] = {
  L"Min num pt per image",
  L"每个图片最小比值",
  L"Min Cant. de part. por imagen",
  L"@",
  L"Min соотношение фото"  };

static wchar_t* string_2084[NLANGS] = {
  L"Min num of images",
  L"最少图片数目",
  L"Min Numero de imagenes",
  L"@",
  L"MIN количество фото"  };




static wchar_t* string_2090[NLANGS] = {
  L"Minimum width",
  L"最小宽度",
  L"Ancho minimo",
  L"@",
  L"MIN ширина"  };


static wchar_t* string_2092[NLANGS] = {
  L"Minimum height",
  L"最低高度",
  L"Altura minima",
  L"@",
  L"Низшая высота"  };

static wchar_t* string_2094[NLANGS] = {
  L"Minimum elongation",
  L"最小倍数",
  L"Alargamiento minimo",
  L"@",
  L"MIN число"  };

static wchar_t* string_2096[NLANGS] = {
  L"Minimum avg diff",
  L"最小平均差值",
  L"Dif prom minima",
  L"@",
  L"MIN средняя разница"  };

static wchar_t* string_2102[NLANGS] = {
  L"Max distance",
  L"最大距离",
  L"Distancia maxima",
  L"@",
  L"MAX расстояние"  };

static wchar_t* string_2104[NLANGS] = {
  L"Min size",
  L"最小尺寸",
  L"Tamaño minimo",
  L"@",
  L"MIN размер"  };

static wchar_t* string_2106[NLANGS] = {
  L"Max size",
  L"最大尺寸",
  L"Tamaño maximo",
  L"@",
  L"MAX размер"  };

static wchar_t* string_2108[NLANGS] = {
  L"Min depth",
  L"最小深度",
  L"profundidad minima",
  L"@",
  L"MIN глубина"  };


static wchar_t* string_2110[NLANGS] = {
  L"Min abs lum (lower)",
  L"最小绝对值亮度 (底部区域)",
  L"Min abs lum (inferior)",
  L"@",
  L"MIN  абсолютная яркость（в дне）"  };

static wchar_t* string_2112[NLANGS] = {
  L"Min abs lum (upper)",
  L"最小绝对值亮度 (上部区域)",
  L"Min abs lum (superior)",
  L"@",
  L"MIN  абсолютная яркость（наверху）"  };







static wchar_t* string_2120[NLANGS] = {
  L"Gray levels",
  L"灰度",
  L"Nivel de grises",
  L"@",
  L"Градации серого"  };

static wchar_t* string_2122[NLANGS] = {
  L"Pseudocolors",
  L"假彩色",
  L"Pseudocolores",
  L"@",
  L"Псевдоцвета"  };

static wchar_t* string_2124[NLANGS] = {
  L"Show only rejects",
  L"仅仅显示剔除",
  L"Mostrar solo rechazos",
  L"@",
  L"Показать только ОТБР."  };

static wchar_t* string_2130[NLANGS] = {
  L"Results recording",
  L"结果记录",
  L"Resultados grabados",
  L"@",
  L"Результаты записываются"  };

static wchar_t* string_2140[NLANGS] = {
  L"Liquid mode ON",
  L"水针检测模式",
  L"Modo líquido ON",
  L"@",
  L"Режим инспекции ЖЛФ ВКЛ."  };

static wchar_t* string_2142[NLANGS] = {
  L"Freeze-dried mode ON",
  L"冻干检测模式",
  L"Modo liofilizado ON",
  L"@",
  L"Режим инспекции ЛИО ВКЛ."  };

/*
#define ENGLISH   0
#define CHINESE   1   // ^
#define SPANISH   2   // #
#define TURKISH   3   // @
#define RUSSIAN   4   // &
*/

//Pour marked for OPENCV_PARTICLE on 20190514

static wchar_t* string_2150[NLANGS] = {
	L"Number of images",
	L"图像张数",
	L"#",
	L"@",
	L"Количество изображений" };


static wchar_t* string_2152[NLANGS] = {
	L"Partcile in single frame",
	L"单帧粒子",
	L"#",
	L"@",
	L"однокадровая частица" };

static wchar_t* string_2154[NLANGS] = {
	L"Inspection Ratio",
	L"检测比例",
	L"#",
	L"@",
	L"контрольный коэффициент" };

static wchar_t* string_2156[NLANGS] = {
	L"Inspection Sensitivity",
	L"检测灵敏度",
	L"#",
	L"@",
	L"чувствительность к обнаружению" };

//Pour marked for OPENCV_PARTCIEL on 20190514


static wchar_t* string_2200[NLANGS] = {
  L"Height to check Tcknss",
  L"检查厚度区域的高度范围",
  L"Altura para revisar grosor",
  L"@",
  L"Высота проверки толщину"  };

static wchar_t* string_2202[NLANGS] = {
  L"Threshold for Tcknss pts",
  L"厚度点数的区间",
  L"Umbral para puntos grosor",
  L"@",
  L"Порог толщины"  };

static wchar_t* string_2204[NLANGS] = {
  L"Min num of Tcknss pts (*)",
  L"最小厚度点数 (*)",
  L"Número mín. puntos grosor (*)",
  L"@",
  L"MIN толщина (*)"  };


static wchar_t* string_2210[NLANGS] = {
  L" - Tcknss REJECT %d/%d",
  L" - 厚度检测剔除 %d/%d",
  L" - Grosor RECHAZO %d/%d",
  L"@",
  L" –ОТБР.: толщина %d/%d "  };

static wchar_t* string_2212[NLANGS] = {
  L" - Tcknss GOOD %d/%d",
  L" - 厚度检测合格 %d/%d",
  L" - Grosor CORRECTO %d/%d",
  L"@",
  L" –ПРИГОДЕН: толщина %d/%d "  };

static wchar_t* string_2214[NLANGS] = {
  L" - Tcknss DISABLED",
  L" - 厚度检测关闭",
  L" - Grosor DESACTIVADO",
  L"@",
  L" –Инспекция толщины ОТКЛ"  };



// OPACITY / DENSITY CONTROL
static wchar_t* string_2240[NLANGS] = { // too much difference (liquid too much thick/opaque)
  L"Density control",
  L"浓度控制",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_densityrej[NLANGS] = {
  L"Density rejects",
  L"浓度剔除",
  L"#",
  L"@",
  L"&"  };



static wchar_t* string_2250[NLANGS] = { // too much difference (liquid too much thick/opaque)
  L"Density too HIGH REJECT %d (%d/%d)",
  L"浓度太高剔除 %d (%d/%d)",
  L"# %d (%d/%d)",
  L"@ %d (%d/%d)",
  L"& %d (%d/%d)"  };

static wchar_t* string_2252[NLANGS] = { // too little difference (liquid too little thick/opaque)
  L"Density too LOW REJECT %d (%d/%d)",
  L"浓度太低剔除 %d (%d/%d)",
  L"# %d (%d/%d)",
  L"@ %d (%d/%d)",
  L"& %d (%d/%d)"  };

static wchar_t* string_2254[NLANGS] = { // good difference
  L"Density GOOD %d (%d/%d)",
  L"浓度检测合格品 %d (%d/%d)",
  L"# %d (%d/%d)",
  L"@ %d (%d/%d)",
  L"& %d (%d/%d)"  };

static wchar_t* string_2258[NLANGS] = { 
  L"Density control DISABLED",
  L"浓度控制未使能",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_2260[NLANGS] = {
  L"Minimum allowed Density (*)",
  L"最小允许浓度值 (*)",
  L"# (*)",
  L"@ (*)",
  L"& (*)"  };

static wchar_t* string_2262[NLANGS] = {
  L"Maximum allowed Density (*)",
  L"最大允许浓度值 (*)",
  L"# (*)",
  L"@ (*)",
  L"& (*)"  };

static wchar_t* string_2268[NLANGS] = {
  L"Distance top-bottom area",
  L"顶部和底部检测面积距离",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_2270[NLANGS] = {
  L"Height of top area",
  L"顶部区域高度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_2272[NLANGS] = {
  L"hor/ver scan step",
  L"水平/竖直扫描步骤",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_2280[NLANGS] = {
  L"Enable density rejects",
  L"使能浓度剔除",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_2282[NLANGS] = {
  L"Density rejects to Tray2",
  L"浓度剔除到托盘2",
  L"#",
  L"@",
  L"&"  };


// SIDEWALL SIDECAKE AND TOPCAKE CONTROL STRINGS

static wchar_t* string_2302[NLANGS] = {
  L"%s REJECT (%d/%d)",
  L"%s 剔除 (%d/%d)",
  L"%s RECHAZO (%d/%d)",
  L"@",
  L"%s ОТБР. (%d/%d)"  };

static wchar_t* string_2304[NLANGS] = {
  L"%s GOOD (%d/%d)",
  L"%s 合格 (%d/%d)",
  L"%s CORRECTO (%d/%d)",
  L"@",
  L"%s ПРИГОДЕН (%d/%d)"  };

    // added for deyuantang sidecake
static wchar_t* string_2305[NLANGS] = {
  L"Cake avg brightness %d(%d/%d)",
  L"粉饼平均亮度 %d(%d/%d)",
  L"*** %d(%d/%d)",
  L"@",
  L"%d (%d/%d)"  };
// added end by yang 20190808


static wchar_t* string_2306[NLANGS] = {
  L"SideCake",
  L"粉饼侧面",
  L"Lateral",
  L"@",
  L"Бок ЛИО"  };

static wchar_t* string_2308[NLANGS] = {
  L"TopCake",
  L"粉饼上面",
  L"Superior",
  L"@",
  L"Верх  ЛИО"  };

static wchar_t* string_2310[NLANGS] = {
  L"Heel",
  L"瓶根",
  L"Base",
  L"@",
  L"Нижняя ЛИО"  };





static wchar_t* string_2312[NLANGS] = {
  L"Cake volume REJECT: too low %d,%d,%d",
  L"粉饼装量剔除: 太低 %d,%d,%d",
  L"Volúmen pastel RECHAZO: muy bajo %d,%d,%d",
  L"@",
  L"ОТБР. объем ЛИО: низкий %d,%d,%d "  };

  static wchar_t* string_2312a[NLANGS] = {
  L"Stopper REJECT: too low %d,%d,%d",
  L"胶塞位置剔除: 太低 %d,%d,%d",
  L"Volúmen pastel RECHAZO: muy bajo %d,%d,%d",
  L"@",
  L"ОТБР. объем ЛИО: низкий %d,%d,%d "  };



static wchar_t* string_2314[NLANGS] = {
  L"Cake volume REJECT: too high %d,%d,%d",
  L"粉饼装量剔除: 太高 %d,%d,%d",
  L"Volúmen pastel RECHAZO:muy alto %d,%d,%d",
  L"@",
  L"ОТБР. объем ЛИО:  большой %d,%d,%d "  };

  static wchar_t* string_2314a[NLANGS] = {
  L"Stopper REJECT: too high %d,%d,%d",
  L"胶塞位置剔除: 太高 %d,%d,%d",
  L"Volúmen pastel RECHAZO:muy alto %d,%d,%d",
  L"@",
  L"ОТБР. объем ЛИО:  большой %d,%d,%d "  };


static wchar_t* string_2316[NLANGS] = {
  L"Cake volume GOOD %d,%d,%d",
  L"粉饼装量合格 %d,%d,%d",
  L"Volumen pastel BIEN %d,%d,%d",
  L"@",
  L"ПРИГОДЕН: объем ЛИО %d,%d,%d "  };

  static wchar_t* string_2316a[NLANGS] = {
  L"Stopper position GOOD %d,%d,%d",
  L"胶塞位置合格 %d,%d,%d",
  L"Volumen pastel BIEN %d,%d,%d",
  L"@",
  L"ПРИГОДЕН: объем ЛИО %d,%d,%d "  };

static wchar_t* string_2320[NLANGS] = {
  L"Lower Cake volume NOT FOUND",
  L"粉饼底部未发现",
  L"Vol pastel inferior NO ENCONTRADO",
  L"@",
  L"НЕ ОБНАРУЖЕН: объем ЛИО низ"  };

static wchar_t* string_2322[NLANGS] = {
  L"Upper Cake volume NOT FOUND",
  L"粉饼顶部未发现",
  L"Vol. pastel superior NO ENCONTRADO",
  L"@",
  L"НЕ ОБНАРУЖЕН: объем ЛИО верх"  };

static wchar_t* string_2324[NLANGS] = {
  L"Cake volume DISABLED",
  L"粉饼装量检测未启用",
  L"Vol pastel DESHABILITADO",
  L"@",
  L"Инспекция объема ЛИО ОТКЛ"  };


    static wchar_t* string_2326[NLANGS] = {
  L"Min cake volume (*)",
  L"最小粉饼量",
  L"#",
  L"@",
  L"минимальный объем торта"  };

static wchar_t* string_2328[NLANGS] = {
  L"Max cake volume (*)",
  L"最大粉饼量",
  L"#",
  L"@",
  L"максимальный объем торта"  };


static wchar_t* string_2330[NLANGS] = {
  L"Sidewall REJECT (%d/%d)",
  L"外观检测剔除 (%d/%d)",
  L"Superficie RECHAZO (%d/%d)",
  L"@ (%d/%d)",
  L"ОТБР.: дефекты стенки (%d/%d)"  };

  static wchar_t* string_2330a[NLANGS] = {
  L"Sidewall REJECT (%d/%d/%d)",
  L"外观检测剔除 (%d/%d/%d)",
  L"Superficie RECHAZO (%d/%d/%d)",
  L"@ (%d/%d/%d)",
  L"ОТБР.: дефекты стенки (%d/%d/%d)"  };


static wchar_t* string_2332[NLANGS] = {
  L"Sidewall GOOD (%d/%d)",
  L"外观检测合格品 (%d/%d)",
  L"Superficie BIEN (%d/%d)",
  L"@ (%d/%d)",
  L"ПРИГОДЕН: дефекты стенки (%d/%d)"  };

static wchar_t* string_2340[NLANGS] = {
  L"Side Wall control",
  L"外观检测",
  L"Control superficie",
  L"@",
  L"Инспекция дефектов стенок"  };

static wchar_t* string_2342[NLANGS] = {
  L"Side Cake control",
  L"粉饼侧面检测",
  L"Control lateral",
  L"@",
  L"Инспекция бок ЛИО"  };

static wchar_t* string_2344[NLANGS] = {
  L"Top Cake control",
  L"粉饼上面检测",
  L"Control superior",
  L"@",
  L"Инспекция верхней ЛИО"  };

static wchar_t* string_2346[NLANGS] = {
  L"Heel control",
  L"瓶根检测",
  L"Control Base",
  L"@",
  L"Инспекция дно ЛИО"  };


// Sidewall
static wchar_t* string_2350[NLANGS] = {
  L"Min brgtn for top area (*)",
  L"顶部区域最小亮度",
  L"Mín brillo zona superior",
  L"@",
  L"MIN яркость cверху"  };

static wchar_t* string_2352[NLANGS] = {
  L"Offset of top area",
  L"顶部区域补偿",
  L"Compensación zona super.",
  L"@",
  L"Смещение верхней части"  };

static wchar_t* string_2354[NLANGS] = {
  L"Analysis area height",
  L"分析区域高度",
  L"Altura zona analísis",
  L"@",
  L"Анализ высоты"  };


static wchar_t* string_2360[NLANGS] = {
  L"Upper sidewall NOT FOUND",
  L"外观检测上部未找到",
  L"Superf. superior NO ENCONT.",
  L"@",
  L"НЕ ОБНАРУЖЕНО: верх боковой стенки"  };

static wchar_t* string_2362[NLANGS] = {
  L"Sidewall analysys area NOT FOUND",
  L"外观检测分析区域未找到",
  L"Zona analísis superf. NO ENCONTRADA",
  L"@",
  L"НЕ ОБНАРУЖЕНО: зона инспекции боковой"  };


// Sidecake
static wchar_t* string_2370[NLANGS] = {
  L"Min brgtn for bottom area (*)",
  L"底部区域最小亮度",
  L"Mín brillo zona del fondo",
  L"@",
  L"MIN яркость основания (*)"  };

static wchar_t* string_2372[NLANGS] = {
  L"Offset of bottom area",
  L"底部区域补偿",
  L"Compensación zona fondo",
  L"@",
  L"Компенсания донной части"  };

static wchar_t* string_2374[NLANGS] = {
  L"Sidecake NOT found",
  L"粉饼侧面检测未找到",
  L"Lateral pastel NO ENCONT.",
  L"@",
  L"НЕ ОБНАРУЖЕНО: бок ЛИО"  };


// BOTTOM PROFILE/THICKNESS

  /*
#define ENGLISH   0
#define CHINESE   1   // ^
#define SPANISH   2   // #
#define TURKISH   3   // @
#define RUSSIAN   4   // &
  */

static wchar_t* string_2380[NLANGS] = {
  L"Bottom profile REJECT (%d/%d)",
  L"底部轮廓剔除 (%d/%d)",
  L"# (%d/%d)",
  L"@ (%d/%d)",
  L"Низ.контур не квалифицирован (%d/%d)"  };

static wchar_t* string_2382[NLANGS] = {
  L"Bottom profile GOOD (%d/%d)",
  L"底部轮廓合格 (%d/%d)",
  L"# (%d/%d)",
  L"@ (%d/%d)",
  L"Низ.контур квалифицирован (%d/%d)"  };

static wchar_t* string_2386[NLANGS] = {
  L"Max bottom thickness",
  L"最大底部厚度",
  L"#",
  L"@",
  L"Макс.толщина дна"  };

static wchar_t* string_2388[NLANGS] = {
  L"Max bottom error",
  L"最大底部错误值",
  L"#",
  L"@",
  L"Макс.кол.ошибки дна"  };

static wchar_t* string_2390[NLANGS] = {
  L"Upper area brightn change",
  L"上部区域亮度变化",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_2391[NLANGS] = {
  L"Upper area band height",
  L"上部区域设定高度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_2392[NLANGS] = {
  L"Lower area brightn change",
  L"底部区域亮度变化",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_2393[NLANGS] = {
  L"Lower area band height",
  L"底部区域设定高度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_2394[NLANGS] = {
  L"Min abs brightn (liquid)",
  L"最小绝对亮度（水针）",
  L"#",
  L"@",
  L"Мин.абсолютная яркость(жидкость)"  };

  static wchar_t* string_2395[NLANGS] = {
  L"Max abs brightn (liquid)",
  L"最大绝对亮度（水针）",
  L"#",
  L"@",
  L"Мин.абсолютная яркость(жидкость)"  };


static wchar_t* string_2396[NLANGS] = {
  L"Min abs brightn (bottom)",
  L"最小绝对亮度（底部）",
  L"#",
  L"@",
  L"Мин.абсолютная яркость(дно)"  };

static wchar_t* string_2398[NLANGS] = {
  L"Min abs brightn (sidewall)",
  L"最小绝对亮度（外观）",
  L"#",
  L"@",
  L"Мин.абсолютная яркость(жидкость)"  };

// FLIP OFF COLOR CONTROL STRINGS
static wchar_t* string_2400[NLANGS] = {
  L"Flip Off inspection",
  L"易开盖检测",
  L"Inspección presintos",
  L"@",
  L"Инспекция Flip-Off"  };

static wchar_t* string_2402[NLANGS] = {
  L"New reference L*a*b*=%.1lf,%.1lf,%.1lf RGB=%d,%d,%d",
  L"新参考值 %.1lf,%.1lf,%.1lf RGB=%d,%d,%d",
  L"Nueva referencia %.1lf,%.1lf,%.1lf RGB=%d,%d,%d",
  L"@",
  L"Новое значение L*a*b*=%.1lf,%.1lf,%.1lf RGB=%d,%d,%d "  };

static wchar_t* string_2404[NLANGS] = {
  L" Color REJECT (%.1lf,%.1lf,%.1lf) Dist=%.1lf",
  L" 颜色剔除 (%.0lf,%.0lf,%.0lf) 距离=%.1lf",
  L" RECHAZO de color (%.1lf,%.1lf,%.1lf) Dist=%.1lf",
  L"@",
  L"ОТБР.: цвет (%.1lf,%.1lf,%.1lf) Dist=%.1lf "  };

static wchar_t* string_2406[NLANGS] = {
  L" Color GOOD (%.1lf,%.1lf,%.1lf) Dist=%.1lf",
  L" 颜色合格 (%.0lf,%.0lf,%.0lf) 距离=%.1lf",
  L" Color CORRECTO (%.1lf,%.1lf,%.1lf) Dist.=%.1lf",
  L"@",
  L"ПРИГОДЕН: цвет (%.1lf,%.1lf,%.1lf) Dist=%.1lf "  };

static wchar_t* string_2408[NLANGS] = {
  L"Learn Color",
  L"学习颜色",
  L"Aprenda color",
  L"@",
  L"Обучение: цвет"  };

static wchar_t* string_2410[NLANGS] = {
  L"Max color distance (*)",
  L"最大颜色距离 (*)",
  L"Distancia máx color (*)",
  L"@",
  L"MAX расстояние цвет (*)"  };
  //to be translated 20180606
  static wchar_t* string_2410L[NLANGS] = {
  L"Color reference L",
  L"参考颜色L",
  L"Color de referencia L",
  L"@",
  L"Исходные цвета L"  };

  static wchar_t* string_2410a[NLANGS] = {
  L"Color reference a",
  L"参考颜色A",
  L"Color de referencia a",
  L"@",
  L"Исходные цвета a"  };

  static wchar_t* string_2410b[NLANGS] = {
  L"Color reference b",
  L"参考颜色B",
  L"Color de referencia b",
  L"@",
  L"Исходные цвета b"  };

static wchar_t* string_2412[NLANGS] = {
  L"Max spots error (*)",
  L"最大斑点 (*)",
  L"Error de puntos máx (*)",
  L"@",
  L"MAX ошибок точки (*) "  };

  static wchar_t* string_2412a[NLANGS] = {
  L"Max spots error 2 (*)",
  L"最大斑点 2 (*)",
  L"Error de puntos máx (*)",
  L"@",
  L"MAX ошибок точки (*) "  };

static wchar_t* string_2414[NLANGS] = {
  L"Spot color difference",
  L"斑点差异",
  L"Detectar Dif. color",
  L"@",
  L"Цвета точек различаются"  };

static wchar_t* string_2420[NLANGS] = {
  L"Spots REJECT %d/%d",
  L"斑点剔除 %d/%d",
  L"RECHAZO puntos %d/%d",
  L"@",
  L"ОТБР.: точки %d/%d "  };

static wchar_t* string_2422[NLANGS] = {
  L"Spots GOOD %d/%d",
  L"斑点合格 %d/%d",
  L"CORRECTOS puntos %d/%d",
  L"@",
  L"ПРИГОДЕН: точки %d/%d "  };

static wchar_t* string_2424[NLANGS] = {
  L"Spots DISABLED",
  L"斑点检测关闭",
  L"Puntos DESHABILI",
  L"@",
  L"Инспекция точек ОТКЛ"  };

  static wchar_t* string_2426[NLANGS] = {
  L" - Color DISABLED",
  L" - 颜色检测关闭",
  L" - Color DESHABILI",
  L"@",
  L"- Инспекция цвета ОТКЛ"  };

  static wchar_t* string_2430[NLANGS] = {
  L"Center found (%d,%d) err=%d - niter=%d",
  L"中心认定 (%d,%d) 错误值=%d - 距离=%d",
  L" - Centro Encontrado",
  L"@",
  L" Середина (%d,%d) погрешность=%d - расстояние=%d "  };

  static wchar_t* string_2432[NLANGS] = {
  L"Center NOT found (%d,%d) err=%d - niter=%d",
  L"没找到中心 (%d,%d) 错误值=%d - 距离=%d",
  L"Centro NO encontrado",
  L"@",
  L"Середина не найдена(%d,%d) погрешность=%d - расстояние=%d "  };



// NEW AREA SCAN SKINNING - TOPCAKE ANALYSIS


  static wchar_t* string_2452[NLANGS] = {
  L"Skinning",
  L"结皮",
  L"#",
  L"@",
  L"Поверхность"  };

  static wchar_t* string_2454[NLANGS] = {
  L"Alignment width",
  L"对齐宽度",
  L"Alineación Ancho",
  L"@",
  L"Выравнивание ширины"  };

  static wchar_t* string_2456[NLANGS] = {
  L"Alignment height",
  L"对齐高度",
  L"Alineación Altura",
  L"@",
  L"Выравнивание высоты"  };

  static wchar_t* string_2458[NLANGS] = {
  L"Outside-Inside gap",
  L"内外圈间隙",
  L"Exterior-Interior Diferencia",
  L"@",
  L"Зазор"  };

  static wchar_t* string_2460[NLANGS] = {
  L"Min alignment value",
  L"最小对齐分值",
  L"Min valor de alineación",
  L"@",
  L"MIN значение выравнивания"  };

  static wchar_t* string_2462[NLANGS] = {
  L"Analysis width",
  L"分析宽度",
  L"Análisis de grosor",
  L"@",
  L"Анализ ширины"  };

  static wchar_t* string_2464[NLANGS] = {
  L"Analysis height",
  L"分析高度",
  L"Análisis Alto",
  L"@",
  L"Анализ высоты"  };

  static wchar_t* string_2466[NLANGS] = {
  L"Vertical offset",
  L"垂直补偿",
  L"Compensación Vertical",
  L"@",
  L"Выртикальное смещение"  };

  static wchar_t* string_2468[NLANGS] = {
  L"Minimum brightness",
  L"最小亮度",
  L"Brillo mínimo",
  L"@",
  L"MIN яркость"  };

  static wchar_t* string_2478[NLANGS] = {
  L"Skinning REJECT error %d/%d",
  L"结皮剔除误差 %d/%d",
  L"# %d/%d",
  L"@ %d/%d",
  L"Ошибка ОТБР.: пов-ть ЛИО %d/%d"  }; 

  static wchar_t* string_2480[NLANGS] = {
  L"Skinning GOOD error %d/%d",
  L"结皮合格误差 %d/%d",
  L"# %d/%d",
  L"@ %d/%d",
  L"Ошибка определения пригодности %d/%d"  }; 

  static wchar_t* string_2482[NLANGS] = {
  L"Unable to align %d/%d",
  L"不能对齐 %d/%d",
  L"Imposible Alinear %d/%d",
  L"@ %d/%d",
  L"Выравнивание невозможно %d/%d"  };

  static wchar_t* string_2484[NLANGS] = {
  L"Aligned in %d,%d (%d/%d)",
  L"对齐中 %d,%d (%d/%d)",
  L"Alineado en %d,%d (%d/%d)",
  L"@ %d,%d (%d/%d)",
  L"Выравнивание %d,%d (%d/%d)"  };




// NEW DOUBLE CAP 


  static wchar_t* string_2502[NLANGS] = {
  L"Show BACKLIGHT cap",
  L"显示背光源下的瓶头",
  L"Mostrar tapa luz trasera ",
  L"@",
  L"Задний свет - колпачок"  };

  static wchar_t* string_2506[NLANGS] = {
  L"Exposure (top light)",
  L"曝光时间(顶光) ",
  L"Exponer(luz superior)",
  L"@",
  L"Экспозиция (верхный свет)"  };

  static wchar_t* string_2510[NLANGS] = {
  L"Backlight contour DISABLED",
  L"背光轮廓检测屏蔽",
  L"Contorno luz trasera DESHABILI",
  L"@",
  L"Инсп. контура с задним светом ОТКЛ"  };

    // NEW COSMETIC COMBO

  static wchar_t* string_2520[NLANGS] = {
  L"Bright. threshold for bottom",
  L"亮度，底部阈值",
  L"#",
  L"@",
  L"Яркость,порог дна"  };

  static wchar_t* string_2522[NLANGS] = {
  L"Bright. threshold for meniscus",
  L"亮度， 液位阈值",
  L"#",
  L"@",
  L"Яркость,порог уровня жидкости"  };

  static wchar_t* string_2524[NLANGS] = {
  L"Bright. threshold for top",
  L"亮度， 顶部阈值",
  L"#",
  L"@",
  L"Яркость,порог верха"  };

  static wchar_t* string_2530[NLANGS] = {
  L"Allowance above bottom",
  L"底部上面容差",
  L"#",
  L"@",
  L"Толерантность над дном"  };

  static wchar_t* string_2532[NLANGS] = {
  L"Allowance below meniscus",
  L"液位区域下面容差",
  L"#",
  L"@",
  L"Толерантность ниже уровня жидкости"  };

  static wchar_t* string_2534[NLANGS] = {
  L"Allowance above meniscus",
  L"液位上面容差",
  L"#",
  L"@",
  L"Толерантность выше уровня жидкости"  };

  static wchar_t* string_2536[NLANGS] = {
  L"Allowance below top",
  L"顶部下面容差",
  L"#",
  L"@",
  L"Толерантность ниже верха"  };



// NEW ORAL INSPECTION (LUNAN)

  static wchar_t* string_2550[NLANGS] = {
  L"Max circle distance",
  L"最大圆距离",
  L"Distanc. máx círculo",
  L"@",
  L"MAX расстояние круга"  };

  static wchar_t* string_2554[NLANGS] = {
  L"Max contour",
  L"最大轮廓",
  L"Error de perfil máx",
  L"@",
  L"MAX контур"  };

  //
  static wchar_t* string_2560[NLANGS] = {
  L"CANNOT FIND PROFILE",
  L"找不到轮廓",
  L"NO SE ENCUENTRA EL PERFIL",
  L"@",
  L"НЕ ОБНАРУЖЕНО: контур"  };

  static wchar_t* string_2562[NLANGS] = {
  L"Profile GOOD (%d/%d)",
  L"轮廓检测良好 (%d/%d)",
  L"Perfil CORRECTO (%d/%d)",
  L"@",
  L"ПРИГОДЕН: контур (%d/%d)"  };

  static wchar_t* string_2564[NLANGS] = {
  L"Profile REJECT (%d/%d)",
  L"轮廓检测剔除 (%d/%d)",
  L"RECHAZO perfil (%d/%d)",
  L"@",
  L"ОТБР.: контур"  };

  static wchar_t* string_2568[NLANGS] = {
  L"Contour NOT closed (%d pts)",
  L"轮廓未闭合 (%d pts)",
  L"Contador NO cerrado (%d pts)",
  L"@",
  L"Контур не замкнут (%d pts)"  };

  static wchar_t* string_2572[NLANGS] = {
  L"Top camera control DISABLED",
  L"顶部相机控制屏蔽",
  L"Control de cámara sup. DESHABILI",
  L"@",
  L"Верхняя камера ОТКЛ"  };



// DOT RING COLOR CONTROL STRINGS
static wchar_t* string_2600[NLANGS] = {
  L"Dot-ring parameters",
  L"色点色环参数",
  L"",
  L"@",
  L""  };

static wchar_t* string_drrej[NLANGS] = {
  L"DR rejects",
  L"色点色环剔除",
  L"#",
  L"@",
  L"&"  };


// POWDER ON STOPPER ANALYSIS STRINGS
static wchar_t* string_2640[NLANGS] = {
  L"Powder on stopper",
  L"胶塞带药",
  L"Polvo en Tapón",
  L"@",
  L"Лиофилизат на пробке"  };

static wchar_t* string_2642[NLANGS] = {
  L"GOOD: points %d/%d",
  L"合格: 分数 %d/%d",
  L"CORRECTO: Puntos %d/%d",
  L"@ %d/%d",
  L"ПРИГОДЕН: точки s %d/%d "  };

static wchar_t* string_2644[NLANGS] = {
  L"REJECT: too many points %d/%d",
  L"剔除: 点数太多 %d/%d",
  L"RECHAZO: Demasiados Puntos %d/%d",
  L"@ %d/%d",
  L"ОТБР.:  много точек %d/%d"  };




// CRIMPING ANALYSIS STRINGS
static wchar_t* string_2650[NLANGS] = {
  L"Crimping inspection",
  L"瓶肩检测",
  L"Inspec. Ondulacion",
  L"@",
  L"Инспекция закатки"  };

  static wchar_t* string_2650a[NLANGS] = {
  L"Neck crack inspection",
  L"瓶肩裂纹检测",
  L"Inspec. Ondulacion",
  L"@",
  L"Инспекция закатки"  };

static wchar_t* string_2660[NLANGS] = {
  L"REJECT: too few good points %d/%d",
  L"剔除: 太少合格点",
  L"RECHAZO:pocos puntos buenos",
  L"@",
  L"ОТБР.:  мало точек"  };

static wchar_t* string_2662[NLANGS] = {
  L"GOOD: enough good points %d/%d",
  L"合格: 足够的合格点数",
  L"BIEN: Suficientes puntos buenos",
  L"@",
  L"ПРИГОДЕН: достаточно точек"  };

static wchar_t* string_2670[NLANGS] = {
  L"Analysis area width",
  L"分析区域宽度",
  L"Analisis ancho area",
  L"@",
  L"Анализ ширины"  };

static wchar_t* string_2672[NLANGS] = {
  L"Analysis area height",
  L"分析区域高度",
  L"Analisis alto area",
  L"@",
  L"Анализ высоты"  };

static wchar_t* string_2674[NLANGS] = {
  L"Analysis area vert offs",
  L"分析区域竖直补偿",
  L"Analisis compens. vert.",
  L"@",
  L"Анализ вертикального смещения"  };


static wchar_t* string_2680[NLANGS] = {
  L"Min dark edge score",
  L"最小暗色边缘分值",
  L"minimo borde oscuro",
  L"@",
  L"MIN значение темного контура"  };

static wchar_t* string_2682[NLANGS] = {
  L"Min dark edge width",
  L"最小暗色边缘宽度",
  L"ancho min borde oscuro",
  L"@",
  L"MIN ширина темного контура"  };

static wchar_t* string_2684[NLANGS] = {
  L"Max dark edge width",
  L"最大暗色边缘宽度",
  L"Ancho max. borde oscuro",
  L"@",
  L"MAX ширина темного контура"  };

static wchar_t* string_2686[NLANGS] = {
  L"Min number of good points",
  L"最少合格点数值",
  L"Num. Min. Ptos buenos",
  L"@",
  L"MIN кол-во квалиф. точек "  };

static wchar_t* string_2688[NLANGS] = {
  L"Min edge side value (*)",
  L"最小边缘范围内的值(*)",
  L"valor min. borde(*)",
  L"@",
  L"MIN значение границы контура (*)"  };

static wchar_t* string_2692[NLANGS] = {
  L"Min dark edge height",
  L"最小暗边高度",
  L"altura min. borde oscuro",
  L"@",
  L"MIN высота темного контура"  };

static wchar_t* string_2694[NLANGS] = {
  L"Max dark edge height",
  L"最大暗边高度",
  L"Altura max. borde oscuro",
  L"@",
  L"MAX высота темного контура"  };



// LIQUID COLOR CONTROL STRINGS
static wchar_t* string_2700[NLANGS] = {
  L"Liquid color inspection",
  L"色水检测",
  L"&",
  L"@",
  L"#"  };

static wchar_t* string_2704[NLANGS] = {
  L"Maximum allowed redness",
  L"最大允许红色值",
  L"&",
  L"@",
  L"#"  };

static wchar_t* string_2706[NLANGS] = {
  L"REJECT too much redness %d/%d",
  L"红色值 %d/%d 太高剔除",
  L"& %d/%d",
  L"@ %d/%d",
  L"# %d/%d"  };

static wchar_t* string_2708[NLANGS] = {
  L"GOOD redness OK %d/%d",
  L"红色值 %d/%d 合格",
  L"& %d/%d",
  L"@ %d/%d",
  L"# %d/%d"  };




// BOTTOM INSPECTION'S STRINGS


static wchar_t* string_2740[NLANGS] = {
  L"Analysis DISABLED",
  L"分析未激活",
  L"Analísis DESHABILI",
  L"@",
  L"Инспекция ОТКЛ"  };

static wchar_t* string_2742[NLANGS] = {
  L"Out of inspection area",
  L"超出检测区域",
  L"Fuera de zona de inspección",
  L"@",
  L"За пределами зоны инспекции"  };

static wchar_t* string_2744[NLANGS] = {
  L"Cake inspection",
  L"粉饼检测",
  L"Inspección pastel",
  L"@",
  L"Инспекция ЛИО"  };

static wchar_t* string_2746[NLANGS] = {
  L"Alignment DISABLED",
  L"对齐未激活",
  L"Alineación DESHABILI",
  L"@",
  L"Выравнивание ОТКЛ"  };


static wchar_t* string_2752[NLANGS] = {
  L"Center found (%d,%d)",
  L"找到中心 (%d,%d)",
  L"Centro encontrado (%d,%d)",
  L"@",
  L"Середина обнаружена (%d,%d)"  };

static wchar_t* string_2753[NLANGS] = {
  L" - Center2 found (%d,%d)",
  L" - 找到中心 (%d,%d)",
  L" - Centro2 encontrado (%d,%d)",
  L"@",
  L"- Середина 2 обнаружена (%d,%d)"  };


static wchar_t* string_2754[NLANGS] = {
  L"Center NOT found (%d,%d,%d,%d)",
  L"未找到中心 (%d,%d,%d,%d)",
  L"Centro NO encontrado (%d,%d,%d,%d)",
  L"@",
  L"Середина НЕ ОБНАРУЖЕНА (%d,%d,%d,%d)"  };

static wchar_t* string_2755[NLANGS] = {
  L" - Center2 NOT found (%d,%d,%d,%d)",
  L" - 未找到中心 (%d,%d,%d,%d)",
  L" - Centro2 NO encontrado (%d,%d,%d,%d)",
  L"@",
  L"- Середина 2 НЕ ОБНАРУЖЕНА (%d,%d,%d,%d)"  };


static wchar_t* string_2756[NLANGS] = {
  L"Max average brightness",
  L"最大平均亮度",
  L"Brillo promedio máx",
  L"@",
  L"MAX средняя яркость"  };

  //Pour added for double thershold 20180926
  static wchar_t* string_2756b[NLANGS] = {
  L"Max average brightness 2",
  L"最大平均亮度2",
  L"Brillo promedio máx",
  L"@",
  L"MAX средняя яркость"  };

static wchar_t* string_2758[NLANGS] = {
  L"Min average brightness",
  L"最小平均亮度",
  L"Brillo promedio mín",
  L"@",
  L"MIN средняя яркость"  };

  //Pour added for double thershold 20180926
  static wchar_t* string_2758b[NLANGS] = {
  L"Min average brightness 2",
  L"最小平均亮度2",
  L"Brillo promedio mín",
  L"@",
  L"MIN средняя яркость"  };

static wchar_t* string_2760[NLANGS] = {
  L"Bottom inspection",
  L"底部检测",
  L"Inspección de fondo",
  L"@",
  L"Инспекция дна"  };

static wchar_t* string_2762[NLANGS] = {
  L"Max positive diff",
  L"最大正差",
  L"Dif. positiva máx",
  L"@",
  L"MAX положительный дифференциал"  };

  static wchar_t* string_2762c[NLANGS] = {
  L"Max positive diff 1",
  L"最大正差（上）",
  L"Dif. positiva máx",
  L"@",
  L"MAX положительный дифференциал"  };


static wchar_t* string_2764[NLANGS] = {
  L"Max negative diff",
  L"最大负差",
  L"Dif. negativa máx",
  L"@",
  L"MAX отрицательный дифференциал"  };

  static wchar_t* string_2764c[NLANGS] = {
  L"Max negative diff 1",
  L"最大负差（上）",
  L"Dif. negativa máx",
  L"@",
  L"MAX отрицательный дифференциал"  };

      //Pour added for testing 20180111
  static wchar_t* string_2762a[NLANGS] = {
  L"Color detection threshold 1",
  L"坏药检测阈值一",
  L"Dif. positiva máx",
  L"@",
  L"MAX положительный дифференциал"  };

static wchar_t* string_2764a[NLANGS] = {
  L"Color detection threshold 2",
  L"坏药检测阈值二",
  L"Dif. negativa máx",
  L"@",
  L"MAX отрицательный дифференциал"  };
  //Pour added for testing 20180111

static wchar_t* string_2766[NLANGS] = {
  L"Alignment radius",
  L"搜寻边缘",
  L"Alineación de radio",
  L"@",
  L"Радиус выравнивания"  };

static wchar_t* string_2768[NLANGS] = {
  L"Found threshold",
  L"找寻阈值",
  L"Umbral encontrado",
  L"@",
  L"Найдено пороговое значение"  };

static wchar_t* string_2770[NLANGS] = {
  L"Analysis radius",
  L"分析区域",
  L"Analísis de radio",
  L"@",  
  L"Радиус анализа",  };

static wchar_t* string_2772[NLANGS] = {
  L"Max positive differential",
  L"最大正差",
  L"Error máx positivo",
  L"@",
  L"MAX положительный дифференциал"  };

  //Pour added for 2 threshold 20180926
  static wchar_t* string_2772b[NLANGS] = {
  L"Max positive differential 2",
  L"最大正差2",
  L"Error máx positivo",
  L"@",
  L"MAX положительный дифференциал"  };

static wchar_t* string_2774[NLANGS] = {
  L"Max negative differential",
  L"最大负差",
  L"Error máx negativo",
  L"@",
  L"MAX отрицательный дифференциал"  };

  //Pour added for 2 tereshold 20180926
  static wchar_t* string_2774b[NLANGS] = {
  L"Max negative differential 2",
  L"最大负差2",
  L"Error máx negativo",
  L"@",
  L"MAX отрицательный дифференциал"  };


static wchar_t* string_2776[NLANGS] = {
  L"Max absolute lum",
  L"最大绝对值亮度",
  L"Lum absoluta máx",
  L"@",
  L"MAX  абсолютная яркость"  };

  static wchar_t* string_2776c[NLANGS] = {
  L"Max absolute lum 1",
  L"最大绝对值亮度（上）",
  L"Lum absoluta máx",
  L"@",
  L"MAX  абсолютная яркость"  };

static wchar_t* string_2778[NLANGS] = {
  L"Min absolute lum",
  L"最小绝对值亮度",
  L"Lum absoluta mín",
  L"@",
  L"MIN  абсолютной яркости"  };

  static wchar_t* string_2778c[NLANGS] = {
  L"Min absolute lum 1",
  L"最小绝对值亮度（上）",
  L"Lum absoluta mín",
  L"@",
  L"MIN  абсолютной яркости"  };

static wchar_t* string_2780[NLANGS] = {
  L"Max HIGH absolute",
  L"最大高绝对值错误",
  L"Absoluto ALTO máx",
  L"@",
  L"Высокий абсолютный максимум"  };

  static wchar_t* string_2780c[NLANGS] = {
  L"Max HIGH absolute 1",
  L"最大高绝对值错误（上）",
  L"Absoluto ALTO máx",
  L"@",
  L"Высокий абсолютный максимум"  };

  //Pour added for 2 thereshold 20180926
  static wchar_t* string_2780b[NLANGS] = {
  L"Max HIGH absolute 2",
  L"最大高绝对值错误 2",
  L"Absoluto ALTO máx",
  L"@",
  L"Высокий абсолютный максимум"  };

static wchar_t* string_2782[NLANGS] = {
  L"Max LOW absolute",
  L"最大低绝对",
  L"Absoluto BAJO máx",
  L"@",
  L"Низкий абсолютный максимум"  };

  //Pour added for 2 threshlod 20180926
  static wchar_t* string_2782b[NLANGS] = {
  L"Max LOW absolute 2",
  L"最大低绝对 2",
  L"Absoluto BAJO máx",
  L"@",
  L"Низкий абсолютный максимум"  };

static wchar_t* string_2784[NLANGS] = {
  L" - Brght TOO LOW %d (%d,%d)",
  L" - 亮度太低 %d (%d,%d)",
  L" - Brillo MUY BAJO %d (%d,%d)",
  L"@",
  L"Низкая яркость %d (%d,%d)"  };

static wchar_t* string_2786[NLANGS] = {
  L" - Brght TOO HIGH %d (%d,%d)",
  L" - 亮度太高 %d (%d,%d)",
  L" - Brillo MUY ALTO %d (%d,%d)",
  L"@",
  L" - Высокая яркость %d (%d,%d)"  };

static wchar_t* string_2788[NLANGS] = {
  L" - Brght OK %d (%d,%d)",
  L" - 亮度 %d (%d,%d)",
  L" - Brillo OK %d (%d,%d)",
  L"@",
  L" – Яркость ОК %d (%d,%d)"  };

static wchar_t* string_2790[NLANGS] = {
  L"REJECT pos diff - %d %d %d %d",
  L"正差剔除 - %d %d %d %d",
  L"RECHAZO dif. posit. - %d %d %d %d",
  L"@",
  L"ОТБР.: полож.  дифференциал- %d %d %d %d "  };

      //Pour added for color difference detection 20180112
  static wchar_t* string_2790a[NLANGS] = {
  L"REJECT color diff - %d %d %d %d",
  L"坏药剔除 - %d %d %d %d",
  L"RECHAZO dif. posit. - %d %d %d %d",
  L"@",
  L"ОТБР.: полож.  дифференциал- %d %d %d %d "  };
  //Pour added for color difference detection 20180112


static wchar_t* string_2792[NLANGS] = {
  L"REJECT neg diff error - %d %d %d %d",
  L"负差剔除 - %d %d %d %d",
  L"RECHAZO error dif.  - %d %d %d %d",
  L"@",
  L"ОТБР.: ошибка - %d %d %d %d "  };

static wchar_t* string_2794[NLANGS] = {
  L"REJECT abs max - %d %d %d %d",
  L"最大绝对值剔除 - %d %d %d %d",
  L"RECHAZO abs máx - %d %d %d %d",
  L"@",
  L"ОТБР.: абсолютный максимум - %d %d %d %d "  };

static wchar_t* string_2796[NLANGS] = {
  L"REJECT abs min - %d %d %d %d",
  L"绝对最小剔除 - %d %d %d %d",
  L"RECHAZO abs mín - %d %d %d %d",
  L"@",
  L"ОТБР.: абсолютный минимум - %d %d %d %d "  };

static wchar_t* string_2798[NLANGS] = {
  L"GOOD %d %d %d %d",
  L"合格 %d %d %d %d",
  L"CORRECTO %d %d %d %d",
  L"@",
  L"ПРИГОДЕН %d %d %d %d "  };

static wchar_t* string_2800[NLANGS] = {
  L"Alignment error",
  L"对齐错误",
  L"Error alineación",
  L"@",
  L"Ошибка выравнивания"  };


static wchar_t* string_2810[NLANGS] = {
  L"Cracks inspection",
  L"破瓶检测",
  L"Inspección grietas",
  L"@",
  L"Инспекция на трещины"  };

static wchar_t* string_2812[NLANGS] = {
  L"Inner radius",
  L"内部分析区域",
  L"Radio interior",
  L"@",
  L"Внутренний радиус"  };

static wchar_t* string_2814[NLANGS] = {
  L"Outer radius",
  L"外部分析区域",
  L"Radio exterior",
  L"@",
  L"Внешний радиус"  };
  

static wchar_t* string_2850[NLANGS] = {
  L"Top-cap inspection",
  L"盖子顶部检测",
  L"Inspección tapa-super",
  L"@",
  L"Инспекция верхней части колпачка"  };
  
static wchar_t* string_2852[NLANGS] = {
  L"Extern inspection",
  L"外部检测",
  L"Inspección externa",
  L"@",
  L"Внешняя инспекция"  };
  

static wchar_t* string_2860[NLANGS] = {
  L"Inner inspection",
  L"内部检测",
  L"Inspección interior",
  L"@",
  L"Внутренняя инспекция"  };
  
static wchar_t* string_2870[NLANGS] = {
  L"Inner: %s",
  L"内部: %s",
  L"Interior: %s",
  L"@",
  L"Внутренняя: %s "  };
  
static wchar_t* string_2872[NLANGS] = {
  L"%s - Outer: %s",
  L"%s - 外部: %s",
  L"%s - Exterior: %s",
  L"@",
  L"%s - Внешняя: %s "  };
  

//  - - - - NEW CAP V30 - - - - 

static wchar_t* string_2910[NLANGS] = {
  L"PROFILE",
  L"轮廓",
  L"Perfil",
  L"@",
  L"Контур"  };

static wchar_t* string_2912[NLANGS] = {
  L"FlipOff side",
  L"易开盖侧面",
  L"Lado de Precinto",
  L"@",
  L"Бок  Flip-off"  };

static wchar_t* string_2914[NLANGS] = {
  L"Aluminium side",
  L"铝盖侧面",
  L"Lado de Aluminio",
  L"@",
  L"Боковая сторона колпачка"  };

static wchar_t* string_2916[NLANGS] = {
  L"Aluminium Crimping",
  L"扎盖",
  L"Engaste de Aluminio",
  L"@",
  L"Обжим колпачка"  };

static wchar_t* string_2918[NLANGS] = {
  L"Rubber Stopper",
  L"胶塞",
  L"Tapón de Goma",
  L"@",
  L"Пробка"  };

static wchar_t* string_2920[NLANGS] = {
  L"DISABLED",
  L"未使能",
  L"DESHABILITADO",
  L"@",
  L"ОТКЛ"  };

static wchar_t* string_2922[NLANGS] = {
  L"GOOD %d/%d",
  L"合格 %d/%d",
  L"BIEN",
  L"@",
  L"ПРИГОДЕН %d/%d "  };

static wchar_t* string_2923[NLANGS] = {
  L"REJECT %d/%d",
  L"剔除 %d/%d",
  L"RECHAZO",
  L"@",
  L"ОТБР.%d/%d "  };

static wchar_t* string_2925[NLANGS] = {
  L"Bad coords",
  L"错误坐标",
  L"Coordenadas Incorrectas",
  L"@",
  L"Неправильные коодинаты"  };

static wchar_t* string_2927[NLANGS] = {
  L"F:%s A:%s C:%s S:%s",
  L"F:%s A:%s C:%s S:%s",
  L"%s %s %s %s",
  L"%s %s %s %s",
  L" F:%s A:%s C:%s S:%s "  };









// General STRINGS

static wchar_t* string_2930[NLANGS] = {
  L"Vertical offset",
  L"竖直补偿",
  L"Compensación vertical",
  L"@",
  L"Вертикальное смещение"  };

static wchar_t* string_2932[NLANGS] = {
  L"Analysis width",
  L"分析宽度",
  L"Análisis de grosor",
  L"@",
  L"Анализ ширины"  };

static wchar_t* string_2934[NLANGS] = {
  L"Analysis height",
  L"分析高度",
  L"Análisis de peso",
  L"@",
  L"Анализ высоты"  };

  //Pour 20200321
  static wchar_t* string_2935[NLANGS] = {
  L"Crack brightness",
  L"裂纹灰度",
  L"Análisis de peso",
  L"@",
  L"Анализ высоты"  };
  //Pour 20200321

static wchar_t* string_2936[NLANGS] = {
  L"Change threshold",
  L"差值门槛",
  L"Cambio de umbral",
  L"@",
  L"Изменение порога"  };

    //Pour 20200321
  static wchar_t* string_2937[NLANGS] = {
  L"Blob Area",
  L"BLOB区域",
  L"Análisis de peso",
  L"@",
  L"Анализ высоты"  };
  //Pour 20200321

  //Pour 20200321
  static wchar_t* string_2939[NLANGS] = {
  L"Blob Ratio",
  L"BLOB比例",
  L"Análisis de peso",
  L"@",
  L"Анализ высоты"  };
  //Pour 20200321

static wchar_t* string_2940[NLANGS] = {
  L"Maximum brightness",
  L"最高亮度",
  L"Brillo Máx.",
  L"@",
  L"MAX яркость"  };

    //Pour 20200321
  static wchar_t* string_2941[NLANGS] = {
  L"Blob Width",
  L"BLOB宽度",
  L"#",
  L"@",
  L"Анализ высоты"  };
  //Pour 20200321

static wchar_t* string_2942[NLANGS] = {
  L"Minimum brightness",
  L"最低亮度",
  L"Brillo mín.",
  L"@",
  L"MIN яркость"  };

      //Pour 20200321
  static wchar_t* string_2943[NLANGS] = {
  L"Crack Cross Area(*)",
  L"裂纹面积(*)",
  L"#",
  L"@",
  L"Анализ высоты"  };
  //Pour 20200321

static wchar_t* string_2944[NLANGS] = {
  L"Analysis lenght",
  L"分析长度",
  L"Análisis de longitud",
  L"@",
  L"Анализ длины"  };

static wchar_t* string_2946[NLANGS] = {
  L"Maximum error (*)",
  L"最大错误值 (*)",
  L"Error máx (*)",
  L"@",
  L"MAX погрешность"  };

  //Pour added 20180927
  static wchar_t* string_2946b[NLANGS] = {
  L"Maximum error 2(*)",
  L"最大错误值2 (*)",
  L"Error máx (*)",
  L"@",
  L"MAX погрешность"  };


static wchar_t* string_2950[NLANGS] = {
  L"Threshold low",
  L"阈值低",
  L"Límite Inferior",
  L"@",
  L"Порог низок"  };

static wchar_t* string_2952[NLANGS] = {
  L"Threshold high",
  L"阈值高",
  L"Límite Superior",
  L"@",
  L"Порог высоток"  };


// STRINGS RELATED TO DOT&RING COLOR ANALYSIS

static wchar_t* string_3020[NLANGS] = {
  L"Hue maximum",
  L"色度最大值",
  L"Tinte max.",
  L"@",
  L"MAX оттенок"  };

static wchar_t* string_3022[NLANGS] = {
  L"Hue minimum",
  L"色度最小值",
  L"Tinte min.",
  L"@",
  L"MIN оттенок"  };

static wchar_t* string_3024[NLANGS] = {
  L"Saturation maximum",
  L"饱和度最大值",
  L"Saturacion max.",
  L"@",
  L"MAX насыщенность"  };

static wchar_t* string_3026[NLANGS] = {
  L"Saturation minimum",
  L"饱和度最小值",
  L"Saturacion min.",
  L"@",
  L"MIN насыщенность"  };

static wchar_t* string_3028[NLANGS] = {
  L"Value maximum",
  L"纯度最大值",
  L"Valor max.",
  L"@",
  L"MAX прозрачность"  };

static wchar_t* string_3030[NLANGS] = {
  L"Value minimum",
  L"纯度最小值",
  L"valor min.",
  L"@",
  L"MIN прозрачность"  };

static wchar_t* string_3032[NLANGS] = {
  L"Minimum numb of points (*)",
  L"最少点数 (*)",
  L"num. min. de puntos (*)",
  L"@",
  L"MIN кол-во точек (*)"  };


// WAIST RELATED STRINGS


static wchar_t* string_3110[NLANGS] = {
  L"Waist control",
  L"收腰控制",
  L"@",
  L"@",
  L"Контроль горловины"  };

static wchar_t* string_3114[NLANGS] = {
  L"Bottom brigtn thrs",
  L"底部亮度",
  L"@",
  L"@",
  L"Яркость дна"  };

static wchar_t* string_3120[NLANGS] = {
  L"Bottom not found",
  L"未发现底部",
  L"@",
  L"@",
  L"Не обнаружено дно"  };

static wchar_t* string_3130[NLANGS] = {
  L"Sides detect brightness",
  L"侧面检测亮度",
  L"@",
  L"@",
  L"Яркость боковая"  };

static wchar_t* string_3140[NLANGS] = {
  L"Max analysis height",
  L"最大分析高度",
  L"@",
  L"@",
  L"Анализ MAX высоты"  };

static wchar_t* string_3142[NLANGS] = {
  L"Min analysis height",
  L"最小分析高度",
  L"@",
  L"@",
  L"Анализ MIN высоты"  };

static wchar_t* string_3146[NLANGS] = {
  L"Max allowed width",
  L"最大允许宽度",
  L"@",
  L"@",
  L"MAX допустимая ширина"  };

static wchar_t* string_3148[NLANGS] = {
  L"Min allowed width",
  L"最小允许宽度",
  L"@",
  L"@",
  L"MIN допустимая ширина"  };

static wchar_t* string_3150[NLANGS] = {
  L"Min continuous height",
  L"最小连续高度",
  L"@",
  L"@",
  L"MIN непрерывная высота "  };


static wchar_t* string_3160[NLANGS] = {
  L"Layered control",
  L"分层控制",
  L"@",
  L"@",
  L"Контроль уровня"  };

static wchar_t* string_3170[NLANGS] = {
  L"Analysis width",
  L"分析宽度",
  L"@",
  L"@",
  L"Анализ ширины"  };

static wchar_t* string_3172[NLANGS] = {
  L"Minimum layered brightness",
  L"最小分层亮度",
  L"@",
  L"@",
  L"Минимальная яркость слоя"  };


static wchar_t* string_3178[NLANGS] = {
  L"Maximum error (*)",
  L"最大错误(*)",
  L"@ (*)",
  L"@",
  L"MAX погрешность (*)"  };


static wchar_t* string_3180[NLANGS] = {
  L"Waist shrink GOOD (%d/%d)",
  L"收腰收缩良好 (%d/%d)",
  L"@ (%d/%d)",
  L"@ (%d/%d)",
  L"ПРИГОДЕН: сжатие ЛИО (%d/%d)"  };

static wchar_t* string_3182[NLANGS] = {
  L"Waist shrink REJECT (%d/%d)",
  L" 收腰收缩剔除(%d/%d)",
  L"@ (%d/%d)",
  L"@ (%d/%d)",
  L"ОТБР.: сжатие ЛИО(%d/%d)"  };


static wchar_t* string_3184[NLANGS] = {
  L"Layered cake GOOD (%d/%d)",
  L"分层粉饼合格(%d/%d)",
  L"@ (%d/%d)",
  L"@ (%d/%d)",
  L"ПРИГОДЕН: слои ЛИО (%d/%d)"  };

static wchar_t* string_3186[NLANGS] = {
  L"Layered cake REJECT (%d/%d)",
  L"分层粉饼剔除(%d/%d)",
  L"@ (%d/%d)",
  L"@ (%d/%d)",
  L"ОТБР.: слои ЛИО (%d/%d)"  };

static wchar_t* string_3188[NLANGS] = {
  L"REJECT because of bad alignment (%d/%d)",
  L"排列不齐剔除 (%d/%d)",
  L"@ (%d/%d)",
  L"@ (%d/%d)",
  L"ОТБР.: смещение (%d/%d)"  };



// STRINGS RELATED TO BULGE SHRINK ANALYSIS


static wchar_t* string_3208[NLANGS] = {
  L"BULGE SHRINK",
  L"泡头萎缩",
  L"Reducir Abultamiento",
  L"@",
  L"Выпуклости/Усадка"  };

static wchar_t* string_3210[NLANGS] = {
  L"Blg shrink REJECT %d/%d",
  L"泡头萎缩剔除 %d/%d",
  L"Reducción Abultamiento RECHAZO %d/%d",
  L"@ %d/%d",
  L"ОТБР.: выпуклости/усадка %d/%d"  };

static wchar_t* string_3212[NLANGS] = {
  L"Blg shrink GOOD %d/%d",
  L"泡头萎缩合格 %d/%d",
  L"Reducción Abultamiento CORRECTO %d/%d",
  L"@ %d/%d",
  L"ПРИГОДЕН: выпуклости/усадка %d/%d"  };

static wchar_t* string_3214[NLANGS] = {
  L"Blg shrink disabled",
  L"泡头萎缩未使能",
  L"Reducción Abultamiento Deshabilitado",
  L"@",
  L"Выпуклости/усадка ОТКЛ"  };

static wchar_t* string_3216[NLANGS] = {
  L"Shrink check height",
  L"萎缩检测高度",
  L"Revisar Altura Reducción",
  L"@",
  L"Инспекция высоты усадки"  };

static wchar_t* string_3218[NLANGS] = {
  L"Max shrink variation",
  L"最大萎缩范围",
  L"Variación Máximo Reducción",
  L"@",
  L"MAX вариация усадки"  };


// STRINGS RELATED TO INNER BURNS ANALYSIS


static wchar_t* string_3238[NLANGS] = {
  L"BURNS",
  L"焦头",
  L"Quemaduras",
  L"@",
  L"Пригар"  };

static wchar_t* string_3240[NLANGS] = {
  L"Burns REJECT %d/%d",
  L"焦头剔除 %d/%d",
  L"Quemaduras RECHAZADAS %d/%d",
  L"@ %d/%d",
  L"ОТБР.: пригар %d/%d"  };

static wchar_t* string_3242[NLANGS] = {
  L"Burns GOOD %d/%d",
  L"焦头合格 %d/%d",
  L"Quemaduras CORRECTAS %d/%d",
  L"@ %d/%d",
  L"ПРИГОДЕН: пригар %d/%d"  };

static wchar_t* string_3244[NLANGS] = {
  L"Burns disabled",
  L"焦头未使能",
  L"Quemaduras DESHABILITAR",
  L"@",  
  L"Пригар ОТКЛ",  };

static wchar_t* string_3248[NLANGS] = {
  L"Edge min distance",
  L"边缘最小距离",
  L"Distancia min borde",
  L"@",
  L"MIN расстояние до края"  };

static wchar_t* string_3250[NLANGS] = {
  L"Edge min brightness",
  L"边缘最低亮度",
  L"Brillo min borde",
  L"@",
  L"MIN яркость края"  };

static wchar_t* string_3252[NLANGS] = {
  L"Min filter value",
  L"最小过滤值",
  L"valor min filtro",
  L"@",
  L"MIN значение фильтра"  };


static wchar_t* string_3262[NLANGS] = {
  L"Height disabled",
  L"高度未激活",
  L"Altura desactivada",
  L"@",
  L"Высота ОТКЛ"  };


// CURVATURE STRINGS

static wchar_t* string_3280[NLANGS] = {
  L"CURVATURE",
  L"曲线",
  L"Curvatura",
  L"@",
  L"Кривизна"  };

static wchar_t* string_3282[NLANGS] = {
  L"Curv REJECT %d/%d",
  L"曲线不合格 %d/%d",
  L"RECHAZO de curva %d/%d",
  L"@",
  L"ОТБР.: Кривизна %d/%d "  };

static wchar_t* string_3284[NLANGS] = {
  L"Curv GOOD %d/%d",
  L"曲线检测合格 %d/%d",
  L"Curva CORRECTA %d/%d",
  L"@",
  L"ПРИГОДЕН: Кривизна %d/%d "  };

static wchar_t* string_3286[NLANGS] = {
  L"Curv DISABLED",
  L"曲度检测关闭",
  L"Curva DESHABILITADA",
  L"@",
  L"Кривизна ОТКЛ"  };

static wchar_t* string_3288[NLANGS] = {
  L"Reference width",
  L"参考宽度",
  L"Ancho de referencia",
  L"@",
  L"Ширина"  };

static wchar_t* string_3290[NLANGS] = {
  L"Reference height",
  L"参考高度",
  L"Altura de referencia",
  L"@",
  L"Высота"  };


// C40 RELATED STIRNGS

  static wchar_t* string_3310[NLANGS] = {
  L"Freeze dried mode",
  L"冻干模式",
  L"#",
  L"@",
  L"Режим жидкого продукты"  };

  static wchar_t* string_3312[NLANGS] = {
  L"Liquid mode",
  L"水针模式",
  L"#",
  L"@",
  L"Режим ЛИО.продукты"  };

  static wchar_t* string_3320[NLANGS] = {
  L"AFTER CHANGING MODE IT IS REQUIRED TO EXECUTE ZERO SEARCH ON THE MACHINE!",
  L"机器更改模式后需要归零",
  L"#",
  L"@",
  L"ПОСЛЕ ИЗМЕНЕНИЯ РЕЖИМА РАБОТЫ,НЕОБХОДИМО ОБНУЛИТЬ ТЕХНОЛОГИЧЕСКИЙ ШАГ НА МАШИНЕ!"  };




// HV CONTROL RELATED STIRNGS

  static wchar_t* string_3504[NLANGS] = {
  L"Enable HV control",
  L"使能高压放电控制",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_3506[NLANGS] = {
  L"Enable HV1 loop",
  L"使能高压放电单工位1",
  L"#",
  L"@",
  L"&"  };

  //Pour added 20180301
  static wchar_t* string_3506b[NLANGS] = {
  L"Enable HV2 loop",
  L"使能高压放电单工位2",
  L"#",
  L"@",
  L"&"  };

    static wchar_t* string_3506c[NLANGS] = {
  L"Enable HV3 loop",
  L"使能高压放电单工位3",
  L"#",
  L"@",
  L"&"  };

    static wchar_t* string_3506d[NLANGS] = {
  L"Enable HV4 loop",
  L"使能高压放电单工位4",
  L"#",
  L"@",
  L"&"  };


  static wchar_t* string_3510[NLANGS] = {
  L"HV transport zero position",
  L"高压放电运输原点位置",
  L"#",
  L"@",
  L"&"  };
  //Pour added 20180127
    static wchar_t* string_3510a[NLANGS] = {
  L"HV sorting zero position",
  L"高压放电星轮原点位置",
  L"#",
  L"@",
  L"&"  };
  //Pour added 20180127
  
  //Pour added "INSP" and "frm" on 20190410
  /*
  static wchar_t* string_3514[NLANGS] = {
  L"%d insp %d/%d/%d frm %d/%dms %.1lffps",
  L"%d insp %d/%d/%d %d/%dms %.1lffps",
  L"%d %d/%d/%d %d/%dms %.1lffps",
  L"%d %d/%d/%d %d/%dms %.1lffps",
  L"%d %d/%d/%d %d/%dms %.1lffps" };
  */
    static wchar_t* string_3514[NLANGS] = {
  L"%d %d/%d/%d %d/%dms %.1lffps",
  L"%d %d/%d/%d %d/%dms %.1lffps",
  L"%d %d/%d/%d %d/%dms %.1lffps",
  L"%d %d/%d/%d %d/%dms %.1lffps",
  L"%d %d/%d/%d %d/%dms %.1lffps" };

  static wchar_t* string_3514a[NLANGS] = {
  L"%d %d %d/%d/%d %d/%dms %.1lffps",
  L"%d %d %d/%d/%d %d/%dms %.1lffps",
  L"%d %d %d/%d/%d %d/%dms %.1lffps",
  L"%d %d %d/%d/%d %d/%dms %.1lffps",
  L"%d %d %d/%d/%d %d/%dms %.1lffps" };
  
  static wchar_t* string_3516[NLANGS] = {
  L" Inp ph (%d) @3=%.2lf @2=%.2lf",
  L" 进料相位 (%d) @3=%.2lf @2=%.2lf",
  L" Fase ent (%d) @3=%.2lf @2=%.2lf",
  L"@ (%d) @3=%.2lf @2=%.2lf",
  L"Фаза загрузки (%d) @3=%.2lf @2=%.2lf"  };


  static wchar_t* string_3520[NLANGS] = {
  L"HV CONTROL PARAMETERS",
  L"高压放电控制参数",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_3522[NLANGS] = {
  L"HV spinning speed",
  L"高压工位旋转速度",
  L"#",
  L"@",
  L"&"  };


  static wchar_t* string_3530[NLANGS] = {
  L"Read start phase (A)",
  L"读取开始相位",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_3532[NLANGS] = {
  L"Read end phase (B)",
  L"读取结束相位",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_3533[NLANGS] = {
  L"Read window width",
  L"读取检测值宽度",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_3534[NLANGS] = {
  L"Input-HV1 control distance",
  L"进料到HV1控制距离",
  L"#",
  L"@",
  L"&"  };

  //Pour added 20180301
  static wchar_t* string_3534b[NLANGS] = {
  L"Input-HV2 control distance",
  L"进料到HV2控制距离",
  L"#",
  L"@",
  L"&"  };

    static wchar_t* string_3534c[NLANGS] = {
  L"Input-HV3 control distance",
  L"进料到HV3控制距离",
  L"#",
  L"@",
  L"&"  };

    static wchar_t* string_3534d[NLANGS] = {
  L"Input-HV4 control distance",
  L"进料到HV4控制距离",
  L"#",
  L"@",
  L"&"  };
  //Pour added 20180301

  static wchar_t* string_3540[NLANGS] = {
  L"Voltage output HV1",
  L"HV第一工位电压输出",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_3542[NLANGS] = {
  L"Voltage output HV2",
  L"HV第二工位电压输出",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_3544[NLANGS] = {
  L"Voltage output HV3",
  L"HV第三工位电压输出",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_3546[NLANGS] = {
  L"Voltage output HV4",
  L"HV第四工位电压输出",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_3550[NLANGS] = {
  L"Voltage Min Threshold HV1",
  L"HV第一工位电压阈值小",
  L"#",
  L"@",
  L"&"  };

  //Pour added 20180301
  static wchar_t* string_3550a[NLANGS] = {
  L"Voltage Max Threshold HV1",
  L"HV第一工位电压阈值大",
  L"#",
  L"@",
  L"&"  };
  //Pour added 20180301

  static wchar_t* string_3552[NLANGS] = {
  L"Voltage Min Threshold HV2",
  L"HV第二工位电压阈值小",
  L"#",
  L"@",
  L"&"  };
  
  //Pour added 20180301
  static wchar_t* string_3552a[NLANGS] = {
  L"Voltage Max Threshold HV2",
  L"HV第二工位电压阈值大",
  L"#",
  L"@",
  L"&"  };
  //Pour added 20180301

  static wchar_t* string_3554[NLANGS] = {
  L"Voltage Min Threshold HV3",
  L"HV第三工位电压阈值小",
  L"#",
  L"@",
  L"&"  };

  //Pour added 20180301
  static wchar_t* string_3554a[NLANGS] = {
  L"Voltage Max Threshold HV3",
  L"HV第三工位电压阈值大",
  L"#",
  L"@",
  L"&"  };
  //Pour added 20180301

  static wchar_t* string_3556[NLANGS] = {
  L"Voltage Min Threshold HV4",
  L"HV第四工位电压阈值小",
  L"#",
  L"@",
  L"&"  };

  //Pour added 20180301
  static wchar_t* string_3556a[NLANGS] = {
  L"Voltage Max Threshold HV4",
  L"HV第四工位电压阈值大",
  L"#",
  L"@",
  L"&"  };
  //Pour added 20180301


  static wchar_t* string_hvrej[NLANGS] = {
  L"HV rejects",
  L"高压检测剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_3560[NLANGS] = {
  L"HV Data rec Off",
  L"进料传感器数据收集关闭",
  L"#",
  L"@",
  L"&"  };

  //Pour added 20180224
  static wchar_t* string_3560a[NLANGS] = {
  L"HV Data rec On",
  L"HV数据收集开启",
  L"#",
  L"@",
  L"&"  };
  //Pour added 20180224

static wchar_t* string_3568[NLANGS] = {
  L"VIBRATOR",
  L"震动装置",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_3570[NLANGS] = {
  L"Vibrator speed",
  L"震动装置速度",
  L"#",
  L"@",
  L"&"  };

// NEW PASSWORDS & USERS MANAGEMENT

static wchar_t* string_3810[NLANGS] = {
  L"SUPERVISOR",
  L"管理员",
  L"#",
  L"@",
  L"руководитель"  };

static wchar_t* string_3812[NLANGS] = {
  L"MAINTENANCE",
  L"工艺管理员",
  L"#",
  L"@",
  L"обслуживание"  };

  //Pour added for HZB 20190109
  static wchar_t* string_3810a[NLANGS] = {
  L"Admin",
  L"设备管理员",
  L"#",
  L"@",
  L"руководитель"  };

static wchar_t* string_3812a[NLANGS] = {
  L"QA",
  L"质量管理员",
  L"#",
  L"@",
  L"обслуживание"  };
  //The end 20190109

static wchar_t* string_3814[NLANGS] = {
  L"OPERATOR",
  L"操作员",
  L"#",
  L"@",
  L"оператор"  };

static wchar_t* string_3816[NLANGS] = {
  L"NO ACCESS",
  L"访客",
  L"#",
  L"@",
  L"нет доступа"  };

static wchar_t* string_3820[NLANGS] = {
  L"User\r\n%d/%d",
  L"用户\r\n%d/%d",
  L"#\r\n%d/%d",
  L"@\r\n%d/%d",
  L"&\r\n%d/%d"  };
  
static wchar_t* string_3830[NLANGS] = {
  L"Username",
  L"用户名",
  L"#",
  L"@",
  L"имя пользователя"  };

static wchar_t* string_3832[NLANGS] = {
  L"Password",
  L"密码",
  L"#",
  L"@",
  L"пароль"  };

  //Pour added for translation
  static wchar_t* string_3835[NLANGS] = {
  L"UPDATE",
  L"更新",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_3836[NLANGS] = {
  L"ADD",
  L"增加",
  L"#",
  L"@",
  L"&"  };

  //Pour added for HZB 20190109
  static wchar_t* string_3837[NLANGS] = {
  L"DELETE",
  L"删除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_3838[NLANGS] = {
  L"CANCEL",
  L"退出",
  L"#",
  L"@",
  L"&"  };
  //The end 20200914


static wchar_t* string_3840[NLANGS] = {
  L"Max number of user (%d) reached\r\nCannot add more",
  L"到达用户上限人数(%d)\r\n不能再增加",
  L"#",
  L"@",
  L"&"  };

    static wchar_t* string_3840a[NLANGS]={
  L"Cannot add more user to this level",
  L"该权限使用人不能再增加",
  L"#",
  L"@",
  L"&"  };


static wchar_t* string_3842[NLANGS] = {
  L"Users n.1/2/3/4 cannot be deleted",
  L"用户n.1/2/3/4不能删除",
  L"#",
  L"@",
  L"имя пользователя n.1/2/3/4 не может быть исключен"  };

static wchar_t* string_3844[NLANGS] = {
  L"User <%s> already exists at position <%d>\r\nCannot add another with same name",
  L"用户 <%s>已经存在于 <%d>，不能增加相同的用户名\r\n",
  L"#",
  L"@",
  L"&"  };


    static wchar_t* string_3844a[NLANGS] = {
  L"Password already in use, Cannot set same password to one user",
  L"已在使用的密码, 不能对一个用户设相同的密码",
  L"#",
  L"@",
  L"&"  };

  //Pour added for Aohong on 20191101
  static wchar_t* string_3846[NLANGS] = {
  L"It is not allowed to change other users' password？",
  L"不允许更改其他用户的密码",
  L"#",
  L"@",
  L"&"  };
  //Pour added for Aohong on 20191101

  //Pour added for Aohong on 20191101
  static wchar_t* string_3847[NLANGS] = {
  L"You need to accesss to creat a new user",
  L"您没有足够的权限创建新用户",
  L"#",
  L"@",
  L"&"  };
  //Pour added for Aohong on 20191101

  //Pour added for Aohong on 20191101
  static wchar_t* string_3848[NLANGS] = {
  L"You need to accesss to delete a user",
  L"您没有足够的权限删除用户",
  L"#",
  L"@",
  L"&"  };
  //Pour added for Aohong on 20191101

  //Pour added for Sunho on 20200502
  static wchar_t* string_3849[NLANGS] = {
  L"Are you sure you want to change the password for selected user <%s> ?",
  L"确定更改当前用户密码吗 <%s> ？",
  L"¿Esta seguro que desea borrar la receta seleccionada <%s>?"  ,
  L"@",
  L"Удалить рецепт <%s>?"  };
  //Pour added for Sunho on 20200502

static wchar_t* string_3850[NLANGS] = {
  L"Enter Username",
  L"输入用户名",
  L"#",
  L"@",
  L"введите имя пользователя"  };

static wchar_t* string_3852[NLANGS] = {
  L"Enter Password",
  L"输入密码",
  L"#",
  L"@",
  L"введите пароль"  };

static wchar_t* string_3854[NLANGS] = {
  L"The first and second passwords don't match or are too long/short\r\nPlease enter password again",
  L"第一个和第二个密码不符或者太长/短\r\n 请再次输入密码",
  L"#",
  L"@",
  L"первый и второй пароли не совпадают или слишком длинны / короткие\r\n Please введите пароль снова"  };

static wchar_t* string_3860[NLANGS] = {
  L"Password not acceptable: %d char long while must be at least %d long",
  L"密码不正确： %d 个字符长度至少 %d 个",
  L"# %d %d ",
  L"@ %d %d ",
  L"неверный пароль: %d символы по крайней мере %d "  };

    static wchar_t* string_3860a[NLANGS] = {
  L"Input not acceptable: length <=20",
  L"输入不正确：长度不超过20",
  L"# %d %d ",
  L"@ %d %d ",
  L"вход неприемлем: длина < = 20 "  };


static wchar_t* string_3862[NLANGS] = {
  L"Password not acceptable: contains %d uppercase chars while at least %d are required",
  L"密码不正确：包含 %d 个大写字符但是至少需要%d个 ",
  L"# %d %d ",
  L"@ %d %d ",
  L"пароль неприемлем: содержит %d Uppercase chars, в то время как требуется по крайней мере %d "  };

static wchar_t* string_3864[NLANGS] = {
  L"Password not acceptable: contains %d lowercase chars while at least %d are required",
  L"密码不正确: 包含 %d 个小写字符但是至少需要 %d 个",
  L"# %d %d ",
  L"@ %d %d ",
  L"пароль неприемлем: содержит %d строчные шарики, в то время как требуется по крайней мере %d "  };

static wchar_t* string_3866[NLANGS] = {
  L"Password not acceptable: contains %d digits while at least %d are required",
  L"密码不正确： 包含 %d 字节但是至少需要 %d 个",
  L"# %d %d ",
  L"@ %d %d ",
  L"пароль неприемлем: содержит %d цифры, в то время как требуется по крайней мере %d "  };


    static wchar_t* string_3868[NLANGS] = {
  L"Input not acceptable: contains illegal character",
  L"输入不正确：包含非法字符",
  L"# %d %d ",
  L"@ %d %d ",
  L"вход неприемлем: содержит незаконный символ "  };

static wchar_t* string_3869[NLANGS] = {
  L"Input not acceptable: the username is not valid",
  L"输入不正确：该用户名无效",
  L"# %d %d ",
  L"@ %d %d ",
  L"ввод недопустим: имя пользователя не действительно "  };

static wchar_t* string_3870[NLANGS] = {
  L"User <%s> <%d>logged in",
  L"用户名 <%s> <%d>登录成功",
  L"# %d %d ",
  L"@ %d %d ",
  L"пользователь <% s > <% d > вошел в систему "  };

static wchar_t* string_3871[NLANGS] = {
  L"User <%s> <%d>was locked for 10 minutes",
  L"用户 <%s> <%d>被锁定10分钟",
  L"# %d %d ",
  L"@ %d %d ",
  L"пользователь <%s> <%d> заблокирован в течение 10 минут "  };

static wchar_t* string_3872[NLANGS] = {
  L"Expired password for user <%s> <%d>, ask supervisor to help",
  L"用户 <%s> <%d> 的密码过期，需管理员帮助",
  L"# %d %d ",
  L"@ %d %d ",
  L"пароль пользователя <%s> <%d> истекает, нужна помощь администратора "  };

static wchar_t* string_3872a[NLANGS] = {
  L"Password for user <%s> will exprire soon, ask supervisor to help",
  L"用户 <%s> 的密码即将过期，需管理员帮助",
  L"# %d %d ",
  L"@ %d %d ",
  L"пароль пользователя <%s> вот-вот истечет, нужна помощь администратора "  };

  //Pour 20200728
  static wchar_t* string_3872b[NLANGS] = {
  L"Password for user <%s> will exprire soon, ask supervisor to help",
  L"用户 <%s> 的锁定时间未到，请耐心等待",
  L"# %d %d ",
  L"@ %d %d ",
  L"время блокировки для пользователя <%s> не включено. пожалуйста, подождите терпеливо. "  };

static wchar_t* string_3873[NLANGS] = {
  L"No access level need not password to login",
  L"访客无需密码登录",
  L"# %d %d ",
  L"@ %d %d ",
  L"посетителям не нужен пароль для входа в систему "  };

static wchar_t* string_3874[NLANGS] = {
  L"Username can not be changed",
  L"用户名不能被改变",
  L"# %d %d ",
  L"@ %d %d ",
  L"& %d %d "  };

static wchar_t* string_3875[NLANGS] = {
  L"Password input error, up to 5 times, <%d>times remaining ",
  L"密码输入错误，最多5次，剩余<%d>次",
  L"# %d %d ",
  L"@ %d %d ",
  L"неверный пароль, до 5 раз, оставаясь <%d> раз "  };


// NEW LOG MANAGEMENT

static wchar_t* string_3900[NLANGS] = {
  L"Show log",
  L"显示LOG",
  L"#",
  L"@",
  L"Показать журнал"  };

static wchar_t* string_3904[NLANGS] = {
  L"Close",
  L"关闭",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_3910[NLANGS] = {
  L"Save text file",
  L"保存TXT文档",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_3912[NLANGS] = {
  L"Load log file",
  L"下载log文档",
  L"#",
  L"@",
  L"&"  };

    //Pour 20190603
  static wchar_t* string_3914[NLANGS] = {
  L"Save PDF file",
  L"保存PDF文档",
  L"#",
  L"@",
  L"сохранить файл PDF"  };
  //Pour 20190603


static wchar_t* string_3920[NLANGS] = {
  L"EMPTY OR MISSING LOG FILE",
  L"log文档为空或者丢失",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_3930[NLANGS] = {
  L"LOG FILE SIGNATURE MISMATCH: FILE <%s> HAS BEEN ALTERED!\r\nFile has been renamed as <%s>",
  L"LOG文档签名不符<%s> 文档已被修改\r\n <%s>",
  L"#\r\n<%s> \r\n <%s>",
  L"@\r\n<%s> \r\n <%s>",
  L"&\r\n<%s> \r\n <%s>"  };

static wchar_t* string_3932[NLANGS] = {
  L"Current log has been exported in text format to <%s> file",
  L"^ <%s>",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };



// 4100 - PFS Flange inspection
static wchar_t* string_4100[NLANGS] = {
  L"Flange inspection",
  L"法兰检测",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4102[NLANGS] = {
  L"Inner radius",
  L"内部半径",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4102a[NLANGS] = {
  L"Flange shape learning",
  L"法兰形状学习",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4103[NLANGS] = {
  L"Stopper brightness",
  L"胶塞亮度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4104[NLANGS] = {
  L"Alignment min points",
  L"对齐最小点数",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4104a[NLANGS] = {
  L"Difference threshold",
  L"差异门槛值",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4104b[NLANGS] = {
  L"Convex depth threshold",
  L"凸包深度门槛值",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_4104c[NLANGS] = {
  L"Symetry measure threshold",
  L"对称度门槛值",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_4105[NLANGS] = {
  L"Flange radius",
  L"法兰半径",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4106[NLANGS] = {
  L"Flange analysis lenght",
  L"法兰分析长度",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4106a[NLANGS] = {
  L"Flange learning success",
  L"法兰学习成功",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4108[NLANGS] = {
  L"Flange min radius",
  L"法兰最小半径",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4108a[NLANGS] = {
  L"Flange shape difference %d/%d, %d",
  L"法兰形状差异度 %d/%d, %d",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

  static wchar_t* string_4109[NLANGS] = {
  L"Symetry/convex defects %d,%d/%d | %d",
  L"对称性凸性缺陷 %d,%d/%d | %d",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_4110[NLANGS] = {
  L"Flange max radius",
  L"法兰最大半径",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4110a[NLANGS] = {
  L"Failed to create dat file",
  L"创建数据文件失败",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_4112[NLANGS] = {
  L"Petal min lenght",
  L"花瓣最小长度",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4112a[NLANGS] = {
  L"Learning data not enough",
  L"学习数据不足",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

  static wchar_t* string_4113[NLANGS] = {
  L"Flange shape not good for learning",
  L"法兰形状不佳",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_4114[NLANGS] = {
  L"Petal max angle",
  L"花瓣最大角度",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4114a[NLANGS] = {
  L"Flange data file not found",
  L"法兰数据文件未找到",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_4116[NLANGS] = {
  L"Flange inspection",
  L"法兰检测",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4116a[NLANGS] = {
  L"Flange data file can not open",
  L"法兰数据文件不能打开",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_4118[NLANGS] = {
  L"Flange inspection",
  L"法兰检测",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4118a[NLANGS] = {
  L"Read flange data error",
  L"数据文件读取有误",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

  static wchar_t* string_4119[NLANGS] = {
  L"Flange bounding box inormal",
  L"法兰包络框异常",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_4122[NLANGS] = {
  L"Flange threshold",
  L"法兰阈值",
  L"#",
  L"@",
  L"&"  };


   static wchar_t* string_4170[NLANGS] = {
  L"XTS1 gear pos",
  L"校准进料磁导轨耦合点",
  L"# ",
  L"@",
  L"& "  };


  static wchar_t* string_4172[NLANGS] = {
  L"XTS1 stop",
  L"停止进料磁导轨",
  L"# ",
  L"@",
  L"& "  };

  static wchar_t* string_4174[NLANGS] = {
  L"XTS1 homing",
  L"复位进料磁导轨",
  L"# ",
  L"@",
  L"& "  };


    static wchar_t* string_4176[NLANGS] = {
  L"XTS1 testing",
  L"测试进料磁导轨",
  L"# ",
  L"@",
  L"& "  };


     static wchar_t* string_4180[NLANGS] = {
  L"XTS2 gear pos",
  L"校准出料磁导轨耦合点",
  L"# ",
  L"@",
  L"& "  };


  static wchar_t* string_4182[NLANGS] = {
  L"XTS2 stop",
  L"停止出料磁导轨",
  L"# ",
  L"@",
  L"& "  };

  static wchar_t* string_4184[NLANGS] = {
  L"XTS2 homing",
  L"复位出料磁导轨",
  L"# ",
  L"@",
  L"& "  };


  static wchar_t* string_4186[NLANGS] = {
  L"XTS2 testing",
  L"测试出料磁导轨",
  L"# ",
  L"@",
  L"& "  };


  static wchar_t* string_4188[NLANGS] = {
  L"XTS adjust 1&&2",
  L"磁导轨调试1&&2",
  L"# ",
  L"@",
  L"& "  };


static wchar_t* string_4190[NLANGS] = {
  L"REJECTS TRAY 1&&2",
  L"剔除托盘1&&2",
  L"# RECHAZOS",
  L"@",
  L"& ОТБР."  };

static wchar_t* string_4192[NLANGS] = {
  L"REJECTS TRAY 3",
  L"剔除托盘3",
  L"# RECHAZOS",
  L"@",
  L"& ОТБР."  };

static wchar_t* string_4194[NLANGS] = {
  L"REJECTS TRAY 4",
  L"剔除托盘4",
  L"# RECHAZOS",
  L"@",
  L"& ОТБР."  };


  static wchar_t* string_4196[NLANGS] = {
  L"Clean all syringes",
  L"排空所有产品",
  L"#",
  L"@",
  L"&"  };


static wchar_t* string_4198[NLANGS] = {
  L"Enable Spin rejects",
  L"旋转不良剔除到托盘",
  L"Habilitar rechazos giro",
  L"@",
  L"ОТБР. в лоток: плохое вращение"  };



static wchar_t* string_4200[NLANGS] = {
  L"Enable flange reject",
  L"使能法兰剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4210[NLANGS] = {
  L"Enable particle 1 rejects",
  L"使能粒子1剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4212[NLANGS] = {
  L"Enable filling 1 rejects",
  L"使能液位1剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4214[NLANGS] = {
  L"Enable particle 2 rejects",
  L"使能粒子2剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4216[NLANGS] = {
  L"Enable filling 2 rejects",
  L"使能液位2剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4218[NLANGS] = {
  L"Enable sidewall 1 rejects",
  L"使能外观1剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4220[NLANGS] = {
  L"Enable stopper 1 rejects",
  L"使能胶塞1剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4222[NLANGS] = {
  L"Enable sidewall 2 rejects",
  L"使能外观2剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4224[NLANGS] = {
  L"Enable stopper 2 rejects",
  L"使能胶塞2剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4226[NLANGS] = {
  L"Enable Cosmetic3A rejects",
  L"使能外观3-A剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4228[NLANGS] = {
  L"Enable Cosmetic3B rejects",
  L"使能外观3-B剔除",
  L"#",
  L"@",
  L"&"  };




static wchar_t* string_4240[NLANGS] = {
  L"Distance load - TCA",
  L"进料到TCA的距离",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4242[NLANGS] = {
  L"Distance load - P1A",
  L"进料到P1A的距离",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4244[NLANGS] = {
  L"Distance load - P1B",
  L"进料到P1B的距离",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4246[NLANGS] = {
  L"Distance load - P1C",
  L"进料到P1C的距离",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_4247[NLANGS] = {
  L"Distance load - P1D",
  L"进料到P1D的距离",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4248[NLANGS] = {
  L"Distance load - C3A",
  L"进料到C3A的距离",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_4249[NLANGS] = {
  L"Distance load - C3B",
  L"进料到C3B的距离",
  L"#",
  L"@",
  L"&"  };


static wchar_t* string_4250[NLANGS] = {
  L"Cosmetic 1 phase",
  L"外观1检测相位",
  L"# Fase superficie",
  L"@",
  L"& Фаза : дефекты "  };

static wchar_t* string_4252[NLANGS] = {
  L"Cosmetic 2 phase",
  L"外观2检测相位",
  L"# Fase superficie",
  L"@",
  L"& Фаза : дефекты "  };

static wchar_t* string_4254[NLANGS] = {
  L"TCA phase",
  L"法兰检测相位",
  L"# Fase superficie",
  L"@",
  L"& Фаза : дефекты "  };

static wchar_t* string_4256[NLANGS] = {
  L"Cosmetic3A phase",
  L"外观3-A检测相位",
  L"# Fase superficie",
  L"@",
  L"& Фаза : дефекты "  };

static wchar_t* string_4258[NLANGS] = {
  L"Cosmetic3B phase",
  L"外观3-B检测相位",
  L"# Fase superficie",
  L"@",
  L"& Фаза : дефекты "  };


// Strings related to needle shield

static wchar_t* string_5006[NLANGS] = {
  L"Needle control DISABLED",
  L"针帽检测禁用",
  L"#",
  L"@",
  L"&"  };


static wchar_t* string_5010[NLANGS] = {
  L"Needle Shield",
  L"针帽",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5014[NLANGS] = {
  L"Needle shield LEARNING %d,%d,%d,%d",
  L"针帽学习 %d,%d,%d,%d",
  L"# %d,%d,%d,%d",
  L"@ %d,%d,%d,%d",
  L"& %d,%d,%d,%d"  };

static wchar_t* string_5016[NLANGS] = {
  L"Needle Shield GOOD",
  L"针帽良好",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5018[NLANGS] = {
  L"REJECT %s",
  L"剔除 %s",
  L"# %s",
  L"@ %s",
  L"& %s"  };





static wchar_t* string_5020[NLANGS] = {
  L"Learn model",
  L"学习模式",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_5022[NLANGS] = {
  L"Start learning",
  L"开始学习",
  L"#",
  L"@",
  L"&"  }; 


static wchar_t* string_5024[NLANGS] = {
  L"Stop learning",
  L"停止学习",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5026[NLANGS] = {
  L"Start data saving",
  L"开始数据保存",
  L"#",
  L"@",
  L"&"  }; 


static wchar_t* string_5028[NLANGS] = {
  L"Stop data saving",
  L"停止数据保存",
  L"#",
  L"@",
  L"&"  };


static wchar_t* string_5030[NLANGS] = {
  L"Save model data",
  L"保存模型数据",
  L"#",
  L"@",
  L"&"  };

  static wchar_t* string_5034[NLANGS] = {
  L"Error: <%d> not enough samples for <%s>",
  L"错误 <%d> 没有足够的样本<%s>",
  L"# <%d> <%s>",
  L"@ <%d> <%s>",
  L"& <%d> <%s>"  };


static wchar_t* string_5106[NLANGS] = {
  L"Max deviation",
  L"最大偏差",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5110[NLANGS] = {
  L"A l-r width",
  L"A l-r 宽度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5112[NLANGS] = {
  L"A l-r height",
  L"A l-r 高度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5114[NLANGS] = {
  L"B l-r width",
  L"B l-r 宽度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5116[NLANGS] = {
  L"B l-r height",
  L"B l-r 高度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5118[NLANGS] = {
  L"Needle tilt",
  L"针帽歪斜",
  L"#",
  L"@",
  L"&"  };


  static wchar_t* string_5120[NLANGS] = {
  L"C l-r width",
  L"C l-r 宽度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5122[NLANGS] = {
  L"D l-r width",
  L"D l-r 宽度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5124[NLANGS] = {
  L"E l-r width",
  L"E l-r 宽度",
  L"#",
  L"@",
  L"&"  };


static wchar_t* string_5126[NLANGS] = {
  L"PAL-PBR diag",
  L"PAL-PBR 斜线",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5128[NLANGS] = {
  L"PAR-PBL diag",
  L"PAR-PBL 斜线",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5130[NLANGS] = {
  L"Model maxerr(*)",
  L"模型最大错误(*)",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5140[NLANGS] = {
  L"Low corner maxerr(*)",
  L"拐角最大错误(*)",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5150[NLANGS] = {
  L"LEARNING %d,%d,%d,%d,%d",
  L"学习 %d,%d,%d,%d,%d",
  L"# %d,%d,%d,%d,%d",
  L"@ %d,%d,%d,%d,%d",
  L"& %d,%d,%d,%d,%d"  };


static wchar_t* string_5160[NLANGS] = {
  L"cannot find closed contour %d (%d/%d)",
  L"不能找到接近的轮廓 %d (%d/%d)",
  L"# %d (%d/%d)",
  L"@ %d (%d/%d)",
  L"& %d (%d/%d)"  };

static wchar_t* string_5162[NLANGS] = {
  L"too many points in contour %d (%d/%d)",
  L"轮廓上过多的点 %d (%d/%d)",
  L"# %d (%d/%d)",
  L"@ %d (%d/%d)",
  L"& %d (%d/%d)"  };

static wchar_t* string_5164[NLANGS] = {
  L"too few points in contour %d (%d/%d)",
  L"轮廓上过少的点 %d (%d/%d)",
  L"# %d (%d/%d)",
  L"@ %d (%d/%d)",
  L"& %d (%d/%d)"  };


static wchar_t* string_5170[NLANGS] = {
  L"Can't find %s",
  L"不能找到 %s",
  L"# %s",
  L"@ %s",
  L"& %s"  };


static wchar_t* string_5180[NLANGS] = {
  L"<%s> Good %d %d/%d",
  L"<%s> 好 %d %d/%d",
  L"<%s> # %d %d/%d",
  L"<%s> @ %d %d/%d",
  L"<%s> & %d %d/%d"  };

static wchar_t* string_5182[NLANGS] = {
  L"<%s> too high %d /%d",
  L"<%s> 太高 %d /%d",
  L"<%s> # %d /%d",
  L"<%s> @ %d /%d",
  L"<%s> & %d /%d"  };

static wchar_t* string_5184[NLANGS] = {
  L"<%s> too low %d /%d",
  L"<%s> 太低 %d /%d",
  L"<%s> # %d /%d",
  L"<%s> @ %d /%d",
  L"<%s> & %d /%d"  };

static wchar_t* string_5186[NLANGS] = {
  L"<%s> NO MODEL DATA",
  L"<%s> 无模型数据",
  L"<%s> #",
  L"<%s> @",
  L"<%s> &"  };



static wchar_t* string_5206[NLANGS] = {
  L"Stopper DISABLED",
  L"禁用胶塞",
  L"#",
  L"@",
  L"&"  };



static wchar_t* string_5210[NLANGS] = {
  L"Stopper",
  L"胶塞",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5230[NLANGS] = {
  L"REJECT: profile NOT found %d/%d",
  L"^ 剔除: 没有找到瓶子 %d/%d",
  L"# RECHAZO:Contenedor no encontrado  %d/%d",
  L"@ %d/%d",
  L"& ОТБР.: не найден контейнер %d/%d "  };

static wchar_t* string_5234[NLANGS] = {
  L"REJECT: profile going out",
  L"剔除：轮廓超出 ",
  L"# ",
  L"@ ",
  L"& "  };



static wchar_t* string_5240[NLANGS] = {
  L"Stopper width",
  L"胶塞宽度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5242[NLANGS] = {
  L"Stopper height",
  L"胶塞高度",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_5244[NLANGS] = {
  L"Stopper vpos",
  L"胶塞位置",
  L"#",
  L"@",
  L"&"  };


static wchar_t* string_5250[NLANGS] = {
  L"UPSIDE-DOWN %d/%d",
  L"颠倒 %d/%d",
  L"# %d/%d",
  L"@ %d/%d",
  L"& %d/%d"  };


static wchar_t* string_5256[NLANGS] = {
  L"Minnum pts bottom",
  L"底部最少点数",
  L"#",
  L"@",
  L"&"  };



static wchar_t* string_5260[NLANGS] = {
  L"Sidewall DISABLED",
  L"禁用外观",
  L"#",
  L"@",
  L"&"  };



  // STRINGS RELATED TO NECK INSPECTION 
// added by yang 2018/12/1



static wchar_t* string_3940[NLANGS] = {
  L"Inspection area",
  L"检测区域",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3942[NLANGS] = {
  L"Head border",
  L"瓶头边缘门槛",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3944[NLANGS] = {
  L"Head2neck offset",
  L"瓶颈半径缩进",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3946[NLANGS] = {
  L"Neck height",
  L"瓶脖高度",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3948[NLANGS] = {
  L"Bloc width",
  L"分块宽度",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3950[NLANGS] = {
  L"Low limit",
  L"灰度低限",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3952[NLANGS] = {
  L"High limit",
  L"灰度高限",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3954[NLANGS] = {
  L"Absolute error number",
  L"绝对值坏点数量",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3956[NLANGS] = {
  L"Difference error number",
  L"相对坏点数量",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3958[NLANGS] = {
  L"Threshold of difference",
  L"相对差值门槛",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3960[NLANGS] = {
  L"Distance of difference",
  L"相对差值距离",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3962[NLANGS] = {
  L"Neck errors %d, %d | %d, %d",
  L"瓶脖错误 %d, %d | %d, %d",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3964[NLANGS] = {
  L"Crimp errors %d, %d",
  L"翻边错误 %d, %d",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3966[NLANGS] = {
  L"Min points",
  L"搜索最少点数",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3968[NLANGS] = {
  L"Crimping height",
  L"翻边高度",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3970[NLANGS] = {
  L"Neck Inspection",
  L"瓶脖检测",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3972[NLANGS] = {
  L"Crimping Inspection",
  L"翻边检测",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };


  static wchar_t* string_3974[NLANGS] = {
  L"Crimping edge abnormal",
  L"翻边边缘异常",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3974a[NLANGS] = {
  L"Too few key points searched",
  L"搜索到的边界点太少",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3974c[NLANGS] = {
  L"Vertical spacing of key points too big",
  L"边界点垂直间距太大",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3974b[NLANGS] = {
  L"Horizontal spacing of key points too big",
  L"边界点水平间距太大",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3974d[NLANGS] = {
  L"Vial head not good",
  L"瓶头不合适",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3976[NLANGS] = {
  L"Tolerance of zigzag",
  L"锯齿边容忍度",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3978[NLANGS] = {
  L"Ellipse 2A:%.2f,2B:%.2f,angle:%.2f",
  L"椭圆参数 2A:%.2f,2B:%.2f,倾角:%.2f",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };


static wchar_t* string_3980[NLANGS] = {
  L"Ellipse center:(%.2f,%.2f)",
  L"椭圆参数 中心:(%.2f,%.2f)",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3982[NLANGS] = {
  L"Ellipse params learning",
  L"椭圆参数学习",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3983[NLANGS] = {
  L"Ellipse params need improve",
  L"椭圆参数需改进",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3984[NLANGS] = {
  L"Crimping brightness",
  L"翻边搜索亮度",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_3986[NLANGS] = {
  L"Crimping edge out of range",
  L"翻边边缘搜索出界",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };



// STRINGS RELATED TO HEEL INSPECTION  
// added by yang 2018/12/10
static wchar_t* string_4066[NLANGS] = {
  L"Cake contour",
  L"粉饼轮廓搜索",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };

static wchar_t* string_4068[NLANGS] = {
  L"Max errors",
  L"最大错误点数",
  L"# <%s>",
  L"@ <%s>",
  L"& <%s>"  };




// STRINGS RELATED TO BATCH REPORT GENERATION

static wchar_t* string_8000[NLANGS] = {
  L"BATCH REPORT",
  L"批次报告",
  L"REPORTE DE LOTE",
  L"@",
  L"ОТЧЕТ ПО ПАРТИИ "  };

static wchar_t* string_8006[NLANGS] = {
  L"Page %d/%d",
  L"页 %d/%d",
  L"Página %d/%d",
  L"@",
  L"Стр. %d/%d "  };

  static wchar_t* string_8006a[NLANGS] = {
  L"Page %d",
  L"第 %d 页",
  L"Página %d",
  L"@",
  L"Стр. %d "  };

static wchar_t* string_8010[NLANGS] = {
  L"Machine ID ____________ %s",
  L"机器编号 _______________ %s",
  L"ID máquina",
  L"@",
  L"ID машины ____________"  };


    //Pour chaged format on 20191101
  static wchar_t* string_8010a[NLANGS] = {
  L"Machine ID _ _ _ _ _ _  %s",
  L"机器编号 _ _ _ _ _ _ _ %s",
  L"ID máquina",
  L"@",
  L"ID машины ____________"  };
  //Pour chaged format on 20191101

  static wchar_t* string_8011[NLANGS] = {
  L"Machine model",
  L"设备型号",
  L"ID máquina",
  L"@",
  L"ID машины"  };


static wchar_t* string_8012[NLANGS] = {
  L"Machine ID",
  L"机器编号",
  L"ID máquina",
  L"@",
  L"ID машины"  };

  //Pour added for Kexing
  static wchar_t* string_8013[NLANGS] = {
  L"Date of production",
  L"生产日期",
  L"ID máquina",
  L"@",
  L"ID машины"  };


    static wchar_t* string_8014[NLANGS] = {
  L"Expiry date",
  L"产品有效期",
  L"ID máquina",
  L"@",
  L"ID машины"  };
  //Pour added for Kexing




static wchar_t* string_8020[NLANGS] = {
  L"Current recipe ________ %s",
  L"生产配方 _______________  %s",
  L"Receta actual",
  L"@",
  L"Текущий рецепт________%s "  };

static wchar_t* string_8021[NLANGS] = {
  L"Current recipe",
  L"当前配方",
  L"Receta actual",
  L"@",
  L"Текущий рецепт"  };

static wchar_t* string_8022[NLANGS] = {
  L"Product name ____________ %s",
  L"品名/规格 _______________ %s",
  L"Nombre de lote___________%s",
  L"@",
  L"Наименование продуктов ____________% s "  };

static wchar_t* string_8023[NLANGS] = {
  L"Product name",
  L"品名/规格",
  L"Nombre lote",
  L"@",
  L"Наименование продуктов"  };

  //Pour added 20180505
  static wchar_t* string_8023a[NLANGS] = {
  L"Batch name",
  L"品名/规格",
  L"Nombre lote",
  L"@",
  L"Наименование продуктов"  };
  //Pour added 20180505

static wchar_t* string_8024[NLANGS] = {
  L"Batch operator",
  L"操作人",
  L"Operador de lote",
  L"@",
  L"Оператор"  };

static wchar_t* string_8025[NLANGS] = { 
  L"Batch operator",
  L"操作人",
  L"Operador lote",
  L"@",
  L"Оператор"  };

static wchar_t* string_8026[NLANGS] = {
  L"Batch date _____ %s",
  L"机检日期 ______________ %s",
  L"Descripción Lote _____ %S",
  L"@",
  L"Дата изготовления______________ %s "  };

static wchar_t* string_8027[NLANGS] = {
  L"Batch date",
  L"机检日期",
  L"Descripción Lote",
  L"@",
  L"Дата изготовления"  };

static wchar_t* string_8028[NLANGS] = {
  L"Batch start ___________ %s",
  L"开始时间 ______________ %s",
  L"Arranque lote___________%s",
  L"@",
  L"Начало партии______________ %s "  };

static wchar_t* string_8029[NLANGS] = {
  L"Batch start",
  L"开始批次",
  L"Arranque lote",
  L"@",
  L"Начало партии"  };

static wchar_t* string_8030[NLANGS] = {
  L"Batch end _____________ %s",
  L"结束时间 ______________ %s",
  L"Fin de lote_____________%s",
  L"@",
  L"Конец партии______________ %s "  };

static wchar_t* string_8031[NLANGS] = {
  L"Batch end",
  L"结束批次",
  L"Fin de lote",
  L"@",
  L"Конец партии"  };

static wchar_t* string_8032[NLANGS] = {
  L"Batch status __________ %s",
  L"批次状态 _______________ %s",
  L"Estado de lote_________ %s",
  L"@",
  L"Статус партии__________ %s "  };

static wchar_t* string_8033[NLANGS] = {
  L"Batch status",
  L"批次状态",
  L"Estado de lote",
  L"@",
  L"Статус партии"  };

static wchar_t* string_8034[NLANGS] = {
  L"RUNNING",
  L"运行",
  L"ARRANQUE",
  L"@",
  L"В работе"  };

static wchar_t* string_8036[NLANGS] = {
  L"NOT ACTIVE",
  L"未激活",
  L"NO ACTIVO",
  L"@",
  L"ОТКЛ"  };

static wchar_t* string_8040[NLANGS] = {
  L"Batch responsible: _____________________ Signature: ___________________",
  L"操作人及日期: __________________________  复核人及日期: __________________________",
  L"Responsable de lote: ____________________ Firma: ______________________",
  L"@",
  L"Ответственный: ______________________  Подпись: _______________________"  };

static wchar_t* string_8041[NLANGS] = {
  L"Verify by: _________________________ Signature: _______________________",
  L"批次负责人: __________________________  签名: __________________________",
  L"Responsable de lote: __________________ Firma: _________________________",
  L"@",
  L"Ответственный: _______________________  Подпись: _______________________"  };


    //Pour added for Aonong only on 20191103
  static wchar_t* string_8042[NLANGS] = {
  L"Operator: _________________________ Checker: _______________________",
  L"操作人: __________________________  复核人: __________________________",
  L"Responsable de lote: __________________ Firma: _________________________",
  L"@",
  L"Ответственный: _______________________  Подпись: _______________________"  };
  //Pour added for Aonong only on 20191103

  /*
static wchar_t* string_8042[NLANGS] = {
  L"Verify by: __________________________ Signature: __________________________ (QA)",
  L"审核人：__________________________ 签名: __________________________ (质保)",
  L"#",
  L"@",
  L"Ревизор: __________________________ Подпись: ________________________(QA)"  };
*/

static wchar_t* string_8046[NLANGS] = {
  L"Unable to read data from PLC",
  L"不能从PLC读取数据",
  L"Incapaz de leer info del PLC",
  L"@",
  L"Чтение с PLC невозможно"  };

static wchar_t* string_8050[NLANGS] = {
  L"OVERALL PRODUCTION STATISTICS",
  L"产品统计",
  L"ESTADISTICAS DE PRODUCCIÓN GENERAL",
  L"@",
  L"Статистика"  };

static wchar_t* string_8052[NLANGS] = {
  L"NO PROCESSED PRODUCT",
  L"无检测产品",
  L"PRODUCTO NO PROCESADO",
  L"@",
  L"Обработанный продукт отсутствует"  };
  
static wchar_t* string_8055[NLANGS] = {
  L"Processed products",
  L"检测产品总数",
  L"Productos procesados",
  L"@",
  L"Обработанный продукт"  };


static wchar_t* string_8057[NLANGS] = {
  L"Accepted products",
  L"合格品数",
  L"Productos aceptados",
  L"@",
  L"Пригодный продукт"  };

  #ifdef MANU_CHECK
static wchar_t* string_8056[NLANGS] = {
  L"Total accepted products",
  L"合格品总数",
  L"Productos aceptados",
  L"@",
  L"Пригодный продукт"  };

  static wchar_t* string_8057a[NLANGS] = {
  L"Machine accepted products",
  L"机器合格品",
  L"Productos aceptados",
  L"@",
  L"Пригодный продукт"  };

static wchar_t* string_8058[NLANGS] = {
  L"Manually accepted products",
  L"人工合格品",
  L"Productos aceptados",
  L"@",
  L"Пригодный продукт"  };


#endif 

static wchar_t* string_8059[NLANGS] = {
  L"Rejected products",
  L"次品总数",
  L"Productos rechazados",
  L"@",
  L"ОТБР. продукт"  };

    static wchar_t* string_8060[NLANGS] = {
  L"Sampling products",
  L"抽检产品",
  L"Productos procesados",
  L"@",
  L"Обработанный продукт"  };

static wchar_t* string_8061[NLANGS] = {
  L"Products rejected on tray 1",
  L"托盘1剔除产品数",
  L"Productos rechazados en bandeja1",
  L"@",
  L"ОТБР. продукта в лоток 1"  };

static wchar_t* string_8063[NLANGS] = {
  L"Products rejected on tray 2",
  L"托盘2剔除产品数",
  L"Productos rechazados en bandeja2",
  L"@",
  L"ОТБР. продукта в лоток 2"  };

static wchar_t* string_8063b[NLANGS] = {
  L"Products rejected on tray 3",
  L"托盘3剔除产品数",
  L"Productos rechazados en bandeja3",
  L"@",
  L"ОТБР. продукта в лоток 3"  };

static wchar_t* string_8063c[NLANGS] = {
  L"Products rejected on tray 4",
  L"托盘4剔除产品数",
  L"Productos rechazados en bandeja4",
  L"@",
  L"ОТБР. продукта в лоток 4"  };


  
static wchar_t* string_8064[NLANGS] = {
  L"Products rejected on tray 3 (HGA)",
  L"托盘3剔除产品 (HGA)",
  L"Productos rechazados en bandeja3 (HGA)",
  L"@ (HGA)",
  L"ОТБР. продукта в лоток 3 (HGA)"  };

static wchar_t* string_8065[NLANGS] = {
  L"Particle rejects",
  L"粒子剔除(所有异物)",
  L"Particula rechazada",
  L"@",
  L"ОТБР.: включения"  };

  static wchar_t* string_8066[NLANGS] = {
  L"Flange rejects",
  L"法兰剔除",
  L"Particula rechazada",
  L"@",
  L"ОТБР.: включения"  };

static wchar_t* string_8067[NLANGS] = {
  L"Particle 1 rejects",
  L"粒子1剔除（不反光异物）",
  L"Particula1 rechazada",
  L"@",
  L"ОТБР.: включения 1"  };

static wchar_t* string_8069[NLANGS] = {
  L"Particle 2 rejects",
  L"粒子2剔除（反光异物）",
  L"Particula2 rechazada",
  L"@",
  L"ОТБР.: включения 2"  };

static wchar_t* string_8071[NLANGS] = {
  L"Particle 3 rejects",
  L"粒子3剔除",
  L"Particula3 rechazada",
  L"@",
  L"ОТБР.: включения 3"  };

static wchar_t* string_8073[NLANGS] = {
  L"Filling level rejects",
  L"液位剔除",
  L"Rechazos nivel llenado",
  L"@",
  L"ОТБР.: уровень"  };

static wchar_t* string_8074[NLANGS] = {
  L"Density rejects",
  L"浓度剔除",
  L"#",
  L"@",
  L"&"  };

static wchar_t* string_8075[NLANGS] = {
  L"Filling level 1 rejects",
  L"液位1剔除",
  L"Rechazos nivel llenado1",
  L"@",
  L"ОТБР.: уровень 1"  };

static wchar_t* string_8077[NLANGS] = {
  L"Filling level 2 rejects",
  L"液位2剔除",
  L"Rechazos nivel llenado2",
  L"@",
  L"ОТБР.: уровень 2"  };

static wchar_t* string_8079[NLANGS] = {
  L"Cosmetic rejects",
  L"外观剔除",
  L"Rechazos cosmeticos",
  L"@",
  L"ОТБР.: косм. дефекты"  };

static wchar_t* string_8081[NLANGS] = {
  L"Tip rejects",
  L"头部剔除",
  L"Rechazos punta",
  L"@",
  L"ОТБР.: кончик"  };

static wchar_t* string_8083[NLANGS] = {
  L"Cap rejects",
  L"头部剔除（轧盖不良）",
  L"Rechazos tapa",
  L"@",
  L"ОТБР.: колпачок"  };

   static wchar_t* string_8084[NLANGS] = {
  L"Crimping-neck rejects",
  L"裙边/瓶脖剔除",
  L"Rechazo HGA",
  L"@",
  L"ОТБР.HGA"  };

static wchar_t* string_8087[NLANGS] = {
  L"Flip-off color rejects",
  L"易开盖颜色剔除",
  L"Rechazos color presintos",
  L"@",
  L"ОТБР.: цвета Flip-off"  };

static wchar_t* string_8088[NLANGS] = {
  L"Dot-ring color rejects",
  L"色点色环剔除",
  L"Rechazos anillo color",
  L"@",
  L"ОТБР.: точки и цв.  контур"  };

static wchar_t* string_8089[NLANGS] = {
  L"Spin rejects",
  L"旋转不良剔除",
  L"Rechazos giro",
  L"@",
  L"ОТБР.: плохое вращение"  };

static wchar_t* string_8090[NLANGS] = {
  L"COMMUNICATION WITH PLC NOT ACTIVE - STATISTICS ARE NOT AVAILABLE",
  L"和PLC通讯未激活 - 读不到数据",
  L"COMUNICACION CON PLC NO ACTIVA - ESTADISTICAS NO DISPONIBLES",
  L"@",
  L"Нет связи с PLC – Статистика недоступна"  };

static wchar_t* string_8092[NLANGS] = {
  L"LIQUID MODE ENABLED",
  L"使能水针模式",
  L"#",
  L"@",
  L"Режим жидкого продукты"  };

static wchar_t* string_8094[NLANGS] = {
  L"FREEZE DRIED MODE ENABLED",
  L"使能冻干模式",
  L"#",
  L"@",
  L"Режим ЛИО.продукты"  };



static wchar_t* string_8103[NLANGS] = {
  L"Sidewall rejects",
  L"外观检测剔除",
  L"Rechazos superficie",
  L"@",
  L"ОТБР.: боковые стенки"  };

static wchar_t* string_8105[NLANGS] = {
  L"Side Cake rejects",
  L"粉饼侧面检测剔除",
  L"Rechazos laterales",
  L"@",
  L"ОТБР.: бок ЛИО"  };

static wchar_t* string_8107[NLANGS] = {
  L"Particle2/Top Cake rejects",
  L"粒子2/粉饼顶部检测剔除",
  L"Rechazos superiores",
  L"@",
  L"ОТБР.: верхняя ЛИО"  };

static wchar_t* string_8109[NLANGS] = {
  L"Bottom A rejects",
  L"底部A检测剔除",
  L"Rechazos fondo A",
  L"@",
  L"ОТБР.: дно, камера A"  };

static wchar_t* string_8111[NLANGS] = {
  L"Bottom B rejects",
  L"底部B检测剔除",
  L"Rechazos fondo B",
  L"@",
  L"ОТБР.: дно, камера A"  };

static wchar_t* string_8112[NLANGS] = {
  L"Heel rejects",
  L"瓶根剔除",
  L"Rechazo base",
  L"@",
  L"ОТБР.: низ ЛИО"  };

static wchar_t* string_8114[NLANGS] = {
  L"Waist/layered rejects",
  L"收腰/分层剔除",
  L"cintura/capas rechazadas",
  L"@",
  L"ОТБР.: усадка/слои ЛИО"  };

static wchar_t* string_8116[NLANGS] = {
  L"HGA rejects",
  L"头部气体分析剔除",
  L"Rechazos HGA",
  L"@",
  L"ОТБР.HGA"  };

static wchar_t* string_8118[NLANGS] = {
  L"Liquid color rejects",
  L"色水剔除",
  L"#",
  L"@",
  L"&"  };


static wchar_t* string_8120[NLANGS] = {
  L"Particle1/Sidewall rejects",
  L"粒子1/外观检测剔除",
  L"Rechazos particula1/superficie",
  L"@",
  L"ОТБР.: Включение 1/боковые стенки"  };

static wchar_t* string_8122[NLANGS] = {
  L"Sidecake rejects",
  L"粉饼侧面检测剔除",
  L"Rechazos particula2/lateral",
  L"@",
  L"ОТБР.: боковые стенки"  };


static wchar_t* string_8150[NLANGS] = {
  L"GRIPPERS STATISTICS",
  L"夹爪剔除率统计",
  L"ESTADISTICAS PINZAS",
  L"@",
  L"Статистика захватов"  };


    static wchar_t* string_8160[NLANGS] = {
  L"HGA DATA STATISTICS",
  L"HGA数据统计",
  L"@",
  L"@",
  L"@"  };





// DESCRIPTIONS OF STATISTICS / UPDATE LIST ITEMS


static wchar_t* string_entered[NLANGS] = {
  L"Products entered",
  L"进料产品",
  L"Productos ingresados",
  L"@",
  L"Контейнеров загружено"  };

static wchar_t* string_totgoods[NLANGS] = {
  L"Total good products",
  L"合格品",
  L"Productos buenos",
  L"@",
  L"Пригодный продукт"  };

  // yang added 2018/03/30 for machines with manual check 
static wchar_t* string_goods[NLANGS] = {
  L"Good products",
  L"机器合格品",
  L"Productos buenos",
  L"@",
  L"хорошие продукты"  };

static wchar_t* string_manugoods[NLANGS] = {
  L"Manual Good products",
  L"人工合格品",
  L"Productos buenos",
  L"@",
  L"@"  };
// yang added 2018/03/30 for machines with manual check 

static wchar_t* string_totrej[NLANGS] = {
  L"Total rejects",
  L"剔除总数",
  L"Rechazos totales",
  L"@",
  L"Отбраковано, всего"  };

static wchar_t* string_1strej[NLANGS] = {
  L"First tray rejects",
  L"托盘1剔除",
  L"Rechazo primera charola",
  L"@",
  L"ОТБР.: лоток 1"  };

static wchar_t* string_2ndrej[NLANGS] = {
  L"Second tray rejects",
  L"托盘2剔除",
  L"Rechazo segunda charola",
  L"@",
  L"ОТБР.: лоток 2"  };

  static wchar_t* string_3rdrej[NLANGS] = {
  L"Third tray rejects",
  L"托盘3剔除",
  L"Rechazo segunda charola",
  L"@",
  L"ОТБР.: лоток 2"  };

static wchar_t* string_3rdrejhga[NLANGS] = {
  L"Third tray rejects (HGA)",
  L"托盘3剔除 (HGA)",
  L"Rechazo terda charola (HGA)",
  L"@ (HGA)",
  L"ОТБР.: лоток 3 (HGA)"  };

static wchar_t* string_4threj[NLANGS] = {
  L"Fourth tray rejects",
  L"托盘4剔除",
  L"Rechazo  charola",
  L"@",
  L"ОТБР.: лоток 2"  };



      //Pour added for xinchang 20171019
  static wchar_t* string_1strej_a[NLANGS] = {
  L"First tray rejects",
  L"不合格品剔除",
  L"Rechazo primera charola",
  L"@",
  L"ОТБР.: лоток 1"  };

static wchar_t* string_2ndrej_b[NLANGS] = {
  L"Second tray rejects",
  L"人工复检通道剔除",
  L"Rechazo segunda charola",
  L"@",
  L"ОТБР.: лоток 2"  };
  //Pour added for xinchang 20171019


static wchar_t* string_caprej[NLANGS] = {
  L"Cap rejects",
  L"头部剔除",
  L"Rechazos tapa",
  L"@",
  L"ОТБР.: колпачок"  };

static wchar_t* string_tiprej[NLANGS] = {
  L"Tip rejects",
  L"头部剔除",
  L"Rechazos punta",
  L"@",
  L"ОТБР.: кончик"  };

  static wchar_t* string_flangerej[NLANGS] = {
  L"Flange rejects",
  L"法兰剔除",
  L"Rechazos tapa",
  L"@",
  L"ОТБР.: колпачок"  };

static wchar_t* string_focolrej[NLANGS] = {
  L"FO color rejects",
  L"易开盖颜色剔除",
  L"Rechazo color FO",
  L"@",
  L"ОТБР.: цвет Flip-off"  };

static wchar_t* string_lqcolrej[NLANGS] = {
  L"Liquid color rejects",
  L"色水剔除",
  L"&",
  L"@",
  L"#"  };

static wchar_t* string_bottarej[NLANGS] = {
  L"Bottom A rejects",
  L"底部相机A剔除",
  L"Rechazos fondo A",
  L"@",
  L"ОТБР.: дно, камера A"  };

static wchar_t* string_bottbrej[NLANGS] = {
  L"Bottom B rejects",
  L"底部相机B剔除",
  L"Rechazos fondo B",
  L"@",
  L"ОТБР.: дно, камера B"  };

static wchar_t* string_sidewlrej[NLANGS] = {
  L"Sidewall rejects",
  L"外观剔除",
  L"Rechazos superficie",
  L"@",
  L"ОТБР.: боковые стенки"  };

static wchar_t* string_sideckrej[NLANGS] = {
  L"Sidecake rejects",
  L"粉饼侧面剔除",
  L"Rechazo lado ",
  L"@",
  L"ОТБР.: боковая ЛИО" };
static wchar_t* string_topckrej[NLANGS] = {
  L"Topcake rejects",
  L"粉饼顶部剔除",
  L"Rechazo por parte superior",
  L"@",
  L"ОТБР.: верхняя ЛИО"  };

static wchar_t* string_heelrej[NLANGS] = {
  L"Heel rejects",
  L"瓶根剔除",
  L"Rechazo base",
  L"@",
  L"ОТБР.: нижняя ЛИО"  };

static wchar_t* string_spinrej[NLANGS] = {
  L"Spin rejects",
  L"旋转不良剔除",
  L"Rechazos giro",
  L"@",
  L"ОТБР.: плохое вращение"  };

static wchar_t* string_fillevrej[NLANGS] = {
  L"Filling level rejects",
  L"装量剔除",
  L"Rechazos nivel llenado",
  L"@",
  L"ОТБР.: уровень "  };


static wchar_t* string_partrej[NLANGS] = {
  L"Particle rejects",
  L"粒子剔除",
  L"Rechazo particula",
  L"@",
  L"ОТБР.: Включения"  };

  static wchar_t* string_num_sampling[NLANGS] = {
  L"Sampling NUM",
  L"抽样数",
  L"Rechazos nivel llenado",
  L"@",
  L"ОТБР.: уровень "  };


static wchar_t* string_part1rej[NLANGS] = {
  L"Particle1 rejects",
  L"粒子1剔除",
  L"Rechazo particula1",
  L"@",
  L"ОТБР.: Включения 1"  };

static wchar_t* string_part2rej[NLANGS] = {
  L"Particle2 rejects",
  L"粒子2剔除",
  L"Rechazo particula2",
  L"@",
  L"ОТБР.: Включения 2"  };

static wchar_t* string_part3rej[NLANGS] = {
  L"Particle3 rejects",
  L"粒子3剔除",
  L"Rechazo particula3",
  L"@",
  L"ОТБР.: Включения 3"  };

static wchar_t* string_cosmrej[NLANGS] = {
  L"Cosmetic rejects",
  L"外观1剔除",
  L"Rechazos cosmeticos",
  L"@",
  L"ОТБР.: косм. дефекты"  };

static wchar_t* string_cosm2rej[NLANGS] = {
  L"Cosmetic2 rejects",
  L"外观2剔除",
  L"Rechazos cosmeticos#",
  L"@",
  L"ОТБР.: косм. дефекты&"  };

static wchar_t* string_cosm3rej[NLANGS] = {
  L"Cosmetic3 rejects",
  L"外观3剔除",
  L"Rechazos cosmeticos#",
  L"@",
  L"ОТБР.: косм. дефекты&"  };

static wchar_t* string_cosm3arej[NLANGS] = {
  L"Cosmetic3 A rejects",
  L"外观剔除3-A",
  L"Rechazos cosmeticos#",
  L"@",
  L"ОТБР.: косм. дефекты&"  };

static wchar_t* string_cosm3brej[NLANGS] = {
  L"Cosmetic3 B rejects",
  L"外观剔除3-B",
  L"Rechazos cosmeticos#",
  L"@",
  L"ОТБР.: косм. дефекты&"  };


static wchar_t* string_wstlyrrej[NLANGS] = {
  L"Waist-layer rejects",
  L"收腰/分层剔除",
  L"Rechazo de cintura",
  L"@",
  L"ОТБР.: усадка/слои"  };

static wchar_t* string_hgarej[NLANGS] = {
  L"HGA rejects",
  L"头部气体分析剔除",
  L"Rechazo HGA",
  L"@",
  L"ОТБР.HGA"  };


    static wchar_t* string_foldrej[NLANGS] = {
  L"Crimping-neck rejects",
  L"翻边/瓶脖剔除",
  L"Rechazo HGA",
  L"@",
  L"ОТБР.HGA"  };



  static wchar_t* string_hgarejb[NLANGS] = {
  L"HGA rejects (Tray3)",
  L"头部气体分析剔除 (Tray3)",
  L"Rechazo HGA (Tray3)",
  L"@ (Tray3)",
  L"ОТБР.HGA (Tray3)"  };

static wchar_t* string_fhgasample[NLANGS] = {
  L"F_HGA_SAMPLE",
  L"F_HGA_样品",
  L"&",
  L"@",
  L"F_HGA_образцы"  };

static wchar_t* string_enrs232com[NLANGS] = {
  L"EN_RS232COM",
  L"EN_RS232通讯",
  L"&",
  L"@",
  L"RS232 коммуникация"  };

static wchar_t* string_hgasampling[NLANGS] = {
  L"HGA_SAMPLING",
  L"HGA_取样",
  L"&",
  L"@",
  L"HGA_Отбор образцов"  };

static wchar_t* string_hgadisplaywave[NLANGS] = {
  L"HGA_DISPLAYWAVE",
  L"HGA_显示波形",
  L"&",
  L"@",
  L"HGA_Дисплей волна"  };

static wchar_t* string_hgawritetype[NLANGS] = {
  L"HGA_WRITETYPE",
  L"HGA_写入类型",
  L"&",
  L"@",
  L"HGA_пиши тип"  };

static wchar_t* string_hgawriteaddress[NLANGS] = {
  L"HGA_WRITEADDRESS",
  L"HGA_写入地址",
  L"&",
  L"@",
  L"HGA_Письменный адрес"  };

static wchar_t* string_hgawritedata[NLANGS] = {
  L"HGA_WRITEDATA",
  L"HGA_写入数据",
  L"&",
  L"@",
  L"HGA_Запись данных"  };

static wchar_t* string_hgawritecmd[NLANGS] = {
  L"HGA_WRITECMD",
  L"HGA_写入命令",
  L"&",
  L"@",
  L"HGA_пиши команду"  };

static wchar_t* string_hgareadtype[NLANGS] = {
  L"HGA_READTYPE",
  L"HGA_读取类型",
  L"&",
  L"@",
  L"HGA_Метод считывания"  };

static wchar_t* string_hgareadaddress[NLANGS] = {
  L"HGA_READADDRESS",
  L"HGA_读取地址",
  L"&",
  L"@",
  L"HGA_прочтите адрес"  };

static wchar_t* string_hgareaddata[NLANGS] = {
  L"HGA_READDATA",
  L"HGA_读取数据",
  L"&",
  L"@",
  L"HGA_читайте данные"  };

static wchar_t* string_hgareadcmd[NLANGS] = {
  L"HGA_READCMD",
  L"HGA_读取命令",
  L"&",
  L"@",
  L"HGA_читайте команды"  };








#ifdef TIVS_A60
  #ifdef FO_COLOR    // If defined camera is searched ad added for flip off color control
    static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects"  };
    static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格产品",       L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",      L"易开盖颜色剔除剔除", L"装量剔除",              L"粒子1剔除",           L"粒子2剔除",          L"外观剔除",         L"旋转不良剔除"   };
static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects"  };
static wchar_t* obj_desc_rus[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"Отбрраковано, всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок", L"ОТБР.: цвет Flip-off",  L"ОТБР.: уровень наполнения", L"ОТБР.: включения 1",  L"ОТБР.: включения 2", L"ОТБР.: косметические дефекты", L"ОТБР.: плохое вращение"   };
  #else // NO FO_COLOR
    #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK // station3 is for particle
      static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Particle3 rejects", L"Spin rejects" };
      static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格产品",       L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",      L"装量剔除",              L"粒子1剔除",           L"粒子2剔除",         L"粒子3剔除",          L"旋转不良剔除"  };
      static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Particle3 rejects", L"Spin rejects"  };
     static wchar_t* obj_desc_rus[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР., всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок", L"ОТБР.: уровень наполнения", L"ОТБР.: включения 1",  L"ОТБР.: включения 2",  L"ОТБР.: косметические дефекты", L"ОТБР.: плохое вращение"   };
    #else // standard cosmetic
      static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects"   };
      static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格产品",       L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",      L"装量剔除",              L"粒子1剔除",           L"粒子2剔除",         L"外观剔除",          L"旋转不良剔除"    };
      static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects"   };
static wchar_t* obj_desc_rus[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок", L"ОТБР.: уровень наполнения", L"ОТБР.: включения 1",  L"ОТБР.: включения 2",  L"ОТБР.: косметические дефекты", L"ОТБР.: плохое вращение" };
    #endif
  #endif
#endif


#ifdef TIVS_A40
  #ifdef FO_COLOR    // If defined camera is searched ad added for flip off color control
//    static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects"  ,
//    static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格产品",        L"剔除总数",       L"托盘1剔除",           L"托盘2剔除",             L"头部剔除",      L"易开盖颜色剔除剔除",   L"装量剔除",                L"粒子1剔除",          L"粒子2剔除",          L"外观剔除",           L"旋转不良剔除"   ,

    #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK // station3 is for particle
      static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Particle3 rejects", L"Spin rejects"  };
      static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格产品",       L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",      L"易开盖剔除",  L"装量剔除",               L"粒子1剔除",          L"粒子2剔除",          L"粒子3剔除",         L"旋转不良剔除"   };
      static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Particle3 rejects", L"Spin rejects"  };
      static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",            L"",                      L"",                   L"",                  L"",                  L""              };
      static wchar_t* obj_desc_002[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: уровень наполнения", L"ОТБР.: включения 1",  L"ОТБР.: включения 2", L"ОТБР.: косметические дефекты", L"ОТБР.: плохое вращение"   };
    #else // standard cosmetic
      static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects"  };
      static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格产品",       L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",     L"易开盖剔除",   L"装量剔除",               L"粒子1剔除",          L"粒子2剔除",          L"外观剔除",         L"旋转不良剔除"   };
      static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects"  };
      static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",            L"",                      L"",                   L"",                  L"",                  L""              };
      static wchar_t* obj_desc_rus[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: уровень наполнения", L"ОТБР.: включения 1",  L"ОТБР.: включения 2", L"ОТБР.: косметические дефекты", L"ОТБР.: плохое вращение"   };
    #endif

  #else

    #if STN3==PARTICLE_BOTTOM || STN3==PARTICLE_BACK // station3 is for particle

      #ifdef DOTRING_COLOR
        static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Particle3 rejects", L"Spin rejects", L"Dot&ring rejects"     };
        static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格产品",       L"剔除总数",       L"托盘1剔除",           L"托盘2剔除",           L"头部剔除",      L"装量剔除",              L"粒子1剔除",           L"粒子2剔除",         L"粒子3剔除",          L"旋转不良剔除", L"色点色环剔除"           };
        static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Particle3 rejects", L"Spin rejects", L"Rechazo anillo color"  };
        static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                      L"",                   L"",                  L"",                  L"",             L""                      };
        static wchar_t* obj_desc_rus[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР.всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: уровень наполнения", L"ОТБР.: включения 1",  L"ОТБР.: включения 2", L"ОТБР.: косметические дефекты", L"ОТБР.: плохое вращение",L"ОТБР.: цветной контур"   };
      #else
        static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Particle3 rejects", L"Spin rejects"  };
        static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格产品",       L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",      L"装量剔除",              L"粒子1剔除",           L"粒子2剔除",          L"粒子3剔除",         L"旋转不良剔除"   };
        static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Particle3 rejects", L"Spin rejects"  };
        static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                      L"",                   L"",                  L"",                  L"",             };
        static wchar_t* obj_desc_rus[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР.всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: уровень наполнения", L"ОТБР.: включения 1",  L"ОТБР.: включения 2", L"ОТБР.: косметические дефекты", L"ОТБР.: плохое вращение"   };
      #endif
  
    #else // standard cosmetic

      #ifdef DOTRING_COLOR
        static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects", L"Dot&ring rejects"       };
        static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格产品",       L"剔除总数",       L"托盘1剔除",           L"托盘2剔除",           L"头部剔除",      L"装量剔除",              L"粒子1剔除",           L"粒子2剔除",         L"外观剔除",          L"旋转不良剔除", L"色点色环剔除"            };
        static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects", L"Rechazo anillo color"   };
        static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                      L"",                   L"",                  L"",                  L"",            L""                       };
        static wchar_t* obj_desc_002[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР.всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: уровень наполнения", L"ОТБР.: включения 1",  L"ОТБР.: включения 2", L"ОТБР.: косметические дефекты", L"ОТБР.: плохое вращение"L"ОТБР.: цветной контур"     };
      #else
        static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects", L"Dot&ring rejects"       };
        static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格产品",       L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",      L"装量剔除",              L"粒子1剔除",           L"粒子2剔除",          L"外观剔除",         L"旋转不良剔除", L"色点色环剔除"            };
        static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Tip rejects",  L"Filling level rejects", L"Particle1 rejects",  L"Particle2 rejects", L"Cosmetic rejects", L"Spin rejects", L"Rechazo anillo color"  };
        static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                      L"",                   L"",                  L"",                  L"",            L""                       };
        static wchar_t* obj_desc_002[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР.всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: уровень наполнения", L"ОТБР.: включения 1",  L"ОТБР.: включения 2", L"ОТБР.: косметические дефекты", L"ОТБР.: плохое вращение"L"ОТБР.: цветной контур"     };
      #endif
    #endif

  #endif
#endif

#ifdef TIVS_F40

  //#if SERIAL_N==201400022 // Yaopharma top-cake changed to HEEL cameras
  #ifdef HEEL_INSPECTION
    static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Heel rejects", L"Spin rejects" };
    static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格品",         L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",     L"易开盖颜色剔除",    L"底部相机A剔除",     L"底部相机B剔除",     L"外观剔除",          L"粉饼侧面剔除",     L"瓶根剔除",     L"旋转不良剔除"   };
    static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Heel rejects", L"Spin rejects" };
    static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                 L"",                 L"",                 L"",                 L"",                 L"",             L""             };
    static wchar_t* obj_desc_002[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: цветной Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: боковые стенки", L"ОТБР.: боковая поверхность ЛИО", L"ОТБР.: нижняя поверхность ЛИО", L"ОТБР.: плохое вращение"    };
  #else  // Normal - Top-cake
    static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
    static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格品",         L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",     L"易开盖颜色剔除",    L"底部相机A剔除",     L"底部相机B剔除",     L"外观剔除",          L"粉饼侧面剔除",     L"粉饼顶部剔除",     L"旋转不良剔除"   };
    static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
    static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                 L"",                 L"",                 L"",                 L"",                 L"",                L""             };
    static wchar_t* obj_desc_002[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: боковые стенки", L"ОТБР.: боковая поверхность ЛИО", L"ОТБР.: верхняя поверхность ЛИО", L"Отбрю: плохое вращение"  };
  #endif

#endif

	#ifdef TIVS_F20

  //#if SERIAL_N==201400022 // Yaopharma top-cake changed to HEEL cameras
  #ifdef HEEL_INSPECTION
    static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Heel rejects", L"Spin rejects" };
    static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格品",         L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",     L"易开盖颜色剔除",    L"底部相机A剔除",     L"底部相机B剔除",     L"外观剔除",          L"粉饼侧面剔除",     L"瓶根剔除",     L"旋转不良剔除"   };
    static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Heel rejects", L"Spin rejects" };
    static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                 L"",                 L"",                 L"",                 L"",                 L"",             L""             };
    static wchar_t* obj_desc_002[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: цветной Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: боковые стенки", L"ОТБР.: боковая поверхность ЛИО", L"ОТБР.: нижняя поверхность ЛИО", L"ОТБР.: плохое вращение"    };
  #else  // Normal - Top-cake
    static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
    static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格品",         L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",     L"易开盖颜色剔除",    L"底部相机A剔除",     L"底部相机B剔除",     L"外观剔除",          L"粉饼侧面剔除",     L"粉饼顶部剔除",     L"旋转不良剔除"   };
    static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
    static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                 L"",                 L"",                 L"",                 L"",                 L"",                L""             };
    static wchar_t* obj_desc_002[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: боковые стенки", L"ОТБР.: боковая поверхность ЛИО", L"ОТБР.: верхняя поверхность ЛИО", L"Отбрю: плохое вращение"  };
  #endif

#endif


#ifdef TIVS_C20
//  char* ADS_objs[] =               { ".NUM_PROD_IN",      ".NUM_GOODS",    ".NUM_REJECTS",  ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ",

  static wchar_t* obj_desc_eng[] = { L"Products entered",    L"Good products",     L"Total rejects",   L"Cap rejects",              L"FO color rejects",     L"Bottom A rejects",     L"Bottom B rejects",      L"Particle2/Topcake rej",                             L"Particle1/Sidewall rej",                  L"Sidecake rejects",                       L"Filling level rejects",        L"Spin rejects"     };
  static wchar_t* obj_desc_chn[] = { L"进料产品",             L"合格品",            L"剔除总数",         L"头部剔除",                 L"易开盖颜色剔除剔除",     L"底部相机A剔除",         L"底部相机B剔除",         L"粒子2/粉饼顶部剔除",                                  L"粒子1/外观剔除",                          L"粉饼侧面剔除",                            L"液位剔除",                      L"旋转不良剔除"     };
  static wchar_t* obj_desc_esp[] = { L"Productos ingresados", L"Productos buenos", L"Total rechazados", L"Rechazos por precintos",  L"Rechazos por color FO", L"Rechazos por fondo A", L"Rechazos por fondo B", L"Rech. partículas2/Parte sup Liof.", L"Rech. por partículas1/Pared lat", L"Rech. por pared lateral liof", L"Rechazo por nivel de llenado", L"Rechazo por giro"                              };
  static wchar_t* obj_desc_001[] = { L"",                     L"",               L"",                   L"",                        L"",                      L"",                     L"",                     L"",                                  L"",              L"",              L"",                    L"",                                      L"",                             L""             };
  static wchar_t* obj_desc_002[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: включения 2/верхняя  ЛИО",  L"ОТБР.: включения 1/Боковые стенки", L"ОТБР.: боковая поверхность ЛИО",  L"ОТБР.: уровень наполнения", L"ОТБР.: плохое вращение"    };

#endif


#ifdef TIVS_S40
  static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
  static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格品",         L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",     L"易开盖颜色剔除",    L"底部相机A剔除",     L"底部相机B剔除",     L"外观剔除",          L"粉饼侧面剔除",     L"粉饼顶部剔除",     L"旋转不良剔除"   };
  static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
  static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                 L"",                 L"",                 L"",                 L"",                 L"",                L""             };
  static wchar_t* obj_desc_002[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: боковые стенки", L"ОТБР.: боковая поверхность ЛИО", L"ОТБР.: верхняя поверхность ЛИО", L"Отбрю: плохое вращение"  };
#endif


  #ifdef TIVS_S60
  static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
  static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格品",         L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",     L"易开盖颜色剔除",    L"底部相机A剔除",     L"底部相机B剔除",     L"外观剔除",          L"粉饼侧面剔除",     L"粉饼顶部剔除",     L"旋转不良剔除"   };
  static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
  static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                 L"",                 L"",                 L"",                 L"",                 L"",                L""             };
  static wchar_t* obj_desc_002[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: боковые стенки", L"ОТБР.: боковая поверхность ЛИО", L"ОТБР.: верхняя поверхность ЛИО", L"Отбрю: плохое вращение"  };
#endif


#ifdef TIVS_C40

#ifdef HEEL_INSPECTION

  static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
  static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格品",         L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",     L"易开盖颜色剔除",    L"底部相机A剔除",     L"底部相机B剔除",     L"外观剔除",          L"粉饼侧面剔除",     L"粉饼顶部剔除",     L"旋转不良剔除"   };
  static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
  static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                 L"",                 L"",                 L"",                 L"",                 L"",                L""             };
  static wchar_t* obj_desc_002[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: боковые стенки", L"ОТБР.: боковая поверхность ЛИО", L"ОТБР.: верхняя поверхность ЛИО", L"Отбрю: плохое вращение"  };
#else
  static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
  static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格品",         L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",     L"易开盖颜色剔除",    L"底部相机A剔除",     L"底部相机B剔除",     L"外观剔除",          L"粉饼侧面剔除",     L"粉饼顶部剔除",     L"旋转不良剔除"   };
  static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
  static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                 L"",                 L"",                 L"",                 L"",                 L"",                L""             };
  static wchar_t* obj_desc_002[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: боковые стенки", L"ОТБР.: боковая поверхность ЛИО", L"ОТБР.: верхняя поверхность ЛИО", L"Отбрю: плохое вращение"  };

#endif

#endif



#ifdef TIVS_C60
  static wchar_t* obj_desc_eng[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
  static wchar_t* obj_desc_chn[] = { L"进料产品",          L"合格品",         L"剔除总数",       L"托盘1剔除",          L"托盘2剔除",            L"头部剔除",     L"易开盖颜色剔除",    L"底部相机A剔除",     L"底部相机B剔除",     L"外观剔除",          L"粉饼侧面剔除",     L"粉饼顶部剔除",     L"旋转不良剔除"   };
  static wchar_t* obj_desc_esp[] = { L"Products entered", L"Good products",  L"Total rejects", L"First tray rejects", L"Second tray rejects", L"Cap rejects",  L"FO color rejects", L"Bottom A rejects", L"Bottom B rejects", L"Sidewall rejects", L"Sidecake rejects", L"Topcake rejects", L"Spin rejects" };
  static wchar_t* obj_desc_001[] = { L"",                 L"",               L"",              L"",                   L"",                    L"",             L"",                 L"",                 L"",                 L"",                 L"",                 L"",                L""             };
  static wchar_t* obj_desc_002[] = { L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: лоток 1", L"ОТБР.: лоток 2", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: боковые стенки", L"ОТБР.: боковая поверхность ЛИО", L"ОТБР.: верхняя поверхность ЛИО", L"Отбрю: плохое вращение"  };
#endif



#ifdef TIVS_A20L
//  char* ADS_objs[] =               { ".NUM_PROD_IN",      ".NUM_GOODS",    ".NUM_REJECTS",  ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ",

  static wchar_t* obj_desc_eng[] = { L"Products entered",    L"Good products",     L"Total rejects",   L"Cap rejects",              L"FO color rejects",     L"Bottom A rejects",     L"Bottom B rejects",      L"Particle2/Topcake rej",                             L"Particle1/Sidewall rej",                  L"Sidecake rejects",                       L"Filling level rejects",        L"Spin rejects"     };
  static wchar_t* obj_desc_chn[] = { L"进料产品",             L"合格品",            L"剔除总数",         L"头部剔除",                 L"易开盖颜色剔除剔除",     L"底部相机A剔除",         L"底部相机B剔除",         L"粒子2/粉饼顶部剔除",                                  L"粒子1/外观剔除",                          L"粉饼侧面剔除",                            L"液位剔除",                      L"旋转不良剔除"     };
  static wchar_t* obj_desc_esp[] = { L"Productos ingresados", L"Productos buenos", L"Total rechazados", L"Rechazos por precintos",  L"Rechazos por color FO", L"Rechazos por fondo A", L"Rechazos por fondo B", L"Rech. partículas2/Parte sup Liof.", L"Rech. por partículas1/Pared lat", L"Rech. por pared lateral liof", L"Rechazo por nivel de llenado", L"Rechazo por giro"                              };
  static wchar_t* obj_desc_001[] = { L"",                     L"",               L"",                   L"",                        L"",                      L"",                     L"",                     L"",                                  L"",              L"",              L"",                    L"",                                      L"",                             L""             };
  static wchar_t* obj_desc_002[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: включения 2/верхняя  ЛИО",  L"ОТБР.: включения 1/Боковые стенки", L"ОТБР.: боковая поверхность ЛИО",  L"ОТБР.: уровень наполнения", L"ОТБР.: плохое вращение"    };

#endif

//Pour added 20180523
  #ifdef TIVS_A20
//  char* ADS_objs[] =               { ".NUM_PROD_IN",      ".NUM_GOODS",    ".NUM_REJECTS",  ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ",

  static wchar_t* obj_desc_eng[] = { L"Products entered",    L"Good products",     L"Total rejects",   L"Cap rejects",              L"FO color rejects",     L"Bottom A rejects",     L"Bottom B rejects",      L"Particle2/Topcake rej",                             L"Particle1/Sidewall rej",                  L"Sidecake rejects",                       L"Filling level rejects",        L"Spin rejects"     };
  static wchar_t* obj_desc_chn[] = { L"进料产品",             L"合格品",            L"剔除总数",         L"头部剔除",                 L"易开盖颜色剔除剔除",     L"底部相机A剔除",         L"底部相机B剔除",         L"粒子2/粉饼顶部剔除",                                  L"粒子1/外观剔除",                          L"粉饼侧面剔除",                            L"液位剔除",                      L"旋转不良剔除"     };
  static wchar_t* obj_desc_esp[] = { L"Productos ingresados", L"Productos buenos", L"Total rechazados", L"Rechazos por precintos",  L"Rechazos por color FO", L"Rechazos por fondo A", L"Rechazos por fondo B", L"Rech. partículas2/Parte sup Liof.", L"Rech. por partículas1/Pared lat", L"Rech. por pared lateral liof", L"Rechazo por nivel de llenado", L"Rechazo por giro"                              };
  static wchar_t* obj_desc_001[] = { L"",                     L"",               L"",                   L"",                        L"",                      L"",                     L"",                     L"",                                  L"",              L"",              L"",                    L"",                                      L"",                             L""             };
  static wchar_t* obj_desc_002[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: включения 2/верхняя  ЛИО",  L"ОТБР.: включения 1/Боковые стенки", L"ОТБР.: боковая поверхность ЛИО",  L"ОТБР.: уровень наполнения", L"ОТБР.: плохое вращение"    };

#endif
//Pour added 20180523


//Pour added 20180124
  #ifdef TIVS_A40L
//  char* ADS_objs[] =               { ".NUM_PROD_IN",      ".NUM_GOODS",    ".NUM_REJECTS",  ".NUM_CAPSEAL_REJ", ".NUM_FPOFFCOL_REJ", ".NUM_BOTTOMA_REJ", ".NUM_BOTTOMB_REJ",  ".NUM_COSMETIC_REJ", ".NUM_PARTICLE1_REJ", ".NUM_PARTICLE2_REJ", ".NUM_SPIN_REJ",

  static wchar_t* obj_desc_eng[] = { L"Products entered",    L"Good products",     L"Total rejects",   L"Cap rejects",              L"FO color rejects",     L"Bottom A rejects",     L"Bottom B rejects",      L"Particle2/Topcake rej",                             L"Particle1/Sidewall rej",                  L"Sidecake rejects",                       L"Filling level rejects",        L"Spin rejects"     };
  static wchar_t* obj_desc_chn[] = { L"进料产品",             L"合格品",            L"剔除总数",         L"头部剔除",                 L"易开盖颜色剔除剔除",     L"底部相机A剔除",         L"底部相机B剔除",         L"粒子2/粉饼顶部剔除",                                  L"粒子1/外观剔除",                          L"粉饼侧面剔除",                            L"液位剔除",                      L"旋转不良剔除"     };
  static wchar_t* obj_desc_esp[] = { L"Productos ingresados", L"Productos buenos", L"Total rechazados", L"Rechazos por precintos",  L"Rechazos por color FO", L"Rechazos por fondo A", L"Rechazos por fondo B", L"Rech. partículas2/Parte sup Liof.", L"Rech. por partículas1/Pared lat", L"Rech. por pared lateral liof", L"Rechazo por nivel de llenado", L"Rechazo por giro"                              };
  static wchar_t* obj_desc_001[] = { L"",                     L"",               L"",                   L"",                        L"",                      L"",                     L"",                     L"",                                  L"",              L"",              L"",                    L"",                                      L"",                             L""             };
  static wchar_t* obj_desc_002[] = {  L"Контейнеров загружено", L"Пригодный продукт",  L"ОТБР. всего", L"ОТБР.: колпачок",  L"ОТБР.: цвет Flip-off", L"ОТБР.: дно, камера А",  L"ОТБР.: дно, камера В", L"ОТБР.: включения 2/верхняя  ЛИО",  L"ОТБР.: включения 1/Боковые стенки", L"ОТБР.: боковая поверхность ЛИО",  L"ОТБР.: уровень наполнения", L"ОТБР.: плохое вращение"    };

#endif
//Pour added 20180124


#ifdef FILLEV
  static wchar_t* obj_desc_eng[] = { L"Products entered" };
  static wchar_t* obj_desc_chn[] = { L"进料产品"          };
  static wchar_t* obj_desc_rus[] = { L"Контейнеров загружено"          };
#endif

#ifdef STOPPER_CTRL
  static wchar_t* obj_desc_eng[] = { L"Products entered" };
  static wchar_t* obj_desc_chn[] = { L"进料产品"          };
  static wchar_t* obj_desc_rus[] = { L"Контейнеров загружено"        };
#endif


// Text shown after pressing TOFFLON button on top/left corner of program interface
static wchar_t* info_text_eng[] = { 
    L"Machine model %s number %s",
    L"Inspection Software version %s",
    L"PLC Software version %s",
    L"Shanghai Tofflon Science and Technology Co., Ltd.",
    L"1509, Duhui Road",
    L"Shanghai, China, 201108",
    L"Tel. +86 21 64901123/64906201",
    L"Fax. +86 21 64905148/64906202",
    L"Website: www.tofflon.com",
    L"Email: TIVS@tofflon.com"  };


static wchar_t* info_text_chn[] = { 
    L"机器型号 %s 序列号 %s",
    L"检测软件版本 %s",
    L"PLC软件版本 %s",
    L"上海东富龙科技股份有限公司",
    L"都会路1509号",
    L"中国上海 201108",
    L"电话: +86 21 64901123/64906201",
    L"传真: +86 21 64905148/64906202",
    L"网址: www.tofflon.com",
    L"邮箱: TIVS@tofflon.com"   };


static wchar_t* info_text_esp[] = { 
    L"Machine model %s number %s",
    L"Inspection Software version %s",
    L"PLC Software version %s",
    L"Shanghai Tofflon Science and Technology Co., Ltd.",
    L"1509, Duhui Road",
    L"Shanghai, China, 201108",
    L"Tel. +86 21 64901123/64906201",
    L"Fax. +86 21 64905148/64906202",
    L"Website: www.tofflon.com",
    L"Email: TIVS@tofflon.com"   };

static wchar_t* info_text_001[] = { 
    L"Machine model %s number %s",
    L"Inspection Software version %s",
    L"PLC Software version %s",
    L"Shanghai Tofflon Science and Technology Co., Ltd.",
    L"1509, Duhui Road",
    L"Shanghai, China, 201108",
    L"Tel. +86 21 64901123/64906201",
    L"Fax. +86 21 64905148/64906202",
    L"Website: www.tofflon.com",
    L"Email: TIVS@tofflon.com"   };

static wchar_t* info_text_002[] = { 
    L"Machine model %s number %s",
    L"Inspection Software version %s",
    L"PLC Software version %s",
    L"Shanghai Tofflon Science and Technology Co., Ltd.",
    L"1509, Duhui Road",
    L"Shanghai, China, 201108",
    L"Tel. +86 21 64901123/64906201",
    L"Fax. +86 21 64905148/64906202",
    L"Website: www.tofflon.com",
    L"Email: TIVS@tofflon.com"   };


	static wchar_t* info_accessinfo_chn1[NLANGS] = { 
  L"ADMIN",
  L"所有权限",
  L"#",
  L"@",
  L"&" };

  	static wchar_t* info_accessinfo_chn2[NLANGS] = { 
  L"MA",
  L"可操作机械参数，检测参数，操作批次，配方，公用工程",
  L"#",
  L"@",
  L"&" };


  static wchar_t* info_accessinfo_chn3[NLANGS] = { 
  L"OP",
  L"可操作批次，开始/停止检测",
  L"#",
  L"@",
  L"&" };



//#define ALARM_TXT_NUM     58
#define ALARM_TXT_NUM     67

static wchar_t* alarm_msg[NLANGS*ALARM_TXT_NUM] = { 
/*   0  */              L"No pending alarms",                       
                        L"无警报",                                                      
                        L"No alarmas pendientes",                                                 
                        L"",
                        L"Нет отложенных тревог", // 000
/*   1  */              L"SerialLineControl: Error n.%d",         
                        L"串口线控制: 错误 %d",                                             
                        L"Control lineal serial:ERROR %d",  
                        L"",
                        L"Серийный линейный контроль: Ошибка %d ",  // 001
/*   2  */              L"Spinmotors communication error. No more retries available!", 
                        L"旋转电机通讯错误。不能再重试",                                        
                        L"Error comunicacion Spinmotors. No mas intentos disponibles",
                        L"",
                        L"Ошибка связи Spinmotors. Попыток больше нет", // 002
/*   3  */              L"Spinmotors comm error n.%d writing to motor n.%d",               
                        L"旋转电机通讯错误 %d 反馈给 %d 号电机",                                
                        L"Error comu Spinmotors n.%d procesando motor n.%d",
                        L"",
                        L"Ошибка связи Spinmotors %d Обратная связь с мотором %d", // 003
/*   4  */              L"Spinmotors communication error %d on motor %d",                  
                        L"%d号电机，旋转电机通讯错误 %d",                                      
                        L"Error comunicacion Spinmotors %d sobre motor %d",
                        L"",
                        L"%d батареи, Ошибка связи Spinmotors %d ", // 004
/*   5  */              L"Spinmotors comm error reading from motor n.%d",      
                        L"从%d号电机读取旋转电机通讯错误 %d",                                  
                        L"Error comu Spinmotors lectura del motor %d",
                        L"",
                        L"Ошибка связи Spinmotors, %d батарея ", // 005
/*   6  */              L"Bad reading from carousel height position sensor (%.1lf)",   
                        L"不能读取主轴高度 %.1lf",                                         
                        L"Mala lectura del sensor de altura del carrusel %.1lf",
                        L"",
                        L"Ошбика чтения сенсора высоты башни %.1lf ", // 006
/*   7  */              L"Out of allowed range carousel height (%.1lf)",              
                        L"主轴高度超过允许范围 %.1lf",                                    
                        L"Altura de carrusel fuera del rango %.1lf",
                        L"",
                        L"За пределами допустимого диапазона высоты башни", // 007
/*   8  */              L"Carousel DOWNWARD instead of UPWARD: check power phases",    
                        L"主轴向下而非向上: 检查电源相位",                                  
                        L"Carrusel hacia abajo en vez de hacia arriba: revise fases de energia",
                        L"",
                        L"Башня двигается вниз, а не вверх: проверьте фазу тока", // 008
/*   9  */              L"Carousel UPWARD instead of DOWNWARD: check power phases",   
                        L"主轴向上而非向下: 检查电源相位",                                
                        L"Carrusel hacia arriba en vez de hacia abajo: revise fases de energia",
                        L"",
                        L"Башня двигается вверх, а не вниз: проверьте фазу тока", // 009
/*  10  */              L"",                                                       
                        L"",                                                             
                        L"",
                        L"",
                        L"", // 010
/*  11  */              L"Emergency button depressed",                             
                        L"急停按钮被按下",                                                 
                        L"Libere el boton de emergencia",
                        L"",
                        L"Нажата аварийная кнопка", // 011
/*  12  */              L"Door n.%d open",                                          
                        L"%d门被打开",                                                
                        L"Puerta %d abierta %d",
                        L"",
                        L"Дверь %d открыта ", // 012
/*  13  */              L"Carousel drive error n.%d/%d",                            
                        L"主轴驱动器错误 %d/%d",                                      
                        L"Error en unidad de carrusel%d%d",
                        L"",
                        L"Ошибка привода башни %d%d", // 013
/*  14  */              L"Input screw drive error n.%d/%d",                         
                        L"进料螺杆驱动器错误 %d/%d",                                   
                        L"Error en unidad de husillo de entrada %d/%d",
                        L"",
                        L"Ошибка привода подающего шнека %d/%d ", // 014
/*  15  */              L"Tracker drive error n.%d/%d",                           
                        L"跟随驱动器错误 %d/%d",                                       
                        L"Error en unidad de seguimiento %d/%d",
                        L"",
                        L"Ошибка привода трекера %d/%d ", // 015
/*  16  */              L"Extra-travel for tracker motor",                        
                        L"跟随电机摆动角度超出范围",                                     
                        L"Movimiento extra para rastreador de motor",
                        L"",
                        L"Угол поворота трекера за пределами допустимого диапазона", // 016
/*  17  */              L"Missing container at photosensor %s",           
                        L"%s光电传感器处缺瓶",                                          
                        L"Contenedor faltante en fotosensor %s",
                        L"",
                        L"Отсутствует контейнер перед сенсором %s ", // 017
/*  18  */              L"Too wide window at photosensor %s",              
                        L"%s光电传感器处检测宽度太大",                                   
                        L"Ventana demasiada ancha para fotosensor %s",
                        L"",
                        L"Значение ширины обнаружения фотодатчика %s  велико", // 018
/*  19  */              L"Too narrow window at photosensor %s",            
                        L"%s光电传感器处检测宽度太窄",                                    
                        L"Ventada demasiado estrecha para fotosensor %s",
                        L"",
                        L"Значение ширины обнаружения фотодатчика %s  низкое", // 019
/*  20  */              L"Unknown container at photosensor %s",            
                        L"%s光电传感器处检测到未知瓶子",                                  
                        L"Contenedor desconocido en fotosensor %s",
                        L"",
                        L"Неизвестный контейнер перед фотодатчиком %s ", // 020
/*  21  */              L"Low compressed air pressure - Check sensor",     
                        L"压缩空气压力低-检查传感器",                                     
                        L"Presion baja de aire comprimido - revise sensor",
                        L"",
                        L"Низкое давление сжатого воздуха - Проверьте датчик", // 021
/*  22  */              L"Low cleaning air pressure - Check sensor",       
                        L"清扫气压低 - 检查传感器",                                       
                        L"Presion baja de aire de limpieza - revise sensor",
                        L"",
                        L"Низкое давление воздуха очистки - Проверьте датчик", // 022
/*  23  */              L"Low vacuum pressure - Check sensor",             
                        L"真空压力低 - 检查传感器",                                        
                        L"Presion de vacio baja - revise sensor",
                        L"",
                        L"Низкое давление вакуума - Проверьте датчик", // 023
/*  24  */              L"Low vacuum pressure - Check sensor",              
                        L"真空压力低 - 检查传感器",                                      
                        L"Presion de vacio baja -  revise sensor",
                        L"",
                        L"Низкое давление вакуума - Проверьте датчик", // 024
/*  25  */              L"Low vacuum pressure - Check sensor",        
                        L"真空压力低 - 检查传感器",                                      
                        L"Presion de vacio baja -  revise sensor",
                        L"",
                        L"Низкое давление вакуума - Проверьте датчик", // 025
/*  26  */              L"Gripper n.%d opening not detected",         
                        L"没检测到%d夹爪打开",                                           
                        L"Pinzas...apertura no detectada %d",
                        L"",
                        L"Захват %d не открылся ", // 026
/*  27  */              L"Gripper n.%d closing not detected",         
                        L"没检测到%d夹爪闭合",                                          
                        L"Pinzas...clausura no detectada %d",
                        L"",
                        L"Захват %d не закрылся ", // 027
/*  28  */              L"Low vacuum pressure - Check sensor",  
                        L"真空压力低 - 检查传感器",                                         
                        L"Presion de vacio baja -  revise sensor",
                        L"",
                        L"Низкое давление вакуума - Проверьте датчик", // 028
/*  29  */              L"Low vacuum pressure - Check sensor",        
                        L"真空压力低 - 检查传感器",                                        
                        L"Presion de vacio baja - revise sensor",
                        L"",
                        L"Низкое давление вакуума - Проверьте датчик", // 029
/*  30  */              L"Carousel 2nd feedback error n.%d/%d",       
                        L"主轴第二反馈错误 %d/%d",                                    
                        L"Error de retroalimentacion carrusel 2do %d/%d",
                        L"",
                        L"2ая ошибка обратной связи с башней %d/%d ", // 030
/*  31  */              L"n.%d Illuminator fault",                     
                        L"n.%d 号光源警报",                                              
                        L"Falla iluminacion %d",
                        L"",
                        L"n.%d неисправность освещения", // 031
/*  32  */              L"Tracker breaker OFF",                           
                        L"跟随断路器断电",                                             
                        L"Interruptor de rastreador apagado",
                        L"",
                        L"ВЫКЛ.ючатель трекера ОТКЛ ", // 032
/*  33  */              L"Carousel breaker OFF",                           
                        L"主轴断路器断电",                                              
                        L"Interruptor de carrusel apagado",
                        L"",
                        L"ВЫКЛ.ючатель башни ОТКЛ ", // 033
/*  34  */              L"Input screw breaker OFF",                      
                        L"进料螺杆断路器断电",                                            
                        L"Interruptor de husillo de entrada apagado",
                        L"",
                        L"ВЫКЛ.ючатель входного шнека ОТКЛ ", // 034
/*  35  */              L"Input belt breaker OFF",                        
                        L"进料网带断路器断电",                                           
                        L"Interruptor de banda de entrada apagado",
                        L"",
                        L"ВЫКЛ.ючатель загрузочного транспортёра ОТКЛ ", // 035
/*  36  */              L"Output belt breaker OFF",                        
                        L"出料网带断路器断电",                                            
                        L"Interruptor de banda de salida apagado",
                        L"",
                        L"ВЫКЛ.ючатель транспортёра выгрузки ОТКЛ ", // 036
/*  37  */              L"Pre-input belt breaker OFF",                      
                        L"进料网带前网带断路器断电",                                       
                        L"Interruptor de banda de pre-entrada apagado",
                        L"",
                        L"ВЫКЛ.ючатель передного траспортёра ОТКЛ ", // 037
/*  38  */              L"Back belt breaker OFF",                            
                        L"运输网带断路器断电",                                          
                        L"Interruptor de banda de regreso apagado",
                        L"",
                        L"ВЫКЛ.ючательзаднего траспортёра ОТКЛ ", // 038
/*  39  */              L"Rotary table breaker OFF",                          
                        L"转盘断路器断电",                                              
                        L"Interruptor de mesa rotatoria apagado",
                        L"",
                        L"ВЫКЛ.ючатель поворотного стола ОТКЛ ", // 039
/*  40  */              L"Fallen container",                                    
                        L"倒瓶",                                                      
                        L"Contenedor caido",
                        L"",
                        L"Упавший контейнер", // 040
/*  41  */              L"Spinmotor error on motor n.%d",                              
                        L"%d号步进电机错误",                                         
                        L"Error Spinmotor en motor %d",
                        L"",
                        L"Ошибка Spinmotor в приводе %d ", // 041
/*  42  */              L"Input starwheel drive error n.%d/%d",              
                        L"进料星轮驱动器错误 %d/%d",                                  
                        L"Error en rueda de estrella de entrada %d/%d",
                        L"",
                        L"Ошибка привода загрузочного колеса-звёздочки %d/%d ", // 042
/*  43  */              L"Input starwheel breaker OFF",                      
                        L"进料星轮断路器断开",                                          
                        L"Interruptor de rueda de estrella apagado",
                        L"",
                        L"ВЫКЛ.ючатель загрузочного колеса-звёздочки ОТКЛ ", // 043
/*  44  */              L"Input sensor close to CAM change - Change its position",   
                        L"进料传感器靠近CAM原点-改变其位置",                              
                        L"Sensor de entrada cerrado para cambio de CAM - cambie su posicion",
                        L"",
                        L"Датчик загрузки близко к камере - измените позицию датчика", // 044
/*  45  */              L"Broken/empty container at particle1",              
                        L"粒子1检测发现碎/空瓶",                                       
                        L"Contenedor roto/vacio en particula1",
                        L"",
                        L"Поломанный / пустой контейнер на инспекции на включение 1", // 045
/*  46  */              L"Input screw safety switch OPEN",
                        L"进料螺杆安全开关打开",
                        L"",
                        L"",
                        L"Предохранительный ВЫКЛ.ючатель подающего шнека открыт",     // 046
/*  47  */              L"HV leak test ALARM",
                        L"高压检漏警报",
                        L"",
                        L"",
                        L"Сигнал тревоги - проверка утечки высокого давления",        // 047
/*  48  */              L"HV transport error %d/%d",
                        L"高压放电运输报错 %d/%d",
                        L"# %d/%d",
                        L"@ %d/%d",
                        L"& %d/%d",        // 048
/*  49  */              L"HV spin breaker OFF",
                        L"高压放电旋转断路器断开",
                        L"#",
                        L"@",
                        L"&",        // 049
/*  50  */              L"HV suction pump breaker OFF",
                        L"高压放电吸气泵断路器断开",
                        L"#",
                        L"@",
                        L"&",        // 050
/*  51  */              L"HV transport motor breaker OFF",
                        L"高压放电运输电机断路器断开",
                        L"#",
                        L"@",
                        L"&",        // 051
/*  52  */              L"HV power supply breaker OFF",
                        L"高压电源断路器断开",
                        L"#",
                        L"@",
                        L"&",        // 052
/*  53  */              L"HV Door n.%d open",
                        L"高压检测门 %d 打开",
                        L"# %d",
                        L"@ %d",
                        L"& %d",     // 053
/*  54  */              L"Fault on HV module n.%d",
                        L"高压模块 %d 错误",
                        L"# %d",
                        L"@ %d",
                        L"& %d",     // 054
/*  55  */              L"HV Sorting motor breaker OFF",
                        L"高压分拣电机断路器断开",
                        L"# %d",
                        L"@ %d",
                        L"& %d",     // 055
/*  56  */              L"HV continuous rejects",
                        L"高压电源连续剔除",
                        L"# %d",
                        L"@ %d",
                        L"& %d",     // 056
/*  57  */              L"Camera n.%d communication error",
                        L"相机%d通讯错误",
                        L"# %d",
                        L"@ %d",
                        L"& %d",       // 057
/*  58  */              L"Gripper zero position alarm",
                        L"夹爪原点位置警报",
                        L"# %d",
                        L"@ %d",
                        L"& %d",       // 058
/*  59  */              L"Gripper closing alarm",
                        L"夹爪关闭警报",
                        L"# %d",
                        L"@ %d",
                        L"& %d",       // 059
/*  60  */              L"Output screw drive 1 error",
                        L"出料螺杆一驱动器错误",
                        L"# %d",
                        L"@ %d",
                        L"& %d",       // 060
/*  61  */              L"Output screw drive 2 error",
                        L"出料螺杆二驱动器错误",
                        L"# %d",
                        L"@ %d",
                        L"& %d",       // 061
/*  62  */              L"Denest XTS reset failure",
                        L"脱巢XTS复位失败",
                        L"# %d",
                        L"@ %d",
                        L"& %d",       // 062
/*  63  */              L"Renest XTS reset failure",
                        L"回巢XTS复位失败",
                        L"# %d",
                        L"@ %d",
                        L"& %d",       // 063
/*  64  */              L"Denest XTS fault",
                        L"脱巢XTS错误",
                        L"# %d",
                        L"@ %d",
                        L"& %d",       // 064
/*  65  */              L"Renest XTS fault",
                        L"脱巢XTS错误",
                        L"# %d",
                        L"@ %d",
                        L"& %d",       // 065
/*  66  */              L"Renest lost synchronization signal",
                        L"回巢同步信号丢失",
                        L"# %d",
                        L"@ %d",
                        L"& %d"       // 066
                        }; 



/*
#define ENGLISH   0
#define CHINESE   1   // ^
#define SPANISH   2   // #
#define TURKISH   3   // @
#define RUSSIAN   4   // &
*/

//#define WARNING_TXT_NUM   28
#define WARNING_TXT_NUM   59

static wchar_t* warning_msg[NLANGS*WARNING_TXT_NUM] = { 
/*   0  */              L"No pending warnings",                                           
                        L"无警告",   
                        L"No alarmas pendientes", 
                        L"", 
                        L"Нет предупреждений", // 000
/*   1  */              L"Carousel required to go UP to %.1lfmm (%.1lf). RUN to start",   
                        L"要求主轴升高到 %.1lf %.1lf. 按运行按钮开始",  
                        L"Carrusel solicitado para ir arriba %.1lf %.1lf ARRANQUE para comenzar",
                        L"", 
                        L"Необходимо поднять башню на %.1lfmm (%.1lf). Нажмите RUN для начала работы", // 001
/*   2  */              L"Carousel required to go DOWN to %.1lfmm (%.1lf). RUN to start", 
                        L"要求主轴降到 %.1lf %.1lf. 按运行按钮开始",    
                        L"Carrusel solicitado para ir abajo %.1lf %.1lf ARRANQUE para comenzar",
                        L"", 
                        L"Необходимо опустить башню на %.1lfmm (%.1lf). Нажмите RUN для начала работы", // 002
/*   3  */              L"Carousel moving UP to required height %.1lfmm (%.1lf)",         
                        L"主轴上升到指定高度 %.1lf %.1lf",             
                        L"Carrusel hacia arriba a la altura requerida %.1lf %.1lf",                                             
                        L"", 
                        L"Башня поднимается до заданной высоты %.1lfmm (%.1lf)", // 003
/*   4  */              L"Carousel moving DOWN to required height %.1lfmm (%.1lf)",       
                        L"主轴下降到指定高度 %.1lf %.1lf",            
                        L"Carrusel hacia abajo a la altura requerida %.1lf %.1lf",                                           
                        L"", 
                        L"Башня опускается до заданной высоты %.1lfmm (%.1lf)", // 004
/*   5  */              L"Carousel height adjustment stopped by operator",               
                        L"操作人停止主轴高度调整",                    
                        L"Ajuste de altura del carrusel detenida por el operador",                                               
                        L"", 
                        L"Корректировка высоты башни была остановлена оператором", // 005
/*   6  */              L"Carousel required height achieved (%.1lf)",                    
                        L"达到要求的主轴高度 %.1lf",                   
                        L"Altura requerida por el carrusel lograda %.1lf",                                                   
                        L"", 
                        L"Достигнута необходимая высота башни(%.1lf)", // 006
/*   7  */              L"Axis homing procedure requested. Push STOP+RUN to start",       
                        L"需要轴归零步骤。按 停止+运行 按钮开始",      
                        L"Procedimiento de eje inicial solicitado. Presione PARO+ARRANQUE para comenzar",                       
                        L"", 
                        L"Чтобы вернуть башню в исходное положение, нажмите STOP+RUN ", // 007
/*   8  */              L"Axis homing procedure in progress...",                         
                        L"轴归零中。。。",  
                        L"Procedimiento de eje inicial en progreso",          
                        L"", 
                        L"Башня возвращается в исходное положение…", // 008
/*   9  */              L"Axis homing procedure completed successfully",                  
                        L"轴归零步骤完成",           
                        L"Procedimiento de eje inicial completado satisfactoriamente", 
                        L"",
                        L"Башня успешно вернулась в исходное положение", // 009
/*  10  */              L"Machine ready to run (AUTOMATIC MODE)",                         
                        L"机器准备就绪（自动模式）",                   
                        L"Maquina lista para comenzar (MODO AUTOMATICO)",                                                        
                        L"", 
                        L"Машина готова к запуску (автоматический режим)", // 010
/*  11  */              L"Machine ready to run (MANUAL MODE)",                           
                        L"机器准备就绪（手动模式）",                   
                        L"Maquina lista para comenzar (MODO MANUAL)",                                                      
                        L"", 
                        L"Машина готова к запуску (ручной режим)", // 011
/*  12  */              L"Machine idle",                                            
                        L"机器空置",                          
                        L"Maquina idle",                                                  
                        L"", 
                        L"Машина в режиме ожидания", // 012
/*  13  */              L"Machine running at %d pcm",                              
                        L"机器运行速度 %d pcm",      
                        L"Maquina corriendo en %d pcm",                                             
                        L"", 
                        L"Машина работает %d в месяц ", // 013
/*  14  */              L"Machine stopping",                                     
                        L"机器停止",              
                        L"Maquina deteniendose",       
                        L"", 
                        L"Остановка машины", // 014
/*  15  */              L"Goods output full/missing - please check it",            
                        L"合格品托盘已满/缺失 - 请检查",            
                        L"Salida de correctos llenos/perdidos -  por favor revise",      
                        L"", 
                        L"Станция выгрузки заполнена / отсутствует - пожалуйста, проверьте", // 015
/*  16  */              L"STANDBY waiting for goods output",                     
                        L"待机等待合格品托盘放入",      
                        L"En espera para salida de correctos",   
                        L"", 
                        L"Машина в режиме ожидания, происходит выгрузка продукта", // 016
/*  17  */              L"Rejects 1 output full/missing - please check it",            
                        L"托盘1已满/缺失 - 请检查",        
                        L"Salida de rechazos1 llenos/perdidos - por favor revise",                     
                        L"", 
                        L"Лоток 1 заполнен / отсутствует - пожалуйста, проверьте", // 017
/*  18  */              L"STANDBY waiting for rejects1 output",                      
                        L"待机等待托盘1放入",            
                        L"En espera para salida de rechazos1",                                                 
                        L"", 
                        L"Машина в режиме ожидания готовности лотка 1", // 018
/*  19  */              L"Rejects 2 output full/missing - please check it",        
                        L"托盘2已满/缺失 - 请检查",        
                        L"Salida de rechazos2 llenos/perdidos - por favor revise",                                  
                        L"", 
                        L"Лоток 2 заполнен / отсутствует - пожалуйста, проверьте", // 019
/*  20  */              L"STANDBY waiting for rejects2 output",                  
                        L"待机等待托盘2放入",                     
                        L"En espera para salida de rechazos2",                                         
                        L"", 
                        L"Машина в режиме ожидания готовности лотка 2", // 020
/*  21  */              L"Input sensor close to CAM change - Change its position",    
                        L"进料传感器靠近CAM原点-改变其位置",      
                        L"Sensor de entrada cerrada para cambiar CAM - cambie su posicion",         
                        L"", 
                        L"Датчик загрузки близко к камере - измените позицию датчика", // 021
/*  22  */              L"Rotary table FULL",                                   
                        L"转盘满",                               
                        L"Mesa rotatoria LLENA",                
                        L"", 
                        L"Поворотный стол заполнен", // 022
/*  23  */              L"Missing products at input",                              
                        L"进料处缺瓶",                                 
                        L"Productos faltantes en entrada",                                       
                        L"", 
                        L"На станции загрузки недостаточно контейнеров", // 023
/*  24  */              L"STANDBY waiting for input products",                 
                        L"等待进料产品",                  
                        L"En espera para entrada de productos",                                  
                        L"", 
                        L"Машина в режиме ожидания готовности станции загрузки ", // 024
/*  25  */              L"Rejects 3 output full/missing - please check it",    
                        L"托盘3已满/缺失 - 请检查",                               
                        L"Salida de rechazos3 llenos/perdidos - por favor revise",   
                        L"", 
                        L"Лоток 3 заполнен / отсутствует - пожалуйста, проверьте", // 025
/*  26  */              L"STANDBY waiting for rejects3 output",
                        L"待机等待托盘3放入",
                        L"En espera para salida de rechazos3",
                        L"", 
                        L"Машина в режиме ожидания готовности лотка 3", // 026
/*  27  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //027
/*  28  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //028
/*  29  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //029
/*  30  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //030
/*  31  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //031
/*  32  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //032
/*  33  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //033
/*  34  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //034
/*  35  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //035
/*  36  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //036
/*  37  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //037
/*  38  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //038
/*  39  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //039
/*  40  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //040
/*  41  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //041
/*  42  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //042
/*  43  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //043
/*  44  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //044
/*  45  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //045
/*  46  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //046
/*  47  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //047
/*  48  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //048
/*  49  */              L"", 
                        L"",
                        L"",
                        L"", 
                        L"",  //049
/*  50  */              L"Renest XTS blocking", 
                        L"回巢机XTS轨道阻塞",
                        L"",
                        L"", 
                        L"",  //050
/*  51  */              L"Denest XTS reset failure", 
                        L"脱巢机XTS复位失败",
                        L"",
                        L"", 
                        L"",  //051
/*  52  */              L"Denest XTS calibration failure", 
                        L"脱巢机XTS校准失败",
                        L"",
                        L"", 
                        L"",  //052
/*  53  */              L"Renest XTS reset failure", 
                        L"回巢机XTS复位失败",
                        L"",
                        L"", 
                        L"",  //053
/*  54  */              L"Renest XTS calibration failure", 
                        L"回巢机XTS校准失败",
                        L"",
                        L"", 
                        L"",  //054
/*  55  */              L"Waiting denest XTS ready", 
                        L"等候脱巢机XTS就位",
                        L"",
                        L"", 
                        L"",  //055
/*  56  */              L"Waiting denest ready", 
                        L"等候脱巢机就位",
                        L"",
                        L"", 
                        L"",  //056
/*  57  */              L"Waiting renest XTS ready", 
                        L"等候回巢XTS就位",
                        L"",
                        L"", 
                        L"",  //057
/*  58  */              L"Waiting renest ready", 
                        L"等候回巢机就位",
                        L"",
                        L"", 
                        L""  //058
						}; // 058


/*
#define ENGLISH   0
#define CHINESE   1   // ^
#define SPANISH   2   // #
#define TURKISH   3   // @
#define RUSSIAN   4   // &
*/
