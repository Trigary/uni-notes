## Szerződések

Három fő szerződés van:

- Osztály invariáns (`invariant`)
  - Létrehozó művelet beállítja az invariáns
  - Műveletek megőrzik az invariánst
- Rutin előfeltétel (`require`)
  - Osztály mezőit és a rutin paramétereit használhatja
- Rutin utófeltétel (`ensure`)

~~~eiffel
class TEST
feature
  counter: INTEGER
  nullable: detachable STRING
feature
  div(a: INTEGER, b: INTEGER): INTEGER
    require
      division_by_zero: b /= 0
    do
      counter := counter + 1
      Result := a // b
    ensure
      counter_incremented: counter = old counter + 1
      Result = a // b --Címke megadása nem kötelező
      frame: strip(counter) ~ old strip(counter)
    end
invariant
  some_check: counter >= 0
  --detachable attribútum esetén körülményes a tud lenni check megfogalmazás:
  another: nullable /= Void implies attached nullable as a_n and then a_n.count > 0
end
~~~

### `old` operátor

- Csak utófeltételben szerepelhet
- `old xyz` azt az értéket adja vissza, ami `xyz` értéke volt, a rutin futásának legelején

### Frame problem, frame rule, frame condition

- Utófeltételbe ha nem írjuk bele, hogy nem változik meg minden, akkor nem is lenne szabad feltételezni
- Egy megoldás: `strip(a,b,c)` segítségével ellenőrizhető, hogy `a,b,c` mezők kivételével minden ugyan az maradt-e `Current`-ben
- Példa: `frame: strip(balance) ~ old strip(balance)`

### Ellenőrzések `do` klózban

Futási idejű assertion-ök hozhatók létre az alábbi módon. A program végrehajtás csak akkor halad tovább, ha az állítások igazak.

~~~eiffel
check size <= capacity end

check
	size_is_not_too_large: size <= capacity
end
~~~

### Felüldefiniált rutin elő- és utófeltétele

~~~eiffel
class IDOPONT
inherit
  DATUM redefine from_array end
create
    from_array
feature
    from_array(arr: attached ARRAY[INTEGER])
    require else --Előfeltétel gyengítése: vagy az eredetinek, vagy ennek kell teljesülnie
        arr.count = 5
    do ...
    ensure then --Utófeltétel szigorítása: mind a réginek, mind ennek teljesülnie kell
        ora = 0 or else ora = arr[arr.lower+3] \\ 24
        perc = 0 or else perc = arr[arr.lower+4] \\ 60
    end
end
~~~

- Nincs `require` $\equiv$ `require True`
- Nincs `require else` $\equiv$ `require else False`
- Nincs `ensure` $\equiv$ `ensure True`
- Nincs `ensure then` $\equiv$ `ensure then True`

### Hoare-hármas

- `{előfeltétel} program {utófeltétel}` szintaxisban megadott elő- és utófeltétel
- Továbbiakban ezt a jelölést fogjuk alkalmazni
