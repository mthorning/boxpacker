use diesel::pg::PgConnection;
use diesel::prelude::*;
use dotenv::dotenv;
use std::env;

use super::models::{Container, NewContainer};

pub fn establish_connection() -> PgConnection {
    dotenv().ok();

    let database_url = env::var("DATABASE_URL").expect("DATABASE_URL must be set");

    PgConnection::establish(&database_url).expect(&format!("Error connecting to {}", database_url))
}

pub fn add_container<'a>(conn: &PgConnection, name: &'a str) -> Container {
    use super::schema::containers;

    let new_container = NewContainer { name: name };

    diesel::insert_into(containers::table)
        .values(&new_container)
        .get_result(conn)
        .expect("Error saving new post")
}
