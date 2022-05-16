status main() {
    object SD;
    SD = find_object("/secure/socketd/socketd");
    map_array(m_indices((mapping)SD->query_sockets()),"socket_close",SD);
    return 1;
}
