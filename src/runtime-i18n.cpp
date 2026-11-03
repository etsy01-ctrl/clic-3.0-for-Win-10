#include "runtime-i18n.h"
#include "rcomu/clicres.h"
#include <stdio.h>
#include <string.h>
int playerLanguage=0, playerDefaultLanguage=0;
static const char *catalog[PT_COUNT][PLAYER_LANGUAGE_COUNT]={
  {"&Archivo","&File","&Fichier","&Fitxer"},
  {"&Actividad","&Activity","&Activit\351","&Activitat"},
  {"&Herramientas","&Tools","&Outils","&Eines"},
  {"A&yuda","&Help","&Aide","A&juda"},
  {"&Abrir juego...\tCtrl+O","&Open game...\tCtrl+O","&Ouvrir un jeu...\tCtrl+O","&Obre un joc...\tCtrl+O"},
  {"Vincular archivos .pcc...","Associate .pcc files...","Associer les fichiers .pcc...","Associa fitxers .pcc..."},
  {"Minimizar","Minimize","R\351duire","Minimitza"},
  {"&Salir\tAlt+F4","E&xit\tAlt+F4","&Quitter\tAlt+F4","&Surt\tAlt+F4"},
  {"&Reiniciar\tF5","&Restart\tF5","&Recommencer\tF5","&Reinicia\tF5"},
  {"&Ayuda de la actividad\tF1","Activity &help\tF1","&Aide de l'activit\351\tF1","&Ajuda de l'activitat\tF1"},
  {"Opciones de puzzle...","Puzzle options...","Options du puzzle...","Opcions del trencaclosques..."},
  {"Informaci\363n de la actividad...","Activity information...","Informations sur l'activit\351...","Informaci\363 de l'activitat..."},
  {"Imprimir...","Print...","Imprimer...","Imprimeix..."},
  {"&Apariencia","&Appearance","&Apparence","&Aparen\347a"},
  {"Clic 3.0 cl\341sico","Clic 3.0 classic","Clic 3.0 classique","Clic 3.0 cl\340ssic"},
  {"Moderno","Modern","Moderne","Modern"},
  {"Acerca de Clic Player...","About Clic Player...","\300 propos de Clic Player...","Quant al Clic Player..."},
  {"&Idioma","&Language","&Langue","&Llengua"},
  {"Usar idioma actual por defecto","Use current language by default","D\351finir cette langue par d\351faut","Fes servir aquest idioma per defecte"},
  {"Predeterminado: %s","Default: %s","Par d\351faut : %s","Per defecte: %s"},
  {"Abrir juego","Open game","Ouvrir un jeu","Obre un joc"},
  {"Salir","Exit","Quitter","Surt"},
  {"Reiniciar","Restart","Recommencer","Reinicia"},
  {"Ayuda","Help","Aide","Ajuda"},
  {"Anterior","Previous","Pr\351c\351dent","Anterior"},
  {"Siguiente","Next","Suivant","Seg\374ent"},
  {"Imprimir","Print","Imprimer","Imprimeix"},
  {"Puzzle","Puzzle","Puzzle","Trencaclosques"},
  {"Informaci\363n","Information","Informations","Informaci\363"},
  {"Salir","Exit","Quitter","Surt"},
  {"Abrir juego (Ctrl+O)","Open game (Ctrl+O)","Ouvrir un jeu (Ctrl+O)","Obre un joc (Ctrl+O)"},
  {"Imprimir","Print","Imprimer","Imprimeix"},
  {"Reiniciar (F5)","Restart (F5)","Recommencer (F5)","Reinicia (F5)"},
  {"Ayuda de la actividad (F1)","Activity help (F1)","Aide de l'activit\351 (F1)","Ajuda de l'activitat (F1)"},
  {"Opciones de puzzle","Puzzle options","Options du puzzle","Opcions del trencaclosques"},
  {"Informaci\363n","Information","Informations","Informaci\363"},
  {"Siguiente (Ctrl+Derecha)","Next (Ctrl+Right)","Suivant (Ctrl+Droite)","Seg\374ent (Ctrl+Dreta)"},
  {"Anterior (Ctrl+Izquierda)","Previous (Ctrl+Left)","Pr\351c\351dent (Ctrl+Gauche)","Anterior (Ctrl+Esquerra)"},
  {"Listo","Ready","Pr\352t","A punt"},
  {"Aciertos","Score","R\351ussites","Encerts"},
  {"Intentos","Tries","Essais","Intents"},
  {"Tiempo","Time","Temps","Temps"},
  {"aciertos","score","r\351ussis","encerts"},
  {"intentos","tries","essais","intents"},
  {"tiempo","time","temps","temps"},
  {"Actividad %d de %d","Activity %d of %d","Activit\351 %d sur %d","Activitat %d de %d"},
  {"Actividad individual","Single activity","Activit\351 individuelle","Activitat individual"},
  {"Abre un juego para comenzar","Open a game to begin","Ouvrez un jeu pour commencer","Obre un joc per comen\347ar"},
  {"Actividades interactivas","Interactive activities","Activit\351s interactives","Activitats interactives"},
  {"Selecciona una actividad para comenzar","Select an activity to begin","S\351lectionnez une activit\351 pour commencer","Selecciona una activitat per comen\347ar"},
  {"Seleccionar juego Clic","Select a Clic game","Choisir un jeu Clic","Selecciona un joc Clic"},
  {"Paquetes Clic (*.pcc;*.pac;*.exe)","Clic packages (*.pcc;*.pac;*.exe)","Paquets Clic (*.pcc;*.pac;*.exe)","Paquets Clic (*.pcc;*.pac;*.exe)"},
  {"Todos los archivos (*.*)","All files (*.*)","Tous les fichiers (*.*)","Tots els fitxers (*.*)"},
  {"Acerca de Clic Player","About Clic Player","\300 propos de Clic Player","Quant al Clic Player"},
  {"Clic Player para Windows\n\nMotor Clic 3.0 de Francesc Busquets.\nPortada Clic 3.0 creada con ImageGen.\n\nAbrir juegos: Ctrl+O\nReiniciar: F5\nAnterior / siguiente: Ctrl+Izquierda / Ctrl+Derecha","Clic Player for Windows\n\nClic 3.0 engine by Francesc Busquets.\nClic 3.0 splash artwork created with ImageGen.\n\nOpen games: Ctrl+O\nRestart: F5\nPrevious / next: Ctrl+Left / Ctrl+Right","Clic Player pour Windows\n\nMoteur Clic 3.0 de Francesc Busquets.\nIllustration Clic 3.0 cr\351\351e avec ImageGen.\n\nOuvrir un jeu : Ctrl+O\nRecommencer : F5\nPr\351c\351dent / suivant : Ctrl+Gauche / Ctrl+Droite","Clic Player per a Windows\n\nMotor Clic 3.0 de Francesc Busquets.\nIl\267lustraci\363 Clic 3.0 creada amb ImageGen.\n\nObre jocs: Ctrl+O\nReinicia: F5\nAnterior / seg\374ent: Ctrl+Esquerra / Ctrl+Dreta"},
  {"Vincular archivos .pcc","Associate .pcc files","Associer les fichiers .pcc","Associa fitxers .pcc"},
  {"Abre tus juegos con doble clic","Open your games with a double-click","Ouvrez vos jeux d'un double clic","Obre els jocs amb un doble clic"},
  {"Registra Clic Player para tus archivos .pcc. Se aplica solo a tu usuario de Windows.","Register Clic Player for your .pcc files. Applies only to your Windows user account.","Enregistrez Clic Player pour vos fichiers .pcc. Cela concerne uniquement votre compte Windows.","Registra el Clic Player per als teus fitxers .pcc. Nom\351s s'aplica al teu compte de Windows."},
  {"Reproductor","Player","Lecteur","Reproductor"},
  {"En Configuraci\363n de Windows, abre las aplicaciones predeterminadas por tipo de archivo. Busca .pcc y elige Clic Player.","In Windows Settings, open default apps by file type. Find .pcc and choose Clic Player.","Dans les param\350tres Windows, ouvrez les applications par d\351faut par type de fichier. Recherchez .pcc et choisissez Clic Player.","A la configuraci\363 de Windows, obre les aplicacions per defecte segons el tipus de fitxer. Busca .pcc i tria Clic Player."},
  {"Registrar y abrir ajustes","Register and open settings","Enregistrer et configurer","Registra i obre la configuraci\363"},
  {"Cancelar","Cancel","Annuler","Cancel\267la"},
  {"No se pudo registrar Clic Player. C\363digo de Windows: %ld","Could not register Clic Player. Windows error: %ld","Impossible d'enregistrer Clic Player. Erreur Windows : %ld","No s'ha pogut registrar el Clic Player. Codi de Windows: %ld"},
  {"Registro completado","Registration complete","Enregistrement termin\351","Registre completat"},
  {"Clic Player est\341 registrado. Abre Configuraci\363n > Aplicaciones > Aplicaciones predeterminadas y elige Clic Player para .pcc.","Clic Player is registered. Open Settings > Apps > Default apps and choose Clic Player for .pcc.","Clic Player est enregistr\351. Ouvrez Param\350tres > Applications > Applications par d\351faut et choisissez Clic Player pour .pcc.","Clic Player est\340 registrat. Obre Configuraci\363 > Aplicacions > Aplicacions per defecte i tria Clic Player per als fitxers .pcc."},
  {"No se pudo abrir la ventana de vinculaci\363n. C\363digo de Windows: %lu","Could not open the association dialog. Windows error: %lu","Impossible d'ouvrir la fen\352tre d'association. Erreur Windows : %lu","No s'ha pogut obrir la finestra d'associaci\363. Codi de Windows: %lu"},
  {"No se pudo guardar el idioma predeterminado.","Could not save the default language.","Impossible d'enregistrer la langue par d\351faut.","No s'ha pogut desar l'idioma per defecte."},
  {"Reproductor de actividades Clic","Clic activity player","Lecteur d'activit\351s Clic","Reproductor d'activitats Clic"},
  {"Paquete Clic (.pcc)","Clic package (.pcc)","Paquet Clic (.pcc)","Paquet Clic (.pcc)"}
};
const char *tr(PlayerText text) {return text>=0 && text<PT_COUNT ? catalog[text][playerLanguage] : "";}
const char *playerLanguageName(int language) {
  static const char *names[]={"Espa\361ol","English","Fran\347ais","Catal\340"};
  return names[language>=0 && language<PLAYER_LANGUAGE_COUNT ? language : 0];
}
void setPlayerLanguage(int language) {playerLanguage=language>=0 && language<PLAYER_LANGUAGE_COUNT ? language : 0;}
void initPlayerLanguage(const char *iniFile) {
  setPlayerLanguage(GetPrivateProfileInt("Player","Language",0,iniFile));
  playerDefaultLanguage=playerLanguage;
}
BOOL savePlayerLanguage(const char *iniFile) {
  char value[12]; sprintf(value,"%d",playerLanguage);
  if(!WritePrivateProfileString("Player","Language",value,iniFile)) return FALSE;
  playerDefaultLanguage=playerLanguage; return TRUE;
}
struct LegacyText {UINT id; const char *text[PLAYER_LANGUAGE_COUNT];};
static LegacyText legacy[]={
  {IDS_TITOL,{"Clic Player","Clic Player","Clic Player","Clic Player"}},
  {IDS_NOMEM,{"MEMORIA INSUFICIENTE","NOT ENOUGH MEMORY","M\311MOIRE INSUFFISANTE","MEM\322RIA INSUFICIENT"}},
  {IDS_LECT,{"Leyendo el archivo  ","Reading file  ","Lecture du fichier  ","Lectura del fitxer  "}},
  {IDS_ERROPEN,{"ERROR AL ABRIR EL ARCHIVO","COULD NOT OPEN FILE","IMPOSSIBLE D'OUVRIR LE FICHIER","NO S'HA POGUT OBRIR EL FITXER"}},
  {IDS_ERRREAD,{"ERROR AL LEER EL ARCHIVO","COULD NOT READ FILE","IMPOSSIBLE DE LIRE LE FICHIER","NO S'HA POGUT LLEGIR EL FITXER"}},
  {IDS_ERRFMT,{"ERROR: Formato incorrecto","ERROR: Invalid format","ERREUR : format incorrect","ERROR: format incorrecte"}},
  {IDS_ERR8PX,{"ERROR: M\341s de 8 bits por p\355xel","ERROR: More than 8 bits per pixel","ERREUR : plus de 8 bits par pixel","ERROR: m\351s de 8 bits per p\355xel"}},
  {IDS_ERRBMP,{"ERROR EN LA CREACI\323N DE LA IMAGEN","COULD NOT CREATE IMAGE","IMPOSSIBLE DE CR\311ER L'IMAGE","NO S'HA POGUT CREAR LA IMATGE"}},
  {IDS_ERRMIDI,{"ERROR DEL SECUENCIADOR MIDI","MIDI SEQUENCER ERROR","ERREUR DU S\311QUENCEUR MIDI","ERROR DEL SEQ\334ENCIADOR MIDI"}},
  {IDS_ERRWAVE,{"ERROR DEL SISTEMA DE SONIDO","SOUND SYSTEM ERROR","ERREUR DU SYST\310ME AUDIO","ERROR DEL SISTEMA DE SO"}},
  {IDS_ERRMCI,{"ERROR DEL SISTEMA MULTIMEDIA","MULTIMEDIA SYSTEM ERROR","ERREUR DU SYST\310ME MULTIM\311DIA","ERROR DEL SISTEMA MULTIM\310DIA"}},
  {IDS_PROMPEXIT,{"\277Finalizar el programa?","Exit the player?","Quitter le lecteur ?","Voleu sortir del reproductor?"}},
  {IDS_SENSENOM,{"(Sin nombre)","(Untitled)","(Sans titre)","(Sense nom)"}},
  {IDS_OPENFITX,{"ABRIR ACTIVIDAD O PAQUETE","OPEN ACTIVITY OR PACKAGE","OUVRIR UNE ACTIVIT\311 OU UN PAQUET","OBRE UNA ACTIVITAT O UN PAQUET"}},
  {IDS_ERRWRITE,{"ERROR AL ESCRIBIR EL ARCHIVO","COULD NOT WRITE FILE","IMPOSSIBLE D'\311CRIRE LE FICHIER","NO S'HA POGUT ESCRIURE EL FITXER"}},
  {IDS_WRITE,{"Escribiendo el archivo  ","Writing file  ","\311criture du fichier  ","Escriptura del fitxer  "}},
  {IDS_ERRNEWV,{"Esta actividad necesita una versi\363n m\341s reciente de Clic.","This activity requires a newer version of Clic.","Cette activit\351 n\351cessite une version plus r\351cente de Clic.","Aquesta activitat necessita una versi\363 m\351s recent del Clic."}},
  {IDS_ERRPATH,{"Directorio incorrecto.","Invalid folder.","Dossier incorrect.","Carpeta incorrecta."}},
  {IDS_ERRHLPFN,{"No se encuentra el archivo de ayuda.","Help file not found.","Fichier d'aide introuvable.","No s'ha trobat el fitxer d'ajuda."}},
  {IDS_ERRTOPIC,{"No se pudo abrir la informaci\363n de la actividad.","Could not open activity information.","Impossible d'ouvrir les informations de l'activit\351.","No s'ha pogut obrir la informaci\363 de l'activitat."}},
  {IDS_ERRNOFILE,{"Archivo no encontrado.","File not found.","Fichier introuvable.","No s'ha trobat el fitxer."}},
  {IDS_ERRDLL,{"No se pudo cargar el m\363dulo de la actividad.","Could not load the activity module.","Impossible de charger le module de l'activit\351.","No s'ha pogut carregar el m\362dul de l'activitat."}},
  {IDS_ERRDLLNAME,{"M\363dulo de actividad incorrecto.","Invalid activity module.","Module d'activit\351 incorrect.","M\362dul d'activitat incorrecte."}},
  {IDS_ERRNOFONS,{"No se pudo cargar el fondo.","Could not load the background.","Impossible de charger l'arri\350re-plan.","No s'ha pogut carregar el fons."}},
  {IDS_ERRREC,{"ERROR DE GRABACI\323N","RECORDING ERROR","ERREUR D'ENREGISTREMENT","ERROR D'ENREGISTRAMENT"}},
  {IDS_ERRPLAYREC,{"ERROR AL REPRODUCIR LA GRABACI\323N","COULD NOT PLAY RECORDING","IMPOSSIBLE DE LIRE L'ENREGISTREMENT","NO S'HA POGUT REPRODUIR L'ENREGISTRAMENT"}},
  {IDS_ERREXE,{"No se pudo iniciar la aplicaci\363n.","Could not start the application.","Impossible de d\351marrer l'application.","No s'ha pogut iniciar l'aplicaci\363."}},
  {IDS_800x600,{"Esta actividad est\341 dise\361ada para una pantalla de 800 x 600 o superior. \277Continuar?","This activity is designed for a display of 800 x 600 or larger. Continue?","Cette activit\351 est con\347ue pour un \351cran de 800 x 600 ou plus. Continuer ?","Aquesta activitat est\340 dissenyada per a una pantalla de 800 x 600 o superior. Voleu continuar?"}},
  {IDS_256COLORS,{"Esta actividad requiere al menos 256 colores. \277Continuar?","This activity requires at least 256 colors. Continue?","Cette activit\351 n\351cessite au moins 256 couleurs. Continuer ?","Aquesta activitat necessita almenys 256 colors. Voleu continuar?"}},
  {IDS_INTERC,{"Intercambio","Swap","\311change","Intercanvi"}},
  {IDS_FORAT,{"Agujero","Sliding","Taquin","Forat"}},
  {IDS_DOBLE,{"Doble","Double","Double","Doble"}},
  {IDS_MEMORY,{"Memoria","Memory","M\351moire","Mem\362ria"}},
  {IDS_MASSA,{"Demasiadas casillas.","Too many cells.","Trop de cases.","Hi ha massa caselles."}},
  {IDS_UNA,{"Se necesita m\341s de una casilla.","More than one cell is required.","Plus d'une case est n\351cessaire.","Cal m\351s d'una casella."}},
  {IDS_DUES,{"Se necesitan al menos dos casillas.","At least two cells are required.","Au moins deux cases sont n\351cessaires.","Calen almenys dues caselles."}}
};
const char *legacyPlayerText(UINT id) {
  for(unsigned i=0;i<sizeof(legacy)/sizeof(legacy[0]);i++) if(legacy[i].id==id) return legacy[i].text[playerLanguage];
  return NULL;
}
static const char *dialogText[][PLAYER_LANGUAGE_COUNT]={
  {"OK","OK","OK","D'acord"},
  {"Cancel","Cancel","Annuler","Cancel\267la"},
  {"Cancelar","Cancel","Annuler","Cancel\267la"},
  {"CANCEL","Cancel","Annuler","Cancel\267la"},
  {"Ayuda","Help","Aide","Ajuda"},
  {"Ajuda","Help","Aide","Ajuda"},
  {"Cerrar","Close","Fermer","Tanca"},
  {"Rompecabezas","Puzzle","Puzzle","Trencaclosques"},
  {"Intercambio","Swap","\311change","Intercanvi"},
  {"Agujero","Sliding","Taquin","Forat"},
  {"Doble","Double","Double","Doble"},
  {"Memoria","Memory","M\351moire","Mem\362ria"},
  {"Est\341ndar","Default","Standard","Est\340ndard"},
  {"Columnas:","Columns:","Colonnes :","Columnes:"},
  {"Filas:","Rows:","Lignes :","Files:"},
  {"Modalidad:","Mode:","Mode :","Modalitat:"}
};
static BOOL CALLBACK _export translateControl(HWND window, LPARAM) {
  char text[512]; GetWindowText(window,text,sizeof(text));
  for(unsigned i=0;i<sizeof(dialogText)/sizeof(dialogText[0]);i++)
    if(strcmp(text,dialogText[i][0])==0) {SetWindowText(window,dialogText[i][playerLanguage]); break;}
  return TRUE;
}
void localizePlayerDialog(HWND dialog) {
  translateControl(dialog,0); EnumChildWindows(dialog,(WNDENUMPROC)translateControl,0);
}
HMENU createPlayerMenu() {
  HMENU menu=CreateMenu(), file=CreatePopupMenu(), activity=CreatePopupMenu(), tools=CreatePopupMenu(), appearance=CreatePopupMenu(), language=CreatePopupMenu(), help=CreatePopupMenu();
  AppendMenu(file,MF_STRING,IDM_OPENFITX,tr(PT_OPEN_MENU));
  AppendMenu(file,MF_STRING,IDM_ASSOCIATE_PCC,tr(PT_ASSOC_MENU));
  AppendMenu(file,MF_SEPARATOR,0,NULL); AppendMenu(file,MF_STRING,IDM_MINIMIZE,tr(PT_MINIMIZE));
  AppendMenu(file,MF_SEPARATOR,0,NULL); AppendMenu(file,MF_STRING,IDM_EXIT,tr(PT_EXIT_MENU));
  AppendMenu(menu,MF_POPUP,(UINT)file,tr(PT_FILE));
  AppendMenu(activity,MF_STRING,IDFA_INICI,tr(PT_RESTART_MENU));
  AppendMenu(activity,MF_STRING,IDFA_AJUDA,tr(PT_ACT_HELP_MENU));
  AppendMenu(activity,MF_SEPARATOR,0,NULL);
  AppendMenu(activity,MF_STRING,BOTO_PUZZLE,tr(PT_PUZZLE_MENU));
  AppendMenu(activity,MF_STRING,BOTO_INFO,tr(PT_ACT_INFO_MENU));
  AppendMenu(activity,MF_STRING,BOTO_PRINTER,tr(PT_PRINT_MENU));
  AppendMenu(menu,MF_POPUP,(UINT)activity,tr(PT_ACTIVITY));
  AppendMenu(appearance,MF_STRING,IDM_SKIN_CLASSIC,tr(PT_CLASSIC_SKIN));
  AppendMenu(appearance,MF_STRING,IDM_SKIN_MODERN,tr(PT_MODERN_SKIN));
  AppendMenu(tools,MF_POPUP,(UINT)appearance,tr(PT_APPEARANCE));
  for(int i=0;i<PLAYER_LANGUAGE_COUNT;i++) AppendMenu(language,MF_STRING|(playerLanguage==i ? MF_CHECKED:0),IDM_LANGUAGE_ES+i,playerLanguageName(i));
  AppendMenu(language,MF_SEPARATOR,0,NULL);
  AppendMenu(language,MF_STRING|(playerLanguage==playerDefaultLanguage ? MF_GRAYED:0),IDM_LANGUAGE_SAVE,tr(PT_SAVE_DEFAULT));
  char current[150]; sprintf(current,tr(PT_DEFAULT_FMT),playerLanguageName(playerDefaultLanguage));
  AppendMenu(language,MF_STRING|MF_GRAYED,IDM_LANGUAGE_DEFAULT,current);
  AppendMenu(tools,MF_POPUP,(UINT)language,tr(PT_LANGUAGE));
  AppendMenu(menu,MF_POPUP,(UINT)tools,tr(PT_TOOLS));
  AppendMenu(help,MF_STRING,IDM_INFO,tr(PT_ABOUT_MENU));
  AppendMenu(menu,MF_POPUP,(UINT)help,tr(PT_HELP_MENU));
  return menu;
}

