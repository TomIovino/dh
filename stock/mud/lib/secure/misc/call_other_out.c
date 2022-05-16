void call_other_out(mixed obj, string function, int delay) {
    call_out(lambda( ({ 'ob, 'func }), 
            ({ #'call_other, 'ob, 'func }) ), delay, obj, function);
}

