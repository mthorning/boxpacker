#![feature(proc_macro_hygiene, decl_macro)]
#[macro_use]
extern crate diesel;
#[macro_use]
extern crate serde_derive;
#[macro_use]
extern crate rocket;
#[macro_use]
extern crate rocket_contrib;
extern crate dotenv;

use rocket_contrib::database;

pub mod routes;
pub mod storage;

#[database("boxpacker_db")]
pub struct DbConn(diesel::PgConnection);

fn main() {
    use routes::containers::*;
    use routes::items::*;
    rocket::ignite()
        .attach(DbConn::fairing())
        .mount(
            "/api/containers",
            routes![
                get_all_containers,
                get_container_by_id,
                add_container,
                delete_container
            ],
        )
        .mount(
            "/api/items",
            routes![
                get_all_items,
                get_item_by_id,
                add_item,
                delete_item,
                edit_item
            ],
        )
        .launch();
}
