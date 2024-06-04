# Tétel 8: Adatcsatorna tétel

A csatornák fogalma ehhez a tételhez is illik, de korábbi tételben már kifejtésre került, így nem lett itt is leírva.

Legyen $F$ egy hosszú számítás, ami felbontható kis lépések kompoziciójára: $F = f_n \circ \cdots \circ f_0$. $F$-et a $D = \langle d_1,\dots,d_m \rangle$ sorozat elemeire szeretnénk elemenként alkalmazni. Tegyük fel, hogy $m \gg n$, azaz nagyságrenddel több adatpontunk van, mint alkalmazandó függvényünk.

Célunk a számítás optimalizálása. Az egyik megközelítés, hogy $j$ felé osztjuk az $m$ adatpontot és mindegyik darabot (batch-et) egy külön számítási egységnek (például processzornak) adunk ($j$ jelölje a processzorok számát), amik mind ugyan azt a programot futtatják: $F$-et alkalmazzák a batch-re.

Egy másik megközelítés, hogy $n$ processzort használunk, mindegyik csak egy $f_i$ függvény alkalmazásáért felel. Ezeket a processzorokat pipeline-szerűen egymás után helyezzük és csatornákkal összekötjük, így $n+m$ lépésben (feltéve, hogy az $f_i$ függvények számítási ideje körülbelül azonos) előállítható az eredmény szemben az egyprocesszoros $n*m$ műveletigénnyel. Az adatcsatorna tétel ezt a megközelítést formalizálja.

## Adatcsatorna specifikáció

$A = \stackrel{Ch(T)}{x_0} \times \stackrel{Ch(T)}{\overline{x_0}} \times \stackrel{Ch(T)}{x_{n+1}} \times \stackrel{Ch(T)}{\overline{x_{n+1}}}$

$B = \stackrel{Ch(T)}{x'_0} \times \stackrel{Ch(T)}{\overline{x'_0}} \times \stackrel{Ch(T)}{x'_{n+1}} \times \stackrel{Ch(T)}{\overline{x'_{n+1}}}$

Számomra teljes mértékben felfoghatatlan, hogy a paramétertér miért nem csupán egy $D$-t tartalmazó változóból áll.

- $Q = (x_0 = \overline{x_0} = x'_0 = \overline{x'_0} = D) \land (x_{n+1} = \overline{x_{n+1}} = x'_{n+1} = \overline{x'_{n+1}} = <>)$
- $Q \in INIT_h$
- $FP_h \Rightarrow \overline{x_{n+1}} = F(D)$ $\quad$ (utolsó csatornán a kimenet előáll)
- $Q \in TERM_h$
- $(\overline{x_0} = \overline{x'_0}=D) \in inv_h$ $\quad$ ($D$-n túl nem kerül feldolgozásra más elem)

Probléma: hiányoznak a köztes komponensek.

## Adatcsatorna finomítás

A megoldás megtalálása érdekében bővítjük az állapotteret köztes csatornaváltozókkal és finomítjuk a specifikációt a fixpontfeltétel finomításának tétele alapján.

$A = (\times_{i=0}^{n+1} \stackrel{Ch(T)}{x_i} \times \stackrel{Ch(T)}{\overline{x_i}})$

$B = \stackrel{Queue(T)}{D}$

- $Q = (x_0 = \overline{x_0} = D) \land (x_{n+1} = \overline{x_{n+1}} = <>)$
- $Q \in INIT_h$
- $Q \in TERM_h$
- $(\overline{x_0}=D) \in inv_h$ $\quad$ ($D$-n túl nem kerül feldolgozásra más elem: zajmentesség egyik része)
- $FP_h \Rightarrow \forall i \in [0..n]: x_i = <>$
- $\forall i \in [0..n]: (f_i(\overline{x_i}-x_i) = \overline{x_{i+1}}) \in inv_h$ $\quad$ (sorrendhelyesség, veszteségmentesség, zajmentesség másik része)

A $FP_h \Rightarrow \overline{x_{n+1}} = F(D)$ állítás már nem része a specifikációnak, de belátható (fixpontfeltétel finomítás tétellel), hogy ez következik a specifikáció állításaiból.

A megfelelés bizonyításához variánsfüggvényt használhatunk. Legyen $t = \sum_{i=0}^n |x_i|*(m+1)^{n-i}$, azaz egy $n$ helyiértékű, $m{+}1$ számrendszerbeli szám, amely számjegyei rendre a $|x_0|,\dots,|x_n|$ csatornahosszak. Amikor egy adatpont az adatcsatornán a következő csatornára átadásra került, akkor a variánsfüggvény értéke csökken: egy nagyobb helyiértékű számjegy csökken és (ha nem a kimeneti csatornára került az adatpont) egy kisebb helyiértékű számjegy nő.

## Adatcsatorna megoldás

$S=(\parallel_{i=1}^n x_i := <> ; \; \begin{Bmatrix} \square_{i=1}^n x_i,x_{i+1} := lorem(x_i),hiext(x_{i+1},f_i(lov(x_i))) \text{, ha } x_i \ne <> \end{Bmatrix})$
