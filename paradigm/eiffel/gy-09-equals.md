## Egyenlőségvizsgálat és másolás

`is_equal`-t (`~`-t) és `copy`-t lehetőségünk van felüldefiniálni. Alapértelmezetten a sekély implementációval ekvivalensek.

A sekély egyenlőségvizsgálat az objektum mezőket `=`-vel hasonlítja össze. A mély egyenlőségvizsgálat a mezőket mély egyenlőségvizsgálattal hasonlítja össze.

A `clone` egy elavult művelet, helyette a `twin` használandó.

|Egyedi (`~`)|Sekély|Mély|
|:---:|:---:|:---:|
| **is_equal(b)** | frozen standard_is_equal(b) | frozen is_deep_equal(b) |
| frozen equal(a,b) | frozen standard_equal(a,b)|  frozen deep_equal(a,b) |
| **copy(b)** | frozen standard_copy(b) | frozen deep_copy(b) |
| frozen twin | frozen standard_twin | frozen deep_twin |
| ~~frozen clone(b)~~ | ~~frozen standard_clone(b)~~ | ~~frozen deep_clone(b)~~ |

~~~eiffel
a.copy(b)     -- a-ba másoljuk b-t
a := b.twin   -- másolat b-ről
a := clone(b) -- másolat b-ről (obsolate)
~~~

### Referencia típus egyenlőségvizsgálata

- Azonosság (referencia összehasonlítás): `a = b`
- Tartalmi egyenlőség, programozó által definiálva: `a ~ b`
- $=\; \subset \text{standard\_equal} \subseteq \text{equal} \equiv \;\sim$
- $=\; \subset \text{standard\_equal} \subseteq \text{deep\_equal}$

### Kifejtett típus egyenlőségvizsgálata

- Tartalmi egyenlőség, programozó által definiálva: `a ~ b` vagy `a = b` (kettő megegyezik)
- $\text{standard\_equal} \subseteq \;=\; \equiv \text{equal} \equiv \;\sim$
- $\text{standard\_equal} \subseteq \text{deep\_equal}$
