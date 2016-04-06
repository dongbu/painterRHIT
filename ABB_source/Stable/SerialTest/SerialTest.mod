MODULE SerialTest
    !***********************************************************
    !
    ! Module:  SerialTest
    !
    ! Description:
    !   <Insert description here>
    !
    ! Author: drongla
    !
    ! Version: 1.0
    !
    !***********************************************************


    !***********************************************************
    !
    ! Procedure main
    !
    !   This is the entry point of your program
    !
    !***********************************************************
    VAR iodev terminal;
    VAR string response;
    VAR rawbytes rawDataIn;
    VAR num timeDelay := 1;    
    VAR num index;
    
    PROC main()
        Open "COM1:",terminal\Bin;

        !WriteStrBin terminal,"\02Hello!\03";
        WaitTime 5;
        ClearRawBytes rawDataIn;
        ReadRawBytes terminal,rawDataIn\Time:=timeDelay;
        !WriteStrBin terminal,ValToStr(RawBytesLen(rawDataIn))+" Raw bytes read!\0A\0D";

        UnpackRawBytes rawDataIn,1,response\ASCII:=(RawBytesLen(rawDataIn));

        index:=StrFind(response, 1, ";");
        IF index < StrLen(response) THEN
            response := StrPart(response, 1, index-1);
            ! Note: TP commands are not allowed in auto mode
            !TPErase;
            !TPWrite response;
        ENDIF 

        WriteStrBin terminal,"\06";
        
        Close terminal;
        
    ENDPROC
    


ENDMODULE