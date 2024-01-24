## Kivételkezelés

~~~eiffel
feature
my_routine
  require
    ... --Előfeltétel egy retry során nincsen ellenőrizve
  local --Lokális változó megőrzi értékét egy retry során
    already_tried: BOOLEAN
  do
    if not already_tried then
      --Normal operation
    else
      --Alternative operation
    end
  rescue --Ez fut le, ha kivétel lépett fel (ez van try-catch helyett)
    if not already_tried then
      already_tried := True
      retry --Csak rescue-ban lehet
      --Akár végtelenszer is lehetne retry-olni
    end --Ha nem retry-olunk: feljebb terjed a kivétel a stack-en
  end
~~~

### Kivételkezelés fajtái

- Organized panic
  - `resuce` egy `retry` nélkül fejeződött be
  - Az utófeltételt nem teljesül
  - Az osztályinvariánst helyreállítottuk
  - A kivétel feljebb terjed a stack-en
- Újrapróbálkozás
  - Törzs újra le fog futni
  - Az előfeltételt a `resuce`-ban helyreállítottuk

### `default_rescue`

- `ANY`-ből mindenki megörökli
- Felüldefiniálható, de nem szerepelhet benne `retry`
  - Megjegyzés: át is nevezhető a `default_rescue`
- Alapból nem csinál semmit
- Ez fut le, ha nincs egy rutinhoz explicit megadva egy `rescue` klóz

### Kivételek

~~~eiffel
class CONNECTION
inherit EXCEPTIONS --Így elérhető a `raise` rutin
...
  ... raise( "Communication_failure" ) ... --Kivételek szöveges üzenetek
...
end
~~~
