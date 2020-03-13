use crate::storage::models::{Container, NewContainer};
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
pub fn get_all_containers(conn: DbConn) -> BpResp {
    check_err(
        Container::get_all(&conn),
        "There was a problem getting all containers from the database.",
    )
}

#[get("/<id>")]
pub fn get_container_by_id(conn: DbConn, id: i32) -> BpResp {
    check_err(
        Container::get_by_id(&conn, id),
        &format!("Cannot get container with id \"{}\" from the database.", id),
    )
}

#[post("/", format = "json", data = "<new_container>")]
pub fn add_container(conn: DbConn, new_container: Json<NewContainer>) -> BpResp {
    check_err(
        Container::add(&conn, new_container.into_inner()),
        "There was a problem adding the container to the database.",
    )
}

#[delete("/<id>")]
pub fn delete_container(conn: DbConn, id: i32) -> BpResp {
    check_err(
        Container::delete(&conn, id),
        "There was a problem deleting the container from the database.",
    )
}
