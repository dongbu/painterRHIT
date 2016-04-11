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
    ! Version: 0.1
    !
    !***********************************************************
    
    
  

    ! *** Constants ***
    ! Commonly Tweaked declarations
    CONST num brushLength:=200;
    CONST num paintHeight:=10;
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
    VAR orient ZeroZeroQuat:=[0.7071067811,0.01906,0.7071067811,0.01904];    
       
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
    VAR orient paintStrokeQuat:=[0.7071067811,0.01906,0.7071067811,0.01904]; 
    VAR orient paintCupQuat:=[0.51863, 0.50936, 0.49498, -0.476];
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

    VAR robtarget overClean;
    VAR robtarget clean;
    VAR robtarget overDryer;
    VAR robtarget dryer;
    !    
    VAR bool newStroke;
    VAR num distanceTravelled := 0;
    ! UI Variables/Constants
    VAR btnres answer;
    CONST string my_message{5}:= ["Please check and verify the following:","- The serial cable is connected to COM1", "- The PC is connected to the serial cable","- BobRoss is running on the PC","- BobRoss has opened the serial channel on the PC"];
    CONST string my_buttons{2}:=["Ready","Abort"];
    ! First-loop flags
    VAR bool firstTimeRun := TRUE;
    VAR string currentColor:= "A";
    VAR string lastColor := "A";
    
    !***********************************************************
    !
    ! Procedure main
    !
    !   This is the entry point of your program
    !
    !***********************************************************
    PROC main()
        IF PPMovedInManMode() THEN
            answer:= UIMessageBox(
                \Header:="Pre-Paint Com Checks"
                \MsgArray:=my_message
                \BtnArray:=my_buttons
                \Icon:=iconInfo);
            IF answer = 1 THEN
                ! Operator said ready
                paintProgram;
    !        ELSEIF answer = 2 THEN 
    !            ! operator said abort
    !        ELSE 
    !            ! no such case defined. 
            ENDIF 

        ELSE
        TPWrite "Program Started. Assuming that BobRoss host is online on COM1";
        paintProgram;

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
        
        initializeColors;
        Open "COM1:", iodev1 \Bin;
        ClearIOBuff iodev1;
        ClearRawBytes rawMsgData;
        WaitTime 0.1;                 
        WriteStrBin iodev1, "\05";
        response := readSerial();
        WHILE response = "" DO
            response := readSerial();
        ENDWHILE
            
        
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
        overA:=[[376,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorA:=[[376,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

        overB:=[[426,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorB:=[[426,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

        overC:=[[476,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorC:=[[476,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

        overD:=[[526,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorD:=[[526,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

        overE:=[[576,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorE:=[[576,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

        overF:=[[626,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        colorF:=[[626,-290,paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];

            ! TODO: Accurately describe these locations. 
        overClean:=[[426,-315,cleanerHeight+brushLength],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        clean:=[[426,-315,cleanerHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        overDryer:=[[476,-315,cleanerHeight+brushLength],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        dryer:=[[476,-315,cleanerHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        
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
        IF multiCoordMessage = TRUE AND result = TRUE THEN 
            result := processCoordinates(remainingMessage, TRUE);
        ENDIF 
        
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
            sizeX := tX;
            sizeY := tY;
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
        ReadRawBytes iodev1, rawMsgData \Time:=25;   
           
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
                IF timeoutCounter > 20 THEN
                    moveToFinish;
                    Break;
                ENDIF
            ENDWHILE
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
        ! TODO: test this
        VAR bool result;
        VAR bool testCheck;
        IF directive = "COORD" THEN 
            result := processCoordinates(params, FALSE);
            testCheck:=true;
            ! TESTING: Send back OK before command execution if it passes. Should slow down serial lag. 
            WriteStrBin iodev1, "\06";
            ! END TESTING
            IF firstTimeRun THEN
                GotoPaint(currentColor);
                firstTimeRun := FALSE; 
            endif
            testCheck:=checkForBadPoints( tX,tY);
            XTGT:=(SF*tX)+canvasXmin;
            YTGT:=(SF*tY)+canvasYmin;
            moveToXY XTGT,YTGT;
            ! After moving, update our case. This ensures that we are starting new strokes
            ! correctly if NEXT was called before this
            newStroke:=FALSE;
            IF testCheck = TRUE THEN 
            !WriteStrBin iodev1, "\06";
            RETURN TRUE;
            ELSE 
                WriteStrBin iodev1, "\15";
                RETURN FALSE;
            ENDIF 
        ELSEIF directive = "SWAP" THEN 
               currentColor := params;
                ! TESTING: Send back OK before command execution if it passes. Should slow down serial lag. 
                WriteStrBin iodev1, "\06";
                ! END TESTING
               GotoPaint(currentColor);
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
            MoveL overA,v500,fine,paintBrush;
            Stop;
            RETURN FALSE; 
        ENDIF
        IF  (Xcoord<0) OR (Ycoord<0) THEN
            WriteStrBin iodev1, "\15";
            ErrLog 4800, "Coord Error", "Negative Coordinates are not allowed.","-","-","-";
            TPWrite "Bad coordinates in the file: outside expected bounds";
            MoveL overA,v500,fine,paintBrush;
            Stop;
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
            GotoPaint(currentColor);
            distanceTravelled:=0;
        ENDIF
        IF NOT (newStroke=TRUE) THEN
            distanceTravelled:=distanceTravelled+vectorMag;
        ENDIF
        MoveL [[XCoord,YCoord,canvasHeight],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v100,z0,paintBrush;
        lastX:=XTGT;
        lastY:=YTGT;

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
        IF NOT firstTimeRun THEN 
        MoveL [[LastX,LastY,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z0,paintBrush;
        ENDIF 
        MoveL [[400,-250,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z0,paintBrush;
        Stop;
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
        !over target
        IF not firstTimeRun THEN 
            ! Move off the canvas before getting paint. 
            !MoveL [[LastX,LastY,canvasHeight],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v100,fine,paintBrush;
            MoveL [[LastX,LastY,canvasHeight+70],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z0,paintBrush;
            MoveL [[376,-290,canvasHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z0,paintBrush;
        ELSE 
            MoveL [[376,-290,canvasHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z0,paintBrush;
            firstTimeRun := FALSE; ! prevents double-dipping at start. 
        ENDIF 
            
        IF (colorToPaint="A") THEN
            !over paint
            MoveL overA,v500,z0,paintBrush;
            !into paint
            MoveL colorA,v100,fine,paintBrush;
            !over paint
            MoveL overA,v500,z0,paintBrush;

        ELSEIF (colorToPaint="B") THEN
            !over paint
            MoveL overB,v500,z0,paintBrush;
            !into paint
            MoveL colorB,v100,fine,paintBrush;
            !over paint
            MoveL overB,v500,z0,paintBrush;
        ELSEIF (colorToPaint="C") THEN
            !over paint
            MoveL overC,v500,z0,paintBrush;
            !into paint
            MoveL colorC,v100,fine,paintBrush;
            !over paint
            MoveL overC,v500,z0,paintBrush;
        ELSEIF (colorToPaint="D") THEN
            !over paint
            MoveL overD,v500,z0,paintBrush;
            !into paint
            MoveL colorD,v100,fine,paintBrush;
            !over paint
            MoveL overD,v500,z0,paintBrush;
        ELSEIF (colorToPaint="E") THEN
            !over paint
            MoveL overE,v500,z0,paintBrush;
            !into paint
            MoveL colorE,v100,fine,paintBrush;
            !over paint
            MoveL overE,v500,z0,paintBrush;
        ELSEIF (colorToPaint="F") THEN
            !over paint
            MoveL overF,v500,z0,paintBrush;
            !into paint
            MoveL colorF,v100,fine,paintBrush;
            !over paint
            MoveL overF,v500,z0,paintBrush;

        ELSEIF (NOT (colorToPaint=lastColor)) THEN
            !NEED TO CLEAN
            MoveL overClean,v500,z0,paintBrush;
            MoveL clean,v100,fine,paintBrush;
            MoveL overClean,v500,z0,paintBrush;
            MoveL overDryer,v500,z0,paintBrush;
            MoveL dryer,v100,fine,paintBrush;
            MoveL overDryer,v500,z0,paintBrush;
            
        ENDIF
        !over target
        IF (newStroke=TRUE) THEN
            lastX:=XTGT;
            lastY:=YTGT;
        ENDIF
        IF NOT firstTimeRun THEN 
        MoveL [[376,-290,canvasHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z0,paintBrush;
        MoveL [[LastX,LastY,canvasHeight+20],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z0,paintBrush;
        MoveL [[LastX,LastY,canvasHeight],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v100,fine,paintBrush;
        ENDIF 
        lastColor:=colorToPaint;
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
       
    endproc
ENDMODULE