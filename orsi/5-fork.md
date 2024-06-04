# Tétel 5: FORK (elágazás)

## Csatorna, csatornaváltozók

A párhuzamos programok több komponensből állnak. Ezek közötti kommunikációra szolgálnak a csatornaváltozók. A csatornák üzenetek (egy adott típus elemeinek) továbbítására alkalmasak, a sor adatszerkezetet valósítják meg. A gyakorlatban is sokszor használunk hasonló sorokat elosztott/párhuzamos programok kommunikácijónának megvalósításához.

A specifikációban minden csatornaváltozóhoz tartozik egy történetváltozó is, ami a specifikálás, helyességbizonyítás kifejezőerejét biztosítja. A történetváltozóba minden csatornára írt üzenet bekerül, de míg a csatornáról olvasáskor az olvasott üzenetet általában töröljük, addig a történetváltozóból való törlés nem lehetséges. A csatornatörténetet a program nem használhatja.

Jelölés: $x,\overline{x} : Ch(\mathbb{T})$ ahol $\mathbb{T}$ az üzenetek típusa, $x$ a csatornaváltozó, $\overline{x}$ a csatornatörténet

Lemma: $\overline{x}-lorem(x) = hiext(\overline{x}-x,lov(x))$

## $split$

Egy csatorna két felé osztásának (vagy két csatorna egyesítésének) sorrendhelyességét, veszteségmentességét (minden üzenet megmarad) és zajmentességét (nincsenek légből kapott üzenetek) képes leírni.

Induktívan van definiálva a $split : Ch(T) \times Ch(T) \times Ch(T) \mapsto L$ logikai függvény:

- $split(<>,<>,<>) = Igaz$
- $split(a,b,c) \Rightarrow split(hiext(a,e),hiext(b,e),c)$
- $split(a,b,c) \Rightarrow split(hiext(a,e),b,hiext(c,e))$

## FORK (elágazás)

A bemeneti csatornára érkező adatokat a két kimeneti csatornájára írja (minden adatpontot csak az egyikre). Sorrendhelyes (adatok egy-egy kimeneti csatornán belüli relatív sorrendje megegyezik a bemeneti csatornán megfigyelhető relatív sorrenddel), veszteségmentes (minden beérkezett adatot kiír) és zajmentes (csak a beérkezett adatokat írja ki).

Felhasználás példa: load balancer

### FORK specifikáció

$A = \stackrel{Ch(T)}{x} \times \stackrel{Ch(T)}{\overline{x}} \times \stackrel{Ch(T)}{y} \times \stackrel{Ch(T)}{\overline{y}} \times \stackrel{Ch(T)}{z} \times \stackrel{Ch(T)}{\overline{z}}$

$B = \stackrel{Ch(T)}{x'} \times \stackrel{Ch(T)}{\overline{x'}} \times \stackrel{Ch(T)}{y'} \times \stackrel{Ch(T)}{\overline{y'}} \times \stackrel{Ch(T)}{z'} \times \stackrel{Ch(T)}{\overline{z'}}$

$x$ a bemeneti-, $y$ és $z$ pedig a kimeneti csatornák.

- $Q = (x=\overline{x}=x'=\overline{x'}=y=\dots=\overline{z'}=<>)$
- $Q \in INIT_h$
- $split(\overline{x}-x,\overline{y},\overline{z}) \in inv_h$ $\quad$ (sorrendhelyesség, veszteségmentesség, zajmentesség megkövetelése)
- $\forall k \in N: |\overline{x}| \ge k \hookrightarrow_h |\overline{y}| + |\overline{z}| \ge k$ $\quad$ (haladás megkövetelése)

### FORK megoldása

$S=(s_0: SKIP ; \; \begin{Bmatrix} s_1: x,y := lorem(x), hiext(y,lov(x)) \text{, ha } x \ne <> \\ s_2: x,z := lorem(x), hiext(z,lov(x)) \text{, ha } x \ne <> \end{Bmatrix})$
