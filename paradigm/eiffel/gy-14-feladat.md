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

--Non-conforming inheritance doesn't work because we rename the creation
--  procedure the HASH_TABLE superclass is trying to use
inherit
	HASH_TABLE [attached ARRAYED_SET[attached T], attached T]
		rename make as init, count as node_count, has_key as has_node
		export
			{HASH_TABLE} all --Don't let clients modfify the contents directly;
			                 --  force them to use e.g. add_node
			{ANY} node_count, has_node
		end

create init, union

feature
    add_node(node: attached T)
    require
        not_exists: not has_node(node)
    do
        put(create {ARRAYED_SET[attached T]}.make(0), node)
    ensure
        created: has_node(node)
        node_count_increased: node_count = old node_count + 1
        --edge_count_same: edge_count = old edge_count
    end

    add_edge(node_from, node_to: attached T)
    require
        edge_not_exists: not has_edge(node_from, node_to)
        endpoints_exist: has_node(node_from) and has_node(node_to)
    do
        check attached item(node_from) as attached_array then
            attached_array.extend(node_to)
        end
    ensure
        created: has_edge(node_from, node_to)
        node_count_same: node_count = old node_count
        --edge_count_increased: edge_count >= old edge_count + 1 --Increases by 2 in undirected graphs
    end

    has_edge(node_from, node_to: attached T): BOOLEAN
    require
        endpoints_exist: has_node(node_from) and has_node(node_to)
    do
        check attached item(node_from) as attached_array then
            Result := attached_array.has(node_to)
        end
    ensure
        frame: Current ~ old Current
    end

feature {NONE}
    union(a, b: attached like Current)
    do
        init(10)
        copy(a)
        --First add all nodes, only then can we add edges
        from
            b.start
        until
            b.off
        loop
            if not has_node(b.key_for_iteration) then
                add_node(b.key_for_iteration)
            end
            b.forth
        end
        from
            b.start
        until
            b.off
        loop
            from
                b.item_for_iteration.start
            until
                b.item_for_iteration.off
            loop
                if not has_edge(b.key_for_iteration, b.item_for_iteration.item) then
                    add_edge(b.key_for_iteration, b.item_for_iteration.item)
                end
                b.item_for_iteration.forth
            end

            b.forth
        end
    ensure
        node_count_min: a.node_count <= node_count and b.node_count <= node_count
        node_count_max: node_count <= a.node_count + b.node_count
        --edge_count_min: a.edge_count <= edge_count and b.edge_count <= edge_count
        --edge_count_max: edge_count <= a.edge_count + b.edge_count
    end

feature
    depth_first_traverse(start_node: attached T; visitor: attached PROCEDURE[attached T])
    require
        valid_start: has_node(start_node)
    local
        discovered: attached ARRAYED_SET[attached T]
        to_visit: attached ARRAYED_STACK[attached T]
        visited_count: INTEGER
        v: attached T
    do
        from
            create discovered.make(0)
            create to_visit.make(0)
            discovered.put(start_node)
            to_visit.put(start_node)
        invariant
            node_count >= discovered.count
            discovered.count = visited_count + to_visit.count
        until
            to_visit.is_empty
        loop
            v := to_visit.item
            to_visit.remove
            visitor(v)
            visited_count := visited_count + 1

            check attached item(v) as attached_array then
                from
                    attached_array.start
                until
                    attached_array.off
                loop
                    if not discovered.has(attached_array.item) then
                        discovered.put(attached_array.item)
                        to_visit.put(attached_array.item)
                    end
                    attached_array.forth
                end
            end
        variant
            node_count - visited_count
        end
    ensure
        frame: Current ~ old Current
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

create init

feature
    add_edge(node_from, node_to: attached T)
    do
        Precursor(node_from, node_to)
        if node_from /~ node_to then
            Precursor(node_to, node_from)
        end
    ensure then
        edge_present_both_ways: has_edge(node_from, node_to) and has_edge(node_to, node_from)
    end

end
~~~

### Főprogram, minimális tesztelés

~~~eiffel
local
    g1, g2, g3: attached DIGRAPH[INTEGER]
    g4: attached GRAPH[INTEGER]
do
    create g1.init(0)
    g1.add_node(1); g1.add_node(2); g1.add_node(3)
    g1.add_edge(1, 2); g1.add_edge(2, 3); g1.add_edge(3, 3)

    --g1.add_node(1) --Contract violation
    --g1.add_edge(1, 2) --Contract violation
    --g1.add_edge(123, 1) --Contract violation
    --print(g1.has_edge(1, 123)) --Contract violation

    create g2.init(0)
    g2.add_node(3); g2.add_node(4); g2.add_node(5)
    g2.add_edge(3, 5); g2.add_edge(3, 4); g2.add_edge(4, 5)

    create g3.union(g1, g2)
    print("Actual output:   ")
    g3.depth_first_traverse(1, agent(v: INTEGER) do print(v); print(" ") end)
    print("%NExpected output: 1 2 3 4 5 %N")

    create g4.init(0)
    g4.add_node(1); g4.add_node(2); g4.add_node(3); g4.add_node(4)
    g4.add_edge(1, 3); g4.add_edge(1, 2); g4.add_edge(2, 4); g4.add_edge(4, 4)

    print("Actual output:   ")
    g4.depth_first_traverse(1, agent(v: INTEGER) do print(v); print(" ") end)
    print("%NExpected output: 1 2 4 3 %N")
end
~~~
