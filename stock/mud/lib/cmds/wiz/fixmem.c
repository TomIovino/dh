status _fixmem(string arg) {
    garbage_collection();
    return printf("Memory/Garbage collection complete.\n"),1;
}
