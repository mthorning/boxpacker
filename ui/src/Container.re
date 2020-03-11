type t = {
  id: int,
  name: string,
};

let decoder = json => {
  Json.Decode.{
    id: json |> field("id", int),
    name: json |> field("name", string),
  };
};

let fetch_all = onData => {
  ignore(
    Js.Promise.(
      Axios.get("/api/containers")
      |> then_(resp => {resp##data |> Json.Decode.array(decoder) |> resolve})
      |> then_(containers => resolve(onData(containers)))
    ),
  );
};