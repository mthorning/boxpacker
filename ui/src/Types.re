module Container = {
  type t = {
    id: int,
    name: string,
  };

  let endpoint = "/api/containers";

  let decode = json => {
    Json.Decode.{
      id: json |> field("id", int),
      name: json |> field("name", string),
    };
  };

  let decoder = json => Json.parseOrRaise(json) |> decode;

  let array_decoder = json =>
    Json.parseOrRaise(json) |> Json.Decode.array(decode);
};

module Item = {
  type t = {
    id: int,
    name: string,
    total: int,
  };

  let endpoint = "/api/items";

  let decode = json => {
    Json.Decode.{
      id: json |> field("id", int),
      name: json |> field("name", string),
      total: json |> field("total", int),
    };
  };

  let decoder = json => Json.parseOrRaise(json) |> decode;

  let array_decoder = json =>
    Json.parseOrRaise(json) |> Json.Decode.array(decode);
};