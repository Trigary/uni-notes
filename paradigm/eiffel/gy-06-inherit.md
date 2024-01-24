## Öröklődés

- Osztályból való öröklődés letiltható a `frozen` kulcsszóval: `frozen class XYZ ...`
  - Valójában csak altípusosságot tiltja meg: privát öröklődést nem
- Alosztályban el lehet érni a szülőosztály titkos dolgait

~~~eiffel
class SAVINGS_ACCOUNT
inherit ACCOUNT
  rename make as make_account --Rutin átnevezhető
  end
create
  make_account --Feature a szülőosztályból származik
  make
feature
  --Újabb make_account deklarálása névütközést okozna
  interest: INTEGER assign set_interest
  make(id_, interest_: INTEGER)
    require
      non_negative_interest: interest_ >= 0
    do
      make_account(id) --Szülő "konstruktor" hívás
      set_interest(interest_)
    require
      id = id_; interest = interest_
    end
  ...
invariant
  interest >= 0
end
~~~

~~~eiffel
local
    a: attached ACCOUNT
do
    create {SAVINGS_ACCOUNT} a.make(42, 3)

    a.set_interest(10) -- fordítási hiba
    
    if attached {SAVINGS_ACCOUNT} a as sa then
        sa.set_interest(10)
    end
    
    check attached {SAVINGS_ACCOUNT} a as sa then
        sa.set_interest(10)
    end
~~~

### Rutinok felüldefiniálása

- Jelezni kell a felüldefiniálást: `inherit XYZ redefine abc end`
- Felüldefiniálás letiltható a `frozen` kulcsszóval: `feature frozen xyz ...`
- Megörökölt ("előző") implementáció meghívható a `Precursor` kulcsszóval
  - Több előző implementáció közül a `Precursor {CLASSNAME}` szintaxis segítségével kiválasztható, hogy melyik `Precursor`-re gondolunk

### Kifejtett osztályok öröklődése

- Kifejtett osztály lehet altípusa egy referencia típusnak
- Kifejtett osztályból való öröklődés viszont **nem** vezet be altípusosságot
  - Akkor se, ha egy referencia típusú osztály örököl a kifejtett osztályból

### Privát öröklődés

- Csak a kód öröklődik
  - **Nem** vezet be altípusosságot
  - Gyakorlatilag bemásolásra kerül a kód:
    - Publikus feature publikus marad
    - Once rutin így "többször" le fog futni (egyszer az eredeti osztályban, egyszer ebben az új osztályban)
- Lehet `frozen` osztályból privát módon örökölni

### Örökölt rutinok láthatóságának megváltoztatása

~~~eiffel
class QUEUE[T]
inherit {NONE} SEQUENCE[T]
  export {ANY} hiext, lov, lorem, size;
         {QUEUE} all -- e.g. hirem, hiv, loext
  end
end
~~~

## Absztrakt osztályok, rutinok

- `deferred` osztályban deklarálható implementáció nélküli (`deferred`) rutin
- `deferred` osztályból öröklődés során...
  - Vagy meg kell adni a hiányzó implementációt
  - Vagy az öröklő osztálynak is `deferred`-nek kell lennie
- Hiányzó (`deferred`) implementáció megvalósítása esetén nem kell `redefine`-t írni
- Szülőosztály rutin implementációja eldobható a `redefine` kulcsszóval, ilyenkor `deferred` rutin lesz belőle
  - Többszörös öröklődésnél lesz elsősorban hasznos, hogy ne legyen egy rutinhoz több implementáció
- A `deferred` a `do`-t helyettesíti; elő- és utófeltétel megadható attól még

~~~eiffel
deferred class ANIMAL
feature
  talk: STRING deferred end
end

--Egy lehetséges megvalósítás
class CAT
inherit ANIMAL
feature
  talk: STRING do Result := "Miaow" end
end

--Alternatív megvalósítás
class CAT
inherit ANIMAL redefine default_create end
feature
  talk: STRING
  default_create do talk := "Miaow" end
end
~~~

## Többszörös öröklődés

- Lehet publikus és privát módon is több osztályból örökölni
  - Egy osztályt akár többször is meg lehet örökölni
- Amennyiben több feature...
  - Különböző néven öröklődés: több feature lesz
  - Azonos néven öröklődik, több irányból, azonos implementációval: egy feature lesz (automatikus join)
    - Csak akkor, ha ezek a feature-ök azonosak, azaz diamond inheritance esetén
  - Azonos néven öröklődik, több irányból, eltérő implementációval: nem lehet join-olni
  - Azonos néven öröklődik, több irányból, egy kivételell mindegyik `deferred`: egy feature lesz (automatikus join)

### Névütközések feloldása

- `rename xyz as abc`: két külön feature-t készítünk az ütköző feature-ökből
- `undefine xyz`: az egyik implementációt eldobjuk, így egy feature lesz
- `redefine xyz`: mindegyik implementációt eldobjuk és új implementációt adunk meg, így egy feature lesz

~~~eiffel
class STATEMENT -- inherits is_equal from ANY
  feature is_right: BOOLEAN
end

class PARTY -- inherits is_equal from ANY
  feature is_right: BOOLEAN
end

--Névütközés, fordítási hiba
class CAMPAIGN_PROMISE
inherit STATEMENT PARTY
end

--Öröklődés során átnevezés
class CAMPAIGN_PROMISE
inherit
  STATEMENT rename is_right as holds end
  PARTY
end --is_equal: két irányból azonos implementáció => join
~~~

### Dinamikus kötés, late binding, dynamic dispatch

~~~eiffel
class ALPHA
feature test: INTEGER do Result := 1 end
end

class BRAVO
feature test: INTEGER do Result := 2 end
end

class TEST
inherit
  ALPHA rename test as t end
  BRAVO
end

--Mi fut le?
local
  a: attached ALPHA
  b: attached BRAVO
  y: attached TEST
do
  create y; a := y; b := y
  print(a.test) --ALPHA#test = TEST#t
  print(b.test) --BRAVO#test = TEST#test
  print(y.test) --BRAVO#test = TEST#test
  print(y.t)    --ALPHA#test = TEST#t
~~~

Diamond inheritance esetén a `select` klóz segítségével meg kell mondani, hogy melyik "oldalon" található implementáció fusson le.

~~~eiffel
deferred class BASE
feature test: INTEGER deferred end
end

class ALPHA
inherit BASE
feature test: INTEGER do Result := 1 end
end

class BRAVO
inherit BASE
feature test: INTEGER do Result := 2 end
end

--Megtartjuk a feature mindkét implementációját, de az egyiket átnevezzük
class TEST
inherit
  ALPHA rename test as t end
  BRAVO select test end --Kötelező a select klóz
end

--Mi fut le?
local
  x: attached BASE
  a: attached ALPHA
  b: attached BRAVO
  y: attached TEST
do
  create y; x := y; a := y; b := y
  print(x.test) --BRAVO#test = TEST#test
  print(a.test) --BRAVO#test = TEST#test (!!!)
  print(b.test) --BRAVO#test = TEST#test
  print(y.test) --BRAVO#test = TEST#test
  print(y.t)    --ALPHA#test = TEST#t
~~~

### Ismételt öröklődés

~~~eiffel
class BINTREE[T]
inherit CELL[T] -- provides item and put
inherit {NONE}
    LINKED
        rename
            next as left,
            set as set_left
        end
    LINKED
        rename
            next as right,
            set as set_right
        end
create put
end
~~~
