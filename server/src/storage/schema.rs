table! {
    containers (id) {
        id -> Int4,
        name -> Varchar,
    }
}

table! {
    items (id) {
        id -> Int4,
        name -> Varchar,
        total -> Int4,
    }
}

allow_tables_to_appear_in_same_query!(
    containers,
    items,
);
