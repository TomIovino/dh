// find_ob.h : generic object locator for wizard commands.
// Bubbs, 10/July/93

#if !defined(FIND_OB)

#define FIND_OB

object find_ob2(string str, object env) {
  object target;
  if (str == "ENV")
    return (env ? environment(env) : 0);
  if (!env) {
    if (str == "me") target = this_player();
    else if (str == "here") target = environment(this_player());
    else if (target = present(str, this_player()));
    else if (target = present(str, environment(this_player())));
    else if (target = _find_player(str, 1));
    else if (target = find_living(str));
    else if (target = find_object(eval_path(str)));
    return target;
    }
  return present(str, env);
  }

object find_ob(string str) {
  object target;
  string *bits;
  int index, size;
  if (!str) return 0;
  bits = explode(str, ":");
  if (!bits || !bits[index]) bits = ({ });
  size = sizeof(bits);
  for(index = 0;
   index < size && target = find_ob2(bits[index], target);
   index++);
  return target;
  }

void help_find() {
  write("An object is described like so:\n"
        "<env>:<obj>\n"
        "eg: bubbs:bag:sword\n"
        "The environment of an object can be got like this:\n"
        "bubbs:ENV\n");
  return;
  }

#endif   // FIND_OB
