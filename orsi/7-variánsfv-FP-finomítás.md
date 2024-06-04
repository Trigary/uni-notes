# Tétel 7: Variáns függvény tétel, fixpont finomítás tétele

Ezekkel célunk vagy egy feladat finomítása (például egy könnyebben megoldható, megoldáshoz közelebb álló feladattá) vagy pedig egy megfelelés bizonyításának megkönnyítése.

## Variáns függvény

Motiváció: a görbenyíl bizonyítása problémás, például definíció alapján strukturális indukció szükséges hozzá több esetben. A variánsfüggvény erre kíván egy alternatívát biztosítani. A ciklus levezetési szabályához hasonlóan belátjuk, hogyha egy alulról korlátos függvény értéke folyamatosan csökken, amíg el nem érünk egy kívánt állapotba, akkor előbb-utóbb el fogunk abba az állapotba érni.

Tétel:

- Legyen $P,Q : A \mapsto L$ és variánsfüggvény $t : A \mapsto \mathbb{Z}$
- Ha $(P \land \neg Q) \implies t>0$ és $\forall m \in \mathbb{N}: (P \land \neg Q \land t=m) \hookrightarrow_S ((P \land t<m) \lor Q)$
  - $\lceil t = m \rceil = \{a \in A \;|\; t(a) = m\}\quad$ (hasonlóan "$<$" esetén)
- Akkor $P \hookrightarrow_S Q$

A függvényérték csökkenését okozó $\hookrightarrow_S$ több utasítást is magába foglalhat. Nem követeljük meg, hogy az utasítás során keletkező köztes állapotokban is csökkenjen $t$ értéke: elég, ha csak $\hookrightarrow_S$ után csökken. Azaz például ha minden $2k$-adik $\mapsto_S$ $t$-t csökkenti 2-vel, de minden $2k+1$-edik $\mapsto_S$ $t$-t növeli 1-gyel, attól még a tétel alkalmazható: $\hookrightarrow_S$ mindig jelöljön 2 egymást követő $\mapsto_S$ lépést.

$m$-re teljes indukció segítségével bizonyítható.

## Fixpontfeltétel finomítása

Nem fixpont tulajdonság, hanem fixpont feltétel finomítása.

Ha van egy olyan fixpont kikötésünk, amire szeretnénk egy megfelelő programot találni, de ezt a fixpont kikötést nem szeretjük (például túl bonyolult belátni a vele való megfelelést), akkor ezt az egy kikötést lecserélhetjük egyszerűbb feltételekre.

Például képzeljük el, hogy egy egyszerű értékadásokkal le nem írható $Q$-t kéne belátni, ezért inkább lecseréljük azt. Felhasználjuk az invariánsunkat, ami az algoritmus lelkét tartalmazza és a tételt alkalmazva egyszerűbben belátható fixpont feltételeket készítünk.

Tétel: ha $S$ megfelel $inv_h P$ és $FP_h \Rightarrow R$ specifikációs feltételeknek és $P \land R \Rightarrow Q$, akkor $S$ megfelel $FP_h \Rightarrow Q$ specifikációs feltételnek is.
