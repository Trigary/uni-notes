## Osztály vázlat

~~~eiffel
(expanded | deferred | frozen) --Value type? Abstract? Disallow subtyping?
class <CLASS_NAME> [<TYPE_PARAMETER> -> <TYPE_PARAMETER_UPPER_BOUND>]

inherit --Conforming inheritance
  <PARENT_CLASS> rename <old_name> as <new_name> -- e.g. to fix name clashes
         redefine <routine_name> -- define new implementation for a non-deferred routine
         undefine <routine_name> -- drop a routine's implementation (make it deferred)
         select <routine_name>   -- select feature for dynamic dispath in case of diamond inheritance
         export {VISIBILITY} <routine_name> -- change feature's visibility
                {ANOTHER_VISIBILITY} <routine_name>
         end
  <PARENT_CLASS>
  ...

inherit {NONE} --Inherit code without creating subtype relation
  ...

create <routine_name>, <another_routine_name>
create {<CLASS_NAME>} <routine_name> --Only accessible by the class and its subclasses
convert <routine_name>

feature --Public attributes
  <Constant_name>: <TYPE> = <LITERAL>
  <variable>: <TYPE>
  <variable>: <TYPE> assign <variable_setter> --Usage: xyz.variable := ... or xyz.variable_setter(...)

feature --Public functions/procedures (routines)
  (frozen) --Disallow redefinition?
  <procedure> | <procedure>(<param1, param2>: <TYPE12>; <param3>: <TYPE3>) | <function>: <RETURN_TYPE>
    require <precondition: logical expression>
    local <temporal variables>
    (do | once) <routine body> --Once: body is only run once, result is cached (params -> dangerous)
    ensure <postcondition: logical expression>
    rescue <exception handling>
    end

  <routine_name>
    require <...>
    deferred --Abstract routine can only be defined in an abstract class
    ensure <...>

  <redefined_routine_name>
    require else <...> --Default implementation: require else False
    do ... --Previous implementation is accessible via Precursor(...)
    ensure then <...> --Default implementation: ensure then True

  <function_name> alias "binary_infix_operator" (<param>: <TYPE>): <RETURN_TYPE>
    ...

  <function_name> alias "unary_prefix_operator" : <RETURN_TYPE>
    ...

  <function_name> alias "[]" (<any number of parameters>): <RETURN_TYPE> (assign <procedure_name>)
    ...

feature {NONE} --Features only accessible by the current object instance (in this class or a subclass)
  ...

feature {<CLASS_NAME>} --Features which are only accessible by the class and its subclasses
  ...

invariant <logical expression> --Must be true after any creation procedure finishes
end
~~~
