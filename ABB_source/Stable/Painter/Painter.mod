MODULE Painter
    !***********************************************************
    !
    ! Module:  Painter
    !
    ! Description:
    !   Painting module for the ABB IRB120 
    !
    ! Author: drongla, crookjj, doughezj, horvegc
    !
    ! Version: 0.3a - For Brush Cleaner v0.2
    !
    !***********************************************************
    
    
  

    ! *** Constants ***
    ! Commonly Tweaked declarations
    CONST num brushLength:=200;
    CONST num paintHeight:=5;
    CONST num cleanerHeight:=100; ! TODO: actually find out what this is. 
    !CONST num dryerHeight:=100; ! TODO: actually find out what this is. 
    CONST num canvasHeight:=0;
    CONST num PAINT_MAX_DIST:=50;
    ! Canvas size Declarations
    ! * The largest usable square area the robot can draw in is 500mm wide by 150mm tall
    ! * This is a rectangular large canvas, about 19.6" by 9.8"
    CONST num canvasXmin:=400;
    CONST num canvasXmax:=650;
    CONST num canvasYmin:=-250;
    CONST num canvasYmax:=250;  
    ! Used in the conversion of pixels to mm on the canvas
    CONST num XOffset:=260;
    CONST num YOffset:=-150;
    ! Scaling factor for when we load an image (Default 0.5)
    VAR num SF:=0.5;
    ! Orientation constants
    VAR orient ZeroZeroQuat:=[0.70427,0.02028,0.71115,0.01996];    
       
    ! Describes the paintbrush location. TODO: verify with metric calipers. 
    PERS tooldata paintBrush:=[TRUE,[[87,0,146],[1,0,0,0]],[0.2,[0,0,146],[0,0,1,0],0,0,0]];
    ! *** Variables ***  
    VAR iodev iodev1;
    VAR rawbytes rawMsgData;
    ! Store image size, in pixels
    VAR num sizeX;
    VAR num sizeY;
    VAR num XTGT:=0;
    ! X target
    VAR num YTGT:=0;
    ! Y Target
    VAR num lastX:=500;
    VAR num lastY:=0;
    ! processing coordinates
    VAR num tX;
    VAR num tY;
    !

    VAR num vX;
    VAR num vY;

    VAR num vectorMag;

    ! Locations of the painting targets. 
    VAR orient paintStrokeQuat:=[0.70427,0.02028,0.71115,0.01996]; 
    VAR orient paintCupQuat:=[0.70427,0.02028,0.71115,0.01996]; 
    VAR orient paintcleanerQuat:=[0.51854, 0.50842, 0.49217, -0.47999];
    ! Change these in procedure: initializeColors
    VAR robtarget overA;
    VAR robtarget colorA;

    VAR robtarget overB;
    VAR robtarget colorB;

    VAR robtarget overC;
    VAR robtarget colorC;

    VAR robtarget overD;
    VAR robtarget colorD;

    VAR robtarget overE;
    VAR robtarget colorE;

    VAR robtarget overF;
    VAR robtarget colorF;

    VAR robtarget overG;
    VAR robtarget colorG;

    VAR robtarget overH;
    VAR robtarget colorH;
    
    VAR robtarget approachClean;
    VAR robtarget overClean;
    VAR robtarget clean;
    VAR robtarget overDryer;
    VAR robtarget dryer;
    VAR robtarget dryerL;
    VAR robtarget dryerH;
    !    
    VAR bool newStroke;
    VAR num distanceTravelled := 0;
    ! UI Variables/Constants
    VAR btnres answer;
    CONST string my_message{5}:= ["Please check and verify the following:","- The serial cable is connected to COM1", "- The PC is connected to the serial cable","- BobRoss is running on the PC","- BobRoss has opened the serial channel on the PC"];
    CONST string my_buttons{5}:=["Ready","Clean", "Super-Dry","Home", "Park"];
    ! First-loop flags
    VAR bool firstTimeRun := TRUE;
    VAR string currentColor:= "A";
    VAR string lastColor := "A";
    VAR num serialTimeout := 25;
    VAR bool doubleDip := FALSE ;
    VAR bool brushClean := FALSE; 
    VAR bool brushDirty := FALSE;
    VAR bool isRotated := FALSE;
    
    !***********************************************************
    !
    ! Procedure main
    !
    !   This is the entry point of your program
    !
    !***********************************************************
    PROC main()
        ConfL\Off;
        initializeColors;
            
        answer:= UIMessageBox(
            \Header:="Pre-Paint Com Checks"
            \MsgArray:=my_message
            \BtnArray:=my_buttons
            \Icon:=iconInfo);
        IF answer = 1 THEN
            ! Operator said ready
            paintProgram;
        ELSEIF answer = 2 THEN 
            ! operator said clean
            MoveL approachClean, v500,z50,paintBrush;
            cleanCycle 1;
            MoveL approachClean, v500,z50,paintBrush;
            MoveL [[400,-250,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z50,paintBrush;
        ELSEIF answer = 3 THEN 
            MoveL approachClean, v500,z50,paintBrush;
            MoveL overDryer,v500,z20,paintBrush;
            MoveL dryer,v100,fine,paintBrush;
            WaitTime 3;
            MoveL overDryer,v500,z50,paintBrush;
            MoveL approachClean, v500,z50,paintBrush;
            MoveL [[400,-250,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z50,paintBrush;
        ELSEIF answer = 4 THEN 
            MoveL [[400,-250,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z50,paintBrush;
        ELSE 
            ! Parking spot for storage. 
            MoveL [[489,0,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z50,paintBrush;
            Stop;
        ENDIF      
       

    ENDPROC
    
    !***********************************************************
    !
    ! Procedure paintProgram
    !
    !   This initializes our program and immediately looks for SIZE to be set by serial commands. 
    !
    !***********************************************************    
    PROC paintProgram()
        VAR bool result;
        VAR string response;
        VAR num splitnum;
        VAR string directive;
        VAR string params;
        VAR num endTokenPos;
        
        Open "COM1:", iodev1 \Bin;
        ClearIOBuff iodev1;
        ClearRawBytes rawMsgData;
        WaitTime 0.1;                 
        WriteStrBin iodev1, "\05";
        response := readSerial();
        WHILE response = "" DO
            WriteStrBin iodev1, "\05";
            response := readSerial();
        ENDWHILE
            TPWrite response;
        
        ! Slice this up into directive and parameters
        endTokenPos:=StrFind(response, 1, ";");
        IF endTokenPos > StrLen(response) THEN
            throwError "semicolon", response;
        ELSE            
            response:=StrPart(response,1, endTokenPos-1); ! trim string to ignore end token.
        ENDIF
        splitNum := StrFind(response, 1, ":");
        ! note: StrPart( string, startIndexInclusive, length)
        directive := StrPart(response, 1, splitNum - 1); ! We don't care about the ':'
        params := StrPart(response, splitNum+1, Strlen(response) - (splitnum + 1) + 1);
        
        IF directive = "SIZE" THEN
            ! Expected 'response' to be SIZE:X400,Y200;
            result:=readSize(params);
            IF result = TRUE THEN
                ! do other stuff! WoohoooooooooO!OOO!O!O!O!OO!O!O!O
                WriteStrBin iodev1, "\06";
                paintLoop;  ! When this is called for the first time, it will be after obtaining the image size. 
            ELSE 
                WriteStrBin iodev1, "\15";
            ENDIF 
        ELSE 
            WriteStrBin iodev1, "\15";
        ENDIF
        Close iodev1;
    ENDPROC

    !***********************************************************
    !
    ! Procedure initializeColors
    !
    !   This sets up all targets in the program. 
    !
    !***********************************************************
    PROC initializeColors()
        overA:=[[276,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorA:=[[276,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        
        overB:=[[326,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorB:=[[326,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        
        overC:=[[376,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorC:=[[376,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

        overD:=[[426,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorD:=[[426,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

        overE:=[[476,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorE:=[[476,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

        overF:=[[526,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorF:=[[526,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

        overG:=[[576,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorG:=[[576,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

        overH:=[[626,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorH:=[[626,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        
        approachClean:=[[465,-290,350],paintcleanerQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        overClean:=[[452.3,-482.4,350],paintcleanerQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        clean:=[[452.3,-482.4,272],paintcleanerQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        
        overDryer:=[[225.9,-476,350],paintcleanerQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        dryer:=[[225.9,-476,166.5],paintcleanerQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        dryerL:=[[225.9,-476,163.6],paintcleanerQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        dryerH:=[[225.9,-476,169.3],paintcleanerQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        firstTimeRun := TRUE;
    ENDPROC
    !***********************************************************
    !
    ! function processCoordinates(string)
    !           String: "x:123,y:456'
    !           Returns TRUE or FALSE if it passes or fails. 
    !
    !   This takes a string and puts the x, y into globals tX and tY
    !
    !***********************************************************
    FUNC bool processCoordinates(string parameters, bool contiguous)
        ! exampleParams are X:230,Y:170;
        ! note: it may or may not contain a semicolon
        ! Slice this up into directive and parameters
        
        VAR num splitNum;
        
        ! note: StrPart( string, startIndexInclusive, length)
        VAR bool ok;
        VAR bool result;
        VAR string dA;
        VAR string dB ;
        VAR num dataIndex ;
        VAR string dAtype;
        VAR string dBtype;
        VAR num dAval;
        VAR num dBval;
        VAR num specialCheckIndex;
        VAR string remainingMessage;
        VAR bool multiCoordMessage;
        
        result  := TRUE;
        
        multiCoordMessage := FALSE;
        splitNum:= StrFind(parameters, 1, ",");
        dA := StrPart(parameters, 1, splitNum - 1); ! Should be X:230 - We don't care about the ','
        dataIndex := StrFind(dA, 1, ":"); ! finding the ':' in X:230
        
        
        IF dataIndex < StrLen(dA) THEN
            ! continue processing
            dAtype := StrPart(dA, 1, 1); ! Should be X
            ok:=StrToVal(StrPart(dA, 3, StrLen(dA)-2), dAval); ! should trim the X: from X:230 and parse 230
            IF ok = TRUE THEN 
                IF dAtype = "X" OR dAtype = "x" THEN                
                    tX:=dAval;
                ELSE 
                    tY:=dAval;
                ENDIF 
            ELSE 
                ! throw error!
                throwError "coord", dA;
                
                result := FALSE;
            endif
            
            dB:= StrPart(parameters, splitNum + 1, StrLen(parameters) - (splitNum+ 1) + 1); ! should contain Y:170 
            dataIndex := StrFind(dB, 1, ":"); ! finding the ':' in Y:170
            specialCheckIndex:= StrFind(db, 1, ",");
            if (specialCheckIndex < StrLen(db)) THEN ! TODO: This does not currently work!! RUH_ROH
            ! Oh boy, We have a list of coords!
                dB:= StrPart(parameters, splitNum+1, specialCheckIndex  - (splitNum) );
                remainingMessage := StrPart(parameters, specialCheckIndex+1, StrLen(parameters) - (specialCheckIndex) + 1);
                TPWrite "Multi-coord message:";
                TPWrite remainingMessage;
                multiCoordMessage:=TRUE;
            endif 
            
            IF dataIndex < StrLen(dB) THEN 
                            ! continue processing
                dBtype := StrPart(dB, 1, 1); ! Should be X
                ok:=StrToVal(StrPart(dB, 3, StrLen(dB) - 3 + 1), dBval);
                IF ok = TRUE THEN 
                    IF dBtype = "X" OR dBtype = "x" THEN
                        IF dAtype = "Y" OR dAtype = "y" THEN 
                        tX:=dBval;
                        ok := TRUE;
                        ELSE
                            ok := FALSE;
                        ENDIF 
                    ELSE 
                        IF dAtype = "X" OR dAtype = "x" THEN 
                        tY:=dBval;
                        ok := TRUE;
                        ELSE 
                            ok:= FALSE;
                        ENDIF 
                    ENDIF 
                ELSE 
                    ! throw error!
                throwError "coord", dB;
                result := FALSE;
                ok:= TRUE;
                endif
                ! check and see if OK changed to false again.
                IF ok = FALSE THEN
                    throwError "multicoord", parameters;                    
                    result := FALSE;
                ENDIF 
            ELSE
                ! we have a problem. X or Y not found
                    throwError "missingcoord", parameters;  
                    result := FALSE;
            ENDIF 
        ELSE 
            ! we have a problem. Throw an error. 
            throwError "missingcoord", parameters;  
            
            result := FALSE;
        ENDIF
        
        ! NOTE: Recursion here! Warning: this may or may not be good. TODO: test this
        !IF multiCoordMessage = TRUE AND result = TRUE THEN 
        !    result := processCoordinates(remainingMessage, TRUE);
        !ENDIF 
        
        RETURN result;
    endfunc 
    
    !***********************************************************
    !
    ! function result readSize(string)
    !           String: "x:123,y:456"
    !           Returns TRUE or FALSE if it passes or fails. 
    !
    !   Reads the size off the passed file
    !
    !***********************************************************    
    FUNC bool readSize(string parameters)   
        
        VAR bool result;
        result := processCoordinates(parameters, FALSE);
        IF result = TRUE THEN
            ! are we over the size constraints and in need of a scaling factor?
            IF tX > tY THEN 
                isRotated := TRUE;
                sizeY := tX;
                sizeX := tY;                
            else 
                sizeX := tX;
                sizeY := tY;   
            ENDIF

            
            IF sizeX <= 5 OR sizeY <= 5 THEN
                throwError "canvas", parameters;
            ENDIF 
            

            
            IF (sizeX > (canvasXmax-canvasXmin)) OR (sizeY > (canvasYmax-canvasYmin))THEN
                ! the Y proportion should be the scaling factor, as it was the smaller number
                IF ((canvasXmax-canvasXmin)/sizeX) > ((canvasYmax-canvasYmin)/sizeY) THEN
                    SF:=(canvasYmax-canvasYmin)/sizeY;
                ELSE
                    SF:=(canvasXmax-canvasXmin)/sizeX;
                ENDIF
                    
            ELSE
                throwError "canvas", parameters;  
            ENDIF 
            
            
                
        ELSE
            throwError "canvas", parameters;  
            
        ENDIF
        ! Just in case of trouble later on.         
        XTGT:=(SF*tX)+canvasXmin;
        YTGT:=(SF*tY)+canvasYmin;
        RETURN result;
    ENDFUNC 
    !***********************************************************
    !
    ! function result readSerial()
    !           Returns string on completion. 
    !
    !   reads from the serial channel, returning commands sent. 
    ! Note: this effectively never times out. 
    !
    !***********************************************************        
    func string readSerial()
        VAR bool passed := TRUE;
        VAR string response;
        VAR rawBytes rawMsgData;
        ReadRawBytes iodev1, rawMsgData \Time:=serialTimeout;   
           
        ERROR
            IF ERRNO = ERR_DEV_MAXTIME THEN
                ! do something
                passed := FALSE;
                RETURN "";
            ENDIF        
        IF passed = TRUE THEN 
            UnpackRawBytes rawMsgData, 1, response \ASCII:=(RawBytesLen(rawMsgData));
            ClearIOBuff iodev1; 
            
            RETURN response;
             
            
        ELSE 
            RETURN "";
        ENDIF 
    ENDFUNC
    
    !***********************************************************
    !
    ! function result paintLoop()
    !           Returns FALSE on completion
    !
    !   loops and reads from the serial channel, and passes the commands to subroutines.
    !
    !***********************************************************    
    proc paintLoop()
        ! TODO: Properly handle extra colons and Test this. Warining: Changed directives without parameters to not include the colon ':' character. Changed how this case was handled.
        VAR bool loop := TRUE;
        VAR string response;
        VAR num splitNum;
        VAR string directive;
        VAR string params;
        VAR num timeoutCounter:= 0;
        
        VAR num endTokenPos;
        WHILE loop = TRUE DO
            timeoutCounter := 0;
            response := readSerial();
            WHILE response = "" DO
                response := readSerial();
                timeoutCounter:= timeoutCounter + 1;
                IF timeoutCounter > 500 THEN
                    TPWrite "Aborting: Timeout";
                    moveToFinish;
                    Break;
                ENDIF
            ENDWHILE
            serialTimeout := 0.1;
            TPWrite response;
            
            endTokenPos:=StrFind(response, 1, ";");
            IF endTokenPos > StrLen(response) THEN
                throwError "semicolon", response;
            ELSE            
                response:=StrPart(response,1, endTokenPos-1); ! trim string to ignore end token.
            ENDIF
        
            ! Slice this up into directive and parameters
            splitNum := StrFind(response, 1, ":");
            ! note: StrPart( string, startIndexInclusive, length)
            IF splitNum > StrLen(response) THEN 
                
                loop:=directiveNoParams(response);
                
            ELSEIF splitNum < StrLen(response) THEN 
                directive := StrPart(response, 1, splitNum - 1); ! We don't care about the ':'
                params := StrPart(response, splitNum+1, Strlen(response) - (splitNum + 1) + 1);  
                
                loop:= directiveWithParams(directive, params);
            ELSE 
                WriteStrBin iodev1, "\15";
                ErrLog 4800, "Command Error", "Colon at end of message is extraneous",response,"-","-";
                TPWrite "Command Error: Extra colon at command termination";
                
                loop:= FALSE;
            ENDIF 
            
        ENDWHILE
        
    endproc
    !***********************************************************
    !
    ! function result directiveNoParams(string)
    !           String: "NEXT" or "END"
    !           Returns TRUE or FALSE if it passes or fails. 
    !
    !   does the asssociated directive and echoes back what it has done.
    !
    !***********************************************************    
    FUNC bool directiveNoParams(string directive)
        ! TODO: Test this
        IF directive = "NEXT" THEN 
            WriteStrBin iodev1, "\06";
            newStroke:=TRUE;
            
            RETURN TRUE;
            
        ELSEIF directive = "END" THEN 
            WriteStrBin iodev1, "\06";
            moveToFinish;
            RETURN FALSE;
        ELSE 
            throwError "unknown", directive;  
            
            RETURN FALSE;
        ENDIF 
            
    ENDFUNC
    !***********************************************************
    !
    ! function result directiveWithParams(directive, params)
    !           directive: "COORD" or "SWAP"
    !           params: "X:NUM,Y:NUM" or a character in range A-F inclusive.
    !           Returns TRUE or FALSE if it passes or fails. 
    !
    !   does the asssociated directive and echoes OK or FAILED
    !
    !***********************************************************  
    FUNC bool directiveWithParams(string directive, string params)
        VAR bool result;
        VAR bool testCheck;
        VAR num tempA;
        VAR num tempB;
        IF directive = "COORD" THEN 
            result := processCoordinates(params, FALSE);
            testCheck:=true;
            doubleDip := FALSE;

            WriteStrBin iodev1, "\06";
                
            IF isRotated THEN 
                tempA := tX;
                tempB := tY;  
                tX:= tempB;
                tY:= tempA; 
            ENDIF
            testCheck:=checkForBadPoints( tX,tY);
            XTGT:=(SF*tX)+canvasXmin;
            YTGT:=(SF*tY)+canvasYmin;
            moveToXY XTGT,YTGT;
            ! After moving, update our case. This ensures that we are starting new strokes
            ! correctly if NEXT was called before this
            newStroke:=FALSE;
            
            firstTimeRun := FALSE;
            
            IF testCheck = TRUE THEN 
            !WriteStrBin iodev1, "\06";
            
            
            RETURN TRUE;
            ELSE 
                WriteStrBin iodev1, "\15";
                RETURN FALSE;
            ENDIF
            
             
            
        ELSEIF directive = "SWAP" THEN 
                newStroke := TRUE; 
               currentColor := params;
                ! TESTING: Send back OK before command execution if it passes. Should slow down serial lag. 
                WriteStrBin iodev1, "\06";
                ! END TESTING
               GotoPaint(currentColor);
               
               doubleDip := TRUE ;
               !WriteStrBin iodev1, "\06";
               RETURN TRUE; 
        ELSE 
            throwError "unknown", directive;  
            
            RETURN FALSE; 
        ENDIF 
     RETURN FALSE;
    ENDFUNC
    
    !***********************************************************
    !
    ! function result checkForBadPoints(x, y)
    !           x, y: points within the canvas
    !           
    !           Returns TRUE or FALSE if it passes or fails. 
    !
    !   ensures that the point is within the defined canvas. 
    !
    !*********************************************************** 
    func bool checkForBadPoints(num Xcoord, num Ycoord)
        IF (Xcoord>sizeX) OR (Ycoord>sizeY) THEN
            WriteStrBin iodev1, "\15";
            ErrLog 4800, "Coord Error", "One of the coordinates is outside expected bounds","Coordinates larger than image size are not allowed","-","-";
            TPWrite "Bad coordinates in the file: outside expected bounds";
            moveToFinish;
            RETURN FALSE; 
        ENDIF
        IF  (Xcoord<0) OR (Ycoord<0) THEN
            WriteStrBin iodev1, "\15";
            ErrLog 4800, "Coord Error", "Negative Coordinates are not allowed.","-","-","-";
            TPWrite "Bad coordinates in the file: outside expected bounds";
            moveToFinish;
            RETURN FALSE;
        ENDIF
            RETURN TRUE;
    ENDFUNC 
    !***********************************************************
    !
    ! procedure  moveToXY(x, y)
    !           x, y: points within the canvas
    !           
    !
    !   Moves to points and goes to paint if the end of a stroke is reached
    !
    !***********************************************************
    PROC moveToXY(num XCoord,num YCoord)
        niceStroke;
        ConfL\Off;
        IF distanceTravelled>=PAINT_MAX_DIST OR newStroke=TRUE THEN
            !if we've gone maximum distance or we reach the end of a line.
            IF distanceTravelled > (PAINT_MAX_DIST / 2) THEN 
            GotoPaint(currentColor);
            distanceTravelled:=0;
            ENDIF 
            IF newStroke AND (NOT firstTimeRun) THEN 
                MoveL [[lastX,lastY,canvasHeight+20],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z10,paintBrush; 
                MoveL [[XCoord,YCoord,canvasHeight+20],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z10,paintBrush; 
            ENDIF 
        ENDIF
        IF NOT (newStroke=TRUE) THEN
            distanceTravelled:=distanceTravelled+vectorMag;
        ENDIF
        
        IF brushClean THEN
            IF NOT newStroke THEN 
            MoveL [[lastX,lastY,canvasHeight+20],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z10,paintBrush;
            MoveL [[lastX,lastY,canvasHeight],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z10,paintBrush;
            ELSE
                MoveL [[XCoord,YCoord,canvasHeight+20],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z10,paintBrush;
                
            ENDIF 
            brushClean:= FALSE;
        endif
        
        MoveL [[XCoord,YCoord,canvasHeight],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v100,z0,paintBrush;
        lastX:=XTGT;
        lastY:=YTGT;
        brushDirty:=TRUE;
        ! This moves to point at 100 mm/sec. 
    ENDPROC
    !***********************************************************
    !
    ! procedure  moveToFinish()
    !          
    !      Moves to a photogenic finishing spot. 
    !
    !***********************************************************
    PROC moveToFinish()
        ! TODO: To be tested. We want to move to a nice parking spot when we are done. 
            MoveL approachClean, v500,z50,paintBrush;
            cleanCycle 2;
            MoveL approachClean, v500,z50,paintBrush;
        IF NOT firstTimeRun THEN 
        MoveL [[LastX,LastY,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z20,paintBrush;
        ENDIF 
        MoveL [[400,-250,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z50,paintBrush;
        WriteStrBin iodev1, "\15";
        Close iodev1;        
        main;
    ENDPROC 
    !***********************************************************
    !
    ! procedure  GotoPaint(paintString)
    !       paintString: a character from A-F inclusive
    !          
    !      Moves and gets paint. If the paint color has changed, clean the brush.
    !
    !***********************************************************
    PROC GotoPaint(string colorToPaint)
        ! NOTE: Dirty cases here! TODO: test this
        ConfL\Off;
        
        IF (newStroke=TRUE) THEN
            lastX:=XTGT;
            lastY:=YTGT;
        ENDIF
        
        IF brushDirty THEN 
            ! Area commented - was necessary for changing quaternion.
            ! Move off the canvas before getting paint. 
            !MoveL [[LastX,LastY,canvasHeight],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v100,fine,paintBrush;
            !IF lastY > 0 OR lastX > 500 THEN 
            !    MoveL [[LastX,LastY,canvasHeight+70],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z20,paintBrush;
            !    MoveL [[400,-270,canvasHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z150,paintBrush;
            !ELSE 
                MoveL [[lastX,lastY,canvasHeight+70],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z20,paintBrush;
            !ENDIF 
            brushDirty:=FALSE;
        ENDIF 
            
        IF (NOT (colorToPaint=lastColor)) AND (NOT firstTimeRun ) THEN
            !NEED TO CLEAN
            MoveL approachClean, v500,z50,paintBrush;           
            cleanCycle 1;    
            MoveL approachClean, v500,z50,paintBrush;
        ENDIF 
            
        IF (colorToPaint="A") THEN
            !over paint
            MoveL overC,v500,z50,paintBrush;
            MoveL overA,v500,z50,paintBrush;
            !into paint
            MoveL colorA,v100,fine,paintBrush;
            !over paint
            MoveL overA,v500,z50,paintBrush;
            MoveL overC,v500,z50,paintBrush;

        ELSEIF (colorToPaint="B") THEN
            !over paint
            MoveL overC,v500,z50,paintBrush;
            MoveL overB,v500,z50,paintBrush;
            !into paint
            MoveL colorB,v100,fine,paintBrush;
            !over paint
            MoveL overB,v500,z50,paintBrush;
            MoveL overC,v500,z50,paintBrush;
        ELSEIF (colorToPaint="C") THEN
            !over paint
            MoveL overC,v500,z50,paintBrush;
            !into paint
            MoveL colorC,v100,fine,paintBrush;
            !over paint
            MoveL overC,v500,z50,paintBrush;
        ELSEIF (colorToPaint="D") THEN
            !over paint
            MoveL overD,v500,z50,paintBrush;
            !into paint
            MoveL colorD,v100,fine,paintBrush;
            !over paint
            MoveL overD,v500,z50,paintBrush;
        ELSEIF (colorToPaint="E") THEN
            !over paint
            MoveL overE,v500,z50,paintBrush;
            !into paint
            MoveL colorE,v100,fine,paintBrush;
            !over paint
            MoveL overE,v500,z50,paintBrush;
        ELSEIF (colorToPaint="F") THEN
            !over paint
            MoveL overF,v500,z50,paintBrush;
            !into paint
            MoveL colorF,v100,fine,paintBrush;
            !over paint
            MoveL overF,v500,z50,paintBrush;
        ELSEIF (colorToPaint="G") THEN
            !over paint
            MoveL overG,v500,z50,paintBrush;
            !into paint
            MoveL colorG,v100,fine,paintBrush;
            !over paint
            MoveL overG,v500,z50,paintBrush;
        ELSEIF (colorToPaint="H") THEN
            !over paint
            MoveL overH,v500,z50,paintBrush;
            !into paint
            MoveL colorH,v100,fine,paintBrush;
            !over paint
            MoveL overH,v500,z50,paintBrush;            
        ENDIF

        lastColor:=colorToPaint;
        brushClean:=TRUE;
    
    ENDPROC
    
    PROC cleanCycle(num count)
        VAR num counter:=1;
        WHILE counter <= count DO
            MoveL overClean,v500,z50,paintBrush;
            MoveL clean,v100,fine,paintBrush;
            WaitTime 0.5;
            MoveL overClean, v100, z0, paintBrush;
            MoveL overDryer,v500,z20,paintBrush;
            MoveL dryer,v100,fine,paintBrush;
            WaitTime 1;
            MoveL overDryer,v500,z50,paintBrush;
            counter := counter + 1;
        ENDWHILE
            MoveL overClean,v500,z50,paintBrush;
            MoveL clean,v100,fine,paintBrush;
            WaitTime 1;
            MoveL overClean, v100, z0, paintBrush;
            MoveL overDryer,v500,z20,paintBrush;
            MoveL dryerL,v100,fine,paintBrush;
            WaitTime 0.5;
            MoveL dryer,v5,fine,paintBrush;
            WaitTime 0.5;
            MoveL dryerH,v10,fine,paintBrush; 
            WaitTime 0.5;
            MoveL overDryer,v500,z50,paintBrush;
    ENDPROC 
    !***********************************************************
    !
    ! procedure  niceStroke()
    !       
    !          Access for modifying the quaternion in the future. 
    !
    !***********************************************************
    PROC niceStroke()
        vX:=XTGT-lastX;
        vY:=YTGT-lastY;
        vectorMag:=sqrt(vX*vX+vY*vY);
        ! Actual nice strokes nuked for now. FUTURE: Create actual nice quaternion strokes. 
        paintStrokeQuat:=ZeroZeroQuat;
    ENDPROC
    !***********************************************************
    !
    ! procedure  throwError(int errorType)
    !       
    !          Handles all error throwing and program halting. 
    !
    !***********************************************************    
    proc throwError(string errorType, string response)
        WriteStrBin iodev1, "\15";
        Close iodev1;
        IF errorType = "semicolon" THEN
            ErrLog 4800, "Command Error", "Missing ';' terminator in message",response,"-","-";
            TPWrite "Command Error: Missing semicolon to terminate command";
        ELSEIF errorType = "coord" THEN 
            ErrLog 4800, "Coord Error", "Mangled coordinates in pair",response,"-","-";
            TPWrite "Bad coordinates in the file: mangled in pair";
        ELSEIF errorType = "multicoord" THEN 
            ErrLog 4800, "Coord Error", "Multiple X or Y coords recieved","Expected to see coordinates of the format X:###,Y:###","Saw two declarations of X or Y",response;
            TPWrite "Bad coordinates in the file: duplicated";
        ELSEIF errorType = "missingcoord" THEN 
            ErrLog 4800, "Coord Error", "Missing X or Y coords","Expected to see coordinates of the format X:###,Y:### ","Saw missing declaration of X or Y in pair",response;
            TPWrite "Bad coordinates in the file: missing pair dec";
        ELSEIF errorType = "canvas" THEN 
            ErrLog 4800, "Data Error", "The canvas size data was malformed",response,"","You should have seen other errors before this.";
            TPWrite "Bad canvas size data";
        ELSEIF errorType = "unknown" THEN 
           ErrLog 4800, "Command Error", "Unknown Command",response,"-","-";
            TPWrite "Command Error: Unknown Command";
        ELSE 
            ErrLog 4800, "Generic Error", "Generic Message",response,"-","-";
            TPWrite "Generic Error: Unknown";
        ENDIF 
        moveToFinish;
    endproc
ENDMODULE