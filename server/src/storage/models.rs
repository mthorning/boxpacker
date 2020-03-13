use super::schema::containers;
use super::schema::containers::dsl::*;
use diesel::prelude::*;

#[derive(Insertable, Deserialize)]
#[table_name = "containers"]
pub struct NewContainer<'a> {
    pub name: &'a str,
}

#[derive(Queryable, Serialize, Deserialize)]
pub struct Container {
    pub id: i32,
    pub name: String,
}

impl Container {
    pub fn get_all(conn: &diesel::PgConnection) -> QueryResult<Vec<Container>> {
        containers.order(id.asc()).load::<Container>(conn)
    }

    pub fn get_by_id(conn: &diesel::PgConnection, requested_id: i32) -> QueryResult<Container> {
        containers.find(requested_id).first::<Container>(conn)
    }

    pub fn add(conn: &diesel::PgConnection, new_container: NewContainer) -> QueryResult<Container> {
        diesel::insert_into(containers)
            .values(&new_container)
            .get_result(conn)
    }

    pub fn delete(conn: &diesel::PgConnection, id_to_delete: i32) -> QueryResult<usize> {
        diesel::delete(containers.filter(id.eq(id_to_delete))).execute(conn)
    }
}
