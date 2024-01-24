## Példányok létrehozása, creation procedure

~~~eiffel
class PERSON
create set_name --Opcionálisan megadható láthatóság, pl. protected: create {PERSON} set_name
--{NONE} láthatóságú feature lehet (publikus) creation procedure: független a láthatóságuk
feature set_name(name: attached STRING) do ... end
--Gyakori creation procedure: saját osztálypéldányt paraméterként

--Használat:
local
  bob: attached PERSON
  alice: ANY
do
  create bob.set_name("Bob") -- utasítás
  bob := create{PERSON}.set_name("Bob") -- kifejezés
  create{PERSON} alice.set_name("Jack") -- változónak alosztály értéket adunk
~~~

Szülőosztály creation procedure-jei nem lesznek az alosztálynak is creation procedure-jei: minden osztálynak saját magának deklarálni kell, hogy mely rutinok a creation procedure-ök (kivétel: `default_create`).

### `default_create`

Osztály nem ad meg creation procedure-t $\implies$ az `ANY` osztályból megörökölt azon művelet egy engedélyezett creation procedure lesz, amit az `ANY` osztályban `default_create`-nek neveznek. Ez a creation procedure lehet, hogy át lett nevezve.

~~~eiffel
--default_create-et (eredeti vagy új nevén) nem kell kiírni meghíváskor
class TEST
inherit
  ANY rename default_create as make end
end

create {TEST} --create {TEST}.make
~~~

~~~eiffel
--default_create "letiltásra" kerül, ha van másik creation procedure, de explicit újra lehet engedélyezni
class TEST
create make, default_create
feature make do end
end
~~~

~~~eiffel
--default_create felüldefiniálása
class MYCLASS
inherit ANY redefine default_create end
create default_create
feature default_create do ... end
end
~~~
