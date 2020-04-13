table! {
    containers (id) {
        id -> Int4,
        name -> Varchar,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

table! {
    items (id) {
        id -> Int4,
        name -> Varchar,
        total -> Int4,
        created_at -> Timestamp,
        updated_at -> Timestamp,
    }
}

allow_tables_to_appear_in_same_query!(containers, items,);
