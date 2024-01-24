## Utasítások

### Elágazás

~~~eiffel
if ev \\ 400 = 0 then Result := True
elseif ev \\ 100 = 0 then Result := False
elseif ev \\ 4 = 0 then Result := True
else Result := False
end
~~~

### Sokágú elágazás: `inspect`

~~~eiffel
inspect honap
  when 1,3,5,7,8,10,12 then Result := 31
  when 4,6,9,11 then Result := 30
  when 2 then
    if szokoev then Result := 29
    else Result := 28
    end
end
~~~

### Általános ciklus

~~~eiffel
gcd(a, b: INTEGER): INTEGER
local
  number: INTEGER
do
  from
    Result := a
    number := b
  invariant
    0 < Result; 0 < number
    -- gcd(a,b) = gcd(Result,number) --This is logically correct, but would result in infinite recursion
  until
    Result = number
  loop
    if Result > number
    then Result := Result - number
    else number := number - Result
    end
  variant
    Result + number
  end
end
~~~

#### Ciklus szerződése

~~~eiffel
from INIT -- Inicializáció; utasítás(ok)
invariant INV -- Végig igaz állítás; A -> BOOLEAN
until COND -- Terminálási feltétel; A -> BOOLEAN
loop BODY -- Ciklustörzs; utasítás(ok)
variant VAR -- Iterációnként csökkenő érték; A -> INTEGER
end
--"A" jelentése: állapottér, azaz paraméterek, lokális változók, adattagok értékei
~~~

Egy ciklus megfelel a szerződésnek, ha:

- $\{True\} \; INIT \; \{INV\}$
- $\{INV \land \neg COND\} \; BODY \; \{INV\}$
- $INV \implies VAR \ge 0$
- $\forall v: \{INV \land \neg COND \land VAR=v\} \; BODY \; \{VAR < v\}$

### Bejáró ciklus

~~~eiffel
across
  <<1969, 7, 20, 20, 17, 40>> as i
loop
  print(i.item.out) --i egy iterátor féleség, i.item tartalmazza az értéket
  print("%N")
end
~~~

### Kvantálás

Hasznos például elő- és utófeltételek írásánál, hiszen ezek logikai kifejezések.

~~~eiffel
--Univerzális kvantálás
mybool := across <<7, 20, 20, 17, 40>> as i all i.item > 0 end

--Egzisztenciális kvantálás
mybool := across <<7, 20, 20, 17, 40>> as i some i.item = 17 end
~~~
