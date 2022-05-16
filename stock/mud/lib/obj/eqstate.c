//////////
// eqstate.h -- Handles Eq state save/restore
//////////                                                 //////////
// Copyright (c) 1997,1998 Andrew T. Graham -- All Rights Reserved //
//////////                                                 //////////
// $ Id: $
//////////
// ChangeLog:
//   $Log: eqstate.c,v $
//   Revision 1.1  1998/03/03 23:58:30  atgraham
//   Initial revision
//
//////////

#include <eqstate.h>

private static closure containerFilter =  lambda( ({ 'ob }),
                                    ({ #'==, 
                                         "container",
                                         ({ #'call_other, 'ob, 
                                                  "query_object_type"
                                         })
                                    }) );


varargs private object _present(string id, object env) {
    if(!id || !stringp(id) || !env || !objectp(env)) {
        return 0;
    }
    return present(id,env);
}

void save_eq() {
    object ob, *containers;
    string *eq_types, id;
    mapping EQ_save;
    int i, j;

    if(!this_object())
        return;

    if(!this_object()->query_attrib("oktoSaveEq"))
        return;

    eq_types = ({
     "floating","helm","amulet","armour","cloak","shield",
     "sleeves","gloves","wrists","ankles","mask","ring",
     "belt","pants","boots" });
    i = sizeof(eq_types);

    EQ_save = ([ ]);
    EQ_save["armor"] = ({ });

    while(i--)
        if(ob = (object) this_object()->query_armour_type(eq_types[i]))
            EQ_save["armor"] += ({ ob->query_IDfile() });

    EQ_save["weapon"] = ([ ]);
    if(ob = (object) this_object()->query_right_weapon())
        EQ_save["weapon"]["right"] = (string) ob->query_IDfile();
    if(ob = (object) this_object()->query_left_weapon()) {
        EQ_save["weapon"]["left"] = (string) ob->query_IDfile();
        if(EQ_save["weapon"]["left"] == EQ_save["weapon"]["right"])
            EQ_save["weapon"]["left"] += " 2";
    }    
    this_object()->set_attrib("EQrestore",EQ_save);
    (query_verb() == "quit") && tell_object(this_object(),"Your equipment status has been saved.\n");

    containers = filter_array(all_inventory(this_object()),containerFilter);
    if(i = sizeof(containers)) {
        EQ_save["container"] = ([ ]);
        while(i--) {
            id = (string) containers[i]->query_IDfile();
            if(!EQ_save["container"][id])
                EQ_save["container"][id] = ({ });
            EQ_save["container"][id] += ({ (string *) containers[i]->query_container_contents() });
        }
        (query_verb() == "quit") && tell_object(this_object(),"Your container status has been saved.\n");
    }
}

void restore_eq() {
    mapping EQ_save;

    if(!this_object())
        return;

    EQ_save = (mapping) this_object()->query_attrib("EQrestore");
    this_object()->unset_attrib("EQrestore");
    this_object()->set_tmp_attrib("oktoSaveEq",1);
    if(!EQ_save || !mappingp(EQ_save)) {
        tell_object(this_object(),
      "Your equipment status could not be restored.\n");
        return;
    }

    this_object()->set_tmp_attrib("saveDisable",1);
    call_out("restore_eq_containers",0,EQ_save);
    call_out("restore_eq_weapon",0,EQ_save);
    call_out("restore_eq_armor",0,EQ_save);
    call_out("force_save_me",8);
}    


void force_save_me() {
    this_object()->unset_tmp_attrib("saveDisable");
    this_object()->save_me(0);
}

void restore_one_armor(string what) {
    if(what && _present(what,this_object()))
        call_out(#'command,0,sprintf("*wear %s",what),this_object());
}

void restore_eq_armor(mapping EQ_save) {
    int i;
    
    i = sizeof(EQ_save["armor"]);
    if(i) {
        while(i--)
            call_out("restore_one_armor",0,EQ_save["armor"][i]);
        tell_object(this_object(),"Your armor status is being restored.\n");
    }

}

void restore_eq_weapon(mapping EQ_save) {
    if(EQ_save["weapon"] && mappingp(EQ_save["weapon"])) {
       if(EQ_save["weapon"]["right"] && _present(EQ_save["weapon"]["right"],this_object()))
//            command(sprintf("*wield %s in right hand",EQ_save["weapon"]["right"]),this_object());
        command(sprintf("*wield %s",EQ_save["weapon"]["right"]),this_object());
       if(EQ_save["weapon"]["left"] && _present(EQ_save["weapon"]["left"],this_object()))
//            command(sprintf("*wield %s in left hand",EQ_save["weapon"]["left"]),this_object());
         command(sprintf("*wield %s",EQ_save["weapon"]["left"]),this_object());
       if(EQ_save["weapon"]["right"] || EQ_save["weapon"]["left"]) {
            tell_object(this_object(),"Your weapon status has been restored.\n");
        }
    }

}

void restore_eq_containers_put(string item, string bag) {
    if(_present(item,this_object())) 
        call_out(#'command,0,sprintf("*put %s in %s",item,bag),this_object());
}

void restore_eq_containers(mapping EQ_save) {
    object ob;
    string *bags, bag, item;
    int i, j, k, l;

    if(EQ_save["container"]
    && i = sizeof(bags = m_indices(EQ_save["container"]))) {
        while(i--) {
            if(EQ_save["container"][bags[i]]
            && j = sizeof(EQ_save["container"][bags[i]])) {
                while(j--) {
                    if((ob = _present((bag = bags[i] + " " + (j+1)),this_object()))
                    && EQ_save["container"][bags[i]][j]
                    && k = sizeof(EQ_save["container"][bags[i]][j])) {
                        if(ob && (int) ob->query_closed() == 1)
                            command(sprintf("*open %s",bag),this_object());
                        l = 0;
                        while(k--) {
                            call_out("restore_eq_containers_put",0,EQ_save["container"][bags[i]][j][l],bag);
                            l++;
                        }
                    }
                }
            }
        }
        tell_object(this_object(),"Your container status is being restored.\n");
    }
}

