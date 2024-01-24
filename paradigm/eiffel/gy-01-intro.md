# Gyakorlati jegyzet

- [Standard könyvtár dokumentáció](https://www.eiffel.org/files/doc/static/trunk/libraries/class_list.html)

## Általános információk, elvek

- Minden kód osztályokban található
  - Főprogram is egy osztály, fordításkor kell megadni
  - Nincs statikus rutin, attribútum
- Szerződések futási időban kerülnek ellenőrzésre
  - Valójában a szerződések csak szintaxikus cukorkák assertion-ök beszúrására
- Szerződésekkel nem tudunk mindent kifejezni
  - pl. `gcd(a,b)=gcd(Result,number)` szerződés végtelen rekurzióhoz vezetne
  - Ha ki is tudunk valamit fejezni, lehet, hogy nem jó ötlet, mert túl lassú lesz tőle a program

## Vizsgán ezekre oda kell figyelni

- Adattaghoz tilos "triviális" gettert írni
  - Indoklás: adattagot csak az ő osztálypéldánya tudja módosítani, senki más. Tehát korlátozni a láthatóságát és gettert készíteni hozzá hülyeség.
  - Viszont néha szükséges, hogy a belső állapot ne szökhessen ki.
- Ciklushoz kötelező invariáns és termináló függvényt írni
- `attached` kulcsszót mindenhol explicit ki kell írni
- Frame condition-t is írjunk `ensure`-be

Saját tippjeim:

- Ha valami nem működik és a hibaüzenet teljesen értelmetlennek tűnik, akkor keressük a hibát az alosztályokban (pl. ideiglenesen töröljük ki őket - megszűnik így a hiba?)
  - Gyakori hiba: hiányzik alosztályból egy creation procedure

## Szintaxis, konvenciók

- Pontosvessző opcionális
- Kis- és nagybetűk:
  - Nem számít a különbség kulcsszavakban, azonosítókban, viszont vannak konvenciók
  - Kulcsszó: kisbetű
  - Egyéb foglalt szó: első betű nagy
    - pl.: `Result`, `Current`, `Void`, `True`, `False`
  - Konstans: `Line_width: INTEGER = 256`
  - Osztálynév: csupa nagybetű
  - Többi azonosító: kisbetű
  - Tagolás: alsó vonás (`_`)
- Üres paraméterlistát (`()`) nem írjuk ki
  - Így attribútumok és paraméter nélküli függvények használata megegyezik

| Más nyelvek | Eiffel |
|:------------|:-------|
| `this` | `Current` |
| `return ...` | `Result := ...` |
| `null` | `Void` | 

## Láthatóság

- Publikus: `{ANY}` vagy semmi
- Protected-szerűség: `class C feature {C} f ... end`
- Titkos: `{NONE}`
  - Nem ugyan az, mint a privát más nyelvekből:
    - Ez nem osztályra, hanem objektumra privát (azaz csak saját magadra lehet meghívni)
    - Alosztályban el lehet érni a szülőosztály titkos dolgait

## Alapvető műveletek, operátorok, konstansok

- `True`, `False`
- `not`, `and`, `or`, `xor`, `implies`
- Lusta operátorok: `and then`, `or else`
- Egész osztás: `//`; osztási maradék: `\\`; hatványozás: `^`
- `<`, `<=`, `>`, `>=`
- Referencia összehasonlítás: `=`, `/=`
- Belső tartalom összehasonlítás: `~`, `/~`
- Tömb indexelés: `[,]`
- Típusozott literál: `{REAL_32} 3.14`
- Egész intervallumon iterálás: `across n |..| m as i ...`
- Standard kimenetre írás
  - Objektum kiírása: `print(something.out)`
  - Újsor: `print("%N")`

## `Void`-biztonság, `attached`, `detachable`

- Csak referencia típusoknál kell erre figyelni
- `Void`-mentes változó: `attached TÍPUS`
  - Ez az alapértelmezett, de explicit ki kell írni a vizsgán
- `Void`-ot tartalmazni képes változó: `detachable TÍPUS`

~~~eiffel
local
  d: detachable MYCLASS
do
  d.something() --Hiba: d egy detached típusú változó

  if attached d as ad then
    ad.something()
  end

  --Check: szerződésszegés történik, ha nem teljesül a feltétel
  --Emiatt egyes esetekben jobb, mint az if: itt nincs else ág
  check attached d as ad then
    ad.something()
  end

  if d /= Void then
    d.something() --CSAK paraméter/lokális változó esetén tudja a fordító, hogy d itt már attached
  end
end
~~~
