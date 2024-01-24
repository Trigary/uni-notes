## Variancia

- `detached` helyett `attached` használata: a lehetséges értékek szűkítésének felel meg (kovariancia)
- Kapcsolt típus: `like Current` vagy `like featureName`
  - Ennek használatával könnyen tudunk kovariáns rutinokat készíteni, amelyek altípusra is helyesen működnek
  - A `like` kulcsszó az `attached/detachable` tulajdonságot is "lemásolja", de ez felülírható (pl. `detachable like featureName`)
- Függvénytípusok zárójelezése: $A \to (B \to C) \Leftrightarrow A \to B \to C$
- Függvénytípusok altípusossága:
  - $A <: A', B' <: B$  
    $\implies A' \to B' <: A \to B$
  - $A <: A' \land B <: B' \land C' <: C$  
    $\implies A' \to B' \to C' <: A \to B \to C$
  - $A' <: A \land B <: B' \land C' <: C$  
    $\implies (A' \to B') \to C' <: (A \to B) \to C$

### Liskov helyettesítési elvet nem sértő variancia

- Kovariáns eredmény típus
- Kovariáns utófeltétel
- Kontravariáns paraméter típus
  - Eiffel: csak kovariáns módon változtatható
- Kontravariáns láthatóság
  - Eiffel: kovariáns módon is változtatható
- Kontravariáns előfeltétel

Ahol nincs ellenkezőleg jelezve, arra van lehetőség Eiffelben.

## Kovariáns paraméter példa

~~~eiffel
deferred class ANIMAL
feature feed( f: attached FOOD ) deferred end
end

class CAT
inherit ANIMAL
feature feed( m: attached MILK ) do ... end
end

--Előny:
(create {CAT}).feed( (create {GRASS}) ) -- fordítási hiba

--Probléma: ("polymorphic CAT-call")
local
  a_cat: CAT
  some_grass: GRASS
  polymorphic: ANIMAL
do
  create a_cat; create some_grass
  a_cat.feed( some_grass ) -- fordítási hiba
  polymorphic := cat
  polymorphic.feed( some_grass ) -- sikeres fordítás :(
~~~

## CAT: Changed Availability or Type

Problémát okozhat, ha leszármaztatásnál...

- Vagy örökölt feature-nek csökken a láthatósága
- Vagy mezőnek/rutin paraméternek szűkül a típusa
