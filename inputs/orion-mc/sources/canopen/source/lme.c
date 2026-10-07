/**
 *++ lme - Initialisation of the Layer Management Entity
 *-- lme - Initialisierung der Layer Management Entity
 *
 * Copyright (c) 1995-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *--------------------------------------------------------------------------
 *
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:04:40+01  driet
 * Initial revision
 *
 * Revision 2.27  2003/07/30 08:10:18  boe
 * include time and sync header only if CONFIG_TIME/CONFIG_SYNC is set
 *
 * Revision 2.26  2003/03/31 13:43:34  boe
 * add timer add startup for eva-version
 *
 * Revision 2.25  2003/03/31 12:35:46  boe
 * use new function for leaving emcy consumer entries
 * don't release memory for cob-structures if CONFIG_COB_ARRAY is set
 *
 * Revision 2.24  2003/01/27 09:25:59  boe
 * include srdo.h for consumer and producer
 *
 * Revision 2.23  2002/12/11 07:53:30  boe
 * add initialization of coTimerTicks
 *
 * Revision 2.22  2002/11/15 09:46:08  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * call loadParameterInd() at startup
 * move NMT COB definittion to nmt.c
 *
 * Revision 2.21  2002/05/29 08:40:56  hae
 * documentation correction
 *
 * Revision 2.20  2002/05/21 14:08:13  boe
 * cleanup new timer usage
 *
 * Revision 2.19  2002/03/26 08:12:11  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.18  2001/05/10 12:34:11  boe
 * structure for data mapping changed
 *
 * Revision 2.17  2001/04/12 14:07:44  boe
 * define prototyps for single line always by void
 *
 * Revision 2.16  2001/04/05 12:23:48  boe
 * command line parameter for single line changed to void
 *
 * Revision 2.15  2001/04/05 08:45:44  boe
 * comment changed
 *
 * Revision 2.14  2001/03/29 14:26:46  boe
 * comment changed
 *
 * Revision 2.13  2001/03/28 15:59:39  boe
 * define master variables only if CONFIG_MASTER is set
 *
 * Revision 2.12  2001/03/28 15:50:12  boe
 * include header files for master only if CONFIG_MASTER is set
 *
 * Revision 2.11  2001/03/28 12:45:49  boe
 * added functionality for SLAVE_PLUS
 *
 * Revision 2.10  2001/03/14 15:46:03  ro
 * Redundancy Support functionality added
 *
 * Revision 2.9  2001/02/26 14:06:28  boe
 * documentation format changed
 * flying master functionality added
 *
 * Revision 2.8  2001/01/26 10:56:27  boe
 * split include files into function specific headers
 *
 * Revision 2.7  2001/01/17 16:11:58  boe
 * expand all implicite if tests and add type castings
 * limit the max object dictionary entries for eva license
 *
 * Revision 2.6  2000/10/04 14:02:25  boe
 * defines for PDO,SYNC,TIME changed from CLIENT to CONSUMER and SERVER to PRODUCER
 *
 * Revision 2.5  2000/07/27 10:17:35  boe
 * pdo timer event counter moved from struct node_t to global variables
 *
 * Revision 2.4  2000/06/23 09:43:07  oe
 * Reworking for generating the Regerence Manual
 *
 * Revision 2.3  2000/06/13 08:30:28  boe
 * release all functionality for SYNC_PRODUCER for SYNC_CONSUMER too
 *
 * Revision 2.2  2000/03/28 14:26:44  boe
 * adaption for multi-line version
 *
 * Revision 2.1  2000/02/04 13:38:13  oe
 * - Changes for generating reference pages
 *
 * Revision 2.0  2000/01/21 11:02:25  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *------------------------------------------------------------------
 */


/****************************************************************************/
/**
*  \file lme.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This modul contains functions to initialise
*++ or deactivate the CANopen Library.
*-- Mit den hier beschriebenen Funktionen
*-- wird die CANopen Library
*-- initialisiert bzw. beendet.
* 
*++ Closely associated with this module
*++ are the functions to initialize and serve
*++ CAN controller initCAN() and CANopen timer initTimer().
*-- In engem funktionalen Zusammenhang zu diesem Modul stehen auch die
*-- hardwarenahen Funktionen zur Initialisierung und Bedienung von
*-- CAN Controller initCAN()
*-- und Timer initTimer().
*
* \sa
* can.c cpu.c
*/


/****************************************************************************/
/**
*++ \mainpage The CANopen Library Reference Manual
*-- \mainpage Das CANopen Library Referenz Handbuch
*
*++ The CANopen Protocol Library by
*++ \e port
*++ is a extensible software package.
*++ It conforms to the standard
*++ "CANopen Application Layer and Communication Profile"
*++ DS-301
*++ and DS-302 of CiA e.V. respective EN50325-4.
*++ 
*++ The library is available 
*++ for master, for slave and  
*++ for both master and slave.
*++ Additionally 
*++ \e port
*++ offers a multi CAN line version.
*++ \section a Application
*++ The programs are fully ANSI-C coded,
*++ and hardware specific interfaces
*++ are located in separate modules.
*++ This way an easy adaptation
*++ to different systems is provided.
*++ 
*++ Usage of an application layer decouples
*++ the application completly from the communication system.
*++ That means that an application is
*++ much easier to maintain
*++ and also much more portable.
*++ \section b Description
*++ \e CANopen
*++ Library by \e port
*++ offers the following CANopen properties:
*++ \li Minimum Boot Up
*++ \li NMT services 
*++ \li Service Data Object (SDO) 
*++ \li Process Data Object (PDO) 
*++ \li Emergency Object (EMCY)
*++ \li Synchronisation Object (SYNC) 
*++ \li Time Stamp (TIME)
*++ \li Node Guarding/Heartbeat
*++ \li Programm Download
*++ \li SDO-Manager
*++ \li Flying Master
*++ 
*++ The standard Boot Up for CANopen Devices (Minimum Boot Up) has been 
*++ implemented in the CANopen Library.  
*++ That guarantees an automatical device initialization.
*++ After that the device will be forced in the state PRE_OPERATIONAL.
*++ In this state
*++ the user is able to change the CAN Object Identifier \%(COB-ID) of
*++ the CANopen services via SDO communication.
*++ 
*++ Further it is possible
*++ to set the PDO parameters and their mapping (variable PDO Mapping).
*++ The implemented PDOs support the asynchronous, synchronous, cyclic
*++ and acyclic transmission modes.  
*++ The number of usable CANopen Data Objects (SDO and PDO) depends only on
*++ memory restrictions of the user's target hardware.
*++ 
*++ The Object Dictionary contains references to the user's application variables.
*++ The user's variables can be included in the Object Dictionary
*++ without any changes of the users application code.
*++ 
*++ The interface to the user application is built by functions.
*++ With these functions the user can determine which types of reactions will be
*++ processed on an alteration of Object-Directory entries.
*++ 
*++ A further highlight of this library is the scalability. 
*++ Every kind of CANopen service is located in its own module e.g pdo.c, sdo.c.
*++ Therefore the user can select only the modules he actually needs.
*++ Additionally it is possible to use compiler defines
*++ to select several properties.
*++ The advantage is that the code size is proportional to
*++ the used CANopen functionalities.
*++ 
*++ \e port
*++ supports the development, test and integration of CANopen devices
*++ with a complete set of tools.
*++ One of them is the CANopen Design Tool.
*++ This tool generates for every device  
*++ the Object Dictionary implementation, the Electronic Data Sheet (EDS) 
*++ and a documentation about the implemented device interface
*++ from a database.
*++ It reduces the development cycle and
*++ ensures the quality by the consistency of implementation and documentation.
*++ 
*++ Further variants of the CANopen Library for supporting
*++ multiple CAN networks (max. 255) are available.
*++ By these the user can implement devices 
*++ which can handle independent CAN networks, 
*++ on targets without operating system
*++ or with operating systems without resource allocating mechanism.
*++ This is useful for building gateways and for a convenient segmentation
*++ of CAN networks.
*++ 
*++ The highlights of this library are:
*++ \li supports all CANopen services
*++ \li all transmission modes of PDOs are implemented
*++ \li variable PDO Mapping is possible
*++ \li bitwise PDO Mapping is possible
*++ \li supports PDO Dummy Mapping
*++ \li unlimited number of PDOs and SDOs
*++ \li easy interface to the user's application
*++ \li universal Object Dictionary implementation
*++ created by a database tool
*++ \li Program download is possible
*++ \li Support of multiple CAN-networks possible 
*++ \li scalable program code size
*++ supported by an interactive configuration tool.
*++ \li On-Line Reference Manual as UNIX-man pages
*++ or HTML files
*++ \li complete set of tools for generating
*++ the Object Dictionary, EDS and device documentation 
*++ and for testing and integration  
*++ 
*++ \par System enviroment
*++ The CANopen Library runs on targets with and without operating systems.
*++ It supports all available CAN controllers
*++ and many microcontrollers/processors.
*++ For detailed information see the data sheet of the CANopen Library Driver Packages.
*++ Furthermore a CANopen Starter Kit for evaluation of the library
*++ is available.
*++ 
*++ For further information 
*++ please use the \b CANopen \b User \b Manual .
*++ \note
*++ This documentation was created using the wonderful tool
*++ \b Doxygen http://www.doxygen.org/index.html .
*
*-- Die CANopen-Library 
*-- von
*-- \e port
*-- ist eine flexible CANopen-Bibliothek.
*-- Sie entspricht dem Standard 
*-- "CANopen Application Layer and Communication Profile"
*-- DS-301
*-- und DS-302 des CiA e. V. beziehungsweise dem Standard EN50325-4.
*-- \par
*-- Sie ist verfügbar in den Varianten für
*-- Master, Slave als auch für Master und Slave.
*-- Zusätzlich bietet 
*-- \e port
*-- die o.g. Varianten auch als Multi-CAN-Linien-Version an.
*-- \section a Anwendung
*-- Der Kode wurde vollständig in ANSI-C erstellt.
*-- Hardware-spezifische Teile
*-- sind in separaten Modulen kodiert,
*-- um eine Portierung
*-- zu vereinfachen.
*-- \par
*-- Die Nutzung einer Anwendungsschicht entkoppelt
*-- das Anwendungssystem vollständig
*-- vom Kommunikationssystem.
*-- D.h., daß eine Anwendung viel leichter gepflegt und portiert werden kann.
*-- 
*-- \section b Beschreibung
*-- Beschreibung
*-- Die CANopen-Library
*-- von \e port
*-- unterstützt folgende CANopen-Eigenschaften:
*-- \li Minimum Boot Up
*-- \li Service Data Object (SDO) 
*-- \li Process Data Object (PDO) 
*-- \li Emergency Object (EMCY)
*-- \li Synchronisation Object (SYNC) 
*-- \li Time Stamp (TIME)
*-- \li Nodeguarding oder Heartbeat
*-- \li Programm Download
*-- \li SDO Manager
*-- \li Flying Master
*-- 
*-- In der CANopen-Library
*-- ist das Standard-Netzwerk-Boot-Up
*-- für CANopen-Geräte
*-- (Minimum Boot Up) implementiert.
*-- Es garantiert ein automatisches 
*-- Durchlaufen der Geräteinitialisierung.
*-- Im daran anschließenden Knotenzustand
*-- PRE_OPERATIONAL können vom Anwender die CAN Object Identifier (COB-ID)
*-- den jeweiligen Nachrichtenobjekten mittels eines SDO zugewiesen werden.
*-- In diesem Zustand können weiterhin die PDO-Parameter verändert und den
*-- PDOs andere Variablen zugewiesen werden (variables PDO-Mapping).
*-- Die implementierten PDOs
*-- unterstützen den asynchronen, synchronen, zyklischen sowie
*-- azyklischen Sendemodus (Transmission Type).
*-- Ihre Anzahl sowie die Anzahl der
*-- verwendeten SDOs unterliegt nur den
*-- Speicherrestriktionen der Zielhardware.
*-- \par
*-- Das Objektverzeichnis (OV) ist so ausgelegt, daß es Referenzen auf
*-- die Variablen der Anwenderapplikation enthält.
*-- Damit ist es möglich,
*-- daß Variablen bereits existierender Software
*-- ohne Veränderung des Applikationskodes
*-- in das OV aufgenommen werden können.  
*-- \par
*-- Die Schnittstelle zur Anwenderapplikation wird über Funktionen realisiert.
*-- In diesen kann der Anwender Reaktionen festlegen, die bei Änderung
*-- von Objektverzeichniseinträgen abgearbeitet werden. 
*-- \par
*-- Eine weitere Besonderheit dieser Bibliothek ist ihre hohe Skalierbarkeit.
*-- Auf der einen Seite wird dies durch die Modularisierung der Bibliothek
*-- in einzelne Dienstgruppen z.B sdo.c, pdo.c, ... erreicht und zum anderen
*-- durch die Nutzung von Compilerdirektiven in den jeweiligen Modulen.
*-- Auf diese Art und Weise ist die Kodegröße proportional zu den 
*-- genutzten CANopen-Diensten. 
*-- \par
*-- Zur Entwicklung, zum Test und zur Inbetriebnahme von CANopen-Geräten 
*-- bietet 
*-- \e port
*-- eine vollständige Toolkette an.
*-- Ein Werkzeug ist das CANopen-Design-Tool, daß für jedes Gerät 
*-- ein Objektverzeichnis, ein Electronic Data Sheet (EDS) 
*-- und eine Dokumentation des Geräteinterfaces aus einer Datenbank generiert.
*-- Mit diesem Tool wird die Entwicklung entscheident beschleunigt
*-- und die Konsistenz von 
*-- Implementierung und Dokumentation gewährleistet.
*-- \par
*-- Weiterhin sind Varianten der Library zur 
*-- Unterstützung mehrerer CAN Linien (max. 255) verfügbar.
*-- Damit ist es möglich,
*-- auf Geräten ohne Betriebssystem oder Betriebssystem
*-- mit unzureichenden Ressourcenschutzmechanismen mehrere
*-- von einander unabhängige CAN-Netzwerke zu bedienen.
*-- Es können damit Gateways geschaffen und Netze bequem segmentiert werden.
*-- \par
*-- Die Besonderheiten dieser Bibliothek sind:
*-- \li
*-- alle CANopen-Dienste werden unterstützt
*-- \li
*-- alle Sendemodi von PDOs sind implementiert
*-- \li
*-- variables PDO-Mapping ist möglich
*-- \li
*-- bitweises PDO-Mapping ist möglich
*-- \li
*-- unterstützt PDO-Dummy-Mapping
*-- \li
*-- unbegrenzte Anzahl von PDOs und SDOs
*-- \li
*-- einfache Schnittstelle zur Anwenderapplikation
*-- \li
*-- universale Objektverzeichnisimplementierung
*-- mit datenbankgestützter Generierung
*-- \li
*-- Programmdownload ist möglich
*-- \li
*-- Unterstützung mehrerer CAN-Netzwerke möglich
*-- \li
*-- hohe Skalierbarkeit der Kodegröße
*-- unterstützt durch ein interaktives Konfigurationstool
*-- \li
*-- On-Line-Reference-Manual als UNIX-man-pages
*-- oder im HTML-Format
*-- \li
*-- komplette Toolkette zur Erzeugung von Objekt-
*-- verzeichnis, EDS und Dokumentation sowie zur 
*-- Inbetriebnahme, zum Test und Integration verfügbar
*-- 
*-- \section s Systemumgebung
*-- Die CANopen-Library
*-- ist lauffähig auf Plattformen mit und ohne Betriebssystem.
*-- Es werden alle gängigen CAN-Controller
*-- und viele Mikrocontroller/-prozessoren
*-- unterstützt.
*-- Für detallierte Informationen siehe Datenblatt CANopen-Driver-Packages.
*-- \par
*-- Weiterhin ist ein CANopen-Starter-Kit zur Evaluierung dieser Library
*-- erhältlich.
*-- \par
*-- Für weitere Informationen benutzen Sie bitte das
*-- \b CANopen \b Benutzerhandbuch .
*-- 
*-- \note
*-- Die Dokumentation wurde unter Verwendung von
*-- \b Doxygen http://www.doxygen.org/index.html
*-- erstellt
*-- 
*/


/* header of standard C - libraries */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* header of project specific types */

#include <cal_conf.h>

#include <co_lme.h>
#include <co_usr.h>
#include <co_setcp.h>
#include "cmsevent.h"
#include "pdo.h"
#include "sdo.h"
#include "drv.h"
#include "nmt.h"
#include "nmt_s.h"
#include "emerg.h"

#if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
# include "sync.h"
#endif /* defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER) */

#if defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER)
# include "time_lib.h"
#endif /* defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER) */

#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
# include "nmt_m.h"
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */

#ifdef CONFIG_NON_VOLATILE_MEM
# include <co_stor.h>
#endif /* CONFIG_NON_VOLATILE_MEM */

#ifdef CONFIG_FLYING_MASTER
# include "flyma.h"
#endif /* CONFIG_FLYING_MASTER */

#ifdef CONFIG_SLAVE_PLUS
# include <co_splus.h>
#endif /* CONFIG_SLAVE_PLUS */

#if defined(CONFIG_SRDO_CONSUMER) || defined(CONFIG_SRDO_PRODUCER)
# include "srdo.h"
#endif /* CONFIG_SRDO_CONSUMER/PRODUCER */

/* constant definitions
---------------------------------------------------------------------------*/

/* local defined data types
---------------------------------------------------------------------------*/

/* list of external used functions, if not in headers
---------------------------------------------------------------------------*/

/* list of global defined functions
---------------------------------------------------------------------------*/

/* list of local defined functions
---------------------------------------------------------------------------*/
static void leaveEvents(CMS_EVENT_T *pFirstEntry, BOOL_T withMapping);

/* external variables
---------------------------------------------------------------------------*/
extern volatile UNSIGNED8    coTimerTicks ;

#ifdef CONFIG_EVA_VERSION
extern UNSIGNED16	maxObjDicElements;
extern TIMER_EVENT_T	evaTimer;
#endif /* CONFIG_EVA_VERSION */

/* global variables
---------------------------------------------------------------------------*/
UNSIGNED8	coNodeId ;	/* CANopen Node Id */
/**
*++ this variable contains global flags
*++ indicating special events.
*++ Each call to internal function flagIndentification()
*++ first checks these flags.
*-- Diese Variable enthält globale Flags für spezielle Ereignisse.
*-- Bei jedem Aufruf der Funktion flagIdentification()
*-- werden diese Flags ausgewertet.
*/
UNSIGNED8	coFlags ;		/* CANopen flags */

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */

/****************************************************************************/
/**
*
*++ \brief initCANopen - initialisation of the CANopen Library
*-- \brief initCANopen - Initialisierung der CANopen Library
*
*-- Diese Funktion ist für die Initialisierung der Daten- und Funktionaufrufe
*-- der Library notwendig.
*++ The function does all necessary initialization
*++ of the CANopen library functions and data.
*-- Neben der Initialisierung der library-internen Pointer
*-- wird die Initialisierungsfunktion für den dynamischen Speicher
*++ It initialiases the internal library pointers
*++ and calls the function
* InitCalMalloc(0)
*-- innerhalb des Treibers aufgerufen.
*++ at the driver
*++ for usage of the dynamic memory.
*-- Weiterhin werden alle Werte im Kommunikationsteil des OV
*-- auf ihre Standardwerte (Predefined Connection Set) gesetzt.
*++ Additionally all values at the communication part at the object dictionary
*++ are set to it default values according the predefined connection set.
*-- Wenn der Knoten nichtflüchtigen Speicher besitzt,
*-- wird anschliessend die Indikation-Funktion
*++ If the node has non-volatile memory
*++ the indication function
* loadParameterInd()
*-- aufgerufen,
*++ is called,
*-- so dass die mit der Funktion
*++ to reload the values saved by
* saveParameterInd()
*-- gespeicherten Daten restauriert werden können.
*-- Der Anwender kann anschliessend auch andere Power-On Werte
*-- im OV hinterlegen,
*-- die dann bei der Initialisierung der Dienste genutzt werden.
*++ The user can load other power-on values after this function.
*-- Zur Ermittlung der Node-Id wird die Funktion
*++ To get the node id the user function
* getNodeId()
*-- aus dem File
*++ from the file 
* usr_301.c
*-- aufgerufen, welche vom Anwender entsprechend bereitzustellen ist.
*++ is called.
* \par
*-- Diese Funktion ist nach der Hardware Initialisierung des CAN Controllers
*-- und vor der Initialisierung des Timers aufzurufen.
*++ It should be called after calling hardware initialization
*++ of the CAN controller and before initialization of the CANopen timer.
* \par
*-- Eine typische Aufrufreihenfolge für CANopen Applikationen ist:
*++ A typical calling sequence for CANopen applications might be:
* \par
* \code
* initCAN();
* initCANopen();
* Start_CAN();
* initTimer();
* \endcode
*
* \retval CO_OK
*-- Erfolg
*++ Success
* \retval CO_E_NO_ACCESS
*++ no access to Object Dictionary (Node - ID)
*-- kein Zugriff auf das Objektverzeichnis möglich (Node - ID)
*
*/

RET_T initCANopen(
    void
	)
{

#ifdef CONFIG_EVA_VERSION
    /* don't allow more object dictionary entries for eval version */
    if (maxObjDicElements > 50)  {
	coNodeId  = 0;
	return(CO_E_NO_ACCESS);
    }
#endif /* CONFIG_EVA_VERSION */

    InitCalMalloc(0);
    /* dTimerValue = 0; */


	/* init the global library flags */
	coFlags  = 0;

/*---- initialize cal variables ----------------------------------*/
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
	co_pNetwork  = NULL;
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */

#ifdef CONFIG_PDO_CONSUMER
	co_pFirstRecPdo  = NULL;
#endif /* PDO_CONSUMER */
#ifdef CONFIG_PDO_PRODUCER
	co_pFirstTrPdo  = NULL;
#endif /* PDO_PRODUCER */

#ifdef CONFIG_EMCY_PRODUCER
	co_pFirstEmcyProd  = NULL;
#endif /* CONFIG_EMCY_PRODUCER */
#ifdef CONFIG_EMCY_CONSUMER
	co_pFirstEmcyCons  = NULL;
#endif /* CONFIG_EMCY_CONSUMER */
#if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
	co_pSync  = NULL;
#endif /* defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER) */
#if defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER)
	co_pFirstTime  = NULL;
#endif /* defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER) */
	/* not NMT funtionality */
	co_pNode  = NULL;

	co_pFirstDomEntry  = NULL;

# ifdef CONFIG_REDUNDANCY_SUPPORT
	co_pFirst_COB_Entry [0] = NULL;
	co_pFirst_COB_Entry [1] = NULL;
# else /* CONFIG_REDUNDANCY_SUPPORT */
	co_pFirst_COB_Entry  = NULL;
# endif /* CONFIG_REDUNDANCY_SUPPORT */

#ifdef CONFIG_FLYING_MASTER
	coDetectMCap  .pCOB_Tr = NULL;
	coDetectMCap  .pCOB_Rec = NULL;
	coResponseMCap  .pCOB_Tr = NULL;
	coResponseMCap  .pCOB_Rec = NULL;
	coDetectActM  .pCOB_Tr = NULL;
	coDetectActM  .pCOB_Rec = NULL;
	coManagerIdent  .pCOB_Tr = NULL;
	coManagerIdent  .pCOB_Rec = NULL;
	coTriggerTimeSlot  .pCOB_Tr = NULL;
	coTriggerTimeSlot  .pCOB_Rec = NULL;
#endif /* CONFIG_FLYING_MASTER */

#ifdef CONFIG_SRDO_PRODUCER
	co_pProdSrdo  = NULL;
#endif /* CONFIG_SRDO_PRODUCER */
#ifdef CONFIG_SRDO_CONSUMER
	co_pConSrdo  = NULL;
#endif /* CONFIG_SRDO_CONSUMER */

	/* calls user function to get the node id from DIP switch or EEPROM */
	coNodeId  = getNodeId(CO_LINE_PARA);

	/* set comm-objects to their default cob-id values */
	resetObjDir(MEM_SEG_ALL_PARAMETERS CO_COMMA_LINE_PARA);

#ifdef CONFIG_NON_VOLATILE_MEM
	/* load saved values from flash */
	loadParameterInd(MEM_SEG_ALL_PARAMETERS, CO_RESTORE_MODE_BOOTUP
		CO_COMMA_LINE_PARA);
#endif /* CONFIG_NON_VOLATILE_MEM */

	/* init timer ticks */
	coTimerTicks  = 0;


#ifdef CONFIG_EVA_VERSION
    /* reset application after 1 h */
    addTimerEvent(&evaTimer, 10UL * 1000UL * 60UL * 60UL, 0);
#endif /* CONFIG_EVA_VERSION */

    return (CO_OK);
}


/****************************************************************************/
/**
*
*-- \brief leaveCANopen - Beendigung von CANopen
*++ \brief leaveCANopen - leave and finish CANopen
*
*-- Mit dieser Funktion sollten
*-- nach Beendigung des Anwenderprogramms
*-- die CANopen-Initialisierungen wieder
*-- zurückgenommen werden.
*++ The application should use this function to
*++ reset all CANopen library inizializations,
*++ possibly after ending the application programm
* \par
*-- Die CAN Controller Hardware oder der Timer
*-- werden von dieser Funktion nicht berührt.
*-- Beide müssen aber vor Aufruf dieser Funktion deaktiviert werden.
*++ The function does not reset the hardware of the CAN controller
*++ nor the timer.
*++ But both have to be deactivated before calling
*++ \em leaveCANopen()
* \par
* \code
  ReleaseTimer();
  leaveCANopen();
* \endcode
*/

void leaveCANopen(
	void
	)
{
/* help variables for deallocation of memory */
CMS_DOMAIN_T  *pCurDomEntry;		/* pointer to current domain */
COB_T	*pCob;				/* pointer to cob struct */


    /* freeing of domain resources */
    pCurDomEntry = co_pFirstDomEntry ;
    while (pCurDomEntry != NULL) {
	/* release mux */
	co_pFirstDomEntry  = pCurDomEntry;
	pCurDomEntry = pCurDomEntry->pNext;
	CalFree(co_pFirstDomEntry );
    }

    /* freeing of event resources */
#ifdef CONFIG_PDO_PRODUCER
    leaveEvents(co_pFirstTrPdo , CO_TRUE);
#endif /* CONFIG_PDO_PRODUCER */
#ifdef CONFIG_PDO_CONSUMER
    leaveEvents(co_pFirstRecPdo , CO_TRUE);
#endif /* CONFIG_PDO_CONSUMER */
#ifdef CONFIG_EMCY_PRODUCER
    leaveEvents(co_pFirstEmcyProd , CO_FALSE);
#endif /* CONFIG_EMCY_PRODUCER */
#ifdef CONFIG_EMCY_CONSUMER
    leaveEmcyCons(CO_LINE_PARA);
#endif /* CONFIG_EMCY_CONSUMER */

/* free SYNC */
#if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
    if (co_pSync  != NULL)  {
	CalFree(co_pSync );
    }
#endif /* defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER) */
    
#if defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER)
    CalFree(co_pFirstTime );
#endif /* defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER) */

# ifdef CONFIG_COB_ARRAY
# else /* CONFIG_COB_ARRAY */
    /* freeing the COB-Ids */
#  ifdef CONFIG_REDUNDANCY_SUPPORT
    {
    UNSIGNED8	i;	/* loop counter */

    for (i = 0; i < 2; i++)  {
	pCob = co_pFirst_COB_Entry [i];
	while (pCob != NULL)  {
	    co_pFirst_COB_Entry [i] = pCob->pNext;
	    CalFree(pCob);
	    pCob = co_pFirst_COB_Entry [i];
	}
    }
    }
#  else /* CONFIG_REDUNDANCY_SUPPORT */
    pCob = co_pFirst_COB_Entry ;
    while (pCob != NULL)  {
	co_pFirst_COB_Entry  = pCob->pNext;
	CalFree(pCob);
	pCob = co_pFirst_COB_Entry ;
    }
#  endif /* CONFIG_REDUNDANCY_SUPPORT */
# endif /* CONFIG_COB_ARRAY */

    return;
}


/******************************************************************
*
*-- leaveEvents - leave all events
*++ leaveEvents - leave all events
*
* NOMANUAL
*
*-- Mit dieser Funktion sollten
*-- nach Beendigung des Anwenderprogramms
*-- alle Initialisierungen für Events wieder
*-- zurückgenommen werden.
*++ The application should use this function to
*++ reset all library inizializations,
*++ possibly after ending the application programm for events
*/

static void leaveEvents(
	CMS_EVENT_T *pFirstEntry,	/* Address of first Event entry */
	BOOL_T withMapping		/* mapping enabled flag */
    )
{
CMS_EVENT_T	*pCurEvEntry, *pEv;	/* pointer to event structures */
PDO_MAP_T	*pPdoMap;		/* pointer to mapping structure */

    pCurEvEntry = pFirstEntry;

    while (pCurEvEntry != NULL) {
	if (withMapping == CO_TRUE)  {
	    pPdoMap = pCurEvEntry->pMapEntries;
	    while (pPdoMap != NULL)  {
		pCurEvEntry->pMapEntries = pPdoMap->pNext;
		CalFree(pPdoMap);
		pPdoMap = pCurEvEntry->pMapEntries;
	    }
	}
	pEv = pCurEvEntry->pNext;
	CalFree(pCurEvEntry);
	pCurEvEntry = pEv;
    }
}

/*______________________________________________________________________EOF_*/
