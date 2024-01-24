## Generikus osztály

~~~eiffel
class ARRAY [G] --Típusparaméter
...
~~~

- A típusparaméter általában kovariáns
  - Statikusan nem biztonságos a típusrendszer
  - `class STACK[T]` $\implies$ `STACK[INTEGER] <: STACK[ANY]`
- Kivétel: `frozen` típusparaméter esetén invariáns
  - `class STACK[frozen T]`

~~~eiffel
VECTOR[G -> ADDABLE] -- G-nek ADDABLE alosztályának kell lennie (upper bound)
HASHTABLE[K -> HASHABLE, V] -- több típusparaméter
VECTOR[G -> {ADDABLE, HASHABLE}] -- több megszorítás

VECTOR[G -> ADDABLE create make end] --G származzon le az ADDABLE osztályból és
--   tegye az ADDABLE osztályban található make feature-t egy creation procedure-ré.
-- Lehet, hogy G-nek megfeltetett osztályban az ADDABLE-ben make-nek hívott feature-t
--   már nem make-nek hívják, de ez nem probléma, mi make-ként fogunk hivatkozni rá.

VECTOR[G -> ADDABLE rename add as plus end] -- átnevezés
VECTOR[G -> {A rename v as w, B}] -- átnevezés, hogy A és B között ne legyen névütközés
--A rename valójában nem megszorítás: nem befolyásolja a típusparaméter helyére
--  helyettesített osztályt. Az az osztály lehet, hogy névütközés esetén például
--  `A rename v as w` helyett a más nevet ad `v`-nek, vagy inkább a `B` osztály `v`
--  metódusát nevezi át a névütközés feloldása érdekében.

VECTOR[frozen G -> ADDABLE] -- invariant generic param
~~~

## Tuple

~~~eiffel
t2: TUPLE[INTEGER,INTEGER]
t2 := [1,3]
t2.item(0) --ANY típusú, hiszen az item rutin így van definiálva

t3: TUPLE[i,j: INTEGER; r:REAL] --Rekord féleség, név rendelhető komponenshez
t3 := [1,3,0.0]
t3.i --INTEGER típusú
~~~

`TUPLE[INTEGER,STRING] <: TUPLE[INTEGER,ANY] <: TUPLE[INTEGER] <: TUPPLE[ANY] <: TUPLE`
