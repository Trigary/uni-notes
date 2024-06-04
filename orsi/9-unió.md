# Tétel 9: Unió definíció + viselkedési tulajdonságok

Szekvenciális programok kompozicionálisak: ha $\{Q\}S_1\{P\}$ és $\{P\}S_2\{R\}$, akkor $\{Q\}S_1;S_2\{R\}$. Azaz a programok a belső működésük ismerete nélkül komponálhatóak. Ez egy kicsit bonyolultabban, de többnyire absztrakt párhuzamos programok esetén is működik. Tehát teljesül a célunk, hogy ne kelljen értékadásokból újra kiszámolni mindent, hanem a komponens programok tulajdonságait használjuk csak fel.

Emlékeztető: összefésüléses szemantikát használunk, így az utasítások (diszjunkt) uniója (bizonyos szempontból) nem eredményez nem várt állapotokat.

Kitekintés: programok egyesítéséhez hasonlóan feladat specifikációk is egyesíthetőek. Bár a tényleges célunk nem ez lenne, hanem a feladat specifikáció dekomponálás: bonyolult feladatot automatikusan, szabályok alapján, bizonyítottan helyes módon több kisebb részfeladatra bontani. Ez a probléma megoldatlan.

## Definíció

Definíció: Legyen $S_1 = (s_{1,0},\{s_{1,1},\dots,s_{1,k}\})$ és $S_2 = (s_{2,0},\{s_{2,1},\dots,s_{2,m}\})$. Ekkor $S=S_1 \cup S_2=(s_0,\{s_{1,1},\dots,s_{1,k},s_{2,1},\dots,s_{2,m}\})$ amennyiben $s_0 = s_{1,0} \parallel s_{2,0}$ és az $s_{1,0},s_{2,0}$ kezdeti értékadások azonos értéket rendelnek az összes $S_1$ és $S_2$ legnagyobb közös alterén található változókhoz.

Megjegyzés: $S_1 \cup S_2$ utasításai az $S_1$ és az $S_2$ utasításainak a diszkrét uniója. Továbbá általánosított feltételes értékadások az utasítások, azaz ha explicit nem is rendel egy utasítás egy változóhoz egy értéket, akkor a korábbi (azonos) értéket rendeli hozzá.

Azaz ammenyiben a $S_1$-nek és $S_2$-nek van közös változója (bárhol a programban, nem csak a kezdeti értékadást nézve), akkor csak akkor képezhető únió, ha $S_1$ és $S_2$ kezdeti értékadása azonos értéket rendel hozzájuk. Ez az invariánsok teljesülése miatt fontos: az invariánsok teljesülését a kezdetei értékadások biztosítják. Valójában ezt nem kötelező megkövetelni, az előadó (önmagának ellentmondva) mutatott olyan példát, ahol ez nem állt fenn. Viszont ilyen esetben is fennáll az invariánsok kompozicionalitása egy bizonyos feltétel miatt, amely majd a hozzá tartozó szabályban olvasható.

## Viselkedési tulajdonságok

Legyen $S = S_1 \cup S_2$

- $\triangleright_S = \triangleright_{S_1} \cap \triangleright_{S_2}$
  - Definícióból triviálisan belátható: minden $s \in S_1$ és $s \in S_2$ esetén is igaz, és $S$-ben is ezek az $s$-ek találhatóak
- $\mapsto_S = \triangleright_{S_1} \cap \triangleright_{S_2} \cap (\mapsto_{S_1} \cup \mapsto_{S_2})$
  - Definícióból triviálisan belátható ($\exists$ miatt van $\mapsto$ között $\cup$)
- $\hookrightarrow_S = (\triangleright_{S_1} \cap \triangleright_{S_2} \cap (\mapsto_{S_1} \cup \mapsto_{S_2}))^{tdl}$
  - Kimondható lenne egy ilyesmi tétel: $\hookrightarrow_{S_1}$ és $\hookrightarrow_{S_2}$ és a kettőhöz tartozik közös variánsfüggvény, akkor $\hookrightarrow_S$
- Ha $sp(s_0, Q) \Rightarrow sp(s_{1,0},Q) \land sp(s_{2,0},Q)$ akkor $inv_{S_1}(Q) \cap inv_{S_2}(Q) \subseteq inv_S(Q)$
  - Szerintem ez nem tökéletes: a feltétel nem szükséges, hiszen definíció szerint $s_{1,0}$ és $s_{2,0}$ nem rendel közös változókhoz eltérő értéket. Az előadáson ismertetett példa (hogy miért szükséges a feltétel) egy helytelen példa, hiszen a kezdeti értékadások eltérő értékeket rendeltek közös változókhoz (egyik $x,y:=x+1,y$, másik $x,y:=x,y+1$).
  - Csak részhalmazt tudunk kimondani: az unióban megjelenhet újabb invariáns is
- $\varphi_S = \varphi_{S_1} \land \varphi_{S_2}$
  - Definícióból triviálisan belátható: ott van fixpontban, ahol sem $S_1$, sem $S_2$ utasításai nem okoznak változást
- $FP_{S_1} \cap FP_{S_2} \subseteq FP_S$
  - Definícióból ($\varphi_S \Rightarrow R \equiv R \in FP_S$) triviálisan belátható, hiszen a fixpont csak szigorúbb lett
  - Csak részhalmazt tudunk kimondani: az unióban megjelenhet újabb tulajdonság is
- $((\triangleright_{S_1} \cap \triangleright_{S_2} \cap (\mapsto_{S_1} \cup \mapsto_{S_2}))^{tdl})^{(-1)}(\varphi_S) = TERM_S$
  - $TERM_S$-ből indulva görbenyíllal eljutunk $\varphi_S$-be. Vegyük a görbenyíl inverzét és induljunk fixpontból, azaz ellenkező irányba haladjunk. Így megkapjuk $TERM_S$-t.
