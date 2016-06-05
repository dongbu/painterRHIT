MODULE PainterTest
        ! Of Relation to Colors:
        CONST num firstPaint{2}:=[276, -290];
        CONST num paintCupRadius:=50; ! The center-to-center spacings between paint cups (See: dixie cup :D )
        CONST num maxPaintX:=626;
        
        VAR robtarget overPaint;
        VAR robtarget inPaint;
        
        VAR num paintHeight:=0;
        
        VAR orient paintCupQuat:=[0.70427,0.02028,0.71115,0.01996];    

        
    PROC main()
        ! Whack this color into shape!
        VAR string colorToPaint;
        VAR byte part;
        VAR num index; 
        VAR num maxPerRow;
        VAR num yOffset;
        VAR string s1;
        VAR string s2;
        
        colorToPaint := "Q";
        
        TPErase;
        
        IF (StrLen(colorToPaint) > 1) THEN
            TPWrite "BAD LENGTH";
        ENDIF 
        
                    
        yOffset :=0;
        maxPerRow:= (maxPaintX - firstPaint{1})/50;
        part := StrToByte(colorToPaint\Char);
        ! 65<= X <= 90
        IF (part >= 65) AND (part <=90) THEN 
            TPWrite "Accepted:"+colorToPaint;
        ELSE 
            TPWrite "INVALID PAINT. DISQUALIFIED!";
            
        ENDIF 
        
        ConfL\Off;
        
        
        IF (part >= 65) AND (part <=90) THEN 
            TPWrite "Accepted and converted:"+colorToPaint;
            ! Recall from earlier. 
                !CONST num firstPaint{2}:=[276, -290];
                !CONST num paintCupRadius:=50; ! The center-to-center spacings between paint cups (See: dixie cup :D )
                !CONST num maxPaintX:=626;
                !CONST num maxPaintY:=-340;
            index := part - 65;
            WHILE ((firstPaint{1} + (paintCupRadius * index)) > maxPaintX) DO
                yOffset:= YOffset+1;
                index := index - maxPerRow - 1; ! lol @ zero-based indexing in a 1-based indexing world. 
                TPWrite "Overflow! Rolling back and travelling along Y-axis: index now " + NumToStr(index, 0);
            ENDWHILE
            
            overPaint := [[firstPaint{1} + (paintCupRadius * index),(-290 - (paintCupRadius*YOffset)),paintHeight+70],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
            inPaint:= [[firstPaint{1} + (paintCupRadius * index),(-290 - (paintCupRadius*YOffset)),paintHeight],paintCupQuat,[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];
        
        ELSE 
            TPWrite "INVALID PAINT. DISQUALIFIED!";
            
        ENDIF  
        IF (index < 3) THEN
            !over paint
            TPWrite "Approaching Paint (Embark)";
        ENDIF 
            s1:=NumToStr(firstPaint{1} + (paintCupRadius * index), 0);
            s2:=NumToStr(-290 - (paintCupRadius*YOffset), 0);
            TPWrite "Getting Paint " + s1 + " " + s2  + " " ;
        IF (index < 3) THEN
            !over paint
            TPWrite "Getting Paint (Disembark)";
        ENDIF 

        Stop;
    
    ENDPROC
    
ENDMODULE