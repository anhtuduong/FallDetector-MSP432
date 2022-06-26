#ifndef LIB_WIFI_H_
#define LIB_WIFI_H_

#include "simplelink.h"
#include "sl_common.h"

static _i32 establishConnectionWithAP();
static _i32 disconnectFromAP();
static _i32 configureSimpleLinkToDefaultState();
static _i32 initializeAppVariables();
static void displayBanner();
static _i32 getHostIP();
static _i32 createConnection();
static _i32 getResponse();
static _i32 getData();

void wifi_start();



#endif /* LIB_WIFI_H_ */
