# Tétel 4: Megoldás definíciója

## Megoldás

Definíció: $S$ program megoldja az $F$ feladatot, ha $\forall b \in B: \exists h \in F(b)$, hogy $S$ megfelel $h$ specifikációs feltételeinek.

Elegendő a feltételeket az elérhető állapotokban, tehát a legszűkebb invariáns ($INV_S(Q)$) mellett vizsgálni. Azonban célszerűbb gyengébb, azaz bővebb invariáns állítás mellett belátni a következő állításokat, hiszen ha bővebb invariáns mellett fennállnak, akkor szűkebb mellett is fenn fognak állni. Gyengébb invariáns választásával nem kell az $INV_S(Q)$-t kiszámolni, ami problémás lehet a gyakorlatban.

## Megfelelés

Amennyiben az alábbi programtulajdonságokat be tudjuk látni az $S$ programról, akkor az megfelel a $h$ specifikációs feltételekenek.

Minden program akkor felel meg egy specifikációs feltételnek, ha van olyan invariáns, ami mellett megfelel neki.

A továbbiakban legyen $K$ egy tetszőleges eleme az $inv_S(\land_{Q \in INIT_h} Q)$ halmaznak. Amennyiben az adott feltétel egy invariáns mellett nem teljesül, attól még egy szűkebb invariáns mellett teljesülhet. Ha a legszűkebb invariáns mellett sem teljesül, akkor az $S$ program nem felel meg.

### Megfelel $\triangleright_h$, $\mapsto_h$, $\hookrightarrow_h$ tulajdonságnak

- Legyen $\oplus \in \{\triangleright, \mapsto, \hookrightarrow\}$
- $S$ megfelel $P \oplus_h Q$ $\impliedby$ $P \land K \oplus_S Q \land K$

### Megfelel invariáns feltételnek

- $S$ megfelel $P \in inv_h$ $\impliedby$ $P \land K \in inv_S(\land_{Q \in INIT_h} Q)$

### Megfelel terminálási feltételnek

- $P \hookrightarrow FP_h \equiv P \in TERM_h$
- $S$ megfelel $P \in TERM_h$ $\impliedby$ $sp(s_0,P) \land K \hookrightarrow_S \varphi_S$
- Bal oldal gyengítése tétel miatt elég belátni, hogy $K \hookrightarrow_S \varphi_S$
  - Ami pedig belátható például egy variánsfüggvény segítségével:
    - Ha $\exists t: A \mapsto \mathbb{Z}$ melyre: $\neg \varphi_S \land K \Rightarrow t>0$ és $\forall m \in \mathbb{N}: \neg \varphi_S \land K \land t=m \hookrightarrow_S (K \land t<m) \lor \varphi_S$
    - Akkor $S$ megfelel a $P \in TERM_h$ kikötésnek

### Megfelel fixpont feltételnek

- $S$ megfelel $FP_h \Rightarrow R$ $\impliedby$ $\varphi_S \land K \Rightarrow R$
