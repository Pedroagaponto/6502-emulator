; Test adc instructions with all the address mode

.segment "HEADER"
.segment "STARTUP"
.segment "ZEROPAGE"

.segment "CODE"
main: 
    ADC #10 ; A = A + 10 


.segment "VECTORS"
.segment "CHARS"

