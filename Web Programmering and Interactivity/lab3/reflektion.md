# Vad är skillnaden mellan ett asynkront och ett synkront anrop till servern?
Vid ett asynkront anrop behöver inte användaren vänta på svar från servern innan
användaren kan göra andra saker. Men vid ett synkront så måste användaren vänta tills
processen är klar innan man kan gå vidare i livet på hemsidan.

# Vad skulle hända ifall ni använde asynkrona anrop i er kod?
Användaren kommer kunna göra flera saker samtidigt på hemsidan och i bakgrunden så kommer
flera anrop till servern kunna hända samtidigt. Men en sak att tänka lite extra på är att detta kan
leda till att användaren upplever att inget händer (om det är ett tungt anrop) efter att
denne klickade på en knapp. Detta gör att man vill gärna ha någon form av förloppsindikator
så att användaren vet att det händer något i bakgrunden.

# Vad skulle hända ifall ni använde synkrona anrop i er kod?
Om användaren ska lägga upp en post så skulle all JavaScript-kod behöva vänta på
anropen tills servern är klar innan det är möjligt att göra något nytt. Detta betyder att om
användaren submittar och sen direkt vill kunna klicka på något annat (som behöver javascript)
så skulle det inte hända något och användaren skulle bli förvirrad.


Om det skulle ta lång tid att ladda in alla posts skulle användaren behöva vänta
tills alla posts var färdigladdade för att kunna göra något. Detta skulle ge en
uppfattning av en långsam sida.

# Vad är/innebär AJAX?
AJAX står för "Asynchronous JavaScript And XML". Alltså en asynkront metod för
hemsidan att kunna ladda ner och upp data till servrar utan att hemsidan behöver
laddas om.
