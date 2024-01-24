# Minta ZH megoldása

Az alábbi feladat Kozsik Tamás honlapján publikusan megtaláhlató: [peldazh.html](https://kto.web.elte.hu/hu/oktatas/eiffel/anyagok/peldazh.html)

## Feladatleírás

### Irányított gráfot reprezentáló adattípus

Készítsünk egy `DIGRAPH` osztályt, amely egy irányított gráfot ábrázol. A gráf csúcspontjaiban tárolt elemek típusát sablonparaméterként kapjuk. Ez az elemtípus `HASHABLE` kell legyen. A gráfot ábrázoljuk hasítótáblával a következőképpen: a kulcsok értelemszerűen a csúcsok lesznek, egy kulcshoz pedig azon csúcsok halmazát rendeljük, ahova vezet él. Használjuk az Eiffel beépített `HASH_TABLE` és `ARRAYED_SET` típusát.

- Valósítsuk meg a legfontosabb gráfműveleteket: csúcs hozzáadása, él létrehozása, és egy feature-t, ami eldönti, hogy van-e él két csúcs között.
- Biztosítsuk azt, hogy élet csak akkor tudunk létrehozni, ha a végpontjai ebben a gráfban vannak.
- A gráf a creation feature-ét örökölje a hasítótáblából (csak nevezzük át `init`-nek!), amely paraméterként a hasítótábla kapacitását kapja.

### Irányítatlan gráf

Készítsünk egy olyan `GRAPH` osztályt, amelyet a `DIGRAPH` osztályból származtatunk, és egy irányítatlan gráfot valósít meg. Ezt úgy érhetjük el, hogy az élet mindkét irányba felvesszük.

### Unió

Készítsünk olyan creation feature-t a `DIGRAPH` osztályban, amely két gráfot kap paraméterként, és egy olyat hoz létre, amely a két paraméter uniója. A típushelyesség biztosítása mellett (azaz egy irányítatlan gráf nem kaphat irányított gráfot) oldjuk meg, hogy az implementációt ne kelljen felüldefiniálni a `GRAPH` osztályban.

### Bejárás

Készítsünk egy feature-t, amely a gráf mélységi bejárását végezi el. A feature egy ágenst kap paraméterben, és ezt az ágenst hívja meg minden csúcsra a bejárás során.

## Megoldás

### `DIGRAPH` osztály

~~~eiffel
class
    DIGRAPH [T -> HASHABLE]

inherit
    HASH_TABLE [attached ARRAYED_SET[T], attached T]
        rename make as init, count as node_count, has as has_node
        export {HASH_TABLE} all --clients should only use the features declared in the DIGRAPH class
            {ANY} node_count, has_node
        end

create init, union

feature
    add_node(node: attached T)
        require
            node_not_present: not has_node(node)
        do
            extend(create{ARRAYED_SET[T]}.make(0), node)
        ensure
            node_present: has_node(node)
            node_count_increased: node_count ~ old node_count + 1
            edge_count_same: edge_count ~ old edge_count
        end

    add_edge(node_from, node_to: attached T)
        require
            node_from_present: has_node(node_from)
            node_to_present: has_node(node_to)
            edge_not_present: not has_edge_between(node_from, node_to)
        do
            definite_item(node_from).extend(node_to)
        ensure
            edge_present: has_edge_between(node_from, node_to)
            edge_count_increased: edge_count > old edge_count --increases by 2 in GRAPH
            node_count_same: node_count ~ old node_count
        end

    has_edge_between(node_from, node_to: attached T): BOOLEAN
        require
            node_from_present: has_node(node_from)
            node_to_present: has_node(node_to)
        do
            Result := definite_item(node_from).has(node_to)
        ensure
            frame: Current ~ old Current
        end

    edge_count: INTEGER
        do
            across Current as i
            from Result := 0
            loop Result := Result + i.item.count
            end
        ensure
            result_positive: Result >= 0
            frame: Current ~ old Current
        end

    depth_first_traverse(root: attached T; visitor: attached PROCEDURE[attached T])
    require
        root_present: has_node(root)
    local
        discovered: attached ARRAYED_SET[attached T]
        visited_count: INTEGER
        to_visit: attached ARRAYED_STACK[attached T]
        v: attached T
    do
        from
            create discovered.make(node_count)
            visited_count := 0
            create to_visit.make(node_count)
            discovered.extend(root); to_visit.extend(root)
        invariant
            node_count >= discovered.count
            discovered.count ~ visited_count + to_visit.count
        until
            to_visit.is_empty
        loop
            v := to_visit.item; to_visit.remove
            visitor(v)
            visited_count := visited_count + 1
            across definite_item(v) as i loop
                if not discovered.has(i.item) then
                    discovered.extend(i.item)
                    to_visit.extend(i.item)
                end
            end
        variant
            node_count - visited_count
        end
    ensure
        frame: Current ~ old Current
    end

feature {NONE}
    union(x,y: like Current)
        do
            init(0)
            copy(x)
            across y as key_values loop
                if not has_node(key_values.key) then
                    add_node(key_values.key)
                end
                across key_values.item as value loop
                    if not has_edge_between(key_values.key, value.item) then
                        add_edge(key_values.key, value.item)
                    end
                end
            end
        ensure
            node_count_greater_equal: node_count >= x.node_count and node_count >= y.node_count
            node_count_at_most_sum: node_count <= x.node_count + y.node_count
            edge_count_greater_equal: edge_count >= x.edge_count and edge_count >= y.edge_count
            edge_count_at_most_sum: edge_count <= x.edge_count + y.edge_count
        end

end
~~~

### `GRAPH` osztály

~~~eiffel
class
    GRAPH [T -> HASHABLE]

inherit
    DIGRAPH [T]
        redefine add_edge
        end

create init, union

feature
    add_edge(node_from, node_to: attached T)
        do
            Precursor(node_from, node_to)
            if node_from /~ node_to then
                Precursor(node_to, node_from)
            end
        ensure then
            edge_present_backward: has_edge_between(node_to, node_from)
        end

end
~~~

### Főprogram, minimális tesztelés

~~~eiffel
local
    g,g2,g3: attached GRAPH[INTEGER]
do
    create g.init(10)
    g.add_node(11); g.add_node(12); g.add_node(13); g.add_node(14)
    g.add_edge(11, 11); g.add_edge(11, 12); g.add_edge(11, 13); g.add_edge(13, 14)
    g.depth_first_traverse(11, agent(v: INTEGER) do print(v); print("%N") end)

    print("%N")

    --g.add_node(11) --Constraint violation
    --g.add_edge(11, 999) --Constraint violation
    --g.add_edge(11, 12) --Constraint violation
    --print(g.has_edge_between(11, 999)) --Constraint violation

    g2 := g
    create g3.init(10)
    g3.add_node(14); g.add_node(15)
    g.add_edge(14, 15)
    create g.union(g2,g3)
    g.depth_first_traverse(11, agent(v: INTEGER) do print(v); print("%N") end)
end
~~~
