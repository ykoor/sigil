void to_json(Value val, int depth) {
    char indent[128];
    for (int i = 0; i < depth * 2; i++)
        indent[i] = ' ';
    indent[depth * 2] = '\0';

    if (is_atom_def(val)) {
        AtomDef *def = as_atom_def(val);
        printf("\"%s\": ", as_symbol(def->name));
        to_json(def->val, depth + 1);
    } else if (is_string(val)) {
        printf("\"%s\"", as_string(val));
    } else if (is_symbol(val)) {
        printf("\"%s\"", as_symbol(val));
    } else if (is_double(val)) {
        printf("%g", decode_double(val));
    } else if (val == VALUE_TRUE) {
        printf("true");
    } else if (val == VALUE_FALSE) {
        printf("false");
    } else if (val == VALUE_NIL) {
        printf("null");
    } else if (is_pair(val)) {
        bool all_defs = true;
        Value cur = val;
        while (!is_nil(cur)) {
            if (!is_obj_entry(first(cur))) {
                all_defs = false;
                break;
            }
            cur = rest(cur);
        }

        if (all_defs) {
            printf("{\n");
            cur = val;
            while (!is_nil(cur)) {
                printf("%s  ", indent);
                to_json(unwrap_entry(first(cur)), depth + 1);
                cur = rest(cur);
                if (!is_nil(cur))
                    printf(",");
                printf("\n");
            }
            printf("%s}", indent);
        } else {
            printf("[\n");
            cur = val;
            while (!is_nil(cur)) {
                printf("%s  ", indent);
                to_json(first(cur), depth + 1);
                cur = rest(cur);
                if (!is_nil(cur))
                    printf(",");
                printf("\n");
            }
            printf("%s]", indent);
        }
    }
}
