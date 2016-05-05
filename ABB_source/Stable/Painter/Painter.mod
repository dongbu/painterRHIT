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
    ! Version: 0.5 - resizable canvas tools
    !
    !***********************************************************
    
    
  

    ! *** Constants ***
    ! Commonly Tweaked declarations
    CONST num brushLength:=200;
    CONST num paintHeight:=5;
    PERS num canvasHeight:=31; ! 0 for painting on the floor of the work cell. 
    PERS num PAINT_MAX_DIST:=30;
    
    ! Of Relation to Colors:
    CONST num firstPaint{2}:=[276, -290];
    CONST num paintCupRadius:=50; ! The center-to-center spacings between paint cups (See: dixie cup :D )
    CONST num maxPaintX:=626;
    VAR num paintCupYOffset;
    VAR num cupIndex;     
    
    ! Canvas size Declarations
    ! * The largest usable square area the robot can draw in is 500mm wide by 150mm tall
    ! * This is a rectangular large canvas, about 19.6" by 9.8"
    PERS num canvasXmin:=400;
    PERS num canvasXmax:=705;
    PERS num canvasYmin:=-230;
    PERS num canvasYmax:=75;  
    
    ! Used in the conversion of pixels to mm on the canvas
    !CONST num XOffset:=260;! Unused - by default, corner of image goes to min y min x of canvas. 
    !CONST num YOffset:=-150; ! Unused - by default, corner of image goes to min y min x of canvas. 
    
    ! Scaling factor for when we load an image (Default 0.5)
    VAR num SF:=0.5;
    
    ! Orientation constants
    VAR orient ZeroZeroQuat:=[0.70427,0.02028,0.71115,0.01996];    
    VAR orient paintStrokeQuat; ! set to zero-zero in initializeProgram
    VAR orient paintCupQuat;    ! but these values are exposed for special cases where we may want them changed. 
    VAR orient paintcleanerQuat:=[0.51854, 0.50842, 0.49217, -0.47999]; 
    
    ! Describes the paintbrush location. TODO: verify with metric calipers. 
    PERS tooldata paintBrush:=[TRUE,[[87,0,146],[1,0,0,0]],[0.2,[0,0,146],[0,0,1,0],0,0,0]];
    
    ! *** Variables ***  
    VAR iodev iodev1;
    VAR rawbytes rawMsgData;
    
    ! Store image size, in pixels
    VAR num sizeX;
    VAR num sizeY;
    VAR num XTGT:=0; ! X target (processed)
    VAR num YTGT:=0; ! Y Target (processed)
    VAR num lastX:=500;
    VAR num lastY:=0;
    ! Temporary values for processing coordinates
    VAR num tX;
    VAR num tY;
    !
    ! Velocity values for processing coordinates. Note: Is not true velocity, just step-relative velocity.
    VAR num vX;
    VAR num vY;
    
    ! Used in distance calculation
    VAR num vectorMag;

    ! Locations of the painting targets. 
 
    ! Change these in procedure: initializeColors
    VAR robtarget overPaint;
    VAR robtarget inPaint;
    VAR robtarget approachPaint;
    
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
        VAR btnres answer;
        VAR string currentSize;
        currentSize := "Canvas Dims: Z:"+ NumToStr(canvasHeight,0) + "mm, X:"+NumToStr(canvasXmax-canvasXmin,0)+"mm, Y:"+NumToStr(canvasYmax-canvasYmin,0)+"mm, Paint Dist:"+NumToStr(PAINT_MAX_DIST,0);
        ConfL\Off;
        initializeProgram;
            
        answer:= UIMessageBox(
            \Header:="Pre-Paint Com Checks"
            \MsgArray:=["Please check and verify the following:","- The serial cable is connected to COM1", "- The PC is connected to the serial cable","- BobRoss has opened the serial channel on the PC", currentSize]
            \BtnArray:=["Ready","Positioning", "Canvas Tools", "Park"]
            \Icon:=iconInfo);
        IF answer = 1 THEN
            ! Operator said ready
            paintProgram;
        ELSEIF answer = 2 THEN 
            
            CALIBRATIONTOOLS;
        ELSEIF answer = 3 THEN 
            
            CANVASTOOLS;
        ELSE 
            ! Parking spot for storage. 
            MoveL [[489,0,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z50,paintBrush;
            Stop;
        ENDIF      
       

    ENDPROC
    
    PROC CALIBRATIONTOOLS()
        VAR num exitcode;
        VAR num canvasAnswer;
        VAR btnres answer1;
        
        exitcode:=0;
        
        WHILE exitcode = 0 DO 
            
            answer1:= UIMessageBox(
            \Header:="Equipment Position Helper"
            \MsgArray:=["Select an option from the selection below","", "","",""]
            \BtnArray:=["Clean","Super-Dry", "Touch","Home","Return"]
            \Icon:=iconInfo);
            
            IF answer1 = 1 THEN
                ! operator said clean
                MoveL approachClean, v500,z50,paintBrush;
                cleanCycle 1;
                MoveL approachClean, v500,z50,paintBrush;
                MoveL [[400,-250,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z50,paintBrush;
            ELSEIF answer1 = 2 THEN 
                MoveL approachClean, v500,z50,paintBrush;
                MoveL overDryer,v500,z20,paintBrush;
                MoveL dryer,v100,fine,paintBrush;
                WaitTime 3;
                MoveL overDryer,v500,z50,paintBrush;
                MoveL approachClean, v500,z50,paintBrush;
                MoveL [[400,-250,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z50,paintBrush;
            ELSEIF answer1 = 3 THEN 
                ! ends 10mm from both sides, starts at corner. 
                MoveL [[400,-230,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z50,paintBrush;
                MoveL [[400,-230,canvasHeight],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v100,z50,paintBrush;
                MoveL [[410,-220,canvasHeight],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v50,z5,paintBrush;
            ELSEIF answer1 = 4 THEN 
                MoveL [[400,-250,canvasHeight+100],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z50,paintBrush;
            else
                exitcode := 1;
            ENDIF 
        
        ENDWHILE 
        
    ENDPROC 
    
    PROC CANVASTOOLS()
        VAR num exitcode;
        VAR num canvasAnswer;
        VAR btnres answer2;
        VAR string currentSize;
        exitcode:=0;
        
        WHILE exitcode = 0 DO 
            
            currentSize := "Canvas Dims: Z:"+ NumToStr(canvasHeight,0) + "mm, X:"+NumToStr(canvasXmax-canvasXmin,0)+"mm, Y:"+NumToStr(canvasYmax-canvasYmin,0)+"mm, Paint Dist:"+NumToStr(PAINT_MAX_DIST,0);
            answer2:= UIMessageBox(
                \Header:="Canvas Tools Menu"
                \MsgArray:=["Welcome to Canvas Setup!","The values entered here are stored between controller restarts",  currentSize,"Note: Ensure the corner of the canvas is at ","x:400, y:-230"]
                \BtnArray:=["Z","X", "Y","L","Return"]
                \Icon:=iconWarning);
                
            IF answer2 = 1 THEN 
                canvasAnswer := UINumEntry(
                \Header:="Canvas Vertical Height Setup (Z-AXIS)"
                \MsgArray :=["What is the vertical height, in mm, of the canvas"," from the floor of the workcell?", "Current Dim: Z:"+ NumToStr(canvasHeight,0) ]
                \Icon:=iconInfo
                \InitValue:=0
                \MinValue:=0
                \MaxValue:=100
                \AsInteger);
                canvasHeight := canvasAnswer;
            ELSEIF answer2 = 2 THEN 
                canvasAnswer := UINumEntry(
                \Header:="Canvas Height Setup (X-AXIS)"
                \MsgArray:=["What is the height, in mm, of the canvas?","Note: Use 250 for wide canvas due to mobility reasons", "Current Dim: X:"+NumToStr(canvasXmax-canvasXmin,0)]
                \Icon:=iconInfo
                \InitValue:=250
                \MinValue:=50
                \MaxValue:=500
                \AsInteger);
                canvasXmax := canvasXmin + canvasAnswer;
            ELSEIF answer2 = 3 THEN 
                canvasAnswer := UINumEntry(
                \Header:="Canvas Width Setup (Y-AXIS)"
                \MsgArray:=["What is the width, in mm, of the canvas?","Note: A wide canvas restricts X movement, and","can put parts of the image out of bounds", "Current Dim: Y:"+NumToStr(canvasYmax-canvasYmin,0)]
                \Icon:=iconInfo
                \InitValue:=500
                \MinValue:=50
                \MaxValue:=500
                \AsInteger);
                canvasYmax := canvasYmin + canvasAnswer;
            ELSEIF answer2 = 4 THEN 
                canvasAnswer := UINumEntry(
                \Header:="Brush Stroke Length (L)"
                \MsgArray:=["How long should the brush travel before getting more paint?","TRY: Using 50 for paper, 30 for canvas", "Current Paint Dist:"+NumToStr(PAINT_MAX_DIST,0)]
                \Icon:=iconInfo
                \InitValue:=50
                \MinValue:=10
                \MaxValue:=80
                \AsInteger);
                PAINT_MAX_DIST := canvasAnswer;
            else 
                exitcode := 1;
            ENDIF 
        ENDWHILE 
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
    PROC initializeProgram()
        
        paintStrokeQuat:=ZeroZeroQuat; 
        paintCupQuat:=ZeroZeroQuat;
        
        ! FirstTimeRun Flags
        
        firstTimeRun :=TRUE;
        currentColor:= "A";
        lastColor := "A";
        cupIndex := 0;
        overPaint := [[firstPaint{1} + (paintCupRadius * cupIndex),(-290 - (paintCupRadius*paintCupYOffset)),paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        inPaint:= [[firstPaint{1} + (paintCupRadius * cupIndex),(-290 - (paintCupRadius*paintCupYOffset)),paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        serialTimeout := 25;
        doubleDip := FALSE ;
        brushClean := FALSE; 
        brushDirty := FALSE;
        isRotated := FALSE;
        ! 

        
        approachPaint := [[376,-290,paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        
        
        approachClean:=[[465,-290,350],paintcleanerQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        overClean:=[[452.3,-482.4,350],paintcleanerQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        clean:=[[452.3,-482.4,269],paintcleanerQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        
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
                throwError "canvas", parameters + " ::SizeWayTooSmall";
            ENDIF 
            

            
            IF (sizeX > (canvasXmax-canvasXmin)) OR (sizeY > (canvasYmax-canvasYmin))THEN
                ! the Y proportion should be the scaling factor, as it was the smaller number
                IF ((canvasXmax-canvasXmin)/sizeX) > ((canvasYmax-canvasYmin)/sizeY) THEN
                    SF:=(canvasYmax-canvasYmin)/sizeY;
                ELSE
                    SF:=(canvasXmax-canvasXmin)/sizeX;
                ENDIF
                    
            ELSE
                SF:=1;
            ENDIF 
            
            
                
        ELSE
            throwError "canvas", parameters+ " ::CoordProcFail";  
            
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
            ! Prevents the pogo-of-death where pogoing with 0-distance points makes the brush run out of paint            
            distanceTravelled := distanceTravelled + 2; 
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
                
            !
            !   The image as given to the robot is in a mirrored state. 
            !   We must fix this. 
            !   If we need to rotate it, the easiest thing is to simply transpose it. 
            !   Otherwise, we need to flip (mirror) the image. 
            !
            IF isRotated THEN ! Transpose the image
                tempA := tX;
                tempB := tY;  
                tX:= tempB;
                tY:= tempA; 
            ELSE ! mirror the image across the X-axis
                tX := sizeX - tX;
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
            IF distanceTravelled > (PAINT_MAX_DIST) THEN 
                GotoPaint(currentColor);
                distanceTravelled:=0;
            ENDIF 
            IF newStroke AND (NOT firstTimeRun) THEN 
                MoveL [[lastX,lastY,canvasHeight+20],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z10,paintBrush; 
                MoveL [[XCoord,YCoord,canvasHeight+20],paintStrokeQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z10,paintBrush; 
                vectorMag := 0;
            ENDIF 
        ENDIF
        IF (newStroke=FALSE) THEN
            vX:=XCoord-lastX;
            vY:=YCoord-lastY;
            vectorMag:=sqrt(vX*vX+vY*vY);
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
        ! Whack this color into shape!
        VAR byte part;
        VAR num maxPerRow;

        
        IF (StrLen(colorToPaint) > 1) THEN
            throwError "paint", colorToPaint;
        ENDIF 
        
                        
            ConfL\Off;
            
            IF (newStroke=TRUE) THEN
                lastX:=XTGT;
                lastY:=YTGT;
            ENDIF
            
            IF brushDirty THEN 
                MoveL [[lastX,lastY,canvasHeight+70],ZeroZeroQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]],v500,z20,paintBrush;             
                brushDirty:=FALSE;
            ENDIF 
                
            IF (NOT (colorToPaint=lastColor)) AND (NOT firstTimeRun ) THEN
                !NEED TO CLEAN
                MoveL approachClean, v500,z50,paintBrush;           
                cleanCycle 1;    
                MoveL approachClean, v500,z50,paintBrush;
            ENDIF 
        
           IF NOT (colorToPaint = lastColor) THEN 
                paintCupYOffset :=0;
                maxPerRow:= (maxPaintX - firstPaint{1})/50;
                part := StrToByte(colorToPaint\Char);
     
                IF (part >= 65) AND (part <=90) THEN 
                    TPWrite "Accepted and converted:"+colorToPaint;
                    ! Recall from earlier. 
                        !CONST num firstPaint{2}:=[276, -290];
                        !CONST num paintCupRadius:=50; ! The center-to-center spacings between paint cups (See: dixie cup :D )
                        !CONST num maxPaintX:=626;
                        !CONST num maxPaintY:=-340;
                    cupIndex := part - 65;
                    WHILE ((firstPaint{1} + (paintCupRadius * cupIndex)) > maxPaintX) DO
                        paintCupYOffset:= paintCupYOffset+1;
                        cupIndex := cupIndex - maxPerRow - 1; ! lol @ zero-based indexing in a 1-based indexing world. 
                        TPWrite "Overflow! Rolling back and travelling along Y-axis: index now " + NumToStr(cupIndex, 0);
                    ENDWHILE
                    
                    overPaint := [[firstPaint{1} + (paintCupRadius * cupIndex),(-290 - (paintCupRadius*paintCupYOffset)),paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
                    inPaint:= [[firstPaint{1} + (paintCupRadius * cupIndex),(-290 - (paintCupRadius*paintCupYOffset)),paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
                
                ELSE 
                    TPWrite "INVALID PAINT. DISQUALIFIED!";
                    throwError "paint", colorToPaint;
                ENDIF  
            ENDIF
            IF (cupIndex < 3) THEN
                !over paint
                MoveL approachPaint,v500,z50,paintBrush;
            ENDIF 
                MoveL overPaint,v500,z50,paintBrush;
                !into paint
                MoveL inPaint,v100,fine,paintBrush;
                !over paint
                MoveL overPaint,v500,z50,paintBrush;
            IF (cupIndex < 3) THEN
                !over paint
                MoveL approachPaint,v500,z50,paintBrush;
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
        ELSEIF errorType = "paint" THEN 
           ErrLog 4800, "Paint Error", "Out of bounds paint",response,"-","-";
            TPWrite "Command Error: Paint Command is bad";
         
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