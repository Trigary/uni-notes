# Tétel 6: MUX (multiplexer)

A $split$ és a csatornák fogalma ehhez a tételhez is illenek, de korábbi tételben már kifejtésre kerültek, így nem lettek itt is leírva.

A két bemeneti csatornára érkező adatokat sorrendhelyesen a kimeneti csatornára írja. Sorrendhelyes (egy bemeneti csatornán belüli relatív sorrend a kimeneti csatornán is megjelenik), veszteségmentes (minden beérkezett adatot kiír) és zajmentes (csak a beérkezett adatokat írja ki).

Felhasználás példa: több csatornát egyesítünk, például több adatforrásból származó adatokat egy közös socket-en keresztül szeretnénk továbbítani

## MUX specifikáció

$A = \stackrel{Ch(T)}{x} \times \stackrel{Ch(T)}{\overline{x}} \times \stackrel{Ch(T)}{y} \times \stackrel{Ch(T)}{\overline{y}} \times \stackrel{Ch(T)}{z} \times \stackrel{Ch(T)}{\overline{z}}$

$B = \stackrel{Ch(T)}{x'} \times \stackrel{Ch(T)}{\overline{x'}} \times \stackrel{Ch(T)}{y'} \times \stackrel{Ch(T)}{\overline{y'}} \times \stackrel{Ch(T)}{z'} \times \stackrel{Ch(T)}{\overline{z'}}$

$x$ és $y$ a bemeneti-, $z$ pedig a kimeneti csatorna.

- $Q = (x=\overline{x}=x'=\overline{x'}=y=\dots=\overline{z'}=<>)$
- $Q \in INIT_h$
- $split(\overline{z},\overline{y}-y,\overline{x}-x) \in inv_h$ $\quad$ (sorrendhelyesség, veszteségmentesség, zajmentesség megkövetelése)
- $\forall k \in N: |\overline{x}| \ge k \hookrightarrow_h |\overline{x} - x| \ge k$ $\quad$ (haladás megkövetelése)
- $\forall k \in N: |\overline{y}| \ge k \hookrightarrow_h |\overline{y} - y| \ge k$ $\quad$ (haladás megkövetelése)

## MUX megoldása

$S=(s_0: SKIP ; \; \begin{Bmatrix} s_1: x,z := lorem(x), hiext(z,lov(x)) \text{, ha } x \ne <> \\ s_2: y,z := lorem(y), hiext(z,lov(y)) \text{, ha } y \ne <> \end{Bmatrix})$
