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