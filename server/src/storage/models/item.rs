use crate::storage::schema::items;
use crate::storage::schema::items::dsl::*;
use diesel::prelude::*;
use std::time::SystemTime;

#[derive(Insertable, Deserialize)]
#[table_name = "items"]
pub struct NewItem<'a> {
    pub name: &'a str,
    pub total: i32,
}

#[derive(AsChangeset, Deserialize)]
#[table_name = "items"]
pub struct UpdateItem<'a> {
    pub name: Option<&'a str>,
    pub total: Option<i32>,
}

#[derive(Debug, Queryable, Identifiable, Serialize, Deserialize)]
pub struct Item {
    pub id: i32,
    pub name: String,
    pub total: i32,
    pub created_at: SystemTime,
    pub updated_at: SystemTime,
}

impl Item {
    pub fn get_all(conn: &diesel::PgConnection) -> QueryResult<Vec<Item>> {
        items.order(updated_at.desc()).load::<Item>(conn)
    }

    pub fn get_by_id(conn: &diesel::PgConnection, requested_id: i32) -> QueryResult<Item> {
        items.find(requested_id).first::<Item>(conn)
    }

    pub fn add(conn: &diesel::PgConnection, new_item: NewItem) -> QueryResult<Item> {
        match Self::increment_matching_name(conn, &new_item.name, 1) {
            Some(result) => result,
            None => diesel::insert_into(items)
                .values(&new_item)
                .get_result(conn),
        }
    }

    pub fn delete(conn: &diesel::PgConnection, id_to_delete: i32) -> QueryResult<usize> {
        diesel::delete(items.filter(id.eq(id_to_delete))).execute(conn)
    }

    pub fn edit_item(
        conn: &diesel::PgConnection,
        item_id: i32,
        update_item: UpdateItem,
    ) -> QueryResult<Item> {
        match items.find(item_id).first(conn) {
            Err(err) => return Err(err),
            Ok(item) => {
                let patch_item = || -> QueryResult<Item> {
                    diesel::update(&item).set(&update_item).get_result(conn)
                };

                match update_item.name {
                    Some(new_name) => {
                        match Self::increment_matching_name(conn, &new_name, item.total) {
                            Some(result) => {
                                Self::delete(conn, item_id)?;
                                result
                            }
                            None => patch_item(),
                        }
                    }
                    None => patch_item(),
                }
            }
        }
    }

    fn increment_matching_name(
        conn: &diesel::PgConnection,
        received_name: &str,
        received_total: i32,
    ) -> Option<QueryResult<Item>> {
        let stored_items = items
            .filter(name.eq(received_name))
            .load(conn)
            .expect("Unexpected error connecting to DB");
        let num_of_stored = stored_items.len();

        match num_of_stored > 0 {
            true if num_of_stored == 1 => {
                let stored_item = &stored_items[0];
                Some(
                    diesel::update(stored_item)
                        .set(total.eq(stored_item.total + received_total))
                        .get_result(conn),
                )
            }
            true => Some(Err(diesel::result::Error::__Nonexhaustive)),
            false => None,
        }
    }
}
