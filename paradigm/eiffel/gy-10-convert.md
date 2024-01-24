## Konverzió

~~~eiffel
class FRACTION
create
   from_integer
convert
  from_integer({INTEGER}), --konverzis eljárás: más típus -> saját típus
  from_array({ARRAY[INTEGER]}),
  to_real:{REAL_64} --konverzis függvény: saját típus -> más típus
feature
  from_array(arr: attached ARRAY[INTEGER])
    require arr.count = 2
    do ... end
  ...
end

--Használat:
f: attached FRACTION
r: REAL_64
f := 3 -- create f.from_integer(3)
f := <<3,1>> -- create f.from_array(<<3,1>>)
r := f -- f.to_real
~~~
