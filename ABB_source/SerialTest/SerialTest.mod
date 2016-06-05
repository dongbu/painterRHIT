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
    VAR num timeDelay := 0.1;
    
    VAR num index;
    
    PROC main()
        !        !Add your code here
        Open "COM1:",terminal\Bin;

        WriteStrBin terminal,"Hello!\0A\0D";
        WaitTime 5;
        ClearRawBytes rawDataIn;
        ReadRawBytes terminal,rawDataIn\Time:=timeDelay;
        !WriteStrBin terminal,ValToStr(RawBytesLen(rawDataIn))+" Raw bytes read!\0A\0D";

        UnpackRawBytes rawDataIn,1,response\ASCII:=(RawBytesLen(rawDataIn));

        index:=StrFind(response, 1, ";");
        IF index < StrLen(response) THEN
            response := StrPart(response, 1, index);
        ENDIF 

        WriteStrBin terminal,response +"\0A\0D";
        
        Close terminal;
        !Stop;
        !ErrLog 4800, response, "", "", "","";
        
    ENDPROC
    


    !    FUNC string getResponse()
    !        local var byte readMe:=ReadBin(terminal \Time:=5);
    !        response := ByteToStr(readMe\char);
    !        WHILE((readMe=-1) or (not response = ";")) DO 

    !        ENDWHILE 
    !    ENDFUNC 
ENDMODULE