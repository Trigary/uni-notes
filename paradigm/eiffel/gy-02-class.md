## Osztályok

~~~
(expanded) class <OSZTÁLYNÉV>
feature <RUTINOK, ATTRIBÚTUMOK>
end
~~~

- Referencia típus (`class`)
  - Indirekt hozzáférés, aliasing
  - Heap memória, szemétgyűjtés
  - Polimorf változók, dinamikus kötés
  - Üres referencia: `Void`
  - Megosztással került átadásra (call-by-sharing)
    - Vigyázni kell, mert például több paraméter is mutathat ugyan arra az objektumra, példáulra `Current`-re
  - Belső állapotot könnyű kiszivárogtatni
    - `STRING` esetén érdemes a `twin` feature-t használni ennek elkerülése érdekében
- Kifejtett típus (`expanded class`)
  - Direkt módon érhető el, kifejtésre kerül a tároló osztályba/verembe
  - Érték szerint kerül átadásra (call-by-value)
  - Kifejtett típusnak nincs alosztálya

### Rutinok: eljárások és függvények

~~~eiffel
--Eljárás: nincs visszatérési érték, csak mellékhatást okoz
--  Neve legyen cselekvést leíró ige, pl. divide_by
my_procedure(param_a: STRING; param_b: INTEGER)
  local
    temp: INTEGER --Nullaszerű értékre inicializálódik
  do
    --Paraméterek értéke nem írható felül
    print("Hello World!%N")
  end 

--Függvény: van visszatérési érték
--  Neve legyen eredményre utaló névszó, pl. divided_by
my_function(param_b: INTEGER): INTEGER
  do
    Result := param_b + 1
  end

--Once rutin: törzs csak egyszer fut le, az eredmény elmentésre kerül és későbbiekben
--  csak az kerül visszaadásra. Probléma: ha más paraméterekkel kerül meghívásra
--  másodjára, akkor is az eredetileg kiszámolt értéket adja vissza.
feature
  talk(kind_of_ignored: INTEGER): INTEGER
  once
    Result := kind_of_ignored
  end
~~~

### Operátorok

~~~eiffel
feature --1 formális paraméter => bináris infix operátor: (lehet bármilyen karaktersorozat)
  multiplied_by alias "*" (other: attached FRACTION): attached FRACTION
    do create Result.set(num * other.num, ...) end

--Használat:
x, y, z: FRACTION
z := x * y --Jelentése: x.multiplied_by(y)

feature --0 formális paraméter => Unáris prefix operátor: -
negated alias "-": attached FRACTION
	do create Result.set (-num, den) end

--Használat:
x, y: FRACTION
y := -x --Jelentése: x.negated
~~~

`[]` alias-szal rendelkező operátor akárhány aritású lehet, használat: `x[a,b,c]`

### Attribútumok (adattagok)

Attribútumnak értéket adni csak az osztályának a saját **példánya** tud. Következmény:

- Láthatóságot általában nem szükséges korlátozni. Gettert nem kell írni.
- Kivülről módosításhoz egy setter rutin szükséges.

~~~eiffel
feature -- egyszerű attribútum
  --Nullaszerű értékre inicializálódnak
  name: STRING --Kezdeti érték: Void
  x, y: INTEGER --Kezdeti érték: 0

feature -- konstans
  Answer: INTEGER = 42

feature -- inline inicializáció: nem használjuk, mert az osztály invariáns ellenőrzés után fut csak le
  test: INTEGER attribute Result := 123 end

feature -- szintaktikus cukorka: := operátor támogatása
  name: STRING assign set_name
  set_name(new_name: STRING)
    do name := new_name.twin end
  set_friends_name(friend: PERSON)
    --Ez történik valójában: friend.set_name(name)
    do friend.name := name end
~~~
