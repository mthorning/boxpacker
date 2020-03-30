use crate::storage::models::item::{Item, NewItem};
use crate::DbConn;
use diesel::result::QueryResult;
use rocket::response::status::NotFound;
use rocket_contrib::json::{Json, JsonValue};
use serde::Serialize;

type BpResp = Result<JsonValue, NotFound<JsonValue>>;

fn check_err<T: Serialize>(
    db_result: QueryResult<T>,
    message: &str,
) -> Result<JsonValue, NotFound<JsonValue>> {
    match db_result {
        Ok(result) => Ok(json!(result)),
        Err(error) => Err(NotFound(json!({
          "message": message,
          "error": error.to_string()
        }))),
    }
}

#[get("/")]
pub fn get_all_items(conn: DbConn) -> BpResp {
    check_err(
        Item::get_all(&conn),
        "There was a problem getting all items from the database.",
    )
}

#[get("/<id>")]
pub fn get_item_by_id(conn: DbConn, id: i32) -> BpResp {
    check_err(
        Item::get_by_id(&conn, id),
        &format!("Cannot get item with id \"{}\" from the database.", id),
    )
}

#[post("/", format = "json", data = "<new_item>")]
pub fn add_item(conn: DbConn, new_item: Json<NewItem>) -> BpResp {
    check_err(
        Item::add(&conn, new_item.into_inner()),
        "There was a problem adding the item to the database.",
    )
}

#[delete("/<id>")]
pub fn delete_item(conn: DbConn, id: i32) -> BpResp {
    check_err(
        Item::delete(&conn, id),
        "There was a problem deleting the item from the database.",
    )
}

#[patch("/<id>", data = "<name>")]
pub fn change_item_name(conn: DbConn, id: i32, name: String) -> BpResp {
    check_err(
        Item::change_name(&conn, id, name),
        "There was a problem changing the item's name.",
    )
}
