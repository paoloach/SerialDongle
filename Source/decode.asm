///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR C/C++ Compiler V8.30.1.41636 for 8051            20/Sep/2017  18:35:20 /
// Copyright 2004-2013 IAR Systems AB.                                        /
//                                                                            /
//    Core               =  plain                                             /
//    Code model         =  banked                                            /
//    Data model         =  large                                             /
//    Calling convention =  xdata reentrant                                   /
//    Constant location  =  data_rom                                          /
//    Dptr setup         =  1,16                                              /
//                                                                            /
//    Source file        =  F:\Users\paolo\Documents\GitHub\SerialDongle\Sour /
//                          ce\decode.c                                       /
//    Command line       =  -f F:\Users\paolo\Documents\GitHub\SerialDongle\C /
//                          C2530DB\..\Tools\f8wCoord.cfg (-DCPU32MHZ         /
//                          -DROOT=__near_func -DMAC_CFG_APP_PENDING_QUEUE=TR /
//                          UE -DMAC_CFG_TX_DATA_MAX=5 -DMAC_CFG_TX_MAX=8     /
//                          -DMAC_CFG_RX_MAX=5 -DMT_ZDO_MGMT                  /
//                          -DZDO_COORDINATOR -DRTR_NWK) -f                   /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Tools\f8wConfig.cfg (-DZIGBEEPRO          /
//                          -DSECURE=1 -DZG_SECURE_DYNAMIC=0 -DREFLECTOR      /
//                          -DDEFAULT_CHANLIST=0x00000800                     /
//                          -DZDAPP_CONFIG_PAN_ID=0xA465                      /
//                          -DNWK_START_DELAY=100 -DEXTENDED_JOINING_RANDOM_M /
//                          ASK=0x007F -DBEACON_REQUEST_DELAY=100             /
//                          -DBEACON_REQ_DELAY_MASK=0x00FF                    /
//                          -DLINK_STATUS_JITTER_MASK=0x007F                  /
//                          -DROUTE_EXPIRY_TIME=30 -DAPSC_ACK_WAIT_DURATION_P /
//                          OLLED=3000 -DNWK_INDIRECT_MSG_TIMEOUT=7           /
//                          -DMAX_RREQ_ENTRIES=8 -DAPSC_MAX_FRAME_RETRIES=3   /
//                          -DNWK_MAX_DATA_RETRIES=2                          /
//                          -DMAX_POLL_FAILURE_RETRIES=2 -DMAX_BCAST=9        /
//                          -DAPS_MAX_GROUPS=4 -DMAX_RTG_ENTRIES=20           /
//                          -DNWK_MAX_BINDING_ENTRIES=1                       /
//                          -DMAX_BINDING_CLUSTER_IDS=1                       /
//                          "-DDEFAULT_KEY={0x01, 0x03, 0x05, 0x07, 0x09,     /
//                          0x0B, 0x0D, 0x0F, 0x00, 0x02, 0x04, 0x06, 0x08,   /
//                          0x0A, 0x0C, 0x0D}" -DMAC_MAX_FRAME_SIZE=116       /
//                          -DZDNWKMGR_MIN_TRANSMISSIONS=20 "-DCONST=const    /
//                          __code" -DGENERIC=__generic                       /
//                          -DRFD_RCVC_ALWAYS_ON=FALSE -DPOLL_RATE=1000       /
//                          -DQUEUED_POLL_RATE=100 -DRESPONSE_POLL_RATE=100   /
//                          -DREJOIN_POLL_RATE=440) -f                        /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Tools\f8wZCL.cfg                          /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\Sour /
//                          ce\decode.c -D HAL_PA_LNA -D SECURE=0 -D          /
//                          OSALMEM_METRICS -D REFLECTOR -D xPOWER_SAVE -D    /
//                          HAL_KEY=0 -D TC_LINKKEY_JOIN -D REFLECTOR -D      /
//                          NV_INIT -D NV_RESTORE -D xHOLD_AUTO_START -D      /
//                          xZTOOL_P1 -D xMT_TASK -D xMT_APP_FUNC -D          /
//                          xMT_SYS_FUNC -D xMT_ZDO_FUNC -D                   /
//                          MULTICAST_ENABLED=FALSE -D ZCL_READ -D ZCL_WRITE  /
//                          -D ZCL_REPORT -D ZCL_EZMODE -D ZCL_BASIC -D       /
//                          ZCL_IDENTIFY -D ZCL_ON_OFF -D ZCL_DIAGNOSTIC -D   /
//                          FEATURE_SYSTEM_STATS -lC                          /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\CoordinatorEB\List\ -lB                      /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\CoordinatorEB\List\ --diag_suppress          /
//                          Pe001,Pa010 -o F:\Users\paolo\Documents\GitHub\Se /
//                          rialDongle\CC2530DB\CoordinatorEB\Obj\ -e         /
//                          --debug --core=plain --dptr=16,1                  /
//                          --data_model=large --code_model=banked            /
//                          --calling_convention=xdata_reentrant              /
//                          --place_constants=data_rom --nr_virtual_regs 16   /
//                          -I F:\Users\paolo\Documents\GitHub\SerialDongle\C /
//                          C2530DB\ -I F:\Users\paolo\Documents\GitHub\Seria /
//                          lDongle\CC2530DB\..\Source\ -I                    /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Source\ha\ -I F:\Users\paolo\Documents\Gi /
//                          tHub\SerialDongle\CC2530DB\..\Source\zmain\ -I    /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\hal\include\ -I                /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\hal\target\CC2530EB\ -I        /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\hal\target\CC2531_USBDONGLE\   /
//                          -I F:\Users\paolo\Documents\GitHub\SerialDongle\C /
//                          C2530DB\..\Components\hal\target\CC2530USB\usb\cl /
//                          ass_UsbCoordinator\ -I F:\Users\paolo\Documents\G /
//                          itHub\SerialDongle\CC2530DB\..\Components\hal\tar /
//                          get\CC2530USB\usb\library\ -I                     /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\hal\target\CC2530USB\usb\libra /
//                          ry\cc2531\ -I F:\Users\paolo\Documents\GitHub\Ser /
//                          ialDongle\CC2530DB\..\Components\mac\include\ -I  /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\mac\high_level\ -I             /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\mac\low_level\srf04\ -I        /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\mac\low_level\srf04\single_chi /
//                          p\ -I F:\Users\paolo\Documents\GitHub\SerialDongl /
//                          e\CC2530DB\..\Components\mt\ -I                   /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\osal\include\ -I               /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\services\saddr\ -I             /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\services\sdata\ -I             /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\stack\af\ -I                   /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\stack\nwk\ -I                  /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\stack\sapi\ -I                 /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\stack\sec\ -I                  /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\stack\sys\ -I                  /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\stack\zcl\ -I                  /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\stack\zdo\ -I                  /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\zmac\ -I                       /
//                          F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\..\Components\zmac\f8w\ -Ohs                 /
//                          --require_prototypes                              /
//    List file          =  F:\Users\paolo\Documents\GitHub\SerialDongle\CC25 /
//                          30DB\CoordinatorEB\List\decode.s51                /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME decode

        RSEG DOVERLAY:DATA:NOROOT(0)
        RSEG IOVERLAY:IDATA:NOROOT(0)
        RSEG ISTACK:IDATA:NOROOT(0)
        RSEG PSTACK:XDATA:NOROOT(0)
        RSEG XSTACK:XDATA:NOROOT(0)

        EXTERN ?V0
        EXTERN ?V1
        EXTERN ?V2
        EXTERN ?V3
        EXTERN ?BANKED_ENTER_XDATA
        EXTERN ?BANKED_LEAVE_XDATA
        EXTERN ?BDISPATCH
        EXTERN __INIT_XDATA_Z

        PUBLIC ??serialProcessEvent?relay
        PUBLIC lastDataRead
        PUBLIC serialErrorCount
        PUBLIC serialProcessEvent

decodeActiveEndpointEvent SYMBOL "decodeActiveEndpointEvent"
??decodeActiveEndpointEvent?relay SYMBOL "?relay", decodeActiveEndpointEvent
decodeDeviceInfoEvent SYMBOL "decodeDeviceInfoEvent"
??decodeDeviceInfoEvent?relay SYMBOL "?relay", decodeDeviceInfoEvent
decodeIEEEEvents    SYMBOL "decodeIEEEEvents"
??decodeIEEEEvents?relay SYMBOL "?relay", decodeIEEEEvents
decodeINIT          SYMBOL "decodeINIT"
??decodeINIT?relay  SYMBOL "?relay", decodeINIT
decodeNodePowerEvents SYMBOL "decodeNodePowerEvents"
??decodeNodePowerEvents?relay SYMBOL "?relay", decodeNodePowerEvents
decodeRequestAttribute SYMBOL "decodeRequestAttribute"
??decodeRequestAttribute?relay SYMBOL "?relay", decodeRequestAttribute
decodeRequestAttributes SYMBOL "decodeRequestAttributes"
??decodeRequestAttributes?relay SYMBOL "?relay", decodeRequestAttributes
decodeRequestBindTable SYMBOL "decodeRequestBindTable"
??decodeRequestBindTable?relay SYMBOL "?relay", decodeRequestBindTable
decodeRequestBindUnBinf SYMBOL "decodeRequestBindUnBinf"
??decodeRequestBindUnBinf?relay SYMBOL "?relay", decodeRequestBindUnBinf
decodeSendComand    SYMBOL "decodeSendComand"
??decodeSendComand?relay SYMBOL "?relay", decodeSendComand
decodeSendWriteAttribute SYMBOL "decodeSendWriteAttribute"
??decodeSendWriteAttribute?relay SYMBOL "?relay", decodeSendWriteAttribute
`?<Name eventReset 22>` SYMBOL "eventReset"
??eventReset?relay  SYMBOL "?relay", `?<Name eventReset 22>`
osal_msg_allocate   SYMBOL "osal_msg_allocate"
??osal_msg_allocate?relay SYMBOL "?relay", osal_msg_allocate
serialProcessEvent  SYMBOL "serialProcessEvent"
??serialProcessEvent?relay SYMBOL "?relay", serialProcessEvent

        EXTERN ??decodeActiveEndpointEvent?relay
        EXTERN ??decodeDeviceInfoEvent?relay
        EXTERN ??decodeIEEEEvents?relay
        EXTERN ??decodeINIT?relay
        EXTERN ??decodeNodePowerEvents?relay
        EXTERN ??decodeRequestAttribute?relay
        EXTERN ??decodeRequestAttributes?relay
        EXTERN ??decodeRequestBindTable?relay
        EXTERN ??decodeRequestBindUnBinf?relay
        EXTERN ??decodeSendComand?relay
        EXTERN ??decodeSendWriteAttribute?relay
        EXTERN ??eventReset?relay
        EXTERN ??osal_msg_allocate?relay
        EXTERN decodeActiveEndpointEvent
        EXTERN decodeDeviceInfoEvent
        EXTERN decodeIEEEEvents
        EXTERN decodeINIT
        EXTERN decodeNodePowerEvents
        EXTERN decodeRequestAttribute
        EXTERN decodeRequestAttributes
        EXTERN decodeRequestBindTable
        EXTERN decodeRequestBindUnBinf
        EXTERN decodeSendComand
        EXTERN decodeSendWriteAttribute
        EXTERN osal_msg_allocate
        EXTERN rxData
        EXTERN rxDataRead

// F:\Users\paolo\Documents\GitHub\SerialDongle\Source\decode.c
//    1 #include "SerialProcessEvents.h"
//    2 #include "DataStructures.h"
//    3 #include "TimerEvents.h"
//    4 #include "SerialProcessEventsInternal.h"
//    5 
//    6 

        RSEG XDATA_Z:XDATA:NOROOT(0)
        DATA8
//    7 uint8 serialErrorCount;
serialErrorCount:
        DS 1
        REQUIRE __INIT_XDATA_Z

        RSEG XDATA_Z:XDATA:NOROOT(0)
        DATA16
//    8 char * lastDataRead;
lastDataRead:
        DS 2
        REQUIRE __INIT_XDATA_Z

        RSEG BANKED_CODE:CODE:NOROOT(0)
//    9 void serialProcessEvent(void) {
serialProcessEvent:
        CODE
        REQUIRE ?V0
        REQUIRE ?V1
        REQUIRE ?V2
        REQUIRE ?V3
        MOV     A,#-0xc
        LCALL   ?BANKED_ENTER_XDATA
        ; Saved register size: 12
        ; Auto size: 0
//   10 	char * tmp = rxDataRead;
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     ?V0,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     ?V1,A
		
		
		MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     R2,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     DPH,A
        MOV     DPL,R2
        
		
//   11 	if (*rxDataRead == 'I'){
        MOVX    A,@DPTR
        XRL     A,'I'
        JZ      $+5
        LJMP    ??serialProcessEvent_0 & 0xFFFF
//   12 		incDataRead;
			INC     DPTR
			CLR		C
			MOV		A,DPL
			SUBB    A,#(rxData + 128) & 0xff
			MOV		A,DPH
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      ??serialProcessEvent_1
			MOV     DPTR,#rxData
	//   13 		if (*rxDataRead == 'E'){
	??serialProcessEvent_1:
			MOVX    A,@DPTR
			XRL     A,'E'
			JZ      $+5
			LJMP    ??serialProcessEvent_2 & 0xFFFF
		//   14 			incDataRead;
				INC     DPTR
				CLR		C
				MOV		A,DPL
				SUBB    A,#(rxData + 128) & 0xff
				MOV		A,DPH
				SUBB    A,#((rxData + 128) >> 8) & 0xff
				JC      ??serialProcessEvent_3
				MOV     DPTR,#rxData
		//   15 			if (*rxDataRead == 'E'){
		??serialProcessEvent_3:
				MOVX    A,@DPTR
				XRL     A,'E'
				JZ      $+5
				LJMP    ??serialProcessEvent_4 & 0xFFFF
			//   16 				incDataRead;
					INC     DPTR
					CLR		C
					MOV		A,DPL
					SUBB    A,#(rxData + 128) & 0xff
					MOV		A,DPH
					SUBB    A,#((rxData + 128) >> 8) & 0xff
					JC      ??serialProcessEvent_5
					MOV     DPTR,#rxData
			//   17 				if (*rxDataRead == 'E'){
			??serialProcessEvent_5:
					MOVX    A,@DPTR
					XRL     A,'E'
					JZ      $+5
					LJMP    ??serialProcessEvent_4 & 0xFFFF
				//   18 					incDataRead;
						INC     DPTR
						CLR		C
						MOV		A,DPL
						SUBB    A,#(rxData + 128) & 0xff
						MOV		A,DPH
						SUBB    A,#((rxData + 128) >> 8) & 0xff
						JC      ??serialProcessEvent_6
						MOV     DPTR,#rxData
				//   19 					if (*rxDataRead == ':'){
				??serialProcessEvent_6:
						MOVX    A,@DPTR
						XRL     A,':'
						JZ      $+5
						LJMP    ??serialProcessEvent_4 & 0xFFFF
					//   20 						decodeIEEEEvents();
							; Setup parameters for call to function decodeIEEEEvents
							LCALL   ??decodeIEEEEvents?relay; Banked call to: decodeIEEEEvents
							LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   21 					}
	//   22 				}
	//   23 			}
	//   24 		} else if (*rxDataRead == 'N'){
	??serialProcessEvent_2:
			MOVX    A,@DPTR
			XRL     A,'N'
			JZ      $+5
			LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   25 			incDataRead;
				INC     DPTR
				CLR		C
				MOV		A,DPL
				SUBB    A,#(rxData + 128) & 0xff
				MOV		A,DPH
				SUBB    A,#((rxData + 128) >> 8) & 0xff
				JC      ??serialProcessEvent_7
					MOV     DPTR,#rxData
		//   26 			if (*rxDataRead == 'I'){
		??serialProcessEvent_7:
				MOVX    A,@DPTR
				XRL     A,'I'
				JZ      $+5
				LJMP    ??serialProcessEvent_4 & 0xFFFF
			//   27 				incDataRead;
					INC     DPTR
					CLR		C
					MOV		A,DPL
					SUBB    A,#(rxData + 128) & 0xff
					MOV		A,DPH
					SUBB    A,#((rxData + 128) >> 8) & 0xff
					JC      ??serialProcessEvent_8
						MOV     DPTR,#rxData
			//   28 				if (*rxDataRead == 'T'){
				??serialProcessEvent_8:
						MOVX    A,@DPTR
						XRL     A,'T'
						JZ      $+5
							LJMP    ??serialProcessEvent_4 & 0xFFFF
				//   29 					incDataRead;
						INC     DPTR
						CLR		C
						MOV		A,DPL
						SUBB    A,#(rxData + 128) & 0xff
						MOV		A,DPH
						SUBB    A,#((rxData + 128) >> 8) & 0xff
						JC      ??serialProcessEvent_9
							MOV     DPTR,#rxData
				//   30 					if (*rxDataRead == ':'){
				??serialProcessEvent_9:
							MOVX    A,@DPTR
							XRL     A,':'
							JZ      $+5
							LJMP    ??serialProcessEvent_4 & 0xFFFF
					//   31 						decodeINIT();
							; Setup parameters for call to function decodeINIT
								LCALL   ??decodeINIT?relay   ; Banked call to: decodeINIT
							LJMP    ??serialProcessEvent_4 & 0xFFFF
//   33 					}
//   34 				}
//   35 			}
//   36 		}
//   37 	} else if (*rxDataRead == 'N'){
??serialProcessEvent_0:
        MOVX     A,@dptr
        XRL     A,'N'
        JNZ     ??serialProcessEvent_11
	//   38 		incDataRead;
			INC     DPTR
			CLR		C
			MOV		A,DPL
			SUBB    A,#(rxData + 128) & 0xff
			MOV		A,DPH
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      ??serialProcessEvent_12
				MOV     DPTR,#rxData
	//   39 		if (*rxDataRead == 'P'){
	??serialProcessEvent_12:
			MOVX    A,@DPTR
			XRL     A,'P'
			JZ      $+5
			LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   40 			incDataRead;
				INC     DPTR
				CLR		C
				MOV		A,DPL
				SUBB    A,#(rxData + 128) & 0xff
				MOV		A,DPH
				SUBB    A,#((rxData + 128) >> 8) & 0xff
				JC      ??serialProcessEvent_13
					MOV     DPTR,#rxData
		//   41 			if (*rxDataRead == ':'){
		??serialProcessEvent_13:
				MOVX    A,@DPTR
				XRL     A,':'
				JZ      $+5
				LJMP    ??serialProcessEvent_4 & 0xFFFF
		//   42 				decodeNodePowerEvents();
					; Setup parameters for call to function decodeNodePowerEvents
					LCALL   ??decodeNodePowerEvents?relay; Banked call to: decodeNodePowerEvents
					LJMP    ??serialProcessEvent_4 & 0xFFFF
//   43 			}
//   44 		}
//   45 
//   46 	}	else if (*rxDataRead == 'A'){
??serialProcessEvent_11:
		MOVX    A,@DPTR
		XRL     A,'A'
        JNZ     ??serialProcessEvent_14
//   47 		incDataRead;
			INC     DPTR
			CLR		C
			MOV		A,DPL
			SUBB    A,#(rxData + 128) & 0xff
			MOV		A,DPH
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      $+5
				MOV     DPTR,#rxData
	//   48 		if (*rxDataRead == 'E'){
	??serialProcessEvent_15:
			MOVX    A,@DPTR
			XRL     A,'E'
			JZ      $+5
				LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   49 			incDataRead;
				INC     DPTR
				CLR		C
				MOV		A,DPL
				SUBB    A,#(rxData + 128) & 0xff
				MOV		A,DPH
				SUBB    A,#((rxData + 128) >> 8) & 0xff
				JC      $+5
					MOV     DPTR,#rxData
		//   50 			if (*rxDataRead == ':'){
				MOVX    A,@DPTR
				XRL     A,':'
				JZ      $+5
				LJMP    ??serialProcessEvent_4 & 0xFFFF
			//   51 				decodeActiveEndpointEvent();
					; Setup parameters for call to function decodeActiveEndpointEvent
					LCALL   ??decodeActiveEndpointEvent?relay; Banked call to: decodeActiveEndpointEvent
					LJMP    ??serialProcessEvent_4 & 0xFFFF
//   52 			}
//   53 		}
//   54 	} else if (*rxDataRead == 'B'){
??serialProcessEvent_14:
		MOVX	A,@DPTR
        XRL     A,'B'
        JZ      $+5
        LJMP    ??serialProcessEvent_17 & 0xFFFF
//   55 		incDataRead;
			INC     DPTR
			CLR		C
			MOV		A,DPL
			SUBB    A,#(rxData + 128) & 0xff
			MOV		A,DPH
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      $+5
				MOV     DPTR,#rxData
	//   56 		if (*rxDataRead == 'I'){
			MOVX    A,@DPTR
			XRL		A,'I'
			JNZ     ??serialProcessEvent_19
	//   57 			incDataRead;
				INC     DPTR
				CLR		C
				MOV		A,DPL
				SUBB    A,#(rxData + 128) & 0xff
				MOV		A,DPH
				SUBB    A,#((rxData + 128) >> 8) & 0xff
				JC      $+5
					MOV     DPTR,#rxData
		//   58 			if (*rxDataRead == ':'){
				MOVX    A,@DPTR
				XRL     A,':'
				JZ      $+5
				LJMP    ??serialProcessEvent_4 & 0xFFFF
		//   59 				decodeRequestBindUnBinf(1);
				; Setup parameters for call to function decodeRequestBindUnBinf
					MOV     R1,#0x1
					LJMP    ??serialProcessEvent_21 & 0xFFFF
//   60 			}
//   61 		} else if (*rxDataRead == 'T'){
??serialProcessEvent_19:
		MOVX	A,@DPTR
        XRL     A,'T'
        JZ      $+5
        LJMP    ??serialProcessEvent_4 & 0xFFFF
//   62 			incDataRead;
			INC     DPTR
			CLR		C
			MOV		A,DPL
			SUBB    A,#(rxData + 128) & 0xff
			MOV		A,DPH
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      $+5
				MOV     DPTR,#rxData
//   63 			if (*rxDataRead == ':'){
	??serialProcessEvent_22:
			MOVX    A,@DPTR
			XRL     A,':'
			JZ      $+5
			LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   64 				decodeRequestBindTable();
				; Setup parameters for call to function decodeRequestBindTable
				LCALL   ??decodeRequestBindTable?relay; Banked call to: decodeRequestBindTable
				LJMP    ??serialProcessEvent_4 & 0xFFFF
//   65 			}
//   66 		}
//   67 	} else if (*rxDataRead == 'D'){
??serialProcessEvent_17:
		MOVX	A,@DPTR
        XRL     A,'D'
        JNZ     ??serialProcessEvent_23
//   68 		incDataRead;
			INC     DPTR
			CLR		C
			MOV		A,DPL
			SUBB    A,#(rxData + 128) & 0xff
			MOV		A,DPH
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      $+5
				MOV     DPTR,#rxData
	//   69 		if (*rxDataRead == 'I'){
	??serialProcessEvent_24:
			
			MOVX    A,@DPTR
			XRL     A,'I'
			JZ      $+5
			LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   70 			incDataRead;
				INC     DPTR
				CLR		C
				MOV		A,DPL
				SUBB    A,#(rxData + 128) & 0xff
				MOV		A,DPH
				SUBB    A,#((rxData + 128) >> 8) & 0xff
				JC      $+5
					MOV     DPTR,#rxData
	//   71 			if (*rxDataRead == ':'){
				MOVX    A,@DPTR
				XRL     A,':'
				JZ      $+5
				LJMP    ??serialProcessEvent_4 & 0xFFFF
		//   72 				decodeDeviceInfoEvent();
				; Setup parameters for call to function decodeDeviceInfoEvent
					LCALL   ??decodeDeviceInfoEvent?relay; Banked call to: decodeDeviceInfoEvent
					LJMP    ??serialProcessEvent_4 & 0xFFFF
//   73 			}
//   74 		}
//   75 	} else if (*rxDataRead == 'R'){
??serialProcessEvent_23:
        MOVX	A,@DPTR
        XRL     A,'R'
        JZ      $+5
        LJMP    ??serialProcessEvent_26 & 0xFFFF
//   76 		incDataRead;
			INC     DPTR
			CLR		C
			MOV		A,DPL
			SUBB    A,#(rxData + 128) & 0xff
			MOV		A,DPH
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      $+5
				MOV     DPTR,#rxData
	//   77 		if (*rxDataRead == 'A'){
			MOVX    A,@DPTR
			XRL     A,'A'
			JZ      $+5
			LJMP    ??serialProcessEvent_28 & 0xFFFF
	//   78 			incDataRead;
				INC     DPTR
				CLR		C
				MOV		A,DPL
				SUBB    A,#(rxData + 128) & 0xff
				MOV		A,DPH
				SUBB    A,#((rxData + 128) >> 8) & 0xff
				JC      $+5
					MOV     DPTR,#rxData
	//   79 			if (*rxDataRead == ':'){
				MOVX    A,@DPTR
				XRL     A,':'
				JNZ     ??serialProcessEvent_30
		//   80 				decodeRequestAttribute();
				; Setup parameters for call to function decodeRequestAttribute
				LCALL   ??decodeRequestAttribute?relay; Banked call to: decodeRequestAttribute
				LJMP    ??serialProcessEvent_4 & 0xFFFF
		//   81 			} else if (*rxDataRead == 'S'){
		??serialProcessEvent_30:
				MOV     A,#0x53
				XRL     A,R0
				JZ      $+5
				LJMP    ??serialProcessEvent_4 & 0xFFFF
		//   82 				incDataRead;
				MOV     DPTR,#rxDataRead
				MOVX    A,@DPTR
				ADD     A,#0x1
				MOV     R0,A
				INC     DPTR
				MOVX    A,@DPTR
				ADDC    A,#0x0
				MOV     R1,A
				MOV     DPTR,#rxDataRead
				MOV     A,R0
				MOVX    @DPTR,A
				INC     DPTR
				MOV     A,R1
				MOVX    @DPTR,A
				MOV     DPTR,#rxDataRead
				CLR     C
				MOVX    A,@DPTR
				SUBB    A,#(rxData + 128) & 0xff
				INC     DPTR
				MOVX    A,@DPTR
				SUBB    A,#((rxData + 128) >> 8) & 0xff
				JC      ??serialProcessEvent_31
				MOV     DPTR,#rxDataRead
				MOV     A,#rxData & 0xff
				MOVX    @DPTR,A
				INC     DPTR
				MOV     A,#(rxData >> 8) & 0xff
				MOVX    @DPTR,A
		//   83 				if (*rxDataRead == ':'){
		??serialProcessEvent_31:
				MOV     DPTR,#rxDataRead
				MOVX    A,@DPTR
				MOV     R0,A
				INC     DPTR
				MOVX    A,@DPTR
				MOV     DPH,A
				MOV     DPL,R0
				MOVX    A,@DPTR
				XRL     A,#0x3a
				JZ      $+5
				LJMP    ??serialProcessEvent_4 & 0xFFFF
		//   84 					decodeRequestAttributes();
				; Setup parameters for call to function decodeRequestAttributes
				LCALL   ??decodeRequestAttributes?relay; Banked call to: decodeRequestAttributes
				LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   85 				}
	//   86 			}
	//   87 		} else if (*rxDataRead == 'E'){
	??serialProcessEvent_28:
			MOV     A,#0x45
			XRL     A,R0
			JZ      $+5
			LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   88 			incDataRead;
			MOV     DPTR,#rxDataRead
			MOVX    A,@DPTR
			ADD     A,#0x1
			MOV     R0,A
			INC     DPTR
			MOVX    A,@DPTR
			ADDC    A,#0x0
			MOV     R1,A
			MOV     DPTR,#rxDataRead
			MOV     A,R0
			MOVX    @DPTR,A
			INC     DPTR
			MOV     A,R1
			MOVX    @DPTR,A
			MOV     DPTR,#rxDataRead
			CLR     C
			MOVX    A,@DPTR
			SUBB    A,#(rxData + 128) & 0xff
			INC     DPTR
			MOVX    A,@DPTR
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      ??serialProcessEvent_32
			MOV     DPTR,#rxDataRead
			MOV     A,#rxData & 0xff
			MOVX    @DPTR,A
			INC     DPTR
			MOV     A,#(rxData >> 8) & 0xff
			MOVX    @DPTR,A
	//   89 			if (*rxDataRead == 'S'){
	??serialProcessEvent_32:
			MOV     DPTR,#rxDataRead
			MOVX    A,@DPTR
			MOV     R0,A
			INC     DPTR
			MOVX    A,@DPTR
			MOV     DPH,A
			MOV     DPL,R0
			MOVX    A,@DPTR
			XRL     A,#0x53
			JZ      $+5
			LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   90 				incDataRead;
			MOV     DPTR,#rxDataRead
			MOVX    A,@DPTR
			ADD     A,#0x1
			INC     R0
			INC     DPTR
			MOVX    A,@DPTR
			ADDC    A,#0x0
			MOV     R1,A
			MOV     DPTR,#rxDataRead
			MOV     A,R0
			MOVX    @DPTR,A
			INC     DPTR
			MOV     A,R1
			MOVX    @DPTR,A
			MOV     DPTR,#rxDataRead
			CLR     C
			MOVX    A,@DPTR
			SUBB    A,#(rxData + 128) & 0xff
			INC     DPTR
			MOVX    A,@DPTR
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      ??serialProcessEvent_33
			MOV     DPTR,#rxDataRead
			MOV     A,#rxData & 0xff
			MOVX    @DPTR,A
			INC     DPTR
			MOV     A,#(rxData >> 8) & 0xff
			MOVX    @DPTR,A
	//   91 				if (*rxDataRead == 'E'){
	??serialProcessEvent_33:
			MOV     DPTR,#rxDataRead
			MOVX    A,@DPTR
			MOV     R0,A
			INC     DPTR
			MOVX    A,@DPTR
			MOV     DPH,A
			MOV     DPL,R0
			MOVX    A,@DPTR
			XRL     A,#0x45
			JZ      $+5
			LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   92 					incDataRead;
			MOV     DPTR,#rxDataRead
			MOVX    A,@DPTR
			ADD     A,#0x1
			INC     R0
			INC     DPTR
			MOVX    A,@DPTR
			ADDC    A,#0x0
			MOV     R1,A
			MOV     DPTR,#rxDataRead
			MOV     A,R0
			MOVX    @DPTR,A
			INC     DPTR
			MOV     A,R1
			MOVX    @DPTR,A
			MOV     DPTR,#rxDataRead
			CLR     C
			MOVX    A,@DPTR
			SUBB    A,#(rxData + 128) & 0xff
			INC     DPTR
			MOVX    A,@DPTR
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      ??serialProcessEvent_34
			MOV     DPTR,#rxDataRead
			MOV     A,#rxData & 0xff
			MOVX    @DPTR,A
			INC     DPTR
			MOV     A,#(rxData >> 8) & 0xff
			MOVX    @DPTR,A
	//   93 					if (*rxDataRead == 'T'){
	??serialProcessEvent_34:
			MOV     DPTR,#rxDataRead
			MOVX    A,@DPTR
			MOV     R0,A
			INC     DPTR
			MOVX    A,@DPTR
			MOV     DPH,A
			MOV     DPL,R0
			MOVX    A,@DPTR
			XRL     A,#0x54
			JZ      $+5
			LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   94 						incDataRead;
			MOV     DPTR,#rxDataRead
			MOVX    A,@DPTR
			ADD     A,#0x1
			INC     R0
			INC     DPTR
			MOVX    A,@DPTR
			ADDC    A,#0x0
			MOV     R1,A
			MOV     DPTR,#rxDataRead
			MOV     A,R0
			MOVX    @DPTR,A
			INC     DPTR
			MOV     A,R1
			MOVX    @DPTR,A
			MOV     DPTR,#rxDataRead
			CLR     C
			MOVX    A,@DPTR
			SUBB    A,#(rxData + 128) & 0xff
			INC     DPTR
			MOVX    A,@DPTR
			SUBB    A,#((rxData + 128) >> 8) & 0xff
			JC      ??serialProcessEvent_35
			MOV     DPTR,#rxDataRead
			MOV     A,#rxData & 0xff
			MOVX    @DPTR,A
			INC     DPTR
			MOV     A,#(rxData >> 8) & 0xff
			MOVX    @DPTR,A
	//   95 						if (*rxDataRead == ':'){
	??serialProcessEvent_35:
			MOV     DPTR,#rxDataRead
			MOVX    A,@DPTR
			MOV     R0,A
			INC     DPTR
			MOVX    A,@DPTR
			MOV     DPH,A
			MOV     DPL,R0
			MOVX    A,@DPTR
			XRL     A,#0x3a
			JZ      $+5
			LJMP    ??serialProcessEvent_4 & 0xFFFF
	//   96 							struct UsbISR * msg = (struct UsbISR *)osal_msg_allocate(sizeof(struct UsbISR) );
			; Setup parameters for call to function osal_msg_allocate
			MOV     R2,#0x6
			MOV     R3,#0x0
			LCALL   ??osal_msg_allocate?relay; Banked call to: osal_msg_allocate
			MOV     DPL,R2
			MOV     DPH,R3
	//   97 							msg->msg.event = EVENT_SERIAL_CMD;
			MOV     A,#-0x20
			MOVX    @DPTR,A
	//   98 							msg->isr = eventReset;
			INC     DPTR
			INC     DPTR
			MOV     A,#??eventReset?relay & 0xff
			MOVX    @DPTR,A
			INC     DPTR
			MOV     A,#(??eventReset?relay >> 8) & 0xff
			LJMP    ??serialProcessEvent_36 & 0xFFFF
//   99 						}
//  100 					}
//  101 				}
//  102 			}
//  103 		}
//  104 	} else if (*rxDataRead == 'S'){
??serialProcessEvent_26:
        MOV     A,#0x53
        XRL     A,R2
        JNZ     ??serialProcessEvent_37
//  105 		incDataRead;
        MOV     DPTR,#rxDataRead
        MOV     A,R0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,R1
        MOVX    @DPTR,A
        MOV     DPTR,#rxDataRead
        CLR     C
        MOVX    A,@DPTR
        SUBB    A,#(rxData + 128) & 0xff
        INC     DPTR
        MOVX    A,@DPTR
        SUBB    A,#((rxData + 128) >> 8) & 0xff
        JC      ??serialProcessEvent_38
        MOV     DPTR,#rxDataRead
        MOV     A,#rxData & 0xff
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,#(rxData >> 8) & 0xff
        MOVX    @DPTR,A
//  106 		if (*rxDataRead == 'C'){
??serialProcessEvent_38:
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     R0,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     DPH,A
        MOV     DPL,R0
        MOVX    A,@DPTR
        XRL     A,#0x43
        JZ      $+5
        LJMP    ??serialProcessEvent_4 & 0xFFFF
//  107 			incDataRead;
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        ADD     A,#0x1
        INC     R0
        INC     DPTR
        MOVX    A,@DPTR
        ADDC    A,#0x0
        MOV     R1,A
        MOV     DPTR,#rxDataRead
        MOV     A,R0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,R1
        MOVX    @DPTR,A
        MOV     DPTR,#rxDataRead
        CLR     C
        MOVX    A,@DPTR
        SUBB    A,#(rxData + 128) & 0xff
        INC     DPTR
        MOVX    A,@DPTR
        SUBB    A,#((rxData + 128) >> 8) & 0xff
        JC      ??serialProcessEvent_39
        MOV     DPTR,#rxDataRead
        MOV     A,#rxData & 0xff
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,#(rxData >> 8) & 0xff
        MOVX    @DPTR,A
//  108 			if (*rxDataRead == ':'){
??serialProcessEvent_39:
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     R0,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     DPH,A
        MOV     DPL,R0
        MOVX    A,@DPTR
        XRL     A,#0x3a
        JZ      $+5
        LJMP    ??serialProcessEvent_4 & 0xFFFF
//  109 				decodeSendComand();
        ; Setup parameters for call to function decodeSendComand
        LCALL   ??decodeSendComand?relay; Banked call to: decodeSendComand
        LJMP    ??serialProcessEvent_4 & 0xFFFF
//  110 			}
//  111 		}
//  112 	} else if (*rxDataRead == 'U'){
??serialProcessEvent_37:
        MOV     A,#0x55
        XRL     A,R2
        JZ      $+5
        LJMP    ??serialProcessEvent_40 & 0xFFFF
//  113 		incDataRead;
        MOV     DPTR,#rxDataRead
        MOV     A,R0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,R1
        MOVX    @DPTR,A
        MOV     DPTR,#rxDataRead
        CLR     C
        MOVX    A,@DPTR
        SUBB    A,#(rxData + 128) & 0xff
        INC     DPTR
        MOVX    A,@DPTR
        SUBB    A,#((rxData + 128) >> 8) & 0xff
        JC      ??serialProcessEvent_41
        MOV     DPTR,#rxDataRead
        MOV     A,#rxData & 0xff
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,#(rxData >> 8) & 0xff
        MOVX    @DPTR,A
//  114 		if (*rxDataRead == 'B'){
??serialProcessEvent_41:
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     R0,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     DPH,A
        MOV     DPL,R0
        MOVX    A,@DPTR
        XRL     A,#0x42
        JZ      $+5
        LJMP    ??serialProcessEvent_4 & 0xFFFF
//  115 			incDataRead;
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        ADD     A,#0x1
        INC     R0
        INC     DPTR
        MOVX    A,@DPTR
        ADDC    A,#0x0
        MOV     R1,A
        MOV     DPTR,#rxDataRead
        MOV     A,R0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,R1
        MOVX    @DPTR,A
        MOV     DPTR,#rxDataRead
        CLR     C
        MOVX    A,@DPTR
        SUBB    A,#(rxData + 128) & 0xff
        INC     DPTR
        MOVX    A,@DPTR
        SUBB    A,#((rxData + 128) >> 8) & 0xff
        JC      ??serialProcessEvent_42
        MOV     DPTR,#rxDataRead
        MOV     A,#rxData & 0xff
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,#(rxData >> 8) & 0xff
        MOVX    @DPTR,A
//  116 			if (*rxDataRead == 'I'){
??serialProcessEvent_42:
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     R0,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     DPH,A
        MOV     DPL,R0
        MOVX    A,@DPTR
        XRL     A,#0x49
        JZ      $+5
        LJMP    ??serialProcessEvent_4 & 0xFFFF
//  117 				incDataRead;
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        ADD     A,#0x1
        INC     R0
        INC     DPTR
        MOVX    A,@DPTR
        ADDC    A,#0x0
        MOV     R1,A
        MOV     DPTR,#rxDataRead
        MOV     A,R0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,R1
        MOVX    @DPTR,A
        MOV     DPTR,#rxDataRead
        CLR     C
        MOVX    A,@DPTR
        SUBB    A,#(rxData + 128) & 0xff
        INC     DPTR
        MOVX    A,@DPTR
        SUBB    A,#((rxData + 128) >> 8) & 0xff
        JC      ??serialProcessEvent_43
        MOV     DPTR,#rxDataRead
        MOV     A,#rxData & 0xff
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,#(rxData >> 8) & 0xff
        MOVX    @DPTR,A
//  118 				if (*rxDataRead == ':'){
??serialProcessEvent_43:
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     R0,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     DPH,A
        MOV     DPL,R0
        MOVX    A,@DPTR
        XRL     A,#0x3a
        JZ      $+5
        LJMP    ??serialProcessEvent_4 & 0xFFFF
//  119 					decodeRequestBindUnBinf(0);
        ; Setup parameters for call to function decodeRequestBindUnBinf
        MOV     R1,#0x0
??serialProcessEvent_21:
        LCALL   ??decodeRequestBindUnBinf?relay; Banked call to: decodeRequestBindUnBinf
        LJMP    ??serialProcessEvent_4 & 0xFFFF
//  120 				}
//  121 			}
//  122 		}
//  123 	} else if (*rxDataRead == 'W'){
??serialProcessEvent_40:
        MOV     A,#0x57
        XRL     A,R2
        JZ      $+5
        LJMP    ??serialProcessEvent_44 & 0xFFFF
//  124 		incDataRead;
        MOV     DPTR,#rxDataRead
        MOV     A,R0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,R1
        MOVX    @DPTR,A
        MOV     DPTR,#rxDataRead
        CLR     C
        MOVX    A,@DPTR
        SUBB    A,#(rxData + 128) & 0xff
        INC     DPTR
        MOVX    A,@DPTR
        SUBB    A,#((rxData + 128) >> 8) & 0xff
        JC      ??serialProcessEvent_45
        MOV     DPTR,#rxDataRead
        MOV     A,#rxData & 0xff
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,#(rxData >> 8) & 0xff
        MOVX    @DPTR,A
//  125 		if (*rxDataRead == 'A'){
??serialProcessEvent_45:
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     R0,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     DPH,A
        MOV     DPL,R0
        MOVX    A,@DPTR
        XRL     A,#0x41
        JZ      $+5
        LJMP    ??serialProcessEvent_4 & 0xFFFF
//  126 			incDataRead;
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        ADD     A,#0x1
        INC     R0
        INC     DPTR
        MOVX    A,@DPTR
        ADDC    A,#0x0
        MOV     R1,A
        MOV     DPTR,#rxDataRead
        MOV     A,R0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,R1
        MOVX    @DPTR,A
        MOV     DPTR,#rxDataRead
        CLR     C
        MOVX    A,@DPTR
        SUBB    A,#(rxData + 128) & 0xff
        INC     DPTR
        MOVX    A,@DPTR
        SUBB    A,#((rxData + 128) >> 8) & 0xff
        JC      ??serialProcessEvent_46
        MOV     DPTR,#rxDataRead
        MOV     A,#rxData & 0xff
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,#(rxData >> 8) & 0xff
        MOVX    @DPTR,A
//  127 			if (*rxDataRead == 'I'){
??serialProcessEvent_46:
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     R0,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     DPH,A
        MOV     DPL,R0
        MOVX    A,@DPTR
        XRL     A,#0x49
        JNZ     ??serialProcessEvent_4
//  128 				incDataRead;
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        ADD     A,#0x1
        INC     R0
        INC     DPTR
        MOVX    A,@DPTR
        ADDC    A,#0x0
        MOV     R1,A
        MOV     DPTR,#rxDataRead
        MOV     A,R0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,R1
        MOVX    @DPTR,A
        MOV     DPTR,#rxDataRead
        CLR     C
        MOVX    A,@DPTR
        SUBB    A,#(rxData + 128) & 0xff
        INC     DPTR
        MOVX    A,@DPTR
        SUBB    A,#((rxData + 128) >> 8) & 0xff
        JC      ??serialProcessEvent_47
        MOV     DPTR,#rxDataRead
        MOV     A,#rxData & 0xff
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,#(rxData >> 8) & 0xff
        MOVX    @DPTR,A
//  129 				if (*rxDataRead == ':'){
??serialProcessEvent_47:
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     R0,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     DPH,A
        MOV     DPL,R0
        MOVX    A,@DPTR
        XRL     A,#0x3a
        JNZ     ??serialProcessEvent_4
//  130 					decodeSendWriteAttribute();
        ; Setup parameters for call to function decodeSendWriteAttribute
        LCALL   ??decodeSendWriteAttribute?relay; Banked call to: decodeSendWriteAttribute
        SJMP    ??serialProcessEvent_4
//  131 				}
//  132 			}
//  133 		}
//  134 	} else {
//  135 		serialErrorCount++;
??serialProcessEvent_44:
        MOV     DPTR,#serialErrorCount
        MOVX    A,@DPTR
        INC     A
        MOVX    @DPTR,A
//  136 		incDataRead;
        MOV     DPTR,#rxDataRead
        MOV     A,R0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,R1
        MOVX    @DPTR,A
        MOV     DPTR,#rxDataRead
        CLR     C
        MOVX    A,@DPTR
        SUBB    A,#(rxData + 128) & 0xff
        INC     DPTR
        MOVX    A,@DPTR
        SUBB    A,#((rxData + 128) >> 8) & 0xff
        JC      ??serialProcessEvent_4
??serialProcessEvent_10:
        MOV     DPTR,#rxDataRead
        MOV     A,#rxData & 0xff
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,#(rxData >> 8) & 0xff
??serialProcessEvent_36:
        MOVX    @DPTR,A
//  137 	}
//  138 	if(*rxDataRead != '\n'){
??serialProcessEvent_4:
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        MOV     R0,A
        INC     DPTR
        MOVX    A,@DPTR
        MOV     DPH,A
        MOV     DPL,R0
        MOVX    A,@DPTR
        XRL     A,#0xa
        JZ      ??serialProcessEvent_48
//  139 		serialErrorCount++;
        MOV     DPTR,#serialErrorCount
        MOVX    A,@DPTR
        INC     A
        MOVX    @DPTR,A
//  140 	}
//  141 	incDataRead;
??serialProcessEvent_48:
        MOV     DPTR,#rxDataRead
        MOVX    A,@DPTR
        ADD     A,#0x1
        INC     R0
        INC     DPTR
        MOVX    A,@DPTR
        ADDC    A,#0x0
        MOV     R1,A
        MOV     DPTR,#rxDataRead
        MOV     A,R0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,R1
        MOVX    @DPTR,A
        MOV     DPTR,#rxDataRead
        CLR     C
        MOVX    A,@DPTR
        SUBB    A,#(rxData + 128) & 0xff
        INC     DPTR
        MOVX    A,@DPTR
        SUBB    A,#((rxData + 128) >> 8) & 0xff
        JC      ??serialProcessEvent_49
        MOV     DPTR,#rxDataRead
        MOV     A,#rxData & 0xff
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,#(rxData >> 8) & 0xff
        MOVX    @DPTR,A
//  142 	if (serialErrorCount == 0)
??serialProcessEvent_49:
        MOV     DPTR,#serialErrorCount
        MOVX    A,@DPTR
        JNZ     ??serialProcessEvent_50
//  143 		lastDataRead = tmp;
        MOV     DPTR,#lastDataRead
        MOV     A,?V0
        MOVX    @DPTR,A
        INC     DPTR
        MOV     A,?V1
        MOVX    @DPTR,A
//  144 }
??serialProcessEvent_50:
        MOV     R7,#0x4
        LJMP    ?BANKED_LEAVE_XDATA

        RSEG BANK_RELAYS:CODE:REORDER:NOROOT(0)
??serialProcessEvent?relay:
        CODE
        LCALL   ?BDISPATCH
        DATA24
        DC24    serialProcessEvent

        END
// 
// 2 198 bytes in segment BANKED_CODE
//     6 bytes in segment BANK_RELAYS
//     3 bytes in segment XDATA_Z
// 
// 2 204 bytes of CODE  memory
//     3 bytes of XDATA memory
//
//Errors: none
//Warnings: none
