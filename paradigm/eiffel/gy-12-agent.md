## Magasabb rendű függvények

### Call agent (parciális függvény alkalmazás)

Hivatkozás egy függvényre, illetve annak parciális alkalmazása. A hívás nem hajtódik végre, csak hivatkozunk rá.

~~~eiffel
class TEST
feature
  f(s: STRING, i: INTEGER): REAL do ... end
  g(s: STRING) do ... end

  test(obj: TEST)
  do
    ... = agent f --Típus: FUNCTION[STRING, INTEGER, REAL]
    ... = agent Current.g --Típus: PROCEDURE[STRING]
    ... = agent obj.f("abc", ?) --Típus: FUNCTION[INTEGER, REAL]
    ... = agent {TEST}.f(?, 42) --Típus: FUNCTION[TEST, STRING, REAL]

    --FUNCTION és PROCEDURE ősosztálya: ROUTINE
    --A függvényhez tartozó osztálypéldány ha nincs megadva, akkor úgy működik, mint egy paraméter
    --FUNCTION: utolsó típusparaméter a visszatérési érték típusa
  end
end
~~~

~~~eiffel
foreach(f: PROCEDURE[REAL]; a: ARRAY[REAL])
do
  across a as i loop p(i.item) end --agent meghívása: zárójelekkel
end
~~~

### Lambda, inline-agent

- Rutin szintaxissal megegyezik, csak név helyett `agent` kulcsszót tartalmaz
  - Lehet elő- és utófeltétele, lokális változója
  - Használhatja a befoglaló osztály feature-jeit
- Nem használhatja a befoglaló rutin lokális változóit

~~~eiffel
agent(x,y: INTEGER): BOOLEAN do Result := x-y > 5 end
~~~
