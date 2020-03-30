use crate::storage::schema::items;
use crate::storage::schema::items::dsl::*;
use diesel::prelude::*;

#[derive(Insertable, Deserialize)]
#[table_name = "items"]
pub struct NewItem<'a> {
    pub name: &'a str,
    pub total: i32,
}

#[derive(Queryable, Identifiable, Serialize, Deserialize)]
pub struct Item {
    pub id: i32,
    pub name: String,
    pub total: i32,
}

impl Item {
    pub fn get_all(conn: &diesel::PgConnection) -> QueryResult<Vec<Item>> {
        items.order(id.asc()).load::<Item>(conn)
    }

    pub fn get_by_id(conn: &diesel::PgConnection, requested_id: i32) -> QueryResult<Item> {
        items.find(requested_id).first::<Item>(conn)
    }

    pub fn add(conn: &diesel::PgConnection, new_item: NewItem) -> QueryResult<Item> {
        let items_with_name = items.filter(name.eq(new_item.name)).load::<Item>(conn)?;

        let num_of_items = items_with_name.len();
        match num_of_items > 0 {
            true if num_of_items == 1 => {
                let item = &items_with_name[0];
                diesel::update(item)
                    .set(total.eq(item.total + 1))
                    .get_result(conn)
            }
            true => panic!("More than on item with same name in DB"),
            false => diesel::insert_into(items)
                .values(&new_item)
                .get_result(conn),
        }
    }

    pub fn delete(conn: &diesel::PgConnection, id_to_delete: i32) -> QueryResult<usize> {
        diesel::delete(items.filter(id.eq(id_to_delete))).execute(conn)
    }

    pub fn change_name(
        conn: &diesel::PgConnection,
        id_to_edit: i32,
        new_name: String,
    ) -> QueryResult<Item> {
        let target = items.filter(id.eq(id_to_edit));
        diesel::update(target)
            .set(name.eq(new_name))
            .get_result(conn)
    }
}
